Texture2D colorMap : register(t0);
Texture2D colorMap2 : register(t1);
Texture2D TexturaAsfalto : register(t2);
Texture2D TexturaAsfaltoNormal : register(t3);
Texture2D TexturaCesped : register(t4);
Texture2D TexturaCespedNormal : register(t5);
Texture2D TexturaBanqueta : register(t6);
Texture2D TexturaBanquetaNormal : register(t7);
Texture2D blendMap : register(t8);
SamplerState colorSampler : register(s0);

cbuffer cbChangerEveryFrame : register(b0)
{
	matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
	matrix projMatrix;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangente : NORMAL1;
	float3 binormal : NORMAL2;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangent : NORMAL1;
	float3 binorm : NORMAL2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	vsOut.blendTex = vertex.blendTex;
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangent = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.binorm = normalize(mul(vertex.binormal, worldMatrix));

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 fColor = float4(1,0,0,1);

	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	//Texturas Color
	float4 text = colorMap.Sample(colorSampler, pix.tex0 * 10);
	float4 textAsf = TexturaAsfalto.Sample(colorSampler, pix.tex0 * 7);
	float4 textCes = TexturaCesped.Sample(colorSampler, pix.tex0 * 7);
	float4 textBan = TexturaBanqueta.Sample(colorSampler, pix.tex0 * 7);
	float4 mezclaText = blendMap.Sample(colorSampler, pix.blendTex);
	text = lerp(text, textBan, mezclaText.r);
	text = lerp(text, textCes, mezclaText.g);
	text = lerp(text, textAsf, mezclaText.b);
	
	//Texturas Normales
	float4 text2 = colorMap2.Sample(colorSampler, pix.tex0 * 10);
	float4 textAsfNor = TexturaAsfaltoNormal.Sample(colorSampler, pix.tex0 * 7);
	float4 textCesNor = TexturaCespedNormal.Sample(colorSampler, pix.tex0 * 7);
	float4 textBanNor = TexturaBanquetaNormal.Sample(colorSampler, pix.tex0 * 7);
	float4 mezclaTextNor = blendMap.Sample(colorSampler, pix.blendTex);
	text2 = lerp(text2, textBanNor, mezclaTextNor.r);
	text2 = lerp(text2, textCesNor, mezclaTextNor.g);
	text2 = lerp(text2, textAsfNor, mezclaTextNor.b);

	/*float4 alphaBlend = blendMap.Sample(colorSampler, pix.blendTex);
	float4 textf = (text * alphaBlend) + ((1.0 - alphaBlend) * text2);*/

	float3 bump = 2.0 * text2 - 1.0;
	float3x3 TBN = { {pix.tangent}, {pix.binorm}, {pix.normal} };
	float3 newnormal = mul(TBN, bump);


	float3 DiffuseDirection = float3(0.5f, -1.0f, 0.0f);
	float4 DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float3 diffuse = dot(-DiffuseDirection, newnormal);
	diffuse = saturate(diffuse*DiffuseColor.rgb);
	diffuse = saturate(diffuse + ambient);

	fColor = float4(text.rgb * diffuse, 1.0f);

	return fColor;
}