//------------------------------------------------------
//		�ϊ��s��
//------------------------------------------------------
float4x4 WMatrix;		// ���[���h�ϊ��s��
float4x4 VMatrix;		// �r���[�ϊ��s��
float4x4 PMatrix;		// �v���W�F�N�V�����ϊ��s��

float4x4 WVPMatrix;		// ���e�ϊ��s��
float4x4 VPMatrix;		// ���[���h���W���灨�v���W�F�N�V�������W�֎����Ă����s��

float4x4 InvPMatrix;	// �v���W�F�N�V�������W ->View���W�@�֖߂��t�s��
float4x4 InvVMatrix;	// View���W ->���[���h���W�@�֖߂��t�s��
float4x4 InvVPMatrix;	// �v���W�F�N�V�������W -> ���[���h���W �֖߂��t�s��

float4x4 ShadowProjection; // �e

//------------------------------------------------------
//		���֘A
//------------------------------------------------------
float3	ViewPos;		// ���[���h���W�̖ڂ̃|�W�V����

float3 LightVec;		// ���s���̌���
float3 ViewLightVec;	// �r���[���W�ł̌���̌���
float3 LightColor = { 1.0f, 1.0f, 1.0f };

float3 SkyColor = { .4f, .6f, .6f };
float3 GroundColor = { .6f, .4f, .4f };

float RimPowerRate = 2.0f;		 // �������C�g�̌��̕� 
float EmissivePowerRate = 2.0f;// �G�~�b�V�u�̕� �G�~�b�V�u������Ȃ����̓e�N�X����ς������̐��l��������
// ����0��

float FogNear = 514.0f;
float FogFar = 1024.0f;

float3 FogColor = { 100.0f, 100.0f, 100.0f };

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

	Out.Pos = mul(In.Pos, WMatrix);
	Out.Pos += In.worldPos;

	//float4x4 VPMatrix;
	//VPMatrix = mul(VMatrix, PMatrix);

	Out.Pos = mul(Out.Pos, VPMatrix);

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
	Out.wPos = mul(In.Pos, WMatrix);

	Out.Pos = mul(In.Pos, WVPMatrix);
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
//		G_Buffer
//*****************************************

