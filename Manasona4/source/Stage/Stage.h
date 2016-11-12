#pragma once

#include "Data/SelectData.h"

// �O���錾��
class Camera;

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

		//virtual void Render_ShadowBuf() = 0;

		static void CreateStage(Base**p, STAGE id, Camera *pCamera);

		virtual STAGE GetStageID() = 0;	// �X�e�[�W��ID�擾

		void Collision(BasePlayer *player, Vector3 *move);
		void Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2);

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
		iexMesh *m_pObj;	// ���b�V���̎���
		iexMesh *m_pBack;	// �X�J�C�h�[���Ƃ��A�w�i�Ŏg��
		float m_fWidth;		// �X�e�[�W�̕�
		float m_fBottom;	// Y���W
	};


	// ���X�e�[�W
	class Senjo : public Base
	{
	public:
		Senjo() :Base(){}
		~Senjo(){}
		void Initialize(Camera *pCamera);
		//void Update() override;
		//void Render() override;
		//void Render_ShadowBuf() override;

		STAGE GetStageID(){ return STAGE::SENJO; }
	private:
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

		STAGE GetStageID(){ return STAGE::SYUTEN; }
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

		STAGE GetStageID(){ return STAGE::SYUTEN; }
	private:
	};

}

