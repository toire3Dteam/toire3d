#include "TDNLIB.h"
#include "BaseShot.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"
#include "PointLight\PointLight.h"
#include "DeferredEx\DeferredEx.h"
#include "Effect\Particle.h"

Shot::Base::Base(BasePlayer *pPlayer, AttackData *pAttackData,
	BaseUVEffect *pObj,
	const Vector3 &vPos,
	const Vector3 &vVec,
	const Vector3 &vVelocity,
	const Vector3 &vAccel):
	m_pPlayer(pPlayer),
	m_ptagAttackData(pAttackData),
	m_pObj(pObj),
	m_vPos(vPos),
	m_vVec(vVec),
	m_vVelocity(vVelocity),
	m_vAccel(vAccel),
	m_bErase(false)
{
	m_pObj->ActionRoop(m_vPos);
}

void Shot::Base::Update()
{
	// ��{�I�Ȉړ��ʂ̍X�V
	m_vVelocity += m_vAccel;
	m_vPos += m_vVelocity;


	// �I�u�W�F�̍X�V
	m_pObj->SetPos(m_vPos);
	m_pObj->SetAngleAnimation(
		Vector3(0, 0, atan2f(-m_vVec.x, m_vVec.y)),
		Vector3(0, 0, atan2f(-m_vVec.x, m_vVec.y))
		);

	// UV�G�t�F�N�g
	m_pObj->Update();

}

void Shot::Base::Render()
{

	// �����ōs������(�ʒu�E�ړ��x�N�g����񂩂�)
	//Math::SetTransMatrixFrontVec(&m_pObj->TransMatrix, m_vPos, m_vVec);
	m_pObj->Render();

#ifdef _DEBUG
	// ����̕`��
	CollisionShape::Square square;

	/* �U������̕`�� */
	if (m_ptagAttackData)
	{
		//if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
		{
			Vector3 wv[3];	// ���[���h�o�[�e�b�N�X
			Vector2 sv[3];	// �X�N���[���o�[�e�b�N�X

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


Shot::Maya::Maya(BasePlayer *pPlayer,
	AttackData *pAttackData,
	BaseUVEffect *pObj,
	const Vector3 &vPos,
	const Vector3 &vVec) :
	Base(pPlayer, pAttackData, pObj, vPos, vVec, vVec*2.0f)
{
}

Shot::Maya::~Maya()
{
}


void Shot::Maya::Update()
{
// ���N���X�̍X�V
Base::Update();

// �e�ۂ̉��o

// �}�[���͈ł̌��ǉ�
PointLightMgr->AddPointLight(m_vPos + Vector3(0, 5, 0), 
	Vector3(.0f, 0.25f, .95f), 20, 10, 3, 1, 2);// �|�C���g���C�g�G�t�F�N�g�I

// �O�Ղ̃p�[�e�B�N��
ParticleManager::EffectShotLocus(m_vPos);//m_vVelocity

}

void Shot::Maya::Render()
{
// ���N���X�̕`��
Base::Render();
}