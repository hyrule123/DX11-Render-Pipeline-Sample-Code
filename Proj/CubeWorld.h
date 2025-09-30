#pragma once
#include "global.h"

class CubeWorld
{
public:
	CubeWorld();
	void init();

	~CubeWorld();

private:
	//World Coordinates
	Vector3	CubePosition = {};
	Vector3	CubeScale = {};
	Vector3	CubeRotation = {};
	bool	UseQuaternion = {};

	Matrix	matCubeScale = {};
	Matrix	matCubeRotation = {};
	Matrix	matCubeTranslation = {};

	Matrix	WorldMatrix = {};
};

