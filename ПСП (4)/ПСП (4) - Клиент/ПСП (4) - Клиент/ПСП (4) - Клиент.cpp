#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#pragma warning( disable : 4996)
using namespace std;

string SetErrorMsgText(string bfrom, int errNum)
{
	string msgText;
	switch (errNum)
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
		//..........���� WSAGetLastError ..........................
	case		WSAEFAULT: msgText = "	��������� �����"; break;
	case		WSAEINVAL: msgText = "	������ � ���������"; break;
	case		WSAEMFILE: msgText = "	������� ����� ������ �������"; break;
	case		WSAEWOULDBLOCK: msgText = "	������ �������� ����������"; break;
	case		WSAEINPROGRESS: msgText = "	�������� � �������� ��������"; break;
	case		WSAEALREADY: msgText = "	�������� ��� �����������"; break;
	case		WSAENOTSOCK: msgText = "	����� ����� �����������"; break;
	case		WSAEDESTADDRREQ: msgText = "	��������� ����� ������������"; break;
	case		WSAEMSGSIZE: msgText = "	��������� ������� �������"; break;
	case		WSAEPROTOTYPE: msgText = "	������������ ��� ��������� ��� ������"; break;
	case		WSAENOPROTOOPT: msgText = "	������ � ����� ���������"; break;
	case		WSAEPROTONOSUPPORT: msgText = "	�������� �� ��������������"; break;
	case		WSAESOCKTNOSUPPORT: msgText = "	��� ������ �� ��������������"; break;
	case		WSAEOPNOTSUPP: msgText = "	�������� �� ��������������"; break;
	case		WSAEPFNOSUPPORT: msgText = "	��� ���������� �� ��������������"; break;
	case		WSAEAFNOSUPPORT: msgText = "	��� ������� �� �������������� ����������"; break;
	case		WSAEADDRINUSE: msgText = "	����� ��� ������������"; break;
	case	WSAEADDRNOTAVAIL: msgText = "		����������� ����� �� ����� ���� �����������"; break;
	case WSAENETDOWN: msgText = "		���� ���������"; break;
	case WSAENETUNREACH: msgText = "		���� �� ���������"; break;
	case WSAENETRESET: msgText = "		���� ��������� ����������"; break;
	case WSAECONNABORTED: msgText = "	����������� ����� �����"; break;
	case WSAECONNRESET: msgText = "		����� �������������"; break;
	case WSAENOBUFS: msgText = "		�� ������� ������ ��� �������"; break;
	case WSAEISCONN: msgText = "		����� ��� ���������"; break;
	case WSAENOTCONN: msgText = "		����� �� ���������"; break;
	case WSAESHUTDOWN: msgText = "		������ ��������� send : ����� �������� ������"; break;
	case WSAETIMEDOUT: msgText = "		���������� ���������� ��������  �������"; break;
	case WSAECONNREFUSED: msgText = "		���������� ���������"; break;
	case WSAEHOSTDOWN: msgText = "		���� � ����������������� ���������"; break;
	case WSAEHOSTUNREACH: msgText = "		��� �������� ��� �����"; break;
	case WSAEPROCLIM: msgText = "		������� ����� ���������"; break;
	case WSASYSNOTREADY: msgText = "		���� �� ��������"; break;
	case WSAVERNOTSUPPORTED: msgText = "		������ ������ ����������"; break;
	case WSANOTINITIALISED: msgText = "		�� ��������� ������������� WS2_32.DLL"; break;
	case WSAEDISCON: msgText = "		����������� ����������"; break;
	case WSATYPE_NOT_FOUND: msgText = "		����� �� ������"; break;
	case WSAHOST_NOT_FOUND: msgText = "		���� �� ������"; break;
	case WSATRY_AGAIN: msgText = "		������������������ ���� �� ������"; break;
	case WSANO_RECOVERY: msgText = "		��������������  ������"; break;
	case WSANO_DATA: msgText = "		��� ������ ������������ ����"; break;
	case WSA_INVALID_HANDLE: msgText = "		��������� ���������� �������  � �������"; break;
	case WSA_INVALID_PARAMETER: msgText = "		���� ��� ����� ���������� � �������"; break;
	case WSA_IO_INCOMPLETE: msgText = "		������ ����� - ������ �� � ���������� ���������"; break;
	case WSA_IO_PENDING: msgText = "		�������� ���������� �����"; break;
	case WSA_NOT_ENOUGH_MEMORY: msgText = "		�� ���������� ������"; break;
	case WSA_OPERATION_ABORTED: msgText = "		�������� ����������"; break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;

	}
	return bfrom + ": " + msgText;
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

	//�������� ���������
	SOCKADDR_IN to; //��������� �������
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
	cout << bfrom << endl << "\nIP-����� �������: " << inet_ntoa(fromclnt.sin_addr) << "\n���� �������: " << htons(fromclnt.sin_port);

	closesocket(sS);
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	SOCKET cC; //������ ��� �������� � �������; ��� �������� �� ������� ������� �������� ����� ����
	try
	{
		//����� �������
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("WSAStartup", WSASYSCALLFAILURE);
		}
		//����������� ������
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw  SetErrorMsgText("Socket", WSAGetLastError());
		}

		char call[10] = "Hel"; short port = 2000;
		SOCKADDR_IN from;
		GetServer(call, port, (sockaddr*)& from, (int*)sizeof(from));


		//�������� ������
		if (closesocket(cC) == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("CloseSocket", WSAGetLastError());
		}
		//���������� �������
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