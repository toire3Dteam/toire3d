#include "Tutorial.h"
#include "Sound\SoundManager.h"

//+-----------------------------
//	ベース
//+-----------------------------

BaseTutorial::BaseTutorial()
{
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

	// タイトル名
	m_pTaskTitle.pSting = "None";
	m_pTaskTitle.bClear = false;
	m_pTaskTitle.pSting = 0;

	// セレクトナンバー
	m_iSelectTask = 0;

	m_pClearPic = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPic->OrderAlphaMove(55, 6, 45);
	
	m_pClearPicRip = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPicRip->OrderRipple(14, 1, 0.025f);

	m_iDeviceID = 0;
}

BaseTutorial::~BaseTutorial()
{
	SAFE_DELETE(m_pTaskIcon);
	SAFE_DELETE(m_pIntroTips);
	SAFE_DELETE(m_pClearTips);
	SAFE_DELETE(m_pClearPic);
	SAFE_DELETE(m_pClearPicRip);

	m_aTask.clear();	
}

void BaseTutorial::Init(int iDeviceID)
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

	// 初期の状況設定
	// 必要な状況があるなら各自書き換える
	SelectDataMgr->Get()->tagTrainingDatas.eHpRecovery = HP_RECOVERY_TYPE::AUTO_RECOVERY;
	SelectDataMgr->Get()->tagTrainingDatas.iHp1P=100;
	SelectDataMgr->Get()->tagTrainingDatas.iHp2P=100;
	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 0;
	SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery = PARTNER_RECOVERY_TYPE::DEFAULT;
	SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo = TRAINING_INFO_TYPE::NONE;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState=ENEMY_STATE_TYPE::STAND;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard=ENEMY_GUARD_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch = ENEMY_GUARD_SWITCH_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech = ENEMY_TECH_TYPE::ALL;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech = ENEMY_THROW_TECH_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv = 20;

}

void BaseTutorial::Update()
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

void BaseTutorial::Render()
{

	// タスクタイトル
	for (int j = 0; j < m_pTaskTitle.iStingLength + 3/*他より長く*/; j++)
		m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8 - 16 , (int)m_vTaskPos.y - 40 - 4, 32, 32, 0, 0, 32, 32);
	tdnFont::RenderString(m_pTaskTitle.pSting, "HGｺﾞｼｯｸE", 22, (int)m_vTaskPos.x+16, (int)m_vTaskPos.y - 40 , 0xffffffff, RS::COPY);


	// タスク覧
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		for (int j = 0; j < m_aTask[i].iStingLength; j++)
		{
			m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4, 32, 32, 32 * m_aTask[i].bClear , 0, 32, 32);
		
			// 選択しているタスクなら
			if (m_iSelectTask == i)
			{
				m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4,
					32, 32, 0, 32, 32, 32);
			}
		
		}

		DWORD fontCol = 0xffffffff;
		if (m_iSelectTask == i)fontCol = 0xff222222;
		tdnFont::RenderString(m_aTask[i].pSting, "HGｺﾞｼｯｸE", 22, (int)m_vTaskPos.x, (int)m_vTaskPos.y + (i * 40), fontCol, RS::COPY);
	}

	// TIPS
	m_pIntroTips->Render();
	m_pClearTips->Render();

	// クリア演出用
	m_pClearPic->Render(0, 0);
	m_pClearPicRip->Render(0, 0, RS::ADD);
}

void BaseTutorial::ActionIntroTips()
{
	//アクション！今は画像出すだけ
	m_pIntroTips->Action();

}
void BaseTutorial::StopIntroTips()
{
	m_pIntroTips->End();
}

void BaseTutorial::ActionClearTips()
{
	//アクション！今は画像出すだけ
	m_pClearTips->Action();

}
void BaseTutorial::StopClearTips()
{
	m_pClearTips->End();
}

