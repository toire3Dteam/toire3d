#pragma once
#include "TDNLIB.h"
#include "Number\Number.h"

//+-----------------------
//	基本のパラメーター設定UI
//+-----------------------


class BaseParamSetting
{
public:
	BaseParamSetting(int Number, int WidthSize);
	virtual ~BaseParamSetting();

	virtual void Update();
	virtual void Render(int x,int y,bool selectFlag);
	void RenderNumber(int x, int y, int num, bool bSelect = false);// 数字描画
	virtual void Ctrl(int DeviceID);

	// アクセサ
	int GetNumberID() { return m_iNumberID; };
	int GetWidthSize() { return m_iWidthSize; };



protected:
	// 番号・横幅
	int m_iNumberID;
	int m_iWidthSize;

	// 素材
	tdn2DAnim* m_pLeftArrow;
	tdn2DAnim* m_pRightArrow;

	tdn2DObj* m_pPersentFrame;
	tdn2DObj* m_pPersentGage;

	tdn2DObj* m_pNumber;

	// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッ
	// って感じの触り心地に
	int m_iSpeedLv;
	int m_iLeftPushFrame;
	int m_iRightPushFrame;
	bool m_bLeftPush;		// 左押した
	bool m_bRightPush;		// 右押した
};
