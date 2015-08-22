
#include "character_loader.h"
#include <assert.h>
#include "../player/BasePlayer.h"
#include "../../IEX/iextreme.h"

bool Character_loader::Load(BasePlayer *character)
{
	assert(character != nullptr);
	assert(ferror(file) == 0);

	file_pointer = 0;

	char path[256]{}; // �t�H���_�ւ̃p�X
	char work1[256]{}, work2[256]{}; // ���낢��

	iex3DObj2 *obj(nullptr);

	// path �쐬
	{
		bool copy = false;
		{
			unsigned int i = 0;
			for (; filename[i] != '\0'; i++); // �Ō�
			for (; filename[i] != '/'; i--); // ��O
			// �R�s�[
			path[i + 1] = '\0';
			for (; i > 0; i--)
			{
				path[i] = filename[i];
			}
			path[i] = filename[i];
		}		
	}

	// iex3DObj2�쐬
	if (Serch_string("IEM"))
	{
		Load_string(work2, 256);
		Add_string(work1, 256, path, work2); // �p�X�쐬
		// �㔼�g�A�����g�ŌŒ�
		obj = new iex3DObj2(work1, 2);
		character->Set_object(obj);
	}
	else return false;
	// �{�[���̃O���[�v�̐�
	if (Serch_string("BONE"))
	{
		int motion_data(0);
		char *flag[2] = { "LOWER", "UPPER" };
		if (!Serch_string(flag[motion_data])) // flag ����
			return false;
		if(!Serch_string("{")) // �n
			return false;

		while (true)
		{
			Load_string(work1, 256); // �{�[���̔ԍ�
			if (work1[0] == '}') // �I
			{
				motion_data++; // ���̃O���[�v
				if (motion_data >= 2) // �I
					break;

				if (!Serch_string(flag[motion_data])) // flag ����
					return false;
				if (!Serch_string("{")) // �n
					return false;

				continue;
			}
			obj->Set_bone_motion(motion_data, atoi(work1));
		}
	}
	else return false;

	return true;
}
