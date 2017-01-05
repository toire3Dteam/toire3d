#include "TDNLIB.h"
#include "../Sound/SoundManager.h"
#include "system/FrameworkEx.h"
#include "../Data/PlayerData.h"
#include "SceneMenu.h"
#include "SceneCollect.h"
#include "../Fade/Fade.h"
#include "Trophy\TrophyManager.h"


//=============================================================================================
//		初	期	化	と	開	放
sceneCollect::sceneCollect() :BaseGameEntity(ENTITY_ID::SCENE_COLLECT) {}

bool sceneCollect::Initialize()
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	m_fLoadPercentage = .5f;	// ロード割合

	// 画像初期化
	m_pImages[IMAGE::BACK] = new tdn2DAnim("DATA/UI/Collect/bg.png");
	m_pImages[IMAGE::SCROLL_BAR] = new tdn2DAnim("DATA/UI/Collect/ScrollBar.png");
	m_pImages[IMAGE::SCROLL_ARROW] = new tdn2DAnim("DATA/UI/Collect/ScrollArrow.png");
	m_pImages[IMAGE::TITLE] = new tdn2DAnim("DATA/UI/Collect/title.png");
	m_pImages[IMAGE::INFO_PLATE] = new tdn2DAnim("DATA/UI/Collect/Information.png");
	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i]->OrderNone();
		m_pImages[i]->Action();
	}
	m_pImages[IMAGE::TITLE]->OrderMoveAppeared(16, 0, -200);
	m_pImages[IMAGE::INFO_PLATE]->OrderMoveAppeared(24, 0, 592 + (200 ));

	m_fLoadPercentage = 1.0f;	// ロード割合

	m_pPlayerInfo = &PlayerDataMgr->m_PlayerInfo;

	bgm->PlayStreamIn("DATA/Sound/BGM/System/Collect.ogg");

	// スクロールバー
	m_iScrollHeight = 1440;
	m_iScrollScreenSize= 720/2;
	m_vScrollPos = VECTOR2_ZERO;
	m_vScrollPos.x = 1200;
	m_vScrollPos.y = 128;

	// トロフィーアイコン構造体
	TrophyInit();

	// 操作関連
	m_iSpeedLv = 0;
	m_iUpPushFrame = 0;
	m_iDownPushFrame = 0;
	m_iLeftPushFrame = 0;
	m_iRightPushFrame = 0;

	m_bUpPush = false;
	m_bDownPush = false;
	m_bLeftPush = false;
	m_bRightPush = false;

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneCollect>(this);
	m_pStateMachine->SetCurrentState(SceneCollectState::Intro::GetInstance());	// ステートの設定




	return true;
}

sceneCollect::~sceneCollect()
{
	bgm->StopStreamIn();
	FOR(IMAGE::ARRAY_END) SAFE_DELETE(m_pImages[i]);
	
	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_tagTI.pSelectMark);
	SAFE_DELETE(m_tagTI.pSelectMarkEdge);
}
//=============================================================================================


//=============================================================================================
//		更			新
void sceneCollect::Update()
{
	static bool bEnd(false);

	// 更新
	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i]->Update();
	}

	// フェード
	Fade::Update();


	//if (KEY(KEYCODE::KEY_DOWN, 0) == 1)
	//{
	//	m_iScrollPosY -= 10;
	//}

	//if (KEY(KEYCODE::KEY_UP, 0) == 1)
	//{
	//	m_iScrollPosY += 10;
	//}

	//if (KEY(KEYCODE::KEY_SPACE, 0) == 1)
	//{
	//	m_iScrollPosY = 720;
	//}
	//if (KEY(KEYCODE::KEY_C, 0) == 1)
	//{
	//	m_iScrollPosY = m_iScrollHeight;
	//}

	// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
	m_pStateMachine->Update();



#ifdef _DEBUG

	if (bEnd)
	{
		// フェード終わったら帰る
		if (Fade::isFadeStop())
		{
			bEnd = false;
			MainFrameEx->ChangeScene(new sceneMenu);
		}
	}
	// スペースキーで抜ける
	else if (KEY(KEYCODE::KEY_ENTER, 0) == 3)
	{
		//se->Play("戻る");

		// フェードアウト設定
		Fade::Set(Fade::FLAG::FADE_OUT, 4);
		bEnd = true;
	}

