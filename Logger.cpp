#include "Logger.hpp"
#include <iostream>

Logger::Logger() {
	Logger::CURRENTLEVEL = Logger::Severity::LOG_INFO;
}

Logger::~Logger() {
	//Close Any files (if we end up logging to files or syslog)
}

const std::string Logger::SeverityLevelsStrings[8] = {
	"EMER","AERT","CRIT","ERRO","WARN","NOTI","INFO","DEBU"
};

void Logger::Log(Logger::Severity level, std::string message) {
	if (level <= Logger::CURRENTLEVEL) {
		if (level <= Logger::LOG_ERR) {
			std::cerr << SeverityLevelsStrings[level] << " : " << message << std::endl;
		} else {
			std::cout << SeverityLevelsStrings[level] << " : " << message << std::endl;
		}
	}
}
Logger::Severity Logger::getLevel() {
	return Logger::CURRENTLEVEL;
}
void Logger::setLevel(Logger::Severity newLevel) {
	Logger::CURRENTLEVEL = newLevel;
	Log(Logger::LOG_WARNING, "Logging Level Changed to: " + SeverityLevelsStrings[newLevel]);
}