#pragma once

#include "global.h"

class CubeModel
{
public:
	CubeModel();
	void init();

	~CubeModel();

private:
	//Vertex Buffer, Index Buffer(Meshes)
	std::vector<Vertex> vecVBCube;
	std::vector<UINT> vecIBCube;
	ComPtr<ID3D11Buffer>			VBCube;
	ComPtr<ID3D11Buffer>			IBCube;

	std::vector<Vertex>				vecVBAxis;
	UINT							arrIBAxisX[2];
	UINT							arrIBAxisY[2];
	UINT							arrIBAxisZ[2];
	ComPtr<ID3D11Buffer>			VBAxis;
	ComPtr<ID3D11Buffer>			IBAxisX;
	ComPtr<ID3D11Buffer>			IBAxisY;
	ComPtr<ID3D11Buffer>			IBAxisZ;
};

