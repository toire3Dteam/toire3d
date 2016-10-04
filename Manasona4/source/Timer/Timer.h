#pragma once

// タイム管理<Singleton>
class TimeManager
{
private:
	int m_frame;	// 自分のフレーム

	// シングルトンの作法
	TimeManager():m_frame(0){}
	TimeManager(const TimeManager&){}
	TimeManager &operator=(const TimeManager&){}

public:
	static TimeManager *GetInstance(){ static TimeManager i; return &i; }

	// 初期化(引数は「秒単位」で指定する)
	void Reset(int second){ m_frame = second * 60; }

	// 更新(タイム減算処理&終了判定を返す)
	bool Update()
	{
		bool bEnd(--m_frame <= 0);
		return bEnd;
	}

	// 「フレーム単位」取得
	int GetFrame(){ return m_frame; }

	// 「秒単位」取得
	int GetSecond(){ return m_frame / 60; }
};

#define TimeMgr (TimeManager::GetInstance())