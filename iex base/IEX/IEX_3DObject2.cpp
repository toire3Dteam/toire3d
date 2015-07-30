
#include "IEX_3DObject2.h"
#include <assert.h>

void iex3DObj2::UpdateSkinMeshFrame()
{
	u32			i, j;
	LPIEXANIME2	lpAnime;
	float		t;

	u32 frame;
	for (i = 0; i<NumBone; i++)
	{
		frame = motion_data[bone_motion_number[i]].dwFrame;

		lpAnime = &this->lpAnime[i];

		//	ポーズ設定
		if (lpAnime->rotNum == 0) CurPose[i] = orgPose[i];
		else if (lpAnime->rotNum == 1) CurPose[i] = lpAnime->rot[0];
		else {
			//	回転キー補間
			for (j = 0; j<lpAnime->rotNum - 1; j++)
			{
				//	現在位置検索
				if ((frame >= lpAnime->rotFrame[j]) && (frame<lpAnime->rotFrame[j + 1]))
				{
					//	経過フレーム計算
					t = (float)(frame - lpAnime->rotFrame[j]) / (float)(lpAnime->rotFrame[j + 1] - lpAnime->rotFrame[j]);
					//	補間
					CurPose[i] = QuaternionSlerp(lpAnime->rot[j], lpAnime->rot[j + 1], t);
					break;
				}
			}
			if (j == lpAnime->rotNum - 1) CurPose[i] = lpAnime->rot[lpAnime->rotNum - 1];
		}
		//	座標設定
		if (lpAnime->posNum == 0) CurPos[i] = orgPos[i];
		else {
			//	位置補間
			for (j = 0; j<lpAnime->posNum - 1; j++)
			{
				//	現在位置検索
				if ((frame >= lpAnime->posFrame[j]) && (frame<lpAnime->posFrame[j + 1]))
				{
					//	経過フレーム計算
					t = (float)(frame - lpAnime->posFrame[j]) / (float)(lpAnime->posFrame[j + 1] - lpAnime->posFrame[j]);
					//	補間
					CurPos[i] = lpAnime->pos[j] + (lpAnime->pos[j + 1] - lpAnime->pos[j])*t;
					break;
				}
			}
			if (j == lpAnime->posNum - 1) CurPos[i] = lpAnime->pos[lpAnime->posNum - 1];
		}
	}
}

iex3DObj2::iex3DObj2(char* filename, int number_of_motion_data) : iex3DObj(filename), number_of_motion_data(number_of_motion_data)
{
	assert(number_of_motion_data != 0);

	motion_data = new Motion_data[number_of_motion_data]{};
	bone_motion_number = new u32[NumBone]{};
}

iex3DObj2::~iex3DObj2()
{
	delete[] motion_data;
	delete[] bone_motion_number;
}

void iex3DObj2::Update()
{
	/*	スキンメッシュ更新	*/
	UpdateSkinMeshFrame();
	UpdateBoneMatrix();
	UpdateSkinMesh();

	if (iexMesh_Update_use)
		iexMesh2::Update();
	RenderFrame = dwFrame;
	bChanged = FALSE;
}

void iex3DObj2::Render()
{
	//	情報更新
	///if (RenderFrame != dwFrame)
	Update();
	//	メイン行列設定
	iexMesh2::Render();
}

void iex3DObj2::SetMotion(int data_number, int motion)
{
	int		param;

	if (M_Offset[motion] == 65535) return;

	if (motion_data[data_number].Motion == motion)
		return;

	motion_data[data_number].Motion = motion;
	motion_data[data_number].dwFrame = M_Offset[motion];
	motion_data[data_number].bChanged = TRUE;

	param = dwFrameFlag[dwFrame];
	if ((param != 0xFFFF) && (param & 0x4000)) motion_data[data_number].Param[(param & 0x0F00) >> 8] = (u8)(param & 0x00FF);
}

void iex3DObj2::Motion_reset(int data_number)
{
	motion_data[data_number].dwFrame = M_Offset[motion_data[data_number].Motion];
	motion_data[data_number].bChanged = TRUE;
}

void iex3DObj2::Set_bone_motions(int motion_data, int num, ...)
{
	va_list val;
	va_start(val, num);

	for (int n = 0; n < num; n++)
	{
		int i = va_arg(val, int);
		bone_motion_number[i] = motion_data;
	}

	va_end(val);
}

void iex3DObj2::Animation()
{
	int		param;
	u32	work;

	for (int i = 0; i < (int)number_of_motion_data; i++)
	{
		work = motion_data[i].dwFrame;
		param = dwFrameFlag[work];
		if (param & 0x4000) // 0x4 == 0100
			param = 0xFFFF;

		if (param != 0xFFFF)
		{
			//	アニメーションジャンプ
			if (param & 0x8000) // 0x8 == 1000
			{
				SetMotion(i, param & 0xFF);
			}
			else
				motion_data[i].dwFrame = param;
		}
		else
		{
			motion_data[i].dwFrame++;
			if (motion_data[i].dwFrame >= NumFrame)
				motion_data[i].dwFrame = 0;
		}

		if (motion_data[i].dwFrame != work)
			motion_data[i].bChanged = TRUE;

		param = dwFrameFlag[motion_data[i].dwFrame];
		if ((param != 0xFFFF) && (param & 0x4000))
			motion_data[i].Param[(param & 0x0F00) >> 8] = (u8)(param & 0x00FF);
	}
}
