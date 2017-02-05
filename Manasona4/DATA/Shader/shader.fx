//------------------------------------------------------
//		�ϊ��s��
//------------------------------------------------------
float4x4 g_mWMatrix;		// ���[���h�ϊ��s��
float4x4 g_mVMatrix;		// �r���[�ϊ��s��
float4x4 g_mPMatrix;		// �v���W�F�N�V�����ϊ��s��

float4x4 g_mWVPMatrix;		// ���e�ϊ��s��
float4x4 g_mVPMatrix;		// ���[���h���W���灨�v���W�F�N�V�������W�֎����Ă����s��

float4x4 g_mInvPMatrix;	// �v���W�F�N�V�������W ->View���W�@�֖߂��t�s��
float4x4 g_mInvVMatrix;	// View���W ->���[���h���W�@�֖߂��t�s��
float4x4 g_mInvVPMatrix;	// �v���W�F�N�V�������W -> ���[���h���W �֖߂��t�s��

float4x4 g_mShadowProjection; // �e

//------------------------------------------------------
//		���֘A
//------------------------------------------------------
float3	g_vViewPos;		// ���[���h���W�̖ڂ̃|�W�V����

float3 g_vWLightVec = { 1.0f, -1.0f, 1.0f };// ���[���h��Ԃł̕��s���̌���

//float3 LightVec;		// ���s���̌���
float3 g_vViewLightVec;	// �r���[���W�ł̌���̌���
float3 g_vLightColor = { 1.0f, 1.0f, 1.0f };

float3 g_vSkyColor = { .4f, .6f, .6f };
float3 g_vGroundColor = { .6f, .4f, .4f };

float g_fRimPowerRate = 2.0f;		 // �������C�g�̌��̕� 
float g_fEmissivePowerRate = 2.0f;// �G�~�b�V�u�̕� �G�~�b�V�u������Ȃ����̓e�N�X����ς������̐��l��������
// ����0��

float g_fFogNear = 128.0f;
float g_fFogFar = 512.0f;
float3 g_vFogColor = { 0.2f, 0.3f, 0.35f };

/***************************/
//	�ŏ��̒��_�f�[�^
/***************************/
struct VS_INPUT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

//------------------------------------------------------
//		�e�N�X�`���T���v���[	
//------------------------------------------------------

// MinFilter	�k���t�B���^�Ɏg�p����郁�\�b�h
// MagFilter	�g��t�B���^�Ɏg�p����郁�\�b�h
// MipFilter	MIPMAP�Ɏg�p����郁�\�b�h
// AddressU	0 - 1�͈̔͊O�ɂ���u�e�N�X�`���[���W�̉����Ɏg�p����郁�\�b�h
// AddressV	0 - 1�͈̔͊O�ɂ���v�e�N�X�`���[���W�̉����Ɏg�p����郁�\�b�h
// LINEAR		�o�C���j�A��ԃt�B���^�����O�B�ړI�̃s�N�Z���ɊO�ڂ���2�~2�e�N�Z���̏d�ݕt�����ϗ̈���g�p
// NONE		MIPMAP�𖳌��ɂ��A�g��t�B���^���g�p
// CLAMP		�͈�[0.0, 1.0]�̊O�ɂ���e�N�X�`���[���W�ɂ́A���ꂼ��0.0�܂���1.0�̃e�N�X�`���[�J���[��ݒ�
// MIPMAP		���C���ƂȂ�e�N�X�`���̉摜��⊮����悤���O�v�Z����œK�����ꂽ�摜�Q�B�ڍׂ��̃��x���ɂ���ĉ摜��؂�ւ���

// WRAP...���Α����瓯���G���J��Ԃ��܂�
// CLAMP...�[�̐F���J��Ԃ��܂�
// BORDER...�����F�ɂ��܂�

texture Texture;
sampler DecaleSamp = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture NormalMap;	//	�@���}�b�v�e�N�X�`��
sampler NormalSamp = sampler_state
{
	Texture = <NormalMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture RoughnessMap;	//	�e���e�N�X�`��
sampler RoughnessSamp = sampler_state
{
	Texture = <RoughnessMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture MultiMap;		//	�}���`�}�b�v�e�N�X�`��
sampler MultiSamp = sampler_state
{
	Texture = <MultiMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;

	BorderColor = 0x00000000;
};

texture ToonShadowMap;	//	�g�D�[���̉e�p�e�N�X�`��
sampler ToonShadowSamp = sampler_state
{
	Texture = <ToonShadowMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

//********************************************************************
//
//		��{�R�c�V�F�[�_�[		
//
//********************************************************************

/***************************/
//	��TDNMesh�Ŏg�p
/***************************/
struct VSINPUT
{
	float4 Pos : POSITION;
	float4 Normal : NORMAL;
	float4 Color : COLOR;
	float2 UV : TEXCOORD0;
	// �C���X�^���V���O
	float4 worldPos : TEXCOORD1;
};

VSINPUT VS(VSINPUT In)
{
	VSINPUT Out = (VSINPUT)0;

	Out.Pos = mul(In.Pos, g_mWMatrix);
	Out.Pos += In.worldPos;

	//float4x4 g_mVPMatrix;
	//g_mVPMatrix = mul(g_mVMatrix, g_mPMatrix);

	Out.Pos = mul(Out.Pos, g_mVPMatrix);

	Out.UV = In.UV;
	Out.Normal = In.Normal;
	Out.Color = In.Color;
	return Out;
}

float4 PS(VSINPUT In) : COLOR
{
	float4 Out = 1;
	In.Color = min(1, In.Color);
	Out.rgb *= tex2D(DecaleSamp, In.UV);
	Out.rgb *= In.Color;
	//Out.a = alpha;
	Out.a = 1;
	return Out;
}

technique linecopy
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		Zenable = true;
		ZWriteEnable = true;

		// �V�F�[�_
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}

//------------------------------------------------------
//		���_�t�H�[�}�b�g
//------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos			: TEXCOORD1;//�@�s�N�Z���ɑ�����Ƀ��[���h����̃|�W�V�����ǉ�
	float4 ProjectionPos: TEXCOORD2;//�@�s�N�Z���ɑ�����ɃQ�[����ʃ��X�g�̃|�W�V�����ǉ�
};
//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT VS_Basic(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//TransMatrix��Pos����������wPos�̏�񐶐�
	Out.wPos = mul(In.Pos, g_mWMatrix);

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.ProjectionPos = Out.Pos;

	return Out;
}
//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Basic(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	�s�N�Z���F����
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);

	//OUT.r = 1.0f;
	OUT.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		�ʏ�`��e�N�j�b�N
//------------------------------------------------------
technique copy
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic();
	}
}

//------------------------------------------------------
//		���F�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Black(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	�s�N�Z���F����
	OUT.rgb = 0.0f;

	OUT.a = 1.0f;

	return OUT;
}
//------------------------------------------------------
//		���h��Ԃ��`��e�N�j�b�N
//------------------------------------------------------
technique black
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Black();
	}
}

//------------------------------------------------------
//		�J�����s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_CameraAlpha(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	�s�N�Z���F����
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);
	OUT.a = 0.5f;

	return OUT;
}
//------------------------------------------------------
//		�������`��e�N�j�b�N
//------------------------------------------------------
technique cameraAlpha
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_CameraAlpha();
	}
}


/***************************/
//	G_Buffer�p�̒��_�f�[�^
/***************************/

struct VS_DEFERRED
{
	float4 Pos				:POSITION;
	float4 Color			:COLOR0;
	float2 Tex				:TEXCOORD0;
	float3 Normal			:TEXCOORD1;			//�@��
	float4 Depth			:TEXCOORD2;			//�[�x���
	float3 Tangent			:TEXCOORD3;			//�^���W�F���g(�ڐ�)
	float3 Binormal			:TEXCOORD4;			//Binormal(�]�@��)
	//float3 vE				:TEXCOORD5;			//�����x�N�g��
};

// �l�̏������̏��Ԓʂ�ɕ`��
struct PS_DEFERRED
{
	float4 Diffuse			:COLOR0;	// �e�N�X�`���[
	float4 Normal			:COLOR1;	// �@�����
	float4 Specular			:COLOR2;	// �X�y�L����
	float4 Depth			:COLOR3;	// ���s��
};

// 2�̏������̏��Ԓʂ�ɕ`��
struct PS_G_BUFFER
{
	float4 NormalDepth		:COLOR0;	// �@�����&���s��
	float4 Specular			:COLOR1;	// �X�y�L����
};


//*****************************************
//		G_Buffer [1206] �œK������
//*****************************************

VS_DEFERRED VS_G_Buffer(VS_INPUT In)
{
	VS_DEFERRED Out = (VS_DEFERRED)0;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = In.Color;

	// �X�N���[�����W�ł̐[�x�l�ۑ�
	Out.Depth = Out.Pos;

	// �@�����̏C��
	float3x3 WVmat = (float3x3)mul(g_mWMatrix, g_mVMatrix); // ���f�������[���h���r���[��Ԃւ̕ϊ��s��

	// �r���[�s��̖@�����ł�������
	float3 N = mul(In.Normal, WVmat);// 
	N = normalize(N);
	Out.Normal = N;

	// �r���[�s��̖@���̃^���W�F���g�ƃo�C�m�[�}�������

	// ���_�X�N���[�����W�n�Z�o
	float3	vx;
	float3	vy = { .0f, 1.0f, 0.5f };			// ����y�����x�N�g��  0.001f �́@�O�ς𐬌�������ׂɂ��炵�Ă���@�����l�ŊO�ς����0�x�N�g���ɂȂ�

	vx = cross(N, vy);
	vx = normalize(vx);
	Out.Tangent = vx;

	vy = cross(N, vx);
	vy = normalize(vy);
	Out.Binormal = vy;

	return Out;
}

PS_G_BUFFER PS_G_Buffer(VS_DEFERRED In)
{
	PS_G_BUFFER OUT = (PS_G_BUFFER)0;

	// (1212) �A���t�@�������ł��Ȃ���Δj�� Decale��f���o���Ȃ��悤�ɂ��Ă���̂�
	clip((tex2D(DecaleSamp, In.Tex).a - 0.999f));

	//float2 Tex = In.Tex;

		//float3 E = normalize(In.vE);

		//�@�e�F�Ɍ��̃p�����[�^�[��ݒ�
		//  R->AO  G->Emissive  B->���ݒ�
		// float3 multiRate = tex2D(MultiSamp, Tex).rgb;

		//�f�B�q���[�Y
		//OUT.Diffuse = In.Color * tex2D( DecaleSamp,Tex );
	//	OUT.Diffuse = tex2D(DecaleSamp, Tex);
	//OUT.Diffuse.a = 1.0f;

	//�X�y�L����
	OUT.Specular = tex2D(RoughnessSamp, In.Tex);
	//OUT.Specular.a = 1.0f;
	//OUT.Specular.a = multiRate.g;	// �G�~�b�V�u������

	
	// OUT.Depth = In.Depth.z / In.Depth.w;	//���@w�Ŋ�����-1~1�ɕϊ�����@
	// Positon�ƈႢ������NDC��Ԃɂ���K�v������


	{
		// [1206] �ڋ�Ԃ͒��_�Ő��K�����Ă�̂�Pixel���ł͐��K�����Ȃ�
		
		// 	�ڋ�Ԃ���r���[��Ԃ֎����Ă����r���[��]�s����쐬
		// ��������́i�ځ�View��[�ϊ��s��]�j
		float3x3 ts;								//�@�����狁�߂���
		ts[0] = normalize(In.Tangent);				//�ڐ�
		ts[1] = normalize(In.Binormal);				//�]�@��
		ts[2] = normalize(In.Normal);				//�@��


		float3 N = tex2D(NormalSamp, In.Tex).xyz * 2.0f - 1.0f;// �e�N�X�`������F���擾���ăx�N�g���ɕϊ�
		float3 normal = normalize(mul(N, ts));// �ڋ�Ԃ�Normal�����̕ϊ��s����|�����킹View��Ԃ̖@���ց@
		normal = normal * 0.5f + 0.5f;// �@����F�ŏo������0~1�̊Ԃɖ߂�
		OUT.NormalDepth.rg = normal.rg;

	}

	//�[�x
	//OUT.NormalDepth.ba = In.Depth.zw;
	OUT.NormalDepth.b = In.Depth.z / In.Depth.w; // �ꉞW�Ŋ�����NDC��Ԃɂ��Ă݂�
	OUT.NormalDepth.a = In.Depth.w; //�@


	return OUT;
}

