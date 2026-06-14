#pragma once

#include "FRegistryKey.h"

#include <utility>
#include <concepts>
#include <type_traits>
#include <optional>
#include <string>
#include <format>
#include <initializer_list>


#include <QtTypes>
#include <QHash>
#include <QString>
#include <QReadWriteLock>
#include <QtAlgorithms>
#include <QDataStream>
#include <QDebug>

namespace IRegistry {

	template<typename T>
	concept DescriptorType = requires(const T t, QDataStream& stream, FRegistryKey::FRegistryKey mutableKey) {
		{ stream << t } -> std::same_as<QDataStream&>;
		{ stream >> mutableKey } -> std::same_as<QDataStream&>;
	};

	template<DescriptorType T>
	class IRegistry {
		inline static QHash<FRegistryKey::FRegistryKey, T> registry;
		inline static QReadWriteLock lock;

		template<typename U>
		struct LoadItem {
			QString ID;
			U value;

			LoadItem(QString ID, U value) : ID(ID), value(value) {}
		};

		static const bool existsSource(const QString& source) {
			for (auto it = registry.keyBegin(); it != registry.keyEnd(); it++)
				if (it->source() == source) return true;

			return false;
		}

	public:
		IRegistry() = delete;

		// Individual Add / Remove
		static void add(const FRegistryKey::FRegistryKey& newKey, const T& newValue, const bool overrideItem = false) {
			QWriteLocker locker(&lock);

			if (overrideItem)
				registry.insert(newKey, newValue);
			else
				registry.tryInsert(newKey, newValue);
		}
		static const bool remove(const FRegistryKey::FRegistryKey& key) {
			QWriteLocker locker(&lock);
			return registry.remove(key);
		}

		// Bulk Load / Unload
		static bool load(const QString& source, std::initializer_list<LoadItem<T>> newItemList, const bool overrideItem = false) {
			QWriteLocker locker(&lock);
			if (existsSource(source)) return false;

			if (overrideItem) {
				for (const auto& item : newItemList)
					registry.insert(FRegistryKey::FRegistryKey{ source, item.ID }, item.value);
			}
			else {
				for (const auto& item : newItemList)
					registry.tryInsert(FRegistryKey::FRegistryKey{ source, item.ID }, item.value);
			}
			return true;
		}
		static bool unload(const QString& source) {
			QWriteLocker locker(&lock);
			if (!existsSource(source)) return false;

			qsizetype removedCount = registry.removeIf([&source](const FRegistryKey::FRegistryKey& item) {
				return item.source() == source;
			});
			return removedCount > 0;
		}

		// Status Checks
		static const bool exists(const FRegistryKey::FRegistryKey& item) {
			QReadLocker locker(&lock);
			return registry.contains(item);
		}

		// Accessors
		static const std::optional<T> at(const FRegistryKey::FRegistryKey& key) {
			//qDebug() << key.source() << key.id();
			QReadLocker locker(&lock);
			auto it = registry.find(key);
			if (it == registry.end())
				return std::nullopt;

			return *it;
		}

		// Other
		static void clear() {
			QWriteLocker locker(&lock);
			registry.clear();
		}
		static qsizetype size() {
			QReadLocker locker(&lock);
			return registry.size();
		}
	};
}