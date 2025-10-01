#pragma once
#include "global.h"

//설명: 직접 계산한 수식 함수
class MyMath
{
public:
	static Matrix get_rotation_matrix(const Quaternion& _q);

private:
	MyMath() = delete;
	~MyMath() = delete;
	MyMath(const MyMath&) = delete;
	MyMath(MyMath&&) = delete;
};

