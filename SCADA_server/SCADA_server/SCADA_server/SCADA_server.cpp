// SCADA_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool InitializeWindowsSockets();
int main()
{
	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}
	Buffer *commandingBuffer = new Buffer("commanding_buffer", 512);
	Buffer *sharedBuffer = new Buffer("shared_buffer", 512);
	Buffer *streamBuffer = new Buffer("stream_buffer", 8000);

	TCPDriver::getInstance().setIpAddress("127.0.0.1");
	TCPDriver::getInstance().setPort(502);
	TCPDriver::getInstance().tcpConnect();
	/*
	ClientHandler *ch = new ClientHandler(commandingBuffer, streamBuffer, 1, "127.0.0.1", "27016");
	ch->tcpConnect();
	*/
	std::vector<char *> *vector = new std::vector<char *>();
	char req[5];
	req[0] = 0x04;
	req[1] = 0x00;
	req[2] = 0x00;
	req[3] = 0x00;
	req[4] = 0x01;
	vector->push_back(req);
	PollEngine *pollEngine = new PollEngine(vector);
	PollEngine::sendRequests(pollEngine);
	//delete commandingBuffer, commandingBuffer = 0;
	//delete streamBuffer, streamBuffer = 0;
	//delete ch, ch = 0;

	Sleep(INFINITE);
    return 0;
}

bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}