/* deferred�Ń��ɏ��������������@ */
/*
���ʂɕ`���Ɓ@�������I�u�W�F�N�g���d�Ȃ荇����񂪐��m�ɂłȂ�
�������@��
SrcBlend = One;
DestBlend = Zero
ZEnable = true;
�ɐݒ肷��

���ǂ��������Ƃ��N���邩
�\�[�X�u�����h�́@�������Ă�����̂��ǂꂭ�炢������邩�̒l
����� One(1,1,1,1)�ɂ��邱�ƂőS�͂ŏ�ɂ��Ԃ���p�ɐݒ�

�f�v�X�u�����h�́@��ɕ`���Ă������̂��ǂ����߂邩
������@Zero(0,0,0,0)�ɂ��邱�Ƃň�ؕ`���Ȃ�

���ꂾ�Ɓ@��낪��Ό����鏈���ɂȂ��Ă����܂�����
�y�l���l������΂����������������Ȃ��ł���

*/
technique G_Buffer
{
	pass P0
	{
		// �����_�����O�X�e�[�g
		AlphaBlendEnable = true;
		BlendOp = Add;
		//SrcBlend = SrcAlpha;		// �ŏ���RGB�f�[�^�\�[�X���w�肵�܂�
		//DestBlend = InvSrcAlpha;	// 2�Ԗڂ�RGB�f�[�^�\�[�X���w�肵�܂�
		SrcBlend = One;		//1,1,1,1
		DestBlend = Zero;	//0,0,0,0
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;
	
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// ����AlphaRef�ȏ�Ȃ�OK

		VertexShader = compile vs_3_0 VS_G_Buffer();
		PixelShader = compile ps_3_0 PS_G_Buffer();
	}
}

/*������������������������������������������������������������������*/
//	�������炪G_Buffer�𗘗p���ă��C�e�B���O�Ȃǂ�����			
/*������������������������������������������������������������������*/
/****************************/
//	�֐�
/****************************/

float3 CalcNormal(float2 xy)
{
	float3 normal;
	normal.xy = xy;
	normal.z = -sqrt(1.0f - (normal.x*normal.x) - (normal.y*normal.y));

	normal=normalize(normal);

	return normal;
}

// UV�l��Z�l����r���[��ԍ��W���Z�o
float3 CalcViewPosition(float2 UV, float2 zw)
{
	// UV���
	//      0.0 ---> 1.0
	//	  y ��				
	//		1.0f

	// NDC���  UV�ł͂��́��������Ă����̂ł��͔��]����K�v������
	//			1.0
	//			��
	//  -1.0<--- 0.0 ---> 1.0
	//	      y ��				
	//		   -1.0f

	float4 proj;
	proj.xy = (UV*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* zw.y; //�搶�ɕ�����
	//proj.xy = UV*2.0f - 1.0f;
	//proj.y *= -1.0f;
	proj.zw = zw;	// Z��W�͂��̂܂ܓ����

	// �v���W�F�N�V�����t�s��Ńr���[���W�n�ɕϊ��@Proj->View
	float4 viewPos = mul(proj, g_mInvPMatrix);
	//viewPos.xyz /= viewPos.w;		// W�Ŋ����Ă͂����Ȃ��B�Ȃ��H

	return viewPos.xyz;
}


// �I�u�W�F�̏ꏊ�Ɩ@���A���̌����@���̍i�闦�@�ŃX�y�L�������[�g��Ԃ�
float CalcSpecular(float3 pos, float3 normal, float3 lightVec, float power)
{
	//�@�X�y�L����
	float3 E = normalize(pos.xyz);// �r���[�s��ł̖ڐ�
		 
	float3 R = normalize(-lightVec - E);
	float specRate = pow(max(0, dot(R, normal)), power) ;// �����ʂ�i����

	return specRate;
}

//------------------------------------------------------
///		�T���v���[
//------------------------------------------------------

texture NormalDepthTex;
sampler NormalDepthSamp = sampler_state
{
	Texture = <NormalDepthTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


texture RoughnessLightMapTex;
sampler RoughnessLightMapSamp = sampler_state
{
	Texture = <RoughnessLightMapTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = BORDER;
	AddressV = BORDER;
};

// �e
texture ShadowMap;
sampler ShadowSamp = sampler_state
{
	Texture = <ShadowMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

/********************************************************/
//			�V���h�E
/********************************************************/

struct VS_SHADOW
{
	float4 Pos			:POSITION;
	float2 ZW			:TEXCOORD0;		//�[�x�l
	float2 Tex			:TEXCOORD1;
};

// �ߋ����p�V���h�E�}�b�v�쐬
VS_SHADOW VS_ShadowBuf(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
	VS_SHADOW Out;

	// ���W�ϊ��@���z����݂���Ԃ�
	float4x4	mat = mul(g_mWMatrix, g_mShadowProjection);
		Out.Pos = mul(Pos, mat);

	Out.ZW = Out.Pos.zw;
	Out.Tex = Tex;

	return Out;
}

// PixelShader�͋��ʂ�
float4 PS_ShadowBuf(VS_SHADOW In) : COLOR
{
	float4 OUT = float4(0, 0, 0, 0);
	OUT.r = In.ZW.r / In.ZW.g;			// ������F��
	OUT.a = tex2D(DecaleSamp, In.Tex).a;// ������������؂�悤�ɂ���
	
	return OUT;
}

technique ShadowBuf
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;// ����AlphaRef�ȏ�Ȃ�OK

		ZWriteEnable = true;
		CullMode = none;		 // �J�����O�����Ȃ����Ƃ��Y��ɉe��`��@

		VertexShader = compile vs_2_0 VS_ShadowBuf();
		PixelShader = compile ps_2_0 PS_ShadowBuf();
	}
}

/***************************/
//	���C�e�B���O�p�\����
/***************************/
struct PS_LIGHT
{
	float4	color : COLOR0;
	float4  spec  : COLOR1;
};

/***********************************/
//	���s��
PS_LIGHT PS_DirLight(float2 Tex:TEXCOORD0)
{
	PS_LIGHT OUT;

	// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);

	// ���C�g��(�n�[�t�����o�[�g)
	float rate = (dot(Normal, -g_vViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 Lambert = max(0, rate);				// Lambert

	// �s�N�Z���̐F
	OUT.color.rgb = (HalfLambert * g_vLightColor);
	OUT.color.a = 1.0f;

	//�@�X�y�L�������[�g�擾
	float sp = CalcSpecular(Pos, Normal, g_vViewLightVec, 80);
	// �X�y�L�����̐F���ǂꂭ�炢�悹�邩
	OUT.spec.rgb = sp*g_vLightColor;
	OUT.spec.a = 1.0f;

	return OUT;

}

technique DirLight
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // �d�˂ĕ`�悵�Ă���
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_DirLight();
	}
}

/***********************************/
//	���s��(�e)
PS_LIGHT PS_DirLightShadow(float2 Tex:TEXCOORD0)
{
	PS_LIGHT OUT;

	// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
	const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

	// ���C�g��(�n�[�t�����o�[�g)
	float rate = (dot(Normal, -g_vViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 Lambert = max(0, rate);				// Lambert

	// �s�N�Z���̐F
	OUT.color.rgb = (HalfLambert * g_vLightColor);
	OUT.color.a = 1.0f;

	//�@�X�y�L�������[�g�擾
	float specRate = CalcSpecular(Pos, Normal, g_vViewLightVec, RoughnessLightMap.g * 255);
	// �X�y�L�����̐F���ǂꂭ�炢�悹�邩
	OUT.spec.rgb = (specRate*g_vLightColor)*RoughnessLightMap.r;
	OUT.spec.a = 1.0f;

	float4 ShadowTex;
	float ShadowValue = 0.0f;
	float2 depth;		//	�o���A���X�V���h�E�}�b�v

	{
		// ���z���_���猩���v���W�F�N�V�������W�̃I�u�W�F
		ShadowTex = mul(mul(float4(Pos, 1), g_mInvVMatrix), g_mShadowProjection);
		ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
		ShadowTex.y = -ShadowTex.y;
		ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

		depth.r = tex2D(ShadowSamp, ShadowTex.xy).r;
		depth.g = depth.r*depth.r;// ���@���A���X�͓�悵���l���K�v
	}

	{
		//�u���[����
		float2 otherDepth = float2(0, 0);
			float temp = 0;

		float2 GaussShadowSamples[] =
		{
			{ -(1.0f / 1280.0f), (1.0f / 720.0f) },
			{ 0, (1.0f / 720.0f) },
			{ (1.0f / 1280.0f), (1.0f / 720.0f) },

			{ -(1.0f / 1280.0f), 0 },
			{ (1.0f / 1280.0f), 0 },

			{ -(1.0f / 1280.0f), -(1.0f / 720.0f) },
			{ 0, -(1.0f / 720.0f) },
			{ (1.0f / 1280.0f), -(1.0f / 720.0f) },
		};
		// ���̃T���v���񐔕��@For�ŉ��
		for (uint i = 0; i < 8; ++i)
		{
			temp = tex2D(ShadowSamp, ShadowTex.xy + GaussShadowSamples[i]).r;
			otherDepth.x += temp;
			otherDepth.y += temp*temp;
		}
		// ���ʂ̉e�Ɓ@�����ЂƂ̉e�𑫂������̂�����
		depth = depth*0.2f + otherDepth*0.1f;
	}

	{
		const float fragDepth = ShadowTex.z / ShadowTex.w; // ���z����̃I�u�W�F�̉��s��
		
		ShadowValue = 1; // �����N����Ȃ����1�ŕ��ʂɕ`��

		//if (depth.r < fragDepth - 0.001f)
		{
			const float variance = depth.g - depth.r * depth.r;
			ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
		}

		//ShadowValue = tex2D(ShadowSamp, ShadowTex.xy).r;
		
		OUT.color.rgb *= ShadowValue;
		OUT.spec.rgb *= ShadowValue;
	}



	//if (tex2D(ShadowSamp, ShadowTex.xy).r < ShadowTex.z - 0.005f)
	//{
	//	OUT.color.rgb = 0;
	//	OUT.spec.rgb = 0;
	//}

	return OUT;

}

technique DirLightShadow
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // �d�˂ĕ`�悵�Ă���
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_DirLightShadow();
	}
}

//*************************************
///		�������C�e�B���O
float4 PS_HemiLight(float2 Tex: TEXCOORD0) :COLOR
{
	float4 OUT;

	// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);

	//float AO = n.a;// Normal�̃�����AO�̒l���擾

	//��������
	float rate = Normal.y * 0.5f + 0.5f;
	OUT.rgb = g_vSkyColor * rate;
	OUT.rgb += g_vGroundColor * (1.0f - rate);
	//OUT.rgb *= AO;
	OUT.a = 1;

	return OUT;

}
technique HemiLight
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // �d�˂ĕ`�悵�Ă���
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_HemiLight();
	}
}



