#pragma once
#ifndef __NET_H__
#define __NET_H__


#include<WinSock2.h>
#include<cocos2d.h>
class Net
{
protected:
	Net()
	{
		_server = INVALID_SOCKET;
		_client = INVALID_SOCKET;
	}
	SOCKET _server;
	SOCKET _client;
	CC_SYNTHESIZE(bool, _isServer, isServer);
	CC_SYNTHESIZE(bool, _isListen, isListen);
	CC_SYNTHESIZE(bool, _isConnect, isConnect);
	static Net* _theOne;

	void closeNet()
	{
		closesocket(_client);
		closesocket(_server);
		CCLOG("close success!");
	}

	void setNonBlock(SOCKET sock)
	{
		u_long arg = 1;
		ioctlsocket(sock, FIONBIO, &arg);
	}
public:
	static Net* getInstance()
	{
		if (_theOne == NULL)
			return _theOne = new Net;
		return _theOne;
	}

	//作为服务器的接口
	bool StartServer(unsigned short port)
	{
		//创建socket
		_server = socket(AF_INET, SOCK_STREAM, 0);
		if (_server == INVALID_SOCKET)
		{
			int err = WSAGetLastError();
			CCLOG("Socket creat error %d",err);
			return false;
		}
		setNonBlock(_server);

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;

		int ret = bind(_server, (struct sockaddr*)&addr, sizeof(addr));
		if (ret == SOCKET_ERROR)
		{
			CCLOG("bind sockett error!");
			closesocket(_server);
			return false;
		}
		listen(_server, 10);
		_isServer = true;
		_isListen = true;
		_isConnect = false;
		return true;
	}

	bool Accept()
	{
		if (!_isServer || !_isListen || _isConnect) return false;
		_client = accept(_server, NULL, NULL);
		if (_client == INVALID_SOCKET)
			return false;
		setNonBlock(_client);
		_isConnect = true;
		return true;
	}

	bool Connect(unsigned short port, const char* ipaddr)
	{
		//创建socket
		_client = socket(AF_INET, SOCK_STREAM, 0);
		if (_client == INVALID_SOCKET)
		{
			CCLOG("err");
			return false;
		}
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);

		int ret=connect(_client, (struct sockaddr*)&addr, sizeof(addr));
		if (ret == SOCKET_ERROR)
		{
			CCLOG("connect error!");
			closesocket(_client);
			_client = INVALID_SOCKET;
			return false;
		}

		setNonBlock(_client);
		_isServer = false;
		_isListen = false;
		_isConnect = true;
		return true;
	}

	//客户端和服务端公用接口
	int Recv(void* buf, int len)
	{
		if (!_isConnect) return-1;//24 04

		int ret = recv(_client, (char*)buf, len, 0);
		return ret;
	}

	int Send(char* buf, int len)
	{
		if (!_isConnect) return -1;
		return send(_client, buf, len, 0);
	}

};

#endif