#include "TDNLIB.h"
#include "Stand.h"
#include "../Sound/SoundManager.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseEntity\Message\MessageDispatcher.h"

#include "../Shot/BaseShot.h"

// 攻撃フレーム情報読み込み
void Stand::Base::LoadActionFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "アクションフレームのテキスト入ってない");

	char skip[64];	// 読み飛ばし用変数

	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		ifs >> skip;

		// 始動、持続、硬直フレームを読み込み
		int count(0);

		for (int j = 0; j < (int)FRAME_STATE::END; j++)
		{
			int frame;
			ifs >> frame;
			for (int k = 0; k < frame; k++)
			{
				m_ActionFrameList[i][count++] = (FRAME_STATE)j;
			}
		}
		// 終端
		m_ActionFrameList[i][count] = FRAME_STATE::END;
	}
}


Stand::Base::Base(BasePlayer *pPlayer) :
m_pPlayer(pPlayer), 
m_pObj(nullptr),
m_bActive(false),
m_standStockMAX(1),
m_standStock(0),
m_standGageMAX(360),
m_standGage(0),
m_pIcon(nullptr),
m_pIconRip(nullptr),
m_ActionType(SKILL_ACTION_TYPE::NO_ACTION),
m_CurrentActionFrame(0),
m_dir(DIR::LEFT)
{
	FOR((int)SKILL_ACTION_TYPE::MAX) m_pAttackData[i] = nullptr;
}

Stand::Base::~Base()
{
	FOR((int)SKILL_ACTION_TYPE::MAX)SAFE_DELETE(m_pAttackData[i]);
	SAFE_DELETE(m_pObj);

	SAFE_DELETE(m_pIcon);
	SAFE_DELETE(m_pIconRip);
}

void Stand::Base::Update(bool bControl)
{
	// スタンドゲージ更新
	if(bControl)StandGageUpdate();

	// アクティブじゃなかったら出ていけぇ！！
	if (!m_bActive) return;

	// フレームリスト更新
	// アクションフレームの更新
	// 攻撃データがあるなら
	if (m_pAttackData[(int)m_ActionType] != nullptr)
	{
		// 今の経過時間とディレイフレームになったら
		if (m_pAttackData[(int)m_ActionType]->WhiffDelayFrame == m_CurrentActionFrame)
		{
			// (A列車)ここで攻撃判定が発動した瞬間を取ってきてる

			// 振りエフェクト発動（仮）！
			m_pPlayer->AddEffectAction(m_pos + Vector3(0, 5, -3), m_pAttackData[(int)m_ActionType]->WhiffEffectType);

			LPCSTR SE_ID(m_pAttackData[(int)m_ActionType]->WhiffSE);
			// 空振りSE入ってたら
			if (SE_ID)
			{
				// ディレイフレーム経過したら再生
				se->Play((LPSTR)SE_ID);
			}
		}
	}

	// フレーム最後まで再生したら
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame++] == FRAME_STATE::END)
	{
		// ★アクティブをオフ(本来だったら半透明フェードアウトを使う)
		m_bActive = false;
	}

	// 3Dオブジェクトの更新
	m_pObj->Animation();
	m_pObj->SetAngle(DIR_ANGLE[(int)m_dir]);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
}
 
// スタンドゲージ更新
void Stand::Base::StandGageUpdate()
{
	// スタンドストックが最大値まで溜まっていなかったら
	if (m_standStock < m_standStockMAX)
	{	
		m_standGage++;// ゲージ加算

		// スタンドゲージが最大値まで溜まったら
		if (m_standGage >= m_standGageMAX)
		{
			m_standStock++;
			m_standGage = 0;

			// UIにメッセージを送る
			SIDE side = m_pPlayer->GetSide();
			MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::PERSONA_CARD_COUNT_UP, &side);
		}

	}

}

void Stand::Base::Render(tdnShader* shader, char* name)
{

	// アクティブじゃなかったら出ていけぇ！！
	if (!m_bActive) return;

	// 3Dオブジェクトの描画
	m_pObj->Render(shader,name);

#ifdef _DEBUG
	// 判定の描画
	CollisionShape::Square square;

	/* 攻撃判定の描画 */
	if (m_pAttackData[(int)m_ActionType])
	{
		// 攻撃判定アクティブ中なら
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
		{
			Vector3 wv[3];	// ワールドバーテックス
			Vector2 sv[3];	// スクリーンバーテックス

			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			square.pos += m_pos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}
#endif
}

void Stand::Base::Action(SKILL_ACTION_TYPE type)
{
	// UIにメッセージを送る
	SIDE side = m_pPlayer->GetSide();
	MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::UI_MGR,
		MESSAGE_TYPE::PERSONA_CARD_COUNT_DOWN, &side);

	// ストックを消費
	m_standStock--;
	m_standGage = 0;// (12/24)ゲージ0

	m_ActionType = type;		// 技のタイプ設定
	m_CurrentActionFrame = 0;	// フレームリセット

	if(m_pAttackData[(int)m_ActionType])m_pAttackData[(int)m_ActionType]->bHit = false;	// ヒットフラグ解除

	// とりあえず、アクティブにする
	m_bActive = true;

	// プレイヤーの向きをもらう
	m_dir = m_pPlayer->GetDir();
}

