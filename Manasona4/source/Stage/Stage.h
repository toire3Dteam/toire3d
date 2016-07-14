#pragma once

// �O���錾��
class Camera;

// �X�e�[�W�̃^�C�v
enum class STAGE_ID{ SENJO, MAX };

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
		//virtual void Render_ShadowBuf() = 0;

		static void CreateStage(Base**p, STAGE_ID id, Camera *pCamera);

		virtual STAGE_ID GetStageID() = 0;	// �X�e�[�W��ID�擾

		void Collision(BasePlayer *player);

	protected:
		iexMesh *m_pObj;	// ���b�V���̎���
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
}

