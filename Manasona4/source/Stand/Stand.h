#pragma once

#include "../Player/BasePlayer.h"

namespace Stand
{
	enum class ACTION_TYPE
	{
		LAND,
		SQUAT,
		AERIAL,
		AERIALDROP,
		MAX
	};
	class Base
	{
	public:
		Base(BasePlayer *pPlayer);
		virtual ~Base();
		virtual void Update();
		virtual void Render();

		// �v���C���[���y���\�i�{�^����������Ăяo��
		virtual void Action(ACTION_TYPE type) = 0;

		// �Q�b�^�[
		bool isAttackFrame()
		{
			// �U������o�Ă�
			return (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
		}
		bool isActive(){ return m_bActive; }
		bool isHit()
		{
			// �A�^�b�N�f�[�^�����������Ȃ�
			if (!m_pAttackData[(int)m_ActionType]) return false;
			return m_pAttackData[(int)m_ActionType]->bHit;
		}
		AttackData *GetAttackData(){ return m_pAttackData[(int)m_ActionType]; }
		DIR GetDir(){ return m_dir; }
		Vector3 &GetPos(){ return m_pos; }
		int GetDeviceID(){ return m_pPlayer->GetDeviceID(); }

	protected:
		BasePlayer *m_pPlayer;	// �����̎��̂������Ă���v���C���[�ւ̃|�C���^
		iex3DObj *m_pObj;		// 3D����
		Vector3 m_pos;			// ���W
		Vector3 m_move;			// �ړ��l
		DIR m_dir;				// ����
		bool m_bActive;			// �o�Ă邩�ǂ���

		FRAME_STATE m_ActionFrameList[(int)ACTION_TYPE::MAX][FRAME_MAX];
		ACTION_TYPE m_ActionType;	// �A�N�V�����̎��(���̍��̓Y�����̒��g)
		int m_CurrentActionFrame;	// �A�N�V�����t���[�����X�g�̒����Đ����Ă���t���[��(�����̉E�̓Y�����̒��g)

		void LoadActionFrameList(char *filename);

		AttackData *m_pAttackData[(int)ACTION_TYPE::MAX];	// �U���f�[�^
	};

	class Mokoi : public Base
	{
	public:
		Mokoi(BasePlayer *pPlayer);
		//~Mokoi();

		// �������z�I�[�o�[���C�h
		void Action(ACTION_TYPE type);
	};
}