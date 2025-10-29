#include "Scene.h"

#include "Model.h"
#include "WorldObj.h"
#include "Camera.h"

Scene::Scene() {}

void Scene::init()
{
	m_model_obj = std::make_shared<Model>();
	m_model_obj->init();

	m_world_obj = new WorldObj;
	m_world_obj->set_cube_model(m_model_obj);
	m_world_obj->init();

	m_camera = new Camera;
	m_camera->init();
}

Scene::~Scene()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_world_obj);
	m_model_obj = nullptr;
}

void Scene::update()
{
	m_world_obj->update();
	m_camera->update();
}

void Scene::render()
{
	m_camera->render(m_world_obj);
}

void Scene::on_resolution_change(UINT _width, UINT _height)
{
	m_camera->on_resolution_change((float)_width, (float)_height);
}
