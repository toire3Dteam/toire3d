#pragma once

//*****************************************************************************************************************************
//
//		カメラクラス
//
//*****************************************************************************************************************************

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
class PlayerManager;
class EffectCamera;

enum class EFFECT_CAMERA_ID
{
	SAND_STAGE_INTRO	= 0,
	SEA_STAGE_INTRO		= 1,
	SYUTEN_STAGE_INTRO	= 2,
	GARDEN_STAGE_INTRO	= 3,
	NANASATO_STAGE_INTRO= 4,
	AIROU_OVERFLOW		= 5,
	TEKI_OVERFLOW		= 6,
	NAZENARA_OVERFLOW	= 7,
	OVERFLOW_TEST		= 8,
};

// カメラクラス<Singleton>
class Camera :public BaseGameEntity
{
public:
	enum class MODE{ SUMABURA, EFFECT, MAX };

	//------------------------------------------------------
	//	実体取得
	//------------------------------------------------------
	static Camera *GetInstance(){ static Camera instance; return &instance; }

	//------------------------------------------------------
	//	初期化・解放
	//------------------------------------------------------
	//Camera();
	void InitializeResult();
	~Camera();

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();
	void Activate();

	void DebugRender();

	//------------------------------------------------------
	//	セッター&ゲッター
	//------------------------------------------------------
	int GetEventFrame();
	void Set(const Vector3 &pos, const Vector3 &target){ tdnView::Set(pos, target); }
	void SetStageCameraInfo(char *path);	// ステージごとのスマブラカメラの設定
	void SetPlayersPos();

	//------------------------------------------------------
	//	基本パラメータ(ステートマシンからいじりまくるので、publicにおいとく)
	//------------------------------------------------------
	ViewData m_ViewData;
	//Vector3	m_pos;		//	位置
	Vector3 m_ipos;		//	理想の位置
	//Vector3	m_target;	//	注視点
	Vector3 m_itarget;	//	理想の注視点
	Vector3 m_angle;	//	カメラアングル
	//Vector3 m_OrgPos;	// エフェクトカメラ発動時の原点座標

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
	}m_GameCameraData;

	int m_NumPlayer;					// 暇があったらどうにかしよ
	Vector3 **m_pPlayerPosReferences;	// プレイヤーの座標参照用

	// エフェクトカメラ
	EffectCamera *m_pEffectCamera;
	void OnEffectCamera(int id);
	void OffEffectCamera();

	// アクセサ
	// ★この形にするからステートマシンのアクセサを作る
	StateMachine<Camera>* GetFSM()const { return m_pStateMachine; }

private:
	Camera();
	//Camera(const Camera&){}
	//Camera &operator=(const Camera&){}

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

#define CameraMgr (Camera::GetInstance())


//--------------------グローバルステート
class GlobalCameraState : public State<Camera>
{
public:
	// this is a シングルトン
	static GlobalCameraState *GetInstance(){ static GlobalCameraState i; return &i; }

	// 入る
	void Enter(Camera *pCamera);

	// 実行します
	void Execute(Camera *pCamera);

	// 帰る
	void Exit(Camera *pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(Camera *pCamera, const Message& msg);

private:

	struct ShakeData
	{
		float power, MaxPower;
		int frame, MaxFrame;
		const float rate;
		Vector3 ShakedPos, ShakedTarget;

		ShakeData();

		void Start(float power, unsigned int frame);

		void Update(Camera *pCamera);
	}m_ShakeData;

	~GlobalCameraState() {};

	// シングルトンの作法
	GlobalCameraState() {};
	GlobalCameraState(const GlobalCameraState&);
	GlobalCameraState& operator=(const GlobalCameraState&);
};

//--------------------固定カメラステート
class FixCameraState :public State<Camera>
{
public:
	// this is a シングルトン
	static FixCameraState *GetInstance(){ static FixCameraState i; return &i; }

	// 入る
	void Enter(Camera *pCamera){}

	// 実行します
	void Execute(Camera *pCamera){}

	// 帰る
	void Exit(Camera *pCamera){}

	// エージェントからのメッセージを受信した場合、これが実行される
	bool OnMessage(Camera *pCamera, const Message& msg){ return false; }

private:
	~FixCameraState() {};

	// シングルトンの作法
	FixCameraState() {};
	FixCameraState(const FixCameraState&){}
	FixCameraState& operator=(const FixCameraState&){}
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