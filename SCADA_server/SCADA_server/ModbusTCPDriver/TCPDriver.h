#pragma once
#ifndef  TCP_DRIVER_H
#define TCP_DRIVER_H
#include "stdafx.h"

class TCPDriver {
public:
	
	/*
	This function is used for creating request. It makes tcpHeader internally and glue it to the rest of the the message,
	making complete modbus-tcp request.
	@param functionCode - 1 byte
	@param startingAddress - 2 bytes
	@param quantityOfCoils - 2 bytes
	@return request - 12 bytes
	*/
	char *createRequest(char functionCode, char *startingAddress, char *quantityOfCoils);
	/*
	This function is used for sending request over tcp connection.
	@param request - 12 bytes got from createRequest function. Example call: sendRequest(createRequest(...))
	@return enum value
	*/
	int sendRequest(char *request);
	int receiveResponse();
	int pushInBuffer();
	/*
		Connection is made with the modbus simulator. Supporting functions are private.
	*/
	int tcpConnect();
	/*
		In this function connection is closing. Socket is destroying.
	*/
	int tcpCloseConnection();
	/*
		TCPDriver is organized as a singleton. There is only one instance of this class.
	*/
	static TCPDriver& getInstance()
	{
		static TCPDriver instance; // Guaranteed to be destroyed.
							       // Instantiated on first use.
		return instance;
	}
	/*
		We need to delete copy constructor and assignment operator in order to forbid their use, because of the singleton pattern logic.
	*/
	TCPDriver(TCPDriver const&) = delete;
	void operator=(TCPDriver const&) = delete;
	SOCKET *getSocket() const;
	void setSocket(SOCKET *socket);
	char *getIpAddress() const;
	void setIpAddress(char *ipAddress);
	int getPort() const;
	void setPort(int port);
private:
	TCPDriver() {
		sock = INVALID_SOCKET;
		tcpConnect();
	}
	TCPDriver(char *ipAddress_, int port_/*, Buffer *sharedBuffer*/, unsigned long int nonBlockingMode_): ipAddress(ipAddress_), port(port_), nonBlockingMode(nonBlockingMode_){
		sock = INVALID_SOCKET;
		tcpConnect();
	}
	~TCPDriver() {
		delete ipAddress, ipAddress = 0;
		tcpCloseConnection();
	}
	SOCKET sock;
	//Buffer *sharedBuffer;
	//char *response;
	char *ipAddress;
	int port;
	unsigned long int nonBlockingMode;
	/*
		Supporting function. It is used inside tcpConnect() and its job is to create and open socket towards modbus simulator.
	*/
	int createSocket();
	int selectt(int type);
};


#endif // ! TCP_DRIVER_H