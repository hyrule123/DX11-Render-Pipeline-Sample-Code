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
	const Vector2 res = Manager::get_inst().get_DX11_inst().get_resolution();

	m_aspect_ratio = res.x / res.y;

	m_field_of_view = DEFAULT_FOV;
	m_Z_near = DEFAULT_Z_NEAR;
	m_Z_far = DEFAULT_Z_FAR;
	m_position = Vector3{ DEFAULT_CAM_POS_X, DEFAULT_CAM_POS_Y, DEFAULT_CAM_POS_Z };
	
	m_rotation = Quaternion::Identity;
	
	calculate_view_matrix();
	
	m_projection_mode = eProjMode::Projection;
	if (eProjMode::Projection == m_projection_mode)
	{
		calculate_persp_proj_matrix();
	}
	else if (eProjMode::Orthographic == m_projection_mode)
	{
		calculate_ortho_proj_matrix();
	}
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
		m_position += Forward * 100.f * deltatime;
		is_transform_changed = true;
	}
	if (input.GetKeyPressed(eKey::S))
	{
		m_position -= Forward * 100.f * deltatime;
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
		Vector2 dir = input.GetMouseDir();

		Quaternion x_quat = MyMath::get_quaternion(Vector3::UnitX, dir.y * deltatime);
		Quaternion y_quat = MyMath::get_quaternion(Vector3::UnitY, dir.x * deltatime);

		m_rotation = m_rotation * x_quat * y_quat;

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
	m_view_matrix._31 = -m_view_matrix._31;
	m_view_matrix._32 = -m_view_matrix._32;
	m_view_matrix._33 = -m_view_matrix._33;
	m_view_matrix._34 = -m_view_matrix._34;
}

void Camera::calculate_ortho_proj_matrix()
{
	m_projection_matrix = Matrix::Identity;

	Vector2 res = Manager::get_inst().get_DX11_inst().get_resolution();

	m_projection_matrix.m[0][0] = 2.f / res.x;
	m_projection_matrix.m[1][1] = 2.f / res.y;

	m_projection_matrix.m[2][2] = 1.f / (m_Z_far - m_Z_near);
	m_projection_matrix.m[3][2] = -m_Z_near / (m_Z_far - m_Z_near);
}

void Camera::calculate_persp_proj_matrix()
{
	m_projection_matrix = Matrix::Identity;

	float fov_half = m_field_of_view / 2.f;
	float tan_fov = std::tan(fov_half);

	m_projection_matrix.m[0][0] = 1.f / (tan_fov * m_aspect_ratio);
	m_projection_matrix.m[1][1] = 1.f / tan_fov;

	m_projection_matrix.m[2][2] = m_Z_far / (m_Z_far - m_Z_near);
	m_projection_matrix.m[2][3] = 1.f;

	m_projection_matrix.m[3][2] = (-1.f * m_Z_far * m_Z_near) / (m_Z_far - m_Z_near);
}

