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
	m_tagParamDesc(),
	m_iHitStopFrame(0),
	m_bTurnOverUVEffect(false)
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
	m_tagParamDesc(),
	m_iHitStopFrame(0)
{
	m_pUVEffect->ActionRoop(m_tagParamDesc.vPos);
}

void Shot::Base::Update()
{
	// �q�b�g�X�g�b�v����
	if (m_iHitStopFrame > 0)
	{
		m_iHitStopFrame--;
		return;
	}

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

		// �p������
		if (m_bAttitude)
		{
			// �����ōs������(�ʒu�E�ړ��x�N�g����񂩂�)
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

		if (m_bTurnOverUVEffect == true)
		{
			m_pUVEffect->SetAngleAnimation(
				Vector3(0, 3.14f, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)),
				Vector3(0, 3.14f, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y))
				);
		}
		else
		{
			m_pUVEffect->SetAngleAnimation(
				Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)),
				Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y))
				);
		}
	


		// UV�G�t�F�N�g
		m_pUVEffect->Update();
	}

}

void Shot::Base::Render()
{
	if (m_p3DObj)
	{
		if (m_bAttitude)
		{
			// TransMatrix�𒼂ł��������̂ŁA�X�V���s�v�̕`��
			m_p3DObj->Render(false);
		}
		else
		{
			m_p3DObj->Render();
		}
	}
	if (m_pUVEffect)
	{
		// �`��O�Ɉʒu�����X�V
		m_pUVEffect->SetPos(m_tagParamDesc.vPos);
		if (m_bTurnOverUVEffect == true)
		{
			m_pUVEffect->SetAngleAnimation(
				Vector3(0, 3.14f, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)),
				Vector3(0, 3.14f, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y))
				);
		}
		else
		{
			m_pUVEffect->SetAngleAnimation(
				Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)),
				Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y))
				);
		}
		
		// ���b�V���̍X�V
		m_pUVEffect->GetUV()->GetObj()->SetPos(m_pUVEffect->GetPos());
		m_pUVEffect->GetUV()->GetObj()->SetAngle(Vector3(0, 0, atan2f(-m_tagParamDesc.vVec.x, m_tagParamDesc.vVec.y)));
		m_pUVEffect->GetUV()->GetObj()->Update();

		m_pUVEffect->Render();
	}

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
	l_tagParamDesc.vVec = vVec;
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
//	�n��X�L����Ƀg�X���グ�镕�󂳂ꂵ��
/****************************************************/
const int Shot::AramitamaMushi::Land::c_SOJOURN_TIME = 34;
const int Shot::AramitamaMushi::Land::c_ATTACK_FRAME = 8;

