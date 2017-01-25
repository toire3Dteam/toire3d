#include "BaseChallenge.h"
#include "Sound\SoundManager.h"

//+-----------------------------
//	�x�[�X
//+-----------------------------

BaseChallenge::BaseChallenge(int iTitleNo)
{
	m_iTitleNo = iTitleNo;
	// �^�C�g����
	m_pTaskTitle.pString = "No.";

	// 10���ȉ��Ȃ�
	if (m_iTitleNo < 10)
	{
		m_pTaskTitle.pString += "0";// 0�ǉ�
	}
	m_pTaskTitle.pString += std::to_string(m_iTitleNo) + " ";


	m_pTaskTitle.bClear = false;

	m_iWaitFrame = 0;
	m_iWaitFrameMAX = 0;
	m_bClearFlag = false;

	m_pTaskIcon = new tdn2DObj("Data/UI/Tutorial/TaskIcon.png");
	m_bAllTaskClear = false;

	m_pIntroTips = nullptr;
	m_pClearTips = nullptr;;

	// �^�X�N�ꗗ
	m_aTask.clear();
	m_vTaskPos.x = 50;
	m_vTaskPos.y = 250;



	// �Z���N�g�i���o�[
	m_iSelectTask = 0;

	m_pClearPic = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPic->OrderAlphaMove(55, 6, 45);

	m_pClearPicRip = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPicRip->OrderRipple(14, 1, 0.025f);

	m_iDeviceID = 0;

	m_bReplay = false;

	m_strReplayDataName = "Data/ChallengeData/�Ȃ�.bin";
}

BaseChallenge::~BaseChallenge()
{
	SAFE_DELETE(m_pTaskIcon);
	SAFE_DELETE(m_pIntroTips);
	SAFE_DELETE(m_pClearTips);
	SAFE_DELETE(m_pClearPic);
	SAFE_DELETE(m_pClearPicRip);

	m_aTask.clear();
}

void BaseChallenge::Init(int iDeviceID)
{

	m_iWaitFrame = m_iWaitFrameMAX;
	m_bClearFlag = false;


	// �N���A�t���O����������
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		m_aTask[i].bClear = false;
	}

	m_iSelectTask = 0;

	m_iDeviceID = iDeviceID;

	// �O��̉��o������
	m_pClearPic->Stop();
	m_pClearPicRip->Stop();

	// �����̏󋵐ݒ�
	// �K�v�ȏ󋵂�����Ȃ�e������������
	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::MIDDLE;
	SelectDataMgr->Get()->tagTrainingDatas.eHpRecovery = HP_RECOVERY_TYPE::AUTO_RECOVERY;
	SelectDataMgr->Get()->tagTrainingDatas.iHp1P = 100;
	SelectDataMgr->Get()->tagTrainingDatas.iHp2P = 100;
	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 0;
	SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery = PARTNER_RECOVERY_TYPE::DEFAULT;
	SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo = TRAINING_INFO_TYPE::NONE;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::STAND;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard = ENEMY_GUARD_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch = ENEMY_GUARD_SWITCH_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech = ENEMY_TECH_TYPE::ALL;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech = ENEMY_THROW_TECH_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv = 20;
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::NO;
}

void BaseChallenge::Update()
{
	// �N���A���
	if (m_bClearFlag == true)
	{
		// �J�E���g����
		m_iWaitFrame--;
		if (m_iWaitFrame <= 0)
		{
			// ���̎��_�� EndTipsStart()�֐��ŏ󋵂�؂�ւ���
			m_iWaitFrame = 0;
		}
	}
	else
	{
		// �^�X�N���S���N���A����Ă邩�`�F�b�N�I
		int iClearNum = 0;
		for (int i = 0; i < (int)m_aTask.size(); i++)
		{
			if (m_aTask[i].bClear == true)
			{
				iClearNum++;
			}

			// �����N���A�����^�X�N���Ɠ����ɂȂ�����
			if ((int)m_aTask.size() == iClearNum)
			{
				// ���b�X���N���A�I
				ActionClear();
			}
		}
	}



	// Tips
	m_pIntroTips->Update(m_iDeviceID);
	m_pClearTips->Update(m_iDeviceID);

	// �N���A���o�p
	m_pClearPic->Update();
	m_pClearPicRip->Update();

}

