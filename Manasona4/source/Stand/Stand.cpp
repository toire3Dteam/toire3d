#include "TDNLIB.h"
#include "Stand.h"
#include "../Sound/SoundManager.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "Effect\Particle.h"
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
m_dir(DIR::LEFT),
m_ePartnerType(PARTNER::MOKOI),
m_fScale(0.0f),
m_pSummonEffect(nullptr),
m_pSummonWaveEffect(nullptr),
m_pExitEffect(nullptr),
m_pos(Vector3(0,0,0))
{
	m_pSummonEffect = new SummonEffect();
	m_pSummonWaveEffect = new SummonWaveEffect();
	m_pExitEffect = new ExitEffect();

	FOR((int)SKILL_ACTION_TYPE::MAX) m_pAttackData[i] = nullptr;
}

Stand::Base::~Base()
{
	FOR((int)SKILL_ACTION_TYPE::MAX)SAFE_DELETE(m_pAttackData[i]);
	SAFE_DELETE(m_pObj);

	SAFE_DELETE(m_pIcon);
	SAFE_DELETE(m_pIconRip);

	SAFE_DELETE(m_pSummonEffect);
	SAFE_DELETE(m_pSummonWaveEffect);
	SAFE_DELETE(m_pExitEffect);


}

void Stand::Base::Update(bool bControl)
{
	// スタンドゲージ更新
	if(bControl)StandGageUpdate();

	// スケール調整
	if (m_bActive == true) 
	{
		m_fScale += 0.25f;
	}
	else
	{
		m_fScale -= 0.25f;
	}

	// クランプ
	m_fScale = Math::Clamp(m_fScale, 0.0f, 1.0f);


	// アクティブじゃなかったら出ていけぇ！！
	if (m_bActive == true)
	{
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


			// 判定復活フレーム
			if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::RECOVERY_HIT)
			{
				// 判定リセット
				GetAttackData()->bHit = GetAttackData()->bHitSuccess = false;
			}
		}



		// フレーム最後まで再生したら
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame++] == FRAME_STATE::END)
		{
			// 
			Break();
			// ★アクティブをオフ(本来だったら半透明フェードアウトを使う)
			//m_bActive = false;

			// 帰宅エフェクト
			//m_pExitEffect->Action(m_pos + Vector3(0, 0, -10), 1.25f, 0.25f);
		}

	}// 出ていけ

	// 3Dオブジェクトの更新
	m_pObj->Animation();
	m_pObj->SetAngle(DIR_ANGLE[(int)m_dir]);
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// エフェクト更新
	m_pSummonEffect->Update();
	m_pSummonWaveEffect->Update();
	m_pExitEffect->Update();
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
	else m_standGage = m_standGageMAX;
}

void Stand::Base::Render(tdnShader* shader, char* name)
{
	// エフェクト描画
	m_pSummonEffect->Render();
	m_pSummonWaveEffect->Render();
	m_pExitEffect->Render();

	// アクティブじゃなかったら出ていけぇ！！
	if (m_bActive == false && m_fScale <= 0.0f) return;

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

	m_fScale = 0.0f;
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	//m_pSummonEffect->Action(m_pos + Vector3(0, 0, -10), 1, 1);
	m_pSummonWaveEffect->Action(GetCenterPos(), 0.5f, 2.0f);

}

// ペルソナブレイク
void Stand::Base::Break()
{
	if (m_bActive)
	{
		// ここにブレイクの音とかエフェクトとか
		// 帰宅エフェクト
		m_pExitEffect->Action(m_pos + Vector3(0, 0, -10), 1.25f, 0.25f);
		m_bActive = false;

		if (m_pAttackData[(int)m_ActionType]) 
		{
			m_pAttackData[(int)m_ActionType]->bHit = false;// (追加)ヒットフラグ解除
		}

	}
}

