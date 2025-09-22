#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11")

#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler")

#include <wrl.h>
using namespace Microsoft::WRL;

#define CHKFAIL(hr)      if(((HRESULT)(hr)) < 0) assert(false)

#include <vector>

#include <FW1FontWrapper/FW1FontWrapper.h>
#if defined (_WIN64) && (_DEBUG)

#pragma comment (lib, "FW1FontWrapper/x64/Debug/FW1FontWrapper.lib")

#elif defined (_WIN64) && !(_DEBUG)
#include <x64/Release/FW1FontWrapper/FW1FontWrapper.h>
#pragma comment (lib, "FW1FontWrapper/x64/Release/FW1FontWrapper.lib")

#endif

//구조체
struct Vertex
{
	Vec3 vPos;
	Vec4 vColor;
};

struct Transform
{
	Matrix MatWVP;
};

//전역 변수
extern float g_DeltaTime;
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;