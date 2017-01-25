#include "TekiChallenge.h"

//+-------------------------------
// テキチャレンジ
//+-------------------------------

TekiChallenge1::TekiChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "壁に追い込む";

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
	m_pIntroTips = new TipsCard("テキは近距離や画面端での攻撃が非常に強いキャラクターです。\nまずは相手を画面端に追い込む【コンボ】を覚えましょう。");
	m_pClearTips = new TipsCard("よくできました！\n攻撃や移動速度が優秀な分、HPが少し低く打たれ弱いです。\n相手に主導権を握らせない立ち回りを心がけましょう。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Teki/1.bin";

	// タスクセット
	AddTaskData("足払い－＼+▽");
	AddTaskData("キルラッシュ－△");
	AddTaskData("少し間隔を開けて追撃－△");

	Init();
}

void TekiChallenge1::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	
}

// それぞれのクリア処理
void TekiChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		_DOWN,
		_SKILL,
		_SKILL2,

	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{

		if (pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_DOWN);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND2)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
			}

		}


	}
}


//+-----------------------------
//	2番目の説明
//+-----------------------------
TekiChallenge2::TekiChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "空中コンボ";

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
	m_pIntroTips = new TipsCard("空中にいる相手を地面に落とす【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n空中コンボは大きいダメージを与えれるので\n積極的に狙っていきましょう。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Teki/2.bin";

	// タスクセット
	AddTaskData("対空攻撃－↓+▽");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("〆の必殺技－空中で△");

	Init();
}

void TekiChallenge2::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::JUMP;


}

// それぞれのクリア処理
void TekiChallenge2::TaskUpdate(BasePlayer * pPerson)
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

		// ★空中スキルを当てたら
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}




//+-----------------------------
//	3番目の説明
//+-----------------------------
TekiChallenge3::TekiChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "画面端コンボ";

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
	m_pIntroTips = new TipsCard("相手を浮かし空中コンボへ繋げる【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n難しいですが下段技の足払いから\n相手に大きいダメージを与えれるので非常に強力です。");

	// リプレイデータの名前
	m_strReplayDataName = "Data/ChallengeData/Teki/3.bin";

	// タスクセット
	AddTaskData("足払い－＼+▽");
	AddTaskData("キルラッシュ－△");
	AddTaskData("攻撃－□");
	AddTaskData("追加攻撃－□");
	AddTaskData("対空攻撃－↓+▽");

	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("〆の必殺技－空中で△");

	Init();
}

void TekiChallenge3::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;

}

// それぞれのクリア処理
void TekiChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;




	//ここで色々クリア処理頑張る！
	enum
	{
		_DOWN,
		_SKILL,
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
		if (pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_DOWN);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
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

		// ★空中スキルを当てたら
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}

