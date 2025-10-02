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