#endif // _DEBUG


}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneCollect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// 背景
	m_pImages[IMAGE::BACK]->Render(0,0);

	// tdnPolygon::Rect((int)(1280 * .25f) / 2, 96, (int)(1280 * .75f), (int)(720 * .75f), RS::COPY, 0xff808080);

	// プレイ回数
	tdnText::Draw(400, 400, 0xffffffff, "プレイ回数: %d", m_pPlayerInfo->PlayCount);

	// プレイ時間
	const int minutes(m_pPlayerInfo->PlayTime % 60), hour(m_pPlayerInfo->PlayTime / 60);
	tdnText::Draw(400, 460, 0xffffffff, "プレイ時間: %d時間 %d分", hour, minutes);

	// コイン
	tdnText::Draw(400, 520, 0xffffffff, "コイン: %d", m_pPlayerInfo->coin);
	
	// 
	//tdnText::Draw(400, 720, 0xffffffff, "下げてる値: %.1f", m_iScrollPosY);

	
	// ★ここにステートマシン描画(多分2D関係が多いんじゃないかと)
	m_pStateMachine->Render();

	m_pImages[IMAGE::TITLE]->Render(0, 0);


	// 明日は新しく追加でTrophy用のInfoMationを描く
	//そんでとりあえず選択アイコンは作れたからステートでCtrl関数作って
	//	 選択を動かせるように作る。移動処理はメニュー画面からコピペ

	//	 終わればさいごにすくろオール処理　RenderRoomで案ロックならばロックへにチェンジも忘れずに

#ifdef _DEBUG

	tdnText::Draw(10, 200, 0xffffffff, "列数%d", m_tagTI.iRowNum);
	tdnText::Draw(10, 250, 0xffffffff, "最大の高さ%d", m_tagTI.iMaxHeight);
	tdnText::Draw(10, 300, 0xffffffff, "現在のトップ%d", m_tagTI.iTop);
	tdnText::Draw(10, 350, 0xffffffff, "ボトム幅%d", m_tagTI.iBottom);
	tdnText::Draw(10, 400, 0xffffffff, "選択No%d", m_tagTI.iSelectNo);

#endif // _DEBUG



	// フェード
	Fade::Render();
}

void sceneCollect::FirstAction()
{
	m_pImages[IMAGE::TITLE]->Action();
	m_pImages[IMAGE::INFO_PLATE]->Action(6);
}

