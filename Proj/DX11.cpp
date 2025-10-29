#include "DX11.h"

#include "settings.h"

#include "Manager.h"
#include "Timer.h"
#include "Input.h"

#include <FW1FontWrapper/FW1FontWrapper.h>

DX11::DX11()
{
}

void DX11::init()
{
    create_device_context();

    create_rasterizer_state();
    create_blend_state();
    create_sampler();
}

DX11::~DX11()
{
}

void DX11::update()
{
    //출력 병합기에 렌더타겟을 지정.
    m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), m_depth_stencil_view.Get());

    //화면 클리어
    Vector4 ClearColor = Vector4(0.5f, 0.5f, 0.5f, 1.f);
    m_context->ClearRenderTargetView(m_render_target_view.Get(), reinterpret_cast<const FLOAT*>(&ClearColor));
    m_context->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void DX11::render()
{
    m_swapchain->Present(1, 0);
}

void DX11::create_device_context()
{
    int DXFlag = 0;
#ifdef _DEBUG
    DXFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL DXLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        DXFlag,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        m_device.GetAddressOf(), &DXLevel,
        m_context.GetAddressOf()
    );

    CHKFAIL(hr);
}

void DX11::create_swap_chain()
{
    m_swapchain.Reset();

    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.OutputWindow = g_hWnd;
    Desc.Windowed = true;

    Desc.BufferCount = 2;
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.BufferDesc.Width = m_res_width;
    Desc.BufferDesc.Height = m_res_height;
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.BufferDesc.RefreshRate.Denominator = 1;
    Desc.BufferDesc.RefreshRate.Numerator = 60;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.Flags = 0;

    ComPtr<IDXGIDevice> DXGIDevice = nullptr;
    ComPtr<IDXGIAdapter> DXGIAdapter = nullptr;
    ComPtr<IDXGIFactory> DXGIFactory = nullptr;

    CHKFAIL(m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf()));

    CHKFAIL(DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)DXGIAdapter.GetAddressOf()));

    CHKFAIL(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)DXGIFactory.GetAddressOf()));

    CHKFAIL(DXGIFactory->CreateSwapChain(m_device.Get(), &Desc, m_swapchain.GetAddressOf()));
}

void DX11::create_render_target_view()
{
    if (nullptr == m_swapchain) { assert(false); }

    m_render_target_view = nullptr;
    m_depth_stencil_view = nullptr;

    CHKFAIL(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_renter_target_buffer.GetAddressOf()));

    CHKFAIL(m_device->CreateRenderTargetView(m_renter_target_buffer.Get(), nullptr, m_render_target_view.GetAddressOf()));

    D3D11_TEXTURE2D_DESC Desc = {};

    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    Desc.Width = m_res_width;
    Desc.Height = m_res_height;
    Desc.ArraySize = 1;

    Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    Desc.MipLevels = 1;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    CHKFAIL(m_device->CreateTexture2D(&Desc, nullptr, m_depth_stencil_buffer.GetAddressOf()));
    CHKFAIL(m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), nullptr, m_depth_stencil_view.GetAddressOf()));

    m_viewport.TopLeftX = 0.f;
    m_viewport.TopLeftY = 0.f;

    m_viewport.Width = (FLOAT)m_res_width;
    m_viewport.Height = (FLOAT)m_res_height;
    m_viewport.MinDepth = 0.f;
    m_viewport.MaxDepth = 1.f;

    //뷰포트(스크린) 지정
    m_context->RSSetViewports(1, &m_viewport);
}

void DX11::create_depth_stencil_view()
{
}


void DX11::create_rasterizer_state()
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;

    rasterDesc.CullMode = D3D11_CULL_BACK;

    //이 부분!!
    //앞 부분이 CW인지 CCW인지 정의
    rasterDesc.FrontCounterClockwise = TRUE;
    // -----------------

    rasterDesc.DepthClipEnable = TRUE;

    HRESULT hr = m_device->CreateRasterizerState(&rasterDesc, m_rasterizer_state.GetAddressOf());

    CHKFAIL(hr);

    //단일 Rasterizer 사용 예정이므로 한번 등록 후 계속 사용
    m_context->RSSetState(m_rasterizer_state.Get());
}

void DX11::create_blend_state()
{
    m_blend_state = nullptr;
}

void DX11::create_sampler()
{
    D3D11_SAMPLER_DESC SamDesc = {};

    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    m_device->CreateSamplerState(&SamDesc, m_sampler.GetAddressOf());
}

void DX11::set_resolution(UINT _width, UINT _height)
{
    m_res_width = _width;
    m_res_height = _height;

    m_context->OMSetRenderTargets(0u, nullptr, nullptr);

    HRESULT hr = E_FAIL;
    if (nullptr == m_swapchain)
    {
        create_swap_chain();
        hr = S_OK;
    }
    else
    {
        // Pass 0 for buffer count to preserve the existing count
        // Pass 0 for width/height to automatically use the window's new client size
        // Pass DXGI_FORMAT_UNKNOWN to preserve the existing format
        hr = m_swapchain->ResizeBuffers(0u, _width, _height, DXGI_FORMAT_UNKNOWN, 0u);
    }

    CHKFAIL(hr);

    create_render_target_view();
    create_depth_stencil_view();
}