/******************************/
//		モコイ
/******************************/
Stand::Mokoi::Mokoi(BasePlayer *pPlayer) :Base(pPlayer)
{
	// モコイの実体
	m_pObj = new iex3DObj("DATA/CHR/Stand/Mokoi/mokoi.IEM");
	
	// アクションフレームロードする
	LoadActionFrameList("DATA/CHR/Stand/Mokoi/FrameList.txt");

	/*****************************/
	// モコイの基本ステータス
	m_standStockMAX = 1;
	m_standGageMAX = 60*8;

	/*****************************/
	// そのキャラクターのアイコン
	m_pIcon= new tdn2DAnim("Data/UI/Game/PersonaIcon/Mokoi.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/Mokoi.png");

	//==============================================================================================================
	//	地上ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 360;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::UPPER;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.2f, 2.3f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.2f, 2.1f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 14;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 18;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(7.5f, 17, 0);

	// 判定発動した瞬間に空振りSEを再生してみる
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// newされてなかったらスルー
		//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "Xの値が-になることは絶対ない");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "Xの値が-になることは絶対ない");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// アッパーの空振りフレーム調整
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;
}

void Stand::Mokoi::Action(SKILL_ACTION_TYPE type)
{
	// 基底クラスのアクション	★今は全部地上の攻撃扱いにする
	Base::Action(SKILL_ACTION_TYPE::LAND);

	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.5f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
}


/******************************/
//		マーヤ
/******************************/
Stand::Maya::Maya(BasePlayer *pPlayer) :Base(pPlayer)
{
	// マーヤの実体
	m_pObj = new iex3DObj("DATA/CHR/Stand/Maya/maya.IEM");

	// 玉の実体
	m_pBullet = new  MayaShotEffect();/*iexMesh("DATA/UVeffect/Airou/AirouBoost.IMO");*/

	// アクションフレームロードする
	LoadActionFrameList("DATA/CHR/Stand/Maya/FrameList.txt");

	/*****************************/
	// マーヤの基本ステータス
	m_standStockMAX = 1;
	m_standGageMAX = 60*4;

	/*****************************/
	// そのキャラクターのアイコン
	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/maya.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/maya.png");

	//==============================================================================================================
	//	地上ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 0);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, 1.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 10;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	しゃがみ
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.1f, 3);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 0);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.5f, 1.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 12;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 40;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 50;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	空中ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->ShakeCameraInfo.Set(.1f, 3);
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 0);
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.75f, 1.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 10;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	空中下
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->ShakeCameraInfo.Set(.1f, 3);
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->attribute = ATTACK_ATTRIBUTE::BULLET;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 0);
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.75f, 1.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 35;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pCollisionShape->pos.Set(0, 0, 0);

	// 判定発動した瞬間に空振りSEを再生してみる
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// newされてなかったらスルー
		for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "Xの値が-になることは絶対ない");
		//MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "Xの値が-になることは絶対ない");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}
}

Stand::Maya::~Maya()
{
	delete m_pBullet;
}

void Stand::Maya::Update(bool bControl)
{
	// 基底クラスの更新
	Base::Update(bControl);

	// アクティブじゃなかったら出ていけぇ！！
	if (!m_bActive) return;

	// 判定が出た瞬間に飛び道具をうつ
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
	{
		Vector3 ShotVec;
	
		// 仮で出した瞬間に出る
		switch (m_ActionType)
		{
		case SKILL_ACTION_TYPE::LAND:
		case SKILL_ACTION_TYPE::AERIAL:
			ShotVec.Set(1, 0, 0);
			break;
		case SKILL_ACTION_TYPE::SQUAT:
			ShotVec.Set(.85f, .5f, 0);
			break;
		case SKILL_ACTION_TYPE::AERIALDROP:
			ShotVec.Set(.85f, -.5f, 0);
			break;
		}

		// ショットの向き逆
		if (m_pPlayer->GetDir() == DIR::LEFT) ShotVec.x *= -1;

		// マズルエフェクト発動！
		m_pPlayer->AddEffectAction(m_pos + Vector3(0, 4, -2), EFFECT_TYPE::MUZZLE_FLASH, ShotVec);


		// 玉の情報を格納
		Shot::Base *pNewShot(new Shot::Maya(m_pPlayer, m_pAttackData[(int)m_ActionType], m_pBullet, m_pos + Vector3(0, 5, 0), ShotVec));
		MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);
	}
}

void Stand::Maya::Action(SKILL_ACTION_TYPE type)
{
	// 基底クラスのアクション(全員共通系)
	Base::Action(type);

	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 1.0f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
}