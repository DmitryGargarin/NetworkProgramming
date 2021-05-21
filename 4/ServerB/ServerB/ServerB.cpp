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
SOCKADDR_IN GetRequestFromClient(char* name, short port, unsigned short& clntport)
{

	SOCKET sS;
	memset(&sS, 0, sizeof(sS));
	char servname[10] = { *name, *(name + 1), *(name + 2), *(name + 3), *(name + 4), *(name + 5), *(name + 6), *(name + 7), *(name + 8), *(name + 9) };
	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw SetErrorMsgText("Socket", WSAGetLastError());
	int optval = 1;
	int res = setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)& optval, sizeof(int));
	if (res == SOCKET_ERROR)
		throw SetErrorMsgText("SocketOpt", WSAGetLastError());

	SOCKADDR_IN serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(2000);
	serv.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sS, (LPSOCKADDR)& serv, sizeof(serv)) == SOCKET_ERROR)
		throw SetErrorMsgText("Bind", WSAGetLastError());


	while (true)
	{

		SOCKADDR_IN clnt;
		int sizeclnt = sizeof(clnt);
		char msg[50]; int ito;
		ito = recvfrom(sS, msg, sizeof(msg), NULL, (SOCKADDR*)& clnt, (int*)& sizeclnt);
		if (ito == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)
			{
				throw SetErrorMsgText("Timed out", WSAGetLastError());
			}
			else
				throw SetErrorMsgText("RecvFrom", WSAGetLastError());
		}
		cout << "Проверка имени" << endl << endl;

		if (*msg == *servname && *(msg + 1) == *(servname + 1) && *(msg + 2) == *(servname + 2) && *(msg + 3) == *(servname + 3)
			&& *(msg + 4) == *(servname + 4) && *(msg + 5) == *(servname + 5) && *(msg + 6) == *(servname + 6)
			&& *(msg + 7) == *(servname + 7) && *(msg + 8) == *(servname + 8) && *(msg + 9) == *(servname + 9))
		{
			cout << "Соединение" << endl;
			cout << "IP клиента: " << inet_ntoa(clnt.sin_addr) << endl;
			cout << "Порт клиента: " << htons(clnt.sin_port) << endl;
			clntport = htons(clnt.sin_port);
			closesocket(sS);
			return clnt;
		}
		else
		{
			cout << *name << * (name + 1) << * (name + 2) << * (name + 3) << * (name + 4) << * (name + 5) << * (name + 6) << * (name + 7) << * (name + 8) << * (name + 9) << endl;
			cout << (*msg << *(msg + 1) << *(msg + 2) << *(msg + 3) << *(msg + 4) << *(msg + 5) << *(msg + 6) << *(msg + 7) << *(msg + 8) << *(msg + 9));
		}
	}
}

bool PutAnswerToClient(char* name, struct sockaddr* to, int* ito)
{
	//создание и настройка сокета
	SOCKET sS;
	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw SetErrorMsgText("Socket", WSAGetLastError());
	int optval = 1;
	int res = setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)& optval, sizeof(int));
	if (res == SOCKET_ERROR)
		throw SetErrorMsgText("SocketOpt", WSAGetLastError());

	SOCKADDR_IN clnt = (SOCKADDR_IN&)* to;
	int i;
	i = sendto(sS, name, sizeof(name), NULL, (SOCKADDR*)& clnt, sizeof(clnt));
	if (i == SOCKET_ERROR)
		throw SetErrorMsgText("SendTo", WSAGetLastError());
	closesocket(sS);

}



int main()
{
	setlocale(0, """");
	WSADATA wsaData;
	try
	{

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("StartUp", WSAGetLastError());

		do
		{

			SOCKADDR_IN clnt;
			int clntsize = sizeof(clnt);
			char servname[10] = "BCA";
			int servport = 2000;
			unsigned short clntport = 0;
			clnt = GetRequestFromClient(servname, servport, clntport);

			SOCKADDR_IN to;
			to.sin_family = AF_INET;
			to.sin_port = htons(clntport);
			to.sin_addr.S_un.S_addr = clnt.sin_addr.S_un.S_addr;
			int size = sizeof(to);
			PutAnswerToClient((char*)& servname, (LPSOCKADDR)& to, (int*)& size);
		} while (true);

		if (WSACleanup() != 0)
			throw SetErrorMsgText("CleanUp", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}
	return 0;
}