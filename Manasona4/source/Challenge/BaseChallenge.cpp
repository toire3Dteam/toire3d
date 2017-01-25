#include "BaseChallenge.h"
#include "Sound\SoundManager.h"

//+-----------------------------
//	ベース
//+-----------------------------

BaseChallenge::BaseChallenge(int iTitleNo)
{
	m_iTitleNo = iTitleNo;
	// タイトル名
	m_pTaskTitle.pString = "No.";

	// 10桁以下なら
	if (m_iTitleNo < 10)
	{
		m_pTaskTitle.pString += "0";// 0追加
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

	// タスク一覧
	m_aTask.clear();
	m_vTaskPos.x = 50;
	m_vTaskPos.y = 250;



	// セレクトナンバー
	m_iSelectTask = 0;

	m_pClearPic = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPic->OrderAlphaMove(55, 6, 45);

	m_pClearPicRip = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPicRip->OrderRipple(14, 1, 0.025f);

	m_iDeviceID = 0;

	m_bReplay = false;

	m_strReplayDataName = "Data/ChallengeData/なし.bin";
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


	// クリアフラグだけ初期化
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		m_aTask[i].bClear = false;
	}

	m_iSelectTask = 0;

	m_iDeviceID = iDeviceID;

	// 前回の演出を消す
	m_pClearPic->Stop();
	m_pClearPicRip->Stop();

	// 初期の状況設定
	// 必要な状況があるなら各自書き換える
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
	// クリア後の
	if (m_bClearFlag == true)
	{
		// カウントする
		m_iWaitFrame--;
		if (m_iWaitFrame <= 0)
		{
			// この時点で EndTipsStart()関数で状況を切り替える
			m_iWaitFrame = 0;
		}
	}
	else
	{
		// タスクが全部クリアされてるかチェック！
		int iClearNum = 0;
		for (int i = 0; i < (int)m_aTask.size(); i++)
		{
			if (m_aTask[i].bClear == true)
			{
				iClearNum++;
			}

			// もしクリア数がタスク数と同じになったら
			if ((int)m_aTask.size() == iClearNum)
			{
				// レッスンクリア！
				ActionClear();
			}
		}
	}



	// Tips
	m_pIntroTips->Update(m_iDeviceID);
	m_pClearTips->Update(m_iDeviceID);

	// クリア演出用
	m_pClearPic->Update();
	m_pClearPicRip->Update();

}

void BaseChallenge::Render()
{

	// タスクタイトル
	for (int j = 0; j < m_pTaskTitle.iStingLength + 3/*他より長く*/; j++)
		m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8 - 16, (int)m_vTaskPos.y - 40 - 4, 32, 32, 0, 0, 32, 32);
	tdnFont::RenderString(m_pTaskTitle.pString.c_str(), "HGｺﾞｼｯｸE", 22, (int)m_vTaskPos.x + 16, (int)m_vTaskPos.y - 40, 0xffffffff, RS::COPY);


	// タスク覧
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		for (int j = 0; j < m_aTask[i].iStingLength; j++)
		{
			m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4, 32, 32, 32 * m_aTask[i].bClear, 0, 32, 32);

			// 選択しているタスクなら
			if (m_iSelectTask == i)
			{
				m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4,
					32, 32, 0, 32, 32, 32);
			}

		}

		DWORD fontCol = 0xffffffff;
		if (m_iSelectTask == i)fontCol = 0xff222222;
		tdnFont::RenderString(m_aTask[i].pString.c_str(), "HGｺﾞｼｯｸE", 22, (int)m_vTaskPos.x, (int)m_vTaskPos.y + (i * 40), fontCol, RS::COPY);
	}

	// TIPS
	m_pIntroTips->Render();
	m_pClearTips->Render();

	// クリア演出用
	m_pClearPic->Render(0, 0);
	m_pClearPicRip->Render(0, 0, RS::ADD);
}

void BaseChallenge::ActionIntroTips()
{
	//アクション！今は画像出すだけ
	m_pIntroTips->Action();

}
void BaseChallenge::StopIntroTips()
{
	m_pIntroTips->End();
}

void BaseChallenge::ActionClearTips()
{
	//アクション！今は画像出すだけ
	m_pClearTips->Action();

}
void BaseChallenge::StopClearTips()
{
	m_pClearTips->End();
}

void BaseChallenge::ActionClear()
{
	// リプレイならクリアさせない
	if (m_bReplay == false)
	{

	m_bClearFlag = true;	

	// ↓でクリア演出
	m_pClearPic->Action();
	m_pClearPicRip->Action();

	// SE
	se->Play("オールクリア");

	}
}


//void BaseChallenge::TaskReset()
//{
//	// クリアフラグだけ初期化
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

	// 文字の長さを調べる
	UINT	myByte = 0;
	UINT	addByte = 0;
	//	終端文字までループ
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&string[i]);
		addByte += myByte;
	}
	data.iStingLength = addByte;

	m_aTask.push_back(data);

}

void BaseChallenge::TaskSuccess(int select)
{
	// 数字が一致しているもののみ反応 これにより順番通りタスクをこなす必要がある
	if (select == m_iSelectTask)
	{

		if ((int)m_aTask.size() == m_iSelectTask)
		{
			assert(0);// サイズ超え過ぎ
		}

		m_aTask[m_iSelectTask].bClear = true;
		m_iSelectTask++;

		// SE
		se->Play("ステップクリア");
	}
}
