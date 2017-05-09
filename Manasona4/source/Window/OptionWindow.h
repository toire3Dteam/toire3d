#pragma once
#include "BaseWindow.h"

//+------------------------
// オプションウィンドウ
//+------------------------


class OptionWindow: public BaseWindow
{
public:
	OptionWindow(Vector2 vPos);
	~OptionWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// オプション用選択状態
	enum OPTION_STATE
	{
		SYSTEM,
		GAME,
		SOUND,
		KEYCONFIG,
		BACK
	};

	
private:
	static const int SizeX = 384;

	// このウィンドウの演出のために色々作ろう
	//int m_iAlpha;

};