void BaseTutorial::ActionClear()
{
	m_bClearFlag = true;

	// ↓でクリア演出
	m_pClearPic->Action();
	m_pClearPicRip->Action();

	// SE
	se->Play("オールクリア");
}


//void BaseTutorial::TaskReset()
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


void BaseTutorial::AddTaskData(LPSTR string)
{
	Task data;
	data.pSting = string;
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

void BaseTutorial::TaskSuccess(int select)
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

//+------------------------------------------------------
//	歩く説明
//+------------------------------------------------------

WalkTutorial::WalkTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.01 歩き";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("まずは移動の基本、「歩き」です。\n方向キーでキャラクターを動かしてみましょう。");
	m_pClearTips = new TipsCard("よくできました！\n　相手の方向へ方向キーを押すと「前歩き」、\n相手の逆方向へ押すと「後ろ歩き」になります。");


	// タスクセット
	AddTaskData("前歩き－→押しっぱなし");
	AddTaskData("後ろ歩き－←押しっぱなし");

	Init();
}

void WalkTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	m_iWalkFrame = 0;
	m_iBackFrame = 0;
}


//void WalkTutorial::Update()
//{
//	// ★共通
//	BaseTutorial::Update(); 
//}
//
//void WalkTutorial::Render()
//{
//	// ★共通
//	BaseTutorial::Render();
//
//
//	//tdnFont::RenderFont2D(str, 32, 200, 600, 0xffffffff);
//
//}

// それぞれのクリア処理
void WalkTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum 
	{
		WALK = 0, BACK = 1
	};

	// 30フレーム歩くステートにいたら最初クリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()))
	{
		m_iWalkFrame++;

		if (m_iWalkFrame >= 30)
		{
			TaskSuccess(WALK);
		}
	}
	else
	{
		m_iWalkFrame = 0;
	}

	// 30フレーム後ろ歩きステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::BackWalk::GetInstance()))
	{
		m_iBackFrame++;

		if (m_iBackFrame >= 30)
		{
			TaskSuccess(BACK);
		}
	}
	else
	{
		m_iBackFrame = 0;
	}

}


//+------------------------------------------------------
//	しゃがみ説明
//+------------------------------------------------------

SquatTutorial::SquatTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.02 しゃがみ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;
	
	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("次はキャラクターをしゃがませてみましょう。\n↓でその場でしゃがみます。");
	m_pClearTips = new TipsCard("よくできました！\n／や＼と入力しても、しゃがむことが出来ます。");

	// タスクセット
	AddTaskData("しゃがみ－↓押しっぱなし");

	Init();
}

void SquatTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	m_iSquatFrame = 0;
}

// それぞれのクリア処理
void SquatTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum
	{
		SQUAT = 0
	};

	// 30フレームしゃがみステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()))
	{
		m_iSquatFrame++;

		if (m_iSquatFrame >= 30)
		{
			TaskSuccess(SQUAT);
		}
	
	}else
	{
		m_iSquatFrame = 0;
	}


}


//+------------------------------------------------------
//	ジャンプの説明
//+------------------------------------------------------

JumpTutorial::JumpTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.03 ジャンプ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「ジャンプ」と「空中ジャンプ」です。\n地上で↑を押すことで「ジャンプ」をします。\n空中でもう一度↑を押すと「空中ジャンプ」になります。");
	m_pClearTips = new TipsCard("よくできました！\n「ジャンプ」は←や→と入力することで、\n「前ジャンプ」や「後ろジャンプ」になります。");

	// タスクセット
	AddTaskData("ジャンプ－↑");
	AddTaskData("空中ジャンプ－空中で↑");

	Init();
}

void JumpTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);
}

// それぞれのクリア処理
void JumpTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum
	{
		JUMP = 0, AERIAL_JUMP = 1
	};

	// ジャンプステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()))
	{
		TaskSuccess(JUMP);
	}

	// 空中ジャンプステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()))
	{
		TaskSuccess(AERIAL_JUMP);
	}

}


