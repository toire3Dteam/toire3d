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