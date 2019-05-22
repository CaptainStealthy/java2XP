#include "SynchronousServer.h"
#include <thread>


#include  <WinSock2.h>
#include <globalSingletons/drefStore.h>
#include <XPLMDataAccess.h>
#include <globalSingletons/GlobalStore.h>
#include <vector>



#define BUFLEN 1024
#define PORT 52010
#define SCK_VERSION2 0x0202

using namespace std;
SynchronousServer* SynchronousServer::serverInstance = nullptr;
static drefStore *store = drefStore::getInstance();
GlobalStore *globalVars = GlobalStore::getInstance();



SOCKET s;
struct sockaddr_in server, si_other;
int slen, recv_len;
char buf[BUFLEN];
WSADATA wsa;

thread worker;

void initializeDatarefs();

string displayElementsFmc1[] = { 
	//all the buttons
	"B1LS","B2LS","B3LS","B4LS","B5LS","B6LS","B1RS","B2RS","B3RS","B4RS","B5RS","B6RS",
	//take in all the statuses for the text boxes
	"text1LEdit","text2LEdit","text3LEdit","text4LEdit","text5LEdit","text1REdit","text2REdit","text3REdit","text4REdit","text5REdit",
	"title",
	//left side
	"left1A","left1B","left2A","left2B","left3A","left3B","left4A","left4B","left5A","left5B","left6",
	//right side
	"right1A","right1B","right2A","right2B","right3A","right3B","right4A","right4B","right5A","right5B","right6",
	//last cleanup
	"command","panelState"
};

string efbTakeoffRefs[]= {
	"efbTakeOffWt", "efbOat", "efbToFlaps", "efbRwyCond",  "altitude","efbWind","efbCG","efbSelTemp","efbToRtg"
};

string efbCalculationRefs[] = {
	"calcV1","calcVr","calcV2","calcN1","calcTrim"
};


void SynchronousServer::startServer()
{
	waitForExit = true;
	init_server();
	initializeDatarefs();
	SynchronousServer* currentInstance = SynchronousServer::getInstance();
	quit = false;

	bool cmdMode = globalVars->getProperty("debugMode") == "1" ? true : false;

	if (cmdMode) {
		thread t2(&SynchronousServer::startJVMCmd, currentInstance);
		t2.detach();
	}
	else {
		startJVMDebug();
	}
	
	thread t1(&SynchronousServer::worker_thread,currentInstance);
	worker.swap(t1);
	worker.detach();
	initialized = true;
}

void SynchronousServer::closeServer()
{
	initialized = false;
	quit = true;
}

int SynchronousServer::startJVM()
{
	string command = "\"" + globalVars->getProperty("dllDirectory") + "javaUDP\\udpConnect.jar\"";

	SHELLEXECUTEINFO ExecuteInfo;

	memset(&ExecuteInfo, 0, sizeof(ExecuteInfo));

	ExecuteInfo.cbSize = sizeof(ExecuteInfo);
	ExecuteInfo.fMask = 0;
	ExecuteInfo.hwnd = 0;
	ExecuteInfo.lpVerb = "open";
	ExecuteInfo.lpFile = command.c_str();
	ExecuteInfo.lpParameters = NULL;
	ExecuteInfo.lpDirectory = 0;
	ExecuteInfo.nShow = SW_HIDE;
	ExecuteInfo.hInstApp = 0;

	logger.setLogLevel(Logger::log_message);
	logger.logString("Attempting to start JVM at: " + command);
	BOOL success = ShellExecuteEx(&ExecuteInfo);
	

	if (success == FALSE) {
		int error = GetLastError();
		string errorStr = "Failed to start JVM with error code: ";
		logger.setLogLevel(Logger::log_error);
		logger.logIntMessage(error, errorStr);
	}
	else {
		logger.setLogLevel(Logger::log_message);
		logger.logString("JVM started succesfully!");
	}
	return success;
}




void SynchronousServer::worker_thread() {
	logger.setLogLevel(Logger::log_message);
	logger.logString("Entering worker thread.");
	while (!quit) {
		recvfrom(s, buf, 1024, 0, (struct sockaddr *) &si_other, &slen);
		parse_commands(string(buf));
	}
	sendto(s, "exitCode1", strlen("exitCode1"), 0, (struct sockaddr*) &si_other, slen);

	closesocket(s);
	WSACleanup();
	logger.setLogLevel(Logger::log_message);
	logger.logString("Closing synchronous server.");

	waitForExit = false;

}
bool SynchronousServer::hasSentExit()
{
	return waitForExit;
}

void SynchronousServer::startJVMCmd() {

	string command = "java -jar \"" + globalVars->getProperty("dllDirectory") + "javaUDP\\udpConnectDebug.jar\" &";

	system(command.c_str());
}

