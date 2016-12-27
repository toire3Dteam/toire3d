#pragma once
#include "TDNLib.h"
#include "../BasePlayer.h"

// エンティティ関連のインクルード
#include "../../BaseEntity/State/StateMachine.h"
#include "../../BaseEntity/State/state.h"
// ステート
#include "AIState.h"

#include "Data\SelectData.h"

//+---------------------------------
//	AI
//+---------------------------------
class AI : public BaseGameEntity
{
public:
	AI(SIDE side, BasePlayer* myBasePlayer, AI_TYPE AIType = AI_TYPE::PRACTICE_LAND);
	~AI();

	void Control();
	void Update();
	void DoublePushUpdate();
	void UpdateTraining();

	// AIの種類を変える
	void ChangeAIType(AI_TYPE type);

	// 二回押し発動
	void ActionDoublePush(PLAYER_INPUT inputNo);

	bool HandleMessage(const Message & msg);


	//+--------------------------------
	//	アクセサ		
	//+--------------------------------
	StateMachine<AI>* GetFSM()const { return m_pStateMachine; }// ★ステートマシンのアクセサを作る
	BasePlayer* GetBasePlayer(){ return m_pMyBasePlayer; }

	void PushInputList(PLAYER_INPUT inputNo)
	{
		int flag = m_pMyBasePlayer->GetInputList(inputNo);	// 現在のインプットの状態を取得
		flag = (flag & 0x1) ? 1 : 3;						// 押した瞬間か押しっぱなしかの分岐 
		m_pMyBasePlayer->SetInputList(inputNo, flag);		// 設定したインプットをプレイヤーに設定
		m_bPushFlag[(int)inputNo] = true;	// このフレーム中は押してるので、0に初期化しないでください
	}

	BasePlayer* GetTargetPlayer(){ return m_pTargetPlayer; };		// 攻撃相手
	void SetTargetPlayer(BasePlayer* targetplayer){ m_pTargetPlayer = targetplayer; }

	// パラメータ関連 アクセサー
	int GetChaseFrame(){ return m_iChaseFrame; }
	int GetWaitFrame(){ return m_iWaitFrame; }
	int GetGuardFrame(){ return m_iGuardFrame; }	
	int GetPracticeGuardFrame(){ return m_iPracticeGuardFrame; }
	bool GetPracticeGuardFlag(){ return m_bPracticeGuardFlag; }

	void SetChaseFrame(int rate){ m_iChaseFrame = rate; }
	void SetWaitFrame(int rate){ m_iWaitFrame = rate; }
	void SetGuardFrame(int rate){ m_iGuardFrame = rate; }
	void SetPracticeGuardFrame(int rate){ m_iPracticeGuardFrame = rate; }
	void SetPracticeGuardFlag(bool flag){ m_bPracticeGuardFlag = flag; }

	void AddChaseFrame(int add){ m_iChaseFrame += add; }
	void AddWaitFrame(int add){ m_iWaitFrame += add; }
	void AddGuardFrame(int add){ m_iGuardFrame += add; }
	void AddPracticeGuardFrame(int add){ m_iPracticeGuardFrame += add; }

	AI_TYPE GetAIType() { return m_eAIType; }

private:
	StateMachine<AI>* m_pStateMachine; // ★AIステートマシン
	BasePlayer* m_pMyBasePlayer;
	BasePlayer* m_pTargetPlayer;		// 攻撃相手

	bool m_bPushFlag[(int)PLAYER_INPUT::MAX];	// ボタンが押されたフレーム

	// AIに必要なパラメーター
	int m_iChaseFrame;		// 一途に追いかけてる時間
	int m_iWaitFrame;		// もじもじしてる時間
	int m_iGuardFrame;		// 一度のガードしている時間


	AI_TYPE m_eAIType;		// AIのタイプ

	// 練習モードでガードしてる時間
	int m_iPracticeGuardFrame;
	bool m_bPracticeGuardFlag;
	int m_iPracticeAttackIntervalFrame;	//	攻撃間隔フレーム

	// 二回押し用の変数
	bool m_bDoublePush;					// 二回押し命令は他の二回押しが終わるまで受付できない。
	int m_iDoublePushFrame;				// 
	PLAYER_INPUT m_eDoublePushInput;	// 

	//+------------------------------------
	//	トレーニング用
	//+------------------------------------
	


	// ステートをフレンドクラスへ
	friend class AIState::PracticeGlobal;
	friend class AIState::PracticeLand;
	friend class AIState::PracticeSquat;
	friend class AIState::PracticeJump;
	friend class AIState::PracticeAttack;
	friend class AIState::PracticeGuard;
	friend class AIState::Wait;
	friend class AIState::Chase;
	friend class AIState::Guard;
	friend class AIState::InterruptInvincible;

};
