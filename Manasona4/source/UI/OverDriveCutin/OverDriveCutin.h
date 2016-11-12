#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"

//+----------------------------
//	�I�[�o�[�h���C�u�J�b�g�C��
//+----------------------------

class OverDriveCutin
{
public:
	OverDriveCutin(BasePlayer* pPlayer);
	~OverDriveCutin();

	void Update();
	void Render();

	void Action();

private:
	// �w�i
	tdn2DObj* m_pBG;
	// �v���C���[���Q�Ɨp
	BasePlayer* m_pPlayerData;
	Vector2 m_vPos;

	bool m_bRender;
	int m_iFrame;			//	
	bool m_bTurnOverFlag;

	tdn2DObj* m_pScreen;
	//Surface* m_pStencilSurface;//Z
	
							   
	tdn2DObj* m_pScreenMask;// �}�X�N
	float m_fMaskRate;
	float m_fMaskPow;

	// �X�N���[������
	struct ScrollFont
	{
		tdn2DAnim* pPic;
		Vector2 vPos;
	};
	ScrollFont m_pFont1;
	ScrollFont m_pFont2;
	ScrollFont m_pFont3;


	tdn2DObj* m_pPostLight;
};
//�������
//�F�X������
//�t�H��t����炳����
//�Ђ���
//
//����̂��񂵂��
