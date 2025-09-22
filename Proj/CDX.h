#pragma once
#include "global.h"

extern HWND g_hWnd;
class CTimer;
class CKeyMgr;

enum eDirection
{
	eDIRUP,		//X
	eDIRRIGHT,	//Y
	eDIRFORWARD,	//Z
	eDIREND
};

enum class eCalculationMode
{
	Auto,		//Use Built-In Functions.
	SemiAuto,	//Only calculate WVP Matrix. The rest compuded in the shader.
	Manual		//Calculate everything on C++.
};


class CDX
{
private:
	CDX() = delete;
public:
	CDX(const Vec2& _WinSize);
	~CDX();
	
public:
	ComPtr<ID3D11Device> get_device() { return Device; };
	ComPtr<ID3D11DeviceContext> get_context() { return Context; }

//========= DXE ===================
	void _0_DXInit_DeviceContext();
	void _1_DXInit_CreateSwapChain();
	void _2_DXInit_CreateView();
	void _3_DXInit_CreateInputAssembler();
	void _4_DXInit_CreateBlendState();
	void _5_DXInit_CreateSampler();
	void _6_DXInit_CreateDefaultGraphicsShader();
	void _7_DXInit_CreateConstBuffer();
	void _8_DXInit_CreateMeshes();				//Local Space

	void _9_DXLoop_UpdateTime();
	void _10_DXLoop_UpdateKey();
	void _11_DXLoop_WorldSpaceTransform();		//World Space
	void _12_DXLoop_ViewSpaceTransform();		//View Space
	void _13_DXLoop_ProjectionSpaceTransform();	//Projection Space
	void _14_DXLoop_UpdateBuffer();
	void _15_DXLoop_SetShader();
	void _16_DXLoop_DrawCube();
	void _17_DXLoop_DrawAxis();
	void _18_DXLoop_FlipSwapChain ();



//====== LOOP =======
	void DXLoop();



//==================================== VARIABLES ============================================

	Vec2							vRenderResolution;
	
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;
	ComPtr<ID3D11Texture2D>			RTTex;
	ComPtr<ID3D11RenderTargetView>	RTV;

	ComPtr<ID3D11Texture2D>			DSTex;
	ComPtr<ID3D11DepthStencilView>	DSV;

	//Input Assembler
	ComPtr<ID3D11InputLayout>		InputLayout;

	//Shader
	ComPtr<ID3D11VertexShader>		VS;
	ComPtr<ID3D11PixelShader>		PS;

	// Sampler
	ComPtr<ID3D11SamplerState>		Sampler;

	// RasterizerState
	ComPtr<ID3D11RasterizerState>	RSState;

	// DepthStencilState
	ComPtr<ID3D11DepthStencilState>	DSState;

	// BlendState
	ComPtr<ID3D11BlendState>		BSState;


	D3D11_VIEWPORT					ViewPort;

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

	//Constant Buffer
	ComPtr<ID3D11Buffer>			CB;




	//World Coordinates
	Vec3	CubePosition;
	Vec3	CubeScale;
	Vec3	CubeRotation;
	bool	UseQuaternion;

	Matrix	matCubeScale;
	Matrix	matCubeRotation;
	Matrix	matCubeTranslation;
	

	Matrix	WorldMatrix;


	//Manual calculation mode
	//Do all jobs on C++
	bool	ManualCalculation;
	std::vector<Vec4> vecCubeVtxPos;	

	//Camera
	float	AspectRatio;
	float	FieldOfView;	//FOV
	Vec3	CameraPosition;
	Vec3	CameraRotation;
	Matrix	matCameraRotation;	//Needed for camera movement
	
	Matrix	ViewMatrix;
	Matrix	ProjectionMatrix;

	//Timer
	CTimer* Timer;
	
	//Key Manager
	CKeyMgr* KeyMgr;

	
};

