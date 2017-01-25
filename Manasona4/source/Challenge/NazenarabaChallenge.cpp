#include "NazenarabaChallenge.h"

//+-------------------------------
// (∵)チャレンジ
//+-------------------------------

NazenarabaChallenge1::NazenarabaChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "中距離が得意";

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
	m_pIntroTips = new TipsCard("(∵)は中距離の立ち回りが非常に強いキャラクターです。\nまずは中距離から繋がる【コンボ】を覚えましょう。");
	m_pClearTips = new TipsCard("よくできました！\n(∵)は近距離での戦いは苦手です。\n相手を寄せ付けない立ち回りを心がけましょう。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Nazenaraba/1.bin";

	// タスクセット
	AddTaskData("相手を浮かす－↓+△");
	AddTaskData("浮いてる相手を引っ張る－△");

	AddTaskData("近づいた相手を攻撃－□");
	AddTaskData("追撃－□");
	AddTaskData("スキル－□");
	AddTaskData("必殺技－□");

	Init();
}

void NazenarabaChallenge1::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);


	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
}

// それぞれのクリア処理
void NazenarabaChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		_SKILL,
		_SKILL2,
		ATTACK ,
		ATTACK2 ,
		SKIL ,
		OVER_DRIVE,
	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{
		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
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


//+-----------------------------
//	2番目の説明
//+-----------------------------
NazenarabaChallenge2::NazenarabaChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "投げて遠ざける";

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
	m_pIntroTips = new TipsCard("投げ【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n近づいてきた相手は投げて\n自分の得意な距離に持っていきましょう。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Nazenaraba/2.bin";

	// タスクセット
	AddTaskData("投げ－×");

	AddTaskData("浮いてる相手を引っ張る－△");

	AddTaskData("近づいた相手を攻撃－□");
	AddTaskData("追撃－□");
	AddTaskData("スキル－□");
	AddTaskData("必殺技－□");

	Init();
}

void NazenarabaChallenge2::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);


	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
}

// それぞれのクリア処理
void NazenarabaChallenge2::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		THROW,
		_SKILL2,
		ATTACK,
		ATTACK2,
		SKIL,
		OVER_DRIVE,
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



		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
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




//+-----------------------------
//	3番目の説明
//+-----------------------------
NazenarabaChallenge3::NazenarabaChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "(∵)コンボ";

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
	m_pIntroTips = new TipsCard("スキルから空中コンボへ繋げる【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n相手に大きいダメージを与えれる\n【コンボ】なので非常に強力です。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Nazenaraba/3.bin";

	// タスクセット
	AddTaskData("相手を浮かす－↓+△");
	AddTaskData("浮いてる相手を引っ張る－△");

	AddTaskData("近づいた相手を攻撃－□");
	AddTaskData("追撃－□");

	AddTaskData("対空攻撃－↓+▽");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("〆の必殺技－空中で↓+△");

	Init();
}

void NazenarabaChallenge3::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);


}

// それぞれのクリア処理
void NazenarabaChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;




	//ここで色々クリア処理頑張る！
	enum
	{
		_SKILL,
		_SKILL2,
		_ATTACK1,
		_ATTACK2,
		_ANTIAIR,
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
		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
			}

		}


		// 通常攻撃
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 0)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_ATTACK1);
			}

		}

		//
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 1)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_ATTACK2);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_ANTIAIR);
			}

		}


		//+----------------------------------------------------------------
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