void SynchronousServer::startJVMDebug()
{
	string command = "java -jar \"" + globalVars->getProperty("dllDirectory") + "javaUDP\\udpConnect.jar\" &";
	
	WinExec(command.c_str(), SW_HIDE);
	
}
void send_dref(string name) {
	string value = store->get(name);
	char command[BUFLEN];
	strcpy(command, value.c_str());
	
	int result = sendto(s, command, strlen(command), 0, (struct sockaddr*) &si_other, slen);
	result = result;
}

void set_dref(string name) {
	vector<string> command;
	command.push_back("SDREF");
	command.push_back(name);

	recvfrom(s, buf, 512, 0, (struct sockaddr *) &si_other, &slen);
	command.push_back(string(buf));
	store->set(command);
}


void handleFMCMessage() {
	set_dref("messageString");
	int taken = store->getInt("messageTaken");
	while (taken == 0) {
		taken = store->getInt("messageTaken");
	}

	store->setInt("messageTaken", 0);
}

void handleEFBDataSend() {
	for (string str : efbTakeoffRefs) {
		send_dref(str);
	}
}

void setTakeoffDataEFB() {
	for (string str : efbCalculationRefs) {
		set_dref(str);
	}
	store->setInt("utilCommand", 1);
}


void setFmcOneDisp() {

	for (string str : displayElementsFmc1) {
		set_dref(str);
	}

}

void handleDrefCreation() {
	vector<string> commandVector = globalVars->split(std::string(buf), ",");
	store->add(commandVector);
}

void handleDrefSet() {
	vector<string> commandVector = globalVars->split(std::string(buf), ",");
	store->set(commandVector);
}


void return_command() {
	send_dref("command");
	send_dref("panelState");
}

void set_scratch() {
	set_dref("scratchPad");
	store->setString("command", "nothing");
}






void SynchronousServer::init_server() {
		slen = sizeof(si_other);
	
		
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			logger.setLogLevel(Logger::log_error);
			string message = "Socket initialization failed with error code: ";
			int error = WSAGetLastError();
			logger.logIntMessage(error, message);
		}

		if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		{
			logger.setLogLevel(Logger::log_error);
			string message = "Socket initialization failed :INVALID_SOCKET ";
			logger.logString(message);
		}
		
		//Prepare the sockaddr_in structure
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(PORT);

		

		if (::bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logger.setLogLevel(Logger::log_error);
			string message = "Socket initialization failed to bind: SOCKET_ERROR ";
			logger.logString(message);
		}

	
}


