#pragma once

//*****************************************************************************************************************************
//
//		カメラクラス
//
//*****************************************************************************************************************************

#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
class PlayerManager;
class EffectCamera;
//class Stage::Base;	// ×
namespace Stage{		// ○
	class Base;
}


class Camera :public BaseGameEntity
{
public:
	enum class MODE{ SUMABURA, EFFECT, MAX };

	//------------------------------------------------------
	//	初期化・解放
	//------------------------------------------------------
	Camera(Stage::Base *pStage, PlayerManager *pPlayerManager);	// 引数でステージヘッダに宣言してるenumを使うともれなくヘッダーにインクルードが必要なので意地でもそれを阻止する
	~Camera();

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();
	void Activate();

	//------------------------------------------------------
	//	セッター&ゲッター
	//------------------------------------------------------
	void Set(const Vector3 &pos, const Vector3 &target){ tdnView::Set(pos, target); }


	//------------------------------------------------------
	//	基本パラメータ(ステートマシンからいじりまくるので、publicにおいとく)
	//------------------------------------------------------
	Vector3	m_pos;		//	位置
	Vector3 m_ipos;		//	理想の位置
	Vector3	m_target;	//	注視点
	Vector3 m_itarget;	//	理想の注視点
	Vector3 m_angle;	//	カメラアングル
	Vector3 m_OrgPos;	// エフェクトカメラ発動時の原点座標

	//===============================================
	//	スマブラカメラの情報
	//===============================================
	struct
	{
		bool bSumabura;			// スマブラカメラフラグ
		Vector3 pos, target;
		Vector2 FullRange;// レンジ
		float FullZ;// 最大距離のZ値
		float FullY;// 最大距離のY値
		Vector2 MoveMax, MoveMin;// 最大・最少
	}m_CameraData;

	const int m_NumPlayer;				// 暇があったらどうにかしよ
	Vector3 **m_pPlayerPosReferences;	// プレイヤーの座標参照用

	// エフェクトカメラ
	EffectCamera *m_pEffectCamera;
	void OnEffectCamera(int id);
	void OffEffectCamera();

	// アクセサ
	// ★この形にするからステートマシンのアクセサを作る
	StateMachine<Camera>* GetFSM()const { return m_pStateMachine; }

private:

	// ★ステートマシン
	StateMachine<Camera> *m_pStateMachine;

	//===============================================
	//	カメラ委譲クラス(インナークラス)
	//===============================================
	class Mode
	{
	public:

		//===========================================
		//	モード基底クラス
		class Base
		{
		protected:
			Camera *m_pCamera;
			float dist;	// カメラ→プレイヤの距離
			const static float DIST;

		public:
			Base(Camera* me) :m_pCamera(me){}
			virtual void Initialize(const Vector3 &pos, const Vector3 &target) {}
			virtual void Update(){}
		};

		//===========================================
		//	自由設定カメラ
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
	//		投影関係
	//------------------------------------------------------
	struct{
		float fovY, Near, Far;
	}m_projection;


	//------------------------------------------------------
	//	その他パラメータ
	//------------------------------------------------------
	float m_MaxDist;	//	最大距離
	float m_MinDist;	//	最小距離

	bool  HandleMessage(const Message& msg);	// メッセージ更新
};


//--------------------スマブラカメラステート
class SumaburaCameraState :public State<Camera>
{
public:
	// this is a シングルトン
	static SumaburaCameraState *GetInstance(){ static SumaburaCameraState i; return &i; }

	// 入る
	void Enter(Camera *pCamera);

	// 実行します
	void Execute(Camera *pCamera);

	// 帰る
	void Exit(Camera *pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(Camera *pCamera, const Message& msg);

private:
	~SumaburaCameraState() {};

	// シングルトンの作法
	SumaburaCameraState() {};
	SumaburaCameraState(const SumaburaCameraState&);
	SumaburaCameraState& operator=(const SumaburaCameraState&);
};

//--------------------演出カメラステート
class EffectCameraState :public State<Camera>
{
public:
	// this is a シングルトン
	static EffectCameraState *GetInstance(){ static EffectCameraState i; return &i; }

	// 入る
	void Enter(Camera *pCamera);

	// 実行します
	void Execute(Camera *pCamera);

	// 帰る
	void Exit(Camera *pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(Camera *pCamera, const Message& msg);

private:
	~EffectCameraState() {};

	// シングルトンの作法
	EffectCameraState() {};
	EffectCameraState(const EffectCameraState&);
	EffectCameraState& operator=(const EffectCameraState&);
};