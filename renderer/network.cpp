#include <network.hpp>

//public variables
int sock;
struct sockaddr_in info;
struct _app_data_ app_data;

void NETWORK_create_socket()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	info.sin_port = htons(PORT);
	info.sin_family = AF_INET;
	info.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (struct sockaddr *)&info, sizeof(info));
}

int NETWORK_read_data()
{
	int x = recv(sock, &app_data, sizeof(app_data), 0);
	return x;
}