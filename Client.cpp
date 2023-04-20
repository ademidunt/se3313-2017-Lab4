#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "thread.h" // import thread.h

#include <string.h> // import
#include <stdio.h> // import
#include <fcntl.h> // import

using namespace Sync;

#define MAX_LEN 200
#define NUM_COLORS 6

bool exit_flag=false;

std::string def_col="\033[0m";
std::string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

std::string color(int code);
int eraseText(int cnt);


class ClientThread : public Thread{
	private:
		Socket& socket;
		bool &alive;
		// Data
		ByteArray data;
		ByteArray name;
		std::string dataString;
		std::string nameString;
	public:
		// Constructor
		ClientThread(Socket& socket, bool &alive):socket(socket), alive(alive){}
		// Destructor
		~ClientThread(){}
	virtual long ThreadMain(){
		std::cout << "What is your name: ";
		std::cout.flush();
		std::getline(std::cin, nameString);
		name = ByteArray(nameString);
		socket.Write(name);
		std::cout<<colors[NUM_COLORS-1]<<"\n\t  ====== Welcome to the chat-room ======   "<<std::endl<<def_col;

		// fcntl(socket.GetHandle(), F_SETFL, O_NONBLOCK);

		
		while(true){
			// try {
				std::cout << "You: ";
				std::cout.flush();
				std::getline(std::cin, dataString);
				data = ByteArray(dataString);

				if(dataString == "done") {
					std::cout<<"You have disconnected"<<std::endl;
					socket.Write(data);
					alive = false;
					break;
				}

				// Write to the server
				socket.Write(data);

				// Get the response
				// int connection = socket.Read(data);

				// if(connection == 0) {
				// 	alive = false;
				// 	break;
				// }

				// std::cout<<"Server Response: " << data.ToString() << std::endl;
				// eraseText(6);
				// std::cout<<data.ToString() << std::endl;
				// std::cout << "You: ";
			// } 
			// catch (std::string err){
			// 	std::cout<<err<<std::endl;
			// }
		}
		
		return 0;
	}
};

class ServerListenerThread : public Thread {
	private:
		Socket& socket;
		bool& alive;
		ByteArray res;
	public:
		ServerListenerThread(Socket& socket, bool& alive) : socket(socket), alive(alive) {}
	virtual long ThreadMain() {
		while (socket.Read(res)) {
			// try {
			// 	// Read data from the server
			// 	int connection = socket.Read(res);

			// 	if (connection == 0) {
			// 		alive = false;
			// 		break;
			// 	}

				// Print the received message to the console
				std::cout << "Server: " << res.ToString() << std::endl;
			// }
			// catch (std::string err) {
			// 	std::cout << err << std::endl;
			// }
		}
		return 0;
	}
};

int main(void)
{
	std::cout << "I am a client" << std::endl;
	bool alive = true;
	
	// Create socket
	Socket socket("127.0.0.1", 3000);
	ClientThread clientThread(socket, alive);
	ServerListenerThread listenerThread(socket, alive);
	socket.Open();
	
	// clientThread.Start();
	// listenerThread.Start();

	while (alive) {
		sleep(1);
	}

	socket.Close();
	
	return 0;
}

std::string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Erase text from terminal
int eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		std::cout<<back_space;
	}	
}
