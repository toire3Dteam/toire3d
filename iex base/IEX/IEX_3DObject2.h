
#ifndef IEX_3DOBJECT2_H
#define IEX_3DOBJECT2_H

#include "iextreme.h"
#include <stdarg.h>

class iex3DObj2 : public iex3DObj
{
private:
	u32 number_of_motion_data = 0;
	class Motion_data
	{
	public:
		u8 Param[16];

		u8 Motion = 0; // ���݂̃��[�V�����ԍ�
		u32 dwFrame = 0; //	���݂̃t���[��
		u8 bChanged = 0; //	�ύX�t���O

		Motion_data()
		{
			for (int i = 0; i < 16; i++)
			{
				Param[i] = 0;
			}
		}
	};
	u32* bone_motion_number; // bone���̃��[�V�����̎��

	Motion_data* motion_data = nullptr;

	void UpdateSkinMeshFrame();

	bool iexMesh_Update_use = true; // iexMesh2::Update ���g����
public:
	iex3DObj2(char* filename,
		int number_of_motion_data); // ���[�V�����𕪂��鐔
	~iex3DObj2();

	void Update();

	void Render();

	void Animation();

	void SetMotion(int data_number, // Motion_data �̔ԍ�
		int motion); // ���[�V�����ԍ�

	// ���̃��[�V�������ŏ�����
	void Motion_reset(int data_number); // Motion_data �̔ԍ�

	void Set_bone_motion(int motion_data, int bone)
	{
		bone_motion_number[bone] = motion_data;
	}
	void Set_bone_motions(int motion_data, // Motion_data �̔ԍ�
		int num, // �{�[���̐�
		...); // �ݒ肷��{�[���̔ԍ�

	void Is_use_iexMesh_Update(bool in){ iexMesh_Update_use = in; }
};

#endif // !IEX_3DOBJECT2_H
