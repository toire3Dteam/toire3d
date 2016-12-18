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
		virtual void Render(); 
		virtual void Render(tdnShader* shader,char* name);
		virtual void RenderDeferred();
		virtual void RenderShadow();

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

		// このステージで使用するシェーダの情報
		ShaderParam GetShaderParam() { return  m_tagShaderParam; };

	protected:
		iexMesh *m_pObj;				// メッシュの実体
		iexMesh *m_pBack;				// スカイドームとか、背景で使う
		BaseUVEffect *m_pAreWall;		// エリアウォール
		float m_fWidth;					// ステージの幅
		float m_fBottom;				// Y座標

		ShaderParam m_tagShaderParam;	// シェーダの情報

	};


	// 砂ステージ
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

	// 海ステージ
	class Sea : public Base
	{
	public:
		Sea() :Base() {}
		void Initialize(Camera *pCamera);
		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::SEA_STAGE_INTRO; }
	};

	// 庭ステージ
	class Garden : public Base
	{
	public:
		Garden() :Base() {}
		void Initialize(Camera *pCamera);
		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::GARDEN_STAGE_INTRO; }
	};

	// 七里ステージ
	class NanasatoSity : public Base
	{
	public:
		NanasatoSity() :Base() {}
		void Initialize(Camera *pCamera);
		void Update();
		void Render(tdnShader* shader, char* name);

		EFFECT_CAMERA_ID GetIntroCameraScriptID() { return EFFECT_CAMERA_ID::NANASATO_STAGE_INTRO; }
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

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SYUTEN_STAGE_INTRO; }
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

		EFFECT_CAMERA_ID GetIntroCameraScriptID(){ return EFFECT_CAMERA_ID::SYUTEN_STAGE_INTRO; }
	private:
	};

}

