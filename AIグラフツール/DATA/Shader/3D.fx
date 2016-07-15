//********************************************************************
//																									
//		�R�c�p�V�F�[�_		
//
//********************************************************************

//------------------------------------------------------
//		�ϊ��s��
//------------------------------------------------------
float4x4 WMatrix;		// ���[���h�ϊ��s��
float4x4 VMatrix;		// �r���[�ϊ��s��
float4x4 PMatrix;		// �v���W�F�N�V�����ϊ��s��

float4x4 WVPMatrix;		// ���e�ϊ��s��
float4x4 VPMatrix;		// ���[���h���W���灨�v���W�F�N�V�������W�֎����Ă����s��

float4x4 InvPMatrix;	// �v���W�F�N�V�������W ->View���W�@�֖߂��t�s��
float4x4 InvVPMatrix;	// �v���W�F�N�V�������W -> ���[���h���W �֖߂��t�s��


//------------------------------------------------------
//		���֘A
//------------------------------------------------------
float3	ViewPos;		// ���[���h���W�̖ڂ̃|�W�V����

float3 LightVec;		// ���s���̌���

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

texture LightMap;	//	���}�b�v�e�N�X�`��
sampler LightSamp = sampler_state
{
	Texture = <LightMap>;
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

struct VS_INPUT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

//********************************************************************
//
//		��{�R�c�V�F�[�_�[		
//
//********************************************************************

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

	OUT.r = 1.0f;
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


/*�u�����f�B���O �I�v�V����*/
/*
BLEND_ZERO			�\�[�X�̐F�͍� (0, 0, 0, 0) �ł��B�u�����f�B���O�O�̏����͂���܂���B
BLEND_ONE			�\�[�X�̐F�͔� (1, 1, 1, 1) �ł��B�u�����f�B���O�O�̏����͂���܂���B
BLEND_SRC_COLOR		�s�N�Z���V�F�[�_�[����̃J���[ �f�[�^ (RGB) �ł��B�u�����f�B���O�O�̏����͂���܂���B
BLEND_INV_SRC_COLOR �s�N�Z���V�F�[�_�[����̃J���[ �f�[�^ (RGB) �ł��B�u�����f�B���O�O�̏����ɂ���ăf�[�^�����]����A1 - RGB ����������܂��B
BLEND_SRC_ALPHA		�s�N�Z���V�F�[�_�[����̃A���t�@ �f�[�^ (A) �ł��B�u�����f�B���O�O�̏����͂���܂���B
BLEND_INV_SRC_ALPHA �s�N�Z���V�F�[�_�[����̃A���t�@ �f�[�^ (A) �ł��B�u�����f�B���O�O�̏����ɂ���ăf�[�^�����]����A1 - A ����������܂��B
BLEND_DEST_ALPHA	�����_�[�^�[�Q�b�g����̃A���t�@ �f�[�^�ł��B�u�����f�B���O�O�̏����͂���܂���B
*/






