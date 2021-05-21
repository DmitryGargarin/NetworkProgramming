// ПСП (2) - Сервер.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <WinSock2.h>
#include <ctime>
#pragma comment(lib, "WS2_32.lib")
#pragma warning(disable : 4996)
using namespace std;

string SetErrorMsgText(string msg, int errNum)
{
	string text;
	switch (errNum)
	{
	case WSAEINTR: text = "WSAEINTR - Работа функции прервана "; break;
	case WSAEACCES: text = "WSAEACCESS - Разрешение отвергнуто"; break;
	case WSAEFAULT: text = "WSAEFAULT - Ошибочный адрес"; break;
	case WSAEINVAL: text = "WSAEINVAL - Ошибка в аргументе"; break;
	case WSAEMFILE: text = "WSAEMFILE - Слишком много файлов открыто"; break;
	case WSAEWOULDBLOCK: text = "WSAEWOULDBLOCK - Ресурс временно недоступен"; break;
	case WSAEINPROGRESS: text = "WSAEINPROGRESS - Операция в процессе развития"; break;
	case WSAEALREADY: text = "WSAEALREADY - Операция уже выполняется"; break;
	case WSAENOTSOCK: text = "WSAENOTSOCK - Неправильно задан сокет"; break;
	case WSAEDESTADDRREQ: text = "WSAEDESTADDRREQ - Требуется адрес расположения"; break;
	case WSAEMSGSIZE: text = "WSAEMSGSIZE - Сообщение слишком длинное"; break;
	case WSAEPROTONOSUPPORT: text = "WSAEPROTONOSUPPORT - Протокол не поддерживается"; break;
	case WSAEISCONN: text = "WSAEISCONN - Сокет уже подключён"; break;
	case WSAESHUTDOWN: text = "WSAESHUTDOWN - Сокет завершил работу"; break;
	case WSA_NOT_ENOUGH_MEMORY: text = "WSA_NOT_ENOUGH_MEMORY - Не хватает памяти"; break;
	case WSASYSCALLFAILURE: text = "WSASYSCALLFAILURE - Аварийное завершение системного вызова"; break;
	default: text = "Что-то там"; break;

	}
	return msg + ": " + text;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	SOCKET sS; /* собственный сокет сервера*/
	try
	{
		//Старт сервера
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}
		//Подключение сокета
		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket", WSAGetLastError());
		}

		//привязка сокета к АйПи и порту
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(sS, (LPSOCKADDR)& serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Bind", WSAGetLastError());

		while(true)
		{
			//Включение прослушки
			if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
				throw SetErrorMsgText("Listen", WSAGetLastError());

			//Разрешение подключения
			SOCKADDR_IN clnt; /*инфа о клиенте*/
			SOCKET cS; /*сокет для подключения клиента; канал передачи*/
			memset(&clnt, 0, sizeof(clnt));
			int lenCl = sizeof(clnt);
			cS = accept(sS, (sockaddr*)& clnt, &lenCl); //лучше приравнивать отдельно, а не в операторе условия
			if (cS == INVALID_SOCKET)
				throw SetErrorMsgText("Accept", WSAGetLastError());

			//cout << "IP клиента: " << (ULONG)(&clnt.sin_addr.S_un.S_addr) << endl;
			cout << "--------------------------------------------------------------\nIP клиента: " << inet_ntoa(clnt.sin_addr) << endl;
			cout << "Порт клиента: " << htons(clnt.sin_port) << endl;

			//принятие сообщения
			char bfrom[50];
			int ito = 0;
			clock_t t1 = clock();
			do
			{
				if ((ito = recv(cS, (char*)& bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
					break;
					//throw SetErrorMsgText("Recv", WSAGetLastError());
				cout << endl << "Получено: " << bfrom << endl;

				//Отправка сообщения
				if ((ito = send(cS, (char*)& bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
					throw SetErrorMsgText("Send", WSAGetLastError());
				cout << endl << "Отправлено" << endl;
			} while (ito != 0);

			clock_t t2 = clock();

			cout << "Затраченное время: " << (t2 - t1)/1000 << " секунд" << endl;


			//Закрытие сокета
			if (closesocket(cS) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("CloseSocket", WSAGetLastError());
			}
		}
		if (closesocket(sS) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("CloseSocket", WSAGetLastError());
		}	
		//Выключение сервера
		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("WSACleanup", WSAGetLastError());
		};
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}

	return 0;
}
