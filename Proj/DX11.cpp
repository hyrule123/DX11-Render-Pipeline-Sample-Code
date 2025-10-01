#include "DX11.h"

//Shader Include
#ifdef _DEBUG

#include "ShaderHeader/VertexShader_Debug.h"
#include "ShaderHeader/PixelShader_Debug.h"

#else

#include "ShaderHeader/VertexShader.h"
#include "ShaderHeader/PixelShader.h"

#endif

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
    m_resolution.x = DEFAULT_WIDTH;
    m_resolution.y = DEFAULT_HEIGHT;

    _0_DXInit_DeviceContext();
    _1_DXInit_CreateSwapChain();
    _2_DXInit_CreateRenderTargetView();
    //_3_DXInit_CreateInputAssembler();
    _4_DXInit_CreateBlendState();
    _5_DXInit_CreateSampler();
    //_6_DXInit_CreateDefaultGraphicsShader();
    //_7_DXInit_CreateConstBuffer();
    //_8_DXInit_CreateMeshes();
}

DX11::~DX11()
{
}

void DX11::update()
{
    _10_DXLoop_UpdateKey();
    _12_DXLoop_ViewSpaceTransform();
    _11_DXLoop_WorldSpaceTransform();

    _13_DXLoop_ProjectionSpaceTransform();
    _14_DXLoop_UpdateBuffer();
    _15_DXLoop_SetShader();
    _16_DXLoop_DrawCube();
    _17_DXLoop_DrawAxis();
    _18_DXLoop_FlipSwapChain();
}

void DX11::_0_DXInit_DeviceContext()
{
    int DXFlag = 0;
#ifdef _DEBUG
    DXFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL DXLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

    CHKFAIL(D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        DXFlag,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        m_device.GetAddressOf(), &DXLevel,
        m_context.GetAddressOf()
    ));
}

void DX11::_1_DXInit_CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.OutputWindow = g_hWnd;
    Desc.Windowed = true;

    Desc.BufferCount = 2;
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.BufferDesc.Width = (UINT)m_resolution.x;
    Desc.BufferDesc.Height = (UINT)m_resolution.y;
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.BufferDesc.RefreshRate.Denominator = 1;
    Desc.BufferDesc.RefreshRate.Numerator = 60;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.Flags = 0;

    ComPtr<IDXGIDevice> DXGIDevice;
    ComPtr<IDXGIAdapter> DXGIAdapter;
    ComPtr<IDXGIFactory> DXGIFactory;

    CHKFAIL(m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf()));

    CHKFAIL(DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)DXGIAdapter.GetAddressOf()));

    CHKFAIL(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)DXGIFactory.GetAddressOf()));

    CHKFAIL(DXGIFactory->CreateSwapChain(m_device.Get(), &Desc, SwapChain.GetAddressOf()));
}

void DX11::_2_DXInit_CreateRenderTargetView()
{
    CHKFAIL(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)RTTex.GetAddressOf()));

    CHKFAIL(m_device->CreateRenderTargetView(RTTex.Get(), nullptr, RTV.GetAddressOf()));

    D3D11_TEXTURE2D_DESC Desc = {};

    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    Desc.Width = (UINT)m_resolution.x;
    Desc.Height = (UINT)m_resolution.y;
    Desc.ArraySize = 1;

    Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    Desc.MipLevels = 1;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    CHKFAIL(m_device->CreateTexture2D(&Desc, nullptr, DSTex.GetAddressOf()));
    CHKFAIL(m_device->CreateDepthStencilView(DSTex.Get(), nullptr, DSV.GetAddressOf()));


    ViewPort.TopLeftX = 0.f;
    ViewPort.TopLeftY = 0.f;

    ViewPort.Width = m_resolution.x;
    ViewPort.Height = m_resolution.y;
    ViewPort.MinDepth = 0.f;
    ViewPort.MaxDepth = 1.f;

    //뷰포트(스크린) 지정
    m_context->RSSetViewports(1, &ViewPort);
}


void DX11::_4_DXInit_CreateBlendState()
{
    BSState = nullptr;
}

void DX11::_5_DXInit_CreateSampler()
{
    D3D11_SAMPLER_DESC SamDesc = {};

    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    m_device->CreateSamplerState(&SamDesc, Sampler.GetAddressOf());
}

