#pragma once

// �O���錾��
class Camera;

// �X�e�[�W�̃^�C�v
enum class STAGE_ID{ SENJO, SYUTEN,A, MAX };

class BasePlayer;

namespace Stage
{
	// �X�e�[�W�̊��N���X
	class Base
	{
	public:
		Base();
		virtual ~Base();

		virtual void Initialize(Camera *pCamera) = 0;
		virtual void Update();
		virtual void Render(); 
		virtual void Render(tdnShader* shader,char* name);
		virtual void RenderDeferred();
		virtual void RenderShadow();

		//virtual void Render_ShadowBuf() = 0;

		static void CreateStage(Base**p, STAGE_ID id, Camera *pCamera);

		virtual STAGE_ID GetStageID() = 0;	// �X�e�[�W��ID�擾

		void Collision(BasePlayer *player);

	protected:
		iexMesh *m_pObj;	// ���b�V���̎���
		iexMesh *m_pBack;	// �X�J�C�h�[���Ƃ��A�w�i�Ŏg��
		//std::vector<iexMesh*> gimmick_meshes;
		//std::vector<GimmickBase*> gimmicks;

		float m_DeadLineY; // ������Ǝ���
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

		STAGE_ID GetStageID(){ return STAGE_ID::SENJO; }
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

		STAGE_ID GetStageID(){ return STAGE_ID::SYUTEN; }
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

		STAGE_ID GetStageID(){ return STAGE_ID::SYUTEN; }
	private:
	};
}

