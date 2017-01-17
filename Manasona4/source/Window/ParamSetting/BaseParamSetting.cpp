#include "BaseParamSetting.h"
#include "Sound\SoundManager.h"



//+-----------------------
//	基本のパラメーター設定UI
//+-----------------------

BaseParamSetting::BaseParamSetting(int Number, int WidthSize)
{
	// ID番号・横幅入力
	m_iNumberID = Number;
	m_iWidthSize = WidthSize;

	m_pLeftArrow = new tdn2DAnim("Data/UI/Window/Arrow.png");
	m_pLeftArrow->OrderJump(4, 1.0f, 0.30f);
	m_pRightArrow = new tdn2DAnim("Data/UI/Window/Arrow.png");
	m_pRightArrow->OrderJump(4, 1.0f, 0.30f);

	m_pPersentFrame = new tdn2DObj("Data/UI/Window/PersentFrame2.png");
	m_pPersentGage = new tdn2DObj("Data/UI/Window/persentgage2.png");

	m_pNumber = new tdn2DObj("Data/UI/Window/number2.png");

	// 操作関連
	m_iSpeedLv = 0;
	m_iLeftPushFrame = 0;
	m_iRightPushFrame = 0;
	m_bLeftPush = false;
	m_bRightPush = false;


}

BaseParamSetting::~BaseParamSetting()
{
	SAFE_DELETE(m_pLeftArrow);
	SAFE_DELETE(m_pRightArrow);
	SAFE_DELETE(m_pPersentFrame);
	SAFE_DELETE(m_pPersentGage);
	SAFE_DELETE(m_pNumber);
}

void BaseParamSetting::Update()
{
	// アニメーションの更新
	m_pLeftArrow->Update();
	m_pRightArrow->Update();
}

void BaseParamSetting::Render(int x, int y, bool selectFlag)
{
	// 矢印描画
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	m_pRightArrow->Render(x + m_iWidthSize , y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右


}

void BaseParamSetting::RenderNumber(int x, int y, int num, bool bSelect)
{
	int number = num;

	int size = 27;

	for (int count = 0;; ++count)
	{
		int digitNum = number % 10;	// 一番小さい桁を入手
		number = (int)(number / 10);// 数値の一番小さい桁を消す

		m_pNumber->Render(x - (count * ((size / 2) + 4)), y, size, 32, digitNum * size, (bSelect*32), size, 32);// 数字描画

		if (number <= 0)break;// 数値が０以下になったらさよなら
	}
}

// 操作
void BaseParamSetting::Ctrl(int DeviceID)
{

	// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッのための処理
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 1)
	{
		m_iLeftPushFrame++;
		if (m_iSpeedLv == 0 && m_iLeftPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iLeftPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 1)
	{	
		m_iRightPushFrame++;
		if (m_iSpeedLv == 0 && m_iRightPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iRightPushFrame >= 77)m_iSpeedLv = 2;
	
		m_iLeftPushFrame = 0;

	}else
	{
		m_iLeftPushFrame = 0;
		m_iRightPushFrame = 0;
		m_iSpeedLv = 0;
	}

	//+------------------------------------------------
	//		押したときの判定
	//+------------------------------------------------
	
	int iInterval = 16;
	if (m_iSpeedLv == 1)iInterval = 3;
	if (m_iSpeedLv == 2)iInterval = 1;

	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3 ||
		(m_iLeftPushFrame % (iInterval) == 0 && m_iLeftPushFrame != 0))
	{
		// 左押した
		m_bLeftPush = true;
		// 押した音 
		se->Play("カーソル2");
		// 左の矢印アニメ
		m_pLeftArrow->Action();
	
	}else
	{
		// 押してなかったらフラグをOFF
		m_bLeftPush = false;
	}



	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3 ||
		(m_iRightPushFrame % (iInterval) == 0 && m_iRightPushFrame != 0))
	{
		// 右押した
		m_bRightPush = true;
		// 押した音 
		se->Play("カーソル2");
		// 右の矢印アニメ
		m_pRightArrow->Action();

	}else
	{
		// 押してなかったらフラグをOFF
		m_bRightPush = false;
	}


}

