
#ifndef IEX_3DOBJECT2_H
#define IEX_3DOBJECT2_H

#include "Iex_Mesh2.h"


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
public:
	iex3DObj2(char* filename,
		int number_of_motion_data); // ���[�V�����𕪂��鐔
	~iex3DObj2();

	void Update();

	void Render();

	void SetMotion(int motion, int data_number);

	void Motion_reset(int data_number);

	void Set_bone_motion(int motion_data, // motiondata�̔ԍ�
		int arraysize, // �z��̐�
		int* bone_numbers); // �ݒ肷��{�[���̔ԍ���z��

	void Animation();
};

#endif // !IEX_3DOBJECT2_H
