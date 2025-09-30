#include "Camera.h"

#include "Manager.h"
#include "DX11.h"

#include <cmath>


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
	
	/*
	\begin{bmatrix} 
	1 - 2(c^2 + d^2) & 2(ad + bc) & 2(bd - ac) \\
	2(bc - ad) & 1 - 2(b^2 + d^2) & 2(ab + cd) \\
	2(ac + bd) & 2(cd - ab) & 1 - 2(b^2 + c^2)
	\end{bmatrix}
	*/
#define a w //scalar part
#define b x
#define c y
#define d z

	rot_quat_inv.x;

	Matrix rot_mat_inv = Matrix::Identity;

	//MATRIX ROW 1
	rot_mat_inv.m[0][0] = 1.f - 2.f * (rot_quat_inv.c * rot_quat_inv.c + rot_quat_inv.d * rot_quat_inv.d);
	rot_mat_inv.m[0][1] = 2.f * (rot_quat_inv.a * rot_quat_inv.d + rot_quat_inv.b * rot_quat_inv.c);
	rot_mat_inv.m[0][2] = 2.f * (rot_quat_inv.b * rot_quat_inv.d - rot_quat_inv.a * rot_quat_inv.c);

	//MATRIX ROW 2
	rot_mat_inv.m[1][0] = 2.f * (rot_quat_inv.b * rot_quat_inv.c - rot_quat_inv.a * rot_quat_inv.d);
	rot_mat_inv.m[1][1] = 1.f - 2.f * (rot_quat_inv.b * rot_quat_inv.b + rot_quat_inv.d * rot_quat_inv.d);
	rot_mat_inv.m[1][2] = 2.f * (rot_quat_inv.a * rot_quat_inv.b + rot_quat_inv.c * rot_quat_inv.d);

	//MATRIX ROW 3
	rot_mat_inv.m[2][0] = 2.f * (rot_quat_inv.a * rot_quat_inv.c + rot_quat_inv.b * rot_quat_inv.d);
	rot_mat_inv.m[2][1] = 2.f * (rot_quat_inv.c * rot_quat_inv.d - rot_quat_inv.a * rot_quat_inv.b);
	rot_mat_inv.m[2][2] = 1.f - 2.f * (rot_quat_inv.b * rot_quat_inv.b + rot_quat_inv.c * rot_quat_inv.c);

#undef a
#undef b
#undef c
#undef d


	//V = (T * R)^-1 = R^-1 * T^-1
	m_view_matrix = rot_mat_inv * pos_mat_inv;
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

