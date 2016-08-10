#pragma once
#include "TDNLib.h"
#include "../BasePlayer.h"

// エンティティ関連のインクルード
#include "../../BaseEntity/State/StateMachine.h"
#include "../../BaseEntity/State/state.h"

class AI : public BaseGameEntity
{
public:
	AI(int DeviceID, BasePlayer* myBasePlayer);
	~AI();

	void Control();
	void Update();

	bool HandleMessage(const Message & msg);

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

private:
	StateMachine<AI>* m_pStateMachine; // ★AIステートマシン
	BasePlayer* m_pMyBasePlayer;
	BasePlayer* m_pTargetPlayer;		// 攻撃相手

	bool m_bPushFlag[(int)PLAYER_INPUT::MAX];	// ボタンが押されたフレーム

};
