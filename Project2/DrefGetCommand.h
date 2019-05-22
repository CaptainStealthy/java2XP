#pragma once
#include "DrefCommand.h"

#include<globalSingletons/dref.h>
#include <future>
#include<string>
template<class T>class DrefGetCommand :
	public DrefCommand
{
public:
	std::promise<T> promiseToDoWork;


	DrefGetCommand(std::future<T>* &inFutureObject, dref dataRef):DrefCommand(dataRef) {
		inFutureObject = &promiseToDoWork.get_future();
	};

	void executeCommand();
	~DrefGetCommand() {};
};

template<class T>
inline void DrefGetCommand<T>::executeCommand()
{

}
