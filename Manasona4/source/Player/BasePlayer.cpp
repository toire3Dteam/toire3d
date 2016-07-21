#include "TDNLIB.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"


//_/_/_/_/_/_/__/_/__/_/__/_/__
// 定数
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// 移動が終わって待機に戻る移動値ライン
const float BasePlayer::c_FRONT_BRAKE_LINE = .55f;	// 走り中に操作を離してブレーキに移行するまでの移動値ライン
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 2.2f;

static const float DIR_ANGLE[(int)DIR::MAX] =
{
	PI * 1.5f,	// 左向き
	PI * .5,	// 右向き
};

// 攻撃フレーム情報読み込み
void BasePlayer::LoadAttackFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "アタックフレームのテキスト入ってない");

	FOR((int)BASE_ATTACK_STATE::END)
	{
		char skip[64];	// 読み飛ばし用変数

		ifs >> skip;

		// 始動、持続、硬直フレームを読み込み
		int count(0);

		for (int j = 0; j < (int)ATTACK_FRAME::END; j++)
		{
			int frame;
			ifs >> frame;
			for (int k = 0; k < frame; k++)
			{
				m_AttackFrameList[i][count++] = (ATTACK_FRAME)j;
			}
		}
		// 終端
		m_AttackFrameList[i][count] = ATTACK_FRAME::END;
	}
}

BasePlayer::BasePlayer(int deviceID) :BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO), m_bLand(false), m_AttackState(BASE_ATTACK_STATE::END), m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentAttackFrame(0)
{
	// デフォルト設定
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// 初期化
	memset(m_InputList, 0, sizeof(m_InputList));
	ZeroMemory(&m_jump, sizeof(Jump));
	ZeroMemory(&m_RushAttack, sizeof(RushAttack));
	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<BasePlayer>(this);
	m_pStateMachine->SetGlobalState(BasePlayerState::Global::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance());

	// IDで座標の分岐
	switch (deviceID)
	{
	case 0:
		m_pos.Set(-50, 20, 0);
		break;

	case 1:
		m_pos.Set(-25, 20, 0);
		break;

	case 2:
		m_pos.Set(25, 20, 0);
		break;

	case 3:
		m_pos.Set(50, 20, 0);
		break;
	}

	m_angleY = DIR_ANGLE[(int)m_dir];
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pStateMachine);
	delete m_pHitSquare;
}

void BasePlayer::Update()
{
	// 攻撃フレームの更新
	if (isAttackState())
	{
		// フレーム最後まで再生したら
		if (m_AttackFrameList[m_AttackState][m_CurrentAttackFrame++] == ATTACK_FRAME::END)
		{
			// ★攻撃ステート解除
			m_AttackState = BASE_ATTACK_STATE::END;
		}
	}

	// 入力受付
	Control();

	// 入力受付後にステートマシン更新
	m_pStateMachine->Update();

	//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
	//else m_move.x = 0;

	// 動きの制御
	Move();

	// アングル補間処理
	m_angleY = m_angleY*.65f + DIR_ANGLE[(int)m_dir] * .35f;

	// メッシュの更新
	m_pObj->Animation();
	m_pObj->SetAngle(m_angleY);	// 左右のアングルのセット
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
	
}

void BasePlayer::Move() 
{
	// 移動量更新
	m_move.y -= c_GRAVITY;
	//if (m_move.y <= -3.0f) { m_move.y = -3.0f; } // 落ちる速度を抑制
	m_move.y = max(-2.75f, m_move.y);// 落ちる速度を抑制

	// 空気抵抗(xの値を徐々に減らしていく)
	if (m_bLand)
	{
		//if (m_move.x > 0)		m_move.x = max(m_move.x - .055f, 0);
		//else if (m_move.x < 0)	m_move.x = min(m_move.x + .055f, 0);
		m_move.x *= 0.92f;	// 減速(A列車:この値はキャラ固有の値)
	}
	else
	{	
		//if (m_move.x > 0)		m_move.x = max(m_move.x - .035f, 0);
		//else if (m_move.x < 0)	m_move.x = min(m_move.x + .035f, 0);
		m_move.x *= 0.98f;	// 減速(A列車:この値はキャラ固有の値)
	}

	// 左右のMove値
	//m_move.x = Math::Clamp(m_move.x, -m_maxSpeed, m_maxSpeed);


}

void BasePlayer::Control()
{
	// 押したキー判定
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_InputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_deviceID);
	}

}

void BasePlayer::UpdatePos()
{
	// 座標更新
	m_pos += m_move;
}

void BasePlayer::Render()
{
	m_pObj->Render();
	
	m_pStateMachine->Render();// ステートマシンでの描画

	tdnText::Draw(0, 0, 0xffffffff,"%.1f", m_move.y);
}

// ステートマシンへの他から来るメッセージ
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}