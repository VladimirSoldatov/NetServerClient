#include <iostream>
#include<WS2tcpip.h>
#include"windows.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main()
{
	setlocale(LC_ALL, "RUSSIAN");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
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
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);  // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
	if (listening == INVALID_SOCKET)
	{
		cerr << "Не могу создать сокет!\n";
		return 404;
	}

	// Связать сокет спортом и IP - адресом
	sockaddr_in  hint; //Сктрутура подсказок по сокету;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); //Привязываем порт к сокету
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //Можем также использовать другие порты

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//Указать, что сокет включен для прослушивания порта
	listen(listening, SOMAXCONN);

	// Дождаться соединения
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	char host[NI_MAXHOST];// Client remont name
	char service[NI_MAXHOST]; //Service, i.e. the client connect on
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);
	if (getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
		//https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getnameinfo
	{
		cout << host << " connection on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connection on port " << service << endl;
	}


	//Закрыть просулшивающий сокет
	closesocket(listening);
	char buff[4096];
	char * text = new char[4096]{"\0"};
	//В цикле ожижать поступление сообщений
	
	while (true)
	{

		ZeroMemory(buff, 4096);
		//Ожидаем пока клиента получит ответ
		int bytesRecieved;

		bytesRecieved = recv(clientSocket, buff, 4096, 0);
		if (bytesRecieved == SOCKET_ERROR)
		{
			cerr << "Ошибка получения в recv, выходим\n";
			break;
		}
		if (bytesRecieved == 0)
		{
			cout << "Client отключился\n";
			break;
		}


		//Возвращаем эхо сообщение на клиент

		if (strcmp(buff, "\r\n") == 0)
		{
			if (strcmp(text, "exit") == 0)
			{
				send(clientSocket, "Good-Bye", (int)strlen("Good-Bye") + 1, 0);
				delete[] text;
				break;
			}

			send(clientSocket, text, (int)strlen(text) + 1, 0);
			send(clientSocket, "\r\n", 3, 0);
			ZeroMemory(text, 4096);
		}
		else
		{
			strcat_s(text, (int)(strlen(text) + strlen(buff)) + 1, buff);
		}
	
	}

		//Закрыть  сокет
		int error = closesocket(clientSocket);
		// Выключаем WinSock
		WSACleanup();
		return 0;
}


/*



		ZeroMemory(buff, 4096);
		//Ожидаем пока клиента получит ответ
		int bytesRecieved;

		bytesRecieved = recv(clientSocket, buff, 4096, 0);

		if (bytesRecieved == SOCKET_ERROR)
		{
			cerr << "Ошибка получения в recv, выходим\n";
			break;
		}
		if (bytesRecieved == 0)
		{
			cout << "Client отключился\n";
			break;
		}


		//Возвращаем эхо сообщение на клиент

		if (strcmp(buff, "\r\n") == 0)
		{
			if (strcmp(text, "exit") == 0)
			{
				send(clientSocket, "Good-Bye", (int)strlen("Good-Bye") + 1, 0);
				delete[] text;
				break;
			}

			send(clientSocket, text, (int)strlen(text) + 1, 0);
			send(clientSocket, "\r\n", 3, 0);
			ZeroMemory(text, 4096);
		}
		else
		{
				strcat_s(text, (int)(strlen(text) + strlen(buff)) + 1, buff);
		}
	}
 */