#pragma once
#include "BaseWindow.h"
#include "ParamSetting\BGMParamSetting.h"
#include "ParamSetting\SEParamSetting.h"
#include "ParamSetting\VoiceParamSetting.h"

//+-------------------------------
//	サウンドウィンドウ
//+-------------------------------


class SoundWindow : public BaseWindow
{
public:
	SoundWindow(Vector2 vPos);
	~SoundWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo,int DeviceID);


	// 選択状態
	enum SOUND_STATE
	{
		BGM,
		SE,
		VOICE,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 768;

	BGMParamSetting* m_pBGMParam;
	SEParamSetting* m_pSEParam;
	VoiceParamSetting* m_pVoiceParam;

	//// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッ
	//// って感じの触り心地に
	//int iSpeedLv;
	//int iLeftPushFrame;
	//int iRightPushFrame;
	//int iFrame;
};
