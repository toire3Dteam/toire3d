#include "Tutorial.h"
#include "Sound\SoundManager.h"

//+-----------------------------
//	ベース
//+-----------------------------

BaseTutorial::BaseTutorial(int iTitleNo)
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

	// 前回の演出を消す
	m_pClearPic->Stop();
	m_pClearPicRip->Stop();

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
	tdnFont::RenderString(m_pTaskTitle.pString.c_str(), "HGｺﾞｼｯｸE", 22, (int)m_vTaskPos.x+16, (int)m_vTaskPos.y - 40 , 0xffffffff, RS::COPY);


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
		tdnFont::RenderString(m_aTask[i].pString.c_str(), "HGｺﾞｼｯｸE", 22, (int)m_vTaskPos.x, (int)m_vTaskPos.y + (i * 40), fontCol, RS::COPY);
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

WalkTutorial::WalkTutorial(int iTitleNo):BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "歩き";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
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

	// ～フレーム歩くステートにいたら最初クリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()))
	{
		m_iWalkFrame++;

		if (m_iWalkFrame >= 20)
		{
			TaskSuccess(WALK);
		}
	}
	else
	{
		m_iWalkFrame = 0;
	}

	// ～フレーム後ろ歩きステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::BackWalk::GetInstance()))
	{
		m_iBackFrame++;

		if (m_iBackFrame >= 20)
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

SquatTutorial::SquatTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "しゃがみ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;
	
	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
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

	// ～フレームしゃがみステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()))
	{
		m_iSquatFrame++;

		if (m_iSquatFrame >= 20)
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

JumpTutorial::JumpTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "ジャンプ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
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

AttackTutorial::AttackTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "攻撃";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	//m_pIntroTips = new TipsCard("さあ、今度は基本の攻撃方法を学びましょう。\nその場で□、しゃがんで□、ジャンプ中に□、\n相手と逆方向で□を押してみましょう。");
	m_pIntroTips = new TipsCard("さあ、今度は基本の攻撃方法を学びましょう。\nその場で□を押してみましょう。");
	m_pClearTips = new TipsCard("よくできました！\n方向キーとの組み合わせで様々な攻撃が繰り出せます。");// \n「対空攻撃」は空中からの攻撃に対して【無敵】です。

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

SkillTutorial::SkillTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "スキル";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
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

OverDriveTutorial::OverDriveTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "必殺技";

	// クリアから終りまで
	m_iWaitFrameMAX = 120;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
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

RushTutorial::RushTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "連打コンボ";

	// クリアから終りまで
	m_iWaitFrameMAX = 120;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「連打コンボ」は「攻撃」を続けることで、\n【コンボ】が繋がっていくシステムです。\n相手に近づき、□ボタンを連打してみましょう！");
	m_pClearTips = new TipsCard("よくできました！\n□ボタンを連打するだけで「スキル」に繋がり、\nSPゲージがあれば、自動的に「必殺技」まで繋げてくれます\n初めての方は、まず「連打コンボ」で戦ってみましょう。");

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
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) )
		//pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{


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

}


//+------------------------------------------------------
//	立ちガードの説明
//+------------------------------------------------------

StandGuardTutorial::StandGuardTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "立ちガード";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("攻撃の次は守りです。「立ちガード」を行いましょう\n相手と逆方向に方向キーを押しっぱなしにして、\n3回、攻撃を防ぎましょう。");
	m_pClearTips = new TipsCard("よくできました！\n「立ちガード」はジャンプ攻撃や「中段攻撃」などを防げます。\nただし「しゃがみ攻撃」や「足払い」などの\n【下段技】は防げないので注意が必要です。");

	// タスクセット
	AddTaskData("立ちガード－←押しっぱなし");
	AddTaskData("立ちガード－←押しっぱなし");
	AddTaskData("立ちガード－←押しっぱなし");

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

SquatGuardTutorial::SquatGuardTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "しゃがみガード";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「しゃがみガード」は地上戦の基本です!\n相手と逆方向に斜め下に方向キーを押しっぱなしにして、\n3回、攻撃を防ぎましょう。");
	m_pClearTips = new TipsCard("よくできました！\n「しゃがみガード」はジャンプ攻撃などの空中から来る攻撃や \n「中段攻撃」などは防げないですが、\n地上の相手に対しては、殆どの攻撃を防げます。");

	// タスクセット
	AddTaskData("しゃがみガード－／押しっぱなし");
	AddTaskData("しゃがみガード－／押しっぱなし");
	AddTaskData("しゃがみガード－／押しっぱなし");

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
//	回避の説明
//+------------------------------------------------------