void DX11::_12_DXLoop_ViewSpaceTransform()
{
    //ViewMatrix 
    // = (RotationMatrix * TranslationMatrix)^(-1)
    // = (TranslationMatrix^(-1) * RotationMatrix^(-1))          

    //TranslationMatrix^(-1) = 
    /*
    {
    1
        1
            1   
    -x  -y  -z  1
    }
    = TranslationMatrix(-m_position)
    */
    Matrix CameraTranslationMatrix = Matrix::CreateTranslation(-CameraPosition);
   

    //Roll
    Matrix CameraRotationMatrixZ = XMMatrixRotationZ(CameraRotation.z);

    //Pitch
    Matrix CameraRotationMatrixX = XMMatrixRotationX(CameraRotation.x);

    //Yaw
    Matrix CameraRotationMatrixY = XMMatrixRotationY(CameraRotation.y);

    //The order of transformations is roll first, then pitch, then yaw.
    ////R(View) = (R(Z) * R(X) * R(Y))^(-1) = R(Y)^(-1) * R(X)^(-1) * R(Z)^(-1)
    //(R * T)^(-1)
    matCameraRotation = CameraRotationMatrixZ * CameraRotationMatrixX * CameraRotationMatrixY;
    
    matCameraRotation = XMMatrixRotationRollPitchYawFromVector(CameraRotation);


    ////Calculate camera direction(For move camera)
    //Vector3 BasisDir[3] =
    //{
    //    Vector3(1.f, 0.f, 0.f),
    //    Vector3(0.f, 1.f, 0.f),
    //    Vector3(0.f, 0.f, 1.f)
    //};

    //for (int i = 0; i < eDIREND; ++i)
    //{
    //    //Calculate how much each basis vector is rotated
    //    CameraDirection[i] = XMVector3TransformNormal(BasisDir[i], CameraRotationMatrix);
    //}

    //You can rotate reverse by transposed CubeRotation matrix
    //Characteristics of CubeRotation matrix = orthographic matrix
    //Characteristics of orthographic matrix = Its inverse matrix is ​​its transpose matrix.
    //CameraRotationMatrix.Transpose();

    //(Translationmatrix^(-1) * RotationMatrix^(-1))   
    Matrix matRotInv = matCameraRotation.Transpose();
    
    ViewMatrix = CameraTranslationMatrix * matRotInv;
}

void DX11::_13_DXLoop_ProjectionSpaceTransform()
{
    AspectRatio = m_resolution.x / m_resolution.y;
    FieldOfView = XM_PI / 3.f;

    float NearPlane = 1.f;
    float FarPlane = 10000.f;

    ProjectionMatrix = XMMatrixPerspectiveFovLH(FieldOfView, AspectRatio, NearPlane, FarPlane);
}

void DX11::_14_DXLoop_UpdateBuffer()
{
    Matrix WorldViewProjectionMatrix = WorldMatrix * ViewMatrix * ProjectionMatrix;
    WorldViewProjectionMatrix = WorldViewProjectionMatrix.Transpose();

    D3D11_MAPPED_SUBRESOURCE SubRes = {};
    if (!FAILED(m_context->Map(CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRes)))
    {
        memcpy_s(SubRes.pData, sizeof(Matrix), static_cast<void*>(&WorldViewProjectionMatrix), sizeof(Matrix));
        m_context->Unmap(CB.Get(), 0);
    }

    m_context->VSSetConstantBuffers(0, 1, CB.GetAddressOf());
}

void DX11::_15_DXLoop_SetShader()
{
    //출력 병합기에 새 렌더타겟을 등록.
    m_context->OMSetRenderTargets(1, RTV.GetAddressOf(), DSV.Get());

    //화면 클리어
    Vector4 ClearColor = Vector4(0.5f, 0.5f, 0.5f, 1.f);
    m_context->ClearRenderTargetView(RTV.Get(), ClearColor);
    m_context->ClearDepthStencilView(DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    //입력 조립기에 레이아웃을 등록
    m_context->IASetInputLayout(InputLayout.Get());

    m_context->VSSetShader(VS.Get(), nullptr, 0);
    m_context->PSSetShader(PS.Get(), nullptr, 0);
}

void DX11::_16_DXLoop_DrawCube()
{
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT VBStride = sizeof(Vertex);
    UINT VBOffset = 0;
    m_context->IASetVertexBuffers(0, 1, VBCube.GetAddressOf(), &VBStride, &VBOffset);
    m_context->IASetIndexBuffer(IBCube.Get(), DXGI_FORMAT_R32_UINT, 0);

    m_context->DrawIndexed(36, 0, 0);
}

void DX11::_17_DXLoop_DrawAxis()
{
    //Set Tolology Linestrip and register Axis Vertex Buffer
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    UINT VBStride = sizeof(Vertex);
    UINT VBOffset = 0;
    m_context->IASetVertexBuffers(0, 1, VBAxis.GetAddressOf(), &VBStride, &VBOffset);

    m_context->IASetIndexBuffer(IBAxisX.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_context->DrawIndexed(2, 0, 0);
    m_context->IASetIndexBuffer(IBAxisY.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_context->DrawIndexed(2, 0, 0);
    m_context->IASetIndexBuffer(IBAxisZ.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_context->DrawIndexed(2, 0, 0);
}

void DX11::_18_DXLoop_FlipSwapChain()
{
    SwapChain->Present(1, 0);
}

void DX11::set_resolution(const Vector2& _size)
{

}
