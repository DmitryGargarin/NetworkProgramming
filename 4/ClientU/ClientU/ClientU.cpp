#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#pragma warning( disable : 4996)
using namespace std;

string SetErrorMsgText(string msg, int errNum)
{
	string msgText;
	switch (errNum)
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
		//..........коды WSAGetLastError ..........................
	case		WSAEFAULT: msgText = "	Ошибочный адрес"; break;
	case		WSAEINVAL: msgText = "	Ошибка в аргументе"; break;
	case		WSAEMFILE: msgText = "	Слишком много файлов открыто"; break;
	case		WSAEWOULDBLOCK: msgText = "	Ресурс временно недоступен"; break;
	case		WSAEINPROGRESS: msgText = "	Операция в процессе развития"; break;
	case		WSAEALREADY: msgText = "	Операция уже выполняется"; break;
	case		WSAENOTSOCK: msgText = "	Сокет задан неправильно"; break;
	case		WSAEDESTADDRREQ: msgText = "	Требуется адрес расположения"; break;
	case		WSAEMSGSIZE: msgText = "	Сообщение слишком длинное"; break;
	case		WSAEPROTOTYPE: msgText = "	Неправильный тип протокола для сокета"; break;
	case		WSAENOPROTOOPT: msgText = "	Ошибка в опции протокола"; break;
	case		WSAEPROTONOSUPPORT: msgText = "	Протокол не поддерживается"; break;
	case		WSAESOCKTNOSUPPORT: msgText = "	Тип сокета не поддерживается"; break;
	case		WSAEOPNOTSUPP: msgText = "	Операция не поддерживается"; break;
	case		WSAEPFNOSUPPORT: msgText = "	Тип протоколов не поддерживается"; break;
	case		WSAEAFNOSUPPORT: msgText = "	Тип адресов не поддерживается протоколом"; break;
	case		WSAEADDRINUSE: msgText = "	Адрес уже используется"; break;
	case	WSAEADDRNOTAVAIL: msgText = "		Запрошенный адрес не может быть использован"; break;
	case WSAENETDOWN: msgText = "		Сеть отключена"; break;
	case WSAENETUNREACH: msgText = "		Сеть не достижима"; break;
	case WSAENETRESET: msgText = "		Сеть разорвала соединение"; break;
	case WSAECONNABORTED: msgText = "	Программный отказ связи"; break;
	case WSAECONNRESET: msgText = "		Связь восстановлена"; break;
	case WSAENOBUFS: msgText = "		Не хватает памяти для буферов"; break;
	case WSAEISCONN: msgText = "		Сокет уже подключен"; break;
	case WSAENOTCONN: msgText = "		Сокет не подключен"; break;
	case WSAESHUTDOWN: msgText = "		Нельзя выполнить send : сокет завершил работу"; break;
	case WSAETIMEDOUT: msgText = "		Закончился отведенный интервал  времени"; break;
	case WSAECONNREFUSED: msgText = "		Соединение отклонено"; break;
	case WSAEHOSTDOWN: msgText = "		Хост в неработоспособном состоянии"; break;
	case WSAEHOSTUNREACH: msgText = "		Нет маршрута для хоста"; break;
	case WSAEPROCLIM: msgText = "		Слишком много процессов"; break;
	case WSASYSNOTREADY: msgText = "		Сеть не доступна"; break;
	case WSAVERNOTSUPPORTED: msgText = "		Данная версия недоступна"; break;
	case WSANOTINITIALISED: msgText = "		Не выполнена инициализация WS2_32.DLL"; break;
	case WSAEDISCON: msgText = "		Выполняется отключение"; break;
	case WSATYPE_NOT_FOUND: msgText = "		Класс не найден"; break;
	case WSAHOST_NOT_FOUND: msgText = "		Хост не найден"; break;
	case WSATRY_AGAIN: msgText = "		Неавторизированный хост не найден"; break;
	case WSANO_RECOVERY: msgText = "		Неопределенная  ошибка"; break;
	case WSANO_DATA: msgText = "		Нет записи запрошенного типа"; break;
	case WSA_INVALID_HANDLE: msgText = "		Указанный дескриптор события  с ошибкой"; break;
	case WSA_INVALID_PARAMETER: msgText = "		Один или более параметров с ошибкой"; break;
	case WSA_IO_INCOMPLETE: msgText = "		Объект ввода - вывода не в сигнальном состоянии"; break;
	case WSA_IO_PENDING: msgText = "		Операция завершится позже"; break;
	case WSA_NOT_ENOUGH_MEMORY: msgText = "		Не достаточно памяти"; break;
	case WSA_OPERATION_ABORTED: msgText = "		Операция отвергнута"; break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;

	}
	return msg + ": " + msgText;
}

bool GetServer(char* name, short port, struct sockaddr* from, int* flen)
{
	SOCKET sS;
	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
	{
		throw SetErrorMsgText("Socket", WSAGetLastError());
	}

	int optval = 1;
	int res = setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)& optval, sizeof(int));
	if (res == SOCKET_ERROR)
		throw SetErrorMsgText("SocketOpt", WSAGetLastError());

	SOCKADDR_IN to; //структура сервера
	to.sin_family = AF_INET;
	to.sin_port = htons(port);
	to.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	char bto[10] = { *name, *(name + 1), *(name + 2), *(name + 3), *(name + 4), *(name + 5), *(name + 6), *(name + 7), *(name + 8), *(name + 9) };; int ito;
	char bfrom[10]; int ifrom;
	char stop[1] = { '\0' };
	ito = sendto(sS, (char*)& bto, sizeof(bto), NULL, (sockaddr*)& to, sizeof(to));
	if (ito == SOCKET_ERROR)
		throw SetErrorMsgText("SendTo", WSAGetLastError());


	SOCKADDR_IN fromclnt;
	int scl = sizeof(fromclnt);
	ifrom = recvfrom(sS, bfrom, sizeof(bfrom), NULL, (sockaddr*)& fromclnt, &scl);
	if (ifrom == 0)
		throw SetErrorMsgText("RecvFrom", WSAGetLastError());
	cout << bfrom << endl << "\nIP-адрес сервера: " << inet_ntoa(fromclnt.sin_addr) << "\nПорт сервера: " << htons(fromclnt.sin_port) << endl;

	closesocket(sS);
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	SOCKET cC;
	try
	{

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}

		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw  SetErrorMsgText("Socket", WSAGetLastError());
		}

		char call[10] = "BCA"; short port = 2000;
		SOCKADDR_IN from;
		GetServer(call, port, (sockaddr*)& from, (int*)sizeof(from));



		if (closesocket(cC) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("CloseSocket", WSAGetLastError());
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("WSACleanup", WSAGetLastError());
		};
	}
	catch (string errorbfromText)
	{
		cout << endl << errorbfromText << endl;
	}
	system("pause");
	return 0;
}