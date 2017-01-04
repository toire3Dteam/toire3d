#pragma once
#include "TDNLIB.h"

//+------------------------------
//	��{�̃g���t�B�[
//+------------------------------

struct TROPHY_DATA
{
	tdn2DObj* pIcon;	// �g���t�B�[�̃A�C�R��
	std::string sTitle;	// �g���t�B�[�̃^�C�g��
	std::string sText;	// �g���t�B�[�̕���
	Vector2 vPos;		// �ꏊ
	float iAlpha;		// �����x

	TROPHY_DATA();
};

class BaseTrophy
{
public:
	BaseTrophy();
	virtual ~BaseTrophy();

	virtual void Update();
	virtual void Render();
	virtual void Action(int iDelayFrame = 0);

	void RenderRoom(int iX, int iY);		//	�{���p

	// �A�N�Z�T
	TROPHY_DATA GetTrophy() { return m_tagTrophy; }// �g���t�B�[�̍\����

	// �{���p�̃|�W�V����
	Vector2 GetRoomPos() { return m_vRoomPos; };
	void SetRoomPos(Vector2 vPos) { m_vRoomPos = vPos; }

protected:
	bool m_bAction;
	int m_iActiveFrame;		//	�����Ă鎞��

	TROPHY_DATA m_tagTrophy;
	
	tdn2DObj* m_pPlate;
	tdn2DAnim* m_pIconRip;// �g���t�B�[�̃A�C�R���̔g��

	int m_iDelayFrame;

	Vector2 m_vRoomPos;		//	�{���p�̏ꏊ

};

//+---------------------------------
//	���v���C�̃g���t�B�[
//+---------------------------------

class FirstTrophy :public BaseTrophy
{
public:
	FirstTrophy();
	~FirstTrophy();

private:

};


//+---------------------------------
//	���߂đΐ�������̃g���t�B�[
//+---------------------------------

class FirstBattleTrophy :public BaseTrophy
{
public:
	FirstBattleTrophy();
	~FirstBattleTrophy();

private:

};



