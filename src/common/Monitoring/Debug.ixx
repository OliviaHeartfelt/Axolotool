module;

#include <cstdint>
#include <string>
#include <source_location>
#include <functional>
#include <concepts>
#include <utility>

#include <QDebug>
#include <QString>

export module Debug;



void DebugingHelper(const std::string& label, const std::string& message, const std::source_location& location = std::source_location::current()) {
	const std::string path = location.file_name();
	const size_t pos = path.find_last_of("/\\");
	const std::string fileName = (pos == std::string::npos) ? path : path.substr(pos + 1);

	qDebug().noquote().nospace() << "> "
		<< QString::fromStdString(label) << " > "
		<< "Loc: " << fileName.substr(fileName.find_last_of("/\\") + 1) << ":" << location.line() << ". "
		<< "Message: " << QString::fromStdString(message) << ". ";
}

export namespace Debug {

	template<typename T>
	[[nodiscard]] T Debug(T returnValue, const std::string& label = "", const std::string& message = "", const std::source_location& location = std::source_location::current()) {
		DebugingHelper(label, message, location);
		return returnValue;
	}
	[[nodiscard]] void Debug(const std::string& label = "", const std::string& message = "", const std::source_location& location = std::source_location::current()) {
		DebugingHelper(label, message, location);
	}
}