#pragma once
#include "BaseWindow.h"
#include "ParamSetting/DifficultyParamSetting.h"

//+-------------------------------
//	ゲームウィンドウ
//+-------------------------------


class GameWindow : public BaseWindow
{
public:
	GameWindow(Vector2 vPos);
	~GameWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo,int DeviceID);


	// 選択状態
	enum GAME_STATE
	{
		DIFFICULTY,
		BACK
	};


private:
	static const int m_cSizeX = 768;

	DifficultyParamSetting* m_pDifficultyParam;

	//// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッ
	//// って感じの触り心地に
	//int iSpeedLv;
	//int iLeftPushFrame;
	//int iRightPushFrame;
	//int iFrame;
};
