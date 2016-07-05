#pragma once
#include "TDNLIB.h"
#include "BaseEffect\BaseEffect.h"


/********************/
//	�G�t�F�N�g�}�l�[�W���[
/********************/
enum class EFFECT_TYPE
{
	PLUS, HIT, PUT, NOTICE, INEFFECT, INEFFECT_MINI, DOG_EFFECT,
	BURN, ClEAR, DON, PUSH, DELICIOUS, EAT, PERFECT, GREAT, BAD, HAPPY, DARK_NOTICE,SMOKE
	};
class EffectManager
{
private:


public:


	// ����
	static EffectManager& GetInstance();
	static void Release();
	//Number_Effect();
	virtual ~EffectManager();

	// �X�V�E�`��
	void Update();
	void Render();

	// �ǉ�
	void AddEffect(int x, int y, EFFECT_TYPE type);
	
	//// NumberData
	//NumberData  

private:
	// ����
	static EffectManager* m_pInstance;

	// �����̃f�[�^
	std::list<BaseEffect*> m_EffectData;
	std::list<BaseEffect*>::iterator m_it;

	// ����
	EffectManager();
	EffectManager(const EffectManager&);
	EffectManager& operator=(const EffectManager&);

};

// �C���X�^���X��
#define EffectMgr	(EffectManager::GetInstance())
