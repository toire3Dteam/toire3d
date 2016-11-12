#pragma once
#include "TDNLIB.h"

// タイム管理<Singleton>
class TimeManager
{
private:
	int m_frame;		// 自分のフレーム
	tdn2DAnim* m_pic;	// 画像
	Vector2 m_vPos;

	// シングルトンの作法
	TimeManager();
	TimeManager(const TimeManager&){}
	TimeManager &operator=(const TimeManager&){}

public:
	static TimeManager *GetInstance(){ static TimeManager i; return &i; }

	// 初期化
	void  Init();

	// 解放
	void  Release() { SAFE_DELETE(m_pic); };

	// 初期化(引数は「秒単位」で指定する)
	void Reset(int second){ m_frame = second * 60; }

	// 更新(タイム減算処理&終了判定を返す)
	bool Update();

	// 描画
	void Render();

	// 「フレーム単位」取得
	int GetFrame(){ return m_frame; }

	// 「秒単位」取得
	int GetSecond(){ return m_frame / 60; }
};

#define TimeMgr (TimeManager::GetInstance())