/***********************************/
//	���s��(�e)+����
PS_LIGHT PS_AllLight(float2 Tex:TEXCOORD0)
{
	PS_LIGHT OUT;

	// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
	const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

	// ���C�g��(�n�[�t�����o�[�g)
	float rate = (dot(Normal, -g_vViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
														//float3 Lambert = max(0, rate);				// Lambert

														// �s�N�Z���̐F
	OUT.color.rgb = (HalfLambert * g_vLightColor);
	OUT.color.a = 1.0f;

	//�@�X�y�L�������[�g�擾
	float specRate = CalcSpecular(Pos, Normal, g_vViewLightVec, RoughnessLightMap.g * 255);
	// �X�y�L�����̐F���ǂꂭ�炢�悹�邩
	OUT.spec.rgb = (specRate*g_vLightColor)*RoughnessLightMap.r;
	OUT.spec.a = 1.0f;

	float4 ShadowTex;
	float ShadowValue = 0.0f;
	float2 depth;		//	�o���A���X�V���h�E�}�b�v

	{
		// ���z���_���猩���v���W�F�N�V�������W�̃I�u�W�F
		ShadowTex = mul(mul(float4(Pos, 1), g_mInvVMatrix), g_mShadowProjection);
		ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
		ShadowTex.y = -ShadowTex.y;
		ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

		depth.r = tex2D(ShadowSamp, ShadowTex.xy).r;
		depth.g = depth.r*depth.r;// ���@���A���X�͓�悵���l���K�v
	}

	{
		//�u���[����
		float2 otherDepth = float2(0, 0);
		float temp = 0;

		float2 GaussShadowSamples[] =
		{
			{ -(1.0f / 1280.0f), (1.0f / 720.0f) },
			{ 0, (1.0f / 720.0f) },
			{ (1.0f / 1280.0f), (1.0f / 720.0f) },

			{ -(1.0f / 1280.0f), 0 },
			{ (1.0f / 1280.0f), 0 },

			{ -(1.0f / 1280.0f), -(1.0f / 720.0f) },
			{ 0, -(1.0f / 720.0f) },
			{ (1.0f / 1280.0f), -(1.0f / 720.0f) },
		};
		// ���̃T���v���񐔕��@For�ŉ��
		for (uint i = 0; i < 8; ++i)
		{
			temp = tex2D(ShadowSamp, ShadowTex.xy + GaussShadowSamples[i]).r;
			otherDepth.x += temp;
			otherDepth.y += temp*temp;
		}
		// ���ʂ̉e�Ɓ@�����ЂƂ̉e�𑫂������̂�����
		depth = depth*0.2f + otherDepth*0.1f;
	}

	{
		const float fragDepth = ShadowTex.z / ShadowTex.w; // ���z����̃I�u�W�F�̉��s��

		ShadowValue = 1; // �����N����Ȃ����1�ŕ��ʂɕ`��

		// ���������h�~
		if (depth.r < fragDepth - 0.001f)
		{
			const float variance = depth.g - depth.r * depth.r;
			ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
		}

		//ShadowValue = tex2D(ShadowSamp, ShadowTex.xy).r;

		OUT.color.rgb *= ShadowValue;
		OUT.spec.rgb *= ShadowValue;
	}


	//��������
	float HemiRate = Normal.y * 0.5f + 0.5f;
	OUT.color.rgb += g_vSkyColor * HemiRate;
	OUT.color.rgb += g_vGroundColor * (1.0f - HemiRate);


	//// �t�H�O�̒l������
	//float fogRate = smoothstep(g_fFogFar, g_fFogNear, Pos.z);
	//OUT.color.rgb += g_vFogColor * (1.0f - fogRate);

	//if (tex2D(ShadowSamp, ShadowTex.xy).r < ShadowTex.z - 0.005f)
	//{
	//	OUT.color.rgb = 0;
	//	OUT.spec.rgb = 0;
	//}

	return OUT;

}

technique AllLight
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // �d�˂ĕ`�悵�Ă���
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_AllLight();
	}
}


//**********************************
///		�_�����X�t�B�A�z�u�^
//**********************************

struct DEF_POINTLIGHT
{
	float4	color	:	COLOR0;
};

// �X�g���N�`��
struct VS_POINTLIGHT
{
	float4 Pos		:	POSITION;
	float4 wvpPos	:	TEXCOORD0;
	float4 WPos		:	TEXCOORD1;
};

// �O���[�o�����W
float3 g_vPLSpos;		// ��View���W�n�ɕϊ����Ă�������
float  g_fPLSrange = 30.0f;
float3 g_vPLScolor = float3(300, 600, 300);// �J���[
float  g_fPLSpower = 2.0f;// �p���[

//------------------------------------------------------
//		PointLightSphere�̒��_�V�F�[�_�[
//------------------------------------------------------
VS_POINTLIGHT VS_PointLightSphere(VS_INPUT In)
{
	VS_POINTLIGHT Out = (VS_POINTLIGHT)0;

	// �X�P�[����@���ɂ��������Ċg�� �����W��
	In.Pos.xyz += In.Normal*g_fPLSrange;
	// ��΂ɑ���K�v������@�{�[���̃|�W�V����
	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.WPos = mul(In.Pos, g_mWMatrix);

	//�@�t�F�b�`�p
	Out.wvpPos = Out.Pos;

	return Out;
}


DEF_POINTLIGHT PS_PointLightSphere(VS_POINTLIGHT In)
{
	DEF_POINTLIGHT OUT = (DEF_POINTLIGHT)0;

	//�X�N���[����Ԃ��e�N�X�`�����W�� ��
	In.wvpPos.x += 0.5f;// �����炷
	In.wvpPos.y -= 0.5f;// �����炷

	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	//ScreenTex.x += 0.001f;
	// �K�v�ȏ����擾�@��
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);// ��
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	// PL��Pos�Ƃ��̃s�N�Z���̈ʒu�Ł��|�W�V����������
	float3 g_vViewLightVec = g_vPLSpos.xyz - (Pos.xyz);
	float dist = pow(max(0.0f, 1.0f - (length(g_vViewLightVec) / g_fPLSrange)), 2);//�����l��������i��

	// �@���}�b�v����A�e�Ւf
	g_vViewLightVec = normalize(g_vViewLightVec);// ��<-�ꏊ�x�N�g����
	float rate = max(0, dot(Normal.xyz, g_vViewLightVec.xyz));

	OUT.color.rgb = (g_vPLScolor*(dist*rate))*g_fPLSpower;
	OUT.color.a = 1.0f;

	return OUT;

}

//
technique PointLightSphere
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;//�@�ǉ�
		DestBlend = One;
		CullMode = Cw;	// �J�����O�͂Ȃ� �@�������ɂ̂΂��������ǒ�����������CW
		ZEnable = FALSE;	// ���s�l���Ƃ������K�E�@�͂Ȃ��ɂȂ����B�@�v���C���[���O�ɂ���ƍ���
		ZWriteEnable = FALSE;
		//PixelShader = compile ps_3_0 PS_PointLight();
		//VertexShader = compile vs_3_0 VS_PointLightBall();
		VertexShader = compile vs_3_0 VS_PointLightSphere();
		PixelShader = compile ps_3_0 PS_PointLightSphere();
	}
}


/*������������������������������������������������������������������*/
//	�������炪���ō���Ă������C�g�̃o�b�t�@�Ȃǂ��g���Ō�̎d�グ
/*������������������������������������������������������������������*/

//------------------------------------------------------
//		���Z���ʂ̃T���v���[	
//------------------------------------------------------

//texture LightMap;
//sampler LightSamp = sampler_state
//{
//	Texture = <DiffuseLightMap>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = Wrap;
//	AddressV = Wrap;
//};
//
//texture SpecuarLightMap;
//sampler SpecuarLightSamp = sampler_state
//{
//	Texture = <SpecuarLightMap>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = Wrap;
//	AddressV = Wrap;
//};

texture LightMap;
sampler LightSamp = sampler_state
{
	Texture = <LightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture SpecMap;
sampler SpecSamp = sampler_state
{
	Texture = <SpecMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture ForwardTex;
sampler ForwardSamp = sampler_state
{
	Texture = <ForwardTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture ShadowTex;
sampler ShadowTexSamp = sampler_state
{
	Texture = <ShadowTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture SSAOTex;	//	SSAO�e�N�X�`��
sampler SSAOSamp = sampler_state
{
	Texture = <SSAOTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture PLSMap;	//	PLS�e�N�X�`��
sampler PLSSamp = sampler_state
{
	Texture = <PLSMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

//------------------------------------------------------
///		���p�X�N���[��	��Ƀt�H�A�[�h��ԂŎg�p
//------------------------------------------------------
texture EnvFullBuf;	//	EnvFullBuf
sampler EnvFullBufSamp = sampler_state
{
	Texture = <EnvFullBuf>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	// �������~���[�ɂ��Ă��܂���
	AddressU = MIRROR;//MIRROR
	AddressV = WRAP;//MIRROR
};

/***************************************/
/*�ڂ����p�T���v���[*/
/***************************************/

sampler GaussianSamp = sampler_state
{
	Texture = <Texture>;
	MinFilter = GAUSSIANQUAD;
	MagFilter = GAUSSIANQUAD;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


// ��ʃT�C�Y��UV�ɍ��킹��
float g_fTU = 1.0f / 1280.0f, g_fTV = 1.0f / 720.0f;
float g_fBlurValue = 1.0f;

/***************************************/
/*		�K�E�V�A���u���[			�@ */
float4 PS_gaussX(float2 Tex : TEXCOORD0) : COLOR
{

	////�e�N�Z�����擾   
	//float2 Texel0 = Tex + float2(-g_fTU * 1 * g_fBlurValue, 0.0f);
	//float2 Texel1 = Tex + float2(-g_fTU * 2 * g_fBlurValue, 0.0f);
	//float2 Texel2 = Tex + float2(-g_fTU * 3 * g_fBlurValue, 0.0f);
	//float2 Texel3 = Tex + float2(-g_fTU * 4 * g_fBlurValue, 0.0f);
	//float2 Texel4 = Tex + float2(-g_fTU * 5 * g_fBlurValue, 0.0f);

	//float2 Texel5 = Tex + float2(g_fTU * 1 * g_fBlurValue, 0.0f);
	//float2 Texel6 = Tex + float2(g_fTU * 2 * g_fBlurValue, 0.0f);
	//float2 Texel7 = Tex + float2(g_fTU * 3 * g_fBlurValue, 0.0f);
	//float2 Texel8 = Tex + float2(g_fTU * 4 * g_fBlurValue, 0.0f);
	//float2 Texel9 = Tex + float2(g_fTU * 5 * g_fBlurValue, 0.0f);

	////�擾�����e�N�Z���ʒu�̃J���[�����擾����B
	////���ꂼ��̃J���[�l�ɏd�݂������Ă���B���̏d�ݒl�͂��ׂĂ̍��v�� 1.0f �ɂȂ�悤�ɒ�������B
	//float4 p0 = tex2D(GaussianSamp, Tex) * 0.20f;

	//float4 p1 = tex2D(GaussianSamp, Texel0) * 0.12f;
	//float4 p2 = tex2D(GaussianSamp, Texel1) * 0.10f;
	//float4 p3 = tex2D(GaussianSamp, Texel2) * 0.08f;
	//float4 p4 = tex2D(GaussianSamp, Texel3) * 0.06f;
	//float4 p5 = tex2D(GaussianSamp, Texel4) * 0.04f;

	//float4 p6 = tex2D(GaussianSamp, Texel5) * 0.12f;
	//float4 p7 = tex2D(GaussianSamp, Texel6) * 0.10f;
	//float4 p8 = tex2D(GaussianSamp, Texel7) * 0.08f;
	//float4 p9 = tex2D(GaussianSamp, Texel8) * 0.06f;
	//float4 p10 = tex2D(GaussianSamp, Texel9) * 0.04f;

	////�J���[����������
	//return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;

	float4 Out = tex2D(GaussianSamp, Tex)*0.20f;

	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 1 * g_fBlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 2 * g_fBlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 3 * g_fBlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 4 * g_fBlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 5 * g_fBlurValue, 0.0f))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 1 * g_fBlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 2 * g_fBlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 3 * g_fBlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 4 * g_fBlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 5 * g_fBlurValue, 0.0f))*0.04f;

	return Out;

}

float4 PS_gaussY(float2 Tex : TEXCOORD0) : COLOR
{

	////�e�N�Z�����擾   
	//float2 Texel0 = Tex + float2(0.0, -g_fTV * 1 * g_fBlurValue);
	//float2 Texel1 = Tex + float2(0.0, -g_fTV * 2 * g_fBlurValue);
	//float2 Texel2 = Tex + float2(0.0, -g_fTV * 3 * g_fBlurValue);
	//float2 Texel3 = Tex + float2(0.0, -g_fTV * 4 * g_fBlurValue);
	//float2 Texel4 = Tex + float2(0.0, -g_fTV * 5 * g_fBlurValue);

	//float2 Texel5 = Tex + float2(0.0, g_fTV * 1 * g_fBlurValue);
	//float2 Texel6 = Tex + float2(0.0, g_fTV * 2 * g_fBlurValue);
	//float2 Texel7 = Tex + float2(0.0, g_fTV * 3 * g_fBlurValue);
	//float2 Texel8 = Tex + float2(0.0, g_fTV * 4 * g_fBlurValue);
	//float2 Texel9 = Tex + float2(0.0, g_fTV * 5 * g_fBlurValue);

	/////�擾�����e�N�Z���ʒu�̃J���[�����擾����B
	/////���ꂼ��̃J���[�l�ɏd�݂������Ă���B���̏d�ݒl�͂��ׂĂ̍��v�� 1.0f �ɂȂ�悤�ɒ�������B
	//float4 p0 = tex2D(GaussianSamp, Tex) * 0.20f;

	//float4 p1 = tex2D(GaussianSamp, Texel0) * 0.12f;
	//float4 p2 = tex2D(GaussianSamp, Texel1) * 0.10f;
	//float4 p3 = tex2D(GaussianSamp, Texel2) * 0.08f;
	//float4 p4 = tex2D(GaussianSamp, Texel3) * 0.06f;
	//float4 p5 = tex2D(GaussianSamp, Texel4) * 0.04f;

	//float4 p6 = tex2D(GaussianSamp, Texel5) * 0.12f;
	//float4 p7 = tex2D(GaussianSamp, Texel6) * 0.10f;
	//float4 p8 = tex2D(GaussianSamp, Texel7) * 0.08f;
	//float4 p9 = tex2D(GaussianSamp, Texel8) * 0.06f;
	//float4 p10 = tex2D(GaussianSamp, Texel9) * 0.04f;

	////�J���[����������
	//return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;

	float4 Out = tex2D(GaussianSamp, Tex)*0.20f;

	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 1 * g_fBlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 2 * g_fBlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 3 * g_fBlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 4 * g_fBlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 5 * g_fBlurValue))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 1 * g_fBlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 2 * g_fBlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 3 * g_fBlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 4 * g_fBlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 5 * g_fBlurValue))*0.04f;

	return Out;
}