/******************************/
//		モコイ
/******************************/
Stand::Mokoi::Mokoi(BasePlayer *pPlayer) :Base(pPlayer)
{
	// モコイの実体
	m_pObj = new iex3DObj("DATA/CHR/Stand/Mokoi/mokoi.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// タイプ設定
	m_ePartnerType = PARTNER::MOKOI;

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
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 660;
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 17;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(5.5f, 17, 0);

	//==============================================================================================================
	//	しゃがみ
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 1160;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::RUN;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.25f, 6);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 1.0f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.1f, 2.0f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(1.1f, 1.85f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 10;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 10;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 14;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 10;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(5.5f, 10, 0);

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

void Stand::Mokoi::Update(bool bControl)
{
	// 基底クラスの更新
	Base::Update(bControl);

	// アクティブじゃなかったら出ていけぇ！！
	if (!m_bActive) return;

	// 判定が出た瞬間動く
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::START)
	{
		float l_fPow = 0.0f;
		// しゃがみで発動すると前方向に進む力を上げる
		if (m_ActionType == SKILL_ACTION_TYPE::SQUAT)l_fPow = 0.75f;

		// キャラクターの移動
		if (m_dir == DIR::LEFT)
		{
			m_pos.x -= (0.5f+ l_fPow);
		}
		else
		{
			m_pos.x += (0.5f+ l_fPow);
		}
	}

	if (m_ActionType == SKILL_ACTION_TYPE::SQUAT &&
		m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
	{
		// キャラクターの移動
		if (m_dir == DIR::LEFT)
		{
			m_pos.x -= (0.25f);
		}
		else
		{
			m_pos.x += (0.25f);
		}
	}


}

void Stand::Mokoi::Action(SKILL_ACTION_TYPE type)
{

	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.95f);
	if (m_pPlayer->GetDir() == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// 空中ジャンプ復活
	m_pPlayer->SetAerialJump(true);

	// モーション
	m_pObj->SetMotion(0);

	// 基底クラスのアクション
	// 空中はすべて通常技に
	if (type == SKILL_ACTION_TYPE::AERIAL ||
		type == SKILL_ACTION_TYPE::AERIALDROP)
	{
		Base::Action(SKILL_ACTION_TYPE::LAND);
	}
	else
	{
		if (type == SKILL_ACTION_TYPE::SQUAT)
		{
			m_pObj->SetMotion(1);
			if (m_pPlayer->GetDir() == DIR::LEFT)m_pos.x += 15;
			else m_pos.x -= 15;
		}

		Base::Action(type);
	}
}

/******************************/
//		マーヤ
/******************************/
Stand::Maya::Maya(BasePlayer *pPlayer) :Base(pPlayer)
{
	// マーヤの実体
	m_pObj = new iex3DObj("DATA/CHR/Stand/Maya/maya.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// タイプ設定
	m_ePartnerType = PARTNER::MAYA;

	// 玉の実体
	m_pBullet = new  MayaShotEffect();/*iexMesh("DATA/UVeffect/Airou/AirouBoost.IMO");*/
	m_pBullet2 = new  MayaShot2Effect();

	// アクションフレームロードする
	LoadActionFrameList("DATA/CHR/Stand/Maya/FrameList.txt");

	/*****************************/
	// マーヤの基本ステータス
	m_standStockMAX = 1;
	m_standGageMAX = 60*5;

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
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 1000;
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 48;
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 48;
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
	SAFE_DELETE(m_pBullet);
	SAFE_DELETE(m_pBullet2);
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
		ShotVec.x *= m_pPlayer->GetDirVecX();

		// マズルエフェクト発動！
		m_pPlayer->AddEffectAction(m_pos + Vector3(0, 4, -2), EFFECT_TYPE::MUZZLE_FLASH, ShotVec);

		Shot::Base *pNewShot = nullptr;

		// 玉の情報を格納
		if (m_ActionType== SKILL_ACTION_TYPE::SQUAT)
		{
			pNewShot = new Shot::Maya(m_pPlayer, m_pAttackData[(int)m_ActionType], m_pBullet2, m_pos + Vector3(-ShotVec.x * 2, 5, 0), ShotVec);
		}
		else
		{
			pNewShot = new Shot::Maya(m_pPlayer, m_pAttackData[(int)m_ActionType], m_pBullet, m_pos + Vector3(-ShotVec.x * 2, 5, 0), ShotVec);
		}

		MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);
	}
}

void Stand::Maya::Action(SKILL_ACTION_TYPE type)
{
	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 2.0f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// 基底クラスのアクション(全員共通系)
	Base::Action(type);


}




/******************************/
//		ヘテ
/******************************/
Stand::Hete::Hete(BasePlayer *pPlayer) :Base(pPlayer)
{
	// モコイの実体
	m_pObj = new iex3DObj("DATA/CHR/Stand/Hete/hete.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// タイプ設定
	m_ePartnerType = PARTNER::HETE;

	// アクションフレームロードする
	LoadActionFrameList("DATA/CHR/Stand/Hete/FrameList.txt");

	// 多段用
	for (int i = 0; i < 8; i++)
	{
		m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
		m_ActionFrameList[(int)SKILL_ACTION_TYPE::SQUAT][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
	}

	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 13] = FRAME_STATE::RECOVERY_HIT;
	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 19] = FRAME_STATE::RECOVERY_HIT;

	/*****************************/
	// 基本ステータス
	m_standStockMAX = 1;
	m_standGageMAX = 60 * 8;

	/*****************************/
	// そのキャラクターのアイコン
	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/hete.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/hete.png");

	//==============================================================================================================
	//	地上ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 160;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = 0.5f;	//	大めで
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 0.99f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0.9f, 0.0f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0.9f, 0.0f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 6;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 6;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 6.5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(0.5f, 6, 0);

	//==============================================================================================================
	//	地上しゃがみ
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 160;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "ヒット6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "空振り1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = 0.5f;	//	大めで
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 0.99f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0.9f, 0.0f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0.9f, 0.0f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 6;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 6;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 60;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_BACK;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 6.5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(0.5f, 6, 0);

	// 判定発動した瞬間に空振りSEを再生してみる
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// newされてなかったらスルー
											//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "Xの値が-になることは絶対ない");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "Xの値が-になることは絶対ない");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE) { m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// アッパーの空振りフレーム調整
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;
}


void Stand::Hete::Update(bool bControl)
{
	// 基底クラスの更新
	Base::Update(bControl);

	// アクティブじゃなかったら出ていけぇ！！
	if (!m_bActive) return;



	// 前方向に進む
	if (m_ActionType == SKILL_ACTION_TYPE::LAND)
	{
		// 判定が出た瞬間動く
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
		{

			// キャラクターの移動
			if (m_dir == DIR::LEFT)
			{
				m_pos.x -= 0.43f;
			}
			else
			{
				m_pos.x += 0.43f;
			}

		}

		//// 走り終わってたら
		////if (m_ActionType[(int)SKILL_ACTION_TYPE::LAND]->Execute())
		////{
		////	 無理やり走るのを終わらせる
		////		m_ActionType(SKILL_ACTION_TYPE::NO_ACTION);
		////	return;
		////}

		// ヒット回数計測
		if (m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][m_CurrentActionFrame]
			== FRAME_STATE::RECOVERY_HIT)
		{
			//// 最後の一撃だけ強くする
			//if (++m_iHitCount >= 3)
			//{
			//	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.5f, 0);
			//	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.85f, 1.65f);
			//}

			// コンボレートを下げないようにする
			//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 1.0f;
		}
	}
}

