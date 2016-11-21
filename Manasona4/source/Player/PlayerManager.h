#pragma once
#include "BasePlayer.h"
#include "Data/SelectData.h"
#include "BaseEntity\Message\Message.h"

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
	void Initialize(int NumPlayer, Stage::Base *pStage, SideData aSideDatas[(int)SIDE::ARRAY_MAX]);
	void Release(){ SAFE_DELETE(pInstance); }
	void Update(PLAYER_UPDATE flag);
	void UpdateHit();
	void UpdatePos();
	void Render();
	void Render(tdnShader* shader, char* name);
	void RenderShadow();
	void RenderDeferred();
	void RenderUI();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }
	BasePlayer *GetPlayerByDeviceID(int DeviceID){ FOR(m_NumPlayer) { if (m_pPlayers[i]->GetDeviceID() == DeviceID) return m_pPlayers[i]; }return nullptr; }	// �f�o�C�XID����T��

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

	//
	void SetIntroState(){ FOR(m_NumPlayer) m_pPlayers[i]->GetFSM()->ChangeState(BasePlayerState::Intro::GetInstance()); }

	void GetInput(){}

	// ���E���h���ׂ������Ƀ��Z�b�g����p
	void Reset(){ FOR(m_NumPlayer) m_pPlayers[i]->Reset(); }

	// �v���C���[�����������Ă��邢�鐔�̍��v(�������̃��E���h��)
	int GetRoundNumber()
	{
		int sum(0);
		FOR(m_NumPlayer)sum += m_pPlayers[i]->GetWinNum();
		return sum;
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

	// �q�[�z�[�h���C�u���̉��o
	bool m_bHeaveHoDriveOverFlow;

	// �o���Ó]
	float m_OverDriveDim;

	// �v���C���[�������Ă���G�ւ̌����t���O��ݒ�
	//void UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you);

	// ���v���C���[�ƃv���C���[�̍U���̔���
	//void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you,  HIT_DAMAGE_INFO **OutDamageInfo);

	// ���X�^���h�̍U���ɑ΂���v���C���[�̔���
	//bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);

	// �������̍U���ɑ΂���v���C���[�̔���
	//bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you);

	//void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo);

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �V���O���g���̍�@
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// ���̉�
#define PlayerMgr (PlayerManager::GetInstance())