// ピピピ操作
void sceneCollect::PiPiPiCtrl(int iDeviceID)
{


	// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッのための処理
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDeviceID) == 1)
	{
		m_iLeftPushFrame++;
		if (m_iSpeedLv == 0 && m_iLeftPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iLeftPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;
		m_iDownPushFrame = 0;
		m_iUpPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDeviceID) == 1)
	{
		m_iRightPushFrame++;
		if (m_iSpeedLv == 0 && m_iRightPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iRightPushFrame >= 77)m_iSpeedLv = 2;

		m_iDownPushFrame = 0;
		m_iLeftPushFrame = 0;
		m_iUpPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_UP, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_UP, iDeviceID) == 1)
	{
		m_iUpPushFrame++;
		if (m_iSpeedLv == 0 && m_iUpPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iUpPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;
		m_iDownPushFrame = 0;
		m_iLeftPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDeviceID) == 1)
	{
		m_iDownPushFrame++;
		if (m_iSpeedLv == 0 && m_iDownPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iDownPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;
		m_iLeftPushFrame = 0;
		m_iUpPushFrame = 0;

	}
	else
	{
		m_iLeftPushFrame = 0;
		m_iRightPushFrame = 0;
		m_iUpPushFrame = 0;
		m_iDownPushFrame = 0;
		m_iSpeedLv = 0;
	}

	//+------------------------------------------------
	//		押したときの判定
	//+------------------------------------------------

	int iInterval = 16;
	if (m_iSpeedLv == 1)iInterval = 6;
	if (m_iSpeedLv == 2)iInterval = 4;

	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDeviceID) == 3 ||
		(m_iLeftPushFrame % (iInterval) == 0 && m_iLeftPushFrame != 0))
	{
		// 左押した
		m_bLeftPush = true;
		// 押した音 
		se->Play("カーソル2");
		// 左の矢印アニメ

	}
	else
	{
		// 押してなかったらフラグをOFF
		m_bLeftPush = false;
	}



	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDeviceID) == 3 ||
		(m_iRightPushFrame % (iInterval) == 0 && m_iRightPushFrame != 0))
	{
		// 右押した
		m_bRightPush = true;
		// 押した音 
		se->Play("カーソル2");
		// 右の矢印アニメ

	}
	else
	{
		// 押してなかったらフラグをOFF
		m_bRightPush = false;
	}

	if (tdnInput::KeyGet(KEYCODE::KEY_UP, iDeviceID) == 3 ||
		(m_iUpPushFrame % (iInterval) == 0 && m_iUpPushFrame != 0))
	{
		// 上押した
		m_bUpPush = true;
		// 押した音 
		se->Play("カーソル2");

	}
	else
	{
		// 押してなかったらフラグをOFF
		m_bUpPush = false;
	}

	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDeviceID) == 3 ||
		(m_iDownPushFrame % (iInterval) == 0 && m_iDownPushFrame != 0))
	{
		// 下押した
		m_bDownPush = true;
		// 押した音 
		se->Play("カーソル2");

	}
	else
	{
		// 押してなかったらフラグをOFF
		m_bDownPush = false;
	}


}

//------------------------------------------------------
//	トロフィー
//------------------------------------------------------

void sceneCollect::TrophyInit()
{

	//　初期の先頭
	m_tagTI.iTop = 0;
	m_tagTI.iBottom = 2;
	
	// 何列かを設定
	m_tagTI.iRowNum = 6;
	// トロフィー数分に↑列を合わせた結果、縦幅の大きさを調べる
	int l_iCount = 0;
	m_tagTI.iMaxHeight = 1;// ★まず最低限1はある
	// +アイコンの初期位置も決める
	Vector2 l_vIconPos = VECTOR2_ZERO;
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		// カウントが列を超えた時
		if (l_iCount >= m_tagTI.iRowNum)
		{
			m_tagTI.iMaxHeight++;// 最大幅更新			
			l_iCount = 0;

			l_vIconPos.y += 156;
			l_vIconPos.x = 0;
		}

		// ポジション設定！！
		TrophyMgr->GetTrophy(i)->SetRoomPos(l_vIconPos);

		// 横にずらす
		l_vIconPos.x += 156;

		l_iCount++;
	}

	// 次に初期に選択している番号
	m_tagTI.iSelectNo = 0;
	// ★↑その選択している番号から初期ポジション取得
	m_tagTI.vSelectPos = TrophyMgr->GetTrophy(m_tagTI.iSelectNo)->GetRoomPos();
	m_tagTI.vNextPos= TrophyMgr->GetTrophy(m_tagTI.iSelectNo)->GetRoomPos();

	// マーク絵
	m_tagTI.pSelectMark = new tdn2DAnim("Data/Trophy/Select.png");
	//m_tagTI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagTI.pSelectMark->OrderAlphaMove(210, 90, 120);
	//m_tagTI.pSelectMark->OrderNone();
	m_tagTI.pSelectMark->Action();

	m_tagTI.pSelectMarkEdge = new tdn2DAnim("Data/Trophy/SelectEdge.png");
	//m_tagTI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagTI.pSelectMarkEdge->OrderNone();
	m_tagTI.pSelectMarkEdge->Action();

	// スクロール初期位置
	m_tagTI.vScrollPos		= VECTOR2_ZERO;
	m_tagTI.vScrollNextPos	= VECTOR2_ZERO;

}

