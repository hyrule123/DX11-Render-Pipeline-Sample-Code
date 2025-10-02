#pragma once
#include "global.h"

//설명: 직접 계산한 수식 함수
class MyMath
{
public:
	inline constexpr float to_radian(float _radian) { return _radian * g_radian_to_degree_factor; }
	inline constexpr float to_degree(float _degree) { return _degree * g_degreeToRadianFactor; }

	//사원수로부터 회전행렬을 생성
	static Matrix get_rotation_matrix(const Quaternion& _q);

	//회전의 기준이 될 축 벡터와 회전시키고자 하는 각도를 통해 사원수를 생성
	static Quaternion get_quaternion(Vector3 _axis, float _radian);

private:
	MyMath() = delete;
	~MyMath() = delete;
	MyMath(const MyMath&) = delete;
	MyMath(MyMath&&) = delete;
};

