#include	"iextreme.h"

#include	"BasePlayer.h"
#include	"Airou.h"
#include	"PlayerManager.h"




//******************************************************************************
//
//		グローバル変数
//
//******************************************************************************

int PlayerManager::max_player = 0;		// staticメンバ変数の実体




//******************************************************************************
//
//		初期化と解放
//
//******************************************************************************
void PlayerManager::Initialize()
{
	PlayerManager::max_player = 4;	// 仮

	players = new BasePlayer*[PlayerManager::max_player];
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		players[i] = new Airou(i);		// 今はアイルー
	}
}

void PlayerManager::Release()
{
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		SAFE_DELETE(players[i]);
	}
	delete[] players;		// コレをしないとメモリリークする。
}


//******************************************************************************
//
//		更		新
//
//******************************************************************************
void PlayerManager::Update()
{
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		players[i]->Update(i);

		if (players[i]->isDied())	// 死んでたら復活させてあげる
			players[i]->Respawn(i);
	}
}


//******************************************************************************
//
//		描		画
//
//******************************************************************************
void PlayerManager::Render()
{
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		players[i]->Render();
	}
}