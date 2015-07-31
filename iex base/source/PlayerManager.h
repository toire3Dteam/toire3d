#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

//******************************************************************************
//
//
//		Player管理クラス(Singleton)
//
//
//******************************************************************************

class PlayerManager
{
private:

	/* プレイヤー実体 */
	BasePlayer **players;

	/* プレイ人数 */
	static int max_player;


	PlayerManager(){}
	PlayerManager(const PlayerManager &pm){}
	PlayerManager &operator=(const PlayerManager &pm){}

public:


	//=========================================
	//	インスタンス取得
	static PlayerManager *GetInstance()
	{
		static PlayerManager instance;
		return &instance;
	}


	//=========================================
	//	初期化と解放
	void Initialize();
	void Release();


	//=========================================
	//	更新と描画
	void Update();
	void Render();

};


#endif