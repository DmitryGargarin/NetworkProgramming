#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#pragma warning( disable : 4996)
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
	SOCKET cC; //служит для коннекта к серверу; все операции со стороны клиента проходят через него
	try
	{
		int count;
		cout << "Сколько раз отправлять сообщение?\t"; cin >> count;
		//Старт клиента
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}
		//Подключение сокета
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket", WSAGetLastError());
		}


		//отправка сообщения
		SOCKADDR_IN to; //структура сервера
		to.sin_family = AF_INET;
		to.sin_port = htons(2000);
		to.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		char bto[50] = "Hello"; int ito;
		char bfrom[50]; int ifrom;
		char stop[1] = { '\0' };
		for (int i = 0; i < count; i++)
		{
			ito = sendto(cC, (char*)& bto, sizeof(bto), NULL, (sockaddr*)& to, sizeof(to));
			if (ito == SOCKET_ERROR)
				throw SetErrorMsgText("SendTo", WSAGetLastError());
			
			////приём сообщения 
			SOCKADDR_IN from;
			int scl = sizeof(from);
			ifrom = recvfrom(cC, (char*)& bfrom, sizeof(bfrom), NULL, (sockaddr*)& from, &scl);
			if (ifrom == 0)
				throw SetErrorMsgText("RecvFrom", WSAGetLastError());
			cout << bfrom << endl;
		}
		if ((ito = sendto(cC, (char*) &stop, sizeof(stop), NULL, (sockaddr*)&to, sizeof(to))) == SOCKET_ERROR)
			throw SetErrorMsgText("Send", WSAGetLastError());
		//Закрытие сокета
		if (closesocket(cC) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("CloseSocket", WSAGetLastError());
		}
		//Выключение клиента
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