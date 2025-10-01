#pragma once
#include "global.h"

class CubeWorld
{
public:
	CubeWorld();
	void init();

	~CubeWorld();

	void update();

private:
	//World Coordinates
	Vector3	m_scale = {};
	Quaternion m_rotation = {};
	Vector3	m_position = {};

	Matrix	m_world_mat = {};
};

