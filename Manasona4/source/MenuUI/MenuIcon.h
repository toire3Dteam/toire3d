#pragma once
#include "TDNLIB.h"


//+-------------------
//	�x�[�X���j���[�A�C�R��
//+-------------------

class BaseMenuIcon
{
public:
	BaseMenuIcon();
	virtual ~BaseMenuIcon();

	virtual void Update();
	virtual void Render(int addY = 0);
	//void RenderGray();
	virtual void Action(int delayFrame = 0);

	// �A�N�Z�T�[
	//Vector2 GetPrevPos() { return m_vPrevPos; }					// �ߋ��̍��W
	//void SetPrevPos(Vector2 prevPos) { m_vPrevPos = prevPos; }	// �ߋ��̍��W
	//Vector2 GetCurrPos() { return m_vCurrPos; }					// ���݂̍��W
	//void SetCurrPos(Vector2 currPos) { m_vCurrPos = currPos; }	// ���݂̍��W
	//void SetAllPos(Vector2 pos) { m_vCurrPos = pos; m_vPrevPos = pos; }
	//void SetMoveRate(float rate) { m_fMoveRate = rate; };
	Vector2 GetPos() { return m_vPos; }
	void SetPos(Vector2 pos) { m_vPos = pos; m_vNextPos = pos; }
	Vector2 GetNextPos() { return m_vNextPos; }
	void SetNextPos(Vector2 pos) { m_vNextPos = pos; }

	void SetSelectFlag(bool flag) { m_bSelectFlag = flag; };
	void SetScale(float scale) { m_fScale= scale; m_fNextScale = scale;};
	void SetNextScale(float scale) { m_fNextScale = scale; };

	tdn2DAnim* GetInfo() { return m_pInfo; }
protected:
	//tdn2DAnim* m_pFont;					// �����摜
	tdn2DAnim* m_pIcon;					// �A�C�R���摜
	float	   m_fScale;				// �摜�X�P�[��
	float	   m_fNextScale;			// �摜�̎��̃X�P�[��

	Vector2 m_vPos;						// ���݂̍��W
	Vector2 m_vNextPos;					// ���̍��W
	//Vector2 m_vPrevPos;					// �ߋ��̍��W
	//Vector2 m_vCurrPos;					// ���݂̍��W
	//float	m_fMoveRate;				// �ߋ��E���݂̈ړ��̕�Ԃ̒l
	//float	m_fMovePower;				// �ړ���

	bool m_bSelectFlag;					// �I�����ꂽ���̃t���O

	// ������
	tdn2DAnim* m_pInfo;
	Vector2 m_vInfoPos;
};


//+--------------------------
//	�`���[�g���A���A�C�R��
//+--------------------------

class TutorialIcon :public BaseMenuIcon
{
public:
	TutorialIcon();
	~TutorialIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	�o�g���A�C�R��
//+--------------------------

class BattleIcon :public BaseMenuIcon
{
public:
	BattleIcon();
	~BattleIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	�g���[�j���O�A�C�R��
//+--------------------------

class TrainingIcon :public BaseMenuIcon
{
public:
	TrainingIcon();
	~TrainingIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	�`�������W�A�C�R��
//+--------------------------

class ChallengeIcon :public BaseMenuIcon
{
public:
	ChallengeIcon();
	~ChallengeIcon();

	void Update();
	void Render();

protected:

};

//+--------------------------
//	�R���N�V�����A�C�R��
//+--------------------------
class CollectIcon :public BaseMenuIcon
{
public:
	CollectIcon();
	~CollectIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	�I�v�V�����A�C�R��
//+--------------------------
class OptionIcon :public BaseMenuIcon
{
public:
	OptionIcon();
	~OptionIcon();

	void Update();
	void Render();

protected:

};