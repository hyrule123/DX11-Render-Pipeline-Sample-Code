#include "Camera.h"

#include "Manager.h"
#include "DX11.h"
#include "Timer.h"
#include "Input.h"

#include "MyMath.h"

#include "CubeModel.h"
#include "CubeWorld.h"


Camera::Camera() {}

void Camera::init()
{
	m_position = Vector3{ DEFAULT_CAM_POS_X, DEFAULT_CAM_POS_Y, DEFAULT_CAM_POS_Z };
	
	m_rotation = Quaternion::Identity;
	
	calculate_view_matrix();
	
	m_projection_mode = eProjMode::Projection;

	calculate_persp_proj_matrix(DEFAULT_Z_NEAR, DEFAULT_Z_FAR, DEFAULT_FOV, (float)DEFAULT_WIDTH / (float)DEFAULT_HEIGHT);
}

Camera::~Camera()
{

}

void Camera::update()
{
	m_rot_matrix = MyMath::get_rotation_matrix(m_rotation);

	Vector3 Forward = m_rot_matrix.Forward();
	Vector3 Right = m_rot_matrix.Right();
	Vector3 Up = m_rot_matrix.Up();

	Input& input = Manager::get_inst().get_Input_inst();
	float deltatime = Manager::get_inst().get_Timer_inst().get_deltatime();

	bool is_transform_changed = false;
	if (input.GetKeyPressed(eKey::W))
	{
		m_position -= Forward * 100.f * deltatime;
		is_transform_changed = true;
	}
	if (input.GetKeyPressed(eKey::S))
	{
		m_position += Forward * 100.f * deltatime;
		is_transform_changed = true;
	}
	if (input.GetKeyPressed(eKey::A))
	{
		m_position -= Right * 100.f * deltatime;
		is_transform_changed = true;
	}
	if (input.GetKeyPressed(eKey::D))
	{
		m_position += Right * 100.f * deltatime;
		is_transform_changed = true;
	}

	if (input.GetKeyPressed(eKey::MOUSE_RBUTTON))
	{
		/*
<-Y축 회전-> yaw(커서의 X 변화량)
	|
	|            ↑
	|________ X축 회전, pitch(커서의 Y 변화량)
				 ↓

	* RH 기준이므로,
	마우스 X+ -> Y축 yaw +방향 회전 (정방향)
	마우스 Y- -> X축 pitch +방향 회전 (역방향: 부호 변환 필요)
	*/
		Vector2 dir = input.GetMouseDir() * deltatime * 30.f;
		m_pitchyaw_degree.x -= dir.y;
		m_pitchyaw_degree.y += dir.x;

		//pitch clamp( -89 ~ 89 )
		if (m_pitchyaw_degree.x < -89.f) { m_pitchyaw_degree.x = -89.f; }
		if (m_pitchyaw_degree.x > 89.f) { m_pitchyaw_degree.x = 89.f; }

		Quaternion x_quat = MyMath::get_quaternion(Vector3::Right, MyMath::to_radian(m_pitchyaw_degree.x));
		Quaternion y_quat = MyMath::get_quaternion(Vector3::Up, MyMath::to_radian(m_pitchyaw_degree.y));

		m_rotation = x_quat * y_quat;

		is_transform_changed = true;
	}

	//트랜스폼에 변화가 있었다면 뷰 행렬을 새로 계산한다.
	if (is_transform_changed)
	{
		calculate_view_matrix();
	}
}

void Camera::render(CubeWorld* _world)
{
	_world->render(m_view_matrix, m_projection_matrix);
}

void Camera::on_resolution_change(float _width, float _height)
{
	if (m_projection_mode == eProjMode::Projection)
	{
		calculate_persp_proj_matrix(m_near, m_far, m_fov_deg, _width / _height);
	}
	else if (m_projection_mode == eProjMode::Orthographic)
	{
		float width_half = _width / 2.f;
		float height_half = _height / 2.f;
		calculate_ortho_proj_matrix(-width_half, width_half, -height_half, height_half);
	}
}

void Camera::calculate_view_matrix()
{
	constexpr int X = 0, Y = 1, Z = 2;
	Matrix pos_mat_inv = Matrix::Identity;
	pos_mat_inv.m[3][X] = -m_position.x;
	pos_mat_inv.m[3][Y] = -m_position.y;
	pos_mat_inv.m[3][Z] = -m_position.z;

	Quaternion rot_quat_inv = m_rotation;
	rot_quat_inv.Normalize();	//정규화해서 단위 사원수로 변환
	rot_quat_inv.Conjugate();	//단위 사원수의 역원은 켤레 사원수!, 켤레 사원수는 기존의 회전을 반대로 회전시킨다.
	
	Matrix rot_mat_inv = MyMath::get_rotation_matrix(rot_quat_inv);

	//V = (R * T)^-1 = T^-1 * R ^ -1
	m_view_matrix = pos_mat_inv * rot_mat_inv;

	//RH -> LH
	//Z축을 반전시켜주어야 함!
	//m_view_matrix._31 = -m_view_matrix._31;
	//m_view_matrix._32 = -m_view_matrix._32;
	//m_view_matrix._33 = -m_view_matrix._33;
	//m_view_matrix._34 = -m_view_matrix._34;
}

void Camera::calculate_ortho_proj_matrix(float _l, float _r, float _b, float _t)
{
	//m_projection_matrix = MyMath::get_orthographic_projection_matrix(m_Z_near, m_Z_far, res.x, res.y);
}

void Camera::calculate_persp_proj_matrix(float _near, float _far, float _fov_deg, float _aspect_ratio)
{
	assert(0.f < _near);
	assert(0.f < _far);
	assert(0.f < _fov_deg);
	assert(180.f > _fov_deg);
	assert(0 < _aspect_ratio);

	m_near = _near;
	m_far = _far;
	m_fov_deg = _fov_deg;
	m_aspect_ratio = _aspect_ratio;

	m_projection_matrix = MyMath::get_perspective_projection_matrix(m_near, m_far, m_fov_deg, _aspect_ratio);
}

