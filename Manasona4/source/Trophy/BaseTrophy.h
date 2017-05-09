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
	virtual void Stop();

	void RenderRoom(int iX, int iY/*, bool bRock*/);		//	�{���p
	void RenderInfo(int iX, int iY);					// �g���t�B�[�̃^�C�g���Ɛ���
	
	// �A�N�Z�T
	TROPHY_DATA GetTrophy() { return m_tagTrophy; }// �g���t�B�[�̍\����

	// �{���p�̃|�W�V����
	Vector2 GetRoomPos() { return m_vRoomPos; };
	void SetRoomPos(Vector2 vPos) { m_vRoomPos = vPos; }

	bool isHide() { return m_bHide; }

protected:
	bool m_bAction;
	int m_iActiveFrame;		//	�����Ă鎞��

	TROPHY_DATA m_tagTrophy;
	
	tdn2DObj* m_pPlate;
	tdn2DAnim* m_pIconRip;// �g���t�B�[�̃A�C�R���̔g��
	int m_iDelayFrame;
	Vector2 m_vRoomPos;		//	�{���p�̏ꏊ
	
	// ���b�N�֘A
	// bool m_bRock;
	// tdn2DObj* m_pRockIcon;

	bool m_bHide;			// �B���g���t�B�[

};

//+---------------------------------
//	���v���C�̃g���t�B�[
//+---------------------------------
class FirstTrophy :public BaseTrophy
{
public:
	FirstTrophy();
	~FirstTrophy() {};

private:

};


//+---------------------------------
//	���߂đΐ�������̃g���t�B�[
//+---------------------------------
class FirstBattleTrophy :public BaseTrophy
{
public:
	FirstBattleTrophy();
	~FirstBattleTrophy() {};

private:

};


//+---------------------------------
//	��_���[�W�g���t�B�[
//+---------------------------------
class BigDamageTrophy :public BaseTrophy
{
public:
	BigDamageTrophy();
	~BigDamageTrophy() {};

private:

};

//+---------------------------------
//	MAXHP�����g���t�B�[
//+---------------------------------
class MaxHpFinishTrophy :public BaseTrophy
{
public:
	MaxHpFinishTrophy();
	~MaxHpFinishTrophy() {};

private:

};


//+---------------------------------
//	�ő������g���t�B�[
//+---------------------------------
class SpeedFinishTrophy :public BaseTrophy
{
public:
	SpeedFinishTrophy();
	~SpeedFinishTrophy() {};

private:

};

//+---------------------------------
//	�g���[�j���O�ł̘A�����ԃg���t�B�[
//+---------------------------------
class TrainingTimeTrophy :public BaseTrophy
{
public:
	TrainingTimeTrophy();
	~TrainingTimeTrophy() {};

private:

};

//+---------------------------------
//	�ō���Փx��COM��|�����g���t�B�[
//+---------------------------------
class PowerfulEnemyWinTrophy :public BaseTrophy
{
public:
	PowerfulEnemyWinTrophy();
	~PowerfulEnemyWinTrophy() {};

private:

};

//+--------------------------------------------
//	SS�����N�œ|�����g���t�B�[
//+--------------------------------------------
class SSRankWinTrophy :public BaseTrophy
{
public:
	SSRankWinTrophy();
	~SSRankWinTrophy() {};

private:

};

//+--------------------------------------------
//	�ō���Փx��COM��|�����{SS�����N�œ|�����g���t�B�[
//+--------------------------------------------
class PowerfulEnemySSRankWinTrophy :public BaseTrophy
{
public:
	PowerfulEnemySSRankWinTrophy();
	~PowerfulEnemySSRankWinTrophy() {};

private:

};

//+-----------------------------------------
//	�R���e���c�����������������g���t�B�[
//+-----------------------------------------
class BuyManyContentTrophy :public BaseTrophy
{
public:
	BuyManyContentTrophy();
	~BuyManyContentTrophy() {};

private:

};

//+---------------------------------
//	�`�������W���[�h�Ń^�X�N���N���A
//+---------------------------------
class ChallengeTrophy :public BaseTrophy
{
public:
	ChallengeTrophy();
	~ChallengeTrophy() {};

private:

};

//+---------------------------------
//	�A�C���\�`�������W�g���t�B�[
//+---------------------------------
class AirouChallengeTrophy :public BaseTrophy
{
public:
	AirouChallengeTrophy();
	~AirouChallengeTrophy() {};

private:

};

//+---------------------------------
//	�e�L�`�������W�g���t�B�[
//+---------------------------------
class TekiChallengeTrophy :public BaseTrophy
{
public:
	TekiChallengeTrophy();
	~TekiChallengeTrophy() {};

private:

};
//+---------------------------------
//	���_���[�W�g���t�B�[
//+---------------------------------
class BigBigDamageTrophy :public BaseTrophy
{
public:
	BigBigDamageTrophy();
	~BigBigDamageTrophy() {};

private:

};




//+---------------------------------
//	�R���v���[�g�g���t�B�[
//+---------------------------------
class CompleteTrophy :public BaseTrophy
{
public:
	CompleteTrophy();
	~CompleteTrophy() {};

private:

};
