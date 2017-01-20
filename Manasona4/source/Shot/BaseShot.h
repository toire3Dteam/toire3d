#pragma once

// �O���錾
class AttackData;
class BasePlayer;
class BaseUVEffect;
class AramitamaShotEffect;
class AramitamaRippleEffect;

namespace Shot
{
	//------------------------------------------------------
	//	�؍ݎ��ԂȂ�(���Ԃɂ����ł��Ȃ�)
	//------------------------------------------------------
	static const int c_NO_SOJOURN_TIME = -1;

	enum class TYPE
	{
		MAYA  = 100,
		MUSHI1,
		MUSHI2,
		MUSHI3
	};

	//------------------------------------------------------
	//	�V���b�g�̏������\����
	//------------------------------------------------------
	struct ParamDesc
	{
		//------------------------------------------------------
		//	�ʒu�E�����E�ړ��ʂȂ�
		//------------------------------------------------------
		Vector3 vPos;			// �ʒu
		Vector3 vVec;			// �P�ʉ����ꂽ�����x�N�g��
		Vector3 vVelocity;		// �ړ���
		Vector3 vAccel;			// �����x(Move�ɑ����Ă���)
		int		iSojournTime;	// ��ѓ���̑؍ݎ���

		//------------------------------------------------------
		//	�t���O
		//------------------------------------------------------
		bool bCollisionOK;	// ���ꂪtrue�Ȃ画�肪�L��(�ђʒe�Ƃ�������Ƃ��ɑ��i�q�b�g���Ȃ��悤�ɂ���)
		bool bPenetration;	// �ђʂ��邩�ǂ���(true�Ȃ�ǂɂԂ���܂ő��݂���)
		bool bErase;		// �����t���O

		ParamDesc() :vPos(0,0,0), vVec(0,1,0), vVelocity(0,0,0), vAccel(0,0,0), iSojournTime(c_NO_SOJOURN_TIME), bCollisionOK(true), bErase(false), bPenetration(false){}
		//ParamDesc(const Vector3 &vPos, const Vector3 &vVec, const Vector3 &vVelocity, const Vector3 &vAccel = Vector3(0, 0, 0), int iSojournTime = c_NO_SOJOURN_TIME, bool bPenetration = false, bool bCollisionOK = true) :vPos(vPos), vVec(vVec), vVelocity(vVelocity), vAccel(vAccel), iSojournTime(iSojournTime), bCollisionOK(bCollisionOK), bPenetration(bPenetration), bErase(false){}
	};

	//------------------------------------------------------
	//	��ѓ�����N���X
	//------------------------------------------------------
	class Base
	{
	public:

		//------------------------------------------------------
		//	�R���X�g���N�^�E������(�V���b�g�̏��������ɁA�R���X�g���N�^�����ɂ���ƁA�V���b�g�̏���ݒ�ł��Ȃ�)
		//------------------------------------------------------
		Base(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj, bool bAttitude);
		Base(BasePlayer *pPlayer, AttackData *pAttackData, BaseUVEffect *pObj);
		void Initialize(const ParamDesc &tagParamDesc){ memcpy_s(&m_tagParamDesc, sizeof(ParamDesc), &tagParamDesc, sizeof(ParamDesc)); }

		//------------------------------------------------------
		//	�f�X�g���N�^
		//------------------------------------------------------
		virtual ~Base(){}

		//------------------------------------------------------
		//	�X�V
		//------------------------------------------------------
		virtual void Update();

		//------------------------------------------------------
		//	�`��
		//------------------------------------------------------
		virtual void Render();
		//virtual void Render(tdnShader *shader, char *name);

		//------------------------------------------------------
		//	�Q�b�^�[
		//------------------------------------------------------
		Vector3 &GetPos(){ return m_tagParamDesc.vPos; }
		Vector3 &GetVec(){ return m_tagParamDesc.vVec; }
		Vector3 &GetMove(){ return m_tagParamDesc.vVelocity; }
		BasePlayer *GetPlayer(){ return m_pPlayer; }
		BaseUVEffect *GetUVEffect() { return m_pUVEffect; }
		iex3DObj *Get3DObj(){ return m_p3DObj; }
		AttackData *GetAttackData(){ return m_ptagAttackData; }
		bool EraseOK(){ return m_tagParamDesc.bErase; }
		bool isPenetration(){ return m_tagParamDesc.bPenetration; }
		bool isCollisionOK(){ return m_tagParamDesc.bCollisionOK; }
		virtual TYPE GetType() = 0;

