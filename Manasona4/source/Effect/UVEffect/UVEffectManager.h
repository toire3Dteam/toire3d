#pragma once
#include "BaseUVEffect.h"

/**************************************/
//	�}�l�[�W���[ ���L�����N�^�[�Ɉ�E�ϒ��z��g��Ȃ��inew�͍ŏ������j
/**************************************/

class BaseUVEffect;

enum class UV_EFFECT_TYPE
{
	WAVE, HIT_IMPACT, PERSONA, IMPACT, SHOCK_WAVE, UPPER, GUARD,
	RUN, CONV, CONV2, CONV3, CONV4, BURST_BALL,
	SPLASH,
	HIT_RING, AIROU_DRILL, BREAK_IMPACT, FIRE_RING,
	AIROU_CIRCLE,
	OVER_DRIVE_RING,
	JUMP_WAVE, JUMP_SPEED_LINE,
	PRESSURE,ORANGE_BURST,
	ARRAY_END, // ���z��̏I���
};

enum class UV_EFFECT_MULTIPLE_TYPE
{
	GUARD_WAVE,
	GUARD_GRID,
	HIT_RIP,
	HIT_SLASH,
	ARRAY_END, // ���z��̏I���
};

// (TODO) �f�B���C����܂���H
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
    void AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale,
        Vector3 startAngle, Vector3 endAngle);
	void AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale,
		Vector3 startAngle, Vector3 endAngle, int deayTimer);
	void StopEffect(UV_EFFECT_TYPE type);

	// ���[�v�Ƃ��ăG�t�F�N�g����
	void AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type);
	void AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale);
	void StopEffectRoop(UV_EFFECT_TYPE type);

	// �A�N�Z�T
	BaseUVEffect* GetBaseUVEffect(UV_EFFECT_TYPE type){ return m_baseUVEffect[(int)type]; }
	void SetPos(Vector3 pos, UV_EFFECT_TYPE type) { m_baseUVEffect[(int)type]->SetPos(pos); };// �|�W�V�����̐ݒ�̂�蒼��

private:

	BaseUVEffect* m_baseUVEffect[(int)UV_EFFECT_TYPE::ARRAY_END];

	int m_delayTimer[(int)UV_EFFECT_TYPE::ARRAY_END];
		
public:

	// ���i�q�b�g�p
	static const int MULTIPLE_HIT_MAX = 4;
	
	struct MULTIPLE_EFFECT
	{
		// ���i�p���p��
		BaseUVEffect* pBaseUVEffectMultiple[MULTIPLE_HIT_MAX];
		int iDelayTimer[MULTIPLE_HIT_MAX];
	};

	//  
	MULTIPLE_EFFECT m_pMultipleEffect[(int)UV_EFFECT_MULTIPLE_TYPE::ARRAY_END];

	void AddMultipleEffect(Vector3 pos, UV_EFFECT_TYPE type);
	
	void AddMultipleEffect(Vector3 pos, UV_EFFECT_MULTIPLE_TYPE type, float startScale, float endScale,
		Vector3 startAngle, Vector3 endAngle, int deayTimer = 0);

};
