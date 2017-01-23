#include "TutorialSelectParamSetting.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "MenuUI\TutorialManager.h"

//+----------------------------------------------
//	�`���[�g���A���Z���N�g�̃p�����[�^�[�ݒ�
//+----------------------------------------------

TutorialSelectParamSetting::TutorialSelectParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

TutorialSelectParamSetting::~TutorialSelectParamSetting()
{

}

//+----------------------------
//	�X�V�E�`��
//+----------------------------
void TutorialSelectParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ���e���l�X�ȕ`����@�ŕ`��@���[�U�[�ɕ�����₷�����
void TutorialSelectParamSetting::Render(int x, int y, bool selectFlag)
{
	//// ���ʕ`��i���Ƃ��j
	//BaseParamSetting::Render(x, y, selectFlag);
	// ���`��
	if (SelectDataMgr->Get()->iTutorialType != (int)TUTORIAL_TYPE::WALK)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// ��
	if (SelectDataMgr->Get()->iTutorialType != (int)TUTORIAL_TYPE::ARRAY_END - 1)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// �E


	// ����͕����\�L�ŕ`��
	// �����̐F
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	tdnFont::RenderStringCentering(TutorialMgr->GetTitleString(SelectDataMgr->Get()->iTutorialType).c_str(), "HG�޼��E", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	����
void TutorialSelectParamSetting::Ctrl(int DeviceID)
{
	// ��{����
	BaseParamSetting::Ctrl(DeviceID);

	// ������t���O���ݒ肳���̂ł���𗘗p�ɂ���
	// ��������I���W�i���̐ݒ��

	// �������Ă���
	if (m_bLeftPush)
	{
		if ((SelectDataMgr->Get()->iTutorialType > (int)TUTORIAL_TYPE::WALK))
		{
			//TutorialMgr->BackStep();
			SelectDataMgr->Get()->iTutorialType--;
		}
		else
		{
			//TutorialMgr->SetSelectType((TUTORIAL_TYPE)((int)TUTORIAL_TYPE::ARRAY_END - 1));			
			SelectDataMgr->Get()->iTutorialType = (int)TUTORIAL_TYPE::ARRAY_END - 1;
		}
			
	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		if ((SelectDataMgr->Get()->iTutorialType < (int)TUTORIAL_TYPE::ARRAY_END - 1))
		{
			//TutorialMgr->NectStep();
			SelectDataMgr->Get()->iTutorialType++;
		}
		else
		{
			//TutorialMgr->SetSelectType((TUTORIAL_TYPE)((int)TUTORIAL_TYPE::WALK));
			SelectDataMgr->Get()->iTutorialType = 0;
		}

	}
}
