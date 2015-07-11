
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

		u8 Motion = 0; // 現在のモーション番号
		u32 dwFrame = 0; //	現在のフレーム
		u8 bChanged = 0; //	変更フラグ

		Motion_data()
		{
			for (int i = 0; i < 16; i++)
			{
				Param[i] = 0;
			}
		}
	};
	u32* bone_motion_number; // bone毎のモーションの種類

	Motion_data* motion_data = nullptr;

	void UpdateSkinMeshFrame();
public:
	iex3DObj2(char* filename,
		int number_of_motion_data); // モーションを分ける数
	~iex3DObj2();

	void Update();

	void Render();

	void SetMotion(int motion, int data_number);

	void Motion_reset(int data_number);

	void Set_bone_motion(int motion_data, // motiondataの番号
		int arraysize, // 配列の数
		int* bone_numbers); // 設定するボーンの番号を配列

	void Animation();
};

#endif // !IEX_3DOBJECT2_H
