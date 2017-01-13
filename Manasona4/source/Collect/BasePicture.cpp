#include "BasePicture.h"



//+------------------------------
//	��{�̃R���N�V�����p�̃C���X�g
//+------------------------------

// �A�C�R���p�̃f�[�^�̏�����
PICTURE_ICON_DATA::PICTURE_ICON_DATA()
{
	pIcon = nullptr;
	sTitle = "�Ȃ�";
	sText = "�Ȃ�";
	vPos = VECTOR2_ZERO;
	iAlpha = 1.0f;
}

// �摜�p�̃f�[�^�̏�����
PICTURE_DATA::PICTURE_DATA()
{
	pPic = nullptr;
	sTitle = "�Ȃ�";
	sText = "�Ȃ�";
	vPos = VECTOR2_ZERO;
	fScale = 1.0f;
	iAlpha = 1.0f;

}

//+-------------------------
//	�������E���
//+-------------------------
BasePicture::BasePicture()
{

	m_bRock = false;
	m_iPrice = 9999;
}

BasePicture::~BasePicture()
{
	SAFE_DELETE(m_tagPictureIcon.pIcon);
}

//+-------------------------
//	�X�V�E�`��	(�A�C�R��)
//+-------------------------
//void BasePicture::UpdateIcon()
//{
//
//
//}

void BasePicture::RenderIcon(int iX, int iY)
{
	// �A�C�R��
	m_tagPictureIcon.pIcon->Render(
		(int)m_tagPictureIcon.vPos.x + iX,
		(int)m_tagPictureIcon.vPos.y + iY);

}

//	���w���ŃA�C�R��
void BasePicture::RenderIconNonPurchase(int iX, int iY)
{
	// �Â�
	m_tagPictureIcon.pIcon->SetARGB(0xff777777);
	
	// �A�C�R��
	m_tagPictureIcon.pIcon->Render(
		(int)m_tagPictureIcon.vPos.x + iX,
		(int)m_tagPictureIcon.vPos.y + iY);

	// ���ɖ߂�
	m_tagPictureIcon.pIcon->SetARGB(0xffffffff);

}

void BasePicture::RenderIconInfo(int iX, int iY)
{
	// �^�C�g��
	tdnFont::RenderString(m_tagPictureIcon.sTitle.c_str(), "HG�޼��E",
		22, iX, iY, 0xffffffff, RS::COPY);
	// ����
	tdnFont::RenderString(m_tagPictureIcon.sText.c_str(), "HG�޼��E",
		19, iX, iY + 30, 0xffffffff, RS::COPY);

}

//void BasePicture::InitExe()
//{
//
//}

// �摜���
void BasePicture::RereaseExe()
{
	SAFE_DELETE(m_tagPicture.pPic);
}

//+-------------------------
//	�X�V�E�`��	(�N�������Ƃ�)
//+-------------------------
void BasePicture::UpdateExe()
{

}

void BasePicture::CtrlExe(int iDevice)
{
	// �Z���N�g�L�[�ŏ�����
	if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, iDevice)||
		tdnInput::KeyGet(KEYCODE::KEY_R3, iDevice))
	{
		m_tagPicture.vPos = VECTOR2_ZERO;
		m_tagPicture.fScale = 1.0f;
	}


	// �X�e�B�b�N�덷�p
	static const float l_fAbj(0.001f);

	// ���X�e�B�b�N�̎擾
	float l_iLX, l_iLY;
	tdnInput::GetAxisXYf(&l_iLX, &l_iLY, iDevice);

	if (l_iLX > 0.05f - l_fAbj)
	{
		m_tagPicture.vPos.x += l_iLX * 5;
	}
	else if (l_iLX < -0.05f + l_fAbj)
	{
		m_tagPicture.vPos.x += l_iLX * 5;
	}
	if (l_iLY > 0.05f - l_fAbj)
	{
		m_tagPicture.vPos.y += l_iLY * 5;
	}
	else if (l_iLY < -0.05f + l_fAbj)
	{
		m_tagPicture.vPos.y += l_iLY * 5;
	}

	// �\���L�[�ňړ�
	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDevice))
	{
		m_tagPicture.vPos.x += 5;
	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDevice))
	{
		m_tagPicture.vPos.x -= 5;
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_UP, iDevice))
	{
		m_tagPicture.vPos.y -= 5;
	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDevice))
	{
		m_tagPicture.vPos.y += 5;
	}


	// �g��
	// �E�X�e�B�b�N�̎擾
	float l_iRX, l_iRY;
	tdnInput::GetAxisXY2f(&l_iRX, &l_iRY, iDevice);

	if (l_iRY > 0.05f - l_fAbj)
	{
		m_tagPicture.fScale -= 0.01f;
	}
	else if (l_iRY < -0.05f + l_fAbj)
	{
		m_tagPicture.fScale += 0.01f;
	}

	// �X�P�[�����}�C�i�X��g�債�����Ȃ��悤�ɃN�����v
	m_tagPicture.fScale = Math::Clamp(m_tagPicture.fScale, 0.5f, 1.5f);

}

void BasePicture::RenderExe()
{
	// �摜�̐ݒ�
	m_tagPicture.pPic->SetScale(m_tagPicture.fScale);

	// �ӏܗp
	m_tagPicture.pPic->Render(
		(int)m_tagPicture.vPos.x,
		(int)m_tagPicture.vPos.y);
}

