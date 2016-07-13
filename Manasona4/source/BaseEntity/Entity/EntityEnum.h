#pragma once

//********************************************************
//	ゲーム上の定数たち
//********************************************************

#define GAME_PLAYER_MAX 4		//ゲーム出てくる人の最大数

#define GAME_ID_MAX 500

namespace GAME_ID
{
	enum _ENUM_ENTITY_ID
	{
		ID_ERROR,					//エラーID

		PLAYER_MGR,					// マネージャーのID 
		JUDGE_MNG,					// ジャッジマネージャーID
		CAMERA_MGR,					// カメラマネージャーID

		ID_PLAYER_FIRST,			//人のIDの初め
		ID_PLAYER_MAX = ID_PLAYER_FIRST + GAME_PLAYER_MAX,	//人の最大ID+1
	};
}
typedef GAME_ID::_ENUM_ENTITY_ID	ENTITY_ID;	//ゲーム内ID

