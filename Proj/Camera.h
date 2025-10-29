#pragma once
#include "global.h"

//View Space
//https://www.notion.so/hyrule1/3D-Graphics-Study-250cb63f18c18074b5dcca4609f4b447?p=254cb63f18c1810cbcb6fbc3d517340d&pm=s

//Clip Space
//https://www.notion.so/hyrule1/3D-Graphics-Study-250cb63f18c18074b5dcca4609f4b447?p=254cb63f18c180f9bb86f4fbadab0bf9&pm=s

class Model;
class WorldObj;

class Camera
{
public:
	Camera();
	void init();

	~Camera();

public:
	void update();

	void render(WorldObj* _world);

	const Quaternion& get_rotataion() const { return m_rotation; }
	const Matrix& get_rotation_matrix() const { return m_rot_matrix; }

	void on_resolution_change(float _width, float _height);

private:
	void calculate_view_matrix();
	void calculate_ortho_proj_matrix(float _l, float _r, float _b, float _t);
	void calculate_persp_proj_matrix(float _near, float _far, float _fov_deg, float _aspect_ratio);

private:
	//Camera
	float	m_aspect_ratio = {};
	float	m_fov_deg = {};	//FOV
	float	m_near = {};
	float	m_far = {};

	Vector3	m_position = {};
	Quaternion	m_rotation = {};
	Matrix m_rot_matrix = {};

	Matrix	m_view_matrix = {};
	Matrix	m_projection_matrix = {};
	eProjMode m_projection_mode = {};

	//회전 관련
	Vector2 m_pitchyaw_degree;
};

