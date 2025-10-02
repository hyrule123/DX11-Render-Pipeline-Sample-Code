#pragma once
#include "global.h"

class CubeModel;
class CubeWorld;
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

	CubeWorld* get_cube_world() { return m_cube_world; }
	Camera* get_camera() { return m_camera; }

private:
	std::shared_ptr<CubeModel> m_cube_model = {};
	CubeWorld* m_cube_world = {};
	Camera* m_camera = {};
};

