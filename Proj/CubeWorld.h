#pragma once
#include "global.h"

class CubeModel;
class Camera;

class CubeWorld
{
public:
	CubeWorld();
	void init();

	~CubeWorld();

	void update();
	void render(const Matrix& _view_mat, const Matrix& _proj_mat);

	void set_cube_model(const std::shared_ptr<CubeModel>& _cube) { m_model = _cube; }

private:
	void rotate_by_cam_view(Camera* _cam, Vector2 _mouse_dir, float _deltatime);

private:
	std::shared_ptr<CubeModel> m_model = {};

	//World Coordinates
	Vector3	m_scale = {};
	Quaternion m_rotation = {};
	Vector3	m_position = {};

	Matrix	m_world_mat = {};
};

