#pragma once
#include<globalSingletons/dref.h>
#include <future>
#include<string>



class DrefCommand
{
public:

	
	//this is supposed to include a promise that will have a future object in it.
	/*
		Once we have a promise, we can most likely pass in a future object and get the future of the promise object
		Once we have the future of the promise object, the command consumer can call get value and will be blocked till 
		an actual value can be returned to it.
	*/
	
	

	DrefCommand(dref dataRef);

	

	virtual void executeCommand();
	~DrefCommand();

protected:

	dref dataRef;

};

