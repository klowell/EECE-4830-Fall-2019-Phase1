/*************************************************************************************
File Name: UDP_Tx_Rx.h

Objective: Provide functions to send and receive a file between client and server
in an UDP environment

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
#ifndef	UDP_TX_RX
#define UDP_TX_RX

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <ctype.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAX_BUFFER_SIZE 65507	// Maximum buffer size for any UDP operation.


// ****************************************************************************************************
// SEND_FILE_CLIENT_TO_SERVER
//
// Purpose: Sends a file from a UDP client to a UDP server
//
// Arguments:
//	outFile - pointer to file to be sent
//	socketFD - file descriptor for socket to be used to complete UDP operations
//	buffer - character array in which will be sent to and from client to exhange information
//	buffer_size - size of buffer in bytes
//	socketAddress - server's address within socket
//	addressLength - size of server's address
//
// ****************************************************************************************************
unsigned send_file_client_to_server(FILE* outFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength);


// ****************************************************************************************************
// SEND_FILE_SERVER_TO_CLIENT
//
// Purpose: Sends a file from a UDP server to a UDP client
//
// Arguments:
//	outFile - pointer to file to be sent
//	socketFD - file descriptor for socket to be used to complete UDP operations
//	buffer - character array in which will be sent to and from server to exhange information
//	buffer_size - size of buffer in bytes
//	socketAddress - client's address within socket
//	addressLength - size of client's address
//
// ****************************************************************************************************
unsigned send_file_server_to_client(FILE* outFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength);


// ****************************************************************************************************
// RECEIVE_FILE_CLIENT_FROM_SERVER
//
// Purpose: Receives a file from a UDP server
//
// Arguments:
//	inFile - pointer to file in which incoming file will be saved to
//	socketFD - file descriptor for socket to be used to complete UDP operations
//	buffer - character array in which will be sent to and from client to exhange information
//	buffer_size - size of buffer in bytes
//	socketAddress - server's address within socket
//	addressLength - size of server's address
//
// ****************************************************************************************************
unsigned receive_file_client_from_server(FILE* inFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength);


// ****************************************************************************************************
// RECEIVE_FILE_SERVER_FROM_CLIENT
//
// Purpose: Receives a file from a UDP client
//
// Arguments:
//	inFile - pointer to file in which incoming file will be saved to
//	socketFD - file descriptor for socket to be used to complete UDP operations
//	buffer - character array in which will be sent to and from client to exhange information
//	buffer_size - size of buffer in bytes
//	socketAddress - client's address within socket
//	addressLength - size of client'ss address
//
// ****************************************************************************************************
unsigned receive_file_server_from_client(FILE* inFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength);


// ****************************************************************************************************
// CLEAR_BUFFER
//
// Purpose: Remove all data within a character array (buffer) by filling with null characters '\0'
//
// Arguments:
//	buffer - character array to clear data from
//	buffer - size of buffer in bytes
//
// ****************************************************************************************************
void clear_buffer(char* buffer, unsigned buffer_size);

#endif