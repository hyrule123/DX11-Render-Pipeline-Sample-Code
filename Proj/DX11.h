#pragma once
#include "global.h"

extern HWND g_hWnd;
class Timer;
class Input;

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


class DX11
{
	friend class Manager;

private:
	DX11();
	void init();

	DX11(const DX11& _other) = delete;
	DX11(DX11&& _move) = delete;
	~DX11();
	
	void update();
	void render();
	
public:
	ComPtr<ID3D11Device> get_device() { return m_device; };
	ComPtr<ID3D11DeviceContext> get_context() { return m_context; }

	Vector2 get_resolution() const { return m_resolution; }

private:
//========= DXE ===================
	void _0_DXInit_DeviceContext();
	void _1_DXInit_CreateSwapChain();
	void _2_DXInit_CreateRenderTargetView();
	//void _3_DXInit_CreateInputAssembler();
	void _4_DXInit_CreateBlendState();
	void _5_DXInit_CreateSampler();
	//void _6_DXInit_CreateDefaultGraphicsShader();
	//void _7_DXInit_CreateConstBuffer();
	//void _8_DXInit_CreateMeshes();				//Local Space

	//void _10_DXLoop_UpdateKey();
	//void _11_DXLoop_WorldSpaceTransform();		//World Space
	//void _12_DXLoop_ViewSpaceTransform();		//View Space
	//void _13_DXLoop_ProjectionSpaceTransform();	//Projection Space
	//void _14_DXLoop_UpdateBuffer();
	//void _15_DXLoop_SetShader();
	//void _16_DXLoop_DrawCube();
	//void _17_DXLoop_DrawAxis();
	void _18_DXLoop_FlipSwapChain ();

	

private:
	void set_resolution(const Vector2& _size);

private:
//==================================== VARIABLES ============================================

	Vector2							m_resolution = {};
	
	ComPtr<ID3D11Device> m_device = {};
	ComPtr<ID3D11DeviceContext> m_context = {};
	ComPtr<IDXGISwapChain> SwapChain = {};
	ComPtr<ID3D11Texture2D>			RTTex = {};
	ComPtr<ID3D11RenderTargetView>	RTV = {};

	ComPtr<ID3D11Texture2D>			DSTex = {};
	ComPtr<ID3D11DepthStencilView>	DSV = {};

	// Sampler
	ComPtr<ID3D11SamplerState>		Sampler = {};

	// RasterizerState
	ComPtr<ID3D11RasterizerState>	RSState = {};

	// DepthStencilState
	ComPtr<ID3D11DepthStencilState>	DSState = {};

	// BlendState
	ComPtr<ID3D11BlendState>		BSState = {};


	D3D11_VIEWPORT					ViewPort = {};
};

