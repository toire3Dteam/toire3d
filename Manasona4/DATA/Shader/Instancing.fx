
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

// �s�N�Z���֑���\����
struct VS_POINTLIGHT_INSTANCING
{
	float4 Pos		:	POSITION;
	float4 wvpPos	:	TEXCOORD0;

	float4 ColorPower	  :	TEXCOORD1;
	float4 VPosRange	  :	TEXCOORD2;

};

//------------------------------------------------------
//		PointLightSphere�̒��_�V�F�[�_�[
//------------------------------------------------------
VS_POINTLIGHT_INSTANCING VS_PointLightSphereInstancing(VS_INPUT_INSTANCE In)
{
	VS_POINTLIGHT_INSTANCING Out = (VS_POINTLIGHT_INSTANCING)0;

	// ���[���h�s���g�ݗ��Ă�
	float4x4 l_mWMat;

	l_mWMat[0][0] = In.Mat1.x;	l_mWMat[0][1] = In.Mat1.y;	l_mWMat[0][2] = In.Mat1.z;	l_mWMat[0][3] = In.Mat1.w;
	l_mWMat[1][0] = In.Mat2.x;	l_mWMat[1][1] = In.Mat2.y;	l_mWMat[1][2] = In.Mat2.z;	l_mWMat[1][3] = In.Mat2.w;
	l_mWMat[2][0] = In.Mat3.x;	l_mWMat[2][1] = In.Mat3.y;	l_mWMat[2][2] = In.Mat3.z;	l_mWMat[2][3] = In.Mat3.w;
	l_mWMat[3][0] = In.Mat4.x;	l_mWMat[3][1] = In.Mat4.y;	l_mWMat[3][2] = In.Mat4.z;	l_mWMat[3][3] = In.Mat4.w;

	// �|�W�V����
	float4x4 l_mWVMat = mul(l_mWMat, g_mVMatrix);
	float4x4 l_mWVPMat = mul(l_mWVMat, g_mPMatrix);
	Out.Pos = mul(In.Pos, l_mWVPMat);

	//�@�t�F�b�`�p
	Out.wvpPos = Out.Pos;

	// ���C�g�ɕK�v�ȃf�[�^��n��
	Out.ColorPower = In.ColorPower;
	//Out.VPosRange = In.WPosRange;

	// �J������ԕϊ�
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

	//�X�N���[����Ԃ��e�N�X�`�����W�� 
	//In.wvpPos.x += 0.5f;// 
	//In.wvpPos.y -= 0.5f;// 

	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	//ScreenTex.x += 0.001f;
	// �K�v�ȏ����擾�@
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);//
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	// PL��Pos�Ƃ��̃s�N�Z���̈ʒu�Ł��|�W�V����������
	float3 l_vViewLightVec = In.VPosRange.xyz - (Pos.xyz);
	float dist = pow(max(0.0f, 1.0f - (length(l_vViewLightVec) / In.VPosRange.a)), 2);//�����l��������i��

																				   // �@���}�b�v����A�e�Ւf
	l_vViewLightVec = normalize(l_vViewLightVec);// ��<-�ꏊ�x�N�g����
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
		SrcBlend = SrcAlpha;//�@�ǉ�
		DestBlend = One;
		CullMode = Cw;	// �J�����O�͂Ȃ� �@�������ɂ̂΂��������ǒ�����������
		ZEnable = FALSE;	// ���s�l���̓v���C���[���O�ɂ���ƍ���̂łȂ�
		ZWriteEnable = FALSE;
		VertexShader = compile vs_2_0 VS_PointLightSphereInstancing();
		PixelShader = compile ps_2_0 PS_PointLightSphereInstancing();
	}
}