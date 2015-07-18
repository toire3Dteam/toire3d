
#include "motion_blend_airou.h"
#include "IEX_3DObject2.h"

TEST_airou* test_airou = nullptr;

TEST_airou::TEST_airou()
{
	// モーションを2個に分けて管理する
	int num_motion = 2;
	obj = new iex3DObj2("./DATA/character/airou/airou_motion.IEM", num_motion);

	obj->SetPos(0, 0, 0);
	obj->SetAngle(0);
	obj->SetScale(1);

	// モーションデータ0,1にモーション0を
	int motion_data = 0;
	int motion = WAIT;
	obj->SetMotion(motion_data, motion);

	motion_data = 0;
	obj->SetMotion(motion_data, motion);
	// 17個すべてのボーンをモーションデータ0に設定
	int num_bone = 17;
	obj->Set_bone_motion(motion, num_bone, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	angle = 0;
}

TEST_airou::~TEST_airou()
{
	delete obj;
}

void TEST_airou::Update()
{
	angle += 0.02f;
	if (angle > PI) angle -= PI * 2;

	// 1 走る
	if(GetKeyState('1') < 0)
	{
		obj->SetMotion(0, RUN);

		// 2 上半身 攻撃
		if (GetKeyState('2') < 0)
		{
			// モーションデータ１を攻撃モーションに
			obj->SetMotion(1, ATTACK);
			// 上半身のボーンをモーションデータ１に
			obj->Set_bone_motion(1,
				11,
				3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
		}
		else
		{
			// 上半身のボーンをモーションデータ０に
			obj->Set_bone_motion(0,
				11,
				3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
			// モーションデータ１を待機モーションに
			obj->SetMotion(1, WAIT);
		}
	}
	// 待機
	else
	{
		// すべてのボーンをモーションデータ０に
		obj->Set_bone_motion(0,
			17,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

		// 攻撃
		if (GetKeyState('2') < 0)
		{
			obj->SetMotion(0, ATTACK);
		}
		else
		{
			obj->SetMotion(0, WAIT);
		}
	}

	obj->SetAngle(angle);

	obj->Animation();
	obj->Update();
}

void TEST_airou::Render()
{
	obj->Render();
}
