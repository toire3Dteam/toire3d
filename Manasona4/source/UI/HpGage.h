#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"

//+------------------------------
//		HP�Q�[�W
//+------------------------------

enum class UI_SIDE 
{
	LEFT = 0, RIGHT = 1
};

class HpGage
{
public:
	HpGage(BasePlayer* pPlayer);
	~HpGage();

	void Update();
	void Render();

	void UseMaskScreen();

	void MaskUpdate();
	void MaskScreen();

	void FirstAction(int waitFrame = 0);	// �ŏ��̈��

private:
	tdn2DObj* m_pGage;	
	int m_iGageWidth;
	int m_iGageHeight;
	
	tdn2DObj* m_pGageUsually;	// ���ʈʂ̐F
	tdn2DObj* m_pGagePinch;		// �s���`�̐F

	tdn2DObj* m_pDamageGage;
	int m_iDamagePoint;
	float m_fDamageRate;

	// �E�F�[�u
	tdn2DObj* m_pWave;	
	float m_fWaveUV;

	// 
	float m_fRate;

	// HP�p�����[�^�[�p
	int m_iMaxHP;
	int m_iHP;
	Vector2 m_vPos;

	SIDE m_sSideFlag;

	// �v���C���[���Q�Ƃ���悤
	BasePlayer* m_pPlayerReferences;

	// �}�X�N�X�N���[��
	tdn2DObj* m_pMaskScreen;

	// �}�X�N�K�p�X�N���[��
	tdn2DObj* m_pMaskPic;
	float m_fMaskPicScare;
	int m_iWaitFrame;
	tdn2DObj* m_pUseMaskScreen;

	// 
	bool m_bFirstActionFlag;


	//// SP�Q�[�W
	//tdn2DObj* m_pSpGage;
	//Vector2 m_vSpPos;
	//float m_fSpRate;
	//int m_iSPGageWidth;
	//int m_iSPGageHeight;

};
