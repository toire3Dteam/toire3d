#pragma once
#include "BasePlayer.h"

// �O���錾
namespace Stand
{
	class Base;
}


class PlayerManager :public BaseGameEntity
{
public:
	// ���̎擾
	static PlayerManager *GetInstance()
	{
		if (!pInstance) pInstance = new PlayerManager;
		return pInstance;
	}

	~PlayerManager();
	void Initialize(int NumPlayer, Stage::Base *pStage);
	void Release(){ SAFE_DELETE(pInstance); }
	void Update(bool bControl);
	void Render();
	void Render(tdnShader* shader, char* name);
	void RenderDeferred();
	void RenderUI();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }

	void CalcTeamPoint();
	
	// �A�N�Z�T
	int GetPointA(){ return m_PointAteam; }
	int GetPointB(){ return m_PointBteam; }

	float GetOverDriveDim(){ return m_OverDriveDim; }

	// �����̃`�[���̂��T���o��
	BasePlayer* GetPlayer_TeamInSearch(SIDE side)
	{	
	
		FOR(m_NumPlayer)
		{
			if (m_pPlayers[i]->GetSide() == side)
			{
				return m_pPlayers[i];
			}
		}

		MyAssert(0, "PLAYERMGR�F������Ȃ������I");
		return m_pPlayers[0];
	}

private:
	// 1�����Ȃ�����
	static PlayerManager *pInstance;

	int m_NumPlayer;
	BasePlayer **m_pPlayers;
	Stage::Base *m_pStage;	// �Q�Ƃ��邾��(Update�̈����ł�����Ă��̂����ABaseGameEntity�̌p���ň����Ȃ���Update���g��Ȃ��Ƃ����Ȃ��Ȃ������߁A�����o�ϐ���)

	// �`�[���|�C���g
	int m_PointAteam;
	int m_PointBteam;

	// �o���Ó]
	float m_OverDriveDim;

	// ���v���C���[�ƃv���C���[�̍U���̔���
	void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_ATTACK_INFO **OutAttackInfo, HIT_DAMAGE_INFO **OutDamageInfo);

	// ���X�^���h�̍U���ɑ΂���v���C���[�̔���
	bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);

	// �������̍U���ɑ΂���v���C���[�̔���
	bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you);

	void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_ATTACK_INFO *pHitAttackInfo, HIT_DAMAGE_INFO *pHitDamageInfo);

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �V���O���g���̍�@
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// ���̉�
#define PlayerMgr (PlayerManager::GetInstance())