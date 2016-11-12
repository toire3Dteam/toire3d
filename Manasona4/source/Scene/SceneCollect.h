#pragma once

namespace PlayerData
{
struct Infomation;
}

class sceneCollect : public BaseScene
{
public:

	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	bool Initialize();
	~sceneCollect();

	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	void Update();
	void Render();

private:

	enum IMAGE
	{
		BACK,
		SCROLL_BAR,
		SCROLL_ARROW,
		MAX
	};
	tdn2DObj *m_pImages[IMAGE::MAX];
	
	// プレイ情報
	PlayerData::Infomation *m_pPlayerInfo;


	// スクロール実験
	tdn2DObj* m_pScrollBar;
	tdn2DObj* m_pScrollArrow;

	// スクロール
	float m_iScrollHeight;	// 最大の高さ
	float m_iScrollScreenSize;// 一つのスクリーン幅
	float m_iScrollPos;

};