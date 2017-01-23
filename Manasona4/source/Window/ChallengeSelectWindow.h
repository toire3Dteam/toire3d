#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"
#include "ParamSetting\Challenge\ChallengeSelectParamSetting.h"

//+-------------------------------
// チャレンジセレクトウィンドウ
//+-------------------------------


class ChallengeSelectWindow : public BaseWindow
{
public:
	ChallengeSelectWindow(Vector2 vPos);
	~ChallengeSelectWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);

	// 選択状態
	enum CHALLENGE_SELECT_STATE
	{
		SELECT_CHALLENGE,
		BACK
	};


private:
	static const int SizeX = 704;

	// このウィンドウの演出のために色々作ろう
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	// 説明のプレート
	//InformationPlate* m_pInfoPlate;

	ChallengeSelectParamSetting* m_ChallengeSelectParam;

	// 二回戻り防止
	//bool m_bBackPush;
};
