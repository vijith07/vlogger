#pragma once


#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include <format>

class Vlogger
{
public:
	enum LogPriority
	{
		TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
	};
private:
	LogPriority priority = DebugPriority;
	std::mutex logger_mutex;

	const char* filepath = 0;
	std::ofstream file;

public:

	static void SetPriority(LogPriority newPriority) {
		get_instance().priority = newPriority;
	}

	static LogPriority GetPriority()
	{
		return get_instance().priority;
	}

	static bool EnableFileOutput()
	{
		Vlogger& logger_instance = get_instance();
		logger_instance.filepath = "log.txt";
		return logger_instance.enableFileOutput();
	}

	static bool EnableFileOutput(const char* new_filepath)
	{
		Vlogger& logger_instance = get_instance();
		logger_instance.filepath = new_filepath;
		return logger_instance.enableFileOutput();
	}

	static const char* GetFilepath()
	{
		return get_instance().filepath;
	}

	static bool IsFileOutputEnabled()
	{
		return get_instance().file.is_open();
	}


	static void Trace(const char* message)
	{
		get_instance().log("[Trace]    ", TracePriority, message);
	}

	static void Debug(const char* message)
	{
		get_instance().log("[Debug]    ", DebugPriority, message);
	}

	static void Info(const char* message)
	{
		get_instance().log("[Info]     ", InfoPriority, message);
	}

	static void Warn(const char* message)
	{
		get_instance().log("[Warn]     ", WarnPriority, message);
	}

	static void Error(const char* message)
	{
		get_instance().log("[Error]    ", ErrorPriority, message);
	}

	static void Critical(const char* message)
	{
		get_instance().log("[Critical]     ", CriticalPriority, message);
	}

private:
	Vlogger() {}

	Vlogger(const Vlogger&) = delete;
	Vlogger& operator= (const Vlogger&) = delete;

	~Vlogger()
	{
		freeFile();
	}

	static Vlogger& get_instance()
	{
		static Vlogger instance;
		return instance;
	}

	void log(const char* message_priority_str, LogPriority message_priority, const char* message)
	{
		if (priority <= message_priority)
		{

			auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
			std::string timestamp = std::format("{:%Y-%m-%d %X}", time);

			std::scoped_lock lock(logger_mutex);
			std::cout << timestamp << " ";
			std::cout << message_priority_str << " ";
			std::cout << message << " ";
			std::cout << __LINE__ << " ";
			std::cout << __FILE__ << " ";
			std::cout << std::endl;

			if (file.is_open())
			{
				file << timestamp << " ";
				file << message_priority_str << " ";
				file << message << " ";
				file << std::endl;
			}
		}
	}


	bool enableFileOutput()
	{
		freeFile();
		file.open(filepath);
		if (file.is_open())
		{
			return true;
		}
		return false;
	}

	void freeFile()
	{
		if (file.is_open())
		{
			file.close();
		}
	}

};

