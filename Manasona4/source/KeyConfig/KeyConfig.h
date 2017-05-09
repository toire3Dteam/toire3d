#pragma once
#include "TDNLIB.h"

//+--------------------------------------------
//	キーコンフィグ
//+--------------------------------------------

class KeyConfig
{
public:
	KeyConfig();
	~KeyConfig();

	bool Update();
	void Render();

	bool Ctrl();

	void Action(Vector2 vPos, int DeviceID);
	void Stop();

	// アクセサ
	bool IsActive() { return m_bActive; };
	int  GetChoiceState() { return m_iChoiceState; }//　★このゲッターで様々な判定をする

protected:
	static const int SizeX = 432;

	int m_iDeviceID;
	KEY_CONFIG_DATA m_tagConfigData, m_tagOrgConfigData;

	// ウィンドウのデザイン
	tdn2DObj* m_pWindow;	// 全体
	tdn2DAnim* m_pSelect;	// 選択中のアイコン

	// ウィンドウの場所
	Vector2 m_vPos;
	// このウィンドウが起動しているかどうか
	bool m_bActive;
	// 選択してる場所
	int m_iSelectNo;


	
	// 選択状態
	enum CHOICE_STATE
	{
		HOLD = -1
	};

	enum KEY_CONFIG_STATE
	{
		ATTACK,			// 攻撃
		SKILL,			// スキル
		THROW,			// 投げ
		ESCAPE,			// 回避
		HEAVEHO_DRIVE,	// 必殺技
		INVINCIBLE,		// 無敵
		PARTNER,		// パートナー
		DOKKOI,			// 中段
		OVER_DRIVE,		// バースト
		START,
		SELECT,

		BACK,			// 戻る
		ARRAY_END		// 配列の終り
	};

	int m_iChoiceState;


	
	// ウィンドウの中の数
	struct IconData
	{
		LPSTR pString;// 文字
		int iStringLength;// 文字数
		//LPSTR pInfoString;// 説明用文字
	};
	std::vector<IconData> m_aIconData;


	// 二回戻り防止
	bool m_bBackPush;

	// (1/28) 緊急事態
	// 他のPCで起動したときMENU画面が描画が
	// アンチエイリアシングの効果などで見た目が少し変わった
	// ひとまずは補間を考慮して調整
	int m_iAbjustHeight;

	int m_iSelectLength;	//	選択中のアイコンの長さ

protected:
	void AddIconData(LPSTR string);

	void SetConfig(KEYCODE PushedKey);
	BYTE *GetKeyConfigButton(KEY_CONFIG_STATE eState);
};
