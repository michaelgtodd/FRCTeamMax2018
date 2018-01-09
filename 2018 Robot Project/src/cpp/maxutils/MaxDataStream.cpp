#include "maxutils/MaxDataStream.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "maxutils/MaxDefs.h"
#include <iostream>
#include <cerrno>

#define OUTPUT_BUFFER_SIZE 1024

namespace MaxLog
{
	void MaxCautionManager::RegisterCaution(std::string caution)
	{
		bool cautionDuplicate = false;
		for (std::vector<std::string>::iterator i = CautionList.begin();
			i != CautionList.end();
			i++)
		{
			cautionDuplicate = caution == (*i);
		}
		if (!cautionDuplicate)
		{
			CautionList.push_back(caution);
		}
	}

	void MaxCautionManager::ClearCautions()
	{
		CautionList.clear();
	}

	void MaxCautionManager::Run()
	{
		for (std::vector<std::string>::iterator i = CautionList.begin();
			i != CautionList.end();
			i++)
		{
			MaxLog::TransmitString("/caution", (*i));
		}
	}

	void MaxCautionManager::Disable()
	{
		Run();
	}

	void MaxCautionManager::Autonomous()
	{
		Run();
	}

	void MaxCautionManager::ControllerUpdate(MaxControls controls)
	{

	}

	void MaxCautionManager::Init()
	{

	}

	UdpTransmitSocket transmitSocket(IpEndpointName(BROADCASTADDRESS, BROADCASTPORT));

	void OscTransmit(osc::OutboundPacketStream stream)
	{

	}

	void LogPass(std::string error_message)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/pass") << error_message.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void LogInfo(std::string error_message)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/info") << error_message.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void LogError(std::string error_message)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/error") << error_message.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitInt(std::string label, int value) 
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitString(std::string label, std::string value)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitDouble(std::string label, double value)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitBool(std::string label, bool value)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}
}