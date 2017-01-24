#include "AramitamaChallenge.h"

//+-------------------------------
// テキチャレンジ
//+-------------------------------

AramitamaChallenge1::AramitamaChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "御霊ファミリー";

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
	m_pIntroTips = new TipsCard("アラミタマはスキルを当てることで魂ゲージが増加します。\n魂ゲージが溜まるとスキルの終りに仲間が相手を攻撃します。\n仲間と協力する【コンボ】を覚えましょう。");
	m_pClearTips = new TipsCard("よくできました！\nアラミタマは魂ゲージが溜まるまでは弱いですが。\n魂ゲージが溜まった後は最強です。");

	// タスクセット
	AddTaskData("スキル－△");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("空中回転攻撃－空中で△");

	Init();
}

void AramitamaChallenge1::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	// ゲージMAX固定
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::MAX;
}

// それぞれのクリア処理
void AramitamaChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		_SKILL,
		AERIAL_ATTACK,
		AERIAL_SKILL,
	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{

		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// ★空中スキルを当てたら
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIAL)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_SKILL);
			}

		}


	}
}


//+-----------------------------
//	2番目の説明
//+-----------------------------
AramitamaChallenge2::AramitamaChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "補助技が・・";

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
	m_pIntroTips = new TipsCard("補助技だった技は魂ゲージを溜まれば仲間を使い\n攻撃技になります。挑戦してみましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n上手く使い相手を翻弄しましょう。");

	// タスクセット
	AddTaskData("スキル－△");
	AddTaskData("空中回避－空中で↓+△");
	AddTaskData("チャージ－↓+△");

	Init();
}

void AramitamaChallenge2::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	// ゲージMAX固定
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::MAX;
}

// それぞれのクリア処理
void AramitamaChallenge2::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;


	//ここで色々クリア処理頑張る！
	enum
	{
		SKILL,
		AERIAL_SKILL,
		CHARGE
	};

	// 相手の硬直が消えたらやり直し
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// デバイスは自分自身と同じ
	}
	else
	{
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(SKILL);
			}

		}

		// ★空中スキル
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIALDROP)
		{
		
			TaskSuccess(AERIAL_SKILL);		

		}

		// チャージ
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{
		
			TaskSuccess(CHARGE);

		}
	}
}

//+-----------------------------
//	3番目の説明
//+-----------------------------
AramitamaChallenge3::AramitamaChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// タイトル名
	m_pTaskTitle.pString += "ミタマファイナル";

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
	m_pIntroTips = new TipsCard("仲間達と協力する複雑な【コンボ】を覚えましょう。\n");
	m_pClearTips = new TipsCard("よくできました！\n魂ゲージを溜め驚異の破壊力で相手を圧倒しましょう。");

	// タスクセット
	AddTaskData("スキル－△");

	AddTaskData("対空攻撃－↓+▽");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("ジャンプキャンセル－↑");
	AddTaskData("空中攻撃－空中で□");
	AddTaskData("空中回転攻撃－空中で△");

	AddTaskData("チャージ－↓+△");

	AddTaskData("ミタマキャノン－㊨");

	Init();
}

void AramitamaChallenge3::Init(int iDeviceID)
{
	// ★共通
	BaseChallenge::Init(iDeviceID);

	// ゲージMAX固定
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::MAX;
	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// それぞれのクリア処理
void AramitamaChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// クリアしていたらハジク
	if (m_bClearFlag == true)return;




	//ここで色々クリア処理頑張る！
	enum
	{
		_SKILL,
		_ANTIAIR,
		JUMP,
		AERIAL_ATTACK,
		AERIAL_JUMP,
		AERIAL_ATTACK2,
		AERIAL_SKILL,
		CHARGE,
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
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
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
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIAL)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_SKILL);
			}

		}

		// チャージ
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{

			TaskSuccess(CHARGE);

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

