#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")



int main(int argv, char** argc) {
	//    Initialize the socket
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		std::cerr << "Winsock intialization failed! Exiting" << std::endl;
	}

	//    Create a scocket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Socket failed to create! Exiting" << std::endl;
	}

	//    Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	
	hint.sin_addr.S_un.S_addr = INADDR_ANY;//Could also use inet_pton 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//    Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//    Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
		if (clientSocket == INVALID_SOCKET) {//TODO}

		}
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];
	//memset(host, 0, NI_MAXHOST) or
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << " connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
	}
	//    Close listening socket
	closesocket(listening);
	//    While loop: accept and echo messages back to the client
	char buf[4096];
	char buf2[4096];
	while (1) {
		ZeroMemory(buf, 4096);
		//    Wait for client to send data
		int bytesRecieved = recv(clientSocket, buf, 4096, 0);
		if (bytesRecieved == SOCKET_ERROR) {
			std::cerr << "Error in recv()! Exiting" << std::endl;
			break;
		}
		if (bytesRecieved == 0) {
			std::cout << "The client disconnected! " << std::endl;
			break;
		}

		//    Echo message back to client
		send(clientSocket, buf, bytesRecieved + 1, 0);

	}
	//    Close socket
	closesocket(clientSocket);
    //    Cleanup Winsock
	WSACleanup();



	std::cin.get();
	return 0;
}