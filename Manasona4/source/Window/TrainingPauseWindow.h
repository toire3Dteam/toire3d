#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"

//+-------------------------------
// トレーニングポーズウィンドウ
//+-------------------------------


class TrainingPauseWindow : public BaseWindow
{
public:
	TrainingPauseWindow(Vector2 vPos);
	~TrainingPauseWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// トレーニング用選択状態
	enum TRAINING_PAUSE_STATE
	{
		TRAINING_SETTING,
		POSITION_RESET,
		SOUND_SETTING,
		BACK_CHARA_SELECT,
		BACK_MENU_SELECT,
		BACK
	};


private:
	static const int SizeX = 432;

	// このウィンドウの演出のために色々作ろう
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	// 説明のプレート
	InformationPlate* m_pInfoPlate;

};
