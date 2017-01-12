#include "TrainingPauseWindow.h"

#include "PauseWindow.h"

//+------------------------
//  チュートリアルポーズウィンドウ
//+------------------------

TrainingPauseWindow::TrainingPauseWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/PauseWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/PauseWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//選択アイコンの長さ
	m_iSelectLength = 34;

	// ポーズウィンドウに存在するアイコンを詰めていく
	//AddIconData("前のチュートリアルへ", "一つ前のチュートリアルに戻ります。");
	AddIconData("トレーニング設定", "トレーニングの基本設定を行います。");
	AddIconData("トレーニングダミー設定", "トレーニングダミーの設定を行います。");
	AddIconData("ポジションリセット","ポジションを初期状態に戻します。");
	AddIconData("コマンドリスト", "操作しているキャラクターのコマンドリストを確認します。");
	AddIconData("サウンド設定", "各種サウンドの設定を行います。");
	AddIconData("メニュー非表示", "メニュー画面を非表示にします。");
	AddIconData("キャラクターセレクト", "キャラクターセレクトに戻ります。");
	AddIconData("メインメニュー", "メインメニューに戻ります。");
	AddIconData("戻る", "このウィンドウを閉じ、ゲームに戻ります。");

	// 演出
	//m_iAlpha = 0;
	//m_iAddX = 0;
	//m_iAddY = 0;

	m_pInfoPlate = new InformationPlate();


	// 二回戻り防止
	m_bBackPush = false;
}

TrainingPauseWindow::~TrainingPauseWindow()
{
	SAFE_DELETE(m_pInfoPlate);

}


bool TrainingPauseWindow::Update()
{
	// 説明の下に乗せる板
	m_pInfoPlate->Update();

	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 更新だけほかの子のWindowが起動していたら
	// 操作できないようにする。

	m_pSelect->Update();

	// 透明度更新
	m_iAlpha += 255 / 4;
	m_iAlpha = min(255, m_iAlpha);

	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新							 // 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新


	return true;// 起動中
}

void TrainingPauseWindow::Redner()
{
	// 説明の下に乗せる板
	m_pInfoPlate->Render();

	// 起動していなかったらハジく
	if (m_bActive == false)return;

	// タイトル
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ★基本的なウィンドウの位置
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// 中身をブロック単位で描画
	int y = (int)(m_vPos.y + 64.0f);
	int x = (int)m_vPos.x + 18 + (255 - m_iAlpha);
	for (int i = 0; i < (int)m_aIconData.size(); i++)
	{

		// 最後尾ならしっぽを描画
		if (i == (int)m_aIconData.size() - 1)
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 46, 0, 64 + 40, SizeX, 46);
		}
		else// 中
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 40, 0, 64, SizeX, 40);
		}



		//+-----------------------
		// 選択している番号なら
		if (i == m_iSelectNo)
		{
			// (TUDO) 選択している所を目立たせる演出

			//+-----------------------
			// 選択してる黄色い長い枠
			for (int j = 0; j < m_iSelectLength; j++)
			{
				m_pSelect->Render(x + (j * 11) - 4, y, 32, 32, 0, 32, 32, 32);
			}

		}

		//+-----------------------
		// 枠
		for (int j = 0; j < m_aIconData[i].iStringLength; j++)
		{
			m_pFontBox->Render(x + (j * 12) - 4, y, 32, 32, 0, 0, 32, 32);

			// 選択しているタスクなら
			if (m_iSelectNo == i)
			{
				m_pFontBox->Render(x + (j * 12) - 4, y,
					32, 32, 0, 32, 32, 32);
			}

		}

		// 文字
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);


		// 40ピクセルずらして描画していく
		y += 40;
	}

	//// 文字
	//DWORD fontCol = 0xffffffff;
	////if (m_iSelectNo == i)fontCol = 0xff030a58;
	//tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS創英角ｺﾞｼｯｸUB",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
	//	24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
}

// 操作
bool  TrainingPauseWindow::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 操作音
	CtrlSE(DeviceID);

	// 決定　ゲームに戻る選択肢ならば離して進むように
	if ((TRAINING_PAUSE_STATE)m_iSelectNo == TRAINING_PAUSE_STATE::BACK || 
		(TRAINING_PAUSE_STATE)m_iSelectNo == TRAINING_PAUSE_STATE::POSITION_RESET)
	{
		// ゲームに戻る動作は離して進む
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
		{
			// 選択した番号格納！
			m_iChoiceState = (TRAINING_PAUSE_STATE)m_iSelectNo;
			return true;
		}
	}
	else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// 選択した番号格納！
			m_iChoiceState = (TRAINING_PAUSE_STATE)m_iSelectNo;
			return true;
		}
	}

	// キャンセルボタンを押し込む動作が必要に
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3||
		tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 3)
	{
		m_bBackPush = true;
	}

	// キャンセルボタン
	if (m_bBackPush == true)// 一度キャンセルボタンを押し込んでいたら
	{
		// ゲームに戻る動作は離して進む
		if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 2)
		{
			// 選択した番号格納！
			m_iChoiceState = (TRAINING_PAUSE_STATE::BACK);
			return true;
		}
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 2)
		{
			// 選択した番号格納！
			m_iChoiceState = (TRAINING_PAUSE_STATE::BACK);
			return true;
		}
	}

	// Bでもどったらりせっとになる
	//	とりま完成
	//	あしたはWindowの設定とか
	//	セーブデータにトレーニングようのでーたつくってもらう

	// 決定やキャンセルを押してない時
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0 && 
	tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 0)
	{

		// 選択切り替え
		if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
		{
			m_iSelectNo--;
			// 選択時の演出
			m_pSelect->Action();
			// 最後へ進む
			if (m_iSelectNo <= -1)
			{
				m_iSelectNo = ((int)m_aIconData.size() - 1);
			}
		}
		if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
		{
			m_iSelectNo++;
			// 選択時の演出
			m_pSelect->Action();
			// 最初に戻る
			if (m_iSelectNo >= (int)m_aIconData.size())
			{
				m_iSelectNo = 0;
			}
		}

	}


	return false;

}

void TrainingPauseWindow::Action()
{
	if (m_bActive == false)// もうウィンドウが出ているときは演習をハジク
	{
		// 透明度を最初は透明に
		m_iAlpha = 0;
		m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
		m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
		m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新

		m_pInfoPlate->Action();// (TODO) ベースで
	}

	BaseWindow::Action();

	// 二回戻り防止
	m_bBackPush = false;
}

void TrainingPauseWindow::Stop()
{
	BaseWindow::Stop();

	m_pInfoPlate->Stop();

	// 二回戻り防止
	m_bBackPush = false;
}