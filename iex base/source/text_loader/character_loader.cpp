
#include "character_loader.h"
#include <assert.h>
#include "../player/BasePlayer.h"
#include "../../IEX/iextreme.h"

bool Character_loader::Load(BasePlayer *character)
{
	assert(character != nullptr);
	assert(ferror(file) == 0);

	file_pointer = 0;

	char path[256]{}; // フォルダへのパス
	char work1[256]{}, work2[256]{}; // いろいろ

	iex3DObj2 *obj(nullptr);

	// path 作成
	{
		bool copy = false;
		{
			unsigned int i = 0;
			for (; filename[i] != '\0'; i++); // 最後
			for (; filename[i] != '/'; i--); // 一個前
			// コピー
			path[i + 1] = '\0';
			for (; i > 0; i--)
			{
				path[i] = filename[i];
			}
			path[i] = filename[i];
		}		
	}

	// iex3DObj2作成
	if (Serch_string("IEM"))
	{
		Load_string(work2, 256);
		Add_string(work1, 256, path, work2); // パス作成
		// 上半身、下半身で固定
		obj = new iex3DObj2(work1, 2);
		character->Set_object(obj);
	}
	else return false;
	// ボーンのグループの数
	if (Serch_string("BONE"))
	{
		int motion_data(0);
		char *flag[2] = { "LOWER", "UPPER" };
		if (!Serch_string(flag[motion_data])) // flag 検索
			return false;
		if(!Serch_string("{")) // 始
			return false;

		while (true)
		{
			Load_string(work1, 256); // ボーンの番号
			if (work1[0] == '}') // 終
			{
				motion_data++; // 次のグループ
				if (motion_data >= 2) // 終
					break;

				if (!Serch_string(flag[motion_data])) // flag 検索
					return false;
				if (!Serch_string("{")) // 始
					return false;

				continue;
			}
			obj->Set_bone_motion(motion_data, atoi(work1));
		}
	}
	else return false;

	return true;
}
