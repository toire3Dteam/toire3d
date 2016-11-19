#include "TDNLIB.h"
#include "BaseShot.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"

Shot::Base::Base(BasePlayer *pPlayer, AttackData *pAttackData, iexMesh *pObj, const Vector3 &vPos, const Vector3 &vVec, const Vector3 &vVelocity, const Vector3 &vAccel):m_pPlayer(pPlayer), m_ptagAttackData(pAttackData), m_pObj(pObj), m_vPos(vPos), m_vVec(vVec), m_vVelocity(vVelocity), m_vAccel(vAccel), m_bErase(false)
{

}

void Shot::Base::Render()
{
	m_pObj->SetPos(m_vPos);
	m_pObj->SetAngle(0, 0, atan2f(-m_vVec.x, m_vVec.y));
	m_pObj->Update();

	// 自分で行列を作る(位置・移動ベクトル情報から)
	//Math::SetTransMatrixFrontVec(&m_pObj->TransMatrix, m_vPos, m_vVec);
	m_pObj->Render();

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
			square.pos += m_vPos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}
#endif
}


Shot::Maya::Maya(BasePlayer *pPlayer, AttackData *pAttackData, iexMesh *pObj, const Vector3 &vPos, const Vector3 &vVec) :Base(pPlayer, pAttackData, pObj, vPos, vVec, vVec*1.25f)
{
}

Shot::Maya::~Maya()
{
}


void Shot::Maya::Update()
{
// 基底クラスの更新
Base::Update();
}

void Shot::Maya::Render()
{
// 基底クラスの描画
Base::Render();
}