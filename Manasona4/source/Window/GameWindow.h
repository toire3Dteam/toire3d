#pragma once
#include "BaseWindow.h"
#include "ParamSetting/DifficultyParamSetting.h"
#include "ParamSetting/RoundNumTypeParamSetting.h"
#include "ParamSetting/RoundTimeTypeParamSetting.h"

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
		ROUND,
		TIME,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 704;

	DifficultyParamSetting* m_pDifficultyParam;
	RoundNumTypeParamSetting* m_pRoundNumTypeParam;
	RoundTimeTypeParamSetting* m_pRoundTimeTypeParam;

};
