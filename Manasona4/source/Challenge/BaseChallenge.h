#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "MenuUI\TipsCard.h"

//+-------------------------------------
// �x�[�X�`�������W
//+-------------------------------------

class BaseChallenge
{
public:
	BaseChallenge(int iTitleNo);
	virtual ~BaseChallenge();

	virtual void Init(int iDeviceID = 0) = 0;// �ŏ��ɖ��񏉊���
	virtual void Update();
	virtual void Render();

	// �����͋��ʂȂ̂�Virture�͂���Ȃ�
	void ActionIntroTips();
	void StopIntroTips();
	void ActionClearTips();
	void StopClearTips();

	void ActionClear();// �N���A�I���o��

					   // ����ōŌ��Tips�������^�C�~���O���擾
	bool EndTipsStart() { return (m_iWaitFrame <= 0) ? true : false; }

	//
	bool isClear() { return m_bClearFlag; }

	TipsCard* GetIntroTipsCard() { return m_pIntroTips; }
	TipsCard* GetClearTipsCard() { return m_pClearTips; }

	// ���ꂼ��̂�邱��
	virtual void TaskUpdate(BasePlayer* pPerson) = 0; //�������bClear��_�������Ă���
													  //virtual void TaskReset() = 0; // �~�X�������ɔ���

	void AddTaskData(LPSTR string);

	// �����I
	void TaskSuccess(int select);

	void titleTaskClear() { m_pTaskTitle.bClear = true; };

	// �^�C�g����
	std::string GetTitleString() { return m_pTaskTitle.pString; }

protected:
	int m_iWaitFrame;// �N���A����ǂꂭ�炢�ŃG���hTips�ֈڍs���邩
	int m_iWaitFrameMAX;// �ő�E�F�C�g��
	bool m_bClearFlag;	//	

	int m_iDeviceID;   // ���삵�Ă���f�o�C�X

					   // �ŏ���Tips�E�Ō��Tips 
	TipsCard* m_pIntroTips;
	TipsCard* m_pClearTips;

	// �^�X�N�f�[�^
	struct Task
	{
		std::string pString;	// ����
		bool bClear;	// �N���A������
		int iStingLength;	//�@������ 
	};
	std::vector<Task> m_aTask;

	// �^�X�N�^�C�g����
	Task m_pTaskTitle;// �^�C�g���̉摜

	tdn2DObj* m_pTaskIcon;
	Vector2 m_vTaskPos;

	int m_iSelectTask;

	bool m_bAllTaskClear;

	int m_iTitleNo;//

	tdn2DAnim* m_pClearPic;
	tdn2DAnim* m_pClearPicRip;
};
