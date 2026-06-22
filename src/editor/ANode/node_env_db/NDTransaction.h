#pragma once

namespace NDTransaction {

	class NDTransaction {
        QSqlDatabase& db;
        bool committed = false;
        bool hasStarted = false;

    public:
        NDTransaction(QSqlDatabase& database) : db(database) {
            hasStarted = db.transaction();
            if (!hasStarted) {
                qWarning() << "Failed to start database transaction.";
            }
        }
        ~NDTransaction() {
            if (hasStarted && !committed) {
                qCritical() << "Transaction auto-rolled back due to unexpected exit.";
                db.rollback();
            }
        }
        bool started() const { return hasStarted; }
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