#include "MyMath.h"

Matrix MyMath::get_rotation_matrix(const Quaternion& _q)
{
#define a w //scalar part
#define b x
#define c y
#define d z

	Matrix rot_mat = Matrix::Identity;

/*
begin{bmatrix}
1 - 2(c^2 + d^2) & 2(ad + bc) & 2(bd - ac) \\
2(bc - ad) & 1 - 2(b^2 + d^2) & 2(ab + cd) \\
2(ac + bd) & 2(cd - ab) & 1 - 2(b^2 + c^2)
end{bmatrix}
*/

	//MATRIX ROW 1
	rot_mat.m[0][0] = 1.f - 2.f * (_q.c * _q.c + _q.d * _q.d);
	rot_mat.m[0][1] = 2.f * (_q.a * _q.d + _q.b * _q.c);
	rot_mat.m[0][2] = 2.f * (_q.b * _q.d - _q.a * _q.c);

	//MATRIX ROW 2
	rot_mat.m[1][0] = 2.f * (_q.b * _q.c - _q.a * _q.d);
	rot_mat.m[1][1] = 1.f - 2.f * (_q.b * _q.b + _q.d * _q.d);
	rot_mat.m[1][2] = 2.f * (_q.a * _q.b + _q.c * _q.d);

	//MATRIX ROW 3
	rot_mat.m[2][0] = 2.f * (_q.a * _q.c + _q.b * _q.d);
	rot_mat.m[2][1] = 2.f * (_q.c * _q.d - _q.a * _q.b);
	rot_mat.m[2][2] = 1.f - 2.f * (_q.b * _q.b + _q.c * _q.c);

#undef a
#undef b
#undef c
#undef d

    return rot_mat;
}

Matrix MyMath::get_perspective_projection_matrix(float _n, float _f, float _fovY_deg, float _aspect_ratio)
{
	Matrix proj = Matrix::Identity;
	proj._11 = proj._22 = proj._33 = proj._44 = 0.f;

	/*
\therefore M_{proj} = \begin{bmatrix}
\frac{1}{\tan{(\frac{\gamma}{2})} \cdot \frac{w}{h} } & 0 & 0 & 0 \\
0 & \frac{1}{\tan{(\frac{\gamma}{2})}  } & 0 & 0 \\
0 & 0 & -\frac{f}{f-n} & -1\\
0 & 0 & -\frac{fn}{f-n}  & 0
\end{bmatrix}\\
\gamma = \text{FOV}\\
n = \text{near plane}\\
f = \text{far plane}
	*/

	float _fovY_rad = MyMath::to_radian(_fovY_deg);
	proj._11 = 1.f / (std::tan(_fovY_rad / 2.f) * _aspect_ratio);	// 참고: 1/tan 은 atan으로 대체 가능
	proj._22 = 1.f / (std::tan(_fovY_rad / 2.f));
	proj._33 = -(_f / (_f - _n));	proj._34 = -1;
	proj._43 = -(_f * _n / (_f - _n));

	return proj;
}

Matrix MyMath::get_orthographic_projection_matrix(float _n, float _f, float _width, float _height)
{
	Matrix ortho = Matrix::Identity;

	/*
P_{ortho} = 
\begin{bmatrix}
\frac{2}{W_{rect}} & & &\\
 & \frac{2}{H_{rect}} & & \\
 & & -\frac{1}{f - n} & \\
 & & -\frac{n}{f - n} & 1 \\
\end{bmatrix}\\
\text{cf)} W_{rect}, H_{rect}: \text{직육면체 변의 길이}
	*/
	
	ortho._11 = 2.f / _width;
	ortho._22 = 2.f / _height;
	ortho._33 = -(1.f / (_f - _n));
	ortho._43 = -(_n / (_f - _n));

	return ortho;
}

Quaternion MyMath::get_quaternion(Vector3 _axis, float _radian)
{
	Quaternion ret;

	//축을 정규화해서 크기를 1로 맞춘다.
	_axis.Normalize();
	
	//Vector part(i, j, k) *= sin(angle / 2)
	_axis *= std::sin(_radian / 2.f);

	//Scalar part(w) = cos(angle / 2)
	ret.w = std::cos(_radian / 2.f);

	ret.x = _axis.x;
	ret.y = _axis.y;
	ret.z = _axis.z;

	return ret;
}
