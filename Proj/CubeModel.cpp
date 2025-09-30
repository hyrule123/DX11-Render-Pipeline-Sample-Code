#include "CubeModel.h"

#include "Manager.h"
#include "DX11.h"

#include "VertexShader.h"
#include "PixelShader.h"

CubeModel::CubeModel()
{
}

void CubeModel::init()
{
}


CubeModel::~CubeModel()
{
}

void CubeModel::create_input_assembler()
{
    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11Device> device = dx.get_device();

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

    CHKFAIL(device->CreateInputLayout(IADesc, 2, g_VS, sizeof(g_VS), InputLayout.GetAddressOf()));
}

void CubeModel::create_shader()
{
    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11Device> device = dx.get_device();

    CHKFAIL(device->CreateVertexShader(g_VS, sizeof(g_VS), nullptr, &VS));
    CHKFAIL(device->CreatePixelShader(g_PS, sizeof(g_PS), nullptr, &PS));
}

void CubeModel::create_const_buffer()
{
    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11Device> device = dx.get_device();

    //계산된 WVP Matrix를 보내는 용도로 사용할 상수 버퍼
    D3D11_BUFFER_DESC Desc = {};
    Desc.ByteWidth = sizeof(Matrix);
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    CHKFAIL(device->CreateBuffer(&Desc, nullptr, CB.GetAddressOf()));
}

void CubeModel::create_cube_mesh()
{
    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11Device> device = dx.get_device();

    //정점정보 생성
    Vertex v = {};

    v.vPos = Vector3(-0.5f, 0.5f, -0.5f);
    v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.5f, 0.5f, -0.5f);
    v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.5f, -0.5f, -0.5f);
    v.vColor = Vector4(0.f, 0.f, 1.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(-0.5f, -0.5f, -0.5f);
    v.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);


    v.vPos = Vector3(-0.5f, 0.5f, 0.5f);
    v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.5f, 0.5f, 0.5f);
    v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.5f, -0.5f, 0.5f);
    v.vColor = Vector4(0.f, 0.f, 1.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(-0.5f, -0.5f, 0.5f);
    v.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    //정점버퍼 생성
    D3D11_BUFFER_DESC VBDesc = {};
    VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    VBDesc.Usage = D3D11_USAGE_DYNAMIC;

    VBDesc.ByteWidth = (UINT)(sizeof(Vertex) * vecVBCube.size());

    D3D11_SUBRESOURCE_DATA VtxData = {};
    VtxData.pSysMem = (void*)vecVBCube.data();

    CHKFAIL(device->CreateBuffer(&VBDesc, &VtxData, VBCube.GetAddressOf()));

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

    CHKFAIL(device->CreateBuffer(&IBDesc, &IdxData, IBCube.GetAddressOf()));
}

void CubeModel::create_axis_mesh()
{
    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11Device> device = dx.get_device();

    Vertex v = {};

    //=========Create Axis Buffer===============
    //Origin Vertex
    v.vPos = Vector3(0.f, 0.f, 0.f);
    v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
    vecVBAxis.push_back(v);

    //X Vertex
    v.vPos = Vector3(1.5f, 0.f, 0.f);
    v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
    vecVBAxis.push_back(v);
    arrIBAxisX[0] = 0u;
    arrIBAxisX[1] = 1u;

    //Y Vertex
    v.vPos = Vector3(0.f, 1.5f, 0.f);
    v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
    vecVBAxis.push_back(v);
    arrIBAxisY[0] = 0u;
    arrIBAxisY[1] = 2u;

    //Z Vertex
    v.vPos = Vector3(0.f, 0.f, 1.5f);
    v.vColor = Vector4(0.f, 0.f, 1.f, 1.f);
    vecVBAxis.push_back(v);
    arrIBAxisZ[0] = 0u;
    arrIBAxisZ[1] = 3u;

    //Create Axis Vertex Buffer
    D3D11_BUFFER_DESC VBDesc = {};
    VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    VBDesc.Usage = D3D11_USAGE_DYNAMIC;
    VBDesc.ByteWidth = (UINT)(sizeof(Vertex) * vecVBAxis.size());

    D3D11_SUBRESOURCE_DATA VtxData = {};
    VtxData.pSysMem = (void*)vecVBAxis.data();
    CHKFAIL(device->CreateBuffer(&VBDesc, &VtxData, VBAxis.GetAddressOf()));

    //Create Axis Index Buffer
    D3D11_BUFFER_DESC IBDesc = {};
    IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IBDesc.CPUAccessFlags = 0;
    IBDesc.Usage = D3D11_USAGE_DEFAULT;
    
    //X
    IBDesc.ByteWidth = (UINT)(2u * sizeof(UINT));
    D3D11_SUBRESOURCE_DATA IdxData = {};
    IdxData.pSysMem = (void*)arrIBAxisX;
    CHKFAIL(device->CreateBuffer(&IBDesc, &IdxData, IBAxisX.GetAddressOf()));

    //Y
    IBDesc.ByteWidth = (UINT)(2u * sizeof(UINT));
    IdxData = {};
    IdxData.pSysMem = (void*)arrIBAxisY;
    CHKFAIL(device->CreateBuffer(&IBDesc, &IdxData, IBAxisY.GetAddressOf()));

    //Z
    IBDesc.ByteWidth = (UINT)(2u * sizeof(UINT));
    IdxData = {};
    IdxData.pSysMem = (void*)arrIBAxisZ;
    CHKFAIL(device->CreateBuffer(&IBDesc, &IdxData, IBAxisZ.GetAddressOf()));
}


