#pragma once

// 前方宣言ｎ
class Camera;

// ステージのタイプ
enum class STAGE_ID{ SENJO, SYUTEN,A, MAX };

class BasePlayer;

namespace Stage
{
	// ステージの基底クラス
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

		virtual STAGE_ID GetStageID() = 0;	// ステージのID取得

		void Collision(BasePlayer *player);

	protected:
		iexMesh *m_pObj;	// メッシュの実体
		iexMesh *m_pBack;	// スカイドームとか、背景で使う
		//std::vector<iexMesh*> gimmick_meshes;
		//std::vector<GimmickBase*> gimmicks;

		float m_DeadLineY; // 超えると死ぬ
	};


	// 戦場ステージ
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

	// 終点ステージ
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

	// Aステージ
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

