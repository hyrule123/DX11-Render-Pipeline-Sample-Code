#pragma once
#include "global.h"

class Camera
{

private:
	//Camera
	float	AspectRatio;
	float	FieldOfView;	//FOV
	Vector3	CameraPosition;
	Vector3	CameraRotation;
	Matrix	matCameraRotation;	//Needed for camera movement

	Matrix	ViewMatrix;
	Matrix	ProjectionMatrix;
};

