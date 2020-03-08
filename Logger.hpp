#pragma once
#include <string>

class Logger {
public:
	Logger();
	~Logger();
	static const std::string SeverityLevelsStrings[8];
	enum Severity {
		LOG_EMERG,
		LOG_ALERT,
		LOG_CRIT,
		LOG_ERR,
		LOG_WARNING,
		LOG_NOTICE,
		LOG_INFO,
		LOG_DEBUG,
	};
	void Log(Severity level, std::string message);
	void setLevel(Severity newLevel);
	Severity getLevel();
private:
	Severity CURRENTLEVEL;
};