
struct VS_INPUT_INSTANCE
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;

	float4 Mat1	  :	TEXCOORD1;
	float4 Mat2	  :	TEXCOORD2;
	float4 Mat3	  :	TEXCOORD3;
	float4 Mat4	  :	TEXCOORD4;

	float4 ColorPower	  :	TEXCOORD5;
	float4 WPosRange	  :	TEXCOORD6;

};
struct VS_OUTPUT_INSTANCE
{
	float4 Pos    : POSITION;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

VS_OUTPUT_INSTANCE VS_Instance(VS_INPUT_INSTANCE In)
{
	VS_OUTPUT_INSTANCE Out = (VS_OUTPUT_INSTANCE)0;

	float4x4 l_mWMat;

	l_mWMat[0][0] = In.Mat1.x;	l_mWMat[0][1] = In.Mat1.y;	l_mWMat[0][2] = In.Mat1.z;	l_mWMat[0][3] = In.Mat1.w;
	l_mWMat[1][0] = In.Mat2.x;	l_mWMat[1][1] = In.Mat2.y;	l_mWMat[1][2] = In.Mat2.z;	l_mWMat[1][3] = In.Mat2.w;
	l_mWMat[2][0] = In.Mat3.x;	l_mWMat[2][1] = In.Mat3.y;	l_mWMat[2][2] = In.Mat3.z;	l_mWMat[2][3] = In.Mat3.w;
	l_mWMat[3][0] = In.Mat4.x;	l_mWMat[3][1] = In.Mat4.y;	l_mWMat[3][2] = In.Mat4.z;	l_mWMat[3][3] = In.Mat4.w;

	float4x4 l_mWVPMat = mul(mul(l_mWMat, g_mVMatrix), g_mPMatrix);
	Out.Pos = mul(In.Pos, l_mWVPMat);
	Out.Color.rgb =In.Color;
	Out.Color.a = 1.0f;
	Out.Tex = In.Tex;

	return Out;
}
float4 PS_Instance(VS_OUTPUT_INSTANCE In) :COLOR
{
	float4 Out;
	Out = In.Color * tex2D(DecaleSamp, In.Tex);
	return Out;
}

technique Instancing
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;
		VertexShader = compile vs_3_0 VS_Instance();
		PixelShader = compile ps_3_0 PS_Instance();
	}
}

// ピクセルへ送る構造体
struct VS_POINTLIGHT_INSTANCING
{
	float4 Pos		:	POSITION;
	float4 wvpPos	:	TEXCOORD0;

	float4 ColorPower	  :	TEXCOORD1;
	float4 VPosRange	  :	TEXCOORD2;

};

//------------------------------------------------------
//		PointLightSphereの頂点シェーダー
//------------------------------------------------------
VS_POINTLIGHT_INSTANCING VS_PointLightSphereInstancing(VS_INPUT_INSTANCE In)
{
	VS_POINTLIGHT_INSTANCING Out = (VS_POINTLIGHT_INSTANCING)0;

	// ワールド行列を組み立てる
	float4x4 l_mWMat;

	l_mWMat[0][0] = In.Mat1.x;	l_mWMat[0][1] = In.Mat1.y;	l_mWMat[0][2] = In.Mat1.z;	l_mWMat[0][3] = In.Mat1.w;
	l_mWMat[1][0] = In.Mat2.x;	l_mWMat[1][1] = In.Mat2.y;	l_mWMat[1][2] = In.Mat2.z;	l_mWMat[1][3] = In.Mat2.w;
	l_mWMat[2][0] = In.Mat3.x;	l_mWMat[2][1] = In.Mat3.y;	l_mWMat[2][2] = In.Mat3.z;	l_mWMat[2][3] = In.Mat3.w;
	l_mWMat[3][0] = In.Mat4.x;	l_mWMat[3][1] = In.Mat4.y;	l_mWMat[3][2] = In.Mat4.z;	l_mWMat[3][3] = In.Mat4.w;

	// ポジション
	float4x4 l_mWVMat = mul(l_mWMat, g_mVMatrix);
	float4x4 l_mWVPMat = mul(l_mWVMat, g_mPMatrix);
	Out.Pos = mul(In.Pos, l_mWVPMat);

	//　フェッチ用
	Out.wvpPos = Out.Pos;

	// ライトに必要なデータを渡す
	Out.ColorPower = In.ColorPower;
	//Out.VPosRange = In.WPosRange;

	// カメラ空間変換
	float4x4 mat = g_mVMatrix;
	float3 l_vViewPos;
	l_vViewPos.x = In.WPosRange.x * mat._11 + In.WPosRange.y * mat._21 + In.WPosRange.z * mat._31 + mat._41;
	l_vViewPos.y = In.WPosRange.x * mat._12 + In.WPosRange.y * mat._22 + In.WPosRange.z * mat._32 + mat._42;
	l_vViewPos.z = In.WPosRange.x * mat._13 + In.WPosRange.y * mat._23 + In.WPosRange.z * mat._33 + mat._43;

	
	Out.VPosRange.xyz = l_vViewPos;
	Out.VPosRange.a = In.WPosRange.a;

	return Out;
}


DEF_POINTLIGHT PS_PointLightSphereInstancing(VS_POINTLIGHT_INSTANCING In)
{
	DEF_POINTLIGHT OUT = (DEF_POINTLIGHT)0;

	//スクリーン空間をテクスチャ座標に 
	//In.wvpPos.x += 0.5f;// 
	//In.wvpPos.y -= 0.5f;// 

	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	//ScreenTex.x += 0.001f;
	// 必要な情報を取得　
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);//
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	// PLのPosとこのピクセルの位置で←ポジション＆距離
	float3 l_vViewLightVec = In.VPosRange.xyz - (Pos.xyz);
	float dist = pow(max(0.0f, 1.0f - (length(l_vViewLightVec) / In.VPosRange.a)), 2);//←数値をいじり絞る

																				   // 法線マップから陰影遮断
	l_vViewLightVec = normalize(l_vViewLightVec);// 光<-場所ベクトルへ
	float rate = max(0, dot(Normal.xyz, l_vViewLightVec.xyz));

	OUT.color.rgb = (In.ColorPower.xyz*(dist*rate))*In.ColorPower.a;
	OUT.color.a = 1.0f;

	//OUT.color.rgb = float3(1, 0, 1);
	//OUT = In.Color * tex2D(DecaleSamp, In.Tex);
	return OUT;

}

//
technique PointLightSphereInstancing
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;//　追加
		DestBlend = One;
		CullMode = Cw;	// カリングはなし 法線方向にのばしたいけど中を見せたい
		ZEnable = FALSE;	// 奥行考慮はプレイヤーが前にいると困るのでなし
		ZWriteEnable = FALSE;
		VertexShader = compile vs_2_0 VS_PointLightSphereInstancing();
		PixelShader = compile ps_2_0 PS_PointLightSphereInstancing();
	}
}