#pragma once

#include "../BaseEntity/Entity/BaseGameEntity.h"

// 前方宣言
enum class SIDE;
namespace Shot{class Base;}

class ShotManager : public BaseGameEntity
{
public:

	//+-----------------------------------------------------
	//	実体
	//+-----------------------------------------------------
	static ShotManager* GetInstance()
	{
		if (m_pInstance == nullptr)m_pInstance = new ShotManager();
		return m_pInstance;
	}

	static void Release() { SAFE_DELETE(m_pInstance); }

	//------------------------------------------------------
	//	デストラクタ(リストの開放)
	//------------------------------------------------------
	~ShotManager();

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();
	void EraseCheakUpdate();
	void AllEraseSide(SIDE eSide);

	//------------------------------------------------------
	//	描画
	//------------------------------------------------------
	void Render();
	//void Render(tdnShader *shader, char *name);


	//------------------------------------------------------
	//	メッセージ受信(BaseGameEntityのオーバーライド)
	//------------------------------------------------------
	bool  HandleMessage(const Message& msg);

	//------------------------------------------------------
	//	ショットリスト追加
	//------------------------------------------------------
	void AddShot(SIDE side, Shot::Base *pNewShot){ m_list[(int)side].push_back(pNewShot); }
	void Reset();

	//------------------------------------------------------
	//	ゲッター
	//------------------------------------------------------
	std::list<Shot::Base*> *GetList(SIDE side){ return &m_list[(int)side]; }

private:
	// 1つしかない実体
	static ShotManager *m_pInstance;

	//------------------------------------------------------
	//	コンストラクタ
	//------------------------------------------------------
	ShotManager();
	ShotManager(const ShotManager&);
	ShotManager &operator=(const ShotManager&) {};

	//------------------------------------------------------
	//	飛び道具を格納するリスト
	//------------------------------------------------------
	std::list<Shot::Base*> m_list[(int)SIDE::ARRAY_MAX];

};

#define ShotMgr	ShotManager::GetInstance()
