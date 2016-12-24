#pragma once
#include "TDNLIB.h"

//+------------------------
// ウィンドウ画面のベース
//+------------------------

class BaseWindow
{
public:
	BaseWindow(Vector2 vPos);
	virtual ~BaseWindow();

	virtual bool Update() = 0;	// 戻り値で起動しているか確認　他の制御を止める為に使います
	virtual void Redner() = 0;	// 描画
	virtual void RednerInfo();	// 下に表記する文字描画

	virtual bool Ctrl(int DeviceID) = 0; // コントロール
	virtual void CtrlSE(int DeviceID);	 // 操作音

	virtual void Action();	// Windowを出す
	virtual void Stop();	// 止める
	virtual void ChoiceClear();	// 選択していたものを未選択へ

	// アクセサ
	bool IsActive() { return m_bActive; };// ★
	int GetChoiceState() { return m_iChoiceState; }//　★このゲッターで様々な判定をする

protected:
	// ウィンドウの場所
	Vector2 m_vPos;

	// ウィンドウの中の数
	struct IconData
	{
		LPSTR pString;// 文字
		int iStringLength;// 文字数
		LPSTR pInfoString;// 説明用文字
	};
	std::vector<IconData> m_aIconData;

	// ウィンドウのデザイン
	tdn2DObj* m_pWindow;	// 全体
	tdn2DAnim* m_pTop;		// タイトル
	tdn2DAnim* m_pMiddle;	// 中
	tdn2DAnim* m_pBottom;	// しっぽ

	tdn2DAnim* m_pSelect;	//	選択中のアイコン
	int m_iSelectLength;	//	選択中のアイコンの長さ
	tdn2DObj* m_pFontBox;	// 文字を囲む□

	// このウィンドウが起動しているかどうか
	bool m_bActive;
	// 選択してる場所
	int m_iSelectNo;

	// 選択状態
	enum CHOICE_STATE
	{
		HOLD = -1
	};

	int m_iChoiceState;

	// 共通する演出用のパラメータ
	// 他のWindowと違うことやりたかったら子の方で作ってください
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

protected:
	void AddIconData(LPSTR string, LPSTR infoString);


};
