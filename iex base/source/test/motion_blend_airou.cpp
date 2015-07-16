
#include "motion_blend_airou.h"
#include "IEX_3DObject2.h"

TEST_airou* test_airou = nullptr;

TEST_airou::TEST_airou()
{
	// ���[�V������2�ɕ����ĊǗ�����
	int num_motion = 2;
	obj = new iex3DObj2("./DATA/character/airou/airou_motion.IEM", num_motion);

	obj->SetPos(0, 0, 0);
	obj->SetAngle(0);
	obj->SetScale(1);

	// ���[�V�����f�[�^0,1�Ƀ��[�V����0��
	int motion_data = 0;
	int motion = WAIT;
	obj->SetMotion(motion_data, motion);

	motion_data = 0;
	obj->SetMotion(motion_data, motion);
	// 17���ׂẴ{�[�������[�V�����f�[�^0�ɐݒ�
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

	// 1 ����
	if(GetKeyState('1') < 0)
	{
		obj->SetMotion(0, RUN);

		// 2 �㔼�g �U��
		if (GetKeyState('2') < 0)
		{
			// ���[�V�����f�[�^�P���U�����[�V������
			obj->SetMotion(1, ATTACK);
			// �㔼�g�̃{�[�������[�V�����f�[�^�P��
			obj->Set_bone_motion(1,
				11,
				3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
		}
		else
		{
			// �㔼�g�̃{�[�������[�V�����f�[�^�O��
			obj->Set_bone_motion(0,
				11,
				3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
			// ���[�V�����f�[�^�P��ҋ@���[�V������
			obj->SetMotion(1, WAIT);
		}
	}
	// �ҋ@
	else
	{
		// ���ׂẴ{�[�������[�V�����f�[�^�O��
		obj->Set_bone_motion(0,
			17,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

		// �U��
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
