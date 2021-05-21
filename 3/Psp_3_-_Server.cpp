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
	case WSAEINTR: text = "WSAEINTR - ������ ������� �������� "; break;
	case WSAEACCES: text = "WSAEACCESS - ���������� ����������"; break;
	case WSAEFAULT: text = "WSAEFAULT - ��������� �����"; break;
	case WSAEINVAL: text = "WSAEINVAL - ������ � ���������"; break;
	case WSAEMFILE: text = "WSAEMFILE - ������� ����� ������ �������"; break;
	case WSAEWOULDBLOCK: text = "WSAEWOULDBLOCK - ������ �������� ����������"; break;
	case WSAEINPROGRESS: text = "WSAEINPROGRESS - �������� � �������� ��������"; break;
	case WSAEALREADY: text = "WSAEALREADY - �������� ��� �����������"; break;
	case WSAENOTSOCK: text = "WSAENOTSOCK - ����������� ����� �����"; break;
	case WSAEDESTADDRREQ: text = "WSAEDESTADDRREQ - ��������� ����� ������������"; break;
	case WSAEMSGSIZE: text = "WSAEMSGSIZE - ��������� ������� �������"; break;
	case WSAEPROTONOSUPPORT: text = "WSAEPROTONOSUPPORT - �������� �� ��������������"; break;
	case WSAEISCONN: text = "WSAEISCONN - ����� ��� ���������"; break;
	case WSAESHUTDOWN: text = "WSAESHUTDOWN - ����� �������� ������"; break;
	case WSA_NOT_ENOUGH_MEMORY: text = "WSA_NOT_ENOUGH_MEMORY - �� ������� ������"; break;
	case WSASYSCALLFAILURE: text = "WSASYSCALLFAILURE - ��������� ���������� ���������� ������"; break;
	default: text = "���-�� ���"; break;

	}
	return msg + ": " + text;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	SOCKET sS; /* ����������� ����� �������*/
	try
	{
		//����� �������
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}
		//����������� ������
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket", WSAGetLastError());
		}

		//�������� ������ � ���� � �����
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(sS, (LPSOCKADDR)& serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Bind", WSAGetLastError());


		//���� ��������� 
		SOCKADDR_IN from; //��������� �������
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

			//�������� ���������
			ito = sendto(sS, (char*)& bto, sizeof(bto), NULL, (sockaddr*)& from, scl);
			if (ito == SOCKET_ERROR)
				throw SetErrorMsgText("SendTo", WSAGetLastError());

		} while (ifrom != 1);

		clock_t t2 = clock();

		cout << "����������� �����: " << (t2 - t1) / 1000 << " ������" << endl;

		//�������� ������	
		if (closesocket(sS) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("CloseSocket", WSAGetLastError());
		}
		//���������� �������
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
