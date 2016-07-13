#pragma once

//*****************************************************************************************************************************
//
//		�J�����N���X
//
//*****************************************************************************************************************************

#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// �O���錾
class PlayerManager;
class EffectCamera;
//class Stage::Base;	// �~
namespace Stage{		// ��
	class Base;
}


class Camera :public BaseGameEntity
{
public:
	enum class MODE{ SUMABURA, EFFECT, MAX };

	//------------------------------------------------------
	//	�������E���
	//------------------------------------------------------
	Camera(Stage::Base *pStage, PlayerManager *pPlayerManager);	// �����ŃX�e�[�W�w�b�_�ɐ錾���Ă�enum���g���Ƃ���Ȃ��w�b�_�[�ɃC���N���[�h���K�v�Ȃ̂ňӒn�ł������j�~����
	~Camera();

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update();
	void Activate();

	//------------------------------------------------------
	//	�Z�b�^�[&�Q�b�^�[
	//------------------------------------------------------
	void Set(const Vector3 &pos, const Vector3 &target){ tdnView::Set(pos, target); }


	//------------------------------------------------------
	//	��{�p�����[�^(�X�e�[�g�}�V�����炢����܂���̂ŁApublic�ɂ����Ƃ�)
	//------------------------------------------------------
	Vector3	m_pos;		//	�ʒu
	Vector3 m_ipos;		//	���z�̈ʒu
	Vector3	m_target;	//	�����_
	Vector3 m_itarget;	//	���z�̒����_
	Vector3 m_angle;	//	�J�����A���O��
	Vector3 m_OrgPos;	// �G�t�F�N�g�J�����������̌��_���W

	//===============================================
	//	�X�}�u���J�����̏��
	//===============================================
	struct
	{
		bool bSumabura;			// �X�}�u���J�����t���O
		Vector3 pos, target;
		Vector2 FullRange;// �����W
		float FullZ;// �ő勗����Z�l
		float FullY;// �ő勗����Y�l
		Vector2 MoveMax, MoveMin;// �ő�E�ŏ�
	}m_CameraData;

	const int m_NumPlayer;				// �ɂ���������ǂ��ɂ�����
	Vector3 **m_pPlayerPosReferences;	// �v���C���[�̍��W�Q�Ɨp

	// �G�t�F�N�g�J����
	EffectCamera *m_pEffectCamera;
	void OnEffectCamera(int id);
	void OffEffectCamera();

	// �A�N�Z�T
	// �����̌`�ɂ��邩��X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<Camera>* GetFSM()const { return m_pStateMachine; }

private:

	// ���X�e�[�g�}�V��
	StateMachine<Camera> *m_pStateMachine;

	//===============================================
	//	�J�����Ϗ��N���X(�C���i�[�N���X)
	//===============================================
	class Mode
	{
	public:

		//===========================================
		//	���[�h���N���X
		class Base
		{
		protected:
			Camera *m_pCamera;
			float dist;	// �J�������v���C���̋���
			const static float DIST;

		public:
			Base(Camera* me) :m_pCamera(me){}
			virtual void Initialize(const Vector3 &pos, const Vector3 &target) {}
			virtual void Update(){}
		};

		//===========================================
		//	���R�ݒ�J����
		class Free : public Base
		{
		private:
			Vector3 add_pos;
		public:
			Free(Camera *me) :Base(me){}

			void Initialize(const Vector3 &pos, const Vector3 &target);
			void Update();
		};
	};

	//------------------------------------------------------
	//		���e�֌W
	//------------------------------------------------------
	struct{
		float fovY, Near, Far;
	}m_projection;


	//------------------------------------------------------
	//	���̑��p�����[�^
	//------------------------------------------------------
	float m_MaxDist;	//	�ő勗��
	float m_MinDist;	//	�ŏ�����

	bool  HandleMessage(const Message& msg);	// ���b�Z�[�W�X�V
};


//--------------------�X�}�u���J�����X�e�[�g
class SumaburaCameraState :public State<Camera>
{
public:
	// this is a �V���O���g��
	static SumaburaCameraState *GetInstance(){ static SumaburaCameraState i; return &i; }

	// ����
	void Enter(Camera *pCamera);

	// ���s���܂�
	void Execute(Camera *pCamera);

	// �A��
	void Exit(Camera *pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(Camera *pCamera, const Message& msg);

private:
	~SumaburaCameraState() {};

	// �V���O���g���̍�@
	SumaburaCameraState() {};
	SumaburaCameraState(const SumaburaCameraState&);
	SumaburaCameraState& operator=(const SumaburaCameraState&);
};

//--------------------���o�J�����X�e�[�g
class EffectCameraState :public State<Camera>
{
public:
	// this is a �V���O���g��
	static EffectCameraState *GetInstance(){ static EffectCameraState i; return &i; }

	// ����
	void Enter(Camera *pCamera);

	// ���s���܂�
	void Execute(Camera *pCamera);

	// �A��
	void Exit(Camera *pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(Camera *pCamera, const Message& msg);

private:
	~EffectCameraState() {};

	// �V���O���g���̍�@
	EffectCameraState() {};
	EffectCameraState(const EffectCameraState&);
	EffectCameraState& operator=(const EffectCameraState&);
};