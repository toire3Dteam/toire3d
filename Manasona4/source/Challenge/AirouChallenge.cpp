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
	m_pIntroTips = new TipsCard("「連打コンボ」は「攻撃」を続けることで、\n【コンボ】が繋がっていくシステムです。\n相手に近づき、□ボタンを連打してみましょう！");
	m_pClearTips = new TipsCard("よくできました！\n□ボタンを連打するだけで「スキル」に繋がり、\nSPゲージがあれば、自動的に「必殺技」まで繋げてくれます\n初めての方は、まず「連打コンボ」で戦ってみましょう。");

	// タスクセット
	AddTaskData("ジャンプ攻撃－空中で□");
	AddTaskData("攻撃－□");
	AddTaskData("追撃－□");
	AddTaskData("スキル－□");
	AddTaskData("必殺技－□");

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
		ATTACK = 1,
		ATTACK2 = 2,
		SKIL = 3,
		OVER_DRIVE = 4,

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

