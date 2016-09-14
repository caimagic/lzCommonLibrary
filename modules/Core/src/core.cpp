#include <direct.h>
#include <io.h>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include "core.h"

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
	const std::string path_to_log_file = ".//log//";
#else
	const std::string path_to_log_file = "/tmp/";
#endif

std::unique_ptr<g3::LogWorker> worker;
std::unique_ptr<g3::FileSinkHandle> handle;

LZ_EXPORTS_API lzBool lz_InitializeLib(Core_Lib_Log_Mode_Type mode)
{
	std::string log_prefix = "log";

	if (_access(path_to_log_file.c_str(), 0) == -1)
	{
		_mkdir(path_to_log_file.c_str());
	}

	if (mode&Core_Lib_Log_Mode_Type::LEVEL_DEBUG)
	{
		g3::only_change_at_initialization::setLogLevel(DEBUG, true);
	}
	else
	{
		g3::only_change_at_initialization::setLogLevel(DEBUG, false);
	}

	if (mode&Core_Lib_Log_Mode_Type::LEVEL_INFO)
	{
		g3::only_change_at_initialization::setLogLevel(INFO, true);
	}
	else
	{
		g3::only_change_at_initialization::setLogLevel(INFO, false);
	}

	if (mode&Core_Lib_Log_Mode_Type::LEVEL_WARING)
	{
		g3::only_change_at_initialization::setLogLevel(WARNING, true);
	}
	else
	{
		g3::only_change_at_initialization::setLogLevel(WARNING, false);
	}

	worker = g3::LogWorker::createLogWorker();
	handle = worker->addDefaultLogger(log_prefix, path_to_log_file);
	g3::initializeLogging(worker.get());

	return true;
}

LZ_EXPORTS_API lzBool lz_ReleaseLib()
{
	worker = nullptr;
	handle = nullptr;
	return true;
}