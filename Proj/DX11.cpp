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

#include "CTimer.h"
#include "CKeyMgr.h"

#include <FW1FontWrapper/FW1FontWrapper.h>

DX11::DX11():
    UseQuaternion(false),
    ManualCalculation(false)
{
}

DX11::~DX11()
{
    delete KeyMgr;
    delete Timer;
}

bool DX11::init()
{
    Timer = new CTimer;
    KeyMgr = new CKeyMgr;


    CameraPosition = Vector3(0.f, 0.f, -100.f);
    //CameraRotation = Vector3(1.f, 1.f, 1.f);
    CubeScale = Vector3(50.f, 50.f, 50.f);


    vRenderResolution.x = DEFAULT_WIDTH;
    vRenderResolution.y = DEFAULT_HEIGHT;

    AspectRatio = vRenderResolution.x / vRenderResolution.y;

    _0_DXInit_DeviceContext();
    _1_DXInit_CreateSwapChain();
    _2_DXInit_CreateView();
    _3_DXInit_CreateInputAssembler();
    _4_DXInit_CreateBlendState();
    _5_DXInit_CreateSampler();
    _6_DXInit_CreateDefaultGraphicsShader();
    _7_DXInit_CreateConstBuffer();
    _8_DXInit_CreateMeshes();

    return true;
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
        Device.GetAddressOf(), &DXLevel,
        Context.GetAddressOf()
    ));
}

void DX11::_1_DXInit_CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.OutputWindow = g_hWnd;
    Desc.Windowed = true;

    Desc.BufferCount = 2;
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.BufferDesc.Width = (UINT)vRenderResolution.x;
    Desc.BufferDesc.Height = (UINT)vRenderResolution.y;
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

    CHKFAIL(Device->QueryInterface(__uuidof(IDXGIDevice), (void**)DXGIDevice.GetAddressOf()));

    CHKFAIL(DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)DXGIAdapter.GetAddressOf()));

    CHKFAIL(DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)DXGIFactory.GetAddressOf()));

    CHKFAIL(DXGIFactory->CreateSwapChain(Device.Get(), &Desc, SwapChain.GetAddressOf()));
}

void DX11::_2_DXInit_CreateView()
{
    CHKFAIL(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)RTTex.GetAddressOf()));

    CHKFAIL(Device->CreateRenderTargetView(RTTex.Get(), nullptr, RTV.GetAddressOf()));

    D3D11_TEXTURE2D_DESC Desc = {};

    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    Desc.Width = (UINT)vRenderResolution.x;
    Desc.Height = (UINT)vRenderResolution.y;
    Desc.ArraySize = 1;

    Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    Desc.MipLevels = 1;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    CHKFAIL(Device->CreateTexture2D(&Desc, nullptr, DSTex.GetAddressOf()));
    CHKFAIL(Device->CreateDepthStencilView(DSTex.Get(), nullptr, DSV.GetAddressOf()));


    ViewPort.TopLeftX = 0.f;
    ViewPort.TopLeftY = 0.f;

    ViewPort.Width = vRenderResolution.x;
    ViewPort.Height = vRenderResolution.y;
    ViewPort.MinDepth = 0.f;
    ViewPort.MaxDepth = 1.f;

    //뷰포트(스크린) 지정
    Context->RSSetViewports(1, &ViewPort);
}

void DX11::_3_DXInit_CreateInputAssembler()
{
    D3D11_INPUT_ELEMENT_DESC IADesc[2] = {};

    //정점 위치(CubePosition)
    IADesc[0].SemanticName = "POSITION";
    IADesc[0].SemanticIndex = 0;
    IADesc[0].AlignedByteOffset = 0;
    IADesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    IADesc[0].InputSlot = 0;
    IADesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    IADesc[0].InstanceDataStepRate = 0;

    //색상 정보(COLOR) - 정점 위치 다음에 오므로 ByteOffset 12
    IADesc[1].SemanticName = "COLOR";
    IADesc[1].SemanticIndex = 0;
    IADesc[1].AlignedByteOffset = 12;
    IADesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    IADesc[1].InputSlot = 0;
    IADesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    IADesc[1].InstanceDataStepRate = 0;

    CHKFAIL(Device->CreateInputLayout(IADesc, 2, g_VS, sizeof(g_VS), InputLayout.GetAddressOf()));
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
    Device->CreateSamplerState(&SamDesc, Sampler.GetAddressOf());
}

