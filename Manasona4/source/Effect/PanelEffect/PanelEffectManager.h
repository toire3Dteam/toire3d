#pragma once
#include "TDNLIB.h"
#include "PanelEffect.h"


/********************/
//	�G�t�F�N�g�}�l�[�W���[
/********************/
enum class PANEL_EFFECT_TYPE
{
	PLUS, HIT, PUT, DAMAGE, INEFFECT, INEFFECT_MINI, DOG_EFFECT,
	BURN, ClEAR, DON, PUSH, DELICIOUS, EAT, PERFECT, GREAT, BAD, HAPPY, DARK_NOTICE, SMOKE,
	BURST,ONEMORE_BURST,GUARD,BURST_PREV,GLASS,
	ARRAY_END
};

//class EffectManager
//{
//private:
//
//
//public:
//
//	// ����
//	static EffectManager& GetInstance();
//	static void Release();
//	//Number_Effect();
//	virtual ~EffectManager();
//
//	// �X�V�E�`��
//	void Update();
//	void Render();
//	void Render3D();// �r���{�[�h�ŕ`��
//
//	// �ǉ�
//	void AddEffect(int x, int y, PANEL_PANEL_EFFECT_TYPE type);
//	void AddEffect(Vector3 pos, PANEL_EFFECT_TYPE type);
//
//	//// NumberData
//	//NumberData  
//
//private:
//	// ����
//	static EffectManager* m_pInstance;
//
//	// �����̃f�[�^
//	std::list<BaseEffect*> m_EffectData;
//	std::list<BaseEffect*>::iterator m_it;
//
//	// ����
//	EffectManager();
//	EffectManager(const EffectManager&);
//	EffectManager& operator=(const EffectManager&);
//
//};
//
//// �C���X�^���X��
//#define EffectMgr	(EffectManager::GetInstance())

class PanelEffectManager
{

public:
	PanelEffectManager();
	~PanelEffectManager();

	// �X�V�E�`��
	void Update();
	void Render();
	void Render3D();// �r���{�[�h�ŕ`��

	// �ǉ�
	void AddEffect(int x, int y, PANEL_EFFECT_TYPE type);
	void AddEffect(Vector3 pos, PANEL_EFFECT_TYPE type);

private:

	BasePanelEffect* m_basePanelEffect[(int)PANEL_EFFECT_TYPE::ARRAY_END];

};