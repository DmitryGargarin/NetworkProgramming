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

int main()
{
	setlocale(0, "");
	WSADATA wsaData;
	SOCKET sS;
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

		//привязка сокета к IP и порту
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(sS, (LPSOCKADDR)& serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Bind", WSAGetLastError());

		while (true)
		{
			if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
				throw SetErrorMsgText("Listen", WSAGetLastError());

			//Разрешение подключения
			SOCKADDR_IN clnt; //информация о клиенте
			SOCKET cS; //сокет для подключения клиента; канал передачи
			memset(&clnt, 0, sizeof(clnt));
			int lenCl = sizeof(clnt);
			cS = accept(sS, (sockaddr*)& clnt, &lenCl);
			if (cS == INVALID_SOCKET)
				throw SetErrorMsgText("Accept", WSAGetLastError());

			cout << "Создано подключение \n  IP клиента: " << inet_ntoa(clnt.sin_addr) << endl;
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

			cout << "Затраченное время: " << (t2 - t1) / 1000 << " секунд" << endl;


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