void sceneCollect::TrophyUpdate()
{
	// 選択アイコンの画像の動く処理
	
	// 目的地更新
	m_tagTI.vNextPos = TrophyMgr->GetTrophy(m_tagTI.iSelectNo)->GetRoomPos();

	// 場所補間の更新
	m_tagTI.vSelectPos = m_tagTI.vNextPos * 0.5f + m_tagTI.vSelectPos  * 0.5f;

	// 0.1以下の誤差は無くす処理
	if (0.9f >= abs(m_tagTI.vSelectPos.x - m_tagTI.vNextPos.x) &&
		0.9f >= abs(m_tagTI.vSelectPos.y - m_tagTI.vNextPos.y))
	{
		m_tagTI.vSelectPos = m_tagTI.vNextPos;
	}

	//+------------------------------
	//	スクロール処理
	//+------------------------------
	// 現在の選択してるトロフィーの高さ
	int l_iNowHeight = m_tagTI.iSelectNo / m_tagTI.iRowNum;

	// 上と同じ場所・それ以上なら
	if (l_iNowHeight <= m_tagTI.iTop)
	{
		// 一番上以外なら上へ
		if (m_tagTI.iTop >= 1)
		{
			m_tagTI.iTop--;
		}

	}

	// 下へと同じ場所・それ以上なら
	// ★現在のボトムを取得
	int m_iNowBottom = m_tagTI.iTop + m_tagTI.iBottom;

	if (l_iNowHeight >= m_iNowBottom)
	{
		// 一番下以外なら
		if (m_tagTI.iTop < m_tagTI.iMaxHeight - (m_tagTI.iBottom + 1))
		{
			m_tagTI.iTop++;
		}
	}

	// 目的地更新
	m_tagTI.vScrollNextPos.y = (float)(m_tagTI.iTop * 156);

	// 場所補間の更新
	m_tagTI.vScrollPos = m_tagTI.vScrollNextPos * 0.5f + m_tagTI.vScrollPos* 0.5f;

	// 0.1以下の誤差は無くす処理
	if (0.9f >= abs(m_tagTI.vScrollPos.x - m_tagTI.vScrollNextPos.x) &&
		0.9f >= abs(m_tagTI.vScrollPos.y - m_tagTI.vScrollNextPos.y))
	{
		m_tagTI.vScrollPos = m_tagTI.vScrollNextPos;
	}

	// 選択アイコン
	m_tagTI.pSelectMark->Update();
	if (m_tagTI.pSelectMark->GetAction()->IsEnd() == true)
	{
		m_tagTI.pSelectMark->Action();
	}

	m_tagTI.pSelectMarkEdge->Update();

}

void sceneCollect::TrophyRender()
{
	// (仮)　アイコンの絵
	int l_iX = 200;
	int l_iY = 100 - (int)m_tagTI.vScrollPos.y;

	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		// カウントが列を超えた時
		//if (l_iCount >= m_tagTI.iRowNum)
		//{
		//	l_iY += 156;
		//	l_iX = 100;

		//	l_iCount = 0;
		//}

		//l_iX += 156;

		// 生でLib描画　後でスケールやアルファなどもろもろ加工したレンダーを作る
		//TrophyMgr->GetTrophy(i).pIcon->Render(l_iX, l_iY);
		TrophyMgr->RenderRoom(i, l_iX, l_iY);

		//l_iCount++;
	}

	//tdnText::Draw(m_tagTI.vSelectPos.x + l_iX, m_tagTI.vSelectPos.y + l_iY, 0xffff00ff, "選択中！！");
	// セレクトアイコン
	m_tagTI.pSelectMark->Render((int)m_tagTI.vSelectPos.x + l_iX, (int)m_tagTI.vSelectPos.y + l_iY,RS::ADD);
	m_tagTI.pSelectMarkEdge->Render((int)m_tagTI.vSelectPos.x + l_iX, (int)m_tagTI.vSelectPos.y + l_iY, RS::ADD);

	// 説明用プレート
	m_pImages[IMAGE::INFO_PLATE]->Render(0, 592 - 128);

	// トロフィーのタイトルと説明
	TrophyMgr->RenderInfo(m_tagTI.iSelectNo, 60, 580);

	//tdnFont::RenderString(TrophyMgr->GetTrophyData(m_tagTI.iSelectNo).sTitle.c_str(), "HGｺﾞｼｯｸE",
	//	22, 60, 580, 0xffffffff, RS::COPY);
	//// 
	//tdnFont::RenderString(TrophyMgr->GetTrophyData(m_tagTI.iSelectNo).sText.c_str(), "HGｺﾞｼｯｸE",
	//	19,  60, 610, 0xffffffff, RS::COPY);


}

