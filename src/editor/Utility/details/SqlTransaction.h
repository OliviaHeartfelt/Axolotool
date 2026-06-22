#pragma once

namespace SqlTransaction {

	class SqlTransaction {
        QSqlDatabase& db;
        bool committed = false;
        bool hasStarted = false;

    public:
        SqlTransaction(QSqlDatabase& database) : db(database) {
            hasStarted = db.transaction();
            if (!hasStarted) {
                qWarning() << "Failed to start database transaction.";
            }
        }
        ~SqlTransaction() {
            if (hasStarted && !committed) {
                qCritical() << "Transaction auto-rolled back due to unexpected exit.";
                db.rollback();
            }
        }

        bool started() const { return hasStarted; }

        bool rollbackIf(const bool useRollback, const QString& errorMessage = "") {
            if (useRollback && hasStarted && !committed) {
                if (!errorMessage.isEmpty()) {
                    qCritical() << errorMessage;
                }
                db.rollback();
                committed = true;
            }
            return useRollback;
        }
        bool commit() {
            if (!hasStarted || committed) return false;

            if (!db.commit()) {
                qCritical() << "Failed to commit database transaction.";
                db.rollback();
                committed = true;
                return false;
            }

            committed = true;
            return true;
        }
	};
}