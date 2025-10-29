#pragma once
#include "global.h"

//월드 공간을 담당
//https://www.notion.so/hyrule1/3D-Graphics-Study-250cb63f18c18074b5dcca4609f4b447?p=254cb63f18c1810f8eb0e26b74048b0b&pm=s


class Model;
class Camera;

class WorldObj
{
public:
	WorldObj();
	void init();

	~WorldObj();

	void update();
	void render(const Matrix& _view_mat, const Matrix& _proj_mat);

	void set_cube_model(const std::shared_ptr<Model>& _cube) { m_model_obj = _cube; }

private:
	void rotate_by_cam_view(Camera* _cam, Vector2 _mouse_dir, float _deltatime);

private:
	std::shared_ptr<Model> m_model_obj = {};

	//World Coordinates
	Vector3	m_scale = {};
	Quaternion m_rotation = {};
	Vector3	m_position = {};

	Matrix	m_world_mat = {};
};

