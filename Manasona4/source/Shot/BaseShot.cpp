#include "TDNLIB.h"
#include "BaseShot.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"
#include "PointLight\PointLight.h"
#include "DeferredEx\DeferredEx.h"
#include "Effect\Particle.h"

Shot::Base::Base(BasePlayer *pPlayer, AttackData *pAttackData,
	iex3DObj *pObj,
	bool bAttitude):
	m_pPlayer(pPlayer),
	m_ptagAttackData(pAttackData),
	m_p3DObj(pObj),
	m_bAttitude(bAttitude),
	m_pUVEffect(nullptr),
	m_tagParamDesc()
{
	pObj->SetFrame(0);
	pObj->SetAngle(DIR_ANGLE[(int)pPlayer->GetTargetDir()]);
}

Shot::Base::Base(BasePlayer *pPlayer, AttackData *pAttackData,
	BaseUVEffect *pObj) :
	m_pPlayer(pPlayer),
	m_ptagAttackData(pAttackData),
	m_p3DObj(nullptr),
	m_bAttitude(false),
	m_pUVEffect(pObj),
	m_tagParamDesc()
{
	m_pUVEffect->ActionRoop(m_tagParamDesc.vPos);
}

void Shot::Base::Update()
{
	// 滞在時間ありの飛び道具の場合
	if (m_tagParamDesc.iSojournTime != c_NO_SOJOURN_TIME)
	{
		// 滞在時間が終わったら消える
		if (--m_tagParamDesc.iSojournTime <= 0) m_tagParamDesc.bErase = true;
	}

	// 基本的な移動量の更新
	m_tagParamDesc.vVelocity += m_tagParamDesc.vAccel;
	m_tagParamDesc.vPos += m_tagParamDesc.vVelocity;

	// オブジェの更新
	if (m_p3DObj)
	{
		m_p3DObj->Animation();

		// 姿勢制御
		if (m_bAttitude)
		{
			// 自分で行列を作る(位置・移動ベクトル情報から)
			Math::SetTransMatrixFrontVec(&m_p3DObj->TransMatrix, m_tagParamDesc.vPos, m_tagParamDesc.vVec);
		}

		else
		{
			m_p3DObj->SetPos(m_tagParamDesc.vPos);
			m_p3DObj->Update();
		}
	}

	else if (m_pUVEffect)
	{
		m_pUVEffect->SetPos(m_tagParamDesc.vPos);
		m_pUVEffect->SetAngleAnimation(
			Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)),
			Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y))
			);


		// UVエフェクト
		m_pUVEffect->Update();
	}

}

