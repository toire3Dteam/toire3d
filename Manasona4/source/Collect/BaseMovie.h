#pragma once
#include "TDNLIB.h"

//+---------------------------------
//	��{�̃R���N�V�����p�̃C���X�g
//+---------------------------------

// �A�C�R���p�̃f�[�^
struct MOVIE_ICON_DATA
{
	tdn2DObj* pIcon;	// �A�C�R��
	std::string sTitle;	// �^�C�g��
	std::string sText;	// ����
	Vector2 vPos;		// �ꏊ
	float iAlpha;		// �����x

	MOVIE_ICON_DATA();
};

// ���s�������p�̃f�[�^ (1280x720�̉摜�Ƃ�)
struct MOVIE_DATA
{
	tdnMovie* pMovie;	// ���[�r�[
	tdn2DObj* pPic;		// �o�͐�摜
	std::string sTitle;	// �^�C�g��
	std::string sText;	// ����

	MOVIE_DATA();
};

class BaseMovie
{
public:
	BaseMovie();
	virtual ~BaseMovie();

	// �A�C�R���֘A
	// virtual void UpdateIcon();
	void RenderIcon(int iX, int iY);
	void RenderIconNonPurchase(int iX, int iY);	//	���w���ŃA�C�R��
	void RenderIconInfo(int iX, int iY);

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	// �ꊇ�œǂݍ��ނƑ�ςȂ��ƂɂȂ�͂��B
	virtual void InitExe() = 0;
	virtual void RereaseExe(); // ���I������摜�͂��̓s�x�������
	bool UpdateExe();
	void CtrlExe(int iDevice);	// �摜����
	void RenderExe();
	void RenderExeInfo(int iX, int iY);

	//virtual void AnLock();	//�@���b�N����

	// �A�N�Z�T
	MOVIE_ICON_DATA GetMovieIcon() { return m_tagMovieIcon; }	// �C���X�g�A�C�R���̍\����
	MOVIE_DATA GetMovie() { return m_tagMovie; }				// �C���X�g�̍\����
																	
	void SetIconPos(Vector2 vPos) { m_tagMovieIcon.vPos = vPos; }	// �{���p�̃|�W�V�����̐ݒ�
	bool isRock() { return m_bRock; }

	int GetPrice() { return m_iPrice; }
protected:

	MOVIE_ICON_DATA	m_tagMovieIcon;	// �A�C�R���̍\����
	MOVIE_DATA		m_tagMovie;		// �J�������̃C���X�g


	bool m_bRock;			// ���󂳂�Ă郍�b�N�@(�����Ŕ����Ȃ��E�T���l�����}�[�N��)
	int m_iPrice;			// �l�i
};

//+---------------------------------
//	���ꂼ��̃��[�r�[
//+---------------------------------
class OpMovie :public BaseMovie
{
public:
	OpMovie();
	~OpMovie() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};

class OldOpMovie :public BaseMovie
{
public:
	OldOpMovie();
	~OldOpMovie() {};

	// �����Ȃ莿�̗ǂ��摜��ǂݍ��ނ̂őI�������Ƃ��ɉ摜�����[�h������
	void InitExe();

private:

};
