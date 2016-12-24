#pragma once
#include "TDNLIB.h"

//+------------------------
// �E�B���h�E��ʂ̃x�[�X
//+------------------------

class BaseWindow
{
public:
	BaseWindow(Vector2 vPos);
	virtual ~BaseWindow();

	virtual bool Update() = 0;	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	virtual void Redner() = 0;	// �`��
	virtual void RednerInfo();	// ���ɕ\�L���镶���`��

	virtual bool Ctrl(int DeviceID) = 0; // �R���g���[��
	virtual void CtrlSE(int DeviceID);	 // ���쉹

	virtual void Action();	// Window���o��
	virtual void Stop();	// �~�߂�
	virtual void ChoiceClear();	// �I�����Ă������̂𖢑I����

	// �A�N�Z�T
	bool IsActive() { return m_bActive; };// ��
	int GetChoiceState() { return m_iChoiceState; }//�@�����̃Q�b�^�[�ŗl�X�Ȕ��������

protected:
	// �E�B���h�E�̏ꏊ
	Vector2 m_vPos;

	// �E�B���h�E�̒��̐�
	struct IconData
	{
		LPSTR pString;// ����
		int iStringLength;// ������
		LPSTR pInfoString;// �����p����
	};
	std::vector<IconData> m_aIconData;

	// �E�B���h�E�̃f�U�C��
	tdn2DObj* m_pWindow;	// �S��
	tdn2DAnim* m_pTop;		// �^�C�g��
	tdn2DAnim* m_pMiddle;	// ��
	tdn2DAnim* m_pBottom;	// ������

	tdn2DAnim* m_pSelect;	//	�I�𒆂̃A�C�R��
	int m_iSelectLength;	//	�I�𒆂̃A�C�R���̒���
	tdn2DObj* m_pFontBox;	// �������͂ށ�

	// ���̃E�B���h�E���N�����Ă��邩�ǂ���
	bool m_bActive;
	// �I�����Ă�ꏊ
	int m_iSelectNo;

	// �I�����
	enum CHOICE_STATE
	{
		HOLD = -1
	};

	int m_iChoiceState;

	// ���ʂ��鉉�o�p�̃p�����[�^
	// ����Window�ƈႤ���Ƃ�肽��������q�̕��ō���Ă�������
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

protected:
	void AddIconData(LPSTR string, LPSTR infoString);


};
