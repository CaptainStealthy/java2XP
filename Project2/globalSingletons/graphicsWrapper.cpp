#include "graphicsWrapper.h"

#include <thread>
#include <vector>
#include <map>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
/**
THIS CLASS IS RESPONSIBLE FOR WRAPPING THE SASL GRAPHICS API FOR USE THROUGH UDP COMMANDS

The following functions will be implemented:
sasl.gl.drawLine ( number x1 , number y1 , number x2 , number y2 , Color color )

sasl.gl.drawWideLine ( number x1 , number y1 , number x2 , number y2 , number thickness , Color color )

sasl.gl.drawPolyLine ( table points , Color color )

sasl.gl.drawWidePolyLine ( table points , number thickness , Color color )

sasl.gl.drawTriangle ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , Color color )

sasl.gl.drawRectangle ( number x, number y, number width , number height , Color color )

sasl.gl.drawFrame ( number x, number y, number width , number height , Color color )

sasl.gl.setLinePattern ( table pattern )

sasl.gl.drawLinePattern ( number x1 , number y1 , number x2 , number y2 , boolean savePatternState , Color color )

sasl.gl.drawPolyLinePattern ( table points , Color color )

sasl.gl.drawBezierLineQ ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , number parts , Color color )	

sasl.gl.drawWideBezierLineQ ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , number parts , float thickness , Color color)

sasl.gl.drawBezierLineQAdaptive ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , Color color )

sasl.gl.drawWideBezierLineQAdaptive ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , float thickness , Color color )

sasl.gl.drawBezierLineC ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , number x4 , number y4 , number parts , Color color )

sasl.gl.drawWideBezierLineC ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , number x4 , number y4 , number parts , float thickness , Color color )

sasl.gl.drawBezierLineCAdaptive ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , number x4 , number y4 , Color color )

sasl.gl.drawWideBezierLineCAdaptive ( number x1 , number y1 , number x2 , number y2 , number x3 , number y3 , number x4 , number y4 , float thickness , Color color )

sasl.gl.drawCircle ( number x, number y, number radius , boolean isFilled , Color color )

sasl.gl.drawArc ( number x, number y, number radiusInner , number radiusOuter , number startAngle , number arcAngle , Color color )
**/



std::thread threadAPI;

static bool threadRun = false;
bool commandsPending = false;
std::vector<std::vector<std::string>> drawBundles;
std::vector<std::string> temporaryBuffer;

graphicsWrapper::graphicsWrapper()
{
	
}

void graphicsWrapper::addDrawCommand(std::string command) {
	temporaryBuffer.push_back(command);
	commandsPending = true;
}

void graphicsWrapper::createBundle() {
	temporaryBuffer.clear();
}

void graphicsWrapper::endBundle() {
	std::vector<std::string> actualCommands;
	for (int i = 0; i < temporaryBuffer.size(); i++) {
		actualCommands.push_back(std::string(temporaryBuffer.at(i)));
	}
	temporaryBuffer.clear();
	drawBundles.push_back(actualCommands);
}

void graphicsWrapper::startGraphicsAPI() {
	threadRun = true;
	enabled = true;
	std::thread starter(transmitToLua);
	threadAPI.swap(starter);
	threadAPI.detach();
}

void graphicsWrapper::stopGraphicsAPI() {
	threadRun = false;
	enabled = false;
}
 


void graphicsWrapper::transmitToLua() {
	XPLMDataRef commandDref = XPLMFindDataRef("mag787/udp/drawingCommand");
	XPLMDataRef drawingMessage = XPLMFindDataRef("mag787/udp/drawingMessage");
	XPLMDataRef messagesProcessed = XPLMFindDataRef("mag787/udp/drawingCommandProcessed");
	XPLMDataRef nextBundle = XPLMFindDataRef("mag787/udp/next_bundle");
	XPLMDataRef bundleSent = XPLMFindDataRef("mag787/udp/bundle_received");

	while (threadRun == true) {

		
			for (int i = 0; i < drawBundles.size(); i++) {
				std::vector<std::string> currentDrawBundle = drawBundles.at(0); // WE ALWAYS WANT TO DO THE DRAW BUNDLE FIRST IN LINE 
				
				for (int j = 0; j < currentDrawBundle.size(); j++) {
					char messageBuf[2048];
					strcpy(messageBuf, currentDrawBundle.at(i).c_str());
					XPLMSetDatab(drawingMessage, &messageBuf, 0, strlen(messageBuf));
					XPLMSetDatai(commandDref, 1);
					XPLMSetDatai(messagesProcessed, 0);
					while (XPLMGetDatai(messagesProcessed) == 0) {
						
						//WAIT FOR LUA TO PROCESS COMMAND , WHEN LUA SETS THE DREF TO 2, IT HAS SUCCESSFULLY PROCESSED THE COMMAND
					}
					XPLMSetDatai(commandDref, 0);
				}
				XPLMSetDatai(bundleSent, 1);
				drawBundles.erase(drawBundles.begin()); // ERASE THE DRAW BUNDLE AS IT HAS BEEN USED
				while (XPLMGetDatai(nextBundle) == 0) {
					//WAIT TILL LUA PROCESSES THE CURRENT BUNDLE
					
				}
				

			}
			
		
	}
}




graphicsWrapper::~graphicsWrapper()
{
}
/*
char messageBuf[2048];
				strcpy(messageBuf, .at(i).c_str());
				XPLMSetDatab(drawingMessage, &messageBuf, 0, strlen(messageBuf));
				XPLMSetDatai(commandDref, 1);
				XPLMSetDatai(messagesProcessed, 0);
				while (XPLMGetDatai(messagesProcessed) == 0) {
					//XPLMDebugString("WAITING\n");
					//WAIT FOR LUA TO PROCESS COMMAND , WHEN LUA SETS THE DREF TO 2, IT HAS SUCCESSFULLY PROCESSED THE COMMAND
				}
				XPLMSetDatai(commandDref, 0);

			}

			commandsToBeAdded.clear();
			commandsPending = false;
			XPLMSetDatai(commandDref, 0);
*/