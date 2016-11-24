#pragma once
#include "BaseWindow.h"

//+------------------------
// バトルウィンドウ
//+------------------------


class BattleWindow: public BaseWindow
{
public:
	BattleWindow(Vector2 vPos);
	~BattleWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// バトル用選択状態
	enum BATTLE_STATE
	{
		BACK_CHARA_SELECT,
		BACK
	};

	
private:
	static const int SizeX = 384;

	// このウィンドウの演出のために色々作ろう
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

};
