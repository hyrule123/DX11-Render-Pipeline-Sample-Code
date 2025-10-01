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

	void update();

	CubeModel* get_cube_model() { return m_cube_model.get(); }
	CubeWorld* get_cube_world() { return m_cube_world.get(); }
	Camera* get_camera() { return m_camera.get(); }

private:
	std::unique_ptr<CubeModel> m_cube_model = {};
	std::unique_ptr<CubeWorld> m_cube_world = {};
	std::unique_ptr<Camera> m_camera = {};
};

