#pragma once
#include "BaseWindow.h"

//+------------------------
// チャレンジウィンドウ
//+------------------------


class ChallengeWindow: public BaseWindow
{
public:
	ChallengeWindow(Vector2 vPos);
	~ChallengeWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// 選択状態
	enum CHALLENGE_STATE
	{
		AIROU,
		TEKI,
		NAZENARABA,
		ARAMITAMA,
		BACK
	};

	
private:
	static const int SizeX = 432;

	// このウィンドウの演出のために色々作ろう
	//int m_iAlpha;

};