void Stand::Hete::Action(SKILL_ACTION_TYPE type)
{

	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, .675f);
	if (m_pPlayer->GetDir() == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 11.0f;

	// (TODO)2/4 今はここでモーションを設定
	if (type == SKILL_ACTION_TYPE::LAND)
	{
		m_pObj->SetMotion(0);
	}
	else //if (type == SKILL_ACTION_TYPE::SQUAT)
	{
		if (m_pPlayer->GetDir() == DIR::LEFT)
		{
			m_pos.x += -2.5f;
		}
		else
		{
			m_pos.x += +2.5f;
		}

		m_pObj->SetMotion(1);
	}
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// 基底クラスのアクション
	// 空中はすべてしゃがみ技に
	if (type == SKILL_ACTION_TYPE::AERIAL ||
		type == SKILL_ACTION_TYPE::AERIALDROP)
	{
		Base::Action(SKILL_ACTION_TYPE::SQUAT);
	}
	else
	{
		Base::Action(type);
	}


}


/******************************/
//		アッタカイ
/******************************/
Stand::Attakai::Attakai(BasePlayer *pPlayer) :Base(pPlayer)
{
	// アッタカイの実体
	m_pObj = new iex3DObj("DATA/CHR/Stand/Attakai/Attakai.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// タイプ設定
	m_ePartnerType = PARTNER::ATTAKAI;

	// アクションフレームロードする
	LoadActionFrameList("DATA/CHR/Stand/Attakai/FrameList.txt");

	// 多段用
	//for (int i = 0; i < 8; i++)
	//{
	//	m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
	//	m_ActionFrameList[(int)SKILL_ACTION_TYPE::SQUAT][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
	//}

	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 13] = FRAME_STATE::RECOVERY_HIT;
	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 19] = FRAME_STATE::RECOVERY_HIT;

	m_iActiveStartFrame = 0;

	/*****************************/
	// 基本ステータス
	m_standStockMAX = 1;
	m_standGageMAX = 60 * 18;

	/*****************************/
	// そのキャラクターのアイコン
	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/attakai.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/attakai.png");

	//==============================================================================================================
	//	地上ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 3300;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "ヒット1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "フィニッシュ大";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::EXPLOSION;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.5f, 10);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = 0.5f;	//	大めで
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 0.99f;
	
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0.3f, 3.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0.3f, 3.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 26;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 26;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 120;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 120;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 25;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(0.0f, 18, 0);

	//==============================================================================================================
	//	地上しゃがみ
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 2300;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "ヒット1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "フィニッシュ大";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::EXPLOSION;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.5f, 10);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = 0.5f;	//	大めで
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 0.99f;
	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(0.3f, 3.5f);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(0.3f, 3.5);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].iHitStopFrame = 26;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].iHitStopFrame = 26;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].HitRecoveryFrame = 120;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 120;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
	// 判定形状
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 25;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(0.5f, 18, 0);

	// 判定発動した瞬間に空振りSEを再生してみる
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// newされてなかったらスルー
											//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "Xの値が-になることは絶対ない");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "Xの値が-になることは絶対ない");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE) { m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// アッパーの空振りフレーム調整
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;


	// エフェクト
	m_pBomEf = new BomEffect();
	m_pBomAddEf = new BomAddEffect();
	m_fDangerRate = 0.0f;
}