//+------------------------------------------------------
//	攻撃の説明
//+------------------------------------------------------

AttackTutorial::AttackTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.04 攻撃";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	//m_pIntroTips = new TipsCard("さあ、今度は基本の攻撃方法を学びましょう。\nその場で□、しゃがんで□、ジャンプ中に□、\n相手と逆方向で□を押してみましょう。");
	m_pIntroTips = new TipsCard("さあ、今度は基本の攻撃方法を学びましょう。\nその場で□を押してみましょう。");
	m_pClearTips = new TipsCard("よくできました！\n方向キーとの組み合わせで様々な攻撃が繰り出せます。\n「対空攻撃」は空中からの攻撃に対して【無敵】です。");

	// タスクセット
	AddTaskData("攻撃－□");
	//AddTaskData("しゃがみ攻撃－↓＋□");
	//AddTaskData("ジャンプ攻撃－空中で□");
	//AddTaskData("対空攻撃－←＋□");


	Init();
}

void AttackTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

}

// それぞれのクリア処理
void AttackTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum
	{
		ATTACK = 0, SQUAT_ATTACK = 1, JUMP_ATTACK = 2, ANTIAIR_ATTACK = 3
	};

	// 攻撃ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack()==true)
		{
			TaskSuccess(ATTACK);
		}	
	}

	// しゃがみ攻撃ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			//TaskSuccess(SQUAT_ATTACK);
			TaskSuccess(ATTACK);
		}
	}

	// 空中攻撃ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			//TaskSuccess(JUMP_ATTACK);
			TaskSuccess(ATTACK);
		}
	}

	// 対空攻撃ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			//TaskSuccess(ANTIAIR_ATTACK);
			TaskSuccess(ATTACK);
		}
	}
}

//+------------------------------------------------------
//	スキル攻撃の説明
//+------------------------------------------------------

SkillTutorial::SkillTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.05 スキル";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「スキル」はキャラクター毎にある強力な技です。\n△ボタンでスキルを発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「スキル」はキャラクター毎に様々な技があります。\nまた「攻撃」から基本的に繋げて発動できます。");

	// タスクセット
	AddTaskData("スキル－△");

	Init();
}

void SkillTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

}

// それぞれのクリア処理
void SkillTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	enum
	{
		SKILL = 0
	};

	// スキルステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(SKILL);
		}

	}

}


//+------------------------------------------------------
//	必殺技の説明
//+------------------------------------------------------

OverDriveTutorial::OverDriveTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.06 必殺技";

	// クリアから終りまで
	m_iWaitFrameMAX = 120;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("ゲージが50以上ある時に㊨を押し込むと。\n「必殺技」を発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「必殺技」はとても強力な技です。\nまた「攻撃」や「スキル」から繋げて発動できます。");

	// タスクセット
	AddTaskData("必殺技－ゲージ50%以上で㊨");

	Init();
}

void OverDriveTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// それぞれのクリア処理
void OverDriveTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	enum
	{
		OVER_DRIVE = 0
	};

	// 必殺技ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(OVER_DRIVE);
		}

	}

}

//+------------------------------------------------------
//	ラッシュ攻撃の説明
//+------------------------------------------------------

RushTutorial::RushTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.07 ラッシュコンボ";

	// クリアから終りまで
	m_iWaitFrameMAX = 120;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("ずっと□。\n「必殺技」を発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「必殺技」はとても強力な技です。\nまた「攻撃」や「スキル」から繋げて発動できます。");

	// タスクセット
	AddTaskData("攻撃－□");
	AddTaskData("追撃－□");
	AddTaskData("スキル－□");
	AddTaskData("必殺技－□");

	Init();
}

void RushTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// それぞれのクリア処理
void RushTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;	


	//ここで色々クリア処理頑張る！
	enum
	{
		ATTACK = 0,
		ATTACK2 = 1,
		SKIL = 2,
		OVER_DRIVE = 3,

	};

	// 待機に戻ると最初からやり直し
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()))
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}


	// 一段目を当てたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
		pPerson->GetRushStep() == 0)
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(ATTACK);
		}

	}

	// 二段目を当てたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
		pPerson->GetRushStep() == 1)
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(ATTACK2);
		}

	}

	// 三段目を当てたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
		pPerson->GetRushStep() == 2)
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(SKIL);
		}

	}

	// 必殺技ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()))
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(OVER_DRIVE);
		}

	}

}


//+------------------------------------------------------
//	立ちガードの説明
//+------------------------------------------------------

StandGuardTutorial::StandGuardTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.08 立ちガード";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("ガード三回。\n「必殺技」を発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「必殺技」はとても強力な技です。\nまた「攻撃」や「スキル」から繋げて発動できます。");

	// タスクセット
	AddTaskData("ガード－←");
	AddTaskData("ガード－←");
	AddTaskData("ガード－←");

	Init();
}

void StandGuardTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::T_DOKKOI_ATTACK;

	m_bStop = false;
	m_iCount = 0;
}

// それぞれのクリア処理
void StandGuardTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		SUCCESS1 = 0,
		SUCCESS2 = 1,
		SUCCESS3 = 2,
	};

	// 成功した場合ガード状態が解けてから次のタスクへ
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == false)
	{
		m_bStop = false;
	}

	// ガードしたらクリア
	if (m_bStop == false && 
		pPerson->isGuardSuccess() == true)
	{
		//for (int i = 0; i < m_aTask.size(); i++)
		//{

		//}
		if (m_iCount == 0)		TaskSuccess(SUCCESS1);
		else if (m_iCount == 1)	TaskSuccess(SUCCESS2);
		else if (m_iCount == 2)	TaskSuccess(SUCCESS3);

		m_bStop = true;
		m_iCount++;
	}


}



//+------------------------------------------------------
//	しゃがみガードの説明
//+------------------------------------------------------

SquatGuardTutorial::SquatGuardTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.09 しゃがみガード";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("しゃがみガード三回。\n「必殺技」を発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「必殺技」はとても強力な技です。\nまた「攻撃」や「スキル」から繋げて発動できます。");

	// タスクセット
	AddTaskData("しゃがみガード－／");
	AddTaskData("しゃがみガード－／");
	AddTaskData("しゃがみガード－／");

	Init();
}

void SquatGuardTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::T_DOWN_ATTACK;

	m_bStop = false;
	m_iCount = 0;
}

// それぞれのクリア処理
void SquatGuardTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		SUCCESS1 = 0,
		SUCCESS2 = 1,
		SUCCESS3 = 2,
	};

	// 成功した場合ガード状態が解けてから次のタスクへ
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == false)
	{
		m_bStop = false;
	}

	// ガードしたらクリア
	if (m_bStop == false &&
		pPerson->isGuardSuccess() == true)
	{
		//for (int i = 0; i < m_aTask.size(); i++)
		//{

		//}
		if (m_iCount == 0)		TaskSuccess(SUCCESS1);
		else if (m_iCount == 1)	TaskSuccess(SUCCESS2);
		else if (m_iCount == 2)	TaskSuccess(SUCCESS3);

		m_bStop = true;
		m_iCount++;
	}

}


//+------------------------------------------------------
//	しゃがみガードの説明
//+------------------------------------------------------

EscapeTutorial::EscapeTutorial()
{
	// タイトル名
	m_pTaskTitle.pSting = "No.10 回避";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("回避。\n「必殺技」を発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「必殺技」はとても強力な技です。\nまた「攻撃」や「スキル」から繋げて発動できます。");

	// タスクセット
	AddTaskData("回避－○");

	Init();
}

void EscapeTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);


}

// それぞれのクリア処理
void EscapeTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		ESCAPE = 0,
	};

	// 回避したら成功
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Escape::GetInstance()) == true)
	{
		TaskSuccess(ESCAPE);
	}

}