EscapeTutorial::EscapeTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "回避";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「回避」は地上で攻撃を前方へ移動し回避する行動です。\n○ボタンを押してみましょう。");
	m_pClearTips = new TipsCard("よくできました！\n「回避」で移動している間は、\n【投げ技】以外の攻撃に対して【無敵状態】です。");

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



//+------------------------------------------------------
//	投げの説明
//+------------------------------------------------------

ThrowTutorial::ThrowTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "投げ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("相手に近づいて。×を押すと\n地上にいる相手を「投げ」ることが出来ます。");
	m_pClearTips = new TipsCard("よくできました！\n「投げ」は掴まった時すぐに×ボタンを押す【投げ抜け】\nでしかガードする事が出来ません。\nガードが上手い相手には、「投げ」が効果的です。");

	// タスクセット
	AddTaskData("投げ－×");

	Init();
}

void ThrowTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);


}

// それぞれのクリア処理
void ThrowTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		THROW = 0,
	};

	// 投げしたら成功
	if (pPerson->GetFSM()->isInState(*BasePlayerState::ThrowHold::GetInstance()) == true)
	{
		TaskSuccess(THROW);
	}

}


//+------------------------------------------------------
//	無敵技の説明
//+------------------------------------------------------

InvincibleTutorial::InvincibleTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "無敵技";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;

	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("無敵技は全キャラクターで使える\n相手の攻撃を受けない【無敵状態】のある攻撃です。\n㊧を押して発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n「無敵技」は攻撃を防ぐ強力な技です。ただし、はずれると\n致命的な隙が生まれるので使い所には注意が必要です。");

	// タスクセット
	AddTaskData("無敵技－㊧");

	Init();
}

void InvincibleTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);


}

// それぞれのクリア処理
void InvincibleTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		INVINCIBLE = 0,
	};

	// 無敵技ヒット
	if (pPerson->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) == true)
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(INVINCIBLE);
		}

	}

}



//+------------------------------------------------------
//	走る説明
//+------------------------------------------------------

DushTutorial::DushTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "ダッシュ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("実践的な移動方法「ダッシュ」です。\n相手方向に向かってタ・タンと二回→押し続けましょう。\n押しっぱなしでダッシュを続けます。");
	m_pClearTips = new TipsCard("よくできました！\n　「ダッシュ」は相手に近づく時の基本となります。");


	// タスクセット
	AddTaskData("ダッシュ－素早く→→押しっぱなし");

	Init();
}

void DushTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	m_iDushFrame = 0;
}

// それぞれのクリア処理
void DushTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	
	enum
	{
		DUSH = 0,
	};

	// ～フレーム走るステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Run::GetInstance()))
	{
		m_iDushFrame++;

		if (m_iDushFrame >= 20)
		{
			TaskSuccess(DUSH);
		}
	}
	else
	{
		m_iDushFrame = 0;
	}

}


//+------------------------------------------------------
//	バックステップ説明
//+------------------------------------------------------

BackStepTutorial::BackStepTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "バックステップ";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「バックステップ」です。\n相手とは逆方向に向かってタ・タンと二回←押しましょう。");
	m_pClearTips = new TipsCard("よくできました！\n相手の攻撃を避けたり、距離を取りたい時に使用しましょう。");


	// タスクセット
	AddTaskData("バックステップ－素早く←←");

	Init();
}

void BackStepTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

}

// それぞれのクリア処理
void BackStepTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum
	{
		BACK_STEP = 0,
	};

	// バックステップしたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::BackStep::GetInstance()))
	{
		TaskSuccess(BACK_STEP);
	}

}


//+------------------------------------------------------
//	パートナー説明
//+------------------------------------------------------

PartnerTutorial::PartnerTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "パートナースキル";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("自分の選んだパートナーを使用する「パートナースキル」です\n「パートナーゲージ」が最大まで溜まっている状態で\n◇ボタンを押すことで発動できます。");
	m_pClearTips = new TipsCard("よくできました！\n　【パートナー】は使用できるまで時間はかかりますが、\nとても強力な技を行ってくれます。\n自分にあった【パートナー】を見つけましょう。");

	// タスクセット
	AddTaskData("パートナースキル－Pゲージ最大で◇");

	Init();
}

void PartnerTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery = PARTNER_RECOVERY_TYPE::MAX;

}

