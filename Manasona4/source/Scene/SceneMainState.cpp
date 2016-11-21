#include "TDNLIB.h"
#include "SceneMainState.h"
#include "../BaseEntity/Message/Message.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../Stage/Stage.h"
#include "../Timer/Timer.h"
#include "../Fade/Fade.h"
#include "../system/FrameworkEx.h"
#include "sceneResult.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"
#include "RoundCall\RoundCallManager.h"
#include "../Camera/camera.h"
#include "LoadSceneThread.h"
#include "../SceneSwitch/SceneSwitch.h"
#include "MenuUI\TutorialManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "SceneMenu.h"
// これを定義するとラウンドコールがスキップされる(デバッグ時短用)
//#define ROUND_SKIP

//#ifdef ROUND_SKIP
#include "../UI/GameUI.h"
//#endif

/*******************************************************/
//					ゲーム開始のステージカメラステート
/*******************************************************/

void SceneMainState::StageIntro::Enter(sceneMain *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

	// カメラ発動
	EFFECT_CAMERA_INFO EffectCameraInfo;
	EffectCameraInfo.scriptID = (int)EFFECT_CAMERA_ID::SAND_STAGE_INTRO;	// 仮
	MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &EffectCameraInfo);

	m_bSkip = false;
}

void SceneMainState::StageIntro::Execute(sceneMain *pMain)
{
	if (m_bSkip)
	{
		if (Fade::isFadeOutCompletion())
		{
			pMain->GetFSM()->ChangeState(CharaIntro::GetInstance());
			return;
		}
	}
	else
	{
		FOR(PlayerMgr->GetNumPlayer())
		{
			const int PlayerDeviceID(PlayerMgr->GetPlayer(i)->GetDeviceID());
			if (KEY(KEY_A, PlayerDeviceID) == 3 || KEY(KEY_B, PlayerDeviceID) == 3 || KEY(KEY_C, PlayerDeviceID) == 3 || KEY(KEY_D, PlayerDeviceID) == 3)
			{
				m_bSkip = true;
				Fade::Set(Fade::FADE_OUT, 8);
				break;
			}
		}
	}

	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// カメラ更新
	CameraMgr->Update();

	// エフェクトカメラ終わったら掛け合いに行く
	if (!CameraMgr->GetFSM()->isInState(*EffectCameraState::GetInstance()))
	{
		// 掛け合いステートへ
		pMain->GetFSM()->ChangeState(CharaIntro::GetInstance());
	}
}

void SceneMainState::StageIntro::Exit(sceneMain *pMain){}
void SceneMainState::StageIntro::Render(sceneMain *pMain){}

bool SceneMainState::StageIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					キャラ掛け合いステート
/*******************************************************/

void SceneMainState::CharaIntro::Enter(sceneMain *pMain)
{
	// フェード初期化
	if(Fade::m_alpha >= 128) Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

	// キャラクターたちのイントロ
	PlayerMgr->SetIntroState();

	// エフェクトカメラだったら消す
	if (CameraMgr->GetFSM()->isInState(*EffectCameraState::GetInstance())) CameraMgr->OffEffectCamera();

	m_bSkip = false;

	m_iKakeaiTimer = 0;
}

void SceneMainState::CharaIntro::Execute(sceneMain *pMain)
{
	if (m_bSkip)
	{
		if (Fade::isFadeOutCompletion())
		{
			pMain->GetFSM()->ChangeState(Round::GetInstance());
			return;
		}
	}
	else
	{
		FOR(PlayerMgr->GetNumPlayer())
		{
			const int PlayerDeviceID(PlayerMgr->GetPlayer(i)->GetDeviceID());
			if (KEY(KEY_A, PlayerDeviceID) == 3 || KEY(KEY_B, PlayerDeviceID) == 3 || KEY(KEY_C, PlayerDeviceID) == 3 || KEY(KEY_D, PlayerDeviceID) == 3)
			{
				m_bSkip = true;
				Fade::Set(Fade::FADE_OUT, 8);
				break;
			}
		}
	}

	// くっそかりレベルで今は時間経過で次のラウンドコールへ
	if(++m_iKakeaiTimer > 180) pMain->GetFSM()->ChangeState(Round::GetInstance());

	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// カメラ更新
	CameraMgr->Update();
}

