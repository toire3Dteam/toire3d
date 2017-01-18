#pragma once
#include "BaseWindow.h"
#include "ParamSetting/AutoSaveParamSetting.h"

//+-------------------------------
//	システムウィンドウ
//+-------------------------------


class SystemWindow : public BaseWindow
{
public:
	SystemWindow(Vector2 vPos);
	~SystemWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo,int DeviceID);


	// 選択状態
	enum SYSTEM_STATE
	{
		AUTO_SAVE,
		SAVE,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 704;

	AutoSaveParamSetting* m_pAutoSaveParam;

};
