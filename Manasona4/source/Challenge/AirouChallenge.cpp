#include "AirouChallenge.h"

//+-------------------------------
// アイル―チャレンジ
//+-------------------------------

AirouChallenge1::AirouChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "崩し攻撃";

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
	m_pIntroTips = new TipsCard("通常攻撃が大振りで威力がとても高いキャラクターです。\nまずは攻撃を当てるために中段技や下段技を組み込んだ\nガードを崩す【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n相手のガードを崩す攻撃は対人戦ではとても大切です。 \n積極的に【コンボ】に組み込んで行きましょう。");

	// タスクセット
	AddTaskData("ジャンプ攻撃－空中で□");
	AddTaskData("じゃがみ攻撃－↓+□");
	AddTaskData("攻撃－□");
	AddTaskData("追撃－□");
	AddTaskData("スキル－□");
	AddTaskData("必殺技－□");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Airou/1.bin";

	Init();
}

void AirouChallenge1::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// それぞれのクリア処理
void AirouChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		AERIAL_ATTACK = 0,
		DOWN_ATTACK = 1,
		ATTACK = 2,
		ATTACK2 = 3,
		SKIL = 4,
		OVER_DRIVE = 5,

	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{
		// 空中攻撃を当てたらクリア
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// 下段攻撃を当てたらクリア
		if (pPerson->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DOWN_ATTACK);
			}

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
}

//+-------------------------------
//	２番目
//+-------------------------------
AirouChallenge2::AirouChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "対空攻撃";

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
	m_pIntroTips = new TipsCard("空中の相手に有効な\n対空攻撃からの【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\nしっかり相手の動きを見て攻撃をけん制するのも大事です。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Airou/2.bin";

	// タスクセット
	AddTaskData("対空攻撃－↓+▽");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("〆の必殺技－空中で↓+△");

	Init();
}

void AirouChallenge2::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	// SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::JUMP;


}

// それぞれのクリア処理
void AirouChallenge2::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		ANTI_AIR,
		JUMP,
		AERIAL_ATTACK,
		AERIAL_JUMP,
		AERIAL_ATTACK2,
		DROP_SKILL,
	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{

		if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ANTI_AIR);
			}

		}

		// ジャンプしたらクリア
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()))
		{
			TaskSuccess(JUMP);
		}



		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// 空中ジャンプしたらクリア
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()))
		{
			TaskSuccess(AERIAL_JUMP);
		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK2);
			}

		}

		// ★空中↓スキルを当てたら
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) && 
			pPerson->GetSkillActionType()== SKILL_ACTION_TYPE::AERIALDROP)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}


//+-------------------------------
//	3番目
//+-------------------------------
AirouChallenge3::AirouChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "壁で投げると";

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
	m_pIntroTips = new TipsCard("投げからの【コンボ】を覚えましょう。\n壁＝画面端の事");
	m_pClearTips = new TipsCard("よくできました！\n壁際により普段では繋がらない強力なコンボができました。\n投げる以外にも壁際でできるコンボは沢山あります。\n是非自分だけのコンボを見つけましょう。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Airou/3.bin";

	// タスクセット
	AddTaskData("投げ－×");
	AddTaskData("対空攻撃－↓+▽");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("〆の必殺技－空中で↓+△");

	Init();
}

void AirouChallenge3::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
	

}

// それぞれのクリア処理
void AirouChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		THROW,
		ANTI_AIR,
		JUMP,
		AERIAL_ATTACK,
		AERIAL_JUMP,
		AERIAL_ATTACK2,
		DROP_SKILL,
	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{
		// 投げ
		if (pPerson->GetFSM()->isInState(*BasePlayerState::ThrowSuccess::GetInstance()) == true)
		{	
			TaskSuccess(THROW);
		}

		if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ANTI_AIR);
			}

		}

		// ジャンプしたらクリア
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()))
		{
			TaskSuccess(JUMP);
		}



		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// 空中ジャンプしたらクリア
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()))
		{
			TaskSuccess(AERIAL_JUMP);
		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK2);
			}

		}

		// ★空中↓スキルを当てたら
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIALDROP)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}
