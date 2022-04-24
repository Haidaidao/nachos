#include "syscall.h"

void main() 
{	int pingPID, pongPID;		
	PrintString("Ping-Pong test starting ...\n\n");
	pongPID = Exec("./test/pong");
	pingPID = Exec("./test/Ping");

	Join(pingPID);
	Join(pongPID);
}
