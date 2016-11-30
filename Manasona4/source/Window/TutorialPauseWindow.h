#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"

//+-------------------------------
// チュートリアルポーズウィンドウ
//+-------------------------------


class TutorialPauseWindow : public BaseWindow
{
public:
	TutorialPauseWindow(Vector2 vPos);
	~TutorialPauseWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// チュートリアル用選択状態
	enum TUTORIAL_PAUSE_STATE
	{
		//BACK_PREV_TUTORIAL,
		BACK_MENU_SELECT,
		BACK
	};


private:
	static const int SizeX = 432;

	// このウィンドウの演出のために色々作ろう
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

	// 説明のプレート
	InformationPlate* m_pInfoPlate;

};
