#include "CubeWorld.h"

#include "MyMath.h"

#include "Manager.h"
#include "Input.h"
#include "Timer.h"
#include "Scene.h"

#include "Camera.h"
#include "CubeModel.h"

CubeWorld::CubeWorld() {}

void CubeWorld::init()
{
	m_scale = Vector3(50.f, 50.f, 50.f);
}

CubeWorld::~CubeWorld()
{

}

void CubeWorld::update()
{
    Manager& manager = Manager::get_inst();
    Input& input = manager.get_Input_inst();
    float deltatime = manager.get_Timer_inst().get_deltatime();
    Scene& scene = manager.get_Scene_inst();

    if (input.GetKeyPressed(eKey::MOUSE_LBUTTON))
    {
        Vector2 mouse_dir = input.GetMouseDir();
        rotate_by_cam_view(scene.get_camera(), mouse_dir, deltatime);
    }

    //크기
    Matrix scale_mat = Matrix::Identity;

    scale_mat.m[0][0] = m_scale.x;
    scale_mat.m[1][1] = m_scale.y;
    scale_mat.m[2][2] = m_scale.z;

    //회전
    Matrix rot_mat = MyMath::get_rotation_matrix(m_rotation);

    //이동
    Matrix trans_mat = Matrix::Identity;
    trans_mat._41 = m_position.x;
    trans_mat._42 = m_position.y;
    trans_mat._43 = m_position.z;


    //월드행렬
    m_world_mat = scale_mat * rot_mat * trans_mat;
}

void CubeWorld::render(const Matrix& _view_mat, const Matrix& _proj_mat)
{
    Matrix WVP = m_world_mat * _view_mat * _proj_mat;
    
    m_model->render(WVP);
}

void CubeWorld::rotate_by_cam_view(Camera* _cam, Vector2 _mouse_dir, float _deltatime)
{
    if (nullptr == _cam) { return; }

    //카메라의 회전 행렬을 가져온다.
    const Matrix& rot_mat = _cam->get_rotation_matrix();
    

    //아이디어: 마우스 방향을 벡터로 계산하고
    //수직 벡터를 구한뒤 그 벡터로 한번에 회전?(사원수)
    
    
    //마우스의 X, Y 좌표 변화를 회전으로 변환하기

    /*
<-Y축 회전->(키보드의 X 변화량)
    |           
    |            ↑
    |________ X축 회전(키보드의 Y 변화량)
    */ //        ↓

    //직접 사원수 만들어 보기

    //회전량을 구한다
    float theta_x = _mouse_dir.y * _deltatime;
    Vector3 axis_x = Vector3{ rot_mat._21, rot_mat._22, rot_mat._23 };
    Quaternion x_quat = MyMath::get_quaternion(axis_x, theta_x);
    
    float theta_y = _mouse_dir.x * _deltatime;
    Vector3 axis_y = Vector3{ rot_mat._11, rot_mat._12, rot_mat._13 };

    Quaternion y_quat = MyMath::get_quaternion(axis_y, theta_y);

    Quaternion final_quat = x_quat * y_quat;

    m_rotation *= final_quat;
}
