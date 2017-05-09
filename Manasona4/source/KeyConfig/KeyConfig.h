#pragma once
#include "TDNLIB.h"

//+--------------------------------------------
//	�L�[�R���t�B�O
//+--------------------------------------------

class KeyConfig
{
public:
	KeyConfig();
	~KeyConfig();

	bool Update();
	void Render();

	bool Ctrl();

	void Action(Vector2 vPos, int DeviceID);
	void Stop();

	// �A�N�Z�T
	bool IsActive() { return m_bActive; };
	int  GetChoiceState() { return m_iChoiceState; }//�@�����̃Q�b�^�[�ŗl�X�Ȕ��������

protected:
	static const int SizeX = 432;

	int m_iDeviceID;
	KEY_CONFIG_DATA m_tagConfigData, m_tagOrgConfigData;

	// �E�B���h�E�̃f�U�C��
	tdn2DObj* m_pWindow;	// �S��
	tdn2DAnim* m_pSelect;	// �I�𒆂̃A�C�R��

	// �E�B���h�E�̏ꏊ
	Vector2 m_vPos;
	// ���̃E�B���h�E���N�����Ă��邩�ǂ���
	bool m_bActive;
	// �I�����Ă�ꏊ
	int m_iSelectNo;


	
	// �I�����
	enum CHOICE_STATE
	{
		HOLD = -1
	};

	enum KEY_CONFIG_STATE
	{
		ATTACK,			// �U��
		SKILL,			// �X�L��
		THROW,			// ����
		ESCAPE,			// ���
		HEAVEHO_DRIVE,	// �K�E�Z
		INVINCIBLE,		// ���G
		PARTNER,		// �p�[�g�i�[
		DOKKOI,			// ���i
		OVER_DRIVE,		// �o�[�X�g
		START,
		SELECT,

		BACK,			// �߂�
		ARRAY_END		// �z��̏I��
	};

	int m_iChoiceState;


	
	// �E�B���h�E�̒��̐�
	struct IconData
	{
		LPSTR pString;// ����
		int iStringLength;// ������
		//LPSTR pInfoString;// �����p����
	};
	std::vector<IconData> m_aIconData;


	// ���߂�h�~
	bool m_bBackPush;

	// (1/28) �ً}����
	// ����PC�ŋN�������Ƃ�MENU��ʂ��`�悪
	// �A���`�G�C���A�V���O�̌��ʂȂǂŌ����ڂ������ς����
	// �ЂƂ܂��͕�Ԃ��l�����Ē���
	int m_iAbjustHeight;

	int m_iSelectLength;	//	�I�𒆂̃A�C�R���̒���

protected:
	void AddIconData(LPSTR string);

	void SetConfig(KEYCODE PushedKey);
	BYTE *GetKeyConfigButton(KEY_CONFIG_STATE eState);
};