void BasePicture::RenderExeInfo(int iX, int iY)
{
	// �^�C�g��
	tdnFont::RenderString(m_tagPicture.sTitle.c_str(), "HG�޼��E",
		22, iX, iY, 0xffffffff, RS::COPY);
	// ����
	tdnFont::RenderString(m_tagPicture.sText.c_str(), "HG�޼��E",
		18, iX+8, iY + 32, 0xffffffff, RS::COPY);
}


//+---------------------------------
//	�A�C���\�̃C���X�g
//+---------------------------------
AirouPicture::AirouPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Airou.png");
	m_tagPictureIcon.sTitle = "�y�L�����N�^�[�C���X�g�z�A�C���\";
	m_tagPictureIcon.sText = "�A�C���\�̃L�����N�^�[�C���X�g";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void AirouPicture::InitExe()
{
	m_tagPicture.pPic=new tdn2DObj("Data/UI/Collect/Picture/Airou.png");
	m_tagPicture.sTitle = "�y�L�����N�^�[�C���X�g�z�A�C���\";
	m_tagPicture.sText = "�A�C���\�̃L�����N�^�[�C���X�g";
	m_tagPicture.vPos.x = -300.0f;
	m_tagPicture.vPos.y = -200.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	�e�L�̃C���X�g
//+---------------------------------
TekiPicture::TekiPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Teki.png");
	m_tagPictureIcon.sTitle = "�y�L�����N�^�[�C���X�g�z�e�L";
	m_tagPictureIcon.sText = "�e�L�̃L�����N�^�[�C���X�g";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void TekiPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Teki.png");
	m_tagPicture.sTitle = "�y�L�����N�^�[�C���X�g�z�e�L";
	m_tagPicture.sText = "�e�L�̃L�����N�^�[�C���X�g";
	m_tagPicture.vPos.x = -100.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+---------------------------------
//	��̃C���X�g
//+---------------------------------
NazenarabaPicture::NazenarabaPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Nazenaraba.png");
	m_tagPictureIcon.sTitle = "�y�L�����N�^�[�C���X�g�z(��)";
	m_tagPictureIcon.sText = "(��)�̃L�����N�^�[�C���X�g";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void NazenarabaPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Nazenaraba.png");
	m_tagPicture.sTitle = "�y�L�����N�^�[�C���X�g�z(��)";
	m_tagPicture.sText = "(��)�̃L�����N�^�[�C���X�g";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}


//+---------------------------------
//	�ꖇ�G�̃C���X�g
//+---------------------------------
RestPicture::RestPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Rest.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z��������";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void RestPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Rest.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z��������";
	m_tagPicture.sText = "�݂�Ȃł킢�킢�������сB\n���W���[�V�[�g�̏�ł��ꂩ������Ă����Ƃ��Ă���厖�̈����O�̏u�ԁB";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
GatheringPicture::GatheringPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Gathering.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�W���ʐ^";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void GatheringPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Gathering.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�W���ʐ^";
	m_tagPicture.sText = "�݂�Ȏʐ^�Ɏʂ肽�����Č�낪�f���Ă��Ȃ��A������傢�Ȏʐ^�B";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
StylishPicture::StylishPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Stylish.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�X�^�C���b�V��";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void StylishPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Stylish.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�X�^�C���b�V��";
	m_tagPicture.sText = "�e�X��߂Ă���v��������B";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
CoolPicture::CoolPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Cool.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�U�k�@�������ۂ������Ă݂�";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void CoolPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Cool.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�U�k�@�������ۂ������Ă݂�";
	m_tagPicture.sText = "�C�P����";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
AlleyPicture::AlleyPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Alley.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�H�n��";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void AlleyPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Alley.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�H�n��";
	m_tagPicture.sText = "�A�C���\�͕��R�Ɨ��؂�B";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
SadPicture::SadPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Sad.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�J";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void SadPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sad.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�J";
	m_tagPicture.sText = "���Ȃ������B";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
SexyPicture::SexyPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Sexy.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�ڊo�߂���";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void SexyPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sexy.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�ڊo�߂���";
	m_tagPicture.sText = "�Z�N�V�[�Ɉ����ꂽ�A���~�^�}";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}

//+-------------------------------------
// �ݒ�
SeaPicture::SeaPicture()
{
	m_tagPictureIcon.pIcon = new tdn2DObj("Data/UI/Collect/Icon/Sea.png");
	m_tagPictureIcon.sTitle = "�y�X�y�V�����C���X�g�z�C";
	m_tagPictureIcon.sText = "���ʂȃC���X�g�ł��B";

	// �B���g���t�B�[�t���O
	m_bRock = false;

	// �l�i
	m_iPrice = 10;
}

// ���s��̉摜�̏�����
void SeaPicture::InitExe()
{
	m_tagPicture.pPic = new tdn2DObj("Data/UI/Collect/Picture/Sea.png");
	m_tagPicture.sTitle = "�y�X�y�V�����C���X�g�z�C";
	m_tagPicture.sText = "�ޗǌ��ɂ͊C���Ȃ��B";
	m_tagPicture.vPos.x = 0.0f;
	m_tagPicture.vPos.y = 0.0f;
	m_tagPicture.fScale = 1.0f;
	m_tagPicture.iAlpha = 1.0f;
}