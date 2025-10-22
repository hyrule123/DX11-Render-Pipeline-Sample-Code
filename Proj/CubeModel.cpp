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
    create_input_assembler();
    create_shader();
    create_const_buffer();
    //create_cube_mesh();
    create_pyramid_mesh();
    create_axis_mesh();
}


CubeModel::~CubeModel()
{
}

void CubeModel::render(const Matrix& _WVP)
{
    std::vector<Vector4> wvps;

    for (auto& vb : vecVBCube)
    {
        Vector4 v = Vector4(vb.vPos, 1.f);

        Vector4 proj = Vector4::Transform(v, _WVP);

        wvps.push_back(proj);
    }

    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11DeviceContext> context = dx.get_context();

    {
        //입력 조립기에 레이아웃을 등록
        context->IASetInputLayout(InputLayout.Get());

        context->VSSetShader(VS.Get(), nullptr, 0);
        context->PSSetShader(PS.Get(), nullptr, 0);

        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        UINT VBStride = sizeof(Vertex);
        UINT VBOffset = 0;
        context->IASetVertexBuffers(0, 1, VBCube.GetAddressOf(), &VBStride, &VBOffset);
        context->IASetIndexBuffer(IBCube.Get(), DXGI_FORMAT_R32_UINT, 0);

        //상수버퍼에 WVP 행렬 데이터를 전송
        D3D11_MAPPED_SUBRESOURCE SubRes = {};
        if (!FAILED(dx.get_context()->Map(CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRes)))
        {
            memcpy_s(SubRes.pData, sizeof(Matrix), static_cast<const void*>(&_WVP), sizeof(Matrix));
            context->Unmap(CB.Get(), 0);
        }

        context->VSSetConstantBuffers(0, 1, CB.GetAddressOf());

        //그리기 명령(드로우콜)
        context->DrawIndexed((UINT)vecIBCube.size(), 0, 0);
    }

    {
        //축 렌더링
        //Set Tolology Linestrip and register Axis Vertex Buffer
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        UINT VBStride = sizeof(Vertex);
        UINT VBOffset = 0;
        context->IASetVertexBuffers(0, 1, VBAxis.GetAddressOf(), &VBStride, &VBOffset);

        context->IASetIndexBuffer(IBAxisX.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed(2, 0, 0);
        context->IASetIndexBuffer(IBAxisY.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed(2, 0, 0);
        context->IASetIndexBuffer(IBAxisZ.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed(2, 0, 0);
    }
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

    vecVBCube.clear();
    vecIBCube.clear();

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
    
    //오른손 좌표계와 왼손 좌표계는 Z축이 반전되어 있다
    //따라서 외적의 결과도 반대 방향이 되어야 하므로 index 순서를 시계 방향에서 반시계 방향으로 교체한다.
    /*
    UINT arrIB[] = {
    0, 2, 1,
    0, 3, 2,

    1, 6, 5,
    1, 2, 6,

    3, 6, 2,
    3, 7, 6,

    4, 3, 0,
    4, 7, 3,

    4, 1, 5,
    4, 0, 1,

    5, 7, 4,
    5, 6, 7
    };
    */

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

void CubeModel::create_pyramid_mesh()
{
    DX11& dx = Manager::get_inst().get_DX11_inst();
    ComPtr<ID3D11Device> device = dx.get_device();

    vecVBCube.clear();
    vecIBCube.clear();

    //정점정보 생성
    Vertex v = {};

    v.vPos = Vector3(0.0f, 0.f, 0.f);
    v.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.5f, 0.f, 1.f);
    v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.f, 1.f, 1.f);
    v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
    vecVBCube.push_back(v);

    v.vPos = Vector3(0.f, 0.f, 1.f);
    v.vColor = Vector4(0.f, 0, 1.f, 1.f);
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
    
    //UINT arrIB[] = { 
    //    0, 1, 2,
    //    0, 2, 3,
    //    0, 3, 1,
    //    1, 3, 2
    //};

    UINT arrIB[] = {
    0, 2, 1,
    0, 3, 2,
    0, 1, 3,
    1, 2, 3
    };
    
    Vector3 V1 = Vector3(0.5, 0, 1);
    Vector3 V2 = Vector3(0, 0, 1);
    Vector3 V3 = Vector3(0, 1, 1);

    Vector3 l1 = V2 - V1;
    Vector3 l2 = V3 - V2;
    Vector3 cross = l1.Cross(l2);

    //오른손 좌표계와 왼손 좌표계는 Z축이 반전되어 있다
    //따라서 외적의 결과도 반대 방향이 되어야 하므로 index 순서를 시계 방향에서 반시계 방향으로 교체한다.
    //UINT arrIB[] = {
    //    0, 2, 1,
    //    0, 3, 2,
    //    0, 1, 3,
    //    3, 1, 2
    //};

    vecIBCube.insert(vecIBCube.begin(), arrIB, arrIB + (sizeof(arrIB) / sizeof(UINT)));

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


