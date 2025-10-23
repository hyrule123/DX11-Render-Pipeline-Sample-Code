#include "Scene.h"

#include "CubeModel.h"
#include "CubeWorld.h"
#include "Camera.h"

Scene::Scene() {}

void Scene::init()
{
	m_cube_model = std::make_shared<CubeModel>();
	m_cube_model->init();

	m_cube_world = new CubeWorld;
	m_cube_world->set_cube_model(m_cube_model);
	m_cube_world->init();

	m_camera = new Camera;
	m_camera->init();
}

Scene::~Scene()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_cube_world);
	m_cube_model = nullptr;
}

void Scene::update()
{
	m_cube_world->update();
	m_camera->update();
}

void Scene::render()
{
	m_camera->render(m_cube_world);
}

void Scene::on_resolution_change(UINT _width, UINT _height)
{
	m_camera->on_resolution_change((float)_width, (float)_height);
}