void DX11::_6_DXInit_CreateDefaultGraphicsShader()
{
    //GetModuleFileNameW(0, ProgPath, MAX_PATH);

    //int iLen = (int)wcslen(ProgPath);

    ////프로그램 파일 명을 제거한다.
    //for (int i = iLen - 1; i >= 0; --i)
    //{
    //    if (L'\\' == ProgPath[i])
    //    {
    //        ProgPath[i + 1] = 0;
    //        break;
    //    }
    //}   

    //wchar_t ShaderPath[MAX_PATH] = {};
    //lstrcpyW(ShaderPath, ProgPath);
    //lstrcatW(ShaderPath, L"Shader.cso");

    CHKFAIL(Device->CreateVertexShader(g_VS, sizeof(g_VS), nullptr, &VS));
    CHKFAIL(Device->CreatePixelShader(g_PS, sizeof(g_PS), nullptr, &PS));

}



void DX11::_7_DXInit_CreateConstBuffer()
{
    //기본적으로 계산된 WVP Matrix를 보내는 용도로 사용할 상수 버퍼

    D3D11_BUFFER_DESC Desc = {};
    Desc.ByteWidth = sizeof(Matrix);
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    CHKFAIL(Device->CreateBuffer(&Desc, nullptr, CB.GetAddressOf()));
}

void DX11::_8_DXInit_CreateMeshes()
{
    //정점정보 생성
    Vertex v = {};

    v.vPos = Vec3(-0.5f, 0.5f, -0.5f);
    v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vec3(0.5f, 0.5f, -0.5f);
    v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vec3(0.5f, -0.5f, -0.5f);
    v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vec3(-0.5f, -0.5f, -0.5f);
    v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);


    v.vPos = Vec3(-0.5f, 0.5f, 0.5f);
    v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vec3(0.5f, 0.5f, 0.5f);
    v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vec3(0.5f, -0.5f, 0.5f);
    v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vec3(-0.5f, -0.5f, 0.5f);
    v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    //직접 정점 계산 모드를 위한 값을 복사
    //Copy values for manual vertex CubePosition calculation mode
    size_t size = vecVBCube.size();
    for (size_t i = 0; i < size; ++i)
    {
        vecCubeVtxPos.push_back(Vector4(vecVBCube[i].vPos, 1.f));
    }

    //정점버퍼 생성
    D3D11_BUFFER_DESC VBDesc = {};
    VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    VBDesc.Usage = D3D11_USAGE_DYNAMIC;

    VBDesc.ByteWidth = (UINT)(sizeof(Vertex) * vecVBCube.size());

    D3D11_SUBRESOURCE_DATA VtxData = {};
    VtxData.pSysMem = (void*)vecVBCube.data();

    CHKFAIL(Device->CreateBuffer(&VBDesc, &VtxData, VBCube.GetAddressOf()));

    //인덱스 정보 생성
    UINT arrIB[] = {
        0, 1, 2,
        0, 2, 3,

        1, 5, 6,
        1, 6, 2,

        3, 2, 6,
        3, 6, 7,

        4, 0, 3,
        4, 3, 7,

        4, 5, 1,
        4, 1, 0,

        5, 4, 7,
        5, 7, 6
    };

    vecIBCube.insert(vecIBCube.begin(), arrIB, arrIB + 36);

    //인덱스 버퍼 생성
    D3D11_BUFFER_DESC IBDesc = {};
    IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IBDesc.CPUAccessFlags = 0;
    IBDesc.Usage = D3D11_USAGE_DEFAULT;
    IBDesc.ByteWidth = (UINT)(vecIBCube.size() * sizeof(UINT));

    D3D11_SUBRESOURCE_DATA IdxData = {};
    IdxData.pSysMem = (void*)vecIBCube.data();

    CHKFAIL(Device->CreateBuffer(&IBDesc, &IdxData, IBCube.GetAddressOf()));


    //=========Create Axis Buffer===============
    //Origin Vertex
    v.vPos = Vec3(0.f, 0.f, 0.f);
    v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    vecVBAxis.push_back(v);

    //X Vertex
    v.vPos = Vec3(1.5f, 0.f, 0.f);
    v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
    vecVBAxis.push_back(v);
    arrIBAxisX[0] = 0u;
    arrIBAxisX[1] = 1u;

    //Y Vertex
    v.vPos = Vec3(0.f, 1.5f, 0.f);
    v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
    vecVBAxis.push_back(v);
    arrIBAxisY[0] = 0u;
    arrIBAxisY[1] = 2u;
    
    //Z Vertex
    v.vPos = Vec3(0.f, 0.f, 1.5f);
    v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
    vecVBAxis.push_back(v);
    arrIBAxisZ[0] = 0u;
    arrIBAxisZ[1] = 3u;

    //Create Axis Vertex Buffer
    VBDesc.ByteWidth = (UINT)(sizeof(Vertex) * vecVBAxis.size());
    VtxData = {};
    VtxData.pSysMem = (void*)vecVBAxis.data();
    CHKFAIL(Device->CreateBuffer(&VBDesc, &VtxData, VBAxis.GetAddressOf()));

    //Create Axis Index Buffer
    //X
    IBDesc.ByteWidth = (UINT)(2u * sizeof(UINT));
    IdxData = {};
    IdxData.pSysMem = (void*)arrIBAxisX;
    CHKFAIL(Device->CreateBuffer(&IBDesc, &IdxData, IBAxisX.GetAddressOf()));   

    //Y
    IBDesc.ByteWidth = (UINT)(2u * sizeof(UINT));
    IdxData = {};
    IdxData.pSysMem = (void*)arrIBAxisY;
    CHKFAIL(Device->CreateBuffer(&IBDesc, &IdxData, IBAxisY.GetAddressOf()));  

    //Z
    IBDesc.ByteWidth = (UINT)(2u * sizeof(UINT));
    IdxData = {};
    IdxData.pSysMem = (void*)arrIBAxisZ;
    CHKFAIL(Device->CreateBuffer(&IBDesc, &IdxData, IBAxisZ.GetAddressOf()));

}

