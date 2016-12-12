#pragma once
#include "BaseWindow.h"

//+-------------------------------
//	サウンドウィンドウ
//+-------------------------------


class SoundWindow : public BaseWindow
{
public:
	SoundWindow(Vector2 vPos);
	~SoundWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// 選択状態
	enum SOUND_STATE
	{
		BGM,
		SE,
		VOICE,
		BACK
	};


private:
	static const int SizeX = 432;

	// このウィンドウの演出のために色々作ろう
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

	// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッ
	// って感じの触り心地に
	int iSpeedLv;
	int iLeftPushFrame;
	int iRightPushFrame;
	int iFrame;
};