Stand::Attakai::~Attakai()
{
	SAFE_DELETE(m_pBomEf);
	SAFE_DELETE(m_pBomAddEf);
}

void Stand::Attakai::Update(bool bControl)
{
	// 基底クラスの更新
	Base::Update(bControl);

	// アングル再設定
	m_pObj->SetAngle(3.14f);
	m_pObj->Update();




	// エフェクト
	m_pBomEf->Update();
	m_fDangerRate = Math::Clamp((float)(m_CurrentActionFrame) / (float)(m_iActiveStartFrame), 0, 1.0f);
	m_pBomAddEf->GetPanel()->GetPic()->SetAlpha((int)(m_fDangerRate * 255.0f));
	m_pBomAddEf->Update();

	m_pBomEf->SetPos(m_pos + Vector3(0, 17, -6));
	m_pBomAddEf->SetPos(m_pos + Vector3(0, 17, -6));

	// 時間が立つにつれ早くなる
	if (m_fDangerRate >= 0.45f)
	{
		m_pBomEf->Update();
		m_pBomAddEf->Update();

		// だんだん大きくなる
		m_pBomEf->AddAdd3DScale(0.1f);
		m_pBomAddEf->AddAdd3DScale(0.1f);


	}
	if (m_fDangerRate >= 0.75f)
	{
		m_pBomEf->Update();
		m_pBomAddEf->Update();

		// だんだん大きくなる
		m_pBomEf->AddAdd3DScale(0.1f);
		m_pBomAddEf->AddAdd3DScale(0.1f);

	}
	if (m_fDangerRate >= 0.85f)
	{
		m_pBomEf->Update();
		m_pBomAddEf->Update();
		m_pBomEf->Update();
		m_pBomAddEf->Update();

		m_pBomEf->AddAdd3DScale(0.5f);
		m_pBomAddEf->AddAdd3DScale(0.5f);

	}

	// アクティブじゃなかったら出ていけぇ！！
	if (!m_bActive) return;


	// 前方向に進む
	if (m_ActionType == SKILL_ACTION_TYPE::LAND)
	{
		// 判定が出た瞬間動く
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] != FRAME_STATE::ACTIVE)
		{
			// 追尾
			const float fAbjust = 0.5f;
			if (m_pPlayer->GetTargetPlayer()->GetPos().x + fAbjust > m_pos.x)
			{
				m_move.x += 0.04f;
			}
			if (m_pPlayer->GetTargetPlayer()->GetPos().x - fAbjust < m_pos.x)
			{
				m_move.x -= 0.04f;
			}

			// 歩き煙エフェクト
			if (m_CurrentActionFrame % 8 == 0)
			{
				ParticleManager::EffectRunSmoke(m_pos, (m_dir == DIR::LEFT));
			}
		}
	}

	// 摩擦
	Math::Clamp(m_move.x, -2.7f, 2.7f);
	
	// 移動値加算
	m_pos.x += m_move.x;	
	if (abs(m_move.x) > 0.25f)
	{
		m_move.x *= 0.985f;
	}




}

