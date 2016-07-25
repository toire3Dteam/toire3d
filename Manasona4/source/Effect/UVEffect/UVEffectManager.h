#pragma once
#include "BaseUVEffect.h"

/**************************************/
//	�}�l�[�W���[ ���L�����N�^�[�Ɉ�E�ϒ��z��g��Ȃ��inew�͍ŏ������j
/**************************************/

class BaseUVEffect;

enum class UV_EFFECT_TYPE
{
	WAVE, HIT_IMPACT, ARRAY_END, 
};

class UVEffectManager
{
public:
	UVEffectManager();
	~UVEffectManager();

	// �X�V�E�`��
	void Update();
	void Render();

	// ����
	void AddEffect(Vector3 pos, UV_EFFECT_TYPE type);
	void AddEffect(Vector3 pos, UV_EFFECT_TYPE type,float startScale, float endScale);
	//void StopEffect(UV_EFFECT_TYPE type);

private:

	BaseUVEffect* m_baseUVEffect[(int)UV_EFFECT_TYPE::ARRAY_END];

};
