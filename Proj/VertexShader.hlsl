#include "ShaderHeader.hlsli"

VS_OUT VS(VS_IN _input)
{
    VS_OUT OUT = (VS_OUT) 0.f;
    
    //Clip Space의 정점 구하기
    //https://www.notion.so/hyrule1/3D-Graphics-Study-250cb63f18c18074b5dcca4609f4b447?p=272cb63f18c180bd9bf1d338b2e8f145&pm=s
    OUT.vPosition = mul(float4(_input.vLocalPos, 1.f), matWVP);
    OUT.vColor = _input.vColor;

    return OUT;
}