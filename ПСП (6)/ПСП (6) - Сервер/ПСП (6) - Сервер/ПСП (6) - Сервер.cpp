// ПСП (6) - Сервер.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <ctime>
#pragma warning (disable : 4996)

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
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;

	}
	return msg + ": " + msgText;
}


int main()
{
	setlocale(LC_ALL, "RU");
	HANDLE pipe;
	DWORD written, read;
	try
	{
		pipe = CreateNamedPipe(
			L"\\\\.\\pipe\\Kostas", 
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
			1, 
			NULL, 
			NULL, 
			INFINITE, 
			NULL);
		if (pipe == INVALID_HANDLE_VALUE)
			throw SetErrorMsgText("CreateNamedPipe", GetLastError());
		if (!ConnectNamedPipe(pipe, NULL))
			throw SetErrorMsgText("ConnectNamedPipe", GetLastError());
		else
			cout << "Connected" << endl << endl;

		//Создание канала и подключение к нему
		while (true)
		{
			ConnectNamedPipe(pipe, NULL);
			clock_t t1 = clock();
			while (true)
			{
				//Чтение строки из канала
				char bto[50], bfrom[50];
				if (!ReadFile(pipe, bfrom, sizeof(bfrom), &read, NULL))
					throw SetErrorMsgText("ReadFile", GetLastError());
				if (bfrom == "")
					break;
				cout << bfrom << endl;

				//Отправка сообщения
				if (!WriteFile(pipe, bfrom, sizeof(bfrom), &written, NULL))
					break;
				DisconnectNamedPipe(pipe);
				ConnectNamedPipe(pipe, NULL);
			}
			clock_t t2 = clock();

			cout << "Затраченное время: " << (t2 - t1) / 1000 << " секунд\n\n\n\n\n\n";

			DisconnectNamedPipe(pipe);


		}
		//Отключение от канала
		CloseHandle(pipe);
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}
}