void Stand::Attakai::Render(tdnShader * shader, char * name)
{
	Stand::Base::Render(shader, name);

	// アクティブじゃなかったら出ていけぇ！！
	if (m_bActive == false && m_fScale <= 0.0f) return;
	
	// アクティブ(爆発）したら描画しない
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] != FRAME_STATE::ACTIVE)
	{
		// 爆弾
		m_pBomEf->Render3D();
		m_pBomAddEf->Render3D();
	}

	tdnText::Draw(100, 100, 0xffff00ff, "%f", m_fDangerRate);

}

void Stand::Attakai::Action(SKILL_ACTION_TYPE type)
{

	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.675f);
	if (m_pPlayer->GetDir() == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 11.0f;

	// (TODO)2/4 今はここでモーションを設定
	if (type == SKILL_ACTION_TYPE::LAND)
	{
		m_pObj->SetMotion(0);
	}
	else //if (type == SKILL_ACTION_TYPE::SQUAT)
	{
		if (m_pPlayer->GetDir() == DIR::LEFT)
		{
			m_pos.x += -2.5f;
		}
		else
		{
			m_pos.x += +2.5f;
		}

		m_pObj->SetMotion(1);
	}


	if (type == SKILL_ACTION_TYPE::AERIALDROP)
	{
		// そして地面に召喚
		m_pos.y = 0.0f;

		// 相手のいる座標に
		m_pos.x = m_pPlayer->GetTargetPlayer()->GetPos().x - m_pPlayer->GetTargetPlayer()->GetDirVecX() * 5;

	}
	if (type == SKILL_ACTION_TYPE::AERIAL)
	{
		// 地面に召喚
		m_pos.y = 0.0f;

	}
	m_pObj->SetPos(m_pos);
	m_pObj->Update();


	// 基底クラスのアクション
	// 空中はすべてしゃがみ技に
	if (type == SKILL_ACTION_TYPE::AERIALDROP)
	{
		
		Base::Action(SKILL_ACTION_TYPE::SQUAT);

	}
	else if (type == SKILL_ACTION_TYPE::AERIAL)
	{
		Base::Action(SKILL_ACTION_TYPE::LAND);
	}
	else
	{
		Base::Action(type);
	}

	
	// アクティブ時になるフレーム
	for (int i = 0;; i++)
	{
		if (m_ActionFrameList[(int)m_ActionType][i] == FRAME_STATE::ACTIVE)
		{
			m_iActiveStartFrame = i;
			break;
		}
	}

	// 移動量初期化
	m_move = VECTOR_ZERO;
 

	// エフェクト
	m_pBomEf   ->Action(m_pos + Vector3(0, 17, -6));
	m_pBomAddEf->Action(m_pos + Vector3(0, 17, -6));

	m_pBomEf->SetAdd3DScale(0);
	m_pBomAddEf->SetAdd3DScale(0);

}
