#include <iostream>
#include <WinSock2.h>
#pragma comment (lib, "WS2_32.lib")
#pragma warning (disable : 4996)

using namespace std;

string SetErrorMsgText(string msg, int errNum)
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
	return msg + ": " + msgText;
}

//sockaddr* from - ���� � �������
//char* name - ���������� ��� �������

SOCKADDR_IN GetRequestFromClient(char* name, short port, unsigned short &clntport)
{
	//�������� � ��������� ������
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
		//���� ���������
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
		cout << "�������� �����" << endl << endl;

		if (*msg == *servname && *(msg + 1) == *(servname + 1) && *(msg + 2) == *(servname + 2) && *(msg + 3) == *(servname + 3)
			&& *(msg + 4) == *(servname + 4) && *(msg + 5) == *(servname + 5) && *(msg + 6) == *(servname + 6)
			&& *(msg + 7) == *(servname + 7) && *(msg + 8) == *(servname + 8) && *(msg + 9) == *(servname + 9))
		{
			cout << "������� ����" << endl;
			cout << "IP �������: " << inet_ntoa(clnt.sin_addr) << endl;
			cout << "���� �������: " << htons(clnt.sin_port) << endl;
			clntport = htons(clnt.sin_port);
			closesocket(sS);
			return clnt;
		}
	}
}



//struct sockaddr* to - ���������, ����������� �� �������, ������� ����������� � ���������� �-�
//char* name - ��� �������
//�-� ������ ���������� ������� ��� ������� (�.�. name)
bool PutAnswerToClient(char* name, struct sockaddr* to, int* ito)
{
	//�������� � ��������� ������
	SOCKET sS;
	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw SetErrorMsgText("Socket", WSAGetLastError());
	int optval = 1;
	int res = setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)& optval, sizeof(int));
	if (res == SOCKET_ERROR)
		throw SetErrorMsgText("SocketOpt", WSAGetLastError());

	SOCKADDR_IN clnt = (SOCKADDR_IN&)*to;
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
		//����� �������
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("StartUp", WSAGetLastError());

		do
		{
			//���� ����� �������
			SOCKADDR_IN clnt;
			int clntsize = sizeof(clnt);
			char servname[10] = "Hel";
			int servport = 2000;
			unsigned short clntport = 0;
			clnt = GetRequestFromClient(servname, servport, clntport);

			//�������� ����� ������� �������
			SOCKADDR_IN to;
			to.sin_family = AF_INET;
			to.sin_port = htons(clntport);
			to.sin_addr.S_un.S_addr = clnt.sin_addr.S_un.S_addr;
			int size = sizeof(to);
			PutAnswerToClient((char*)& servname, (LPSOCKADDR)& to, (int*)& size);
		} while (true);
		
		//����� �������
		if(WSACleanup() != 0)
			throw SetErrorMsgText("CleanUp", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}
	return 0;
}