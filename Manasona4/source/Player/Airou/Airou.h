#pragma once

class Airou : public BasePlayer
{
public:
	Airou(int deviceID, bool bAI);

	/****************************/
	//	キャラクター固有スキル
	/****************************/
	void SkillUpdate();//

private:
	void InitActionDatas();	// 純粋仮想関数オーバーライド
};

