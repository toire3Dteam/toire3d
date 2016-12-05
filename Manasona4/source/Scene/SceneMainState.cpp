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
#include "SceneSelect.h"
#include "Data\/SelectData.h"

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
	EffectCameraInfo.scriptID = (int)pMain->GetStage()->GetIntroCameraScriptID();
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

void SceneMainState::Main::Enter(sceneMain *pMain)
{
	// フェード初期化
	if (Fade::m_alpha >= 128) Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00000000);

	// タイマー始動
	GameUIMgr->TimerStart();
}

void SceneMainState::Main::Execute(sceneMain *pMain)
{
	// (TODO) 0秒になったらタイムアップ！！
	if (GameUIMgr->isTimerUp())
	{

		//return;
	}

	// 必殺ステートに誰かが入ったら止める (TODO) KOフラグとかあればifで囲みたい
	if (PlayerMgr->GetPlayer(SIDE::LEFT)->isHeavehoDriveState() ||
		PlayerMgr->GetPlayer(SIDE::RIGHT)->isHeavehoDriveState())
	{
		GameUIMgr->TimerStop();
	}
	else
	{
		GameUIMgr->TimerStart();
	}

	// プレイヤー更新
	PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);

	// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
	Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

	// プレイヤー位置確定
	PlayerMgr->UpdatePos();

	// カメラ更新
	CameraMgr->Update();



	// タイマー更新&タイマーが終了なら終了ステートに行く
	//if (TimeMgr->Update())
	//{
	//	//pMain->GetFSM()->ChangeState(Finish::GetInstance());
	//}

	//+--------------------------------------------
	//	ポーズメニューの操作
	//+--------------------------------------------

	// パッド分更新
	const int NumDevice(tdnInputManager::GetNumDevice());

	// パッド何もささってないとき用
	if (NumDevice == 0)
	{
		// メニュー切り替え
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, 0) == 3)
		{
			pMain->GetFSM()->ChangeState(PauseMenu::GetInstance());
			return;
		}

	}else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			// メニュー切り替え
			if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, i) == 3)
			{
				pMain->GetFSM()->ChangeState(PauseMenu::GetInstance());
				return;
			}
		}
	}

}
void SceneMainState::Main::Exit(sceneMain *pMain)
{
	// タイマーストップ
	GameUIMgr->TimerStop();

}
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
	{
		pMain->GetFSM()->ChangeState(HeaveHoDriveOverFlowSuccess::GetInstance());
		
		// 最低HP保存
		GameUIMgr->GetHpGage((SIDE)((int)msg.Sender - (int)ENTITY_ID::ID_PLAYER_FIRST))->JudeRemainingHP();

		// シームレススレッド発動
		ResultData data;// リザルトに必要なデータをここで詰める
		data.eWinnerSide = PlayerMgr->GetPlayer(msg.Sender)->GetSide();
		data.iMaxDamage = 10000;
		data.iRemainingHP= PlayerMgr->GetPlayer(msg.Sender)->GetHPPercentage();
		data.iElapsedTime = GameUIMgr->GetTimer()->GetElapsedTime();

		LoadSceneThreadMgr->Initialize(new sceneResult(data));
		return true;
	}	break;
	case MESSAGE_TYPE::KO:	// 誰かのHPが0になったら切り替え
	{
		KO_INFO *KOInfo = (KO_INFO*)msg.ExtraInfo;

		// 最低HP保存
		GameUIMgr->GetHpGage(KOInfo->WinnerSide)->JudeRemainingHP();

		// シームレススレッド発動
		ResultData data;// リザルトに必要なデータをここで詰める
		data.eWinnerSide = KOInfo->WinnerSide;
		data.iMaxDamage = GameUIMgr->GetComboUI(KOInfo->LoseSide)->GetMaxDamage();		
		data.iRemainingHP =GameUIMgr->GetHpGage(KOInfo->WinnerSide)->GetHPPercentage();
		data.iElapsedTime = GameUIMgr->GetTimer()->GetElapsedTime();

		// ラウンド全部取って勝利確定だったら、フィニッシュの段階でリザルトシーンを読み込む
		if (KOInfo->iNumWinnerRound == pMain->GetRoundNum() - 1)
		{
			LoadSceneThreadMgr->Initialize(new sceneResult(data));
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
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

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
					pMain->Reset();

					pMain->GetFSM()->ChangeState(Round::GetInstance());
					return;
				}
			}
		}
		else
		{
			// ここに来たということは、たぶんトレーニングで敵を倒した場合
			if (Fade::isFadeOutCompletion())
			{
				//チュートリアル途中でやめてゲームしたらバグる
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
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

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

	// 毎回チュートリアル設定！
	TutorialMgr->Init(pMain->GetSelectTutorial(), SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID);
	TutorialMgr->GetTutorial()->ActionIntroTips();// イントロのTipsを用意


	// 情報初期化
	pMain->Reset();

	//  チュートリアル毎に必要な設定をここでしてあげる
	if (pMain->GetSelectTutorial() == TUTORIAL_TYPE::OVER_DRIVE)
	{
		PlayerMgr->GetPlayerByDeviceID
			(SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID)->SetOverDriveGage(100);
	}
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

	// ★TODO　ボタン　が履歴が残ったままなので　はいを選択したらそのまま攻撃してしまう
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
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

		// プレイヤー位置確定
		PlayerMgr->UpdatePos();
	}
	else
	{
		// 通常
		PlayerMgr->Update(PLAYER_UPDATE::CONTROL_OK);

		// プレイヤーといろいろ判定(★ここに書いた理由はショットマネージャーとかステージをsceneMainが持っているから)
		Collision::PlayerCollision(PlayerMgr, pMain->GetShotManager());

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

		// ポーズでチュートリアル専用のポーズメニューへ
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
		{
			pMain->GetFSM()->ChangeState(TutorialPauseMenu::GetInstance());
			return;
		}

		// セレクトでもう一度イントロへ戻る
		if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID) == 3)
		{
			// フェードアウト
			Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

			// (TODO)必殺の時にワープするとバグる
			
			return;
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

	if (Fade::isFadeOutCompletion() == false)
	{
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


/*******************************************************/
//			ポーズメニューステート
/*******************************************************/

void SceneMainState::PauseMenu::Enter(sceneMain *pMain)
{	
	// ポーズウィンドウ起動 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Action();
	pMain->SetPause(true);

}

// 更新
void SceneMainState::PauseMenu::Execute(sceneMain *pMain)
{

	//+--------------------------------------------
	//	ポーズメニューの操作
	//+--------------------------------------------
	// パッド分更新
	const int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)
	{
		// ポーズウィンドウの操作
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Ctrl(0);
	}
	else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			// ポーズウィンドウの操作
			pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Ctrl(i);
		}
	}



	// プレイヤー更新を止める
	//PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	//PlayerMgr->UpdatePos();

	// カメラ更新
	//CameraMgr->Update();

	// 戻るボタンを押したら
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() == PauseWindow::BATTLE_STATE::BACK)
	{
		// ポーズウィンドウ止める
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();

		//// ★ここで状態に応じて分岐
		//// チュートリアル
		//if (pMain->GetTutorialFlag())
		//{
		//	pMain->GetFSM()->ChangeState(TutorialMain::GetInstance());	// チュートリルへ戻る
		//	
		//}
		//else
		//{
			pMain->GetFSM()->ChangeState(Main::GetInstance());	// メインへ戻る

		//}

	}

	// キャラクターセレクトへ
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() == PauseWindow::BATTLE_STATE::BACK_CHARA_SELECT)
	{
		// ポーズウィンドウ止める
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneSelect());
		return;
	}

	// メニューへ
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->GetChoiceState() == PauseWindow::BATTLE_STATE::BACK_MENU_SELECT)
	{
		// ポーズウィンドウ止める
		pMain->GetWindow(BATTLE_WINDOW_TYPE::PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneMenu());
		return;
	}




}

