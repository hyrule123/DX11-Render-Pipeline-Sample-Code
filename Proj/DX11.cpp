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


void DX11::_10_DXLoop_UpdateKey()
{
    Vector3 Forward = matCameraRotation.Forward();
    Vector3 Right = matCameraRotation.Right();
    Vector3 Up = matCameraRotation.Up();

    Input& input = Manager::get_inst().get_Input_inst();
    float deltatime = Manager::get_inst().get_Timer_inst().get_deltatime();

    if (input.GetKeyPressed(eKey::W))
    {
        CameraPosition += Forward * 100.f * deltatime;
    }
    if (input.GetKeyPressed(eKey::S))
    {
        CameraPosition -= Forward * 100.f * deltatime;
    }
    if (input.GetKeyPressed(eKey::A))
    {
        CameraPosition -= Right * 100.f * deltatime;
    }
    if (input.GetKeyPressed(eKey::D))
    {
        CameraPosition += Right * 100.f * deltatime;
    }

}

void DX11::_11_DXLoop_WorldSpaceTransform()
{
    //크기
    Matrix matScale = Matrix::CreateScale(CubeScale);


    //회전
    //The order of transformations is roll first, then pitch, then yaw.

    //Pitch
    Matrix matRotEulerX = XMMatrixRotationX(CubeRotation.x);

    //Yaw
    Matrix matRotEulerY = XMMatrixRotationY(CubeRotation.y);

    //Roll
    Matrix matRotEulerZ = XMMatrixRotationZ(CubeRotation.z);

    matCubeRotation = matRotEulerZ * matRotEulerX * matRotEulerY;

    Input& input = Manager::get_inst().get_Input_inst();
    float deltatime = Manager::get_inst().get_Timer_inst().get_deltatime();

    if (input.GetKeyPressed(eKey::MOUSE_LBUTTON))
    {
        Vector2 Dir = input.GetMouseDir();

        Matrix RotByCamAxisY = Matrix::CreateFromAxisAngle(matCameraRotation.Up(), -Dir.x * deltatime);
        Matrix RotByCamAxisX = Matrix::CreateFromAxisAngle(matCameraRotation.Right(), Dir.y * deltatime);

        matCubeRotation *= RotByCamAxisX * RotByCamAxisY;
        
        CubeRotation = Matrix::ExtractPitchYawRollFromMatrix(matCubeRotation);
        
        
        
        //const Vector3 CubeCamDistance = CubePosition - CameraPosition;


        //Matrix Rot = Matrix::CreateFromAxisAngle()


        //Vector2 Dir = Input->GetMouseDir();

        //const Vector3 CubeCamDistance = CubePosition - CameraPosition;
        //
        ////1. 우선 카메라의 위치까지 큐브를 끌어오는 행렬을 만든다.
        //Matrix PulltoCameraPos = XMMatrixTranslationFromVector(-CubeCamDistance);
        //matCubeRotation = XMMatrixIdentity();
        //matCubeRotation *= PulltoCameraPos;


        ////2. 회전도 반대로 회전시켜서 카메라와 축을 일치시킨다.
        //Vector3 CubeCamRotation = CubeRotation - CameraRotation;

        ////2-1. 카메라의 축 기준으로 반대로 회전시켜 준다.
        //Matrix RotX = Matrix::CreateFromAxisAngle(matCameraRotation.Right(), CubeCamRotation.x);
        //Matrix RotY = Matrix::CreateFromAxisAngle(matCameraRotation.Up(), CubeCamRotation.y);
        //Matrix RotZ = Matrix::CreateFromAxisAngle(matCameraRotation.Forward(), CubeCamRotation.z);
        ////The order of transformations is roll first, then pitch, then yaw.
        //Matrix matCubeCamRotation = RotZ * RotX * RotY;

        //matCubeRotation *= matCubeCamRotation.Transpose();

        //////3. 이 상태에서 회전하고자 하는 양만큼 회전시킨다.
        //CubeRotation.x += Dir.y * g_DeltaTime;
        //CubeRotation.y += Dir.x * g_DeltaTime;
        //CubeRotation.z += Dir.x * g_DeltaTime;
        //Matrix RotByCamAxisX = Matrix::CreateFromAxisAngle(matCameraRotation.Right(), CubeRotation.x);
        ////Matrix RotByCamAxisY = Matrix::CreateFromAxisAngle(matCameraRotation.Up(), CubeRotation.y);
        ////Matrix RotByCamAxisZ = Matrix::CreateFromAxisAngle(matCameraRotation.Forward(), CubeRotation.z);
        //Matrix RotByCamAxisZ = Matrix::CreateRotationZ(CubeRotation.z);
        //matCubeRotation *= RotByCamAxisX;

        ////4. 다시 원위치로 회전시킨다.
        //matCubeRotation *= matCubeCamRotation;

        ////5. 다시 원위치로 이동시킨다.
        //Matrix PushtoCameraPos = XMMatrixTranslationFromVector(CubeCamDistance);
        //matCubeRotation *= PushtoCameraPos;
    }
    else if (input.GetKeyPressed(eKey::MOUSE_RBUTTON))
    {
        Vector2 Dir = input.GetMouseDir();

        CameraRotation.x -= Dir.y * deltatime;
        CameraRotation.y += Dir.x * deltatime;
    }





    //이동
    Matrix matTranslation = XMMatrixTranslationFromVector(CubePosition);
    //월드행렬
    WorldMatrix = matScale * matCubeRotation * matTranslation;

    

    //size_t size = 0;

    //if (ManualCalculation)
    //{
    //   size = vecCubeVtxPos.size();
    //}


    ////CubeScale Matrix
    //Matrix matScale =
    //{
    //    CubeScale.x, 0,         0,          0,
    //    0,       CubeScale.y,   0,          0,
    //    0,       0,         CubeScale.z,    0,
    //    0,       0,         0,          1
    //};

    //if (ManualCalculation)
    //{
    //    for (size_t i = 0; i < size; ++i)
    //    {
    //        //Vector * Matrix
    //        XMVector4Transform(vecCubeVtxPos[i], matScale);
    //    }
    //}


    ////Euler CubeRotation
    //if (false == UseQuaternion)
    //{
    //    Matrix matRotationX =
    //    {
    //        1, 0,                0,                 0,
    //        0, cosf(CubeRotation.x), -sinf(CubeRotation.x), 0,
    //        0, sinf(CubeRotation.x), cosf(CubeRotation.x),  0,
    //        0, 0,                0,                 1
    //    };

    //    Matrix matRotationY =
    //    {
    //        cosf(CubeRotation.y),  0, sinf(CubeRotation.y), 0,
    //        0,                 1, 0,                0,
    //        -sinf(CubeRotation.y), 0, cosf(CubeRotation.y), 0,
    //        0,                 0, 0,                1
    //    };

    //    Matrix matRotationZ =
    //    {
    //        cosf(CubeRotation.z), -sinf(CubeRotation.z), 0, 0,
    //        sinf(CubeRotation.z), cosf(CubeRotation.z),  0, 0,
    //        0,                0,                 1, 0,
    //        0,                0,                 0, 1
    //    };

    //    Matrix matRotation = matRotationX * matRotationY * matRotationZ;


    //    if (ManualCalculation)
    //    {
    //        for (size_t i = 0; i < size; ++i)
    //        {
    //            XMVector4Transform(vecCubeVtxPos[i], matRotation);
    //        }
    //    }

    //}

    ////Quaternion CubeRotation
    //else
    //{

    //    //1. Euler -> Quaternion Transformation
    //    ///Divide Half
    //    Vector3 HalfAngle(CubeRotation.x / 2.f, CubeRotation.y / 2.f, CubeRotation.z / 2.f);

    //    ///1-1. Quaternion CubeRotation XYZ
    //    Quaternion QRotX, QRotY, QRotZ;
    //    QRotX = { sinf(HalfAngle.x), 0.f, 0.f, cosf(HalfAngle.x) };
    //    QRotY = { 0.f, sinf(HalfAngle.y), 0.f, cosf(HalfAngle.y) };
    //    QRotZ = { 0.f, 0.f, sinf(HalfAngle.z), cosf(HalfAngle.z) };

    //    ///1-2. Compare Between Two
    //    Quaternion QRot = QRotX * QRotY * QRotZ;
    //    Quaternion DXQuatRotResult = XMQuaternionRotationRollPitchYawFromVector(CubeRotation);
    //    
    //    //2. Calculate Quaternion Matrix
    //    ///Vertex calculation process 
    //    Quaternion 


    //}
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
    = TranslationMatrix(-CubePosition)
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
