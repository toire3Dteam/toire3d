#include "TDNLIB.h"
#include "../BaseEntity/Message/Message.h"
#include "camera.h"
#include "EffectCamera.h"
#include "../Stage/Stage.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"

//*****************************************************************************************************************************
//
//		カメラクラス
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//	初期化・解放
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
Camera::Camera() :BaseGameEntity(ENTITY_ID::CAMERA_MGR),	// エンティティIDの登録
m_angle(Vector3(0, 0, 0)), m_pos(Vector3(0, 20, -100)), m_ipos(m_pos), m_target(Vector3(0, 0, 0)), m_OrgPos(Vector3(0,0,0)),
m_NumPlayer(-1), m_pPlayerPosReferences(nullptr)
{
	//	ビュー設定
	tdnView::Init();
	m_projection.fovY = D3DX_PI / 4;
	m_projection.Near = .1f;
	m_projection.Far = 2000.0f;

	m_MaxDist = 25.0f;
	m_MinDist = 22.0f;

	m_pEffectCamera = new EffectCamera(this);

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<Camera>(this);
	m_pStateMachine->SetGlobalState(GlobalCameraState::GetInstance());	// グローバルステートの設定
}

void Camera::InitializeResult()
{
	if (!m_pStateMachine->GetCurrentState()) m_pStateMachine->SetCurrentState(EffectCameraState::GetInstance());
	m_pEffectCamera->Start(0);
}

void Camera::SetPlayersPos()
{
	// プレイヤー数取得
	m_NumPlayer = PlayerMgr->GetNumPlayer();

	// スマブラカメラ用にプレイヤー達の座標のアドレスを取得する
	if (m_pPlayerPosReferences) delete[] m_pPlayerPosReferences;	// 既存のバッファの解放
	m_pPlayerPosReferences = new Vector3*[m_NumPlayer];
	FOR(m_NumPlayer)
	{
		// プレイヤーの座標を参照する
		m_pPlayerPosReferences[i] = PlayerMgr->GetPlayer(i)->GetPosAddress();
	}
}

void Camera::SetStageCameraInfo(char *path)
{
	std::fstream ifs(path);
	MyAssert(ifs, "スマブラカメラのテキストパス間違ってるよ！");

	char skip[64];	// 読み飛ばし用変数

	// カメラタイプ読み込み
	ifs >> skip;
	char cType[16];
	ifs >> cType;
	m_CameraData.bSumabura = (strcmp(cType, "SUMABURA") == 0);	// スマブラかどうか

	// XYの範囲
	ifs >> skip;
	ifs >> m_CameraData.FullRange.x;
	ifs >> m_CameraData.FullRange.y;

	// YZの何ちゃら
	ifs >> skip;
	ifs >> m_CameraData.FullZ;
	ifs >> skip;
	ifs >> m_CameraData.FullY;

	// XYの最大値
	ifs >> skip;
	ifs >> m_CameraData.MoveMax.x;
	ifs >> m_CameraData.MoveMax.y;

	// XYの最小値
	ifs >> skip;
	ifs >> m_CameraData.MoveMin.x;
	ifs >> m_CameraData.MoveMin.y;

	// スマブラカメラステージなら
	if (m_CameraData.bSumabura)
		m_pStateMachine->SetCurrentState(SumaburaCameraState::GetInstance());
	else assert(0);
}

//------------------------------------------------------
//	解放
//------------------------------------------------------
Camera::~Camera()
{
	if(m_pPlayerPosReferences) delete[] m_pPlayerPosReferences;
	delete m_pStateMachine;
	delete m_pEffectCamera;
}

//*****************************************************************************************************************************
//		更新
//*****************************************************************************************************************************
//------------------------------------------------------
//		更新
//------------------------------------------------------
void Camera::Update()
{
	//	モードごとの処理
	m_pStateMachine->Update();

	// 座標と注視点設定(グローバルステートでやる)
	//tdnView::Set(m_pos, m_target);

	// 投影設定
	tdnView::SetProjection(m_projection.fovY, m_projection.Near, m_projection.Far);
}

//*****************************************************************************************************************************
//	描画準備
//*****************************************************************************************************************************
void Camera::Activate()
{
	tdnView::Activate();
	tdnView::Clear();

	//Text::Draw(32, 64, 0xffffff00, "CameraFar : %.1f", projection.Far);
	//Text::Draw(32, 128, 0xffffff00, "CameraFov : %.2f", projection.fovY);

	//if (KeyBoard(KB_NUMPAD4)) m_angle.z += .05f;
	//if (KeyBoard(KB_NUMPAD6)) m_angle.z -= .05f;
	//if (KeyBoard(KB_3)) projection.fovY -= .01f;
	//if (KeyBoard(KB_4)) projection.fovY += .01f;
}

