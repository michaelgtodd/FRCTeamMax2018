#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxDefs.h"
#include "Robot.h"
#include <iostream>
#include <cerrno>
#include "pthread.h"

#include "osc/OscOutboundPacketStream.h"
#include "osc/OscPacketListener.h"
#include "osc/OscReceivedElements.h"
#include "ip/UdpSocket.h"

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

	}

	void MaxCautionManager::Always()
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

	void MaxCautionManager::ControllerUpdate(MaxControl * controls)
	{

	}

	void MaxCautionManager::Init()
	{

	}

	class MaxPacketListener : public osc::OscPacketListener {
	protected:

		virtual void ProcessMessage(const osc::ReceivedMessage& m,
			const IpEndpointName& remoteEndpoint)
		{
			(void)remoteEndpoint;

			try {
				ControlTaskInstance.ProcessOscData(m);
			}
			catch (osc::Exception& e) {
				LogError("Failed to parse OSC message");
			}
		}
	};

	std::vector<UdpTransmitSocket *> transmitSockets;

	void RunListener()
	{
		MaxPacketListener listener;
		UdpListeningReceiveSocket s(
			IpEndpointName(IpEndpointName::ANY_ADDRESS, BROADCASTPORT),
			&listener);

		s.RunUntilSigInt();
	}

	void InitializeMaxLog()
	{
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.5", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.9", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.10", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.11", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.12", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.13", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.14", BROADCASTPORT)));
		transmitSockets.push_back(new UdpTransmitSocket(IpEndpointName("10.10.71.15", BROADCASTPORT2)));

		std::thread * oscReceiveThread =  new std::thread(&RunListener);

		int priority = 97;

		sched_param sch;
		sch.sched_priority = priority;

		if (pthread_setschedparam(oscReceiveThread->native_handle(), SCHED_FIFO, &sch) != 0)
		{
			std::cout << "Failed to set task: " << "OSCReceive priority: " << priority << " Error: " << " " << strerror(errno) << std::endl;
			MaxLog::LogError("Failed to set task: OSCReceive priority: " + std::to_string(priority) + " Error: " + strerror(errno));
		}
		else {
			std::cout << "Set priority for task: " << "OSCReceive priority: " << priority << std::endl;
			MaxLog::LogInfo("Set priority for task: OSCReceive priority: " + std::to_string(priority));
		}
	}

	void transmit(osc::OutboundPacketStream p)
	{
		for (std::vector<UdpTransmitSocket *>::iterator i = transmitSockets.begin();
			i < transmitSockets.end();
			i++)
		{
			(*i)->Send(p.Data(), p.Size());
		}
	}
	
	void LogPass(std::string error_message)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/pass") << error_message.c_str() << osc::EndMessage;

		transmit(p);
	}

	void LogInfo(std::string error_message)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/info") << error_message.c_str() << osc::EndMessage;

		transmit(p);
	}

	void LogError(std::string error_message)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/error") << error_message.c_str() << osc::EndMessage;

		transmit(p);
	}

	void TransmitInt(std::string label, int value) 
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmit(p);
	}

	void TransmitString(std::string label, std::string value)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value.c_str() << osc::EndMessage;

		transmit(p);
	}

	void TransmitDouble(std::string label, double value)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmit(p);
	}

	void TransmitBool(std::string label, bool value)
	{
		char buffer[OUTPUT_BUFFER_SIZE];

		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmit(p);
	}
}