void initializeDatarefs() {
	store->add("string", "fmcCommand", "command");
	store->add("string", "panelstate", "panelState");
	store->add("string", "jLabel48", "title");
	store->add("string", "scratch", "scratchPad");
	store->add("integer", "aero787/utility/command", "utilCommand");
	store->add("string", "jLabel51","left1A");
	store->add("string", "jLabel52","left1B");
	store->add("string", "jLabel49","left2A");
	store->add("string", "jLabel50","left2B");
	store->add("string", "jLabel54","left3A");
	  store->add("string", "jLabel53","left3B");
	 store->add("string", "jLabel55","left4A");
	  store->add("string", "jLabel56","left4B");
	  store->add("string", "jLabel58","left5A");
	  store->add("string", "jLabel59","left5B");
	  store->add("string", "jLabel60","left6");
	  store->add("string", "jLabel61","right1A");
	  store->add("string", "jLabel62","right1B");
	  store->add("string", "jLabel63","right2A");
	  store->add("string", "jLabel64","right2B");
	  store->add("string", "jLabel65","right3A");
	  store->add("string","jLabel66","right3B");
	  store->add("string", "jLabel67","right4A");
	  store->add("string", "jLabel68","right4B");
	  store->add("string", "jLabel69","right5A");
	  store->add("string", "jLabel70","right5B");
	  store->add("string", "jLabel57","right6");
	  store->add("string", "button1LEn", "B1LS");
	  store->add("string", "button2LEn","B2LS");
	  store->add("string", "button3LEn","B3LS");
	  store->add("string", "button4LEn","B4LS");
	  store->add("string", "button5LEn","B5LS");
	  store->add("string", "button6LEn","B6LS");
	  store->add("string", "button1REn","B1RS");
	  store->add("string", "button2REn","B2RS");
	  store->add("string", "button3REn","B3RS");
	  store->add("string", "button4REn","B4RS");
	  store->add("string", "button5REn","B5RS");
	  store->add("string", "button6REn","B6RS");
	  store->add("decimal", "sim/flightmodel/weight/m_fuel_total","fuel");
	  store->add("decimal", "sim/cockpit2/gauges/indicators/altitude_ft_pilot","altitude");
	  store->add("decimal", "sim/weather/temperature_ambient_c","AmbientTempOAT");
	  store->add("string", "text1LEdit","text1LEdit");
	  store->add("string", "text2LEdit","text2LEdit");
	  store->add("string", "text3LEdit","text3LEdit");
	  store->add("string", "text4LEdit","text4LEdit");
	  store->add("string", "text5LEdit","text5LEdit");
	  store->add("string", "text1REdit","text1REdit");
	  store->add("string", "text2REdit", "text2REdit");
	  store->add("string", "text3REdit","text3REdit");
	  store->add("string", "text4REdit","text4REdit");
	  store->add("string", "text5REdit","text5REdit");
	  store->add("integer", "vSpeedsUplink","vSpeedsFont");
	  store->add("string", "mag787/efb/efbDepICAO","efbDepIcao");
	  store->add("string", "mag787/efb/efbDepRwy","efbDepRunway");
	  store->add("string", "mag787/efb/efbDestICAO","efbDestIcao");
	  store->add("string", "mag787/efb/TOWt","efbTakeOffWt");
	  store->add("string", "mag787/efb/efbCG","efbCG");
	  store->add("string", "mag787/efb/runwayCond","efbRwyCond");
	  store->add("string", "mag787/efb/efbOat","efbOat");
	  store->add("string", "mag787/efb/efbSelTemp","efbSelTemp");
	  store->add("string", "mag787/efb/efbIceCond","efbAntiIce");
	  store->add("string", "mag787/efb/flaps","efbToFlaps");
	  store->add("string", "mag787/efb/toDR","efbToRtg");
	  store->add("string", "mag787/efb/windDirection","efbWind");
	  store->add("string", "mag787/calc/v1","calcV1");
	  store->add("string", "mag787/calc/v2","calcV2");
	  store->add("string", "mag787/calc/vr","calcVr");
	  store->add("string", "mag787/calc/n1","calcN1");
	  store->add("string", "mag787/calc/stabTakeOff","calcTrim");
	  store->add("string", "mag787/udp/legsCommand","legsCommand");
	  store->add("string", "mag787/udp/legsString","legsString");
	  store->add("string", "mag787/udp/legsControl","legsControl");
	  store->add("string", "mag787/udp/legsDelivered","legsDelivered");
	  store->add("string", "mag787/ND/plnFocus","wptPlnFocus");
	  store->add("string", "mag787/udp/messageString","messageString");
	  store->add("string", "mag787/udp/messageCommand","messageCommand");
	  store->add("string", "mag787/udp/messageTaken","messageTaken");
	  store->add("string", "mag787/ND/rte1Act","rte1Act");
	  store->add("string", "execEn","execOn");
	  store->add("string", "mag787/settings/acftUnits","acftUnits");
	  store->add("string", "mag787/efb/docs/pdfDir", "pdfDirectory");
	  store->add("integer", "mag787/efb/docs/viewer/converted", "pdfLoaded");
	  
}

void SynchronousServer::parse_commands(string command) {

	if (command == "returnCommand") {
		return_command();
	}
	else if (command == "setScratch") {
		set_scratch();
	}
	else if (command == "returnScratch") {
		send_dref("scratchPad");
	}
	else if (command == "setDisplay") {
		setFmcOneDisp();
	}
	else if (command.find("CDREF") != string::npos) {
		handleDrefCreation();
	}
	else if (command.find("SDREF") != string::npos) {
		handleDrefSet();
	}
	else if (command.find("GDREF") != string::npos) {
		vector<string> commandVector = globalVars->split(std::string(buf), ",");
		send_dref(commandVector.at(1));
	}
	else if (command == "returnPDF") {
		logger.setLogLevel(Logger::log_message);
		logger.logString("Starting PDF load through JVM");
		send_dref("pdfDirectory");
		store->setString("command", "nothing");
	}
	else if (command == "pdfLoaded") {
		store->setInt("pdfLoaded", 1);
		logger.setLogLevel(Logger::log_message);
		logger.logString("Successfully loaded PDF");
	}
	else if (command == "addMessage") {
		handleFMCMessage();
	}
	else if (command == "returnTakeOffData") {
		logger.setLogLevel(Logger::log_debug);
		logger.logString("Returning Takeoff params to JVM");
		handleEFBDataSend();
		store->setString("command", "nothing");
	}
	else if (command == "setTakeOffData") {
		logger.setLogLevel(Logger::log_debug);
		logger.logString("Receiving Takeoff data to JVM");
		setTakeoffDataEFB();
		store->setString("command", "nothing");
	}


}

SynchronousServer::SynchronousServer()
{
	
}



SynchronousServer::~SynchronousServer()
{
	quit = true;
}


