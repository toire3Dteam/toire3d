#pragma once

#include "Data/SelectData.h"
#include "../Camera/camera.h"
#include "Effect\UVEffect\BaseUVEffect.h"

// 前方宣言ｎ

class BasePlayer;

namespace Stage
{
	// [1206]
	// ステージ毎にシェーダに渡すパラメーター
	struct ShaderParam
	{
		Vector3 vDirLightVec;	// 太陽の向き
		Vector3 vDirLightColor;	// 太陽の光の色

		Vector3 vSkyColor;		// 環境光(空)
		Vector3 vGroundColor;	// 環境光(地面)
	};

	// ステージの基底クラス
	class Base
	{
	public:
		Base();
		virtual ~Base();

		virtual void Initialize(Camera *pCamera){}
		virtual void Update();
		virtual void RenderCopy(); 
		virtual void Render() = 0;// 純粋仮想関数に
		virtual void Render(tdnShader* shader,char* name);
		virtual void RenderDeferred();
		virtual void RenderShadow();
		virtual void RenderForward() {};

		void ActionWall();
		void StopWall();

		//virtual void Render_ShadowBuf() = 0;

		static void CreateStage(Base**p, STAGE id, Camera *pCamera);

		virtual EFFECT_CAMERA_ID GetIntroCameraScriptID() = 0;	// ステージのID取得

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

		// このステージで使用するシェーダの情報
		ShaderParam GetShaderParam() { return  m_tagShaderParam; };

	protected:
		iexMesh *m_pObj;				// メッシュの実体
		Vector3 m_vStagePos;			// ステージの初期位置 
		iexMesh *m_pBack;				// スカイドームとか、背景で使う
		iexMesh *m_pBack2;				// スカイドームとか、背景で使う
		BaseUVEffect *m_pAreWall;		// エリアウォール
		float m_fWidth;					// ステージの幅
		float m_fBottom;				// Y座標
		bool m_bBakeShadow;				// シャドウマップを一回だけ焼きこませるか

		ShaderParam m_tagShaderParam;	// シェーダの情報

	};


	// 砂ステージ
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

	// 海ステージ
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

	// 庭ステージ
	class Garden : public Base
	{
	public:
		Garden() :Base() {}
		void Initialize(Camera *pCamera);
		void Render() override;

		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::GARDEN_STAGE_INTRO; }
	};

	// 七里ステージ
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
		//Surface*	m_pStencilSurface;	// 奥行
		//tdn2DObj*	m_pWaterEnvScreen;	// ゲーム画面


	};

	// 終点ステージ
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

	// Aステージ
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

