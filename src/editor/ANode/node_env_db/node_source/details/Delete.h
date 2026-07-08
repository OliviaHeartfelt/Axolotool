#pragma once

#include "../../../../Utility/Utility.h"

namespace NDNodeSourceDetails::Delete {

    inline bool removeNodeSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node_source WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeNodeContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node_contributor WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeNodeType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node_type WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool removeNodeData(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare("DELETE FROM node_data WHERE id = :id;");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qWarning() << "Failed to remove node data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}