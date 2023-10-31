#include "LogSystem.h"
#include <locale>
#include <windows.h>
#include <iostream>

#define MAX_LOG_CACHE		(4096)
#define DUMPMEMORY_BYTESPERROW  static_cast<std::size_t>(16)

LogSystem* LogSystem::_log_instance = new LogSystem;
LogSystem::GCHelper LogSystem::GCHelper::auto_gc;

LogSystem::~LogSystem()
{
	// do some cleaning....
	
	if (_log_file_stream.is_open() == true)
	{
		_log_file_stream.close();
	}
	if (_log_file_wstream.is_open() == true)
	{
		_log_file_wstream.close();
	}
}

LogSystem::LogSystem()
{
	_is_output_to_stdout = true;
#ifdef USE_DBG
	_log_file_stream.open("LogSystem_log.txt", std::ios::app);
	_log_file_wstream.open("LogSystem_wlog.txt", std::ios::app);
#endif
}

LogSystem* LogSystem::get_instance()
{
	return _log_instance;
}

void LogSystem::set_log_prefix(std::string prefix_to_set)
{
	_prefix = prefix_to_set;
	// _prefix += " [" __FUNCTION__ "]";
}

void LogSystem::set_log_wprefix(std::wstring wprefix_to_set)
{
	_wprefix = wprefix_to_set;
	// _wprefix += L" [" __FUNCTION__ "]";
}

void LogSystem::is_output_to_stdout(bool input_flag)
{
	_is_output_to_stdout = input_flag;
}

void LogSystem::debug_out(const char* prefix_str, const char* format, ...)
{
	if (!format)
	{
		return;
	}
	const std::shared_ptr<char> log_buffer(new char[MAX_LOG_CACHE]);
	va_list ap;
	va_start(ap, format);
	_vsnprintf_s(log_buffer.get(), (MAX_LOG_CACHE - 1), MAX_LOG_CACHE - 1, format, ap);
	va_end(ap);
	const std::string logout_message(log_buffer.get());
	std::string logout;
	if (_prefix.empty() == true)
	{
		logout = prefix_str;
	}
	else
	{
		logout = _prefix + prefix_str;
	}
	logout += logout_message;
	
	if (_is_output_to_stdout == true)
	{
		std::cout << logout << std::endl;
	}
	logout += "\n";
	OutputDebugStringA(logout.c_str());
}

void LogSystem::wchar_debug_out(const wchar_t* prefix_str, const wchar_t* pszFormat, ...)
{
	if (!pszFormat)
	{
		return;
	}
	const std::shared_ptr<wchar_t> log_buffer(new wchar_t[MAX_LOG_CACHE]);
	va_list ap;
	va_start(ap, pszFormat);
	_vsnwprintf_s(log_buffer.get(), (MAX_LOG_CACHE - 1), MAX_LOG_CACHE - 1, pszFormat, ap);
	va_end(ap);

	const std::wstring logout_message(log_buffer.get());
	std::wstring logout;

	if (_wprefix.empty() == true)
	{
		logout = prefix_str;
	}
	else
	{
		logout = _wprefix + prefix_str;
	}
	logout += logout_message;
	if (_is_output_to_stdout == true)
	{
		// setlocale(LC_CTYPE, "");
		// wprintf(_T("%ls"), log_buffer);
		// wprintf(L"\n");
		// std::wcout << log_buffer.get() << std::endl;

		// std::wcout.imbue(std::locale("chs")); 
		std::wcout.imbue(std::locale("", LC_CTYPE));
		std::wcout << logout << std::endl;
	}
	logout += L"\n";
	OutputDebugStringW(logout.c_str());
}

