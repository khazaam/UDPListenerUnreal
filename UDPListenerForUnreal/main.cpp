#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace  std; //no extra std:: things


int main()
{
	
//startup Win
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if(wsOk != 0)
	{
		cout << "Cant start Winsock" << wsOk;
		return 0;
	}
	// bind ip etc
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);//Dgram on udp
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; //mikä tahansa ip-osoite
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(9958); //Mikä portti mistä kuunnellaan? Converttaa big endianiksi, muista vaihtaa oikeaan osoitteeseen.
	//
	if (bind(in,(sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR )
	{
		cout << "Cant bind socket" << WSAGetLastError() << endl;
		return 0;
	} 

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);
	
	
	char buf[1024];//
	
	//enter loop
	while(true)
	{
		ZeroMemory(buf, 1024);
		
		int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		if(bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client" << WSAGetLastError() << endl;
			continue;
		}

		char clientIP[256];
		ZeroMemory(clientIP, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

		cout << "Message recv from " << clientIP << " : " << buf << endl;
	}
	
		//show text
		//display message and client info
		//
	//close socket
	closesocket(in);
	//shutdown win stuff
	WSACleanup();

	return 0;
}