﻿// ПСП (7) - Сервер.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <ctime>

using namespace std;

string SetErrorMsgText(string msg, int errNum)
{
	string msgText;
	switch (errNum)
	{
	case WSAEINTR: msgText = "WSAEINTR"; break;
	case WSAEACCES: msgText = "WSAEACCES"; break;
		//..........коды WSAGetLastError ..........................
	case WSAEFAULT: msgText = " Ошибочный адрес"; break;
	case WSAEINVAL: msgText = " Ошибка в аргументе"; break;
	case WSAEMFILE: msgText = " Слишком много файлов открыто"; break;
	case WSAEWOULDBLOCK: msgText = " Ресурс временно недоступен"; break;
	case WSAEINPROGRESS: msgText = " Операция в процессе развития"; break;
	case WSAEALREADY: msgText = " Операция уже выполняется"; break;
	case WSAENOTSOCK: msgText = " Сокет задан неправильно"; break;
	case WSAEDESTADDRREQ: msgText = " Требуется адрес расположения"; break;
	case WSAEMSGSIZE: msgText = " Сообщение слишком длинное"; break;
	case WSAEPROTOTYPE: msgText = " Неправильный тип протокола для сокета"; break;
	case WSAENOPROTOOPT: msgText = " Ошибка в опции протокола"; break;
	case WSAEPROTONOSUPPORT: msgText = " Протокол не поддерживается"; break;
	case WSAESOCKTNOSUPPORT: msgText = " Тип сокета не поддерживается"; break;
	case WSAEOPNOTSUPP: msgText = " Операция не поддерживается"; break;
	case WSAEPFNOSUPPORT: msgText = " Тип протоколов не поддерживается"; break;
	case WSAEAFNOSUPPORT: msgText = " Тип адресов не поддерживается протоколом"; break;
	case WSAEADDRINUSE: msgText = " Адрес уже используется"; break;
	case WSAEADDRNOTAVAIL: msgText = " Запрошенный адрес не может быть использован"; break;
	case WSAENETDOWN: msgText = " Сеть отключена"; break;
	case WSAENETUNREACH: msgText = " Сеть не достижима"; break;
	case WSAENETRESET: msgText = " Сеть разорвала соединение"; break;
	case WSAECONNABORTED: msgText = " Программный отказ связи"; break;
	case WSAECONNRESET: msgText = " Связь восстановлена"; break;
	case WSAENOBUFS: msgText = " Не хватает памяти для буферов"; break;
	case WSAEISCONN: msgText = " Сокет уже подключен"; break;
	case WSAENOTCONN: msgText = " Сокет не подключен"; break;
	case WSAESHUTDOWN: msgText = " Нельзя выполнить send : сокет завершил работу"; break;
	case WSAETIMEDOUT: msgText = " Закончился отведенный интервал времени"; break;
	case WSAECONNREFUSED: msgText = " Соединение отклонено"; break;
	case WSAEHOSTDOWN: msgText = " Хост в неработоспособном состоянии"; break;
	case WSAEHOSTUNREACH: msgText = " Нет маршрута для хоста"; break;
	case WSAEPROCLIM: msgText = " Слишком много процессов"; break;
	case WSASYSNOTREADY: msgText = " Сеть не доступна"; break;
	case WSAVERNOTSUPPORTED: msgText = " Данная версия недоступна"; break;
	case WSANOTINITIALISED: msgText = " Не выполнена инициализация WS2_32.DLL"; break;
	case WSAEDISCON: msgText = " Выполняется отключение"; break;
	case WSATYPE_NOT_FOUND: msgText = " Класс не найден"; break;
	case WSAHOST_NOT_FOUND: msgText = " Хост не найден"; break;
	case WSATRY_AGAIN: msgText = " Неавторизированный хост не найден"; break;
	case WSANO_RECOVERY: msgText = " Неопределенная ошибка"; break;
	case WSANO_DATA: msgText = " Нет записи запрошенного типа"; break;
		/*case WSA_INVALID_HANDLE: msgText = " Указанный дескриптор события с ошибкой"; break;
		case WSA_INVALID_PARAMETER: msgText = " Один или более параметров с ошибкой"; break;
		case WSA_IO_INCOMPLETE: msgText = " Объект ввода - вывода не в сигнальном состоянии"; break;
		case WSA_IO_PENDING: msgText = " Операция завершится позже"; break;
		case WSA_NOT_ENOUGH_MEMORY: msgText = " Не достаточно памяти"; break;
		case WSA_OPERATION_ABORTED: msgText = " Операция отвергнута"; break;*/
	case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова"; break;
	default: msgText = "Что-то там"; break;

	}
	return msg + ": " + msgText;
}


int main()
{
	setlocale(LC_ALL, "RU");
	HANDLE mailslot;
	DWORD read;
	try
	{
		mailslot = CreateMailslot(L"\\\\.\\mailslot\\Kostya2", 500,  180 * 1000, NULL); //время в милисекундах
		if (!mailslot)
			throw SetErrorMsgText("CreateMailslot", GetLastError());
		else
			cout << "Mailslot Kostya2 created\n\n";


		clock_t t1 = clock();
		char bfrom[300];
		while (true)
		{
			if (!ReadFile(mailslot, bfrom, sizeof(bfrom), &read, NULL))
				throw SetErrorMsgText("ReadFile", GetLastError());
			if (*bfrom == 's' && *(bfrom+1) == 't' && *(bfrom+2) == 'o' && *(bfrom+3) == 'p' && *(bfrom+4) == '\0')
				break;
			cout << bfrom << endl;
		}
		clock_t t2 = clock();
		cout << "Время: " << (t2 - t1) / 1000 << " секунд.";
		CloseHandle(mailslot);
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}
	system("pause");
}

