#include "ChallengeSelectParamSetting.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "Challenge\ChallengeManagerManager.h"

//+----------------------------------------------
//	�`�������W�Z���N�g�̃p�����[�^�[�ݒ�
//+----------------------------------------------

ChallengeSelectParamSetting::ChallengeSelectParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

ChallengeSelectParamSetting::~ChallengeSelectParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void ChallengeSelectParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void ChallengeSelectParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (SelectDataMgr->Get()->iChallengeType != (int)CHALLENGE_TYPE::MISSION_1)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (SelectDataMgr->Get()->iChallengeType != (int)CHALLENGE_TYPE::ARRAY_END - 1)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��
	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	tdnFont::RenderStringCentering(ChallengeMgrMgr->GetSelectMgr()->GetTitleString(SelectDataMgr->Get()->iChallengeType).c_str(), "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void ChallengeSelectParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		if ((SelectDataMgr->Get()->iChallengeType > (int)CHALLENGE_TYPE::MISSION_1))
		{
			//TutorialMgr->BackStep();
			SelectDataMgr->Get()->iChallengeType--;
		}
		else
		{
			//TutorialMgr->SetSelectType((CHALLENGE_TYPE)((int)CHALLENGE_TYPE::ARRAY_END - 1));			
			SelectDataMgr->Get()->iChallengeType = (int)CHALLENGE_TYPE::ARRAY_END - 1;
		}
			
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		if ((SelectDataMgr->Get()->iChallengeType < (int)CHALLENGE_TYPE::ARRAY_END - 1))
		{
			//TutorialMgr->NectStep();
			SelectDataMgr->Get()->iChallengeType++;
		}
		else
		{
			//TutorialMgr->SetSelectType((CHALLENGE_TYPE)((int)CHALLENGE_TYPE::WALK));
			SelectDataMgr->Get()->iChallengeType = 0;
		}

	}
}