technique gaussZ
{

	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_gaussX();
	}

	pass P1
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_gaussY();
	}
}

technique gaussX
{

	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_gaussX();
	}
}

technique gaussY
{

	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_gaussY();
	}
}

//********************************************************************
//		���˃u���\
//********************************************************************

float g_fCenterX = 0.0f;
float g_fCenterY = 0.0f;

float g_fBlurPower = 1.0f;
const float g_cIMAGE_SIZE = 512.0f;

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_RadialBlur(float2 Tex:TEXCOORD0) : COLOR
{
	float4	OUT;

	/*******************************************/
	// ( NDC��� ) -> ( UV��� ) �Ɏ����Ă���
	// 
	// 
	// 
	/*******************************************/
	
	//�@���˒��S
	float2 ss_center = float2((g_fCenterX + 1.0f) * 0.5f, (-g_fCenterY + 1.0f) * 0.5f);

	//�@�I�t�Z�b�g
	float2 uvOffset = (ss_center - Tex) * (g_fBlurPower / g_cIMAGE_SIZE);

	//�@�T���v�����O���̋t�� �@for���ŉ񂷉񐔕��F�����炵�@���������Ƃ��Ɍ��̐F�ɂ���B
	float InvSampling = 1.0f / 8.0f;

	//�@�e�N�X�`�����W�@���������߂ɍ��̃e�N�X�`���[�̏ꏊ��n���B
	float2 uv = Tex;

		//�@�T���v�����O�̉񐔂������s
	for (int i = 0; i<8; i++)
	{
		OUT += tex2D(DecaleSamp, uv) * InvSampling;
		uv += uvOffset;
	}

	return OUT;

}
//------------------------------------------------------
// �e�N�j�b�N
//------------------------------------------------------
technique RadialBlur
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_3_0 PS_RadialBlur();
	}
}


//********************************************************************
//		�ړ��u���\
//********************************************************************

float g_fDirectionalX = 1.0f;
float g_fDirectionalY = 0.0f;
float g_fDirectionalBlurPower = 0.015f;
//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_DirectionalBlur(float2 Tex:TEXCOORD0) : COLOR
{
	float4	OUT;

	/*******************************************/
	// ( NDC��� ) -> ( UV��� ) �Ɏ����Ă���
	// 
	/*******************************************/

	//�@�I�t�Z�b�g
	float2 uvOffset = float2(g_fDirectionalX, g_fDirectionalY) *(g_fDirectionalBlurPower);

	//�@�T���v�����O���̋t�� �@for���ŉ񂷉񐔕��F�����炵�@���������Ƃ��Ɍ��̐F�ɂ���B
	float InvSampling = 1.0f / 16.0f;

	//�@�e�N�X�`�����W�@���������߂ɍ��̃e�N�X�`���[�̏ꏊ��n���B
	float2 uv = Tex;

	//�@�T���v�����O�̉񐔂������s
	for (int i = 0; i<16; i++)
	{
		OUT += tex2D(GaussianSamp, uv) * InvSampling;
		uv += uvOffset;
	}

	return OUT;
}
//------------------------------------------------------
// �e�N�j�b�N
//------------------------------------------------------
technique DirectionalBlur
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_3_0 PS_DirectionalBlur();
	}
}

/*-------------------*/
//  HDR�u���[������
/*-------------------*/

// ------------------------------
/// �쐬�����u���[�����̂�����
// ------------------------------
float4 PS_hdrBloom(float2 Tex:TEXCOORD0) : COLOR
{
	float4 OUT;
	OUT = tex2D(GaussianSamp, Tex);
	OUT.rgb += OUT.rgb* 2.5f;//���u���[���̐F�̋����𒲐�
	return OUT;
}

// �����_�[�X�e�[�g�W
// https://msdn.microsoft.com/ja-jp/library/cc324307.aspx

technique add_hdrBloom
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = InvDestColor; //�@�\�[�X�A���t�@�̂܂܂��Ɖ��Z�������̂Ł@InvDestColor�ŃX�N���[���ɁI
		DestBlend = one;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_hdrBloom();
	}
}

/****************************/
/*			ToneMap			*/
/****************************/

/*�I���x*/
float g_fExposure = 0.0f;

// ���P�x
float3 g_vBloomColor = { 0.825f, 0.85f, 0.85f };

/***********************************/
//	���P�x���o�p�̍\����
/***********************************/
struct PS_TONEMAP
{
	float4	color : COLOR0;
	float4  high  : COLOR1;
};



/*********************************************/
//
//			�L�����N�^�[�p�V�F�[�_
//
/*********************************************/

//------------------------------------------------------
//		���_�t�H�[�}�b�g
//------------------------------------------------------
struct VS_OUTPUT_FINAL
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos			: TEXCOORD1;//�@�s�N�Z���ɑ�����Ƀ��[���h����̃|�W�V�����ǉ�
	float4 wvpPos		: TEXCOORD2;//�@�s�N�Z���ɑ�����ɃQ�[����ʃ��X�g�̃|�W�V�����ǉ�
};

//********************************************************************
//
//		��{�R�c�V�F�[�_�[		
//
//********************************************************************

//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT_FINAL VS_DefaultLighting(VS_INPUT In)
{
	VS_OUTPUT_FINAL Out = (VS_OUTPUT_FINAL)0;
	//TransMatrix��Pos����������wPos�̏�񐶐�
	Out.wPos = mul(In.Pos, g_mWMatrix);

	Out.wvpPos = Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	//Out.wvpPos = Out.Pos;

	return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_DefaultLighting(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
	lightCol += tex2D(PLSSamp, ScreenTex);
	OUT.color.rgb *= lightCol;
	OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	//OUT.color.g += 0.5;

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(g_fExposure);
	
	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		�ʏ�`��e�N�j�b�N
//------------------------------------------------------
technique DefaultLighting
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		//SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		//DestBlend = InvSrcAlpha;	// �`����Ă����
		SrcBlend = One;		//1,1,1,1
		DestBlend = Zero;	//0,0,0,0

		CullMode = CCW;				// �J�����O�̎d�l

		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// ����AlphaRef�ȏ�Ȃ�OK

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_DefaultLighting();
	}
}

/*************************************/
// �X�e�[�W�ɕK�v�ȕϐ�
/*************************************/

float    g_fOverDriveDim = 1.0f;

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_Stage(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
	lightCol += tex2D(PLSSamp, ScreenTex);
	OUT.color.rgb *= lightCol;
	OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	
	// �K�E�Ó]�̒l
	OUT.color.rgb *= g_fOverDriveDim;

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		�X�e�[�W�p�`��e�N�j�b�N
//------------------------------------------------------
technique Stage
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		//SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		//DestBlend = InvSrcAlpha;	// �`����Ă����
		SrcBlend = One;				//1,1,1,1
		DestBlend = Zero;			//0,0,0,0
		CullMode = CCW;				// �J�����O�̎d�l
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// ����AlphaRef�ȏ�Ȃ�OK

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Stage();
	}
}


//------------------------------------------------------
//		(�����X�e�[�W�p)�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_StageNanasato(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

//	�s�N�Z���F����
OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

float4 lightCol = tex2D(LightSamp, ScreenTex);
lightCol += tex2D(PLSSamp, ScreenTex);
OUT.color.rgb *= lightCol;
OUT.color.rgb += tex2D(SpecSamp, ScreenTex);

// �s�N�Z���F���O���E�p�ɃT���v�����O
const float4 l_fGlowCol = tex2D(MultiSamp, In.Tex);
OUT.color.rgb += l_fGlowCol.rgb;


// �K�E�Ó]�̒l
OUT.color.rgb *= g_fOverDriveDim;

//�g�[���}�b�s���O
OUT.color.rgb *= exp2(g_fExposure);

OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
OUT.high.a = 1.0f;


return OUT;
}

//------------------------------------------------------
//		(�����X�e�[�W�p)�X�e�[�W�p�`��e�N�j�b�N
//------------------------------------------------------
technique StageNanasato
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = One;				//1,1,1,1
		DestBlend = Zero;			//0,0,0,0
		CullMode = CCW;				// �J�����O�̎d�l
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// ����AlphaRef�ȏ�Ȃ�OK

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_StageNanasato();
	}
}



