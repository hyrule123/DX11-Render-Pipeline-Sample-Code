#pragma once

#include "global.h"

class Manager
{
//SINGLETON
public:
	static Manager& get_inst() { 
		if (nullptr == s_manager) {
			s_manager = new Manager;
		}
		return *s_manager;
	}
	static void destroy_inst() {
		if (s_manager) { delete s_manager; }
	}
private:
	static inline Manager* s_manager = nullptr;
////////////////////////////////

public:
	bool init();
	void run();

private:
	Manager();
	~Manager();

	
};