void SceneMainState::CharaIntro::Exit(sceneMain *pMain)
{
	// ★掛け合い終了メッセージを送る
	FOR(PlayerMgr->GetNumPlayer())
	{
		MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, PlayerMgr->GetPlayer(i)->GetID(), MESSAGE_TYPE::KAKEAI_END, nullptr);
	}
}
void SceneMainState::CharaIntro::Render(sceneMain *pMain){}

bool SceneMainState::CharaIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					ラウンドコールステート
/*******************************************************/

void SceneMainState::Round::Enter(sceneMain *pMain)
{
	// フェード初期化
	if (Fade::m_alpha >= 128) Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

#ifdef ROUND_SKIP
	//if (pMain->GetRoundNum() == 0)
	{
		GameUIMgr->Action();
	}
#else
	// ラウンド数が0の時はコールしない
	if (pMain->GetRoundNum() != 0)
	{
		// The Wheel of Fate is Turning.
		const int RoundNumber(PlayerMgr->GetRoundNumber());

		// 初回なら
		if (RoundNumber == 0)pMain->GetRoundCall()->CallFirstGameRound();
		else pMain->GetRoundCall()->CallRound(RoundNumber + 1);
	}
	else
	{
		GameUIMgr->Action();
	}

#endif
}
void SceneMainState::Round::Execute(sceneMain *pMain)
{
#ifdef ROUND_SKIP
	//if (pMain->GetRoundNum() == 0)
	{
		pMain->GetFSM()->ChangeState(Main::GetInstance());
	}
#endif
	
	if (pMain->GetRoundNum() == 0)
	{
		pMain->GetFSM()->ChangeState(Main::GetInstance());
	}

	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::NO_FSM);
	PlayerMgr->UpdatePos();

	// カメラ更新
	CameraMgr->Update();
}
void SceneMainState::Round::Exit(sceneMain *pMain){}
void SceneMainState::Round::Render(sceneMain *pMain){}

bool SceneMainState::Round::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::END_ROUNDCALL:	// ラウンドコールが終わったというメッセージが届いたら
		pMain->GetFSM()->ChangeState(Main::GetInstance());
		break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					メインステート
/*******************************************************/

void SceneMainState::Main::Enter(sceneMain *pMain){}
void SceneMainState::Main::Execute(sceneMain *pMain)
{
	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);

	// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
	Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager(), pMain->GetStage());

	// プレイヤー位置確定
	PlayerMgr->UpdatePos();


	// カメラ更新
	CameraMgr->Update();

	// タイマー更新&タイマーが終了なら終了ステートに行く
	if (TimeMgr->Update())
	{
		//pMain->GetFSM()->ChangeState(Finish::GetInstance());
	}

}
void SceneMainState::Main::Exit(sceneMain *pMain){}
void SceneMainState::Main::Render(sceneMain *pMain){}

bool SceneMainState::Main::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::OVER_DRIVE_STAGE:
	{
	   bool *bAction = (bool*)msg.ExtraInfo;
	   if (*bAction)
		   pMain->OverDriveAction();
	   else
		   pMain->OverDriveEnd();
	   return true;
	}
		break;
		// カットイン発動する瞬間
	case MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START:
		// BGMのクロスフェード(超必殺BGMを流す)
		bgm->StopStreamIn();
		bgm->PlayStreamIn("DATA/Sound/BGM/HeaveHo/HeaveHo.ogg");

		break;
		// カットイン終わったらって感じ
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		pMain->GetFSM()->ChangeState(HeaveHoDriveOverFlowSuccess::GetInstance());
		// シームレススレッド発動
		LoadSceneThreadMgr->Initialize(new sceneResult(PlayerMgr->GetPlayerByDeviceID((int)msg.Sender - (int)ENTITY_ID::ID_PLAYER_FIRST)->GetSide()));
		return true;
		break;
	case MESSAGE_TYPE::KO:	// 誰かのHPが0になったら切り替え
	{
		KO_INFO *KOInfo = (KO_INFO*)msg.ExtraInfo;
		// ラウンド全部取って勝利確定だったら、フィニッシュの段階でリザルトシーンを読み込む
		if (KOInfo->iNumWinnerRound == pMain->GetRoundNum() - 1)
		{
			LoadSceneThreadMgr->Initialize(new sceneResult(KOInfo->WinnerSide)); 
		}

		pMain->GetFSM()->ChangeState(Finish::GetInstance());

		if(KOInfo->FinishType == FINISH_TYPE::NORMAL)pMain->GetRoundCall()->CallFinish(msg.Sender);
		else if (KOInfo->FinishType == FINISH_TYPE::OVER_DRIVE) pMain->GetRoundCall()->CallOverDriveFinish(msg.Sender);

		return true;
	}
		break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/*******************************************************/