//------------------------------------------------------
//		�v���C���[�p�̃O���[�o���G���A
//------------------------------------------------------
float g_InvincibleColRate = 0.0f;//Flash ���̃L�����N�^�[�_�E����̓_�ł̃��[�g
float g_OrangeColRate = 0.0f;//�@�I�����W�̌�
float g_MagentaColRate = 0.0f;//�@�}�[���^�̌�
float g_OverDriveColRate = 0.0f;//
float g_WillPowerRate = 0.0f;//  �����l

// [1206] �œK���̂��߃��[�g�̑���񐔂����炷���ߎd���Ȃ��ł߂邱�Ƃ�
float3 g_vPlayerColDesc;		//	��->�_�� ��->�I�����W ��->�}�[���^
float3 g_vPlayerColDesc2;	//  ��->�I�[�o�[�h���C�u���@��->�o�� �@��->�G�b�W���Ԃ���

/*************************************/
// �A�E�g���C���ɕK�v�ȕϐ�
/*************************************/
float    g_fOutlineSize = 0.005f;
float3   g_vEdgeColor = float3(0.0f, 0.0f, 0.0f);
//------------------------------------------------------
//		���_�t�H�[�}�b�g
//------------------------------------------------------
struct VS_OUTPUT_OUTLINE
{
	float4 Pos		: POSITION;
	float2 Tex		: TEXCOORD0;
	float4 Color	: COLOR;
};

/*************************************/
// �A�E�g���C���p�̒��_�V�F�[�_�[
/*************************************/

VS_OUTPUT_OUTLINE VS_OutLine(VS_INPUT In) //:POSITION �e�N�X�R�[�h�̏�񂪂Ȃ��Ȃ�̂ŏ����Ȃ�
{

	VS_OUTPUT_OUTLINE OUT = (VS_OUTPUT_OUTLINE)0;

	OUT.Pos = mul(In.Pos, g_mWVPMatrix);
	OUT.Pos.xy += normalize(mul(mul(float4(In.Normal, 0), g_mWMatrix), g_mVPMatrix).xy) * OUT.Pos.z * g_fOutlineSize;
	OUT.Pos.z += 0.001f;

	OUT.Tex = In.Tex;

	return OUT;
}

/*************************************/
// �A�E�g���C���p�̃s�N�Z���V�F�[�_�[
/*************************************/

PS_TONEMAP PS_OutLine(VS_OUTPUT_OUTLINE In)
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	float4 col = tex2D(DecaleSamp, In.Tex); //���̃L�����N�^�[�̃G�b�W�t�߂̐F��

	//OUT.color.rgb = col.rgb - float3(0.25,0.5,0.5);// �ӂ��͏����Â�	
	// [11/27] ���삵�Ă���L�����N�^�[���������Ƃ̈ӌ��������������̂ŕ��̐F��ς���H�v�����Ă݂܂��B
	//OUT.color.rgb = g_vEdgeColor;
	OUT.color.rgb = float3(0.95f, 0.1f, 0.0f)*g_vPlayerColDesc2.b;
	OUT.color.rgb += float3(0.0f, 0.65f, 1.0f)*(1.0f - g_vPlayerColDesc2.b);
	OUT.color.a = 1.0f;

	OUT.high = col - 1;
	//OUT.high.rgb = float3(0, 1, 1.5);
	OUT.high.a = 1;

	return OUT;
}


technique OutLine
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CW;				// �J�����O�̎d�l


		VertexShader = compile vs_3_0 VS_OutLine();
		PixelShader = compile ps_3_0 PS_OutLine();
	}
}




//------------------------------------------------------
//		�v���C���[�p�̃s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_Player(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
		lightCol += tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);


	//OUT.color.g += 0.5;



	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	//���P�x���o��ɂ��Ȃ���HDR�Ō����Ă��܂��̂ōŌ��
	OUT.color.rgb += g_InvincibleColRate;

	return OUT;
}

//------------------------------------------------------
//		�v���C���[�p�̃e�N�j�b�N
//------------------------------------------------------
technique Player
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		//SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		//DestBlend = InvSrcAlpha;	// �`����Ă����
		SrcBlend = One;				//1,1,1,1
		DestBlend = Zero;			//0,0,0,0
		CullMode = CCW;				// �J�����O�̎d�l
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// ����AlphaRef�ȏ�Ȃ�OK
		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Player();
	}
}


//------------------------------------------------------
//		�v���C���[�p�̃s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_ToonPlayer(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
	// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	//float4 lightCol = tex2D(LightSamp, ScreenTex);
	//float4 PLlightCol = tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb += PLlightCol.rgb;
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	// ���C�g��(�n�[�t�����o�[�g)
	float rate = (dot(Normal, -g_vViewLightVec));
	float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
		OUT.color.rgb *= toonShadowCol;

	//OUT.color.g += 0.5;
	float3 E = Pos.xyz;// �r���[�̖ڐ�
		E = normalize(E);
	float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)), 4.0f);
	float RimLightPower = max(.0f, dot(-E, g_vViewLightVec));

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	// �L���T�������ۂ���������
	OUT.color.rgb += (RimPower)* float3(1, 1, 1);

	// �I�[�o�[�h���C�u�p
	//float RimPower2 = pow(1.0f - max(0.0f, dot(-E, Normal)), 1.0f);
	OUT.high.rgb += float3(0.8, 0.5, 0.0)*g_vPlayerColDesc.g/*g_OrangeColRate*/;
	OUT.high.rgb += float3(0.7, 0.0, 0.4)*g_vPlayerColDesc.b/*g_MagentaColRate*/;
	OUT.high.rgb += float3(0.0, 0.1, 0.4)*g_vPlayerColDesc2.r/*g_OverDriveColRate*/;
	OUT.high.rgb += float3(0.25f, 0.0, 0.0)*g_vPlayerColDesc2.g/*g_WillPowerRate*/;

	//���P�x���o��ɂ��Ȃ���HDR�Ō����Ă��܂��̂ōŌ��
	OUT.color.rgb += g_vPlayerColDesc.r/*g_InvincibleColRate*/;

	return OUT;
}

//------------------------------------------------------
//		�v���C���[�p�̃e�N�j�b�N
//------------------------------------------------------
technique PlayerToon
{
	pass OutLine
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = false;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_OutLine();
		PixelShader = compile ps_3_0 PS_OutLine();
	}

	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_ToonPlayer();
	}
}


//------------------------------------------------------
//		�v���C���[�p�̃s�N�Z���V�F�[�_�[2	
//------------------------------------------------------
PS_TONEMAP PS_ToonPlayerNoRim(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
// �K�v�ȏ����擾
const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

//	�s�N�Z���F����
OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

//float4 lightCol = tex2D(LightSamp, ScreenTex);
//float4 PLlightCol = tex2D(PLSSamp, ScreenTex);
//OUT.color.rgb += PLlightCol.rgb;
//OUT.color.rgb *= lightCol;
//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

// ���C�g��(�n�[�t�����o�[�g)
float rate = (dot(Normal, -g_vViewLightVec));
float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
OUT.color.rgb *= toonShadowCol;

//OUT.color.g += 0.5;
float3 E = Pos.xyz;// �r���[�̖ڐ�
E = normalize(E);
float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)), 4.0f);
float RimLightPower = max(.0f, dot(-E, g_vViewLightVec));

//�g�[���}�b�s���O
OUT.color.rgb *= exp2(g_fExposure);

OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
OUT.high.a = 1.0f;

// �I�[�o�[�h���C�u�p
//float RimPower2 = pow(1.0f - max(0.0f, dot(-E, Normal)), 1.0f);
OUT.high.rgb += float3(0.8, 0.5, 0.0)*g_vPlayerColDesc.g/*g_OrangeColRate*/;
OUT.high.rgb += float3(0.7, 0.0, 0.4)*g_vPlayerColDesc.b/*g_MagentaColRate*/;
OUT.high.rgb += float3(0.0, 0.1, 0.4)*g_vPlayerColDesc2.r/*g_OverDriveColRate*/;
OUT.high.rgb += float3(0.25f, 0.0, 0.0)*g_vPlayerColDesc2.g/*g_WillPowerRate*/;

//���P�x���o��ɂ��Ȃ���HDR�Ō����Ă��܂��̂ōŌ��
OUT.color.rgb += g_vPlayerColDesc.r/*g_InvincibleColRate*/;

return OUT;
}

//------------------------------------------------------
//		�v���C���[�p�̃e�N�j�b�N2
//------------------------------------------------------
technique PlayerToonNoRim
{
	pass OutLine
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = false;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_OutLine();
		PixelShader = compile ps_3_0 PS_OutLine();
	}

	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_ToonPlayerNoRim();
	}
}



//------------------------------------------------------
//		�y���\�i�p�̃s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_Persona(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
	// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
		lightCol += tex2D(PLSSamp, ScreenTex);
	OUT.color.rgb *= lightCol;
	OUT.color.rgb += tex2D(SpecSamp, ScreenTex);

	// ���C�g��(�n�[�t�����o�[�g)
	//float rate = (dot(Normal, -g_vViewLightVec));
	//float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
	//	OUT.color.rgb *= toonShadowCol;

		// �s�N�Z���̐F
		//OUT.color.rgb = (HalfLambert * g_vLightColor);
	

	// ���������C�e�B���O
	float3 E = Pos.xyz;// �r���[�̖ڐ�
	E = normalize(E);

	float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)),2.0f);

	// �y���\�i�̐���
	OUT.color.rgb += RimPower * float3(0, 0.4, 2.5);

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));

	// �y���\�i�̐���
	//OUT.high.rgb += RimPower * float3(0,1.3, 1.5);

	OUT.high.a = 1.0f;

	//���P�x���o��ɂ��Ȃ���HDR�Ō����Ă��܂��̂ōŌ��
	//OUT.color.rgb += g_InvincibleColRate;



	return OUT;
}

//------------------------------------------------------
//		�y���\�i�e�N�j�b�N
//------------------------------------------------------
technique Persona
{
	//pass OutLine
	//{
	//	ZEnable = true;				// ���s�l��
	//	ZWriteEnable = false;		// ���s���������ނ�

	//	AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
	//	BlendOp = Add;				// �u�����h�d�l
	//	SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
	//	DestBlend = InvSrcAlpha;	// �`����Ă����
	//	CullMode = CW;				// �J�����O�̎d�l

	//	VertexShader = compile vs_3_0 VS_OutLine();
	//	PixelShader = compile ps_3_0 PS_OutLine();

	//}

	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Persona();
	}



}

//********************************************************************
//
//		��{�R�c�V�F�[�_�[		
//
//********************************************************************

//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT_FINAL VS_Sky(VS_INPUT In)
{
	VS_OUTPUT_FINAL Out = (VS_OUTPUT_FINAL)0;
	//TransMatrix��Pos����������wPos�̏�񐶐�
	Out.wPos = mul(In.Pos, g_mWMatrix);

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.wvpPos = Out.Pos;

	return Out;
}
//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_Sky(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	// �K�E�Ó]�̒l
	OUT.color.rgb *= g_fOverDriveDim;

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(g_fExposure);
	//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
	OUT.high.rgb = float3(0.05f, 0.1f, 0.3f)*g_fOverDriveDim;// �K�E�Ó]�̒l
	OUT.high.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		��e�N�j�b�N
//------------------------------------------------------
technique sky
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_Sky();
		PixelShader = compile ps_3_0 PS_Sky();
	}
}

//------------------------------------------------------
//		���s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_NightSky(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

//	�s�N�Z���F����
OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

// �K�E�Ó]�̒l
OUT.color.rgb *= g_fOverDriveDim;

//�g�[���}�b�s���O
OUT.color.rgb *= exp2(g_fExposure);
//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
OUT.high.rgb = float3(0.035f, 0.045f, 0.075f)*g_fOverDriveDim;// �K�E�Ó]�̒l
OUT.high.a = 1.0f;

