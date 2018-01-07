#include "MaxDataStream.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "MaxDefs.h"

#define OUTPUT_BUFFER_SIZE 1024

namespace MaxLog
{
	UdpTransmitSocket transmitSocket(IpEndpointName(BROADCASTADDRESS, BROADCASTPORT));
	char buffer[OUTPUT_BUFFER_SIZE];

	void LogPass(std::string error_message)
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/pass") << error_message.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void LogInfo(std::string error_message)
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/info") << error_message.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void LogError(std::string error_message)
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage("/error") << error_message.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitInt(std::string label, int value) 
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitString(std::string label, std::string value)
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value.c_str() << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitDouble(std::string label, double value)
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}

	void TransmitBool(std::string label, bool value)
	{
		osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

		p << osc::BeginMessage(label.c_str()) << value << osc::EndMessage;

		transmitSocket.Send(p.Data(), p.Size());
	}
}