//					KO時ステート
/*******************************************************/

void SceneMainState::Finish::Enter(sceneMain *pMain)
{
	// フィニッシュコールフラグオフ
	m_bEndCall = false;
}
void SceneMainState::Finish::Execute(sceneMain *pMain)
{
	// シーン切り替わり画面中
	if (pMain->GetSceneSwitchPrev()->IsAction())
	{
		// シーンチェンジくん更新
		pMain->GetSceneSwitchPrev()->Update();

		// シーン切り替え終わったら
		if (pMain->GetSceneSwitchPrev()->IsEnd())
		{
			// 演出が終わった段階でもシーンを読み込んでいたら、終わるまで無限ループ
			while (!LoadSceneThreadMgr->isInitializeEnd());

			// すでに読み込んだリザルトシーンに行く
			MainFrameEx->ChangeScene(LoadSceneThreadMgr->GetNewScene());

			// シングルトンだからシーン変わってもデリートとか大丈夫のはず
			LoadSceneThreadMgr->Release();
		}
	}

	else
	{
		// プレイヤー更新
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager(), pMain->GetStage());

		// プレイヤー位置確定
		PlayerMgr->UpdatePos();

		// カメラ更新
		CameraMgr->Update();

		// フィニッシュコール終わったら
		if (m_bEndCall)
		{
			// フェード終わったら
			if (Fade::isFadeStop())
			{
				// まだ続くよ
				//else
				{
					// 情報初期化
					PlayerMgr->Reset();
					CameraMgr->SetPlayersPos();
					ParticleManager::Reset();

					pMain->GetFSM()->ChangeState(Round::GetInstance());
				}
			}
		}
		else
		{
			// ここに来たということは、たぶんトレーニングで敵を倒した場合
			if (Fade::isFadeOutCompletion())
			{
				// チュートリアルで殺してしまった
				if(pMain->GetFSM()->isPrevState(*SceneMainState::TutorialMain::GetInstance()))
					pMain->GetFSM()->ChangeState(SceneMainState::TutorialIntro::GetInstance());

				// (TODO)トレーニングモードで殺してしまった
				//if()
			}
		}
	}
}
void SceneMainState::Finish::Exit(sceneMain *pMain){}
void SceneMainState::Finish::Render(sceneMain *pMain){}
bool SceneMainState::Finish::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::OVER_DRIVE_STAGE:
	{
										   bool *bAction = (bool*)msg.ExtraInfo;
										   if (*bAction)
											   pMain->OverDriveAction();
										   else
											   pMain->OverDriveEnd();
										   return true;
	}
		break;

	case MESSAGE_TYPE::END_FINISHCALL:

		// チュートリアルorトレーニング
		if (pMain->GetRoundNum() == 0)
		{
			Fade::Set(Fade::FLAG::FADE_OUT, 8, 0x00000000);
		}

		// マジ対戦
		else
		{
			// プレイヤーがラウンドとってリザルトに行くのが確定しているなら
			if (LoadSceneThreadMgr->isAction())
			{
				// リザルト切り替わり発動
				pMain->GetSceneSwitchPrev()->Action();
			}

			// それ以外なら勝ったプレイヤーの勝ち演出
			else
			{
				ENTITY_ID *WinnerID = (ENTITY_ID*)msg.ExtraInfo;
				MsgMgr->Dispatch(0, ENTITY_ID::SCENE_MAIN, *WinnerID, MESSAGE_TYPE::END_FINISHCALL, nullptr);
			}
		}
		break;
	case MESSAGE_TYPE::END_WINNER:
		m_bEndCall = true;	// フィニッシュコール終わったよフラグを立てる
		Fade::Set(Fade::FLAG::FADE_OUT, 8, 0x00000000);
		break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

/*******************************************************/
//					超必ステート
/*******************************************************/

