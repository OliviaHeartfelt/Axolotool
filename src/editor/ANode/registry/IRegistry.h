#pragma once

#include <utility>
#include <concepts>
#include <optional>

#include <QtTypes>
#include <QList>
#include <QString>
#include <QReadWriteLock>
#include <QtAlgorithms>
#include <QDataStream>

namespace IRegistry {
	struct FRegistryKey {
		QString source;
		QString ID;

		bool operator==(const FRegistryKey& other) const {
			return source == other.source && ID == other.ID;
		}
	};
	inline QDataStream& operator<<(QDataStream& out, const FRegistryKey& data) {
		out << data.source << data.ID;
		return out;
	}
	inline QDataStream& operator>>(QDataStream& in, FRegistryKey& data) {
		in >> data.source >> data.ID;
		return in;
	}

	template<typename T>
	concept DescriptorType = requires(const T t, const T u, QDataStream& stream, FRegistryKey mutableKey) {
		{ t.key() } -> std::same_as<FRegistryKey>;
		{ t == u }  -> std::same_as<bool>;
		{ stream << t.key() } -> std::same_as<QDataStream&>;
		{ stream >> mutableKey} -> std::same_as<QDataStream&>;
	};

	template<DescriptorType T>
	class IRegistry {
		inline static QList<T> registry;
		inline static QReadWriteLock lock;

		static const bool existsSource(const QString& source) {
			for (const auto& flow : registry)
				if (flow.key().source == source) return true;

			return false;
		}

	public:
		IRegistry() = delete;

		// Individual Add / Remove
		static bool add(const T& newItem) {
			QWriteLocker locker(&lock);
			for (const auto& item : registry)
				if (item == newItem) return false;

			registry.append(newItem);
			return true;
		}
		static bool remove(const qsizetype index) {
			QWriteLocker locker(&lock);
			if (index < 0 || index >= registry.size()) return false;

			registry.removeAt(index);
			return true;
		}
		static const bool remove(const QString& source, const QString& id) {
			QWriteLocker locker(&lock);
			qsizetype removed = registry.removeIf([&source, &id](const T& item) {
				return item.key().source == source && item.key().ID == id;
			});
			return removed > 0;
		}

		// Bulk Load / Unload
		static bool load(const QString& source, const QList<T>& newItemList, const bool continueOnSourceNotValid = true) {
			QWriteLocker locker(&lock);
			if (existsSource(source)) return false;

			for (const auto& item : newItemList) {
				if (item.key().source != source) {
					if (continueOnSourceNotValid)
						continue;
					else
						return false;
				}
				registry.append(item);
			}
			return true;
		}
		static bool unload(const QString& source) {
			QWriteLocker locker(&lock);
			if (!existsSource(source)) return false;

			qsizetype removedCount = registry.removeIf([&source](const T& item) {
				return item.key().source == source;
				});
			return removedCount > 0;
		}

		// Status Checks
		static bool exists(const qsizetype index) {
			QReadLocker locker(&lock);
			return (index >= 0 && index < registry.size());
		}
		static const bool exists(const T& item) {
			QReadLocker locker(&lock);
			for (const auto& r : registry)
				if (r == item) return true;

			return false;
		}

		// Accessors
		static const std::optional<T> at(const qsizetype index) {
			QReadLocker locker(&lock);
			if (index < 0 || index >= registry.size()) 
				return std::nullopt;

			return registry.at(index);
		}
		static const std::optional<T> at(const FRegistryKey& key) {
			QReadLocker locker(&lock);

			for (const auto& item : registry)
				if (item.key() == key) return item;

			return std::nullopt;
		}


		// Other
		static void clear() {
			QWriteLocker locker(&lock);
			while (registry.size() > 0)
				registry.removeLast();
		}
		static qsizetype size() {
			QReadLocker locker(&lock);
			return registry.size();
		}
	};
}