// 今日はここまで
// あしたは裏の動画作ったり
// トロフィーの演出シーン変わったら消したり
// 残りのユーザーインターフェースやる
// ×でアイコンが出てきてメニューに戻るとかのシーンステート関連

void sceneCollect::TrophyCtrl(int iDeviceID)
{
	// 基本ぴぴぴ操作
	 PiPiPiCtrl(iDeviceID);

	// 左押してたら
	if ( m_bLeftPush)
	{
		 m_tagTI.iSelectNo--;
		 if (m_tagTI.iSelectNo < 0)
		 {
			 m_tagTI.iSelectNo = TrophyMgr->GetMaxTrophyNum() - 1;
		 }

		 // 移動演出
		 m_tagTI.pSelectMark->Action();

	}
	// 右押してたら
	if ( m_bRightPush)
	{
		 m_tagTI.iSelectNo++;
		 if (m_tagTI.iSelectNo >= TrophyMgr->GetMaxTrophyNum())
		 {
			 m_tagTI.iSelectNo = 0;
		 }

		 // 移動演出
		 m_tagTI.pSelectMark->Action();

	}
	// 上押してたら
	if ( m_bUpPush)
	{
		int l_iNo = m_tagTI.iSelectNo - m_tagTI.iRowNum;
		if (l_iNo < 0)
		{
			// 貫通した場合
			// 列を調べる
			int l_iColumn = m_tagTI.iSelectNo%m_tagTI.iRowNum;
			
			// 列文まわす
			int l_iJumpNo = TrophyMgr->GetMaxTrophyNum() - 1;// 一番最後の数字取得
			for (int i = 0; i < m_tagTI.iRowNum; i++)
			{
				// 列が同じか調べる
				if (l_iColumn == l_iJumpNo%m_tagTI.iRowNum)
				{
					// 同じならばその列の場所へ飛ばす
					m_tagTI.iSelectNo = l_iJumpNo;
				}
			
				// なければ次の場所へ
				l_iJumpNo--;
			
			}

		}
		else
		{
			// 列分増減
			m_tagTI.iSelectNo -= m_tagTI.iRowNum;
		}

		// 移動演出
		m_tagTI.pSelectMark->Action();

	}
	// 下押してたら
	if ( m_bDownPush)
	{
		int l_iNo = m_tagTI.iSelectNo + m_tagTI.iRowNum;
		if (l_iNo >= TrophyMgr->GetMaxTrophyNum())
		{
			// 貫通した場合
			// 列を調べる
			int l_iColumn = m_tagTI.iSelectNo%m_tagTI.iRowNum;
			// その列の場所へワープ
			m_tagTI.iSelectNo = l_iColumn;

		}
		else
		{

			// 列分増減
			m_tagTI.iSelectNo += m_tagTI.iRowNum;
		}

		// 移動演出
		m_tagTI.pSelectMark->Action();


	}

	


}

//
//=============================================================================================

// トロフィーのアイコンの情報が詰まった構造体の初期化
sceneCollect::TrophyIconDesc::TrophyIconDesc()
{
	iTop = 0;
	iBottom = 0;
	iMaxHeight = 0;
	iRowNum = 0;

	iSelectNo = 0;
	vSelectPos = VECTOR2_ZERO;
	vNextPos = VECTOR2_ZERO;

	pSelectMark = nullptr;
	pSelectMarkEdge = nullptr;

	vScrollPos = VECTOR2_ZERO;
	vScrollNextPos = VECTOR2_ZERO;

}
