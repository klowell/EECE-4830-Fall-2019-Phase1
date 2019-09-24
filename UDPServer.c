/*************************************************************************************
File Name: UDPServer.c

Objective: To accept an image file, preferably a bitmap(.bmp) from a UDP client
process, save said file, and resend it back to the client process.

Created By: Kristopher Lowell
Date Created: 9/20/2019

Time to Complete: Approx 24 hours, accumulative

References: 
	https://www.geeksforgeeks.org/udp-server-client-implementation-c/
	https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/
	http://www.cplusplus.com/reference/cstdio/
	https://www.tutorialspoint.com/c_standard_library/string_h.htm
	https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux

File History:
	Submitted - KCL - 9/23/2019
*************************************************************************************/
#include "UDP_Tx_Rx.h"								// Custom made header file for UDP functions, contains all other necessary include statements

#define IP_PROTOCOL 	0
#define PORT 			5566						// Arbitrary port number chosen
#define BUFFER_SIZE 	1024						// Arbitrary bufer size, can be 1 - 65507
#define OUTFILE_NAME	"Server_Received.bmp"		// Name of file that the server process made by receiving data from client process

int main() 
{ 
	FILE* fp;
	char buffer[BUFFER_SIZE]; 
	int socketFD; 
	struct sockaddr_in serverAddress, clientAddress;
	socklen_t addressLength;
	
	socketFD = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);		// Creating server side socket

	if (socketFD < 0)
	{
		printf("File Descriptor Not Received\n");
		return -1;
	}

	memset(&serverAddress, '\0', sizeof(serverAddress));		// Allocating and setting socket address parameters

	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(PORT); 
	serverAddress.sin_addr.s_addr = INADDR_ANY; 

	if (bind(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) 	// Binding socket to server address
	{
		printf("Binding Failed!\n"); 
		return -1;
	}

	addressLength = sizeof(serverAddress); 						// Calculating size of adddress for necessary arguments of UDP functions

	fp = fopen(OUTFILE_NAME, "w");								// Creating/opening file to receive from client
	if(fp == NULL)
	{
		printf("Failed to Open File\n");
		return -1;
	}

	printf("Waiting to Receive File...\n");																// Prompting user that program starting the receiving process
	receive_file_server_from_client(fp, socketFD, buffer, BUFFER_SIZE, clientAddress, addressLength);	// Receiving file from client
	printf("File Received\n\n");																		// Prompting user that program successfully received file

	fclose(fp);

	fp = fopen(OUTFILE_NAME, "r");								// Closing and opening file to prove program saved a copy of received file
	if(fp == NULL)
	{
		printf("Failed to Open File\n");
		return -1;
	}

	printf("Sending File...\n");																	// Prompting user that program starting the sending process
	send_file_server_to_client(fp, socketFD, buffer, BUFFER_SIZE, clientAddress, addressLength);	// Sending file to client
	printf("File Sent\n\n");																		// Prompting user that program successfully sent file

	fclose(fp);													// Closing file
	fp = NULL;													// Avoiding dangling pointers by setting file pointer to NULL

	return 0; 
}