void SceneMainState::HeaveHoDriveOverFlowSuccess::Enter(sceneMain *pMain)
{
	// ステージチェンジ
	pMain->GetStage()->ChangeObj((0) ? "DATA/airou_stage.IMO" : "DATA/Stage/HeaveHo/HeaveHoStage.IMO", nullptr);
	pMain->GetStage()->GetObj()->SetScale(4);
	//pMain->GetStage()->GetObj()->SetPos(0, -20, 0);
	pMain->GetStage()->GetObj()->Update();
}
void SceneMainState::HeaveHoDriveOverFlowSuccess::Execute(sceneMain *pMain)
{
	// シーン切り替わり画面中
	if (pMain->GetSceneSwitchPrev()->IsAction())
	{
		// シーンチェンジくん更新
		pMain->GetSceneSwitchPrev()->Update();

		// アクションが終わったら
		if (pMain->GetSceneSwitchPrev()->IsEnd())
		{
			// ないと思うが、演出が終わった段階でもシーンを読み込んでいたら、終わるまで無限ループ
			while (!LoadSceneThreadMgr->isInitializeEnd());

			// すでに読み込んだリザルトシーンに行く
			MainFrameEx->ChangeScene(LoadSceneThreadMgr->GetNewScene());

			// シングルトンだからシーン変わってもデリートとか大丈夫のはず
			LoadSceneThreadMgr->Release();
		}
	}

	else
	{
		// プレイヤー更新
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager(), pMain->GetStage());

		// プレイヤー位置確定
		PlayerMgr->UpdatePos();

		// カメラ更新
		CameraMgr->Update();

		// エフェクトカメラ終わったらリザルトに行く
		if (!CameraMgr->GetFSM()->isInState(*EffectCameraState::GetInstance()))
		{
			// シーン切り替え画面エフェクト発動！
			pMain->GetSceneSwitchPrev()->Action();
		}
	}
}
void SceneMainState::HeaveHoDriveOverFlowSuccess::Exit(sceneMain *pMain) {}
void SceneMainState::HeaveHoDriveOverFlowSuccess::Render(sceneMain *pMain) {}
bool SceneMainState::HeaveHoDriveOverFlowSuccess::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					チュートリアル開始ステート
/*******************************************************/

void SceneMainState::TutorialIntro::Enter(sceneMain *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 16, 0x00000000);

	// (仮)歩き
	TutorialMgr->Init(pMain->GetSelectTutorial());
	TutorialMgr->GetTutorial()->ActionIntroTips();// イントロのTipsを用意
	
	// 最初の一回だけ
	//if (pMain->GetFSM()->isPrevState(*SceneMainState::TutorialClear::GetInstance()) == false)
	{
		GameUIMgr->Action();
	}

	// 情報初期化
	PlayerMgr->Reset();
	CameraMgr->SetPlayersPos();
	ParticleManager::Reset();
}

// 更新
void SceneMainState::TutorialIntro::Execute(sceneMain *pMain)
{
//#ifdef ROUND_SKIP
//	pMain->GetFSM()->ChangeState(Main::GetInstance());
//#endif
	
	// チュートリアル更新
	TutorialMgr->Update();

	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::NO_FSM);
	PlayerMgr->UpdatePos();

	// カメラ更新
	CameraMgr->Update();

	// ヒントカードを読み終えたら
	if (TutorialMgr->GetTutorial()->
		GetIntroTipsCard()->GetSelectState()== TipsCard::SELECT_STATE::OK)
	{
		pMain->GetFSM()->ChangeState(TutorialMain::GetInstance());
		return;
	}
}

void SceneMainState::TutorialIntro::Exit(sceneMain *pMain)
{
	// イントロのTipsを閉じる
	TutorialMgr->GetTutorial()->StopIntroTips();

}

void SceneMainState::TutorialIntro::Render(sceneMain *pMain)
{
	TutorialMgr->Render();
}

bool SceneMainState::TutorialIntro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::END_ROUNDCALL:	// ラウンドコールが終わったというメッセージが届いたら
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//			チュートリアルメインステート
/*******************************************************/

void SceneMainState::TutorialMain::Enter(sceneMain *pMain) 
{

	// ★TODO　ボタン　が履歴が残ったままなので　はいを選択したらそのまま攻撃してしまうの
	// ここで履歴をkesu


}

