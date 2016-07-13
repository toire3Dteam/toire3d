#pragma once

//********************************************************
//	ゲーム上の定数たち
//********************************************************

#define GAME_PERSON_MAX 64		//ゲーム出てくる人の最大数

#define GAME_ID_MAX 500

namespace GAME_ID
{
	enum _ENUM_ENTITY_ID
	{
		ID_ERROR,					//エラーID

		PERSON_MNG,					// マネージャーのID 
		JUDGE_MNG,					// ジャッジマネージャーID
		CAMERA_MGR,					// カメラマネージャーID

		id_person_first,			//人のIDの初め
		id_person_max = id_person_first + GAME_PERSON_MAX,	//人の最大ID+1

	};
}
typedef GAME_ID::_ENUM_ENTITY_ID	ENTITY_ID;	//ゲーム内ID

