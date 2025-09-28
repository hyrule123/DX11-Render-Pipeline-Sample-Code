#pragma once

#include "global.h"

class DX11;
class Timer;
class Input;

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

	DX11& get_DX11_inst() { return *m_dx; }

private:
	Manager();
	~Manager();

	Manager(const Manager& _other) = delete;
	Manager(Manager&& _other) = delete;

	DX11* m_dx;

	//Timer
	Timer* m_timer;

	//Key Manager
	Input* m_input;
};

