#include "BaseChallengeManager.h"
#include "AirouChallenge.h"
#include "TekiChallenge.h"
#include "NazenarabaChallenge.h"
#include "AramitamaChallenge.h"

//+----------------------------
//	ベースチャレンジマネージャー
//+----------------------------

BaseChallengeManager::BaseChallengeManager()
{
	// 全てのチュートリアルを初期化
	//for (int i = 0; i < (int)CHALLENGE_TYPE::ARRAY_END; i++)
	//{
	//	int no = i + 1;
	//	
	//	switch ((CHALLENGE_TYPE)i)
	//	{
	//	case CHALLENGE_TYPE::WALK:
	//		m_pChallenge[i] = new WalkChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::SQUAT:
	//		m_pChallenge[i] = new SquatChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::JUMP:
	//		m_pChallenge[i] = new JumpChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::ATTACK:
	//		m_pChallenge[i] = new AttackChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::SKILL:
	//		m_pChallenge[i] = new SkillChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::OVER_DRIVE:
	//		m_pChallenge[i] = new OverDriveChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::RUSH:
	//		m_pChallenge[i] = new RushChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::STAND_GUARD:
	//		m_pChallenge[i] = new StandGuardChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::SQUAT_GUARD:
	//		m_pChallenge[i] = new SquatGuardChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::DUSH:
	//		m_pChallenge[i] = new DushChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::BACK_STEP:
	//		m_pChallenge[i] = new BackStepChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::PARTNER:
	//		m_pChallenge[i] = new PartnerChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::INVINCIBLE:
	//		m_pChallenge[i] = new InvincibleChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::THROW:
	//		m_pChallenge[i] = new ThrowChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::ESCAPE:
	//		m_pChallenge[i] = new EscapeChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::DOKKOI:
	//		m_pChallenge[i] = new DokkoiChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::DOWN_ATTACK:
	//		m_pChallenge[i] = new DownAttackChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::OD_ONEMORE:
	//		m_pChallenge[i] = new OverDriveOneMoreChallenge(no);// 
	//		break;
	//	case CHALLENGE_TYPE::OD_BURST:
	//		m_pChallenge[i] = new OverDriveBurstChallenge(no);// 
	//		break;
	//	default:
	//		MyAssert(0, "そのタイプは存在しない");
	//		break;
	//	}

	//}

	// 初期設定
	m_iSelectType = 0;

	// 配列の終り
	m_iArrayEnd = (int)CHALLENGE_TYPE::ARRAY_END;
}

BaseChallengeManager::~BaseChallengeManager()
{
	// 全てのチュートリアルを解放
	for (int i = 0; i < (int)CHALLENGE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pChallenge[i]);
	}

}

// 初期化
void BaseChallengeManager::Init(int iSelectType, int iDeviceID)
{
	m_iSelectType = iSelectType;// チュートリアルのタイプ変更

	m_pChallenge[(int)m_iSelectType]->Init(iDeviceID);// 操作するデバイス設定

}

// 初期化
void BaseChallengeManager::Init(int iDeviceID)
{
	if (m_iSelectType == (int)CHALLENGE_TYPE::ARRAY_END) { MyAssert(0, "配列の最後まで到達しました。"); return; }

	m_pChallenge[(int)m_iSelectType]->Init(iDeviceID);// 操作するデバイス設定

}

// 更新
void BaseChallengeManager::Update()
{
	if (m_iSelectType == (int)CHALLENGE_TYPE::ARRAY_END)return;

	// チュートリアル更新
	m_pChallenge[(int)m_iSelectType]->Update();

}

// タスク用更新
void BaseChallengeManager::TaskUpdate(BasePlayer * pPerson)
{
	// タスク用更新
	m_pChallenge[(int)m_iSelectType]->TaskUpdate(pPerson);

}

