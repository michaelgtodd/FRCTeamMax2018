#pragma once
#include <string>
#include "MaxTask.h"
#include <vector>

namespace MaxLog
{
	class MaxCautionManager : public MaxTask
	{
	public:
		void RegisterCaution(std::string caution);
		void ClearCautions();
		void Run();
		void Disable();
		void Autonomous();
		void ControllerUpdate(MaxControl * controls);
	private:
		void Init();
		std::vector<std::string> CautionList;
	};
	void InitializeMaxLog();
	void LogPass(std::string error_message);
	void LogInfo(std::string error_message);
	void LogError(std::string error_message);

	void TransmitInt(std::string label, int value);
	void TransmitString(std::string label, std::string value);
	void TransmitDouble(std::string label, double value);
	void TransmitBool(std::string label, bool value);
}