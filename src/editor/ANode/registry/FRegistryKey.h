#pragma once

namespace FRegistryKey {

	class FRegistryKey {
		QString privateSource = "";
		QString privateID = "";

	public:
		FRegistryKey() {}
		FRegistryKey(const QString& source, const QString& ID) : privateSource(source), privateID(ID) {}

		QString source() const { return privateSource; }
		void source(const QString& newSource) { privateSource = newSource; }
		QString id() const { return privateID; }
		void id(const QString& newID) { privateID = newID; }

		const std::string debug() const { return std::format("{{ source: {}, ID: {} }}", privateSource.toStdString(), privateID.toStdString()); }

		bool operator==(const FRegistryKey& other) const {
			return privateSource == other.privateSource && privateID == other.privateID;
		}
		friend QDataStream& operator<<(QDataStream& out, const FRegistryKey& data) { return out << data.privateSource << data.privateID; }
		friend QDataStream& operator>>(QDataStream& in, FRegistryKey& data) { return in >> data.privateSource >> data.privateID; }
		friend size_t qHash(const FRegistryKey& key, size_t seed = 0) noexcept {
			return qHashMulti(seed, key.privateSource, key.privateID);
		}
	};
}