// 描画
void BaseChallengeManager::Render()
{
	if (m_iSelectType == (int)CHALLENGE_TYPE::ARRAY_END)return;

	// チュートリアル描画
	m_pChallenge[(int)m_iSelectType]->Render();
}


// 次のチュートリアルへ
void BaseChallengeManager::NectStep()
{
	int l_iNectNo = (int)m_iSelectType + 1;
	m_iSelectType = l_iNectNo;

}

void BaseChallengeManager::BackStep()
{
	int l_iNectNo = (int)m_iSelectType - 1;
	m_iSelectType = l_iNectNo;

}

std::string BaseChallengeManager::GetTitleString()
{

	return m_pChallenge[(int)m_iSelectType]->GetTitleString();

}

std::string BaseChallengeManager::GetTitleString(int iNo)
{

	return m_pChallenge[iNo]->GetTitleString();

}



//+---------------------------------
// アイル―チャレンジマネージャー
//+---------------------------------

AirouChallengeManager::AirouChallengeManager()
{
	// チャレンジデータ設定
	for (int i = 0; i < (int)CHALLENGE_TYPE::ARRAY_END; i++)
	{
		int l_iNo = i + 1;

		switch ((CHALLENGE_TYPE)i)
		{
		case CHALLENGE_TYPE::MISSION_1:
			m_pChallenge[i] = new AirouChallenge1(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_2:
			m_pChallenge[i] = new AirouChallenge2(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_3:
			m_pChallenge[i] = new AirouChallenge3(l_iNo);
			break;
		default:
			MyAssert(0, "そのタイプはない");
			break;
		}
	}

}


//+---------------------------------
// テキチャレンジマネージャー
//+---------------------------------

TekiChallengeManager::TekiChallengeManager()
{
	// チャレンジデータ設定
	for (int i = 0; i < (int)CHALLENGE_TYPE::ARRAY_END; i++)
	{
		int l_iNo = i + 1;

		switch ((CHALLENGE_TYPE)i)
		{
		case CHALLENGE_TYPE::MISSION_1:
			m_pChallenge[i] = new TekiChallenge1(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_2:
			m_pChallenge[i] = new TekiChallenge2(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_3:
			m_pChallenge[i] = new TekiChallenge3(l_iNo);
			break;
		default:
			MyAssert(0, "そのタイプはない");
			break;
		}
	}

}


//+---------------------------------
// (∵)チャレンジマネージャー
//+---------------------------------

NazenarabaChallengeManager::NazenarabaChallengeManager()
{
	// チャレンジデータ設定
	for (int i = 0; i < (int)CHALLENGE_TYPE::ARRAY_END; i++)
	{
		int l_iNo = i + 1;

		switch ((CHALLENGE_TYPE)i)
		{
		case CHALLENGE_TYPE::MISSION_1:
			m_pChallenge[i] = new NazenarabaChallenge1(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_2:
			m_pChallenge[i] = new NazenarabaChallenge2(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_3:
			m_pChallenge[i] = new NazenarabaChallenge3(l_iNo);
			break;
		default:
			MyAssert(0, "そのタイプはない");
			break;
		}
	}

}


//+---------------------------------
// アラミタマチャレンジマネージャー
//+---------------------------------

AramitamaChallengeManager::AramitamaChallengeManager()
{
	// チャレンジデータ設定
	for (int i = 0; i < (int)CHALLENGE_TYPE::ARRAY_END; i++)
	{
		int l_iNo = i + 1;

		switch ((CHALLENGE_TYPE)i)
		{
		case CHALLENGE_TYPE::MISSION_1:
			m_pChallenge[i] = new AramitamaChallenge1(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_2:
			m_pChallenge[i] = new AramitamaChallenge2(l_iNo);
			break;
		case CHALLENGE_TYPE::MISSION_3:
			m_pChallenge[i] = new AramitamaChallenge3(l_iNo);
			break;
		default:
			MyAssert(0, "そのタイプはない");
			break;
		}
	}

}