void BaseChallenge::Render()
{

	// �^�X�N�^�C�g��
	for (int j = 0; j < m_pTaskTitle.iStingLength + 3/*����蒷��*/; j++)
		m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8 - 16, (int)m_vTaskPos.y - 40 - 4, 32, 32, 0, 0, 32, 32);
	tdnFont::RenderString(m_pTaskTitle.pString.c_str(), "HG�޼��E", 22, (int)m_vTaskPos.x + 16, (int)m_vTaskPos.y - 40, 0xffffffff, RS::COPY);


	// �^�X�N��
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		for (int j = 0; j < m_aTask[i].iStingLength; j++)
		{
			m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4, 32, 32, 32 * m_aTask[i].bClear, 0, 32, 32);

			// �I�����Ă���^�X�N�Ȃ�
			if (m_iSelectTask == i)
			{
				m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4,
					32, 32, 0, 32, 32, 32);
			}

		}

		DWORD fontCol = 0xffffffff;
		if (m_iSelectTask == i)fontCol = 0xff222222;
		tdnFont::RenderString(m_aTask[i].pString.c_str(), "HG�޼��E", 22, (int)m_vTaskPos.x, (int)m_vTaskPos.y + (i * 40), fontCol, RS::COPY);
	}

	// TIPS
	m_pIntroTips->Render();
	m_pClearTips->Render();

	// �N���A���o�p
	m_pClearPic->Render(0, 0);
	m_pClearPicRip->Render(0, 0, RS::ADD);
}

void BaseChallenge::ActionIntroTips()
{
	//�A�N�V�����I���͉摜�o������
	m_pIntroTips->Action();

}
void BaseChallenge::StopIntroTips()
{
	m_pIntroTips->End();
}

void BaseChallenge::ActionClearTips()
{
	//�A�N�V�����I���͉摜�o������
	m_pClearTips->Action();

}
void BaseChallenge::StopClearTips()
{
	m_pClearTips->End();
}

void BaseChallenge::ActionClear()
{
	// ���v���C�Ȃ�N���A�����Ȃ�
	if (m_bReplay == false)
	{

	m_bClearFlag = true;	

	// ���ŃN���A���o
	m_pClearPic->Action();
	m_pClearPicRip->Action();

	// SE
	se->Play("�I�[���N���A");

	}
}


//void BaseChallenge::TaskReset()
//{
//	// �N���A�t���O����������
//	for (int i = 0; i < (int)m_aTask.size(); i++)
//	{
//		m_aTask[i].bClear = false;
//	}
//
//	m_iSelectTask = 0;
//
//}


void BaseChallenge::AddTaskData(LPSTR string)
{
	Task data;
	data.pString = string;
	data.bClear = false;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0;
	UINT	addByte = 0;
	//	�I�[�����܂Ń��[�v
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&string[i]);
		addByte += myByte;
	}
	data.iStingLength = addByte;

	m_aTask.push_back(data);

}

void BaseChallenge::TaskSuccess(int select)
{
	// ��������v���Ă�����̂̂ݔ��� ����ɂ�菇�Ԓʂ�^�X�N�����Ȃ��K�v������
	if (select == m_iSelectTask)
	{

		if ((int)m_aTask.size() == m_iSelectTask)
		{
			assert(0);// �T�C�Y�����߂�
		}

		m_aTask[m_iSelectTask].bClear = true;
		m_iSelectTask++;

		// SE
		se->Play("�X�e�b�v�N���A");
	}
}
