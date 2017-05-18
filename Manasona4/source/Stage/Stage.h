#pragma once

#include "Data/SelectData.h"
#include "../Camera/camera.h"
#include "Effect\UVEffect\BaseUVEffect.h"

// �O���錾��

class BasePlayer;

namespace Stage
{
	// [1206]
	// �X�e�[�W���ɃV�F�[�_�ɓn���p�����[�^�[
	struct ShaderParam
	{
		Vector3 vDirLightVec;	// ���z�̌���
		Vector3 vDirLightColor;	// ���z�̌��̐F

		Vector3 vSkyColor;		// ����(��)
		Vector3 vGroundColor;	// ����(�n��)
	};

	// �X�e�[�W�̊��N���X
	class Base
	{
	public:
		Base();
		virtual ~Base();

		virtual void Initialize(Camera *pCamera){}
		virtual void Update();
		virtual void RenderCopy(); 
		virtual void Render() = 0;// �������z�֐���
		virtual void Render(tdnShader* shader,char* name);
		virtual void RenderDeferred();
		virtual void RenderShadow();
		virtual void RenderForward() {};

		void ActionWall();
		void StopWall();

		//virtual void Render_ShadowBuf() = 0;

		static void CreateStage(Base**p, STAGE id, Camera *pCamera);

		virtual EFFECT_CAMERA_ID GetIntroCameraScriptID() = 0;	// �X�e�[�W��ID�擾

		void Collision(BasePlayer *player, Vector3 *move);
		//void Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2);

		void ChangeObj(char *ObjFileName, char *BackFileName)
		{
			if (m_pObj) delete m_pObj;
			if (m_pBack) delete m_pBack;
			m_pObj = (ObjFileName) ? new iexMesh(ObjFileName) : nullptr;
			m_pBack = (BackFileName) ? new iexMesh(BackFileName) : nullptr;
		}

		iexMesh *GetObj(){ return m_pObj; }

		float GetBottom(){ return m_fBottom; }
		float GetWidth(){ return m_fWidth; }

		bool isBakeShadow() { return m_bBakeShadow; }

		// ���̃X�e�[�W�Ŏg�p����V�F�[�_�̏��
		ShaderParam GetShaderParam() { return  m_tagShaderParam; };

	protected:
		iexMesh *m_pObj;				// ���b�V���̎���
		Vector3 m_vStagePos;			// �X�e�[�W�̏����ʒu 
		iexMesh *m_pBack;				// �X�J�C�h�[���Ƃ��A�w�i�Ŏg��
		iexMesh *m_pBack2;				// �X�J�C�h�[���Ƃ��A�w�i�Ŏg��
		BaseUVEffect *m_pAreWall;		// �G���A�E�H�[��
		float m_fWidth;					// �X�e�[�W�̕�
		float m_fBottom;				// Y���W
		bool m_bBakeShadow;				// �V���h�E�}�b�v����񂾂��Ă����܂��邩

		ShaderParam m_tagShaderParam;	// �V�F�[�_�̏��

	};


	// ���X�e�[�W
	class Sand : public Base
	{
	public:
		Sand() :Base(){}
		~Sand();
		void Initialize(Camera *pCamera);
		void Update() override;
		void Render() override;
		//void Render_ShadowBuf() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SAND_STAGE_INTRO; }
	private:

		static const int m_cGRASS_MAX = 10;
		struct Grass
		{
			iex3DObj* pObj;
			Vector3 vPos;
		};
		Grass m_pGrass[m_cGRASS_MAX];
	
		static const int m_cGRASS_MAX2 = 4;
		struct Grass2
		{
			iex3DObj* pObj;
			Vector3 vPos;
		};
		Grass2 m_pGrass2[m_cGRASS_MAX2];


	};

	// �C�X�e�[�W
	class Sea : public Base
	{
	public:
		Sea() :Base() {}
		~Sea();
		void Initialize(Camera *pCamera);
		void Update() override;
		void Render() override;
		void RenderDeferred()override;
		void RenderForward() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::SEA_STAGE_INTRO; }

	private:
		float		m_fUvSea;
		iexMesh*	m_pSea;
		tdn2DObj*	m_pEnvSea;

		struct Ship
		{
			iex3DObj* pObj;
			Vector3 vPos;
			float fAngle;
			float fTurnStartAngle;
			bool bTurn;

			Ship();
			~Ship() { SAFE_DELETE(pObj); }
			void Update();
			void Render(tdnShader *shader, char *name) { pObj->Render(shader, name); }
		};
		Ship   m_tagShip;
		
	};

	// ��X�e�[�W
	class Garden : public Base
	{
	public:
		Garden() :Base() {}
		void Initialize(Camera *pCamera);
		void Render() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::GARDEN_STAGE_INTRO; }
	};

	// �����X�e�[�W
	class NanasatoSity : public Base
	{
	public:
		NanasatoSity() :Base() {}
		~NanasatoSity();
		void Initialize(Camera *pCamera);
		void Update();
		void Render() override;
		void Render(tdnShader* shader, char* name);
		
		void RenderForward() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::NANASATO_STAGE_INTRO; }
	
	private:
		float m_fUvWater;
		float m_fWaterHeight;
		iexMesh* m_pWater;
		//Surface*	m_pStencilSurface;	// ���s
		//tdn2DObj*	m_pWaterEnvScreen;	// �Q�[�����


	};

	// �I�_�X�e�[�W
	class Syuten : public Base
	{
	public:
		Syuten() :Base(){}
		~Syuten();
		void Initialize(Camera *pCamera);
		void Update() override;
		void Render() override;
		//void Render_ShadowBuf() override;
		void RenderForward() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SYUTEN_STAGE_INTRO; }
	private:

		float m_fUvWater;
		float m_fWaterHeight;
		iexMesh* m_pWater;
	};

	// A�X�e�[�W
	class A : public Base
	{
	public:
		A() :Base(){}
		~A(){}
		void Initialize(Camera *pCamera);
		void Update() override;
		void Render() override;
		//void Render_ShadowBuf() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SYUTEN_STAGE_INTRO; }
	private:
	};

}

