#include	"TDNLIB.h"

//**************************************************************************************************
//
//		�X�L�����b�V���֘A
//
//**************************************************************************************************

//*****************************************************************************
//	�t���[���X�V
//*****************************************************************************
void	iex3DObj::UpdateSkinMeshFrame(float frame, float slerp)
{
	if (slerp < 0) slerp = 0;
	if (slerp > 1) slerp = 1;
	float slerpInv = 1 - slerp;

	u32			i, j;
	LPIEXANIME2	lpAnime;
	float		t;

	for (i = 0; i<NumBone; i++){
		lpAnime = &this->lpAnime[i];

		//	�|�[�Y�ݒ�
		if (lpAnime->rotNum == 0) CurPose2[i] = orgPose[i];
		else if (lpAnime->rotNum == 1) CurPose2[i] = lpAnime->rot[0];
		else {
			//	��]�L�[���
			for (j = 0; j<lpAnime->rotNum - 1; j++)
			{
				//	���݈ʒu����
				if ((frame >= lpAnime->rotFrame[j]) && (frame<lpAnime->rotFrame[j + 1]))
				{
					//	�o�߃t���[���v�Z
					t = (float)(frame - lpAnime->rotFrame[j]) / (float)(lpAnime->rotFrame[j + 1] - lpAnime->rotFrame[j]);
					//	���
					CurPose2[i] = QuaternionSlerp(lpAnime->rot[j], lpAnime->rot[j + 1], t);
					break;
				}
			}
			if (j == lpAnime->rotNum - 1) CurPose2[i] = lpAnime->rot[lpAnime->rotNum - 1];
			//	�O�t���[�����
			CurPose2[i] = QuaternionSlerp(CurPose2[i], CurPose1[i], slerp);
		}
		//	���W�ݒ�
		if (lpAnime->posNum == 0) CurPos2[i] = orgPos[i];
		else {
			//	�ʒu���
			for (j = 0; j<lpAnime->posNum - 1; j++)
			{
				//	���݈ʒu����
				if ((frame >= lpAnime->posFrame[j]) && (frame<lpAnime->posFrame[j + 1]))
				{
					//	�o�߃t���[���v�Z
					t = (float)(frame - lpAnime->posFrame[j]) / (float)(lpAnime->posFrame[j + 1] - lpAnime->posFrame[j]);
					//	���
					CurPos2[i] = lpAnime->pos[j] + (lpAnime->pos[j + 1] - lpAnime->pos[j])*t;
					break;
				}
			}
			if (j == lpAnime->posNum - 1) CurPos2[i] = lpAnime->pos[lpAnime->posNum - 1];
			//	�O�t���[�����
			CurPos2[i] = CurPos2[i] * slerpInv + CurPos1[i] * slerp;
		}
	}

	void *swap = CurPose1;
	CurPose1 = CurPose2;
	CurPose2 = (Quaternion*)swap;

	swap = CurPos1;
	CurPos1 = CurPos2;
	CurPos2 = (Vector3*)swap;
}

//*****************************************************************************
//	�{�[���s��X�V
//*****************************************************************************
void iex3DObj::UpdateBoneMatrix()
{
	u32			i;

	//	�{�[���X�V
	for (i = 0; i<NumBone; i++)
	{
		CurPose1[i].toMatrix(lpBoneMatrix[i]);
		//	�ʒu���R�s�[
		lpBoneMatrix[i]._41 = CurPos1[i].x;
		lpBoneMatrix[i]._42 = CurPos1[i].y;
		lpBoneMatrix[i]._43 = CurPos1[i].z;

		if (BoneParent[i] != 0xFFFF) lpBoneMatrix[i] *= lpBoneMatrix[BoneParent[i]];
	}
	for (i = 0; i<NumBone; i++)
	{
		lpMatrix[i] = lpOffsetMatrix[i] * lpBoneMatrix[i];
	}
}


//*****************************************************************************
//	�X�L�����b�V���X�V
//*****************************************************************************
void	iex3DObj::UpdateSkinMesh()
{
	//	���b�V���X�V
	void*	pVertex;
	lpMesh->LockVertexBuffer(D3DLOCK_DISCARD, &pVertex);
	lpSkinInfo->UpdateSkinnedMesh(lpMatrix, NULL, lpVertex, pVertex);
	lpMesh->UnlockVertexBuffer();
}