void DX11::_9_DXLoop_UpdateTime()
{
    Timer->tick();
}

void DX11::_10_DXLoop_UpdateKey()
{
    KeyMgr->tick();

    Vec3 Forward = matCameraRotation.Forward();
    Vec3 Right = matCameraRotation.Right();
    Vec3 Up = matCameraRotation.Up();

    if (KeyMgr->GetKeyPressed(eKey::W))
    {
        CameraPosition += Forward * 100.f * g_DeltaTime;
    }
    if (KeyMgr->GetKeyPressed(eKey::S))
    {
        CameraPosition -= Forward * 100.f * g_DeltaTime;
    }
    if (KeyMgr->GetKeyPressed(eKey::A))
    {
        CameraPosition -= Right * 100.f * g_DeltaTime;
    }
    if (KeyMgr->GetKeyPressed(eKey::D))
    {
        CameraPosition += Right * 100.f * g_DeltaTime;
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

    if (KeyMgr->GetKeyPressed(eKey::MOUSE_LBUTTON))
    {
        Vec2 Dir = KeyMgr->GetMouseDir();

        Matrix RotByCamAxisY = Matrix::CreateFromAxisAngle(matCameraRotation.Up(), -Dir.x * g_DeltaTime);
        Matrix RotByCamAxisX = Matrix::CreateFromAxisAngle(matCameraRotation.Right(), Dir.y * g_DeltaTime);

        matCubeRotation *= RotByCamAxisX * RotByCamAxisY;
        
        CubeRotation = Matrix::ExtractPitchYawRollFromMatrix(matCubeRotation);
        
        
        
        //const Vec3 CubeCamDistance = CubePosition - CameraPosition;


        //Matrix Rot = Matrix::CreateFromAxisAngle()


        //Vec2 Dir = KeyMgr->GetMouseDir();

        //const Vec3 CubeCamDistance = CubePosition - CameraPosition;
        //
        ////1. 우선 카메라의 위치까지 큐브를 끌어오는 행렬을 만든다.
        //Matrix PulltoCameraPos = XMMatrixTranslationFromVector(-CubeCamDistance);
        //matCubeRotation = XMMatrixIdentity();
        //matCubeRotation *= PulltoCameraPos;


        ////2. 회전도 반대로 회전시켜서 카메라와 축을 일치시킨다.
        //Vec3 CubeCamRotation = CubeRotation - CameraRotation;

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
    else if (KeyMgr->GetKeyPressed(eKey::MOUSE_RBUTTON))
    {
        Vec2 Dir = KeyMgr->GetMouseDir();

        CameraRotation.x -= Dir.y * g_DeltaTime;
        CameraRotation.y += Dir.x * g_DeltaTime;
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
    //    Vec3 HalfAngle(CubeRotation.x / 2.f, CubeRotation.y / 2.f, CubeRotation.z / 2.f);

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
    //Vec3 BasisDir[3] =
    //{
    //    Vec3(1.f, 0.f, 0.f),
    //    Vec3(0.f, 1.f, 0.f),
    //    Vec3(0.f, 0.f, 1.f)
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
    AspectRatio = vRenderResolution.x / vRenderResolution.y;
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
    if (!FAILED(Context->Map(CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRes)))
    {
        memcpy_s(SubRes.pData, sizeof(Matrix), static_cast<void*>(&WorldViewProjectionMatrix), sizeof(Matrix));
        Context->Unmap(CB.Get(), 0);
    }

    Context->VSSetConstantBuffers(0, 1, CB.GetAddressOf());
}

void DX11::_15_DXLoop_SetShader()
{
    //출력 병합기에 새 렌더타겟을 등록.
    Context->OMSetRenderTargets(1, RTV.GetAddressOf(), DSV.Get());

    //화면 클리어
    Vec4 ClearColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
    Context->ClearRenderTargetView(RTV.Get(), ClearColor);
    Context->ClearDepthStencilView(DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    //입력 조립기에 레이아웃을 등록
    Context->IASetInputLayout(InputLayout.Get());

    Context->VSSetShader(VS.Get(), nullptr, 0);
    Context->PSSetShader(PS.Get(), nullptr, 0);
}

void DX11::_16_DXLoop_DrawCube()
{
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT VBStride = sizeof(Vertex);
    UINT VBOffset = 0;
    Context->IASetVertexBuffers(0, 1, VBCube.GetAddressOf(), &VBStride, &VBOffset);
    Context->IASetIndexBuffer(IBCube.Get(), DXGI_FORMAT_R32_UINT, 0);

    Context->DrawIndexed(36, 0, 0);
}

void DX11::_17_DXLoop_DrawAxis()
{
    //Set Tolology Linestrip and register Axis Vertex Buffer
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    UINT VBStride = sizeof(Vertex);
    UINT VBOffset = 0;
    Context->IASetVertexBuffers(0, 1, VBAxis.GetAddressOf(), &VBStride, &VBOffset);

    Context->IASetIndexBuffer(IBAxisX.Get(), DXGI_FORMAT_R32_UINT, 0);
    Context->DrawIndexed(2, 0, 0);
    Context->IASetIndexBuffer(IBAxisY.Get(), DXGI_FORMAT_R32_UINT, 0);
    Context->DrawIndexed(2, 0, 0);
    Context->IASetIndexBuffer(IBAxisZ.Get(), DXGI_FORMAT_R32_UINT, 0);
    Context->DrawIndexed(2, 0, 0);
}

void DX11::_18_DXLoop_FlipSwapChain()
{
    SwapChain->Present(1, 0);
}

void DX11::DXLoop()
{
    _9_DXLoop_UpdateTime();
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

void DX11::set_resolution(const Vector2& _size)
{

}
