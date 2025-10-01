#include "Scene.h"

#include "CubeModel.h"
#include "CubeWorld.h"
#include "Camera.h"

Scene::Scene() {}

void Scene::init()
{
	m_cube_model = std::make_unique<CubeModel>();
	m_cube_world = std::make_unique<CubeWorld>();
	m_camera = std::make_unique<Camera>();
}

Scene::~Scene()
{
	m_camera.reset();
	m_cube_world.reset();
	m_cube_model.reset();
}

void Scene::update()
{

}
