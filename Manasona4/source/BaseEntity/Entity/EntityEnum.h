#pragma once

//********************************************************
//	ゲーム上の定数たち
//********************************************************

#define GAME_PLAYER_MAX 2		//ゲーム出てくる人の最大数

#define GAME_ID_MAX 500

namespace GAME_ID
{
	enum _ENUM_ENTITY_ID
	{
		ID_ERROR,					//エラーID
		SCENE_MAIN,					// シーンメイン
		SCENE_MENU,					// シーンメニュー
		SCENE_SELECT,				// シーンキャラセレ
		SCENE_COLLECT,				// シーンコレクト
		SCENE_TITLE,				// シーンタイトル
		PLAYER_MGR,					// マネージャーのID 
		JUDGE_MNG,					// ジャッジマネージャーID
		CAMERA_MGR,					// カメラマネージャーID
		UI_MGR,						// UIマネージャー
		CUTIN_MGR,					// カットインマネージャー
		ROUND_CALL_MGR,				// ラウンドコールマネージャー
		SHOT_MGR,					// ショットマネージャー
		SELECT_UI_LEFT,				// キャラクターセレクトのUI左
		SELECT_UI_RIGHT,			// キャラクターセレクトのUI右
		TROPHY_MGR,					// トロフィーマネージャー
		PICTURE_MGR,				// ピクチャーマネージャー
		MOVIE_MGR,					// ムービーマネージャー
		ID_PLAYER_FIRST,			//人のIDの初め
		ID_PLAYER_MAX = ID_PLAYER_FIRST + GAME_PLAYER_MAX,	//人の最大ID+1

		ID_AI_FIRST,			//AIのIDの初め
		ID_AI_MAX = ID_AI_FIRST + GAME_PLAYER_MAX,	//AIの最大ID+1

	};
}
typedef GAME_ID::_ENUM_ENTITY_ID	ENTITY_ID;	//ゲーム内ID