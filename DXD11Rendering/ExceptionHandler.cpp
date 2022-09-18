#include "ExceptionHandler.h"
#include <sstream>

ExceptionHandler::ExceptionHandler(int line, const char* file) noexcept
	: m_line(line), m_file(file)
{
	
}

const char* ExceptionHandler::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	_buffer = oss.str();
	return _buffer.c_str();
}

const char* ExceptionHandler::GetType() const noexcept { return "Exception"; }

int ExceptionHandler::GetLine() const noexcept { return m_line; }

const std::string& ExceptionHandler::GetFile() const noexcept { return m_file; }

std::string ExceptionHandler::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_file << std::endl << "Line: " << m_line;
	return oss.str();
}