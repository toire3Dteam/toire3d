#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"
#include "ParamSetting\Challenge\ChallengeSelectParamSetting.h"

//+-------------------------------
// �`�������W�Z���N�g�E�B���h�E
//+-------------------------------


class ChallengeSelectWindow : public BaseWindow
{
public:
	ChallengeSelectWindow(Vector2 vPos);
	~ChallengeSelectWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);

	// �I�����
	enum CHALLENGE_SELECT_STATE
	{
		SELECT_CHALLENGE,
		BACK
	};


private:
	static const int SizeX = 704;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	// �����̃v���[�g
	//InformationPlate* m_pInfoPlate;

	ChallengeSelectParamSetting* m_ChallengeSelectParam;

	// ���߂�h�~
	//bool m_bBackPush;
};
