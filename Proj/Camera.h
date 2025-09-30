#pragma once
#include "global.h"



class Camera
{
public:
	Camera();
	void init();

	~Camera();

public:

private:
	void calculate_view_matrix();
	void calculate_ortho_proj_matrix();
	void calculate_persp_proj_matrix();

private:
	//Camera
	float	m_aspect_ratio = {};
	float	m_field_of_view = {};	//FOV
	float	m_Z_near = {};
	float	m_Z_far = {};

	Vector3	m_position = {};
	Quaternion	m_rotation = {};

	Matrix	m_view_matrix = {};
	Matrix	m_projection_matrix = {};
	eProjMode m_projection_mode = {};
};

