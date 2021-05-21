#include <iostream>
#include <WinSock2.h>
#include <ctime>
#pragma comment (lib, "WS2_32.lib")
#pragma warning (disable : 4996)

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
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
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


		//приём сообщения 
		SOCKADDR_IN from; //структура клиента
		int scl = sizeof(from);
		char bfrom[50];
		char bto[50] = "Hellp me plz from Server";
		int ifrom, ito;
		clock_t t1 = clock();
		do
		{
			ifrom = recvfrom(sS, (char*)& bfrom, sizeof(bfrom), NULL, (sockaddr*)& from, &scl);
			if (ifrom == 0)
				throw SetErrorMsgText("RecvFrom", WSAGetLastError());
			cout << bfrom << endl;

			//отправка сообщения
			ito = sendto(sS, (char*)& bto, sizeof(bto), NULL, (sockaddr*)& from, scl);
			if (ito == SOCKET_ERROR)
				throw SetErrorMsgText("SendTo", WSAGetLastError());

		} while (ifrom != 1);

		clock_t t2 = clock();

		cout << "Затраченное время: " << (t2 - t1) / 1000 << " секунд" << endl;

		//Закрытие сокета	
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