void Shot::Base::Render()
{
	if (m_p3DObj)
	{
		if (m_bAttitude)
		{
			// TransMatrixを直でいじったので、更新が不要の描画
			m_p3DObj->Render(false);
		}
		else
		{
			m_p3DObj->Render();
		}
	}
	if (m_pUVEffect)m_pUVEffect->Render();

#ifdef _DEBUG
	// 判定の描画
	CollisionShape::Square square;

	/* 攻撃判定の描画 */
	if (m_ptagAttackData)
	{
		//if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
		{
			Vector3 wv[3];	// ワールドバーテックス
			Vector2 sv[3];	// スクリーンバーテックス

			memcpy_s(&square, sizeof(CollisionShape::Square), m_ptagAttackData->pCollisionShape, sizeof(CollisionShape::Square));
			square.pos += m_tagParamDesc.vPos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}
#endif
}


Shot::Maya::Maya(BasePlayer *pPlayer,
	AttackData *pAttackData,
	BaseUVEffect *pObj,
	const Vector3 &vPos,
	const Vector3 &vVec) :
	Base(pPlayer, pAttackData, pObj)
{
	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = vPos;
	l_tagParamDesc.vVec;
	l_tagParamDesc.vVelocity = vVec * 2.0f;
	Base::Initialize(l_tagParamDesc);
}

Shot::Maya::~Maya()
{
}


void Shot::Maya::Update()
{
	// 基底クラスの更新
	Base::Update();

	// 弾丸の演出

	// マーヤは闇の光追加
	PointLightMgr->AddPointLight(m_tagParamDesc.vPos + Vector3(0, 5, 0),
		Vector3(.0f, 0.25f, .95f), 20, 10, 3, 1, 2);// ポイントライトエフェクト！

	// 軌跡のパーティクル
	ParticleManager::EffectShotLocus(m_tagParamDesc.vPos);//m_vVelocity
}

void Shot::Maya::Render()
{
	// 基底クラスの描画
	Base::Render();
}



/****************************************************/
//	地上スキル後にトスを上げるアラミタマ
/****************************************************/
const int Shot::AramitamaMushi::Land::c_SOJOURN_TIME = 34;
const int Shot::AramitamaMushi::Land::c_ATTACK_FRAME = 8;

Shot::AramitamaMushi::Land::Land(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj, false)
{
	DIR eTargetDir((pPlayer->GetTargetDir() == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT);
	pObj->SetAngle(DIR_ANGLE[(int)eTargetDir]);

	Vector3 l_vAppearPos(pPlayer->GetTargetPlayer()->GetPos());
	Vector3 l_vAddMove(pPlayer->GetTargetPlayer()->GetMove() * 1.75f);
	//if (l_vAppearPos.x + l_vAddMove.x )
	l_vAppearPos += l_vAddMove;

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vAppearPos;					// 出現位置
	l_tagParamDesc.vVec = Vector3((eTargetDir == DIR::LEFT) ? -1.0f : 1.0f, 0, 0);
	l_tagParamDesc.iSojournTime = c_SOJOURN_TIME;		// 滞在時間
	l_tagParamDesc.bPenetration = true;					// 当たっても消えない
	l_tagParamDesc.bCollisionOK = false;				// 途中から判定が始まる(モーションに合わせてtrueにする)
	Base::Initialize(l_tagParamDesc);
}

Shot::AramitamaMushi::Land::~Land()
{
}


void Shot::AramitamaMushi::Land::Update()
{
	// 基底クラスの更新
	Base::Update();

	// 攻撃時間になったら、判定をONにする
	if (m_tagParamDesc.iSojournTime == c_SOJOURN_TIME - c_ATTACK_FRAME) m_tagParamDesc.bCollisionOK = true;

	// トスミタマの演出

}

void Shot::AramitamaMushi::Land::Render()
{
	// 基底クラスの描画
	Base::Render();
}



/****************************************************/
//	しゃがみスキル時の隕石アラミタマ
/****************************************************/
const float Shot::AramitamaMushi::Squat::c_SPEED = 3.0f;

Shot::AramitamaMushi::Squat::Squat(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj, true)
{
	const Vector3 l_vAppearPos(0, 100, 0);
	Vector3 l_vTargetVec(pPlayer->GetTargetPlayer()->GetPos() - l_vAppearPos);
	l_vTargetVec.Normalize();

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vAppearPos;					// 出現位置
	l_tagParamDesc.vVec = l_vTargetVec;					// 移動方向
	l_tagParamDesc.vVelocity = l_vTargetVec * c_SPEED;	// 移動速度
	l_tagParamDesc.bPenetration = true;					// 当たっても消えない
	Base::Initialize(l_tagParamDesc);
}

Shot::AramitamaMushi::Squat::~Squat()
{
}


void Shot::AramitamaMushi::Squat::Update()
{
	// 基底クラスの更新
	Base::Update();

	// 隕石ミタマの演出

}

void Shot::AramitamaMushi::Squat::Render()
{
	// 基底クラスの描画
	Base::Render();
}


/****************************************************/
//	空中スキル時のD虫ミタマ
/****************************************************/
const int	Shot::AramitamaMushi::Aerial::c_SOJOURN_TIME = 270;
const float Shot::AramitamaMushi::Aerial::c_SPEED = 0.75f;
const float Shot::AramitamaMushi::Aerial::c_RANGE = 60;

Shot::AramitamaMushi::Aerial::Aerial(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj, false)
{
	pObj->SetAngle(DIR_ANGLE[(pPlayer->GetTargetDir() == DIR::LEFT) ? (int)DIR::RIGHT : (int)DIR::LEFT]);

	const float l_fPlayerX(pPlayer->GetPos().x);
	const float l_fTargetX(pPlayer->GetTargetPlayer()->GetPos().x + (pPlayer->GetTargetPlayer()->GetMove().x * 3));
	Vector3 l_vAppearPos(l_fTargetX, 0, 0);

	// 範囲外
	{
		const float l_vx(l_fTargetX - l_fPlayerX);
		if (fabsf(l_vx) > c_RANGE)
		{
			l_vAppearPos.x = l_fPlayerX + ((l_vx < 0) ? -1 : 1) * c_RANGE;
		}
	}
	Vector3 l_vTargetVec(0, 1, 0);
	l_vTargetVec.Normalize();

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vAppearPos;					// 出現位置
	l_tagParamDesc.vVec = l_vTargetVec;					// 移動方向
	l_tagParamDesc.vVelocity = l_vTargetVec * c_SPEED;	// 移動速度
	l_tagParamDesc.iSojournTime = c_SOJOURN_TIME;		// 滞在時間
	l_tagParamDesc.bPenetration = true;					// 当たっても消えない
	Base::Initialize(l_tagParamDesc);
}

Shot::AramitamaMushi::Aerial::~Aerial()
{
}


void Shot::AramitamaMushi::Aerial::Update()
{
	// 基底クラスの更新
	Base::Update();

	// 多段ヒット用の処理(xフレーム毎に判定を復活させる)
	if (m_tagParamDesc.iSojournTime % 7 == 0) m_tagParamDesc.bCollisionOK = true;

	// D虫ミタマの演出

}

void Shot::AramitamaMushi::Aerial::Render()
{
	// 基底クラスの描画
	Base::Render();
}
