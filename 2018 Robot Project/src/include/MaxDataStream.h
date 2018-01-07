#pragma once
#include <string>

namespace MaxLog
{
	void LogError(std::string error_message);

	void TransmitInt(std::string label, int value);
	void TransmitString(std::string label, std::string value);
	void TransmitDouble(std::string label, std::string value);
	void TransmitBool(std::string label, std::string value);
}