return OUT;
}
//------------------------------------------------------
//		���e�N�j�b�N
//------------------------------------------------------
technique nightsky
{
	pass P0
	{
		ZEnable = true;				// ���s�l��
		ZWriteEnable = true;		// ���s���������ނ�

		AlphaBlendEnable = true;	// �A���t�@�u�����h�l��
		BlendOp = Add;				// �u�����h�d�l
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_Sky();
		PixelShader = compile ps_3_0 PS_NightSky();
	}
}



//**************************************
//
///		�t�u�A�j���[�V����
//
//**************************************

// UV�p
sampler DecaleSampUV = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	//BorderColor = 0x00FFFFFF;
	AddressU = WRAP;
	AddressV = WRAP;
};


struct VS_INPUT_UV
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float2 Tex	  : TEXCOORD0;
	float4 Color	: COLOR0;
};
struct VS_OUTPUT_UV
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float4 wvpPos	: TEXCOORD1;
	float3 Normal	: NORMAL;
};

// UV
float m_fTuAnime = 0.0f;
float m_fTvAnime = 0.0f;
float m_fAlphaUV = 1.0f;

VS_OUTPUT_UV VS_UvAnime(VS_INPUT_UV In)
{
	VS_OUTPUT_UV Out = (VS_OUTPUT_UV)0;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.wvpPos = Out.Pos;
	Out.Normal = In.Normal;
	Out.Color = In.Color;// ���_�J���[�擾
	Out.Tex = In.Tex + float2(m_fTuAnime, m_fTvAnime);//���W

	Out.Color.rgb = 1.0f;
	Out.Color.a *= m_fAlphaUV; //�@�����x

	return Out;
}


//------------------------------------------------------
///		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime(VS_OUTPUT_UV In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);
	
	//�g�[���}�b�s���O
	//OUT.color.rgb *= exp2(g_fExposure); ���͍l���͂Ȃ���
	
	// ���P�x���o
	OUT.high.rgb = max(OUT.color.rgb - 0.45f,0.0f);
	//OUT.high.rgb = float3(0, 0.5, 0);
	OUT.high.a = OUT.color.a;

	return OUT;
}

technique uvAnime
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l������
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}


technique uvAnime_add
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = one;
		CullMode = CCW;
		ZEnable = true;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l������
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}

technique uvAnime_add_noz
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = one;
		CullMode = CCW;
		ZEnable = false;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l�����Ȃ�
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}

//------------------------------------------------------
///		�K�[�h�p�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime_Guard(VS_OUTPUT_UV In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
	//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
	// �K�v�ȏ����擾
	//const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	//const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	//const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);

	// �ڐ�
	//float3 E = Pos.xyz;
	//	E.normalize();

	In.wvpPos.xyz /= In.wvpPos.w;// NDC
	float RimPower = pow(1.0f - max(0.0f, dot(-In.wvpPos.xyz, In.Normal)), 2.0f);
	RimPower *= 3.0f;
	OUT.color.a *= RimPower;
	OUT.color.rgb += float3(0.3, 0.3, 1.0);

	//�g�[���}�b�s���O
	//OUT.color.rgb *= exp2(g_fExposure); ���͍l���͂Ȃ���

	// ���P�x���o
	OUT.high.rgb = max(OUT.color.rgb - 0.5f, 0.0f);
	//OUT.high.rgb = float3(0, 0.5, 0);
	OUT.high.a = OUT.color.a;

	return OUT;
}

//------------------------------------------------------
///		�K�[�h�p�e�N�j�b�N
//------------------------------------------------------
technique uvAnime_guard
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = ONE;// InvSrcAlpha
		CullMode = CCW;
		ZEnable = true;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l������
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime_Guard();
	}
}


//------------------------------------------------------
///		���I�[���p�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime_Soul(VS_OUTPUT_UV In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//�X�N���[����Ԃ��e�N�X�`�����W��  NDC->UV y���]
//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
// �K�v�ȏ����擾
//const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
//const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
//const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

//	�s�N�Z���F����
OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);

// �ڐ�
//float3 E = Pos.xyz;
//	E.normalize();

In.wvpPos.xyz /= In.wvpPos.w;// NDC
float RimPower = pow(1.0f - max(0.0f, dot(-In.wvpPos.xyz, In.Normal)), 1.0f);
RimPower *= 2.2f;
OUT.color.a *= RimPower;
//OUT.color.rgb += float3(0.3, 0.3, 1.0);

float RimPower2 = pow(max(0.0f, dot(-In.wvpPos.xyz, In.Normal)), 2.5f);
RimPower2 *= 2.5f;
OUT.color.a *= RimPower2;

//OUT.color.rgb += float3(0.1, 0.0, 0.0);

//�g�[���}�b�s���O
//OUT.color.rgb *= exp2(g_fExposure); ���͍l���͂Ȃ���

// ���P�x���o
OUT.high.rgb = max(OUT.color.rgb - 0.25f, 0.0f);
//OUT.high.rgb = float3(0, 0.5, 0);
OUT.high.a = OUT.color.a;

return OUT;
}

//------------------------------------------------------
///		���p�e�N�j�b�N
//------------------------------------------------------
technique uvAnime_soul
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = ONE;// InvSrcAlpha
		CullMode = CCW;
		ZEnable = true;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l������
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime_Soul();
	}
}


//------------------------------------------------------
//	�G���A�̃}�X�N�p�̒��_�\����
//------------------------------------------------------
struct VS_OUTPUT_UV_WALL
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float4 wvpPos	: TEXCOORD1;
	float2 MaskTex	: TEXCOORD3;
	float3 Normal	: NORMAL;
};

//------------------------------------------------------
//	�G���A�̃}�X�N
//------------------------------------------------------
texture AreaWallTex;
sampler AreaWallSamp = sampler_state
{
	Texture = <AreaWallTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;// �J��Ԃ�
	AddressV = Wrap;
};


//------------------------------------------------------
///		�G���A�̕Ǘp���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT_UV_WALL VS_UvAnime_AreaWall(VS_INPUT_UV In)
{
	VS_OUTPUT_UV_WALL Out = (VS_OUTPUT_UV_WALL)0;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.wvpPos = Out.Pos;
	Out.Normal = In.Normal;
	Out.Color = In.Color;// ���_�J���[�擾
	Out.Tex = In.Tex + float2(m_fTuAnime, m_fTvAnime);//���W
	Out.MaskTex = In.Tex;
	Out.Color.rgb = 1.0f;
	Out.Color.a *= m_fAlphaUV; //�@�����x

	return Out;
}

//------------------------------------------------------
///		�G���A�̕Ǘp�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime_AreaWall(VS_OUTPUT_UV_WALL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//	�s�N�Z���F����
	OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);

	// �}�X�N�}�b�v�擾
	float rate = tex2D(AreaWallSamp, In.MaskTex).r;

	// �f�J�[���Ɗ|�����킷
	OUT.color.rgb *= rate;

	// �w�L�T���f���Ă��Ȃ��Ƃ��ł������F���悹��
	//OUT.color.a += 0.2f;
	OUT.color.g += 0.1f;
	OUT.color.b += 0.2f;

	//�g�[���}�b�s���O
	//OUT.color.rgb *= exp2(g_fExposure); ���͍l���͂Ȃ���

	// ���P�x���o
	OUT.high.rgb = max(OUT.color.rgb - 0.45f, 0.0f);
	OUT.high.a = OUT.color.a;

	return OUT;
}

//------------------------------------------------------
///		�G���A�̕Ǘp�e�N�j�b�N
//------------------------------------------------------
technique uvAnime_areaWall
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One;
		CullMode = CCW;
		ZEnable = true;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l������
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime_AreaWall();
		PixelShader = compile ps_3_0 PS_UvAnime_AreaWall();
	}
}


//------------------------------------------------------
//		���Ɉڂ肱�ސ��E
//------------------------------------------------------
struct VS_INPUT_WATER_REFLECT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

struct VS_OUTPUT_WATER_REFLECT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos		: TEXCOORD1;//�@�s�N�Z���ɑ�����Ƀ��[���h����̃|�W�V�����ǉ�
	float4 wvpPos	: TEXCOORD2;//�@�s�N�Z���ɑ�����ɃQ�[����ʃ��X�g�̃|�W�V�����ǉ�

	float3 vLight	: TEXCOORD4;
	float3 vE		: TEXCOORD5;

	float3 Normal	: COLOR1;

};

//------------------------------------------------------
///		���Ɉڂ肱�ސ��E�ɕK�v�ȃp�����[�^�[
//------------------------------------------------------
float g_fWaterHeight = 0.0f;		//	���̍���


//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT_WATER_REFLECT VS_WATER_REFLECT(VS_INPUT_WATER_REFLECT In)
{
	VS_OUTPUT_WATER_REFLECT Out = (VS_OUTPUT_WATER_REFLECT)0;

	//���[�J���̏�ԂŃ��f����Y�����]
	In.Pos.y *= -1.0f;

	//TransMatrix��Pos����������wPos�̏�񐶐�
	Out.wPos = mul(In.Pos, g_mWMatrix);

	// �����Ő��̍������l���������W�̔��]���s��
	//Out.wPos.y = g_fWaterHeight - (Out.wPos.y - g_fWaterHeight);

	Out.wPos.y += 1;

	Out.Pos = mul(Out.wPos, g_mVPMatrix);

	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.wvpPos = Out.Pos;

	return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	 ���PS_TONE_MAP �ł�邩��
//------------------------------------------------------
float4 PS_WATER_REFLECT(VS_OUTPUT_WATER_REFLECT In) : COLOR
{
	float4	OUT = (float4)0;

	// ������ɉ摜������Δj��
	clip(g_fWaterHeight - In.wPos.y);

	//	�s�N�Z���F����
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);

	// �K�E�Ó]�̒l
	//OUT.color.rgb *= g_fOverDriveDim;

	//�g�[���}�b�s���O
	//OUT.color.rgb *= exp2(g_fExposure);
	//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
	//OUT.high.rgb = float3(0.05f, 0.1f, 0.3f)*;// �K�E�Ó]�̒l
	//OUT.high.a = 1.0f;

	return OUT;
}

/* ���Ɉڂ锽�˃}�b�v����*/
technique CrystalWaterReflect
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CW;				// �J�����O���t�����ɂ���
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_WATER_REFLECT();
		PixelShader = compile ps_3_0 PS_WATER_REFLECT();
	}

}


// UV�l��Z�l���烏�[���h��ԍ��W���Z�o
float3 CalcWorldPosition(float2 UV, float2 zw)
{
	// UV���
	//      0.0 ---> 1.0
	//	  y ��				
	//		1.0f

	// NDC���  UV�ł͂��́��������Ă����̂ł��͔��]����K�v������
	//			1.0
	//			��
	//  -1.0<--- 0.0 ---> 1.0
	//	      y ��				
	//		   -1.0f

	float4 proj;
	proj.xy = (UV*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* zw.y; //�搶�ɕ�����
																	//proj.xy = UV*2.0f - 1.0f;
																	//proj.y *= -1.0f;
	proj.zw = zw;	// Z��W�͂��̂܂ܓ����

					// �v���W�F�N�V�����t�s��Ńr���[���W�n�ɕϊ��@Proj->View
	float4 worldPos = mul(proj, g_mInvVPMatrix);
	//worldPos.xyz /= worldPos.w;		// ���[���h��Ԃւ�W�Ŋ���K�v������

	return worldPos.xyz;
}
//------------------------------------------------------
//		�����_�t�H�[�}�b�g
//------------------------------------------------------

struct VS_INPUT_WATER
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};


