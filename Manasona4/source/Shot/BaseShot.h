#pragma once

// �O���錾
class AttackData;
class BasePlayer;

namespace Shot
{

	//------------------------------------------------------
	//	��ѓ�����N���X
	//------------------------------------------------------
	class Base
	{
	public:

		//------------------------------------------------------
		//	�R���X�g���N�^
		//------------------------------------------------------
		Base(BasePlayer *pPlayer, AttackData *pAttackData, iexMesh *pObj, const Vector3 &pos, const Vector3 &vVec, const Vector3 &vVelocity, const Vector3 &vAccel = Vector3(0, 0, 0));

		//------------------------------------------------------
		//	�f�X�g���N�^
		//------------------------------------------------------
		virtual ~Base(){}

		//------------------------------------------------------
		//	�X�V
		//------------------------------------------------------
		virtual void Update()
		{
			// ��{�I�Ȉړ��ʂ̍X�V
			m_vVelocity += m_vAccel;
			m_vPos += m_vVelocity;
		}

		//------------------------------------------------------
		//	�`��
		//------------------------------------------------------
		virtual void Render();
		//virtual void Render(tdnShader *shader, char *name);

		//------------------------------------------------------
		//	�Q�b�^�[
		//------------------------------------------------------
		Vector3 &GetPos(){ return m_vPos; }
		Vector3 &GetVec(){ return m_vVec; }
		Vector3 &GetMove(){ return m_vVelocity; }
		BasePlayer *GetPlayer(){ return m_pPlayer; }
		AttackData *GetAttackData(){ return m_ptagAttackData; }
		bool EraseOK(){ return m_bErase; }

		//------------------------------------------------------
		//	�Z�b�^�[
		//------------------------------------------------------
		void Erase(){ m_bErase = true; }

	private:

		//------------------------------------------------------
		//	���b�V��
		//------------------------------------------------------
		iexMesh *m_pObj;	// ���b�V���̎���(�Q�Ƃ��邾��)


		//------------------------------------------------------
		//	�v���C���[�̎���(���܂�g�����Ƃ͂Ȃ����ꉞ�A)
		//------------------------------------------------------
		BasePlayer *m_pPlayer;

		//------------------------------------------------------
		//	�ʒu�E�����E�ړ��ʂȂǂ̏��
		//------------------------------------------------------
		Vector3 m_vPos;		// �ʒu
		Vector3 m_vVec;		// �P�ʉ����ꂽ�����x�N�g��
		Vector3 m_vVelocity;	// �ړ���
		Vector3 m_vAccel;	// �����x(Move�ɑ����Ă���)

		//------------------------------------------------------
		//	����E�U���͂Ƃ��̏��
		//------------------------------------------------------
		AttackData *m_ptagAttackData;

		//------------------------------------------------------
		//	�t���O
		//------------------------------------------------------
		bool m_bErase;		// �����t���O
	};


	class Maya :public Base
	{
	public:
		Maya(BasePlayer *pPlayer, AttackData *pAttackData, iexMesh *pObj, const Vector3 &vPos, const Vector3 &vVec);
		~Maya();
		void Update();
		void Render();
	};

}