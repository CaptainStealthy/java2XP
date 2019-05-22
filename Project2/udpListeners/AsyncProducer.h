#pragma once

class AsyncProducer {

public:
	static inline AsyncProducer* getInstance() {
		if (!serverInstance) {
			serverInstance = new AsyncProducer();
		}
		return serverInstance;
	}
	
	void startServer();
	void closeServer();
	
	bool initialized = false;
	

private:
	static AsyncProducer *serverInstance;
	void worker_thread();
	AsyncProducer();
	~AsyncProducer();
	bool quit = true;
};