Shot::AramitamaMushi::Land::Land(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj,
	AramitamaRippleEffect* pEffect) :
	m_bHit(false),
	Base(pPlayer, pAttackData, pObj, false)
{
	DIR eTargetDir((pPlayer->GetTargetDir() == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT);
	pObj->SetAngle(DIR_ANGLE[(int)eTargetDir]);

	Vector3 l_vAppearPos(pPlayer->GetTargetPlayer()->GetPos());
	Vector3 l_vAddMove(pPlayer->GetTargetPlayer()->GetMove() * 1.75f);
	//if (l_vAppearPos.x + l_vAddMove.x )
	//l_vAppearPos += l_vAddMove;
	l_vAppearPos += Vector3(15 * pPlayer->GetDirVecX() , -15, 0);


	Vector3 l_vTargetPos = pPlayer->GetTargetPlayer()->GetPos();
	l_vTargetPos.x += +(pPlayer->GetTargetPlayer()->GetMove().x * 3);// �����Move�l�l��

	Vector3 l_vToTargetVec = l_vTargetPos - l_vAppearPos;
	l_vToTargetVec.Normalize();


	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vAppearPos;					// �o���ʒu
	l_tagParamDesc.vVec = l_vToTargetVec;
	l_tagParamDesc.vVelocity = l_tagParamDesc.vVec * 2;
	l_tagParamDesc.iSojournTime = c_SOJOURN_TIME;		// �؍ݎ���
	l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	l_tagParamDesc.bCollisionOK = false;				// �r�����画�肪�n�܂�(���[�V�����ɍ��킹��true�ɂ���)
	Base::Initialize(l_tagParamDesc);

	// �G�t�F�N�g
	//m_pShotEF = new AramitamaShotEffect();
	m_pRefShot = pEffect;


	// �U�������������p�x��ς���
	float l_ZAngle = atan2(-l_tagParamDesc.vVec.x, l_tagParamDesc.vVec.y);
	m_pRefShot->Action(l_tagParamDesc.vPos, 1, 1, Vector3(0, 0, l_ZAngle), Vector3(0, 0, l_ZAngle));//Vector3(0, 0, l_ZAngle)

}

Shot::AramitamaMushi::Land::~Land()
{
	// �g�X�A�V�X�g�̉��o�~�߂�
	//m_pRefShot->StopRoop();
}


void Shot::AramitamaMushi::Land::Update()
{
	// ���N���X�̍X�V
	Base::Update();

	// �U�����ԂɂȂ�����A�����ON�ɂ���
	if(!m_bHit) if (m_tagParamDesc.iSojournTime == c_SOJOURN_TIME - c_ATTACK_FRAME)
	{
		m_tagParamDesc.bCollisionOK = true;
		m_bHit = true;
	}

	// �g�X�A�V�X�g�̉��o
	m_pRefShot->SetPos(m_tagParamDesc.vPos + m_tagParamDesc.vVec * 10);
	m_pRefShot->Update();
}

void Shot::AramitamaMushi::Land::Render()
{
	// ���N���X�̕`��
	Base::Render();

	// �g�X�A�V�X�g�̉��o
	m_pRefShot->Render();
}



/****************************************************/
//	���Ⴊ�݃X�L������覐Ε��󂳂ꂵ��
/****************************************************/
const float Shot::AramitamaMushi::Squat::c_SPEED = 4.25f;

Shot::AramitamaMushi::Squat::Squat(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj,
	AramitamaShotEffect* pEffect) :
	Base(pPlayer, pAttackData, pObj, true)
{
	Vector3 l_vAppearPos = pPlayer->GetPos();
	l_vAppearPos += Vector3(50 * -pPlayer->GetDirVecX(), 100, 0);

	Vector3 l_vTargetVec(pPlayer->GetTargetPlayer()->GetPos() - l_vAppearPos);
	l_vTargetVec.Normalize();

	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = l_vAppearPos;					// �o���ʒu
	l_tagParamDesc.vVec = l_vTargetVec;					// �ړ�����
	l_tagParamDesc.vVelocity = l_vTargetVec * c_SPEED;	// �ړ����x
	l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	Base::Initialize(l_tagParamDesc);

	// �G�t�F�N�g
	//m_pShotEF = new AramitamaShotEffect();
	m_pRefShot = pEffect;
	
	
	// �U�������������p�x��ς���
	float l_ZAngle = atan2(-l_tagParamDesc.vVec.x, l_tagParamDesc.vVec.y);
	m_pRefShot->Action(l_tagParamDesc.vPos, 2, 2, Vector3(0, 0, l_ZAngle), Vector3(0, 0, l_ZAngle));//Vector3(0, 0, l_ZAngle)


}

Shot::AramitamaMushi::Squat::~Squat()
{
	//SAFE_DELETE(m_pShotEF);
}


void Shot::AramitamaMushi::Squat::Update()
{
	// ���N���X�̍X�V
	Base::Update();

	// 覐΃A�V�X�g�̉��o
	//m_pRefShot->SetAngleAnimation();
	m_pRefShot->SetPos(m_tagParamDesc.vPos);
	m_pRefShot->Update();

}

void Shot::AramitamaMushi::Squat::Render()
{
	// ���N���X�̕`��
	Base::Render();

	// �G�t�F�N�g�̕`��
	m_pRefShot->Render();
}


/****************************************************/
//	�󒆃X�L������D����
/****************************************************/
const int	Shot::AramitamaMushi::Aerial::c_SOJOURN_TIME = 80;
const float Shot::AramitamaMushi::Aerial::c_SPEED = 0.75f;
const float Shot::AramitamaMushi::Aerial::c_RANGE = 200;

Shot::AramitamaMushi::Aerial::Aerial(BasePlayer *pPlayer,
	AttackData *pAttackData,
	iex3DObj *pObj) :
	Base(pPlayer, pAttackData, pObj, false)
{
	pObj->SetAngle(DIR_ANGLE[(pPlayer->GetTargetDir() == DIR::LEFT) ? (int)DIR::RIGHT : (int)DIR::LEFT]);

	const float l_fPlayerX(pPlayer->GetPos().x);
	const float l_fTargetX(pPlayer->GetTargetPlayer()->GetPos().x + (pPlayer->GetTargetPlayer()->GetMove().x * 3));
	Vector3 l_vAppearPos(l_fTargetX, 0, 0);

	// �͈͊O
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
	l_tagParamDesc.vPos = l_vAppearPos;					// �o���ʒu
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

	// D�����̉��o

}

void Shot::AramitamaMushi::Aerial::Render()
{
	// ���N���X�̕`��
	Base::Render();
}



Shot::Ninjin::Ninjin(BasePlayer *pPlayer,
	AttackData *pAttackData,
	BaseUVEffect/*iex3DObj*/ *pObj,
	const Vector3 &vPos,
	const Vector3 &vVec) :
	Base(pPlayer, pAttackData, pObj)
{
	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = vPos;
	l_tagParamDesc.vVec = vVec;
	//l_tagParamDesc.vVelocity = vVec * 2.0f;
	l_tagParamDesc.vAccel = vVec * 0.0001f;
	Base::Initialize(l_tagParamDesc);

	//DIR eTargetDir((pPlayer->GetTargetDir() == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT);
	//pObj->SetAngle(DIR_ANGLE[(int)eTargetDir]);

	//Vector3 l_vAppearPos(pPlayer->GetTargetPlayer()->GetPos());
	//Vector3 l_vAddMove(pPlayer->GetTargetPlayer()->GetMove() * 1.75f);
	////if (l_vAppearPos.x + l_vAddMove.x )
	////l_vAppearPos += l_vAddMove;
	//l_vAppearPos += Vector3(15 * pPlayer->GetDirVecX(), -15, 0);


	//Vector3 l_vTargetPos = pPlayer->GetTargetPlayer()->GetPos();
	//l_vTargetPos.x += +(pPlayer->GetTargetPlayer()->GetMove().x * 3);// �����Move�l�l��

	//Vector3 l_vToTargetVec = l_vTargetPos - l_vAppearPos;
	//l_vToTargetVec.Normalize();

	//const int c_SOJOURN_TIME = 34;
	//ParamDesc l_tagParamDesc;
	//l_tagParamDesc.vPos = l_vAppearPos;					// �o���ʒu
	//l_tagParamDesc.vVec = l_vToTargetVec;
	//l_tagParamDesc.vVelocity = l_tagParamDesc.vVec * 2;
	//l_tagParamDesc.iSojournTime = c_SOJOURN_TIME;		// �؍ݎ���
	//l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	//l_tagParamDesc.bCollisionOK = false;				// �r�����画�肪�n�܂�(���[�V�����ɍ��킹��true�ɂ���)
	//Base::Initialize(l_tagParamDesc);

}

Shot::Ninjin::~Ninjin()
{
}


void Shot::Ninjin::Update()
{
	// ���N���X�̍X�V
	Base::Update();

	// �e�ۂ̍ő呬�x
	if (m_tagParamDesc.vVelocity.x >=  2.0f)
	{
		m_tagParamDesc.vVelocity.x = 2.0f;
	}

	// ����Ɋɋ}��
	m_tagParamDesc.vAccel += m_tagParamDesc.vVec* 0.0005f;

	// �ǉ�
	PointLightMgr->AddPointLight(m_tagParamDesc.vPos + Vector3(0, 5, 0),
		Vector3(.8f, 0.2f, .05f), 20, 10, 3, 1, 2);// �|�C���g���C�g�G�t�F�N�g�I

	// �O�Ղ̃p�[�e�B�N��
	ParticleManager::EffectNinjinShotLocus(m_tagParamDesc.vPos);//m_vVelocity
}

void Shot::Ninjin::Render()
{
	// ���N���X�̕`��
	Base::Render();
}






Shot::USAShot::USAShot(BasePlayer *pPlayer,
	AttackData *pAttackData,
	BaseUVEffect/*iex3DObj*/ *pObj,
	const Vector3 &vPos,
	const Vector3 &vVec) :
	Base(pPlayer, pAttackData, pObj)
{
	ParamDesc l_tagParamDesc;
	l_tagParamDesc.vPos = vPos;
	l_tagParamDesc.vVec = vVec;
	l_tagParamDesc.vVelocity = vVec * 5.0f;
	//l_tagParamDesc.vAccel = vVec * 0.001f;

	l_tagParamDesc.bPenetration = true;					// �������Ă������Ȃ�
	
	Base::Initialize(l_tagParamDesc);

	//ParamDesc l_tagParamDesc;
	//l_tagParamDesc.vPos = vPos;
	//l_tagParamDesc.vVec = vVec;
	//l_tagParamDesc.vVelocity = vVec * 2.0f;
	//Base::Initialize(l_tagParamDesc);

	// ���b�V�����]
	if (pPlayer->GetDir() == DIR::LEFT)
	{
		m_bTurnOverUVEffect = true;
	}
	else
	{
		m_bTurnOverUVEffect = false;
	}


	if (m_bTurnOverUVEffect == true)
	{
		m_pUVEffect->SetAngleAnimation(
			Vector3(0, 3.14f, 0.0f),
			Vector3(0, 3.14f, 0.0f)
			);
	}
	else
	{
		m_pUVEffect->SetAngleAnimation(
			Vector3(0, 0, 0.0f),
			Vector3(0, 0, 0.0f)
			);
	}


}

Shot::USAShot::~USAShot()
{
}


void Shot::USAShot::Update()
{
	// �q�b�g�X�g�b�v����
	if (m_iHitStopFrame > 0)
	{
		m_iHitStopFrame--;
		return;
	}

	// �؍ݎ��Ԃ���̔�ѓ���̏ꍇ
	if (m_tagParamDesc.iSojournTime != c_NO_SOJOURN_TIME)
	{
		// �؍ݎ��Ԃ��I������������
		if (--m_tagParamDesc.iSojournTime <= 0) m_tagParamDesc.bErase = true;
	}

	// ��{�I�Ȉړ��ʂ̍X�V
	m_tagParamDesc.vVelocity += m_tagParamDesc.vAccel;
	m_tagParamDesc.vPos += m_tagParamDesc.vVelocity;

	if (m_pUVEffect)
	{
		m_pUVEffect->SetPos(m_tagParamDesc.vPos);




		// UV�G�t�F�N�g
		m_pUVEffect->Update();
	}


	// �ǉ�
	//PointLightMgr->AddPointLight(m_tagParamDesc.vPos + Vector3(0, 5, 0),
	//	Vector3(.8f, 0.2f, .05f), 20, 10, 3, 1, 2);// �|�C���g���C�g�G�t�F�N�g�I

												   // �O�Ղ̃p�[�e�B�N��
	ParticleManager::EffectNinjinShotLocus(m_tagParamDesc.vPos);//m_vVelocity

}

void Shot::USAShot::Render()
{
	// ���N���X�̕`��

	if (m_pUVEffect)
	{
		// �`��O�Ɉʒu�����X�V
		m_pUVEffect->SetPos(m_tagParamDesc.vPos);

		// ���b�V���̍X�V
		m_pUVEffect->GetUV()->GetObj()->SetPos(m_pUVEffect->GetPos());
		m_pUVEffect->GetUV()->GetObj()->Update();

		m_pUVEffect->Render();
	}



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