VS_DEFERRED VS_G_Buffer(VS_INPUT In)
{
	VS_DEFERRED Out = (VS_DEFERRED)0;

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = In.Color;
	Out.Color.a = 1.0f;

	// �X�N���[�����W�ł̐[�x�l�ۑ�
	Out.Depth = Out.Pos;

	// �@�����̏C��
	float3x3 WVmat = (float3x3)mul(WMatrix, VMatrix); // ���f�������[���h���r���[��Ԃւ̕ϊ��s��

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
	float4 viewPos = mul(proj, InvPMatrix);
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
	float4x4	mat = mul(WMatrix, ShadowProjection);
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
	float rate = (dot(Normal, -ViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 Lambert = max(0, rate);				// Lambert

	// �s�N�Z���̐F
	OUT.color.rgb = (HalfLambert * LightColor);
	OUT.color.a = 1.0f;

	//�@�X�y�L�������[�g�擾
	float sp = CalcSpecular(Pos, Normal, ViewLightVec, 80);
	// �X�y�L�����̐F���ǂꂭ�炢�悹�邩
	OUT.spec.rgb = sp*LightColor;
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
	float rate = (dot(Normal, -ViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 Lambert = max(0, rate);				// Lambert

	// �s�N�Z���̐F
	OUT.color.rgb = (HalfLambert * LightColor);
	OUT.color.a = 1.0f;

	//�@�X�y�L�������[�g�擾
	float specRate = CalcSpecular(Pos, Normal, ViewLightVec, RoughnessLightMap.g * 255);
	// �X�y�L�����̐F���ǂꂭ�炢�悹�邩
	OUT.spec.rgb = (specRate*LightColor)*RoughnessLightMap.r;
	OUT.spec.a = 1.0f;

	float4 ShadowTex;
	float ShadowValue = 0.0f;
	float2 depth;		//	�o���A���X�V���h�E�}�b�v

	{
		// ���z���_���猩���v���W�F�N�V�������W�̃I�u�W�F
		ShadowTex = mul(mul(float4(Pos, 1), InvVMatrix), ShadowProjection);
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

		if (depth.r < fragDepth - 0.001f)
		{
			const float variance = depth.g - depth.r * depth.r;
			ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
		}

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
	OUT.rgb = SkyColor * rate;
	OUT.rgb += GroundColor * (1.0f - rate);
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
float3 PLSpos;		// ��View���W�n�ɕϊ����Ă�������
float  PLSrange = 30.0f;
float3 PLScolor = float3(300, 600, 300);// �J���[
float  PLSpower = 2.0f;// �p���[

//------------------------------------------------------
//		PointLightSphere�̒��_�V�F�[�_�[
//------------------------------------------------------
VS_POINTLIGHT VS_PointLightSphere(VS_INPUT In)
{
	VS_POINTLIGHT Out = (VS_POINTLIGHT)0;

	// �X�P�[����@���ɂ��������Ċg�� �����W��
	In.Pos.xyz += In.Normal*PLSrange;
	// ��΂ɑ���K�v������@�{�[���̃|�W�V����
	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.WPos = mul(In.Pos, WMatrix);

	//�@�t�F�b�`�p
	Out.wvpPos = Out.Pos;

	return Out;
}


DEF_POINTLIGHT PS_PointLightSphere(VS_POINTLIGHT In)
{
	DEF_POINTLIGHT OUT = (DEF_POINTLIGHT)0;

	//�X�N���[����Ԃ��e�N�X�`�����W�� ��
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	// �K�v�ȏ����擾�@��
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);// ��
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	// PL��Pos�Ƃ��̃s�N�Z���̈ʒu�Ł��|�W�V����������
	float3 ViewLightVec = PLSpos.xyz - (Pos.xyz);
	float dist = pow(max(0.0f, 1.0f - (length(ViewLightVec) / PLSrange)), 2);//�����l��������i��

	// �@���}�b�v����A�e�Ւf
	ViewLightVec = normalize(ViewLightVec);// ��<-�ꏊ�x�N�g����
	float rate = max(0, dot(Normal.xyz, ViewLightVec.xyz));

	OUT.color.rgb = (PLScolor*(dist*rate))*PLSpower;
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
float TU = 1.0f / 1280.0f, TV = 1.0f / 720.0f;
float BlurValue = 1.0f;

/***************************************/
/*		�K�E�V�A���u���[			�@ */
float4 PS_gaussX(float2 Tex : TEXCOORD0) : COLOR
{

	////�e�N�Z�����擾   
	//float2 Texel0 = Tex + float2(-TU * 1 * BlurValue, 0.0f);
	//float2 Texel1 = Tex + float2(-TU * 2 * BlurValue, 0.0f);
	//float2 Texel2 = Tex + float2(-TU * 3 * BlurValue, 0.0f);
	//float2 Texel3 = Tex + float2(-TU * 4 * BlurValue, 0.0f);
	//float2 Texel4 = Tex + float2(-TU * 5 * BlurValue, 0.0f);

	//float2 Texel5 = Tex + float2(TU * 1 * BlurValue, 0.0f);
	//float2 Texel6 = Tex + float2(TU * 2 * BlurValue, 0.0f);
	//float2 Texel7 = Tex + float2(TU * 3 * BlurValue, 0.0f);
	//float2 Texel8 = Tex + float2(TU * 4 * BlurValue, 0.0f);
	//float2 Texel9 = Tex + float2(TU * 5 * BlurValue, 0.0f);

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

	Out += tex2D(GaussianSamp, Tex + float2(-TU * 1 * BlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 2 * BlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 3 * BlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 4 * BlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 5 * BlurValue, 0.0f))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(TU * 1 * BlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 2 * BlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 3 * BlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 4 * BlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 5 * BlurValue, 0.0f))*0.04f;

	return Out;

}

float4 PS_gaussY(float2 Tex : TEXCOORD0) : COLOR
{

	////�e�N�Z�����擾   
	//float2 Texel0 = Tex + float2(0.0, -TV * 1 * BlurValue);
	//float2 Texel1 = Tex + float2(0.0, -TV * 2 * BlurValue);
	//float2 Texel2 = Tex + float2(0.0, -TV * 3 * BlurValue);
	//float2 Texel3 = Tex + float2(0.0, -TV * 4 * BlurValue);
	//float2 Texel4 = Tex + float2(0.0, -TV * 5 * BlurValue);

	//float2 Texel5 = Tex + float2(0.0, TV * 1 * BlurValue);
	//float2 Texel6 = Tex + float2(0.0, TV * 2 * BlurValue);
	//float2 Texel7 = Tex + float2(0.0, TV * 3 * BlurValue);
	//float2 Texel8 = Tex + float2(0.0, TV * 4 * BlurValue);
	//float2 Texel9 = Tex + float2(0.0, TV * 5 * BlurValue);

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

	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 1 * BlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 2 * BlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 3 * BlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 4 * BlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 5 * BlurValue))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 1 * BlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 2 * BlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 3 * BlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 4 * BlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 5 * BlurValue))*0.04f;

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

float CenterX = 0.0f;
float CenterY = 0.0f;

float BlurPower = 1.0f;
const float IMAGE_SIZE = 512.0f;

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
	float2 ss_center = float2((CenterX + 1.0f) * 0.5f, (-CenterY + 1.0f) * 0.5f);

	//�@�I�t�Z�b�g
	float2 uvOffset = (ss_center - Tex) * (BlurPower / IMAGE_SIZE);

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

float DirectionalX = 1.0f;
float DirectionalY = 0.0f;
float DirectionalBlurPower = 0.015f;
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
	float2 uvOffset = float2(DirectionalX, DirectionalY) *(DirectionalBlurPower);

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
float exposure = 0.0f;

// ���P�x
float3 g_bloomColor = { 0.9f, 0.9f, 0.9f };

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
	Out.wPos = mul(In.Pos, WMatrix);

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.wvpPos = Out.Pos;

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
	OUT.color.rgb *= exp2(exposure);
	
	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));
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
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_DefaultLighting();
	}
}