		//------------------------------------------------------
		//	�Z�b�^�[
		//------------------------------------------------------
		void Erase(){ m_tagParamDesc.bErase = true; }
		void SetCollisionFlag(bool bCollisionOK){ m_tagParamDesc.bCollisionOK = bCollisionOK; }
		virtual void SetHitStopFrame(int iHitStopFrame) { m_iHitStopFrame = iHitStopFrame; }

	protected:

		//------------------------------------------------------
		//	���b�V��
		//------------------------------------------------------
		BaseUVEffect *m_pUVEffect;	// ���b�V���̎���(�Q�Ƃ��邾��)
		iex3DObj *m_p3DObj;
		bool m_bAttitude;			// �p������t���O

		//------------------------------------------------------
		//	�v���C���[�̎���(���܂�g�����Ƃ͂Ȃ����ꉞ�A)
		//------------------------------------------------------
		BasePlayer *m_pPlayer;

		//------------------------------------------------------
		//	�ʒu�E�����E�ړ��ʂȂǂ̏��
		//------------------------------------------------------
		ParamDesc m_tagParamDesc;
		int m_iHitStopFrame;

		//------------------------------------------------------
		//	����E�U���͂Ƃ��̏��
		//------------------------------------------------------
		AttackData *m_ptagAttackData;
	};


	class Maya :public Base
	{
	public:
		Maya(BasePlayer *pPlayer, AttackData *pAttackData,  BaseUVEffect *pObj, const Vector3 &vPos, const Vector3 &vVec);
		~Maya();
		void Update();
		void Render();

		TYPE GetType(){ return TYPE::MAYA; }
	};

	namespace AramitamaMushi
	{
		//------------------------------------------------------
		//	�n��ŗL�X�L��: �g�X���グ��
		//------------------------------------------------------
		class Land :public Base
		{
		public:
			Land(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj, AramitamaRippleEffect* pEffect);
			~Land();
			void Update();
			void Render();

			TYPE GetType(){ return TYPE::MUSHI1; }
		private:
			//------------------------------------------------------
			//	�؍݂��Ă��鎞�ԁE�U����������
			//------------------------------------------------------
			static const int c_SOJOURN_TIME;
			static const int c_ATTACK_FRAME;

			AramitamaRippleEffect* m_pRefShot;

			bool m_bHit;
		};

		//------------------------------------------------------
		//	���Ⴊ�݌ŗL�X�L��: 覐΂̂悤�ɓˌ�����
		//------------------------------------------------------
		class Squat :public Base
		{
		public:
			Squat(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj, AramitamaShotEffect *pEffect);
			~Squat();
			void Update();
			void Render();

			TYPE GetType(){ return TYPE::MUSHI2; }
		private:
			//------------------------------------------------------
			//	�ړ����x
			//------------------------------------------------------
			static const float c_SPEED;

			// �G�t�F�N�g
			//AramitamaShotEffect* m_pShotEF;
			AramitamaShotEffect* m_pRefShot;

		};

		//------------------------------------------------------
		//	�󒆌ŗL�X�L��: ��]���Ȃ��珸���Ă���
		//------------------------------------------------------
		class Aerial :public Base
		{
		public:
			Aerial(BasePlayer *pPlayer, AttackData *pAttackData, iex3DObj *pObj);
			~Aerial();
			void Update();
			void Render();

			TYPE GetType(){ return TYPE::MUSHI3; }
			void SetHitStopFrame(int iHitStopFrame) {}	// �q�b�g�X�g�b�v�𖳎�����

			//------------------------------------------------------
			//	�؍ݎ��ԁE�ړ����x�E�˒��͈�
			//------------------------------------------------------
			static const int c_SOJOURN_TIME;
			static const float c_SPEED;
			static const float c_RANGE;
		};
	};
}