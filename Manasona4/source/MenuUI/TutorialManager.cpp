#include "TutorialManager.h"

//+----------------------------
//	チュートリアルマネージャー
//+----------------------------

// 実体の宣言
TutorialManager* TutorialManager::m_pInstance = nullptr;

TutorialManager::TutorialManager()
{
	// 全てのチュートリアルを初期化
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
			MyAssert(0, "そのタイプは存在しない");
			break;
		}

	}

	// 初期設定
	m_eType = TUTORIAL_TYPE::WALK;

	// おまけでの忙しい人のためのチュートリアルを初期化
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
			MyAssert(0, "そのタイプは存在しない");
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
	// 全てのチュートリアルを解放
	for (int i = 0; i < (int)TUTORIAL_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTutorial[i]);
	}

	// 全ての忙しい人のためのチュートリアルを解放
	for (int i = 0; i < (int)TUTORIAL_BUSY_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pBusyTutorial[i]);
	}
}

// 初期化
void TutorialManager::Init(TUTORIAL_TYPE type, int DeviceID)
{
	m_eType = type;// チュートリアルのタイプ変更

	m_pTutorial[(int)m_eType]->Init(DeviceID);// 操作するデバイス設定

}

// 更新
void TutorialManager::Update()
{
	// チュートリアル更新
	m_pTutorial[(int)m_eType]->Update();

}

// タスク用更新
void TutorialManager::TaskUpdate(BasePlayer * pPerson)
{
	// タスク用更新
	m_pTutorial[(int)m_eType]->TaskUpdate(pPerson);

}

// 描画
void TutorialManager::Render()
{
	// チュートリアル描画
	m_pTutorial[(int)m_eType]->Render();
}



// 初期化
void TutorialManager::InitBusy(TUTORIAL_BUSY_TYPE type, int DeviceID)
{
	m_eBusyType = type;// チュートリアルのタイプ変更

	m_pBusyTutorial[(int)m_eBusyType]->Init(DeviceID);// 操作するデバイス設定

}

// 更新
void TutorialManager::UpdateBusy()
{
	// チュートリアル更新
	m_pBusyTutorial[(int)m_eBusyType]->Update();

}

// タスク用更新
void TutorialManager::TaskUpdateBusy(BasePlayer * pPerson)
{
	// タスク用更新
	m_pBusyTutorial[(int)m_eBusyType]->TaskUpdate(pPerson);

}

// 描画
void TutorialManager::RenderBusy()
{
	// チュートリアル描画
	m_pBusyTutorial[(int)m_eBusyType]->Render();
}
