#include "Tutorial.h"

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

void BaseTutorial::Init()
{
	m_iWaitFrame = m_iWaitFrameMAX;
	m_bClearFlag = false;


	// クリアフラグだけ初期化
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		m_aTask[i].bClear = false;
	}

	m_iSelectTask = 0;
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
	m_pIntroTips->Update();
	m_pClearTips->Update();

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
	m_pClearTips = new TipsCard("よくできました！\n「歩き」は移動の基本です。");


	// タスクセット
	AddTaskData("前歩き－→押しっぱなし");
	AddTaskData("後ろ歩き－←押しっぱなし");

	Init();
}

void WalkTutorial::Init()
{
	// ★共通
	BaseTutorial::Init();

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
	m_pIntroTips = new TipsCard("次はしゃがみです。\n↓でしゃがみます。");
	m_pClearTips = new TipsCard("よくできました！\n「しゃがみ」は楽しいね。");

	// タスクセット
	AddTaskData("しゃがみ－↓押しっぱなし");

	Init();
}

void SquatTutorial::Init()
{
	// ★共通
	BaseTutorial::Init();

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
	m_pIntroTips = new TipsCard("次はジャンプです。\n↑でジャンプします。\nもう一度↑で空中で飛べます。");
	m_pClearTips = new TipsCard("よくできました！\n「ジャンプ」は楽しいね。");

	// タスクセット
	AddTaskData("ジャンプ－↑");
	AddTaskData("空中ジャンプ－空中で↑");

	Init();
}

void JumpTutorial::Init()
{
	// ★共通
	BaseTutorial::Init();

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
	m_pIntroTips = new TipsCard("次は攻撃です。\n□で攻撃します。");
	m_pClearTips = new TipsCard("よくできました！\n「攻撃」は楽しいね。アー♂");

	// タスクセット
	AddTaskData("攻撃－□");

	Init();
}

void AttackTutorial::Init()
{
	// ★共通
	BaseTutorial::Init();

}

// それぞれのクリア処理
void AttackTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//ここで色々クリア処理頑張る！

	enum
	{
		ATTACK = 0
	};

	// 攻撃ステートにいたらクリア
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()))
	{
		if (pPerson->isHitAttack()==true)
		{
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
	m_pTaskTitle.pSting = "No.05 ドライブ攻撃";

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
	m_pIntroTips = new TipsCard("次はドライブ攻撃です。\nキャラクタ－により色々なドライブ攻撃があります。 \n△でドライブ攻撃です。");
	m_pClearTips = new TipsCard("よくできました！\n「ドライブ」は楽しいね。列車ぁぁぁアー♂");

	// タスクセット
	AddTaskData("ドライブ攻撃－△");

	Init();
}

void SkillTutorial::Init()
{
	// ★共通
	BaseTutorial::Init();

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
		if (pPerson->isHitAttack() == true)
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
	m_pIntroTips = new TipsCard("次は必殺技です。\nゲージを50%以上で○で必殺技です。");
	m_pClearTips = new TipsCard("よくできました！\n「必殺技」はマジキチだね。列車ぁぁぁアー♂");

	// タスクセット
	AddTaskData("必殺技－ゲージ50%以上で○");

	Init();
}

void OverDriveTutorial::Init()
{
	// ★共通
	BaseTutorial::Init();

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
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(OVER_DRIVE);
		}

	}

}