#pragma once
#include "global.h"

class Model;
class WorldObj;
class Camera;

class Scene
{
public:
	Scene();

	void init();

	~Scene();

	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;

	void update();
	void render();

	WorldObj* get_cube_world() { return m_world_obj; }
	Camera* get_camera() { return m_camera; }

	void on_resolution_change(UINT _width, UINT _height);

private:
	std::shared_ptr<Model> m_model_obj = {};
	WorldObj* m_world_obj = {};
	Camera* m_camera = {};
};

