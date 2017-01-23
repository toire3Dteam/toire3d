#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"
#include "ParamSetting\Tutorial\TutorialSelectParamSetting.h"

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

	// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);

	// チュートリアル用選択状態
	enum TUTORIAL_PAUSE_STATE
	{
		//BACK_PREV_TUTORIAL,
		SELECT_TUTORIAL,
		COMMAND_LIST,
		BACK_MENU_SELECT,
		BACK
	};


private:
	static const int SizeX = 768;

	// このウィンドウの演出のために色々作ろう
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	// 説明のプレート
	InformationPlate* m_pInfoPlate;

	TutorialSelectParamSetting* m_TutorialSelectParam;

	// 二回戻り防止
	bool m_bBackPush;
};
