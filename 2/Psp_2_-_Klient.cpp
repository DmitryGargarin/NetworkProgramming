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
	SOCKET cC; //������ ��� �������� � �������; ��� �������� �� ������� ������� �������� ����� ����
	try
	{

		int count;
		cout << "������� ��� ���������� ���������?\t"; cin >> count;
		//����� �������
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}
		//����������� ������
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket", WSAGetLastError());
		}


		//������� � �������
		SOCKADDR_IN srv;
		srv.sin_family = AF_INET;
		srv.sin_port = htons(2000);
		srv.sin_addr.S_un.S_addr = inet_addr("192.168.43.223");
		if (connect(cC, (sockaddr*)& srv, sizeof(srv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Connect", WSAGetLastError());

		//�������� ���������
		char stop[1] = {'\0'};
		char bto[50] = "Hello from Client - ", bfrom[50]; //�� ������� � �������
		char full[50];	
		int ito = 0;
		for (int i = 0; i < count; i++)
		{
			char numb[5];
			const char* temp = itoa(i, numb, 10);
			strcpy(full, bto);
			strcat_s(full, temp);
			if ((ito = send(cC, (char*)& full, sizeof(full), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("Send", WSAGetLastError());
			cout << endl << "����������" <<  endl; 

			//�������� ���������
			if ((ito = recv(cC, (char*)& bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("Recv", WSAGetLastError());
			cout << endl << "��������: " << bfrom << endl;
		}
		if ((ito = send(cC, (char*)& stop, sizeof(stop), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("Send", WSAGetLastError());

		//�������� ������
		if (closesocket(cC) == SOCKET_ERROR)
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