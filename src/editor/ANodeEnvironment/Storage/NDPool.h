#pragma once

namespace NDPool {

    class DatabasePool {
        struct Connection {
            QSqlDatabase db;
            std::atomic<bool> inUse{ false };
        };

        QString connectionBaseName;
        QString databasePath;
        std::vector<std::unique_ptr<Connection>> connections;
        std::mutex poolMutex;
        std::condition_variable poolCv;

    public:
        DatabasePool(const QString& path, const QString& baseName, int size = 4) : databasePath(path), connectionBaseName(baseName) {
            for (int i = 0; i < size; ++i) {
                auto conn = std::make_unique<Connection>();
                if (!conn) continue;

                QString connName = baseName + QString::number(i);
                conn->db = QSqlDatabase::addDatabase("QSQLITE", connName);
                conn->db.setDatabaseName(path);
                if (conn->db.open()) {
                    QSqlQuery query(conn->db);
                    query.exec("PRAGMA busy_timeout = 5000;");
                    query.exec("PRAGMA journal_mode = WAL;");
                    query.exec("PRAGMA synchronous = NORMAL;");
                    query.exec("PRAGMA foreign_keys = ON;");
                }
                connections.push_back(std::move(conn));
            }
        }

        ~DatabasePool() {
            for (auto& conn : connections) {

                QString name = conn->db.connectionName();
                conn->db.close();
                QSqlDatabase::removeDatabase(name);
            }
        }

        void reconnect(Connection& conn) {
            conn.db.setDatabaseName(databasePath);
            conn.db.open();
        }
        void grow(int additionalConnections) {
            std::unique_lock lock(poolMutex);
            for (int i = connections.size(); i < connections.size() + additionalConnections; ++i) {

                auto conn = std::make_unique<Connection>();
                conn->db = QSqlDatabase::addDatabase("QSQLITE", connectionBaseName + QString::number(i));
                conn->db.setDatabaseName(databasePath);
                if (conn->db.open()) {
                    QSqlQuery query(conn->db);
                    query.exec("PRAGMA busy_timeout = 5000;");
                    query.exec("PRAGMA journal_mode = WAL;");
                    query.exec("PRAGMA synchronous = NORMAL;");
                    query.exec("PRAGMA foreign_keys = ON;");
                }
                connections.push_back(std::move(conn));
            }
        }
        QString path() const { return databasePath; }

        class Lease {
            Connection* conn;
            DatabasePool* pool;
        public:
            Lease(Connection* c, DatabasePool* p) : conn(c), pool(p) {}
            ~Lease() {
                if (conn) {
                    conn->inUse.store(false, std::memory_order_release);
                }
                if (pool) {
                    pool->poolCv.notify_one();
                }
            }
            QSqlDatabase& db() { return conn->db; }

            Lease(const Lease&) = delete;
            Lease& operator=(const Lease&) = delete;
            Lease(Lease&& other) noexcept : conn(other.conn), pool(other.pool) {
                other.conn = nullptr;
                other.pool = nullptr;
            }
        };

        Lease acquire() {
            std::unique_lock lock(poolMutex);
            poolCv.wait(lock, [this] {
                return std::any_of(connections.begin(), connections.end(), [](const auto& conn) {
                    return !conn->inUse.load(std::memory_order_acquire);
                });
            });

            for (auto& conn : connections) {
                bool expected = false;
                if (conn->inUse.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
                    return Lease(conn.get(), this);
                }
            }
            std::unreachable();
        }
    };
}