void SceneMainState::PauseMenu::Exit(sceneMain *pMain)
{
	pMain->SetPause(false);
}

void SceneMainState::PauseMenu::Render(sceneMain *pMain)
{

}

bool SceneMainState::PauseMenu::OnMessage(sceneMain *pMain, const Message & msg)
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
//			チュートリアルポーズメニューステート
/*******************************************************/

void SceneMainState::TutorialPauseMenu::Enter(sceneMain *pMain)
{
	// チュートリアルポーズウィンドウ起動 
	pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Action();
	pMain->SetPause(true);

}

// 更新
void SceneMainState::TutorialPauseMenu::Execute(sceneMain *pMain)
{

	//+--------------------------------------------
	//	チュートリアルポーズメニューの操作
	//+--------------------------------------------
	// パッド分更新
	const int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)
	{
		// ポーズウィンドウの操作
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Ctrl(0);
	}
	else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			// ポーズウィンドウの操作
			pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Ctrl(i);
		}
	}



	// プレイヤー更新を止める
	//PlayerMgr->Update(PLAYER_UPDATE::CONTROL_NO);
	//PlayerMgr->UpdatePos();

	// カメラ更新
	//CameraMgr->Update();

	// 一つ前のチュートリアルへ戻るボタンを押したら
	//if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK_PREV_TUTORIAL)
	//{
	//	// ポーズウィンドウ止める
	//	pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();

	//	pMain->GetFSM()->ChangeState(TutorialIntro::GetInstance());	

	//}

	// 戻るボタンを押したら
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK)
	{
		// ポーズウィンドウ止める
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();

		pMain->GetFSM()->ChangeState(TutorialMain::GetInstance());	// チュートリアルへ戻る

	}

	// メニューへ
	if (pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->GetChoiceState() == TutorialPauseWindow::TUTORIAL_PAUSE_STATE::BACK_MENU_SELECT)
	{
		// ポーズウィンドウ止める
		pMain->GetWindow(BATTLE_WINDOW_TYPE::TUTORIAL_PAUSE)->Stop();
		MainFrameEx->ChangeScene(new sceneMenu());
		return;
	}
	
}

void SceneMainState::TutorialPauseMenu::Exit(sceneMain *pMain)
{
	pMain->SetPause(false);
}

void SceneMainState::TutorialPauseMenu::Render(sceneMain *pMain)
{

}

bool SceneMainState::TutorialPauseMenu::OnMessage(sceneMain *pMain, const Message & msg)
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