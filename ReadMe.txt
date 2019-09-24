---------------------------------------------------------------------------------------------------------------
File Name:
	ReadMe.txt

---------------------------------------------------------------------------------------------------------------
Name: 
	Kristopher Lowell

---------------------------------------------------------------------------------------------------------------
Files: 

	Client_Sent.bmp:
		Original bitmap image file that the simulated UDP client sends to the server.  
		For proper operation, ensure this file is in the same folder as the client executable.

	Design.doc:
		Design document that more closely explores the purposes of the functions and data types used
		in the assignment.  
		Not necessary for proper operation.

	ReadMe.txt:
		This text file explaining the name of the programmer that completed this assignment as well
		as listing the included files and purposes of each file. 
		Not necessary for proper operation, but gives instruction for proper operation.

	Makefile:
		A file used to ease the process in compiling the proper code.  Use (without the quotes)
		"make server" to create the simulated UDP server object and executable.  Use (without the quotes) 
		"make client" to make the simulated UDP client object and executable. Use (without the quotes) 
		"make clean" to delete the UDP server and client objects and executables.
		Not absolutely necessary for proper operation, but eliminates need for compilation command knowledge.
		If used, ensure a copy is included with the UDPServer.c and UDPClient.c.

	UDPServer.c:
		Source code file for the simulated UDP server. Will make a copy of a bitmap file sent to it via UDP
		from a client and send it back to the client. Written in C.
		For proper operation, ensure UDP_Tx_Rx.h and UDP_Tx_Rx.c are located in same folder.

	UDPClient.c:
		Source code file for the simulated UDP client.  Will send a bitmap file to a server via UDP and then
		make a copy of a file sent to it from the server. Written in C.
		For proper operation, ensure UDP_Tx_Rx.h and UDP_Tx_Rx.c are located in same folder.

	UDP_Tx_Rx.h:
		Header file for custom made functions relating to UDP transmission and reception. Written in C.
		For proper operation, ensure a copy is located in the same folder as UDPServer.c and a copy is 
		located in the same folder as UDPClient.c.

	UDP_Tx_Rx.c:
		Source file for custom made functions relating to UDP transmission and reception. Written in C.
		For proper operation, ensure a copy is located in the same folder as each instance of UDP_Tx_Rx.h.

---------------------------------------------------------------------------------------------------------------
Instructions for Proper Operation:

	1. Seperate files into folders as desired, keeping following rules in mind:
		Client_Sent.bmp in same folder as UDPClient.c
		Makefile in same folder as UDPServer.c
		Makefile in same folder as UDPClient.c 			(Make copies of Makefile if necessary)
		UDP_Tx_Rx.h in same folder as UDPServer.c
		UDP_Tx_Rx.h in same folder as UDPClient.c 		(Make copies of UDP_Tx_Rx.h if necessary)
		UDP_Tx_Rx.c in same folder as UDPServer.c
		UDP_Tx_Rx.c in same folder as UDPClient.c 		(Make copies of UDP_Tx_Rx.c if neccessary)

	2. Open Terminal
	3. Change directory to where UDPServer.c is located
	4. Type (without the quotes) "make server" and press Enter key to compile the simulated UDP server code
	5. Type (without the quotes) "./Server" and press Enter key to start executing simulated UDP serer code
	6. Open another Terminal
	7. Change directory to where UDPClient.c is located
	8. Type (without the quotes) "make client" and press Enter key to compile the simulated UDP client code
	9. Type (without the quotes) "./Client" and press Enter key to start executing simulated UDP client code
	10. Both terminals should be freed up as both programs complete execution.

---------------------------------------------------------------------------------------------------------------
Result of Proper Execution:
	
	a. A new bitmap image labeled "Server_Received.bmp" in the same folder as UDPServer.c
		i. Should be an exact copy of "Client_Sent.bmp"
	b. A new bitmap image labeled "Client_Received.bmp" in the same folder as UDPClient.c
		i. Should be an exact copy of "Client_Sent.bmp" or at least "Server_Received.bmp"

---------------------------------------------------------------------------------------------------------------