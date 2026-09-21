#pragma once

#include "NDPool.h"

namespace NDParser {

    inline constexpr qint64 MAX_MANIFEST_SIZE = 64 * 1024; //64KB

	inline std::optional<QJsonObject> parseManifest(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file";
            return std::nullopt;
        }
        if (file.size() > MAX_MANIFEST_SIZE) {
            qWarning() << "Aborting: Manifest file is suspiciously large:" << file.size() << "bytes";
            return std::nullopt;
        }
        if (file.size() == 0) {
            qWarning() << "Aborting: Manifest file is zero-byte file";
            return std::nullopt;
        }

        QByteArray byteArray = file.readAll();

        QJsonParseError parseError;
		const QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parsing failed:" << parseError.errorString();
            return std::nullopt;
        }
        if (!doc.isObject()) return std::nullopt;

        return doc.object();
	}



    template<typename T>
    inline std::optional<T> parse(const QJsonObject& obj, const QString& key, bool isOptionalValid = false) { return std::nullopt; }

    template<>
    inline std::optional<muuid::uuid> parse<muuid::uuid>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);
        if (jsonValue.isUndefined() || !jsonValue.isString()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a string.";
            }
            return std::nullopt;
        }

        std::string rawStr = jsonValue.toString().toStdString();

        auto optID = muuid::uuid::from_chars(rawStr);
        if (!optID) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed: String is not a valid UUID:" << rawStr.c_str();
            }
            return std::nullopt;
        }
        return optID;
    }

    template<>
    inline std::optional<QList<muuid::uuid>> parse<QList<muuid::uuid>>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {
        QList<muuid::uuid> arr;
        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isArray()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a array.";
            }
            return std::nullopt;
        }
        QJsonArray jsonArray = jsonValue.toArray();

        for (const QJsonValue& val : jsonArray) {

            if (val.isUndefined() || !val.isString()) {
                if (isOptionalValid) {
                    qWarning() << "Parsing failed:" << key << "missing or is not a string.";
                }
                continue;
            }

            std::string rawStr = val.toString().toStdString();

            auto optID = muuid::uuid::from_chars(rawStr);
            if (!optID) {
                if (isOptionalValid) {
                    qWarning() << "Parsing failed: String is not a valid UUID:" << rawStr.c_str();
                }
                continue;
            }
            arr.append(*optID);
        }
        return arr;
    }

    template<>
    inline std::optional<QString> parse<QString>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isString()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a string.";
            }
            return std::nullopt;
        }
        return jsonValue.toString();
    }

    template<>
    inline std::optional<short> parse<short>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isDouble()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a double.";
            }
            return std::nullopt;
        }

        int rawInt = jsonValue.toInt();

        if (rawInt < std::numeric_limits<short>::min() || rawInt > std::numeric_limits<short>::max()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed: Value" << rawInt << "overflows the boundaries of a 'short'.";
            }
            return std::nullopt;
        }
        return static_cast<short>(rawInt);
    }

    template<>
    inline std::optional<int> parse<int>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isDouble()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a double.";
            }
            return std::nullopt;
        }

        return jsonValue.toInt();
    }

    template<>
    inline std::optional<double> parse<double>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isDouble()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a double.";
            }
            return std::nullopt;
        }
        return jsonValue.toDouble();
    }

    template<>
    inline std::optional<bool> parse<bool>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isBool()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a bool.";
            }
            return std::nullopt;
        }
        return jsonValue.toBool();
    }

    template<>
    inline std::optional<QColor> parse<QColor>(const QJsonObject& obj, const QString& key, bool isOptionalValid) {

        QJsonValue jsonValue = obj.value(key);

        if (jsonValue.isUndefined() || !jsonValue.isString()) {
            if (isOptionalValid) {
                qWarning() << "Parsing failed:" << key << "missing or is not a string.";
            }
            return std::nullopt;
        }
        QString colorStr = jsonValue.toString();

        QColor color = QColor::fromString(colorStr);
        if (!color.isValid()) {
            qWarning() << "Parsing failed: '" << colorStr << "' is not a recognized color format.";
            return std::nullopt;
        }
        return color;
    }
}