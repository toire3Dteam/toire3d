#pragma once
#include "TDNLIB.h"

/*
	�|�X�g�G�t�F�N�g
*/

class PostEffect
{
public:
	// ������
	static PostEffect& GetInstance();
	static void Release();
	virtual ~PostEffect();

	void Update();

	// �X�N���[�����e
	void RadialBigin();
	void RadialEnd();
	void RadialRender();
	void SetRadialBlur(Vector2 pos,float power);

	void BloomBigin();
	void BloomEnd();
	void BloomRender();
	void Ctrl();

	// 
	void BlueUpdate();


private:
	static PostEffect* pInstance;
	// ����
	PostEffect();
	PostEffect(const PostEffect&) {}
	PostEffect& operator=(const PostEffect&) {}



private:// �ϐ�

	// �X�N���[��
	tdn2DObj* m_radialScreen;
	tdn2DObj* m_postBlurScreen;
	tdn2DObj* m_bloomScreen;

	// �ŏI�I�ɉ�ʂɉf���T�[�t�F�C�X
	Surface* m_backbuffer;

	// �u���\
	float m_blurValue;
	float m_bluePower;


};


#define PostEffectMgr  (PostEffect::GetInstance())