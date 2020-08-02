#ifndef NETWORK_HPP
#define NETWORK_HPP


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8080

struct _app_data_ {
	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
};

void NETWORK_create_socket();
int NETWORK_read_data();


#endif