/*************************************/
// �X�e�[�W�ɕK�v�ȕϐ�
/*************************************/

float    g_OverDriveDim = 1.0f;

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
	OUT.color.rgb *= g_OverDriveDim;

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(exposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));
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
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Stage();
	}
}


/*************************************/
// �A�E�g���C���ɕK�v�ȕϐ�
/*************************************/

float    g_OutlineSize = 0.005f;

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

	OUT.Pos = mul(In.Pos, WVPMatrix);
	OUT.Pos.xy += normalize(mul(mul(float4(In.Normal, 0), WMatrix), VPMatrix).xy) * OUT.Pos.z * g_OutlineSize;
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

	OUT.color.rgb = col.rgb - float3(0.25,0.5,0.5);// �ӂ��͏����Â�
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
		ZWriteEnable = false;		// ���s���������ނ�

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
//		�v���C���[�p�̃O���[�o���G���A
//------------------------------------------------------
float g_InvincibleColRate = 0.0f;//Flash ���̃L�����N�^�[�_�E����̓_�ł̃��[�g�@����
float g_OrangeColRate = 0.0f;//�@�I�����W�̌�
float g_MagentaColRate = 0.0f;//�@�}�[���^�̌�
float g_OverDriveColRate = 0.0f;//


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
	OUT.color.rgb *= exp2(exposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));
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
		SrcBlend = SrcAlpha;		// ���ݕ`���Ă��
		DestBlend = InvSrcAlpha;	// �`����Ă����
		CullMode = CCW;				// �J�����O�̎d�l

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

	float4 lightCol = tex2D(LightSamp, ScreenTex);
		lightCol += tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	// ���C�g��(�n�[�t�����o�[�g)
	float rate = (dot(Normal, -ViewLightVec));
	float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
		OUT.color.rgb *= toonShadowCol;

	//OUT.color.g += 0.5;
	float3 E = Pos.xyz;// �r���[�̖ڐ�
		E = normalize(E);
	float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)), 4.0f);
	float RimLightPower = max(.0f, dot(-E, ViewLightVec));

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(exposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));
	OUT.high.a = 1.0f;

	// �L���T�����̔�������
	OUT.color.rgb += (RimPower)* float3(1, 1, 1);

	// �I�[�o�[�h���C�u�p
	//float RimPower2 = pow(1.0f - max(0.0f, dot(-E, Normal)), 1.0f);
	OUT.high.rgb += float3(0.0, 0.2, 0.4)*g_OverDriveColRate;
	OUT.high.rgb += float3(0.8, 0.5, 0.0)*g_OrangeColRate;
	OUT.high.rgb += float3(0.7, 0.0, 0.4)*g_MagentaColRate;
	//���P�x���o��ɂ��Ȃ���HDR�Ō����Ă��܂��̂ōŌ��
	OUT.color.rgb += g_InvincibleColRate;

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
	//float rate = (dot(Normal, -ViewLightVec));
	//float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
	//	OUT.color.rgb *= toonShadowCol;

		// �s�N�Z���̐F
		//OUT.color.rgb = (HalfLambert * LightColor);
	

	// ���������C�e�B���O
	float3 E = Pos.xyz;// �r���[�̖ڐ�
	E = normalize(E);

	float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)),2.0f);

	// �y���\�i�̐���
	OUT.color.rgb += RimPower * float3(0, 0.4, 2.5);

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(exposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));

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
	Out.wPos = mul(In.Pos, WMatrix);

	Out.Pos = mul(In.Pos, WVPMatrix);
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

	//float4 lightCol = tex2D(LightSamp, ScreenTex);
	//	lightCol += tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	////OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	//OUT.color.g += 0.5;


	// �K�E�Ó]�̒l
	OUT.color.rgb *= g_OverDriveDim;

	//�g�[���}�b�s���O
	OUT.color.rgb *= exp2(exposure);
	//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
	OUT.high.rgb = float3(0.05f, 0.1f, 0.3f)*g_OverDriveDim;// �K�E�Ó]�̒l
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
float tuAnime = 0.0f;
float tvAnime = 0.0f;
float alphaUV = 1.0f;

VS_OUTPUT_UV VS_UvAnime(VS_INPUT_UV In)
{
	VS_OUTPUT_UV Out = (VS_OUTPUT_UV)0;

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.wvpPos = Out.Pos;
	Out.Normal = In.Normal;
	Out.Color = In.Color;// ���_�J���[�擾
	Out.Tex = In.Tex + float2(tuAnime, tvAnime);//���W

	Out.Color.rgb = 1.0f;
	Out.Color.a *= alphaUV; //�@�����x

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
	//OUT.color.rgb *= exp2(exposure); ���͍l���͂Ȃ���
	
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
	//OUT.color.rgb *= exp2(exposure); ���͍l���͂Ȃ���

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
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;			// ���̃I�u�W�F�N�g��Z�o�b�t�@���l������
		ZWriteEnable = false;	// ���̃I�u�W�F�N�g��Z�o�b�t�@�ɏ������܂Ȃ�

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime_Guard();
	}
}