void LogSystem::debug_out_to_file(const char* prefix_str, const char* format, ...)
{
	if (!format)
	{
		return;
	}
	const std::shared_ptr<char> log_buffer(new char[MAX_LOG_CACHE]);
	va_list ap;
	va_start(ap, format);
	_vsnprintf_s(log_buffer.get(), (MAX_LOG_CACHE - 1), MAX_LOG_CACHE - 1, format, ap);
	va_end(ap);
	const std::string logout_message(log_buffer.get());
	std::string logout;
	if (_prefix.empty() == true)
	{
		logout = prefix_str;
	}
	else
	{
		logout = _prefix + prefix_str;
	}
	logout += logout_message;
	_log_file_stream << logout << std::endl;
}

void LogSystem::wchar_debug_out_to_file(const wchar_t* prefix_str, const wchar_t* format, ...)
{
	if (!format)
	{
		return;
	}
	const std::shared_ptr<wchar_t> log_buffer(new wchar_t[MAX_LOG_CACHE]);
	va_list ap;
	va_start(ap, format);
	_vsnwprintf_s(log_buffer.get(), (MAX_LOG_CACHE - 1), MAX_LOG_CACHE - 1, format, ap);
	va_end(ap);

	
	const std::wstring logout_message(log_buffer.get());
	std::wstring logout;
	if (_wprefix.empty() == true)
	{
		logout = prefix_str;
	}
	else
	{
		logout = _wprefix + prefix_str;
	}
	logout += logout_message;
	// _log_file_wstream.imbue(std::locale("chs"));

	_log_file_wstream.imbue(std::locale("", LC_CTYPE));
	_log_file_wstream << logout << std::endl;
}

void LogSystem::dump_memory_to_file(void* memory_addr, std::size_t length_in_bytes)
{
	do
	{
		if ((memory_addr == nullptr) || (length_in_bytes == NULL))
		{
			break;
		}
		unsigned char* mem_addr = reinterpret_cast<unsigned char*>(memory_addr);
		std::size_t dwTimes = length_in_bytes / 16 + (length_in_bytes % 16 > 0 ? 1 : 0);

		for (std::size_t dwTmpIdx = 0; dwTmpIdx < dwTimes; dwTmpIdx++)
		{
			if (dwTmpIdx != dwTimes - 1)
			{
				debug_out_hex_file(mem_addr + dwTmpIdx * 16, 16);
			}
			else
			{
				debug_out_hex_file(mem_addr + dwTmpIdx * 16, length_in_bytes - dwTmpIdx * 16);
			}
		}
	} while (false);
}

void LogSystem::wchar_dump_memory_to_file(void* memory_addr, std::size_t length_in_bytes)
{
	do
	{
		if ((memory_addr == nullptr) || (length_in_bytes == NULL))
		{
			break;
		}
		unsigned char* mem_addr = reinterpret_cast<unsigned char*>(memory_addr);
		std::size_t dwTimes = length_in_bytes / 16 + (length_in_bytes % 16 > 0 ? 1 : 0);

		for (std::size_t dwTmpIdx = 0; dwTmpIdx < dwTimes; dwTmpIdx++)
		{
			if (dwTmpIdx != dwTimes - 1)
			{
				wc_debug_out_hex_file(mem_addr + dwTmpIdx * 16, 16);
			}
			else
			{
				wc_debug_out_hex_file(mem_addr + dwTmpIdx * 16, length_in_bytes - dwTmpIdx * 16);
			}
		}
	} while (false);
}

bool LogSystem::dump_memory(unsigned char* lpBytes, std::size_t nCount)
{
	BOOL bReadException = ::IsBadReadPtr(lpBytes, nCount);
	char szBuffer[DUMPMEMORY_BYTESPERROW * 3 + 1] = { 0 };
	std::size_t nIndex = 0;

	if (!bReadException)
	{
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			//_snprintf(szBuffer, DUMPMEMORY_BYTESPERROW * 3, "%s %02x", szBuffer, lpBytes[nIndex]);
			_snprintf_s(szBuffer, DUMPMEMORY_BYTESPERROW * 3 + 1, DUMPMEMORY_BYTESPERROW * 3, "%s %02x", szBuffer, lpBytes[nIndex]);

		}

		debug_out(_prefix.c_str(), szBuffer);
	}

	return bReadException;
}