// それぞれのクリア処理
void PartnerTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum
	{
		PARTNER_ATTACK = 0,
	};

	// パートナー発動でクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::StandAction::GetInstance()))
	{
		if (pPerson->GetStand()->isHit() == true)
		{
			TaskSuccess(PARTNER_ATTACK);
		}
	
	}

}


//+------------------------------------------------------
//	中段攻撃説明
//+------------------------------------------------------

DokkoiTutorial::DokkoiTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "中段攻撃";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("「中段攻撃」は「しゃがみガード」では防げない強力な一撃です\n▽ボタンで攻撃をヒットさせたら、\n▽か→ボタンを押すと【ダッシュキャンセル】ができます。");
	m_pClearTips = new TipsCard("よくできました！\n相手方向へ進む【ダッシュキャンセル】中は\n「攻撃」や「スキル」などで追撃ができます。");


	// タスクセット
	AddTaskData("中段攻撃－▽");
	AddTaskData("ダッシュキャンセル－中段攻撃HIT後▽or→");

	Init();
}

void DokkoiTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

}

// それぞれのクリア処理
void DokkoiTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	enum
	{
		DOKKOI = 0, DUSH_CHANCEL = 1
	};

	// 中段技
	if (pPerson->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()))
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(DOKKOI);
		}
	}

	// ダッシュキャンセル
	if (pPerson->GetActionState() == BASE_ACTION_STATE::FRAMECOUNT)
	{
		TaskSuccess(DUSH_CHANCEL);
	}
}


//+------------------------------------------------------
//	足払い説明
//+------------------------------------------------------

DownAttackTutorial::DownAttackTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "足払い";

	// クリアから終りまで
	m_iWaitFrameMAX = 60;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("相手方向しゃがみで▽ボタンを押すと「足払い」になります。");
	m_pClearTips = new TipsCard("よくできました！\n「足払い」は相手を必ずダウンさせられる【下段技】です。\n上手くダウンさせることでバトルの流れを呼び込みましょう。");


	// タスクセット
	AddTaskData("足払い－＼+▽");

	Init();
}

void DownAttackTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

}

// それぞれのクリア処理
void DownAttackTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	enum
	{
		DOWN_ATTACK = 0,
	};

	// パートナー発動でクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()))
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(DOWN_ATTACK);
		}
	}

}


//+------------------------------------------------------
//	覚醒説明
//+------------------------------------------------------

OverDriveOneMoreTutorial::OverDriveOneMoreTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "覚醒";

	// クリアから終りまで
	m_iWaitFrameMAX = 90;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("SPゲージが100ある状態で\n◎ボタンを押すと「覚醒」が発動します。");
	m_pClearTips = new TipsCard("よくできました！\n「覚醒」中は時間とともにSPゲージが減少していきますが、\n「必殺技」がいつでも発動できます。さらには発動時に技の隙を\n消す事もできるため【コンボ】で大ダメージを狙うこともできます");

	// タスクセット
	AddTaskData("覚醒－◎");

	Init();
}

void OverDriveOneMoreTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 100;
}

// それぞれのクリア処理
void OverDriveOneMoreTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	enum
	{
		OD = 0,
	};

	// 発動でクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()))
	{
	
		TaskSuccess(OD);
		
	}

}


//+------------------------------------------------------
//	バースト覚醒説明
//+------------------------------------------------------

OverDriveBurstTutorial::OverDriveBurstTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "バースト覚醒";

	// クリアから終りまで
	m_iWaitFrameMAX = 120;

	// 文字の長さを調べる
	UINT	myByte = 0; UINT	addByte = 0;
	// 終端文字までループ
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("SPゲージが100ある状態で相手の攻撃を受けている時に\n◎ボタンを押すと相手を吹き飛ばし「覚醒」する\n「バースト覚醒」が発動します。");
	m_pClearTips = new TipsCard("お疲れ様でした！\n以上でチュートリアルは終了です。");
	//m_pClearTips = new TipsCard("よくできました！\n「バースト覚醒」は相手を吹き飛ばし「覚醒」します\n非常に強力ですが「覚醒」中のSPゲージが早く減少していきます。");

	// タスクセット
	AddTaskData("バースト覚醒－ダメージを受けている最中に◎");

	Init();
}

void OverDriveBurstTutorial::Init(int iDeviceID)
{
	// ★共通
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 100;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::T_DOKKOI_ATTACK;

}

// それぞれのクリア処理
void OverDriveBurstTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！
	enum
	{
		OD = 0,
	};

	// 発動でクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_Burst::GetInstance()))
	{
		TaskSuccess(OD);

	}

}
