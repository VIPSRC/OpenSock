#ifndef SOCKET_H
#define SOCKET_H

#include "includes.h"

#define SOCKET_NAME "\0XARG"
#define BACKLOG 8
#define PacketSize 4096
int sock;
int connected;
struct sockaddr_un addr_server;
char socket_name[108];

int Create()
{
	int created = (sock = socket(AF_UNIX, SOCK_STREAM, 0)) != -1;
	return created;
}

void Close()
{
	connected = 0;
	if (sock > 0)
		close(sock);
}

int Connect()
{
	memset(socket_name, 0, sizeof(socket_name));
	memcpy(&socket_name[0], "\0", 1);
	strcpy(&socket_name[1], SOCKET_NAME);

	memset(&addr_server, 0, sizeof(struct sockaddr_un));
	addr_server.sun_family = AF_UNIX;
	strncpy(addr_server.sun_path, socket_name, sizeof(addr_server.sun_path) - 1);

	if (connect(sock, (struct sockaddr *)&addr_server, sizeof(addr_server)) == -1)
	{
		Close();
		return 0;
	}

	connected = 1;
	return 1;
}

size_t sendData(void *inData, size_t size)
{
	char *buffer = (char *)inData;
	int numSent = 0;
	while (size)
	{
		do
		{
			numSent = write(sock, buffer, size);
		}
		while (numSent == -1 && EINTR == errno);
		if (numSent <= 0)
		{
			Close();
			break;
		}
		size -= numSent;
		buffer += numSent;
	}
	return numSent;
}

size_t send(void *inData, size_t size)
{
	uint32_t length = htonl(size);
	if (sendData(&length, sizeof(uint32_t)) <= 0)
	{
		return 0;
	}
	return sendData((void *)inData, size) > 0;
}

int recvData(void *outData, size_t size)
{
	char *buffer = (char *)outData;
	int numReceived = 0;
	while (size)
	{
		do
		{
			numReceived = read(sock, buffer, size);
		}
		while (numReceived == -1 && EINTR == errno);
		if (numReceived <= 0)
		{
			Close();
			break;
		}
		size -= numReceived;
		buffer += numReceived;
	}
	return numReceived;
}

size_t receive(void *outData)
{
	uint32_t length = 0;
	int code = recvData(&length, sizeof(uint32_t));
	if (code > 0)
	{
		length = ntohl(length);
		recvData(outData, (size_t) (length));
	}
	return length;
}

#endif
