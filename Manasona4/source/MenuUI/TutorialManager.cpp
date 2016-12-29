#include "TutorialManager.h"

//+----------------------------
//	�`���[�g���A���}�l�[�W���[
//+----------------------------

// ���̂̐錾
TutorialManager* TutorialManager::m_pInstance = nullptr;

TutorialManager::TutorialManager()
{
	// �S�Ẵ`���[�g���A����������
	for (int i = 0; i < (int)TUTORIAL_TYPE::ARRAY_END; i++)
	{
		int no = i + 1;
		
		switch ((TUTORIAL_TYPE)i)
		{
		case TUTORIAL_TYPE::WALK:
			m_pTutorial[i] = new WalkTutorial(no);// 
			break;
		case TUTORIAL_TYPE::SQUAT:
			m_pTutorial[i] = new SquatTutorial(no);// 
			break;
		case TUTORIAL_TYPE::JUMP:
			m_pTutorial[i] = new JumpTutorial(no);// 
			break;
		case TUTORIAL_TYPE::ATTACK:
			m_pTutorial[i] = new AttackTutorial(no);// 
			break;
		case TUTORIAL_TYPE::SKILL:
			m_pTutorial[i] = new SkillTutorial(no);// 
			break;
		case TUTORIAL_TYPE::OVER_DRIVE:
			m_pTutorial[i] = new OverDriveTutorial(no);// 
			break;
		case TUTORIAL_TYPE::RUSH:
			m_pTutorial[i] = new RushTutorial(no);// 
			break;
		case TUTORIAL_TYPE::STAND_GUARD:
			m_pTutorial[i] = new StandGuardTutorial(no);// 
			break;
		case TUTORIAL_TYPE::SQUAT_GUARD:
			m_pTutorial[i] = new SquatGuardTutorial(no);// 
			break;
		case TUTORIAL_TYPE::DUSH:
			m_pTutorial[i] = new DushTutorial(no);// 
			break;
		case TUTORIAL_TYPE::BACK_STEP:
			m_pTutorial[i] = new BackStepTutorial(no);// 
			break;
		case TUTORIAL_TYPE::PARTNER:
			m_pTutorial[i] = new PartnerTutorial(no);// 
			break;
		case TUTORIAL_TYPE::INVINCIBLE:
			m_pTutorial[i] = new InvincibleTutorial(no);// 
			break;
		case TUTORIAL_TYPE::THROW:
			m_pTutorial[i] = new ThrowTutorial(no);// 
			break;
		case TUTORIAL_TYPE::ESCAPE:
			m_pTutorial[i] = new EscapeTutorial(no);// 
			break;
		case TUTORIAL_TYPE::DOKKOI:
			m_pTutorial[i] = new DokkoiTutorial(no);// 
			break;
		case TUTORIAL_TYPE::DOWN_ATTACK:
			m_pTutorial[i] = new DownAttackTutorial(no);// 
			break;
		case TUTORIAL_TYPE::OD_ONEMORE:
			m_pTutorial[i] = new OverDriveOneMoreTutorial(no);// 
			break;
		case TUTORIAL_TYPE::OD_BURST:
			m_pTutorial[i] = new OverDriveBurstTutorial(no);// 
			break;
		default:
			MyAssert(0, "���̃^�C�v�͑��݂��Ȃ�");
			break;
		}

	}

	// �����ݒ�
	m_eType = TUTORIAL_TYPE::WALK;

	// ���܂��ł̖Z�����l�̂��߂̃`���[�g���A����������
	for (int i = 0; i < (int)TUTORIAL_BUSY_TYPE::ARRAY_END; i++)
	{
		int no = i + 1;

		switch ((TUTORIAL_BUSY_TYPE)i)
		{
		case TUTORIAL_BUSY_TYPE::WALK:
			m_pBusyTutorial[i] = new WalkTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::SQUAT:
			m_pBusyTutorial[i] = new SquatTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::JUMP:
			m_pBusyTutorial[i] = new JumpTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::ATTACK:
			m_pBusyTutorial[i] = new AttackTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::SKILL:
			m_pBusyTutorial[i] = new SkillTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::OVER_DRIVE:
			m_pBusyTutorial[i] = new OverDriveTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::RUSH:
			m_pBusyTutorial[i] = new RushTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::STAND_GUARD:
			m_pBusyTutorial[i] = new StandGuardTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::SQUAT_GUARD:
			m_pBusyTutorial[i] = new SquatGuardTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::DUSH:
			m_pBusyTutorial[i] = new DushTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::BACK_STEP:
			m_pBusyTutorial[i] = new BackStepTutorial(no);// 
			break;
		case TUTORIAL_BUSY_TYPE::ARRAY_END:
			MyAssert(0, "���̃^�C�v�͑��݂��Ȃ�");
			break;
		default:
			break;
		}

	}

	m_bBusy = false;
	m_eBusyType = TUTORIAL_BUSY_TYPE::WALK;
}

TutorialManager::~TutorialManager()
{
	// �S�Ẵ`���[�g���A�������
	for (int i = 0; i < (int)TUTORIAL_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTutorial[i]);
	}

	// �S�Ă̖Z�����l�̂��߂̃`���[�g���A�������
	for (int i = 0; i < (int)TUTORIAL_BUSY_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pBusyTutorial[i]);
	}
}

// ������
void TutorialManager::Init(TUTORIAL_TYPE type, int DeviceID)
{
	m_eType = type;// �`���[�g���A���̃^�C�v�ύX

	m_pTutorial[(int)m_eType]->Init(DeviceID);// ���삷��f�o�C�X�ݒ�

}

// �X�V
void TutorialManager::Update()
{
	// �`���[�g���A���X�V
	m_pTutorial[(int)m_eType]->Update();

}

// �^�X�N�p�X�V
void TutorialManager::TaskUpdate(BasePlayer * pPerson)
{
	// �^�X�N�p�X�V
	m_pTutorial[(int)m_eType]->TaskUpdate(pPerson);

}

// �`��
void TutorialManager::Render()
{
	// �`���[�g���A���`��
	m_pTutorial[(int)m_eType]->Render();
}



// ������
void TutorialManager::InitBusy(TUTORIAL_BUSY_TYPE type, int DeviceID)
{
	m_eBusyType = type;// �`���[�g���A���̃^�C�v�ύX

	m_pBusyTutorial[(int)m_eBusyType]->Init(DeviceID);// ���삷��f�o�C�X�ݒ�

}

// �X�V
void TutorialManager::UpdateBusy()
{
	// �`���[�g���A���X�V
	m_pBusyTutorial[(int)m_eBusyType]->Update();

}

// �^�X�N�p�X�V
void TutorialManager::TaskUpdateBusy(BasePlayer * pPerson)
{
	// �^�X�N�p�X�V
	m_pBusyTutorial[(int)m_eBusyType]->TaskUpdate(pPerson);

}

// �`��
void TutorialManager::RenderBusy()
{
	// �`���[�g���A���`��
	m_pBusyTutorial[(int)m_eBusyType]->Render();
}
