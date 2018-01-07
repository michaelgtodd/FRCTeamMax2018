#pragma once
#include <string>

namespace MaxLog
{
	void LogPass(std::string error_message);
	void LogInfo(std::string error_message);
	void LogError(std::string error_message);

	void TransmitInt(std::string label, int value);
	void TransmitString(std::string label, std::string value);
	void TransmitDouble(std::string label, std::string value);
	void TransmitBool(std::string label, std::string value);
}