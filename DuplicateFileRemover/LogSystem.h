#pragma once
#include <string>
#include <fstream>

class LogSystem
{
public:
	~LogSystem();
	static LogSystem* get_instance();
	void debug_out(const char* prefix_str, const char* format, ...);
	void wchar_debug_out(const wchar_t* prefix_str, const wchar_t* pszFormat, ...);

	void debug_out_memory(void* memory_addr, std::size_t length_in_bytes);
	void wchar_debug_out_memory(void* memory_addr, std::size_t length_in_bytes);

	void debug_out_to_file(const char* prefix_str, const char* format, ...);
	void wchar_debug_out_to_file(const wchar_t* prefix_str, const wchar_t* format, ...);

	void dump_memory_to_file(void* memory_addr, std::size_t length_in_bytes);
	void wchar_dump_memory_to_file(void* memory_addr, std::size_t length_in_bytes);

	void set_log_prefix(std::string prefix_to_set);
	void set_log_wprefix(std::wstring wprefix_to_set);

	void is_output_to_stdout(bool input_flag);
private:
	class GCHelper
	{
	public:
		~GCHelper()
		{
			if (_log_instance != nullptr) {
				delete _log_instance;
				_log_instance = nullptr;
			}
		}
		static GCHelper auto_gc;
	};
	bool dump_memory(unsigned char* lpBytes, std::size_t nCount);
	bool wdump_memory(unsigned char* lpBytes, std::size_t nCount);
	bool dump_memory_file(unsigned char* lpBytes, std::size_t nCount);
	bool wdump_memory_file(unsigned char* lpBytes, std::size_t nCount);
	void debug_out_hex(unsigned char* lpBytes, std::size_t dwMemorySize);
	void wc_debug_out_hex(unsigned char* lpBytes, std::size_t dwMemorySize);
	void debug_out_hex_file(unsigned char* lpBytes, std::size_t dwMemorySize);
	void wc_debug_out_hex_file(unsigned char* lpBytes, std::size_t dwMemorySize);
	LogSystem();
	static LogSystem* _log_instance;
	bool _is_output_to_stdout;		// will the log output to std::out
	std::string _prefix;
	std::string _sub_prefix;
	std::wstring _wprefix;
	std::wstring _sub_wprefix;
	std::wstring _wchar_prefix;
	std::ofstream _log_file_stream;
	std::wofstream _log_file_wstream;
};

#ifndef __FILENAME__
#define __FILENAME__ \
    (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#ifndef __FILENAMEW__
#define __FILENAMEW__ \
    (wcsrchr(__FILEW__, L'\\') ? wcsrchr(__FILEW__, L'\\') + 1 : __FILEW__)
#endif

#ifdef USE_DBG
#define OUTPUT_TO_STDOUT(flag_in)		LogSystem::get_instance()->is_output_to_stdout( flag_in );
#define DBGOUT(fmt, ...)				LogSystem::get_instance()->debug_out("", "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define W_DBGOUT(fmt, ...)				LogSystem::get_instance()->wchar_debug_out(L"", L"[%s][%s][%d]: " fmt, __FILENAMEW__, __FUNCTIONW__, __LINE__, ##__VA_ARGS__);
#define DBGOUTHEX(addr, len)			LogSystem::get_instance()->debug_out_memory(addr, len);
#define W_DBGOUTHEX(addr, len)			LogSystem::get_instance()->wchar_debug_out_memory(addr, len);
#define LOG_TO_FILE(fmt, ...)			LogSystem::get_instance()->debug_out_to_file("", "[%s][%s][%d]: " fmt, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define WLOG_TO_FILE(fmt, ...)			LogSystem::get_instance()->wchar_debug_out_to_file(L"", L"[%s][%s][%d]: " fmt, __FILENAMEW__, __FUNCTIONW__, __LINE__, ##__VA_ARGS__);
#define LOG_TO_FILE_HEX(addr, len)		LogSystem::get_instance()->dump_memory_to_file(addr, len);
#define WLOG_TO_FILE_HEX(addr, len)		LogSystem::get_instance()->wchar_dump_memory_to_file(addr, len);
#define SET_PREFIX(a)					LogSystem::get_instance()->set_log_prefix(a);
#define SET_WPREFIX(a)					LogSystem::get_instance()->set_log_wprefix(a);

#else
#define OUTPUT_TO_STDOUT(flag_in)
#define DBGOUT(fmt, ...)		// do{}while(0);
#define W_DBGOUT(fmt, ...)		// do{}while(0);
#define DBGOUTHEX(addr, len)
#define W_DBGOUTHEX(addr, len)
#define LOG_TO_FILE(fmt, ...)		
#define WLOG_TO_FILE(fmt, ...)		
#define LOG_TO_FILE_HEX(addr, len)
#define WLOG_TO_FILE_HEX(addr, len)
#define SET_PREFIX(a)
#define SET_WPREFIX(a)
#endif
