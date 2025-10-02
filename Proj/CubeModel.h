#pragma once

#include "global.h"

class CubeModel
{
public:
	CubeModel();
	void init();

	~CubeModel();

	void render(const Matrix& _WVP);

private:
	void create_input_assembler();
	void create_shader();
	void create_const_buffer();
	void create_cube_mesh();
	void create_axis_mesh();
	

private:
	//Vertex Buffer, Index Buffer(Meshes)
	std::vector<Vertex> vecVBCube = {};
	std::vector<UINT> vecIBCube = {};
	ComPtr<ID3D11Buffer>			VBCube = {};
	ComPtr<ID3D11Buffer>			IBCube = {};

	std::vector<Vertex>				vecVBAxis = {};
	UINT							arrIBAxisX[2] = {};
	UINT							arrIBAxisY[2] = {};
	UINT							arrIBAxisZ[2] = {};
	ComPtr<ID3D11Buffer>			VBAxis = {};
	ComPtr<ID3D11Buffer>			IBAxisX = {};
	ComPtr<ID3D11Buffer>			IBAxisY = {};
	ComPtr<ID3D11Buffer>			IBAxisZ = {};

	//Input Assembler
	ComPtr<ID3D11InputLayout>		InputLayout = {};

	//Shader
	ComPtr<ID3D11VertexShader>		VS = {};
	ComPtr<ID3D11PixelShader>		PS = {};

	//Constant Buffer
	ComPtr<ID3D11Buffer>			CB = {};
};