bool Camera::HandleMessage(const Message &msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void Camera::OnEffectCamera(int id)
{
	//m_pStateMachine->SetCurrentState();	// エフェクトカメラに切り替え
	m_pStateMachine->ChangeState(EffectCameraState::GetInstance());
	m_pEffectCamera->Start(id);											// エフェクトカメラスタート
}

void Camera::OffEffectCamera()
{
	//m_pStateMachine->SetCurrentState(m_pStateMachine->GetPreviousState());	// 1個前のステートに戻す
	m_pStateMachine->RevertToPreviousState();
	m_angle.z = 0;
}


//*****************************************************************************************************************************
//
//		カメラ振動
GlobalCameraState::ShakeData::ShakeData() :power(0), MaxPower(0),frame(0), MaxFrame(0), ShakedPos(0, 0, 0), ShakedTarget(0.0f, 0.0f, 0.0f),
rate(5.0f)	// 振動の強さ基準(rate * 0～1.0f)
{}



void GlobalCameraState::ShakeData::Start(float power, unsigned int frame)
{
	this->power = MaxPower = (power * rate);
	this->frame = MaxFrame = frame;
}

void GlobalCameraState::ShakeData::Update(Camera *camera)
{
	ShakedTarget = camera->m_target;
	ShakedPos = camera->m_pos;

	if (MaxFrame == 0) return;

	if (frame > 0)
	{
		power = MaxPower * (frame / (float)MaxFrame);
		const float x(tdnRandom::Get(-1.0f, 1.0f) * power), y(tdnRandom::Get(-1.0f, 1.0f) * power);
		ShakedPos.x += x;
		ShakedPos.y += y;
		ShakedTarget.x += x;
		ShakedTarget.y += y;

		if (--frame <= 0)
		{
			MaxFrame = 0;
			power = MaxPower = 0.0f;
		}
	}
}
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//		グローバルステート
void GlobalCameraState::Enter(Camera *pCamera)
{

}

void GlobalCameraState::Execute(Camera *pCamera)
{
	// カメラ振動更新
	m_ShakeData.Update(pCamera);

	// 座標と注視点の設定	★グローバルステートでやる
	tdnView::Set(pCamera->m_pos, m_ShakeData.ShakedTarget, pCamera->m_angle.z);
}

// 出口
void GlobalCameraState::Exit(Camera *pCamera)
{

}

bool GlobalCameraState::OnMessage(Camera *pCamera, const Message &msg)
{

	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SHAKE_CAMERA:	// 振動メッセージ
		// オリジナル構造体を受け取る
		SHAKE_CAMERA_INFO *info = (SHAKE_CAMERA_INFO*)msg.ExtraInfo;
		m_ShakeData.Start(info->ShakePower, info->ShakeFrame);
		break;
	}

	return false;
}
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//		スマブラカメラ
void SumaburaCameraState::Enter(Camera *pCamera)
{

}

void SumaburaCameraState::Execute(Camera *pCamera)
{
	float MAX_X = -1000, MAX_Y = -1000, MIN_X = +1000, MIN_Y = +1000;
	//補正こみの幅
	FOR(pCamera->m_NumPlayer)
	{
		//最大X
		if (MAX_X<pCamera->m_pPlayerPosReferences[i]->x + 10)
		{
			MAX_X = pCamera->m_pPlayerPosReferences[i]->x + 10;
		}
		//最小X
		if (MIN_X>pCamera->m_pPlayerPosReferences[i]->x - 10)
		{
			MIN_X = pCamera->m_pPlayerPosReferences[i]->x - 10;
		}

		//最大Y
		if (MAX_Y<pCamera->m_pPlayerPosReferences[i]->y + 10)
		{
			MAX_Y = pCamera->m_pPlayerPosReferences[i]->y + 10;
		}
		//最小Y
		if (MIN_Y>pCamera->m_pPlayerPosReferences[i]->y - 10)
		{
			MIN_Y = pCamera->m_pPlayerPosReferences[i]->y - 10;
		}
	}

	MAX_X = Math::Clamp(MAX_X, pCamera->m_CameraData.MoveMin.x, pCamera->m_CameraData.MoveMax.x);
	MAX_Y = Math::Clamp(MAX_Y, pCamera->m_CameraData.MoveMin.y, pCamera->m_CameraData.MoveMax.y);

	//　次に幅を求めるのと真ん中を求める
	float senter_X, senter_Y;
	float range_X, range_Y;
	range_X = abs(MAX_X - MIN_X);//今のXの幅
	range_Y = abs(MAX_Y - MIN_Y);//今のYの幅

	senter_X = (MAX_X + MIN_X) / 2;//中心X取得
	senter_Y = (MAX_Y + MIN_Y) / 2;//中心Y取得

	float percent_X = 0.0f, percent_Y = 0.0f;

	percent_X = range_X / pCamera->m_CameraData.FullRange.x;
	percent_Y = range_Y / pCamera->m_CameraData.FullRange.y;

	//　パーセント取得
	float percent = 0.0f;
	if (percent_X > percent_Y)percent = percent_X;
	else percent = percent_Y;

	//Z値
	pCamera->m_ipos.z = (pCamera->m_CameraData.FullZ * percent) - 40;//パーセントによってカメラを近づけさす －40は補正

	//　動けるX＆Y
	float max_move_x = pCamera->m_CameraData.MoveMax.x * (1.0f - percent);
	float max_move_y = pCamera->m_CameraData.MoveMax.y * (1.0f - percent);
	float min_move_x = pCamera->m_CameraData.MoveMin.x * (1.0f - percent);
	float min_move_y = pCamera->m_CameraData.MoveMin.y * (1.0f - percent);

	//　まずは中心に
	pCamera->m_ipos.x = senter_X;
	pCamera->m_ipos.y = senter_Y;
	pCamera->m_itarget.x = senter_X;
	pCamera->m_itarget.y = senter_Y;

	//補正
	if (max_move_x <= pCamera->m_ipos.x)
	{
		pCamera->m_ipos.x = max_move_x;
		pCamera->m_itarget.x = max_move_x;
	}
	if (min_move_x >= pCamera->m_ipos.x)
	{
		pCamera->m_ipos.x = min_move_x;
		pCamera->m_itarget.x = min_move_x;
	}
	if (max_move_y <= pCamera->m_ipos.y)
	{
		pCamera->m_ipos.y = max_move_y;
		pCamera->m_itarget.y = max_move_y;
	}
	if (min_move_y >= pCamera->m_ipos.y)
	{
		pCamera->m_ipos.y = min_move_y;
		pCamera->m_itarget.y = min_move_y;
	}


	//Y　補正仮
	//ipos.y += 5;
	//itarget.y += 5;

	//Y値 補正
	pCamera->m_ipos.y += (pCamera->m_CameraData.FullY * percent) + 5;//パーセントによってカメラを近づけさす +αは補正
	pCamera->m_itarget.y += (pCamera->m_CameraData.FullY * percent);

	// 補間
	pCamera->m_pos = pCamera->m_pos * 0.9f + pCamera->m_ipos * 0.1f;
	pCamera->m_target = pCamera->m_target * .95f + pCamera->m_itarget * .05f;
}

