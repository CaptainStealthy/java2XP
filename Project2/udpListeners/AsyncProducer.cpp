#include <udpListeners/AsyncProducer.h>
#include <globalSingletons/drefStore.h>
#include <utility/Logger.h>
#include<thread>
#include<ctime>
#include <udpListeners/AsyncProducer.h>
#include <WinSock2.h>
#define SCK_VERSION2 0x0202

void initDrefs();

Logger logger("AsyncProducer");

static drefStore *store = drefStore::getInstance();
AsyncProducer* AsyncProducer::serverInstance = nullptr;
SOCKET asyncSocket;

static struct sockaddr_in asyncReceiver;
static int slen, recv_len;
thread asyncThread;

double time_elapsed = 0.0;
double dt = 0.0;
double prev_time = clock();

void AsyncProducer::startServer()
{

	slen = sizeof(asyncReceiver);

	asyncReceiver.sin_family =AF_INET;
	asyncReceiver.sin_addr.s_addr = inet_addr("127.0.0.1");
	asyncReceiver.sin_port = htons(52009);

	if ((asyncSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		logger.setLogLevel(Logger::log_error);
		logger.logString("Socket failed to initialize : INVALID_SOCKET");
		return;
	}
	else {
		logger.setLogLevel(Logger::log_message);
		logger.logString("Socket successfully created");
	}
	quit = false;
	initDrefs();
	thread t1(&AsyncProducer::worker_thread, getInstance());
	asyncThread.swap(t1);
	asyncThread.detach();
	initialized = true;
}

void AsyncProducer::closeServer()
{
	quit = true;
	initialized = false;
}

void AsyncProducer::worker_thread()
{
	
	
	while (quit == false) {

		dt = (clock() - prev_time) / CLOCKS_PER_SEC;
		prev_time = clock();
		time_elapsed += dt;
		int commandChanged = store->getInt("sendCommand");
		if (commandChanged == 1) {
			
			string commandValue = store->getString("command");
		
			
			int result = sendto(asyncSocket, commandValue.c_str(), strlen(commandValue.c_str()), 0, (struct sockaddr*)&asyncReceiver, sizeof(asyncReceiver));

			store->setInt("sendCommand", 0);
		}
		else {
			if (time_elapsed > 0.095) {
				time_elapsed = 0.0;
				string data = "";
				data += "dataStream," + store->get("planeLat") + "," + store->get("planeLon") + "," + store->get("groundSpeed");
				char bufData[2048];
				bufData[data.length() - 1] = '\0';
				strcpy(bufData, data.c_str());
				int result = sendto(asyncSocket, bufData, strlen(bufData), 0, (struct sockaddr*)&asyncReceiver, sizeof(asyncReceiver));
				if (result == -1) {
					result = WSAGetLastError();
					logger.setLogLevel(Logger::log_error);
					logger.logIntMessage(result, "Failed to send data error code ");
				}
			}
		}
	}

	closesocket(asyncSocket);
	logger.setLogLevel(Logger::log_message);
	logger.logString("Server safely exited thread");
}

void initDrefs() {
	store->add("integer", "mag787/udp/changed", "sendCommand");
	store->add("decimal", "sim/flightmodel/position/latitude", "planeLat");
	store->add("decimal", "sim/flightmodel/position/longitude", "planeLon");
	store->add("decimal", "sim/flightmodel/position/groundspeed", "groundSpeed");
}

AsyncProducer::AsyncProducer()
{
	
}

AsyncProducer::~AsyncProducer()
{
}
