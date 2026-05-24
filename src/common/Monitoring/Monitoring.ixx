module;

#include <cstdint>
#include <string>
#include <sstream>
#include <source_location>
#include <concepts>
#include <utility>

#include <QDebug>
#include <QString>

export module Monitoring;

template <typename P>
concept IsLogPair = requires(P p) {
	{ p.first } -> std::convertible_to<std::string>;
};

template <typename... Args>
concept IsLogPack = (IsLogPair<Args> && ...);

export namespace Monitoring {
	template<typename... Args>
		requires IsLogPack<Args...>
	void monitoringHelper(const std::string& label, const std::string& message, Args&&... args /*const std::source_location& location = ""*/) {
		std::stringstream ss;
		((ss << args.first << ": " << args.second << "; "), ...);

		std::string _message = message + (message.empty() ? "> " : " ");
		/*
		qDebug().noquote() << QString::fromStdString(label) << "> "
			<< "Loc: " << location.file_name() << ":" << location.line() << ". "
			<< "Message: " << QString::fromStdString(_message) << ". "
			<< "Values: { " << QString::fromStdString(ss.str()) << " }. ";
			*/
	}

	template<typename T, typename... Args>
	requires std::is_enum_v<T> && IsLogPack<Args...>
	[[nodiscard]] T log(T logValue, const std::string& label, const std::string& message, Args&&... args /*const std::source_location& location = std::source_location::current()*/) {
		
		monitoringHelper(label, message, std::forward<decltype(args)>(args)...);
		return logValue;
	}
}