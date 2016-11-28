#pragma once

#include "Data/SelectData.h"
#include "../Camera/camera.h"
#include "Effect\UVEffect\BaseUVEffect.h"

// �O���錾��

class BasePlayer;

namespace Stage
{
	// �X�e�[�W�̊��N���X
	class Base
	{
	public:
		Base();
		virtual ~Base();

		virtual void Initialize(Camera *pCamera){}
		virtual void Update();
		virtual void Render(); 
		virtual void Render(tdnShader* shader,char* name);
		virtual void RenderDeferred();
		virtual void RenderShadow();

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

	protected:
		iexMesh *m_pObj;			// ���b�V���̎���
		iexMesh *m_pBack;			// �X�J�C�h�[���Ƃ��A�w�i�Ŏg��
		BaseUVEffect *m_pAreWall;	// �G���A�E�H�[��
		float m_fWidth;				// �X�e�[�W�̕�
		float m_fBottom;			// Y���W
	};


	// ���X�e�[�W
	class Sand : public Base
	{
	public:
		Sand() :Base(){}
		void Initialize(Camera *pCamera);
		//void Update() override;
		//void Render() override;
		//void Render_ShadowBuf() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SAND_STAGE_INTRO; }
	private:
	};

	// �C�X�e�[�W
	class Sea : public Base
{
public:
Sea() :Base(){}
void Initialize(Camera *pCamera);
		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SEA_STAGE_INTRO; }
};

	// �I�_�X�e�[�W
	class Syuten : public Base
	{
	public:
		Syuten() :Base(){}
		~Syuten(){}
		void Initialize(Camera *pCamera);
		void Update() override;
		void Render() override;
		//void Render_ShadowBuf() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SYUTEN_STAGE_INTRO; }
	private:
	};

	// A�X�e�[�W
	class A : public Base
	{
	public:
		A() :Base(){}
		~A(){}
		void Initialize(Camera *pCamera);
		//void Update() override;
		//void Render() override;
		//void Render_ShadowBuf() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SYUTEN_STAGE_INTRO; }
	private:
	};

}

