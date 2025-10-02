#pragma once

#include "settings.h"

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11")

#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler")

#include <wrl.h>
using namespace Microsoft::WRL;

#define CHKFAIL(hr)      if(((HRESULT)(hr)) < 0) assert(false)

//////// STL ////////////
#include <vector>
#include <memory>
#include <cmath>
////////////////////

#include <FW1FontWrapper/FW1FontWrapper.h>
#if defined (_WIN64) && (_DEBUG)

#pragma comment (lib, "FW1FontWrapper/x64/Debug/FW1FontWrapper.lib")

#elif defined (_WIN64) && !(_DEBUG)
#include <x64/Release/FW1FontWrapper/FW1FontWrapper.h>
#pragma comment (lib, "FW1FontWrapper/x64/Release/FW1FontWrapper.lib")

#endif

//MACRO
#define SAFE_DELETE(_ptr) if(_ptr) { delete _ptr; }

#define ERROR_MESSAGE(_msg) MessageBoxW(NULL, _msg, L"Error", MB_OK)
//

//enum
enum class eProjMode
{
	Projection, 
	Orthographic
};
//

//구조체
struct Vertex
{
	Vector3 vPos;
	Vector4 vColor;
};

struct Transform
{
	Matrix MatWVP;
};

//전역 변수
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

//1(rad) / 180 * PI = PI / 180
constexpr const float g_radian_to_degree_factor = DirectX::XM_PI / 180.f;

//1(deg) / PI * 180 = 180 / PI
constexpr const float g_degreeToRadianFactor = 180.f / DirectX::XM_PI;