// 出口
void SumaburaCameraState::Exit(Camera *pCamera)
{

}

bool SumaburaCameraState::OnMessage(Camera *pCamera, const Message &msg)
{

	// メッセージタイプ
	switch ((MESSAGE_TYPE)msg.Msg)
	{
	case MESSAGE_TYPE::EFFECT_CAMERA:
	{
										EFFECT_CAMERA_INFO *eci = (EFFECT_CAMERA_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
										pCamera->m_OrgPos = (eci->pOrgPos) ? *eci->pOrgPos : VECTOR_ZERO;	// 原点座標入ってたら代入
										pCamera->OnEffectCamera(eci->scriptID);								// エフェクトカメラ開始
										return true;
										break;
	}
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}
//
//*****************************************************************************************************************************


//*****************************************************************************************************************************
//
//		エフェクトカメラ
void EffectCameraState::Enter(Camera *pCamera)
{

}

void EffectCameraState::Execute(Camera *pCamera)
{	
	// エフェクトカメラ更新
	pCamera->m_pEffectCamera->Update();
}

// 出口
void EffectCameraState::Exit(Camera *pCamera)
{

}

bool EffectCameraState::OnMessage(Camera *pCamera, const Message &msg)
{

	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//default:
	//	return true;
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//
//		自由設定用(ソフトイマージュ)カメラ
//void Camera::Mode::Free::Initialize(const Vector3 &pos, const Vector3 &target)
//{
//	dist = 47.0f;
//	me->slerp_percentage = 1.0f;
//	add_pos = VECTOR_ZERO;
//}
//
//void Camera::Mode::Free::Update()
//{
//	if (Get_wheel_flag() == WHEEL_FLAG::UP)
//		dist -= 4.0f;
//	else if (Get_wheel_flag() == WHEEL_FLAG::DOWN)
//		dist += 4.0f;
//
//	if (KeyBoard(MOUSE_RIGHT))
//	{
//		me->angle.y -= Get_mouse_move_x()*.01f;
//		me->angle.x -= Get_mouse_move_y()*.01f;
//	}
//	else if (KeyBoard(MOUSE_CENTAR))
//	{
//		// 移動量作成
//		Vector3 move =
//			Vector3(matView._11, matView._21, matView._31) * -Get_mouse_move_x()*.1f +
//			Vector3(matView._12, matView._22, matView._32) * Get_mouse_move_y()*.1f;
//		add_pos += move;
//	}
//
//	// 角度の値によるベクトルを作成
//	float ay_x = sinf(me->angle.y);
//	float ay_z = cosf(me->angle.y);
//	float ax_y = sinf(me->angle.x);
//	float ax_z = cosf(me->angle.x);
//
//	Vector3 vec(
//		ay_x * ax_z,
//		ax_y,
//		ay_z * ax_z);
//
//	// ベクトルの長さ決定
//	vec *= this->dist;
//
//
//	// プレイヤー座標取得
//	Vector3 p_pos = me->my_player->GetCollisionPos();
//
//
//
//	// 角度の値のベクトルとプレイヤーからカメラ位置算出
//	me->pos.x = p_pos.x - vec.x;
//	me->pos.y = p_pos.y - vec.y;
//	me->pos.z = p_pos.z - vec.z;
//
//	me->target = p_pos;
//
//	me->view->Set(me->pos+add_pos, me->target+add_pos);
//}
//
//*****************************************************************************************************************************