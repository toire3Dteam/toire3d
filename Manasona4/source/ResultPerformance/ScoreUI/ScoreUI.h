#pragma once
#include "TDNLIB.h"
#include "Number\Number.h"
#include "Effect\PanelEffect\PanelEffectManager.h"
#include "Data\ResultData.h"

//+--------------------------
//	�X�R�A�\��UI
//+--------------------------

class ScoreUI
{
public:
	ScoreUI(ResultData data);
	~ScoreUI();

	void Update();
	void Render();

	void Action();

	bool GetEnd() { return m_bEnd; };

private:

	// �X�R�A�̎��
	enum SCORE_TYPE
	{
		DAMAGE,
		HP,
		TIME,
		ARRAY_END
	};
	// ���_
	struct SCORE
	{
		tdn2DAnim* pFont;	// �t�H���g�̃C���X�g
		Number* pNumber;	// ���l�\���̃C���X�g
		int x, y;			// �ꏊ
		int iAddNumX;		// �����̏ꏊ
		int iPoint;			// �\�����链�_
	};
	SCORE m_tagScore[SCORE_TYPE::ARRAY_END];

	// �����N����
	enum RANK_TYPE
	{
		SS, S, A, B, C
	};
	struct RANK
	{
		tdn2DAnim* pRank;	// �C���X�g
		tdn2DAnim* pRankRip;	// �g��p
		BasePanelEffect* pLight;	// ���̉��o
		RANK_TYPE iType;			// �����N�̃^�C�v
		int x, y;			// �ꏊ
	};
	RANK m_tagRank;

	// ���o�����t���O
	bool m_bEnd;
	bool m_bActive;
	int m_iFrame;

private:

	int CalcRankScore();	// �����N�v�Z
	int CalcCoin();	// ����

};

