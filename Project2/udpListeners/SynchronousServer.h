#pragma once
#include <utility/Logger.h>

#include <string>
#include <winsock.h>



using namespace std;
class SynchronousServer
{
public:
	static SynchronousServer *serverInstance;
	static inline SynchronousServer* getInstance() {
		if (!serverInstance) {
			serverInstance = new SynchronousServer();
		}
		return serverInstance;
	}
	void startServer();
	void closeServer();
	void worker_thread();
	bool initialized = false;
	bool hasSentExit();

	void startJVMDebug();

	void startJVMCmd();
private:
	struct sockaddr_in server;
	
	BOOL startJVM();
	void init_server();
	bool waitForExit = true;
	void parse_commands(string command);
	Logger logger = Logger("Sync Server");
	SynchronousServer();
	~SynchronousServer();
	bool quit = true;
	
};

