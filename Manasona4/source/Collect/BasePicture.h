#pragma once
#include "TDNLIB.h"

//+---------------------------------
//	��{�̃R���N�V�����p�̃C���X�g
//+---------------------------------
//illust

// �A�C�R���p�̃f�[�^
struct PICTURE_ICON_DATA
{
	tdn2DObj* pIcon;	// �A�C�R��
	std::string sTitle;	// �^�C�g��
	std::string sText;	// ����
	Vector2 vPos;		// �ꏊ
	float iAlpha;		// �����x

	PICTURE_ICON_DATA();
};

// ���s�������p�̃f�[�^ (1280x720�̉摜�Ƃ�)
struct PICTURE_DATA
{
	tdn2DObj* pPic;		// �A�C�R��
	std::string sTitle;	// �^�C�g��
	std::string sText;	// ����
	Vector2 vPos;		// �ꏊ
	float fScale;		// �g��k��
	float iAlpha;		// �����x

	PICTURE_DATA();
};

class BasePicture
{
public:
	BasePicture();
	virtual ~BasePicture();

	// �A�C�R���֘A
	// virtual void UpdateIcon();
	void RenderIcon(int iX, int iY);
	void RenderIconNonPurchase(int iX, int iY);	//	���w���ŃA�C�R��
	void RenderIconInfo(int iX, int iY);

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	// �ꊇ�œǂݍ��ނƑ�ςȂ��ƂɂȂ�͂��B
	virtual void InitExe() = 0;
	virtual void RereaseExe(); // ���I������摜�͂��̓s�x�������
	void UpdateExe();
	void CtrlExe(int iDevice);	// �摜����
	void RenderExe();
	void RenderExeInfo(int iX, int iY);

	//virtual void AnLock();	//�@���b�N����

	// �A�N�Z�T
	PICTURE_ICON_DATA GetPictureIcon() { return m_tagPictureIcon; }	// �C���X�g�A�C�R���̍\����
	PICTURE_DATA GetPicture() { return m_tagPicture; }				// �C���X�g�̍\����
																	
	void SetIconPos(Vector2 vPos) { m_tagPictureIcon.vPos = vPos; }	// �{���p�̃|�W�V�����̐ݒ�
	bool isRock() { return m_bRock; }

	int GetPrice() { return m_iPrice; }
protected:

	PICTURE_ICON_DATA	m_tagPictureIcon;	// �A�C�R���̍\����
	PICTURE_DATA		m_tagPicture;		// �J�������̃C���X�g


	bool m_bRock;			// ���󂳂�Ă郍�b�N�@(�����Ŕ����Ȃ��E�T���l�����}�[�N��)
	int m_iPrice;			// �l�i
};

//+---------------------------------
//	�A�C���\�̃C���X�g
//+---------------------------------
class AirouPicture :public BasePicture
{
public:
	AirouPicture();
	~AirouPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

//+---------------------------------
//	�_�l�Ԃ̃C���X�g
//+---------------------------------
class TekiPicture :public BasePicture
{
public:
	TekiPicture();
	~TekiPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

//+---------------------------------
//	��̃C���X�g
//+---------------------------------
class NazenarabaPicture :public BasePicture
{
public:
	NazenarabaPicture();
	~NazenarabaPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

//+---------------------------------
//	�A���~�^�}�̃C���X�g
//+---------------------------------
class AramitamaPicture :public BasePicture
{
public:
	AramitamaPicture();
	~AramitamaPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

//+---------------------------------
//	�Z�M�̃C���X�g
//+---------------------------------
class AnikiPicture :public BasePicture
{
public:
	AnikiPicture();
	~AnikiPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

//+---------------------------------
//	�ꖇ�G�̃C���X�g
//+---------------------------------
class RestPicture :public BasePicture
{
public:
	RestPicture();
	~RestPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class GatheringPicture :public BasePicture
{
public:
	GatheringPicture();
	~GatheringPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class StylishPicture :public BasePicture
{
public:
	StylishPicture();
	~StylishPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class CoolPicture :public BasePicture
{
public:
	CoolPicture();
	~CoolPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class AlleyPicture :public BasePicture
{
public:
	AlleyPicture();
	~AlleyPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class SadPicture :public BasePicture
{
public:
	SadPicture();
	~SadPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class SexyPicture :public BasePicture
{
public:
	SexyPicture();
	~SexyPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class SeaPicture :public BasePicture
{
public:
	SeaPicture();
	~SeaPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};


class WinTekiPicture :public BasePicture
{
public:
	WinTekiPicture();
	~WinTekiPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};


class WinAnikiPicture :public BasePicture
{
public:
	WinAnikiPicture();
	~WinAnikiPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};


class Manatu1Picture :public BasePicture
{
public:
	Manatu1Picture();
	~Manatu1Picture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};


class SuikaPicture :public BasePicture
{
public:
	SuikaPicture();
	~SuikaPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};


class MahuyuPicture :public BasePicture
{
public:
	MahuyuPicture();
	~MahuyuPicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};


class ToirePicture :public BasePicture
{
public:
	ToirePicture();
	~ToirePicture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class Manatu2Picture :public BasePicture
{
public:
	Manatu2Picture();
	~Manatu2Picture() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

