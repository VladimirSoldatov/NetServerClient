#include <iostream>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main()
{
	//Инициализация winSock
	WSADATA wsData;
	WORD  ver = MAKEWORD(2,2);
	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cerr << "";
	}
	return 0;
}

