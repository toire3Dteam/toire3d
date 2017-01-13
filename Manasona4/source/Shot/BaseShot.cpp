#include "TDNLIB.h"
#include "BaseShot.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"
#include "PointLight\PointLight.h"
#include "DeferredEx\DeferredEx.h"
#include "Effect\Particle.h"

Shot::Base::Base(BasePlayer *pPlayer, AttackData *pAttackData,
	iex3DObj *pObj):
	m_pPlayer(pPlayer),
	m_ptagAttackData(pAttackData),
	m_p3DObj(pObj),
	m_pUVEffect(nullptr),
	m_tagParamDesc()
{
	pObj->SetFrame(0);
}

Shot::Base::Base(BasePlayer *pPlayer, AttackData *pAttackData,
	BaseUVEffect *pObj) :
	m_pPlayer(pPlayer),
	m_ptagAttackData(pAttackData),
	m_p3DObj(nullptr),
	m_pUVEffect(pObj),
	m_tagParamDesc()
{
	m_pUVEffect->ActionRoop(m_tagParamDesc.vPos);
}

void Shot::Base::Update()
{
	// �؍ݎ��Ԃ���̔�ѓ���̏ꍇ
	if (m_tagParamDesc.iSojournTime != c_NO_SOJOURN_TIME)
	{
		// �؍ݎ��Ԃ��I������������
		if (--m_tagParamDesc.iSojournTime <= 0) m_tagParamDesc.bErase = true;
	}

	// ��{�I�Ȉړ��ʂ̍X�V
	m_tagParamDesc.vVelocity += m_tagParamDesc.vAccel;
	m_tagParamDesc.vPos += m_tagParamDesc.vVelocity;

	// �I�u�W�F�̍X�V
	if (m_p3DObj)
	{
		m_p3DObj->Animation();
		Math::SetTransMatrixFrontVec(&m_p3DObj->TransMatrix, m_tagParamDesc.vPos, m_tagParamDesc.vVec);
		m_p3DObj->SetPos(m_tagParamDesc.vPos);
		m_p3DObj->Update();
	}

	else if (m_pUVEffect)
	{
		m_pUVEffect->SetPos(m_tagParamDesc.vPos);
		m_pUVEffect->SetAngleAnimation(
			Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)),
			Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y))
			);


		// UV�G�t�F�N�g
		m_pUVEffect->Update();
	}

}

void Shot::Base::Render()
{

	// �����ōs������(�ʒu�E�ړ��x�N�g����񂩂�)
	//Math::SetTransMatrixFrontVec(&m_pObj->TransMatrix, m_vPos, m_vVec);
	if(m_p3DObj)m_p3DObj->Render();
	if (m_pUVEffect)m_pUVEffect->Render();

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
	// ���N���X�̍X�V
	Base::Update();

	// �e�ۂ̉��o

	// �}�[���͈ł̌��ǉ�
	PointLightMgr->AddPointLight(m_tagParamDesc.vPos + Vector3(0, 5, 0),
		Vector3(.0f, 0.25f, .95f), 20, 10, 3, 1, 2);// �|�C���g���C�g�G�t�F�N�g�I

	// �O�Ղ̃p�[�e�B�N��
	ParticleManager::EffectShotLocus(m_tagParamDesc.vPos);//m_vVelocity
}

void Shot::Maya::Render()
{
	// ���N���X�̕`��
	Base::Render();
}



/****************************************************/
//	�n��X�L����Ƀg�X���グ��A���~�^�}
/****************************************************/
const int Shot::AramitamaMushi::Land::c_SOJOURN_TIME = 34;
const int Shot::AramitamaMushi::Land::c_ATTACK_FRAME = 8;

Shot::AramitamaMushi::Land::Land(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj)
{
	const Vector3 l_vApeearPos(pPlayer->GetTargetPlayer()->GetPos());

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vApeearPos;					// �o���ʒu
	l_tagParamDesc.iSojournTime = c_SOJOURN_TIME;		// �؍ݎ���
	l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	l_tagParamDesc.bCollisionOK = false;				// �r�����画�肪�n�܂�(���[�V�����ɍ��킹��true�ɂ���)
	Base::Initialize(l_tagParamDesc);
}

Shot::AramitamaMushi::Land::~Land()
{
}


void Shot::AramitamaMushi::Land::Update()
{
	// ���N���X�̍X�V
	Base::Update();

	// �U�����ԂɂȂ�����A�����ON�ɂ���
	if (m_tagParamDesc.iSojournTime == c_SOJOURN_TIME - c_ATTACK_FRAME) m_tagParamDesc.bCollisionOK = true;

	// �g�X�~�^�}�̉��o

}

void Shot::AramitamaMushi::Land::Render()
{
	// ���N���X�̕`��
	Base::Render();
}



/****************************************************/
//	���Ⴊ�݃X�L������覐΃A���~�^�}
/****************************************************/
const float Shot::AramitamaMushi::Squat::c_SPEED = 3.0f;

Shot::AramitamaMushi::Squat::Squat(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj)
{
	const Vector3 l_vApeearPos(0, 100, 0);
	Vector3 l_vTargetVec(pPlayer->GetTargetPlayer()->GetPos() - l_vApeearPos);
	l_vTargetVec.Normalize();

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vApeearPos;					// �o���ʒu
	l_tagParamDesc.vVec = l_vTargetVec;					// �ړ�����
	l_tagParamDesc.vVelocity = l_vTargetVec * c_SPEED;	// �ړ����x
	l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	Base::Initialize(l_tagParamDesc);
}

Shot::AramitamaMushi::Squat::~Squat()
{
}


void Shot::AramitamaMushi::Squat::Update()
{
	// ���N���X�̍X�V
	Base::Update();

	// 覐΃~�^�}�̉��o

}

void Shot::AramitamaMushi::Squat::Render()
{
	// ���N���X�̕`��
	Base::Render();
}


/****************************************************/
//	�󒆃X�L������D���~�^�}
/****************************************************/
const int	Shot::AramitamaMushi::Aerial::c_SOJOURN_TIME = 270;
const float Shot::AramitamaMushi::Aerial::c_SPEED = 0.5f;

Shot::AramitamaMushi::Aerial::Aerial(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj)
{
	const Vector3 l_vApeearPos(pPlayer->GetTargetPlayer()->GetPos().x, 0, 0);
	Vector3 l_vTargetVec(0, 1, 0);
	l_vTargetVec.Normalize();

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vApeearPos;					// �o���ʒu
	l_tagParamDesc.vVec = l_vTargetVec;					// �ړ�����
	l_tagParamDesc.vVelocity = l_vTargetVec * c_SPEED;	// �ړ����x
	l_tagParamDesc.iSojournTime = c_SOJOURN_TIME;		// �؍ݎ���
	l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	Base::Initialize(l_tagParamDesc);
}

Shot::AramitamaMushi::Aerial::~Aerial()
{
}


void Shot::AramitamaMushi::Aerial::Update()
{
	// ���N���X�̍X�V
	Base::Update();

	// ���i�q�b�g�p�̏���(x�t���[�����ɔ���𕜊�������)
	if (m_tagParamDesc.iSojournTime % 7 == 0) m_tagParamDesc.bCollisionOK = true;

	// D���~�^�}�̉��o

}

void Shot::AramitamaMushi::Aerial::Render()
{
	// ���N���X�̕`��
	Base::Render();
}
