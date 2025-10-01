#include "CubeWorld.h"

#include "MyMath.h"

#include "Manager.h"
#include "Input.h"
#include "Timer.h"

CubeWorld::CubeWorld()
{

}

void CubeWorld::init()
{
	m_scale = Vector3(50.f, 50.f, 50.f);
}

CubeWorld::~CubeWorld()
{

}

void CubeWorld::update()
{
    //크기
    Matrix scale_mat = Matrix::Identity;

    scale_mat.m[0][0] = m_scale.x;
    scale_mat.m[1][1] = m_scale.y;
    scale_mat.m[2][2] = m_scale.z;

    //회전
    Matrix rot_mat = MyMath::get_rotation_matrix(m_rotation);

    Input& input = Manager::get_inst().get_Input_inst();
    float deltatime = Manager::get_inst().get_Timer_inst().get_deltatime();

    if (input.GetKeyPressed(eKey::MOUSE_LBUTTON))
    {
        Vector2 Dir = input.GetMouseDir();

        Matrix RotByCamAxisY = Matrix::CreateFromAxisAngle(matCameraRotation.Up(), -Dir.x * deltatime);
        Matrix RotByCamAxisX = Matrix::CreateFromAxisAngle(matCameraRotation.Right(), Dir.y * deltatime);

        matCubeRotation *= RotByCamAxisX * RotByCamAxisY;

        CubeRotation = Matrix::ExtractPitchYawRollFromMatrix(matCubeRotation);



        //const Vector3 CubeCamDistance = m_position - CameraPosition;


        //Matrix Rot = Matrix::CreateFromAxisAngle()


        //Vector2 Dir = Input->GetMouseDir();

        //const Vector3 CubeCamDistance = m_position - CameraPosition;
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
    Matrix matTranslation = XMMatrixTranslationFromVector(m_position);
    //월드행렬
    m_world_mat = scale_mat * matCubeRotation * matTranslation;



    //size_t size = 0;

    //if (ManualCalculation)
    //{
    //   size = vecCubeVtxPos.size();
    //}


    ////m_scale Matrix
    //Matrix scale_mat =
    //{
    //    m_scale.x, 0,         0,          0,
    //    0,       m_scale.y,   0,          0,
    //    0,       0,         m_scale.z,    0,
    //    0,       0,         0,          1
    //};

    //if (ManualCalculation)
    //{
    //    for (size_t i = 0; i < size; ++i)
    //    {
    //        //Vector * Matrix
    //        XMVector4Transform(vecCubeVtxPos[i], scale_mat);
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