
#include "motion_blend_airou.h"
#include "IEX_3DObject2.h"

TEST_airou::TEST_airou()
{}

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
			obj->Set_bone_motions(1,
				11,
				3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
		}
		else
		{
			// 上半身のボーンをモーションデータ０に
			obj->Set_bone_motions(0,
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
		obj->Set_bone_motions(0,
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