bool LogSystem::wdump_memory(unsigned char* lpBytes, std::size_t nCount)
{
	BOOL bReadException = ::IsBadReadPtr(lpBytes, nCount);
	wchar_t szBuffer[DUMPMEMORY_BYTESPERROW * 3 + 1] = { 0 };
	std::size_t nIndex = 0;

	if (!bReadException)
	{
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			_snwprintf_s(szBuffer, DUMPMEMORY_BYTESPERROW * 3, DUMPMEMORY_BYTESPERROW * 3, L"%ls %02x", szBuffer, lpBytes[nIndex]);
		}

		wchar_debug_out(_wprefix.c_str(), szBuffer);
	}

	return bReadException;
}

bool LogSystem::dump_memory_file(unsigned char* lpBytes, std::size_t nCount)
{
	BOOL bReadException = ::IsBadReadPtr(lpBytes, nCount);
	char szBuffer[DUMPMEMORY_BYTESPERROW * 3 + 1] = { 0 };
	std::size_t nIndex = 0;

	if (!bReadException)
	{
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			//_snprintf(szBuffer, DUMPMEMORY_BYTESPERROW * 3, "%s %02x", szBuffer, lpBytes[nIndex]);
			_snprintf_s(szBuffer, DUMPMEMORY_BYTESPERROW * 3 + 1, DUMPMEMORY_BYTESPERROW * 3, "%s %02x", szBuffer, lpBytes[nIndex]);

		}

		debug_out_to_file(_prefix.c_str(), szBuffer);
	}

	return bReadException;
}

bool LogSystem::wdump_memory_file(unsigned char* lpBytes, std::size_t nCount)
{
	BOOL bReadException = ::IsBadReadPtr(lpBytes, nCount);
	wchar_t szBuffer[DUMPMEMORY_BYTESPERROW * 3 + 1] = { 0 };
	std::size_t nIndex = 0;

	if (!bReadException)
	{
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			_snwprintf_s(szBuffer, DUMPMEMORY_BYTESPERROW * 3, DUMPMEMORY_BYTESPERROW * 3, L"%ls %02x", szBuffer, lpBytes[nIndex]);
		}

		wchar_debug_out_to_file(_wprefix.c_str(), szBuffer);
	}

	return bReadException;
}

void LogSystem::debug_out_hex(unsigned char* lpBytes, std::size_t dwMemorySize)
{
	std::size_t nLoopCount = 0, nLeftBytes = 0, nIndex = 0;
	BOOL bReadException = FALSE;

	nLoopCount = dwMemorySize / DUMPMEMORY_BYTESPERROW;
	nLeftBytes = dwMemorySize % DUMPMEMORY_BYTESPERROW;

	for (nIndex = 0; nIndex < nLoopCount; nIndex++)
	{
		bReadException = dump_memory(lpBytes + nIndex * DUMPMEMORY_BYTESPERROW, DUMPMEMORY_BYTESPERROW);
		if (bReadException)
		{
			break;
		}
	}

	if (!bReadException && nLeftBytes > 0)
	{
		dump_memory(lpBytes + nLoopCount * DUMPMEMORY_BYTESPERROW, nLeftBytes);
	}
}

void LogSystem::wc_debug_out_hex(unsigned char* lpBytes, std::size_t dwMemorySize)
{
	std::size_t nLoopCount = 0, nLeftBytes = 0, nIndex = 0;
	BOOL bReadException = FALSE;

	nLoopCount = dwMemorySize / DUMPMEMORY_BYTESPERROW;
	nLeftBytes = dwMemorySize % DUMPMEMORY_BYTESPERROW;

	for (nIndex = 0; nIndex < nLoopCount; nIndex++)
	{
		bReadException = wdump_memory(lpBytes + nIndex * DUMPMEMORY_BYTESPERROW, DUMPMEMORY_BYTESPERROW);
		if (bReadException)
		{
			break;
		}
	}

	if (!bReadException && nLeftBytes > 0)
	{
		wdump_memory(lpBytes + nLoopCount * DUMPMEMORY_BYTESPERROW, nLeftBytes);
	}
}


