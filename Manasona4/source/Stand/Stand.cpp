#include "TDNLIB.h"
#include "Stand.h"
#include "../Sound/SoundManager.h"

// 攻撃フレーム情報読み込み
void Stand::Base::LoadActionFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "アクションフレームのテキスト入ってない");

	char skip[64];	// 読み飛ばし用変数

	FOR((int)ACTION_TYPE::MAX)
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
m_standStockMAX(2),
m_standStock(0),
m_standGageMAX(360),
m_standGage(0)
{
	FOR((int)ACTION_TYPE::MAX) m_pAttackData[i] = nullptr;
}

Stand::Base::~Base()
{
	FOR((int)ACTION_TYPE::MAX)SAFE_DELETE(m_pAttackData[i]);
	SAFE_DELETE(m_pObj);
}

void Stand::Base::Update()
{
	// スタンドゲージ更新
	StandGageUpdate();

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

			LPCSTR SE_ID = m_pAttackData[(int)m_ActionType]->WhiffSE;
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

/******************************/
//		モコイ
/******************************/
Stand::Mokoi::Mokoi(BasePlayer *pPlayer) :Base(pPlayer)
{
	// モコイの実体
	m_pObj = new iex3DObj("DATA/CHR/mokoi/mokoi.IEM");
	//m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou4.png"), 0);	// ダークアイルー
	
	// アクションフレームロードする
	LoadActionFrameList("DATA/CHR/Mokoi/FrameList.txt");

	/*****************************/
	// モコイの基本ステータス
	m_standStockMAX = 2;
	m_standGageMAX = 120;


	//==============================================================================================================
	//	地上ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::LAND]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::LAND]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::LAND]->HitSE = "通常3段ヒット";
	m_pAttackData[(int)ACTION_TYPE::LAND]->WhiffSE = "空振り1";
	m_pAttackData[(int)ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::UPPER;
	m_pAttackData[(int)ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.75f, 4);

	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 2.3f);
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 60;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// 判定形状
	m_pAttackData[(int)ACTION_TYPE::LAND]->pCollisionShape->width = 14;
	m_pAttackData[(int)ACTION_TYPE::LAND]->pCollisionShape->height = 18;
	m_pAttackData[(int)ACTION_TYPE::LAND]->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	しゃがみ
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->HitSE = "通常3段ヒット";
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->WhiffSE = "空振り1";
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.75f, 4);

	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// 判定形状
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pCollisionShape->width = 8;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pCollisionShape->height = 12;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	空中ニュートラル
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)ACTION_TYPE::AERIAL] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->HitSE = "通常3段ヒット";
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->WhiffSE = "空振り1";
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->ShakeCameraInfo.Set(.75f, 4);

	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// 判定形状
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pCollisionShape->width = 8;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pCollisionShape->height = 12;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	空中下
	// 地上ヒットも空中ヒットも共通の情報
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->HitSE = "通常3段ヒット";
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->WhiffSE = "空振り1";
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->ShakeCameraInfo.Set(.75f, 4);

	// 地上ヒットと空中ヒットで挙動が変わるもの
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// 判定形状
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pCollisionShape->width = 8;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pCollisionShape->height = 12;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pCollisionShape->pos.Set(2.5f, 17, 0);

	// 判定発動した瞬間に空振りSEを再生してみる
	FOR((int)ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// newされてなかったらスルー
		for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "Xの値が-になることは絶対ない");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "Xの値が-になることは絶対ない");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// アッパーの空振りフレーム調整
	m_pAttackData[(int)ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;
}

void Stand::Mokoi::Action(Stand::ACTION_TYPE type)
{
	// ストックを消費
	m_standStock--;

	m_ActionType = type;		// 技のタイプ設定
	m_CurrentActionFrame = 0;	// フレームリセット

	m_pAttackData[(int)m_ActionType]->bHit = false;	// ヒットフラグ解除

	// とりあえず、アクティブにする
	m_bActive = true;

	// プレイヤーの向きをもらう
	m_dir = m_pPlayer->GetDir();

	// とりあえず、プレイヤーと同じ座標
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.5f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// アッパーエフェクト！
	//if(type == ACTION_TYPE::LAND) m_pPlayer->AddEffectAction(m_pPlayer->GetPos(), EFFECT_TYPE::UPPER);
}
