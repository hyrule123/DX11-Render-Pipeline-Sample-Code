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

	ComPtr<ID3D11RasterizerState> get_rs_state() { return m_rasterizer_state; }

	UINT get_width() const { return m_res_width; }
	UINT get_height() const { return m_res_height; }

private:
//========= DXE ===================
	void create_device_context();
	void create_swap_chain();
	void create_render_target_view();
	void create_depth_stencil_view();

	void create_rasterizer_state();
	void create_blend_state();
	void create_sampler();

	//void DXInit_CreateInputAssembler();
	//void DXInit_CreateDefaultGraphicsShader();
	//void DXInit_CreateConstBuffer();
	//void DXInit_CreateMeshes();				//Local Space

	//void DXLoop_UpdateKey();
	//void DXLoop_WorldSpaceTransform();		//World Space
	//void DXLoop_ViewSpaceTransform();		//View Space
	//void DXLoop_ProjectionSpaceTransform();	//Projection Space
	//void DXLoop_UpdateBuffer();
	//void DXLoop_SetShader();
	//void DXLoop_DrawCube();
	//void DXLoop_DrawAxis();
	//void DXLoop_FlipSwapChain ();

private:
	void set_resolution(UINT _width, UINT _height);

private:
//==================================== VARIABLES ============================================
	UINT m_res_width = {};
	UINT m_res_height = {};
	
	ComPtr<ID3D11Device> m_device = {};
	ComPtr<ID3D11DeviceContext> m_context = {};
	ComPtr<IDXGISwapChain> m_swapchain = {};
	ComPtr<ID3D11Texture2D>			m_renter_target_buffer = {};
	ComPtr<ID3D11RenderTargetView>	m_render_target_view = {};

	ComPtr<ID3D11Texture2D>			m_depth_stencil_buffer = {};
	ComPtr<ID3D11DepthStencilView>	m_depth_stencil_view = {};
	ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state = {};

	// m_sampler
	ComPtr<ID3D11SamplerState>		m_sampler = {};

	// RasterizerState
	ComPtr<ID3D11RasterizerState>	m_rasterizer_state = {};

	// BlendState
	ComPtr<ID3D11BlendState>		m_blend_state = {};


	D3D11_VIEWPORT					m_viewport = {};
};