void SceneMainState::TutorialMain::Execute(sceneMain *pMain)
{
	// プレイヤー更新
	if (TutorialMgr->GetTutorial()->isClear())
	{
		// クリアにたので動きを止める
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);

		// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager(), pMain->GetStage());

		// プレイヤー位置確定
		PlayerMgr->UpdatePos();
	}
	else
	{
		// 通常
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);

		// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager(), pMain->GetStage());

		// プレイヤー位置確定
		PlayerMgr->UpdatePos();
	}

	// カメラ更新
	CameraMgr->Update();

	// 1Pの操作しているプレイヤーの情報を確認しクリア判定を出す！
	TutorialMgr->TaskUpdate(PlayerMgr->GetPlayer(0));

	// チュートリアル更新
	TutorialMgr->Update();
	
	// タスクを全てクリアして指定した時間が過ぎればクリアステートへ
	if (TutorialMgr->GetTutorial()->EndTipsStart())
	{
		pMain->GetFSM()->ChangeState(TutorialClear::GetInstance());
		return;
	}

	/********************************************/
	// 自分リスタート
	if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
	{

		// セレクトでもう一度イントロへ戻る
		if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, 0) == 3)
		{
			// フェードアウト
			Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		}

	}

	// フェードアウト完了後
	if (Fade::isFadeOutCompletion())
	{
		// チュートリアルのイントロへ
		pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());
		return;
	}
	/********************************************/
}

void SceneMainState::TutorialMain::Exit(sceneMain *pMain) 
{

}

void SceneMainState::TutorialMain::Render(sceneMain *pMain)
{
	TutorialMgr->Render();
}

bool SceneMainState::TutorialMain::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::OVER_DRIVE_STAGE:
	{
		bool *bAction = (bool*)msg.ExtraInfo;
		if (*bAction)
			pMain->OverDriveAction();
		else
			pMain->OverDriveEnd();
		return true;
	}
	break;
	// カットイン発動する瞬間
	case MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START:
		// BGMのクロスフェード(超必殺BGMを流す)
		//pMain->GetMyMusicManager()->PlayHeaveHo();
		bgm->StopStreamIn();
		bgm->PlayStreamIn("DATA/Sound/BGM/HeaveHo/HeaveHo.ogg");
		break;
		// カットイン終わったらって感じ
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		pMain->GetFSM()->ChangeState(HeaveHoDriveOverFlowSuccess::GetInstance());
		return true;
		break;
	case MESSAGE_TYPE::KO:	// 誰かのHPが0になったら切り替え
	{
		FINISH_TYPE *type = (FINISH_TYPE*)msg.ExtraInfo;
		pMain->GetFSM()->ChangeState(Finish::GetInstance());
		if (*type == FINISH_TYPE::NORMAL)pMain->GetRoundCall()->CallFinish(msg.Sender);
		else if (*type == FINISH_TYPE::OVER_DRIVE) pMain->GetRoundCall()->CallOverDriveFinish(msg.Sender);

		return true;
	}
	break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

/*******************************************************/
//			チュートリアルクリア(エンド)ステート
/*******************************************************/

void SceneMainState::TutorialClear::Enter(sceneMain *pMain)
{
	// クリア後のTipsを用意
	TutorialMgr->GetTutorial()->ActionClearTips();

	// 次のチュートリアルへ
	// (TODO)ここで配列の最後まできたらメニューに戻る処理
	// お疲れ様でした！とかは最後のクリアティップスで書こう。
	pMain->SetSelectTutorial(TUTORIAL_TYPE(pMain->GetSelectTutorial() + (TUTORIAL_TYPE)1));
	if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::ARRAY_END)
	{

	}

}

// 更新
void SceneMainState::TutorialClear::Execute(sceneMain *pMain)
{

	// チュートリアル更新
	TutorialMgr->Update();

	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	PlayerMgr->UpdatePos();

	// カメラ更新
	CameraMgr->Update();

	if (Fade::isFadeOutCompletion() == false) {
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{

			// ヒントカードを読み終えたら
			if (TutorialMgr->GetTutorial()->
				GetClearTipsCard()->GetSelectState() == TipsCard::SELECT_STATE::OK)
			{
				// クリア後のTipsを閉じる
				TutorialMgr->GetTutorial()->StopClearTips();

				// フェードアウト
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

			}
		}
	}else// フェードアウト完了後	
	{
		// (TODO)ここで配列の最後まできたらメニューに戻る処理
		if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::ARRAY_END)
		{
			MainFrameEx->ChangeScene(new sceneMenu());
			return;
		}
		else
		{
			// チュートリアルのイントロへ
			pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());
			return;
		}

	}

}

void SceneMainState::TutorialClear::Exit(sceneMain *pMain)
{

}

void SceneMainState::TutorialClear::Render(sceneMain *pMain)
{
	TutorialMgr->Render();
}

bool SceneMainState::TutorialClear::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::END_ROUNDCALL:	// ラウンドコールが終わったというメッセージが届いたら
	//	pMain->GetFSM()->ChangeState(Main::GetInstance());
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}