struct VS_OUTPUT_WATER
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex1		: TEXCOORD0;
	float2 Tex2		: TEXCOORD3;

	float4 wPos		: TEXCOORD1;//�@�s�N�Z���ɑ�����Ƀ��[���h����̃|�W�V�����ǉ�
	float4 wvpPos	: TEXCOORD2;//�@�s�N�Z���ɑ�����ɃQ�[����ʃ��X�g�̃|�W�V�����ǉ�

	float3 vLight	: TEXCOORD4;
	float3 vE		: TEXCOORD5;

	float3 Normal	: COLOR1;
	float4 vPos		: TEXCOORD6;//�@�s�N�Z���ɑ�����Ƀr���[����̃|�W�V�����ǉ�

};

//------------------------------------------------------
///		���̃��t���N�V�����}�b�v
//------------------------------------------------------
texture WaterReflectBuf;
sampler WaterReflectSamp = sampler_state
{
	Texture = <WaterReflectBuf>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	// �������~���[�ɂ��Ă��܂���
	AddressU = MIRROR;//MIRROR
	AddressV = WRAP;//MIRROR
};


// �O���[�o���p�����[�^�[
float g_fUvWater = 0.0f;
//float3 wLightVec = { 1.0f, -1.0f, 1.0f };// 

//------------------------------------------------------
//		�����_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT_WATER VS_WATER(VS_INPUT_WATER In)
{
	VS_OUTPUT_WATER Out = (VS_OUTPUT_WATER)0;
	//WMatrix��Pos����������wPos�̏�񐶐�
	Out.wPos = mul(In.Pos, g_mWMatrix);
	Out.vPos = mul(In.Pos, g_mWMatrix);
	
	//  �v���W�F�N�V������Ԃ̍��W��n���܂�
	Out.Pos = Out.wvpPos = mul(In.Pos, g_mWVPMatrix);
	//Out.wvpPos = Out.Pos;

	// ���̔g�p��2�e�N�X�`���[���W��
	Out.Tex1 = In.Tex + float2(-g_fUvWater*1.5f, -g_fUvWater * 1.5f);
	Out.Tex2 = In.Tex + float2(g_fUvWater, g_fUvWater*1.51f);

	// �ꉞ�F���g����p�ɂ��Ƃ��@���͓��Ɏg��Ȃ��̂�1
	Out.Color = 1.0f;

	//	�@���ϊ�	
	float3x3 mat = g_mWMatrix;			// ���̉��Z�����邽��3x3�ɕύX
	float3 N = mul(In.Normal, mat);	//	���[�J���̖@���ƃ��[���h�s��|�����킹�ă��[���h��Ԃ̖@�����܂�
	Out.Normal = normalize(N);		// ���K�����Ƃ��܂��B
	

	//	���_���[�J�����W�n�Z�o
	float3	T;
	float3	B = { .0f,1.0f,0.0001f};// �_�~�[��Y���̃x�N�g�������
	T = cross(B, N);				// �܂�N(z��)�Ɖ���Y���ŊO�ς�X�����
	T = normalize(T);				// X�̒l���x�N�g���ɕϊ�
	B = cross(T, N);				// N(z��)�Ɠ��肵��X���ŊO�ς���Y�������
	B = normalize(B);				// Y���̒l���x�N�g����

	//�@���[���h����ڋ�Ԃւ��ǂ��K�v������
	//�@���C�g�x�N�g����ڋ�Ԃ�
	//�@��]�������̋t�s��͓]�u���邾���ł����̂Ł@���ςɒu��
	Out.vLight.x = dot(T, g_vWLightVec);	// ���[���h��Ԃ̃x�N�g����T�Ɠ��ς��ā@	�ڋ�Ԃ̂w�������
	Out.vLight.y = dot(B, g_vWLightVec);	// ���[���h��Ԃ̃x�N�g����B�Ɠ��ς��ā@	�ڋ�Ԃ̂x�������
	Out.vLight.z = dot(N, g_vWLightVec);	// ���[���h��Ԃ̃x�N�g����N�Ɠ��ς��ā@	�ڋ�Ԃ̂y�������
	Out.vLight = normalize(Out.vLight); // �Ō�ɐ��K�����ăx�N�g����


										// �����x�N�g����ڋ�Ԃ�
	float3 E = Out.wPos - g_vViewPos;		// �����x�N�g��
	Out.vE.x = dot(T, E);				// �������ڋ�Ԃ̂w�������
	Out.vE.y = dot(B, E);				// �������ڋ�Ԃ̂x�������
	Out.vE.z = dot(N, E);				// �������ڋ�Ԃ̂y�������
	Out.vE = normalize(Out.vE);			// �Ō�ɐ��K�����ăx�N�g����

	return Out;
}
//------------------------------------------------------
//		���s�N�Z���V�F�[�_�[
//------------------------------------------------------
float4 PS_WATER(VS_OUTPUT_WATER In) : COLOR
{
	float4 OUT = (float4)0;

	//UV�Q�쐬
	float2 UV1 = In.Tex1;
	float2 UV2 = In.Tex2;

	//**********************************************************
	// �����}�b�s���O
	//**********************************************************  
	// �}���`�}�b�v�̃s�N�Z��������͎����}�b�v�Ƃ��ĎQ�Ƃ��A�������擾����
	float h = tex2D(MultiSamp, UV1).r - 0.5f;
	h += tex2D(MultiSamp, UV2).r - 0.5f;
	h *= 0.5;//�@��T���v�����O���Ă邩�畽�ς�����Ă���

	// �e�N�Z���𒸓_���W�n�ł̎����x�N�g�������ɏd�݂����Ă��炷�B
	float3 E = normalize(In.vE);//�ڐ��̃x�N�g��
	UV1 -= 0.02f * h * E.xy /*+ m_fUvSea*/;//���ƂŐ��l�グ��
	UV2 -= 0.02f * h * E.xy /*+ m_fUvSea*/;

	/*************************/
	//	�����K�p��� �@���擾
	/*************************/

	//�@���}�b�v���Q�Ƃ��A�@�������������炵�擾����
	float3 NMap = (tex2D(NormalSamp, UV1).rgb - 0.5f)*2.0f;
	NMap += (tex2D(NormalSamp, UV2).rgb - 0.5f)*2.0f;
	NMap *= 0.5f;// ����2�񑫂��Ă�̂Ŕ���������

	//	�������˃x�N�g��
	float3 R = reflect(-E, NMap);

	//	���C�g�v�Z
	//In.vLight = normalize(In.vLight);
	float3 light;
	float rate = max(0.0f, dot(-In.vLight, NMap));//�@���̌v�Z�@���ςŌ��̔��˂����߂�
	light = rate;//

				 //	�X�y�L����
	float SpecPower = 1.0f;
	float S = pow(max(0, dot(R, In.vLight)), 80) * (SpecPower);

	// �X�y�L�������Z
	OUT.rgb += S;

	//******************************************************
	/// G_Buffer�����킹�擾����
	//******************************************************
	float2 G_Fetch = (In.wvpPos.xy / In.wvpPos.w)*0.5f + 0.5f;
	G_Fetch.y = -G_Fetch.y;



	//******************************************************
	/// Z�l�擾�@����
	//******************************************************
	////float fZ = tex2D(DepthBufSampWater, G_Fetch).r;	// �[�x�o�b�t�@���牜�s�擾
	////float Z = 1 / fZ;
	//
	//// �K�v�ȏ����擾
	const float4 NormalDepth = tex2D(NormalDepthSamp, G_Fetch);
	//const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	//const float3 envPos = CalcViewPosition(G_Fetch, NormalDepth.zw);
	const float3 envPos = CalcViewPosition(G_Fetch, NormalDepth.zw);
	////float4 proj;
	////proj.xy = (G_Fetch*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* NormalDepth.zw.y;
	////proj.zw = NormalDepth.zw;	// Z��W�͂��̂܂ܓ����
	//// �v���W�F�N�V�����t�s��Ńr���[���W�n�ɕϊ��@Proj->View
	////float4 worldPos = mul(proj, g_mInvVPMatrix);
	////worldPos.xyz /= worldPos.w;		// ���[���h��Ԃւ�W�Ŋ���K�v������


	////ZPos.xyz /= ZPos.w;
	//// PL��Pos�Ƃ��̃s�N�Z���̈ʒu�Ł��|�W�V����������
	////float3 g_vViewLightVec = g_vPLSpos.xyz - (Pos.xyz);
	////float dist = pow(max(0.0f, 1.0f - (length(g_vViewLightVec) / g_fPLSrange)), 2);//�����l��������i��

	//// �����̂�
	//float waterLen = length(g_vViewPos.y - g_fWaterHeight);

	//float vLen = length(envPos-In.vPos.xyz);

	////
	//float Zdist= length (waterLen - worldPos.y);
	////  �x�N�g���̒������O�����̒藝���狁�߁A���߂��x�N�g���̒�����Ԃ�
	////float Zdist = sqrt(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
	//

	/**************************************/
	//	�擾�������s��FOG
	/**************************************/
	// �[�x���[����ΐ[���قǓ��������Ȃ���

	//float ZNear = 100.0f;
	//float ZFar = 300.0f;
	////float ZNear = 0.99f;
	////float ZFar = 1.1f;
	//float3 ZCol = { 0.0f, 1.0f, 0.0f };
	//float ZRate = smoothstep(ZNear, ZFar, Zdist);

	//// �t�H�O�̐F���t�H�O�̒l������ 
	//ZCol = ZCol * (ZRate);
	//OUT.rgb += ZCol;

	//******************************************************
	/// ���܂̌���
	//******************************************************

	// �c�ݓx
	float distortion = 0.01f;

	// ���x
	float3 Env = tex2D(EnvFullBufSamp, G_Fetch + (float2(NMap.x, NMap.y)*distortion));
	
	//�[�x�l�𗘗p���ă}�X�N�����
	//float G_Depth = tex2D(DepthBufSampWater, G_Fetch + (float2(NMap.x, NMap.y)*distortion)).r;
	float4 proj;
	proj.xy = (G_Fetch*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* NormalDepth.zw.y;
	proj.zw = NormalDepth.zw;	// Z��W�͂��̂܂ܓ����
	// �v���W�F�N�V�����t�s��Ńr���[���W�n�ɕϊ��@Proj->View
	float4 worldPos = mul(proj, g_mInvVPMatrix);
	worldPos.xyz /= worldPos.w;		// ���[���h��Ԃւ�W�Ŋ���K�v������
	
	// (TODO) ���
	//float G_Depth = proj.z / proj.w;
	//float G_Depth = envPos.z;
	////float myDepth = In.wvpPos.z / In.wvpPos.w;			//w�Ŋ�����-1~1�ɕϊ�����
	//float myDepth = In.vPos.z;

	//// (if��)��O��Pixel���Ȃ��O�̃I�u�W�F�N�g�̏����Ƃ��Ă��Ȃ�����
	//if (myDepth > G_Depth)
	//{
	//	Env.rgb = tex2D(EnvFullBufSamp, G_Fetch);
	//}

	
	Env *= 0.7f;// �F���Â�����x����
	OUT.rgb += Env;// lerp(Env, OUT.rgb, 0.3f);


	//******************************************************
	/// �t���l������
	//******************************************************

	// ���̖@���x�N�g���Ǝn�_�x�N�g���œ��ρ@
	float fresnel = dot(normalize(g_vViewPos - In.wPos), In.Normal);

	// ���`���
	//float3 RefCol = { -0.1f, -0.08f, -0.0f };
	float3 RefCol = { 0.0f, 0.02f, 0.1f };
	float3 SeaCol = { 0.0f, 0.0f, 0.0f };
	float AbujustF = +0.05f;					// �t���l�����˂̒����p�����[�^�[�@�グ��Ǝ�܂邪��܂Ō����낷��-�̒l�ђʂ��Ă��`�悳���[1207]maxmin�ŉ���

	RefCol += tex2D(WaterReflectSamp, G_Fetch + (float2(NMap.x, NMap.y)*distortion));

	// �`��
	float3 fresnelCol;
	fresnelCol = lerp(RefCol, SeaCol, max(0.0f, min(1.0f, fresnel + AbujustF)));
	OUT.rgb += fresnelCol;

	/**************************************/
	//
	/**************************************/

	//OUT.rgb += ZCol;

	OUT.a = 1.0f;
	return OUT;
}

/* �� */
technique CrystalWater
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_WATER();
		PixelShader = compile ps_3_0 PS_WATER();

	}

}




