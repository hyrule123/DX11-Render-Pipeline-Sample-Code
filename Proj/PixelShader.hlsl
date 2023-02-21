#include "ShaderHeader.hlsli"

float4 PS(VS_OUT _in) : SV_Target
{
    float4 Color = (float4) 0.f;
    
    Color = _in.vColor;
    
    return Color;
}
