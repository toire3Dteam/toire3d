#pragma once
#include "TDNLIB.h"
#include "Number\Number.h"

//+-----------------------
//	��{�̃p�����[�^�[�ݒ�UI
//+-----------------------


class BaseParamSetting
{
public:
	BaseParamSetting(int Number, int WidthSize);
	virtual ~BaseParamSetting();

	virtual void Update();
	virtual void Render(int x,int y,bool selectFlag);
	void RenderNumber(int x, int y, int num, bool bSelect = false);// �����`��
	virtual void Ctrl(int DeviceID);

	// �A�N�Z�T
	int GetNumberID() { return m_iNumberID; };
	int GetWidthSize() { return m_iWidthSize; };



protected:
	// �ԍ��E����
	int m_iNumberID;
	int m_iWidthSize;

	// �f��
	tdn2DAnim* m_pLeftArrow;
	tdn2DAnim* m_pRightArrow;

	tdn2DObj* m_pPersentFrame;
	tdn2DObj* m_pPersentGage;

	tdn2DObj* m_pNumber;

	// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b
	// ���Ċ����̐G��S�n��
	int m_iSpeedLv;
	int m_iLeftPushFrame;
	int m_iRightPushFrame;
	bool m_bLeftPush;		// ��������
	bool m_bRightPush;		// �E������
};
