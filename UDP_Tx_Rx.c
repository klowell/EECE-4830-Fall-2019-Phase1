/*************************************************************************************
File Name: UDP_Tx_Rx.c

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
#include "UDP_Tx_Rx.h"

unsigned send_file_client_to_server(FILE* outFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength)
{
	unsigned bytesReceived, bytesSent, packetCount = 0, timesResent = 0;

	if(buffer_size > MAX_BUFFER_SIZE)																		// In case function is handed an invalid value for buffer size
		buffer_size = MAX_BUFFER_SIZE;

	strcpy(buffer, "Request");																				// Server should be listening for a request, thus sending a request
	sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);	

	recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);			// Waiting for a reply from the server

	while(!feof(outFile))
	{
		bytesSent = 0;
		for(unsigned i = 0; i < buffer_size && !feof(outFile); i++)											// Filling buffer with data from file
		{
			buffer[i] = fgetc(outFile);
			bytesSent++;																					// Keeping track of how many bytes are to be sent
		}

		packetCount++;																						// Keeping track of how many packets that have been sent

		sendto(socketFD, buffer, bytesSent, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);			// Sending buffer to server
		recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);		// Waiting for server to process buffer before next transmission

		bytesReceived = atoi(buffer);

		if(bytesSent != bytesReceived)																		// Simple error checking
		{																									// If the bytes sent doesn't match how much the server
			fseek(outFile, -bytesSent, SEEK_CUR);															// reports, then that packet gets resent
			packetCount--;
			timesResent++;
			strcpy(buffer, "Repeat");																		// Signalling server that previous packet is to be repeated
			sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);
			recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);	// Waiting for server to process repeat singal before next transmission
		}
	}																										// Repeating until the end of file has been reached

	strcpy(buffer, "End Of File");																			// Sending final packet telling server that's the end of the file
	sendto(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, addressLength);

	return packetCount;																						// Returning how many packets it nominally takes to send the file
}

unsigned send_file_server_to_client(FILE* outFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength)
{
	unsigned bytesReceived, bytesSent, packetCount = 0, timesResent = 0;

	if(buffer_size > MAX_BUFFER_SIZE)																		// In case function is handed an invalid value for buffer size
		buffer_size = MAX_BUFFER_SIZE;

	recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);			// Waiting for a request from the client

	strcpy(buffer, "Reply");
	sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);				// Sending reply to client

	while(!feof(outFile))
	{
		bytesSent = 0;
		for(unsigned i = 0; i < buffer_size && !feof(outFile); i++)											// Filling buffer with data from file
		{
			buffer[i] = fgetc(outFile);
			bytesSent++;																					// Keeping track of how many bytes are to be sent
		}

		packetCount++;																						// Keeping track of how many packets that have been sent

		sendto(socketFD, buffer, bytesSent, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);			// Sending buffer to client

		recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);		// Waiting for client to process buffer before next transmission

		bytesReceived = atoi(buffer);

		if(bytesSent != bytesReceived)																		// Simple error checking
		{																									// If the bytes sent doesn't match how much the client
			fseek(outFile, -bytesSent, SEEK_CUR);															// reports, then that packet gets resent
			packetCount--;
			timesResent++;
			strcpy(buffer, "Repeat");																		// Signalling client that previous packet is to be repeated
			sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);
			recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);	// Wait for client to process repeat singal before next transmission
		}
	}																										// Repeating until the end of file has been reached

	strcpy(buffer, "End Of File");																			// Sending final packet telling client that's the end of the file
	sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);

	return packetCount;																						// Returning how many packets it nominally takes to send the file
}

unsigned receive_file_client_from_server(FILE* inFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength)
{
	unsigned numOfBytes, packetCount = 0, isEOF = 0, timesRepeated = 0;

	if(buffer_size > MAX_BUFFER_SIZE)																		// In case function is handed an invalid value for buffer size
		buffer_size = MAX_BUFFER_SIZE;

	strcpy(buffer, "Request");																				// Server should be listening for a request, thus sending a request
	sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);

	recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);			// Waiting for a reply from the server

	while(!isEOF)
	{
		numOfBytes = recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);		// Receiving buffer from server

		if(strcmp(buffer, "End Of File") != 0)																	// Continuing to expect data until an end of file packet has been received
		{
			if(strcmp(buffer, "Repeat") != 0)																	// If a repeated packet is signalled, then ignoring current packet and waiting for next one
			{
				for(int i = 0; i < numOfBytes; i++)																// Filling file with received data from buffer									
					fputc(buffer[i], inFile);

				packetCount++;																					// Keeping track of how many packets that have been received
				clear_buffer(buffer, buffer_size);
				snprintf(buffer, buffer_size, "%d", numOfBytes);												// Converting how many bytes received into string
				sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);		// Sending the amount of bytes the client received back to server
			}
			else
			{
				fseek(inFile, -numOfBytes, SEEK_CUR);															// Moving pointer in file back to account for repeated packet
				timesRepeated++;																				// Keeping track of how many times packets had to be repeated
				strcpy(buffer, "Ready");
				sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);		// Signalling to server that now ready to receive repeated packet
			}
		}
		else																								// Once an end of file packet has been received, finish operation
			isEOF = 1;
	}
	
	return packetCount;																						// Returning how many packets it took to nominally receive file
}

unsigned receive_file_server_from_client(FILE* inFile, int socketFD, char* buffer, unsigned buffer_size, struct sockaddr_in socketAddress, socklen_t addressLength)
{
	unsigned numOfBytes, packetCount = 0, isEOF = 0, timesRepeated = 0;

	if(buffer_size > MAX_BUFFER_SIZE)																		// In case function is handed an invalid value for buffer size
		buffer_size = MAX_BUFFER_SIZE;

	recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);			// Waiting for a request from the client

	strcpy(buffer, "Reply");
	sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);				// Sending reply to the client
	
	while(!isEOF)
	{
		numOfBytes = recvfrom(socketFD, buffer, buffer_size, MSG_WAITALL, (struct sockaddr*)&socketAddress, &addressLength);		// Receive buffer from client

		if(strcmp(buffer, "End Of File") != 0)																	// Continuing to expect data until an end of file packet has been received
		{
			if(strcmp(buffer, "Repeat") != 0)																	// If a repeated packet is signalled, ignoring current packet and waiting for next one
			{
				for(int i = 0; i < numOfBytes; i++)																// Filling file with received data from buffer									
					fputc(buffer[i], inFile);

				packetCount++;																					// Keeping track of how many packets that have been received
				clear_buffer(buffer, buffer_size);
				snprintf(buffer, buffer_size, "%d", numOfBytes);												// Converting how many bytes received into string
				sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);		// Sending the amount of bytes the server received back to client
			}
			else
			{
				fseek(inFile, -numOfBytes, SEEK_CUR);															// Moving pointer in file back to account for repeated packet
				timesRepeated++;																				// Keeping track of how many times packets had to be repeated
				strcpy(buffer, "Ready");
				sendto(socketFD, buffer, buffer_size, MSG_CONFIRM, (struct sockaddr*)&socketAddress, addressLength);		// Signalling to client that now ready to receive repeated packet
			}
		}
		else																								// Once an end of file packet has been received, finish operation
			isEOF = 1;
	}
	
	return packetCount;																						// Returning how many packets it took to nominally receive file
}

void clear_buffer(char* buffer, unsigned buffer_size)
{ 
	if(buffer_size > MAX_BUFFER_SIZE)					// In case function is handed an invalid value for buffer size
		buffer_size = MAX_BUFFER_SIZE;				

	for (int i = 0; i < buffer_size; i++)				// Clearing data by filling buffer with null ('\0') characters
		buffer[i] = '\0'; 								// Also helps formatting character buffer as a string
}