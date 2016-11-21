#pragma once
#include "TDNLIB.h"

//+---------------------
//	�q���g�J�[�h�N���X
//+---------------------

// �T�v
// �F�X���b�Z�\�W�Ƃ��Ă��g��
// �����@���@��������Ƃ��ɏ�������


class TipsCard
{
public:

	// ����
	enum STEP
	{
		INTRO,	// �n��
		EXECUTE,// ���s��
		END		// �I��
	};

	// �I����
	enum SELECT_STATE
	{
		OK,			// OK
		CANCEL,		// �L�����Z��
		HOLD		// �I��
	};

	TipsCard(LPSTR string, bool bChoice = false, bool bOK = false);
	~TipsCard();

	void Update(int DeviceID = 0);
	void Render();

	void Action();
	void End();

	STEP GetStep() { return m_eStep; }
	SELECT_STATE GetSelectState() { return m_eSelectState; }

private:
	LPSTR m_pString;
	tdn2DAnim* m_pCardPic;
	tdn2DAnim* m_pChoiceOKPic;
	tdn2DAnim* m_pChoiceCancelPic;
	Vector2 m_vCardPos;

	bool m_bRender;
	int m_iAlpha;
	int m_iWaitTimer;
	bool m_bChoice;	//	�I�������ǂ���
	bool m_bOK;		//  OK���ǂ���

	STEP m_eStep;
	SELECT_STATE m_eSelectState;

};

