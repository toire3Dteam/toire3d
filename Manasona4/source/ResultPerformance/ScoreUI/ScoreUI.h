#pragma once
#include "TDNLIB.h"
#include "Number\Number.h"
#include "Effect\PanelEffect\PanelEffectManager.h"
#include "Data\ResultData.h"
#include "Data\SelectData.h"

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

	RANK_TYPE GetRankType() { return m_tagRank.eType; }

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
		tdn2DAnim* pFont;		// �t�H���g�̃C���X�g
		Number* pNumber;		// ���l�\���̃C���X�g
		Number* pFlashNumber;	// �_�ŗp���l�\���̃C���X�g
		int x, y;				// �ꏊ
		int iAddNumX, iAddNumY;	// �����̏ꏊ
		int iPoint;				// �\�����链�_
		CloudEffect* pEffect;	// �o���G�t�F�N�g
	};
	SCORE m_tagScore[SCORE_TYPE::ARRAY_END];

	// �����N����
	struct RANK
	{
		tdn2DAnim* pRank;	// �C���X�g
		tdn2DAnim* pRankRip;	// �g��p
		BasePanelEffect* pLight;	// ���̉��o
		RANK_TYPE eType;			// �����N�̃^�C�v
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

