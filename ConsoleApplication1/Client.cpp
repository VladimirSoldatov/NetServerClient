
#include <iostream>
#include <iostream>
#include<WS2tcpip.h>
#include"windows.h"
#include <string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main()
{
	setlocale(LC_ALL, "RUSSIAN");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string ipAddress = "127.0.0.1";
	int port = 54000;
	//Инициализация winSock
	WSADATA wsData; //https://learn.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-wsadata
	WORD  ver = MAKEWORD(2, 2); //Собщим версию сокета https://learn.microsoft.com/en-us/windows/win32/winsock/initializing-winsock
	int wsOK = WSAStartup(ver, &wsData);// Передаем структуру данных и версию сокета
	if (wsOK != 0)
	{
		cerr << "Не получилось инициализиорвать WinSock!\n";
		return 403;
	}

	//Создание сокета
	SOCKET connection = socket(AF_INET, SOCK_STREAM, 0);  // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
	if (connection == INVALID_SOCKET)
	{
		cerr << "Не могу создать сокет!\n";
		return 404;
	}

	// Связать сокет спортом и IP - адресом
	sockaddr_in  hint; //Сктрутура подсказок по сокету;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port); //Привязываем порт к сокету
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	int connResult = connect(connection, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "No connection " <<WSAGetLastError() <<"\n";
		closesocket(connection);
		WSACleanup();
		return 0;
	}
	char buff[4096];
	string userInput;
	do
	{
		cout << "> ";
		getline(cin, userInput);
		if (userInput.size() > 0)
		{
			int sendResult = send(connection, userInput.c_str(), (int)userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buff, 4096);
				int bytesRecieved = recv(connection, buff, 4096, 0);
				if (bytesRecieved > 0)
				{
					cout << "SERVER " << string(buff, 0, bytesRecieved);
				}
			}
		}

	} while (userInput.size() > 0);
	int error = closesocket(connection);
	// Выключаем WinSock
	WSACleanup();
	return 0;


}

