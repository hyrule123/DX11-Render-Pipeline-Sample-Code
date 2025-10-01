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

private:
	Manager();
	~Manager();

	Manager(const Manager& _other) = delete;
	Manager(Manager&& _other) = delete;

	std::unique_ptr<DX11> m_dx = {};

	//Timer
	std::unique_ptr<Timer> m_timer = {};

	//Key Manager
	std::unique_ptr<Input> m_input = {};

	//Scene
	std::unique_ptr<Scene> m_scene = {};
};

