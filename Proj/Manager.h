#pragma once

#include "global.h"

class DX11;
class Timer;
class Input;
class Scene;

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
	void init();
	void run();

	DX11& get_DX11_inst() { return *m_dx; }
	Timer& get_Timer_inst() { return *m_timer; }
	Input& get_Input_inst() { return *m_input; }
	Scene& get_Scene_inst() { return *m_scene; }

	void set_winsize(UINT _width, UINT _height);

private:
	Manager();
	~Manager();

	Manager(const Manager& _other) = delete;
	Manager(Manager&& _other) = delete;

	DX11* m_dx = {};

	//Timer
	Timer* m_timer = {};

	//Key Manager
	Input* m_input = {};

	//Scene
	Scene* m_scene = {};

	UINT m_width = {};
	UINT m_height = {};
};

