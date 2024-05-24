#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "connect.hpp"

void print_help() {
    std::cout << "use: vectozavr-shooter [-s/-c] [-n] [--ip] [--port]\n\n"
              << "options:\n"
			  << "  -s            write only server.txt (only argument),\n"
			  << "  -c            write only connect.txt (only argument),\n"
              << "  -n            nickname in game. Standart: PlayerName,\n"
              << "  --ip          ip for connect. Standard: 127.0.0.1,\n"
              << "  --port        port for connect. Standard: 54000.\n";
}

void update_connect(int argc, char *argv[]){
	bool only_server = false;
	bool only_client = false;
	bool write = false;
    std::string nickname = "PlayerName";
    std::string ip = "127.0.0.1";
    std::string port = "54000";

    std::ifstream in("../connect.txt"); 
    if (in.is_open()){
		std::string n,i,p;
		in >> n >> p >> i;
		if(i != "")
			ip = i;
		if(p != "")
			port = p;
		if(n != "")
			nickname = n;
	}

	const char *a[] = {"-n"};
	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "-h") == 0){
			print_help();
		}
		if(strcmp(argv[i], "-s") == 0){
			only_server = true;
		}
		if(strcmp(argv[i], "-c") == 0){
			only_client = true;
		}
		if(argc >= i+1){
			if(strcmp(argv[i], "-n") == 0) {
				nickname = argv[i+1];
				std::cout << "nickname: " << nickname << std::endl;
				write = true;
			}
			if(strcmp(argv[i], "--ip") == 0) {
				ip = argv[i+1];
				std::cout << "ip: " << ip << std::endl;
				write = true;
			}
			if(strcmp(argv[i], "--port") == 0) {
				port = argv[i+1];
				std::cout << "port: " << port << std::endl;
				write = true;
			}
		}
	}
	if(write){
		std::ofstream out("../connect.txt");
		if (out.is_open() && !only_server && write){
	        out << ip << "\n" << port << "\n" << nickname << std::endl;
			std::cout << "sucsessful writing for client config!" << std::endl;
	    }
		out.close();
	    
		std::ofstream serv("../server.txt");
		if (serv.is_open() && !only_client && write){
	        serv << port << std::endl;
			std::cout << "sucsessful writing for server config!" << std::endl;
	    }

	    serv.close();
		if(only_server && only_client){
			std::cout << "not files for write please input -s or -c or  nothing" << std::endl;
		}
	}
}
