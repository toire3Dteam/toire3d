#pragma once
#include "BaseWindow.h"

//+-------------------------------
// リザルトウィンドウ
//+-------------------------------


class ResultWindow : public BaseWindow
{
public:
	ResultWindow(Vector2 vPos);
	~ResultWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// 選択状態
	enum RESULT_STATE
	{
		AGAIN,
		BACK_CHARA,
		BACK_MENU
	};


private:
	static const int SizeX = 432;

	// このウィンドウの演出のために色々作ろう
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

};
