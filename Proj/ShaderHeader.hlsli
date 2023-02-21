struct VS_IN
{
    float3 vLocalPos : POSITION;
    float4 vColor : COLOR;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
};

cbuffer Transform : register(b0)
{
    matrix matWVP;
}