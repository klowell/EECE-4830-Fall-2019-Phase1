/*************************************************************************************
File Name: UDPClient.c

Objective: To send an image file, preferably a bitmap(.bmp) to a UDP server process,
and receive a copy of the same file from the same UDP server process.

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

#define IP_PROTOCOL		0
#define IP_ADDRESS		"127.0.0.1"					// localhost 
#define PORT			5566						// Arbitrary port number chosen
#define BUFFER_SIZE		1024						// Arbitrary bufer size, can be 1 - 65507
#define OUTFILE_NAME	"Client_Sent.bmp"			// Name of original file to be sent
#define INFILE_NAME		"Client_Received.bmp"		// Name of file to be received back from server, different file to prove that server really did send it back

int main()
{ 
	FILE* outFile;
	FILE* inFile;
	char buffer[BUFFER_SIZE]; 
	int socketFD; 
	struct sockaddr_in serverAddress;
	socklen_t addressLength;

	socketFD = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);		// Creating client side socket

	if (socketFD < 0)
	{
		printf("File Descriptor Not Received\n");
		return -1;
	}

	memset(&serverAddress, '\0', sizeof(serverAddress));		// Allocating and setting socket adddress parameters

	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(PORT); 
	serverAddress.sin_addr.s_addr = inet_addr(IP_ADDRESS); 

	addressLength = sizeof(serverAddress);						// Calculating size of address type for necessary arguments of UDP functions

	outFile = fopen(OUTFILE_NAME, "r");							// Opening original file to send
	if(outFile == NULL)
	{
		printf("Failed to Open File\n");
		return -1;
	}

	printf("Sending File...\n");																		// Prompting user that program starting the sending process
	send_file_client_to_server(outFile, socketFD, buffer, BUFFER_SIZE, serverAddress, addressLength);	// Sending file to server
	printf("File Sent\n\n");																			// Prompting user that program successfully sent file

	fclose(outFile);											// Closing original file
	outFile = NULL;												// Avoiding dangling pointers by setting file pointer to NULL

	inFile = fopen(INFILE_NAME, "w");							// Creating/opening file to receive from server
	if(inFile == NULL)
	{
		printf("Failed to Open File\n");
		return -1;
	}

	printf("Waiting to Receive File...\n");																	// Prompting user that program starting the receiving process
	receive_file_client_from_server(inFile, socketFD, buffer, BUFFER_SIZE, serverAddress, addressLength);	// Receiving file from server
	printf("File Received\n\n");																			// Prompting user that program successfully received file

	fclose(inFile);												// Closing received file
	inFile = NULL;												// Avoiding dangling pointers by setting file pointer to NULL

	return 0; 
}