//**************************************************************************************************
//
//		�C
//
//**************************************************************************************************

//------------------------------------------------------
//		���_�t�H�[�}�b�g
//------------------------------------------------------
struct VS_OUTPUT_SEA
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex1		: TEXCOORD0;
	float2 Tex2		: TEXCOORD1;

	//�s��
	float4 wPos		: TEXCOORD2;//	�s�N�Z���ɑ�����Ƀ��[���h����̃|�W�V�����ǉ�
	float4 RocalPos	: TEXCOORD3;//	�s�N�Z���ɑ�����ɃQ�[����ʃ��X�g�̃|�W�V�����ǉ�

	//���C�e�B���O
	//float4 Light	: COLOR1;
	float4 Ambient	: COLOR2;
	float3 Normal : COLOR3;
	float3 vLight	: TEXCOORD4;
	float3 vE		: TEXCOORD5;

	//�ǉ��@���W�n
	float4 ProjectionPos : TEXCOORD6;
};


struct VS_INPUT_SEA
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float2 Tex	  : TEXCOORD0;
};

////------------------------------------------------------
////		�t�H�O�֘A
////------------------------------------------------------
//float	AlphaNear = 10.0f;
//float	AlphaFar = 200.0f;
//float	SeaAlpha = 0.0f;//

float m_fUvSea = 0.0f;
//float3 m_fSeaColor = { 0.2f, 0.8f, 1.7f };
float3 m_fSeaColor = { 0.3f, 0.65f, 1.1f };


inline float4 CalcHemiLight(float3 normal)//�@�㉺�̊����𕪂���
{
	float4 color;
	float rate = (normal.y*0.5f) + 0.5f;
	color.rgb = g_vSkyColor * rate;
	color.rgb += g_vGroundColor * (1 - rate);
	color.a = 1.0f;

	return color;
}

inline float3 CalcDirLight(float3 dir, float3 normal)//�@���C�g�̌����Ō��̐F�̋�����Ԃ�
{
	float3 light;
	float rate = max(0.0f, dot(-dir, normal));//�@���̌v�Z�@���ςŌ��̔��˂����߂�
	light = g_vLightColor * rate;//

	return light;
}

//------------------------------------------------------
//	���}�b�v�p�@����̕��i���f�肱�ރ}�b�v
//------------------------------------------------------
texture EnvMap;		//	���e�N�X�`��
sampler EnvSamp = sampler_state
{
	Texture = <EnvMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};
//�@EnvSamp�@�͂�����ʂ��Ďg�p���@�@
inline float4 CalcEnvironment(float3 normal)
{
	float4	color;

	float2	uv;
	uv.x = normal.x*0.5f + 0.5f;
	uv.y = normal.y*0.5f + 0.5f;

	color = tex2D(EnvSamp, uv);
	return color;
}


VS_OUTPUT_SEA VS_Sea(VS_INPUT_SEA In)
{
	VS_OUTPUT_SEA Out = (VS_OUTPUT_SEA)0;

	//���[�J�����W�n��ł̒��_�̍��W
	Out.RocalPos = In.Pos;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);//�v���W�F�N�V�����s��ƍ���
	Out.ProjectionPos = Out.Pos;

	Out.Color = 1.0f;//In.Color;
	Out.Tex1 = In.Tex + float2(-m_fUvSea*1.5, -m_fUvSea * 1.5);
	Out.Tex2 = In.Tex + float2(m_fUvSea, m_fUvSea*1.5);


	float4 P = mul(In.Pos, g_mWMatrix);//���[���h���W�Ɏ����Ă���
	Out.wPos = mul(In.Pos, g_mWMatrix);//
	float3x3	mat = g_mWMatrix;//float4x4��TransMatrix�̍s����@float3x3��mat��
								  //�@���x�N�g��
	Out.Normal = mul(In.Normal, mat);//
	Out.Normal = normalize(Out.Normal);//

									   // �������C�e�B���O
	//Out.Ambient.rgb = CalcHemiLight(Out.Normal);

	// ���_���[�J�����W�n�Z�o
	float3	vx;
	float3	vy = { 0, 1, 0.001f };
	vx = cross(vy, Out.Normal);
	vx = normalize(vx);
	vy = cross(vx, Out.Normal);
	vy = normalize(vy);

	//	���C�g�x�N�g���␳ ���C�g�������������̊p�x
	Out.vLight.x = dot(vx, g_vWLightVec);			// ���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
	Out.vLight.y = dot(vy, g_vWLightVec);			// ���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
	Out.vLight.z = dot(Out.Normal, g_vWLightVec);	// ���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
	Out.vLight = normalize(Out.vLight);

	// �����x�N�g���␳
	float3 E = P - g_vViewPos;			 // �����x�N�g��
	Out.vE.x = dot(vx, E);			 // �����x�N�g���𒸓_���W�n�ɕϊ�����
	Out.vE.y = dot(vy, E);			 // �����x�N�g���𒸓_���W�n�ɕϊ�����
	Out.vE.z = dot(Out.Normal, E);   // �����x�N�g���𒸓_���W�n�ɕϊ�����
	Out.vE = normalize(Out.vE);//�ڐ�

							   ////	�t�H�O�v�Z
							   //Out.Ambient.a = (AlphaFar - Out.Pos.z) / (AlphaFar - AlphaNear);
							   //Out.Ambient.a = saturate(Out.Ambient.a);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�

	return Out;
}

float4 PS_Sea(VS_OUTPUT_SEA In) : COLOR
{
	float4	OUT;
//UV�Q�쐬
float2 UV1 = In.Tex1;
float2 UV2 = In.Tex2;

//**********************************************************
// �����}�b�s���O
//**********************************************************  
// �����}�b�v���Q�Ƃ��A�������擾����
float h = tex2D(MultiSamp, UV1).r - 0.5f;
h += tex2D(MultiSamp, UV2).r - 0.5f;
h *= 0.5;//�@��T���v�����O���Ă邩�畽�ς�����Ă���

		 // �e�N�Z���𒸓_���W�n�ł̎����x�N�g�������ɏd�݂����Ă��炷�B
float3 E = normalize(In.vE);// �ڐ�
UV2 -= 0.02f * h * E.xy + m_fUvSea;
UV1 -= 0.02f * h * E.xy + m_fUvSea;// ���ƂŐ��l�グ��

/*************************/
//	�����K�p��� �@���擾
/*************************/
//�@���}�b�v���Q�Ƃ��A�@�����擾����
float3 NMap = (tex2D(NormalSamp, UV1).rgb - 0.5f)*2.0f;
NMap += (tex2D(NormalSamp, UV2).rgb - 0.5f)*2.0f;
NMap *= 0.5f;

////	���C�g�v�Z�͂��Ȃ��I�@���͂�������
In.vLight = normalize(In.vLight);
//float3 light = CalcDirLight(In.vLight, NMap);
//light *= 0.5f;

//In.Color.rgb = light;//Ambient=�����{
////	�s�N�Z���F����
//OUT *= In.Color;

OUT = tex2D(DecaleSamp, UV1) + tex2D(DecaleSamp, UV2);
OUT *= 0.5;//�@��T���v�����O���Ă邩�畽�ς�����Ă���
OUT.rgb *= m_fSeaColor;// +light;// �C�̐F�w��
OUT.a = 1.0f;// �����x

/*************************/
//	�X�y�L�����}�b�v�擾
/*************************/
float4 sp_tex = tex2D(LightSamp, UV1);
sp_tex += tex2D(LightSamp, UV2);
sp_tex *= 0.5;//�@��T���v�����O���Ă邩�畽�ς�����Ă��Ă�

			  //	�������˃x�N�g��
float3 R = reflect(-E, NMap);
//	�X�y�L����
float3 S;
S = pow(max(0, dot(R, In.vLight)), 60) * (sp_tex);

//************************************************************
//�@����
//************************************************************
//�䂪�ݗʂ̌v�Z
//���̌��݂������Ȃ�قǋ��܂��Ȃ��悤�ɂ���
//float4 Offset = float4((Nmap - E).xy * 20, 0.0f, 0.0f);
//�@�ˉe�e�N�X�`���p
//float3 wvpNormal = normalize(mul(Nmap, (float3x3)Projection));
//float2 TexCoord;
//TexCoord = (In.Pos.xy + wvpNormal.xy * 1) / In.Pos.z;
//TexCoord = TexCoord * float2(0.5f, -0.5f) + 0.5f;

//�C��̐F
//float3 SeabedCol;
//SeabedCol = tex2Dproj(TEXSamp, TexCoord );
//OUT.rgb *= SeabedCol;

//	���}�b�v�@R����ꔽ�˃}�b�v��
float3 env = CalcEnvironment(R) * (0.75f - sp_tex.a);
OUT.rgb = OUT.rgb * env + OUT.rgb;
//*********************************************************************************************************
//�t���l������
//*********************************************************************************************************
//���_ �� ���_�x�N�g�� �� �@���x�N�g���̓��ς��v�Z
//float3 pos = In.Pos;
//float fresnel = dot(normalize(-E - In.RocalPos), normalize(In.Normal));

//////���ς̌��ʂ� 0.0f �ɋ߂��قǔ��˃}�b�v�̐F�������Ȃ�A 1.0f �ɋ߂��قǊC��}�b�v�̐F�������Ȃ�悤�ɐ��`��������
//float4 Col;
//float3 ReflectCol = float3(0.8, 0.5, 0.8);
//float3 SeabedCol = float3(0.0, 0.9, 0.2);
//
//Col.rgb = lerp(ReflectCol, SeabedCol, fresnel);
////Col.rgb = Col.rgb * Diffuse * WaterCol + S;
//OUT.rgb = Col.rgb;

//*********************************************************************************************************
//�����x
//*********************************************************************************************************  
//���̌��݂��v�Z����
//float Z = SeabedZ - In.PosWVP.z / m_ZF;
//�y�l���������Ȃ�قǓ����x����������i���ӂP�j
//Z = min(Z * 20.0f, 1.0f);
//Col.rgb = lerp(SeabedCol, Col.rgb, Z);

OUT.rgb += S;

//�@�ˉe�e�N�X�`���p
//float3 wvpNormal = normalize(mul(In.Normal, (float3x3)Projection));
//	ref = (In.wPos.xy + wvpNormal.xy * 1) / In.wPos.z;
//output.TexCoord = output.TexCoord * float2(0.5f, -0.5f) + 0.5f;

////���t���N�g�}�b�v
//float2 ref = (In.ProjectionPos.xy / In.ProjectionPos.w)*0.5f + 0.5f;
//ref.y = -ref.y;
//float3 Env = tex2D(RefSamp, ref - 0.2f * h * E.xy/*��+�ڐ�*/) * (OUT.rgb);
//Env *= 0.35f;
//OUT.rgb += Env;// lerp(Env, OUT.rgb, 0.3f);

			   //	�t�H�O�̗p
			   //OUT.a = ((1 - In.Ambient.a));


// �K�E�Ó]�̒l
OUT.rgb *= g_fOverDriveDim;

return OUT;
}

technique Sea
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Sea();
		PixelShader = compile ps_3_0 PS_Sea();
	}
}

