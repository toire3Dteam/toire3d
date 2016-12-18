#pragma once

//*****************************************************************************************************************************
//
//		�J�����N���X
//
//*****************************************************************************************************************************

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// �O���錾
class PlayerManager;
class EffectCamera;

enum class EFFECT_CAMERA_ID
{
	SAND_STAGE_INTRO	= 0,
	SEA_STAGE_INTRO		= 1,
	SYUTEN_STAGE_INTRO	= 2,
	GARDEN_STAGE_INTRO	= 3,
	NANASATO_STAGE_INTRO= 4,
	AIROU_OVERFLOW		= 5,
	TEKI_OVERFLOW		= 6,
	NAZENARA_OVERFLOW	= 7,
	OVERFLOW_TEST		= 8,
};

// �J�����N���X<Singleton>
class Camera :public BaseGameEntity
{
public:
	enum class MODE{ SUMABURA, EFFECT, MAX };

	//------------------------------------------------------
	//	���̎擾
	//------------------------------------------------------
	static Camera *GetInstance(){ static Camera instance; return &instance; }

	//------------------------------------------------------
	//	�������E���
	//------------------------------------------------------
	//Camera();
	void InitializeResult();
	~Camera();

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update();
	void Activate();

	void DebugRender();

	//------------------------------------------------------
	//	�Z�b�^�[&�Q�b�^�[
	//------------------------------------------------------
	int GetEventFrame();
	void Set(const Vector3 &pos, const Vector3 &target){ tdnView::Set(pos, target); }
	void SetStageCameraInfo(char *path);	// �X�e�[�W���Ƃ̃X�}�u���J�����̐ݒ�
	void SetPlayersPos();

	//------------------------------------------------------
	//	��{�p�����[�^(�X�e�[�g�}�V�����炢����܂���̂ŁApublic�ɂ����Ƃ�)
	//------------------------------------------------------
	ViewData m_ViewData;
	//Vector3	m_pos;		//	�ʒu
	Vector3 m_ipos;		//	���z�̈ʒu
	//Vector3	m_target;	//	�����_
	Vector3 m_itarget;	//	���z�̒����_
	Vector3 m_angle;	//	�J�����A���O��
	//Vector3 m_OrgPos;	// �G�t�F�N�g�J�����������̌��_���W

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
	}m_GameCameraData;

	int m_NumPlayer;					// �ɂ���������ǂ��ɂ�����
	Vector3 **m_pPlayerPosReferences;	// �v���C���[�̍��W�Q�Ɨp

	// �G�t�F�N�g�J����
	EffectCamera *m_pEffectCamera;
	void OnEffectCamera(int id);
	void OffEffectCamera();

	// �A�N�Z�T
	// �����̌`�ɂ��邩��X�e�[�g�}�V���̃A�N�Z�T�����
	StateMachine<Camera>* GetFSM()const { return m_pStateMachine; }

private:
	Camera();
	//Camera(const Camera&){}
	//Camera &operator=(const Camera&){}

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

#define CameraMgr (Camera::GetInstance())


//--------------------�O���[�o���X�e�[�g
class GlobalCameraState : public State<Camera>
{
public:
	// this is a �V���O���g��
	static GlobalCameraState *GetInstance(){ static GlobalCameraState i; return &i; }

	// ����
	void Enter(Camera *pCamera);

	// ���s���܂�
	void Execute(Camera *pCamera);

	// �A��
	void Exit(Camera *pCamera);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(Camera *pCamera, const Message& msg);

private:

	struct ShakeData
	{
		float power, MaxPower;
		int frame, MaxFrame;
		const float rate;
		Vector3 ShakedPos, ShakedTarget;

		ShakeData();

		void Start(float power, unsigned int frame);

		void Update(Camera *pCamera);
	}m_ShakeData;

	~GlobalCameraState() {};

	// �V���O���g���̍�@
	GlobalCameraState() {};
	GlobalCameraState(const GlobalCameraState&);
	GlobalCameraState& operator=(const GlobalCameraState&);
};

//--------------------�Œ�J�����X�e�[�g
class FixCameraState :public State<Camera>
{
public:
	// this is a �V���O���g��
	static FixCameraState *GetInstance(){ static FixCameraState i; return &i; }

	// ����
	void Enter(Camera *pCamera){}

	// ���s���܂�
	void Execute(Camera *pCamera){}

	// �A��
	void Exit(Camera *pCamera){}

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	bool OnMessage(Camera *pCamera, const Message& msg){ return false; }

private:
	~FixCameraState() {};

	// �V���O���g���̍�@
	FixCameraState() {};
	FixCameraState(const FixCameraState&){}
	FixCameraState& operator=(const FixCameraState&){}
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