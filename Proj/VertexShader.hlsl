#include "ShaderHeader.hlsli"

VS_OUT VS(VS_IN _input)
{
    VS_OUT OUT = (VS_OUT) 0.f;
    
    OUT.vPosition = mul(float4(_input.vLocalPos, 1.f), matWVP);
    OUT.vColor = _input.vColor;
    
    
	//float temp = OUT.vPosition.x;
	//OUT.vPosition.x = OUT.vPosition.y;
	//OUT.vPosition.y = temp;

    return OUT;
}