void LogSystem::debug_out_hex_file(unsigned char* lpBytes, std::size_t dwMemorySize)
{
	std::size_t nLoopCount = 0, nLeftBytes = 0, nIndex = 0;
	BOOL bReadException = FALSE;

	nLoopCount = dwMemorySize / DUMPMEMORY_BYTESPERROW;
	nLeftBytes = dwMemorySize % DUMPMEMORY_BYTESPERROW;

	for (nIndex = 0; nIndex < nLoopCount; nIndex++)
	{
		bReadException = dump_memory_file(lpBytes + nIndex * DUMPMEMORY_BYTESPERROW, DUMPMEMORY_BYTESPERROW);
		if (bReadException)
		{
			break;
		}
	}

	if (!bReadException && nLeftBytes > 0)
	{
		dump_memory_file(lpBytes + nLoopCount * DUMPMEMORY_BYTESPERROW, nLeftBytes);
	}
}

void LogSystem::wc_debug_out_hex_file(unsigned char* lpBytes, std::size_t dwMemorySize)
{
	std::size_t nLoopCount = 0, nLeftBytes = 0, nIndex = 0;
	BOOL bReadException = FALSE;

	nLoopCount = dwMemorySize / DUMPMEMORY_BYTESPERROW;
	nLeftBytes = dwMemorySize % DUMPMEMORY_BYTESPERROW;

	for (nIndex = 0; nIndex < nLoopCount; nIndex++)
	{
		bReadException = wdump_memory_file(lpBytes + nIndex * DUMPMEMORY_BYTESPERROW, DUMPMEMORY_BYTESPERROW);
		if (bReadException)
		{
			break;
		}
	}

	if (!bReadException && nLeftBytes > 0)
	{
		wdump_memory_file(lpBytes + nLoopCount * DUMPMEMORY_BYTESPERROW, nLeftBytes);
	}
}

void LogSystem::debug_out_memory(void* memory_addr, std::size_t length_in_bytes)
{
	do 
	{
		if ( (memory_addr == nullptr) || (length_in_bytes == NULL) )
		{
			break;
		}
		unsigned char* mem_addr = reinterpret_cast<unsigned char*>(memory_addr);
		std::size_t dwTimes = length_in_bytes / 16 + (length_in_bytes % 16 > 0 ? 1 : 0);

		for (std::size_t dwTmpIdx = 0; dwTmpIdx < dwTimes; dwTmpIdx++)
		{
			if (dwTmpIdx != dwTimes - 1)
			{
				debug_out_hex(mem_addr + dwTmpIdx * 16, 16);
			}
			else
			{
				debug_out_hex(mem_addr + dwTmpIdx * 16, length_in_bytes - dwTmpIdx * 16);
			}
		}
	} while (false);
}

void LogSystem::wchar_debug_out_memory(void* memory_addr, std::size_t length_in_bytes)
{
	do
	{
		if ((memory_addr == nullptr) || (length_in_bytes == NULL))
		{
			break;
		}
		unsigned char* mem_addr = reinterpret_cast<unsigned char*>(memory_addr);
		std::size_t dwTimes = length_in_bytes / 16 + (length_in_bytes % 16 > 0 ? 1 : 0);

		for (std::size_t dwTmpIdx = 0; dwTmpIdx < dwTimes; dwTmpIdx++)
		{
			if (dwTmpIdx != dwTimes - 1)
			{
				wc_debug_out_hex(mem_addr + dwTmpIdx * 16, 16);
			}
			else
			{
				wc_debug_out_hex(mem_addr + dwTmpIdx * 16, length_in_bytes - dwTmpIdx * 16);
			}
		}
	} while (false);
}