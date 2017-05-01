#include "TDNLIB.h"
#include "Stand.h"
#include "../Sound/SoundManager.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "Effect\Particle.h"
#include "../Shot/BaseShot.h"

// �U���t���[�����ǂݍ���
void Stand::Base::LoadActionFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�A�N�V�����t���[���̃e�L�X�g�����ĂȂ�");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		ifs >> skip;

		// �n���A�����A�d���t���[����ǂݍ���
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
		// �I�[
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
	// �X�^���h�Q�[�W�X�V
	if(bControl)StandGageUpdate();

	// �X�P�[������
	if (m_bActive == true) 
	{
		m_fScale += 0.25f;
	}
	else
	{
		m_fScale -= 0.25f;
	}

	// �N�����v
	m_fScale = Math::Clamp(m_fScale, 0.0f, 1.0f);


	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (m_bActive == true)
	{
		// �t���[�����X�g�X�V
		// �A�N�V�����t���[���̍X�V
		// �U���f�[�^������Ȃ�
		if (m_pAttackData[(int)m_ActionType] != nullptr)
		{
			// ���̌o�ߎ��Ԃƃf�B���C�t���[���ɂȂ�����
			if (m_pAttackData[(int)m_ActionType]->WhiffDelayFrame == m_CurrentActionFrame)
			{
				// (A���)�����ōU�����肪���������u�Ԃ�����Ă��Ă�

				// �U��G�t�F�N�g�����i���j�I
				m_pPlayer->AddEffectAction(m_pos + Vector3(0, 5, -3), m_pAttackData[(int)m_ActionType]->WhiffEffectType);

				LPCSTR SE_ID(m_pAttackData[(int)m_ActionType]->WhiffSE);
				// ��U��SE�����Ă���
				if (SE_ID)
				{
					// �f�B���C�t���[���o�߂�����Đ�
					se->Play((LPSTR)SE_ID);
				}
			}


			// ���蕜���t���[��
			if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::RECOVERY_HIT)
			{
				// ���胊�Z�b�g
				GetAttackData()->bHit = GetAttackData()->bHitSuccess = false;
			}
		}



		// �t���[���Ō�܂ōĐ�������
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame++] == FRAME_STATE::END)
		{
			// 
			Break();
			// ���A�N�e�B�u���I�t(�{���������甼�����t�F�[�h�A�E�g���g��)
			//m_bActive = false;

			// �A��G�t�F�N�g
			//m_pExitEffect->Action(m_pos + Vector3(0, 0, -10), 1.25f, 0.25f);
		}

	}// �o�Ă���

	// 3D�I�u�W�F�N�g�̍X�V
	m_pObj->Animation();
	m_pObj->SetAngle(DIR_ANGLE[(int)m_dir]);
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// �G�t�F�N�g�X�V
	m_pSummonEffect->Update();
	m_pSummonWaveEffect->Update();
	m_pExitEffect->Update();
}
 
// �X�^���h�Q�[�W�X�V
void Stand::Base::StandGageUpdate()
{
	// �X�^���h�X�g�b�N���ő�l�܂ŗ��܂��Ă��Ȃ�������
	if (m_standStock < m_standStockMAX)
	{	
		m_standGage++;// �Q�[�W���Z

		// �X�^���h�Q�[�W���ő�l�܂ŗ��܂�����
		if (m_standGage >= m_standGageMAX)
		{
			m_standStock++;
			m_standGage = 0;

			// UI�Ƀ��b�Z�[�W�𑗂�
			SIDE side = m_pPlayer->GetSide();
			MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::UI_MGR,
				MESSAGE_TYPE::PERSONA_CARD_COUNT_UP, &side);
		}

	}
	else m_standGage = m_standGageMAX;
}

void Stand::Base::Render(tdnShader* shader, char* name)
{
	// �G�t�F�N�g�`��
	m_pSummonEffect->Render();
	m_pSummonWaveEffect->Render();
	m_pExitEffect->Render();

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (m_bActive == false && m_fScale <= 0.0f) return;

	// 3D�I�u�W�F�N�g�̕`��
	m_pObj->Render(shader,name);


#ifdef _DEBUG
	// ����̕`��
	CollisionShape::Square square;

	/* �U������̕`�� */
	if (m_pAttackData[(int)m_ActionType])
	{
		// �U������A�N�e�B�u���Ȃ�
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
		{
			Vector3 wv[3];	// ���[���h�o�[�e�b�N�X
			Vector2 sv[3];	// �X�N���[���o�[�e�b�N�X

			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// ����pos�͐��+(�E)�Ȃ̂ŁA�������Ȃ�t�ɂ���
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
	// UI�Ƀ��b�Z�[�W�𑗂�
	SIDE side = m_pPlayer->GetSide();
	MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::UI_MGR,
		MESSAGE_TYPE::PERSONA_CARD_COUNT_DOWN, &side);

	// �X�g�b�N������
	m_standStock--;
	m_standGage = 0;// (12/24)�Q�[�W0

	m_ActionType = type;		// �Z�̃^�C�v�ݒ�
	m_CurrentActionFrame = 0;	// �t���[�����Z�b�g

	if(m_pAttackData[(int)m_ActionType])m_pAttackData[(int)m_ActionType]->bHit = false;	// �q�b�g�t���O����

	// �Ƃ肠�����A�A�N�e�B�u�ɂ���
	m_bActive = true;

	// �v���C���[�̌��������炤
	m_dir = m_pPlayer->GetDir();

	m_fScale = 0.0f;
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	//m_pSummonEffect->Action(m_pos + Vector3(0, 0, -10), 1, 1);
	m_pSummonWaveEffect->Action(GetCenterPos(), 0.5f, 2.0f);

}

// �y���\�i�u���C�N
void Stand::Base::Break()
{
	if (m_bActive)
	{
		// �����Ƀu���C�N�̉��Ƃ��G�t�F�N�g�Ƃ�
		// �A��G�t�F�N�g
		m_pExitEffect->Action(m_pos + Vector3(0, 0, -10), 1.25f, 0.25f);
		m_bActive = false;

		if (m_pAttackData[(int)m_ActionType]) 
		{
			m_pAttackData[(int)m_ActionType]->bHit = false;// (�ǉ�)�q�b�g�t���O����
		}

	}
}

/******************************/
//		���R�C
/******************************/
Stand::Mokoi::Mokoi(BasePlayer *pPlayer) :Base(pPlayer)
{
	// ���R�C�̎���
	m_pObj = new iex3DObj("DATA/CHR/Stand/Mokoi/mokoi.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// �^�C�v�ݒ�
	m_ePartnerType = PARTNER::MOKOI;

	// �A�N�V�����t���[�����[�h����
	LoadActionFrameList("DATA/CHR/Stand/Mokoi/FrameList.txt");

	/*****************************/
	// ���R�C�̊�{�X�e�[�^�X
	m_standStockMAX = 1;
	m_standGageMAX = 60*8;

	/*****************************/
	// ���̃L�����N�^�[�̃A�C�R��
	m_pIcon= new tdn2DAnim("Data/UI/Game/PersonaIcon/Mokoi.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/Mokoi.png");

	//==============================================================================================================
	//	�n��j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 660;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "��U��1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::UPPER;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 1.0f;
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 14;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 17;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(5.5f, 17, 0);

	//==============================================================================================================
	//	���Ⴊ��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 1160;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "��U��1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::RUN;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.25f, 6);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = .25f;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 1.0f;
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 14;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 10;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(5.5f, 10, 0);

	// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// new����ĂȂ�������X���[
		//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// �A�b�p�[�̋�U��t���[������
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;
}

void Stand::Mokoi::Update(bool bControl)
{
	// ���N���X�̍X�V
	Base::Update(bControl);

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (!m_bActive) return;

	// ���肪�o���u�ԓ���
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::START)
	{
		float l_fPow = 0.0f;
		// ���Ⴊ�݂Ŕ�������ƑO�����ɐi�ޗ͂��グ��
		if (m_ActionType == SKILL_ACTION_TYPE::SQUAT)l_fPow = 0.75f;

		// �L�����N�^�[�̈ړ�
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
		// �L�����N�^�[�̈ړ�
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

	// �Ƃ肠�����A�v���C���[�Ɠ������W
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.95f);
	if (m_pPlayer->GetDir() == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// �󒆃W�����v����
	m_pPlayer->SetAerialJump(true);

	// ���[�V����
	m_pObj->SetMotion(0);

	// ���N���X�̃A�N�V����
	// �󒆂͂��ׂĒʏ�Z��
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
//		�}�[��
/******************************/
Stand::Maya::Maya(BasePlayer *pPlayer) :Base(pPlayer)
{
	// �}�[���̎���
	m_pObj = new iex3DObj("DATA/CHR/Stand/Maya/maya.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// �^�C�v�ݒ�
	m_ePartnerType = PARTNER::MAYA;

	// �ʂ̎���
	m_pBullet = new  MayaShotEffect();/*iexMesh("DATA/UVeffect/Airou/AirouBoost.IMO");*/
	m_pBullet2 = new  MayaShot2Effect();

	// �A�N�V�����t���[�����[�h����
	LoadActionFrameList("DATA/CHR/Stand/Maya/FrameList.txt");

	/*****************************/
	// �}�[���̊�{�X�e�[�^�X
	m_standStockMAX = 1;
	m_standGageMAX = 60*5;

	/*****************************/
	// ���̃L�����N�^�[�̃A�C�R��
	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/maya.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/maya.png");

	//==============================================================================================================
	//	�n��j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "��U��1";
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
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	���Ⴊ��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 1000;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "��U��1";
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
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	�󒆃j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->WhiffSE = "��U��1";
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
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->pCollisionShape->pos.Set(0, 0, 0);

	//==============================================================================================================
	//	�󒆉�
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->damage = 800;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->WhiffSE = "��U��1";
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
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pCollisionShape->width = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pCollisionShape->height = 5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->pCollisionShape->pos.Set(0, 0, 0);

	// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// new����ĂȂ�������X���[
		for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		//MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
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
	// ���N���X�̍X�V
	Base::Update(bControl);

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (!m_bActive) return;

	// ���肪�o���u�Ԃɔ�ѓ��������
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
	{
		Vector3 ShotVec;
	
		// ���ŏo�����u�Ԃɏo��
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

		// �V���b�g�̌����t
		ShotVec.x *= m_pPlayer->GetDirVecX();

		// �}�Y���G�t�F�N�g�����I
		m_pPlayer->AddEffectAction(m_pos + Vector3(0, 4, -2), EFFECT_TYPE::MUZZLE_FLASH, ShotVec);

		Shot::Base *pNewShot = nullptr;

		// �ʂ̏����i�[
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
	// �Ƃ肠�����A�v���C���[�Ɠ������W
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 2.0f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// ���N���X�̃A�N�V����(�S�����ʌn)
	Base::Action(type);


}




/******************************/
//		�w�e
/******************************/
Stand::Hete::Hete(BasePlayer *pPlayer) :Base(pPlayer)
{
	// ���R�C�̎���
	m_pObj = new iex3DObj("DATA/CHR/Stand/Hete/hete.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// �^�C�v�ݒ�
	m_ePartnerType = PARTNER::HETE;

	// �A�N�V�����t���[�����[�h����
	LoadActionFrameList("DATA/CHR/Stand/Hete/FrameList.txt");

	// ���i�p
	for (int i = 0; i < 8; i++)
	{
		m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
		m_ActionFrameList[(int)SKILL_ACTION_TYPE::SQUAT][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
	}

	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 13] = FRAME_STATE::RECOVERY_HIT;
	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 19] = FRAME_STATE::RECOVERY_HIT;

	/*****************************/
	// ��{�X�e�[�^�X
	m_standStockMAX = 1;
	m_standGageMAX = 60 * 8;

	/*****************************/
	// ���̃L�����N�^�[�̃A�C�R��
	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/hete.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/hete.png");

	//==============================================================================================================
	//	�n��j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 160;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "��U��1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = 0.5f;	//	��߂�
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 0.99f;
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 6.5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(0.5f, 6, 0);

	//==============================================================================================================
	//	�n�サ�Ⴊ��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 160;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "�q�b�g6";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "��U��1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::MULTIPLE_HIT_BLOW;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::WHIFF;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::NONE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.2f, 2);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = 0.5f;	//	��߂�
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 0.99f;
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 6.5;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 8;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(0.5f, 6, 0);

	// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// new����ĂȂ�������X���[
											//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE) { m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// �A�b�p�[�̋�U��t���[������
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;
}


void Stand::Hete::Update(bool bControl)
{
	// ���N���X�̍X�V
	Base::Update(bControl);

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (!m_bActive) return;



	// �O�����ɐi��
	if (m_ActionType == SKILL_ACTION_TYPE::LAND)
	{
		// ���肪�o���u�ԓ���
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE)
		{

			// �L�����N�^�[�̈ړ�
			if (m_dir == DIR::LEFT)
			{
				m_pos.x -= 0.43f;
			}
			else
			{
				m_pos.x += 0.43f;
			}

		}

		//// ����I����Ă���
		////if (m_ActionType[(int)SKILL_ACTION_TYPE::LAND]->Execute())
		////{
		////	 ������葖��̂��I��点��
		////		m_ActionType(SKILL_ACTION_TYPE::NO_ACTION);
		////	return;
		////}

		// �q�b�g�񐔌v��
		if (m_ActionFrameList[(int)BASE_ACTION_STATE::SKILL][m_CurrentActionFrame]
			== FRAME_STATE::RECOVERY_HIT)
		{
			//// �Ō�̈ꌂ������������
			//if (++m_iHitCount >= 3)
			//{
			//	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.5f, 0);
			//	m_pTeki->m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.85f, 1.65f);
			//}

			// �R���{���[�g�������Ȃ��悤�ɂ���
			//m_ActionDatas[(int)BASE_ACTION_STATE::SKILL].pAttackData->fComboRate = 1.0f;
		}
	}
}

void Stand::Hete::Action(SKILL_ACTION_TYPE type)
{

	// �Ƃ肠�����A�v���C���[�Ɠ������W
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, .675f);
	if (m_pPlayer->GetDir() == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 11.0f;

	// (TODO)2/4 ���͂����Ń��[�V������ݒ�
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

	// ���N���X�̃A�N�V����
	// �󒆂͂��ׂĂ��Ⴊ�݋Z��
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
//		�A�b�^�J�C
/******************************/
Stand::Attakai::Attakai(BasePlayer *pPlayer) :Base(pPlayer)
{
	// �A�b�^�J�C�̎���
	m_pObj = new iex3DObj("DATA/CHR/Stand/Attakai/Attakai.IEM");
	m_pObj->SetPos(m_pos);
	m_pObj->SetScale(m_fScale, 1.0f, m_fScale);
	m_pObj->Update();

	// �^�C�v�ݒ�
	m_ePartnerType = PARTNER::ATTAKAI;

	// �A�N�V�����t���[�����[�h����
	LoadActionFrameList("DATA/CHR/Stand/Attakai/FrameList.txt");

	// ���i�p
	//for (int i = 0; i < 8; i++)
	//{
	//	m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
	//	m_ActionFrameList[(int)SKILL_ACTION_TYPE::SQUAT][20 + (i * 16)] = FRAME_STATE::RECOVERY_HIT;
	//}

	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 13] = FRAME_STATE::RECOVERY_HIT;
	//m_ActionFrameList[(int)SKILL_ACTION_TYPE::LAND][20 + 19] = FRAME_STATE::RECOVERY_HIT;

	m_iActiveStartFrame = 0;

	/*****************************/
	// ��{�X�e�[�^�X
	m_standStockMAX = 1;
	m_standGageMAX = 60 * 18;

	/*****************************/
	// ���̃L�����N�^�[�̃A�C�R��
	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/attakai.png");
	m_pIconRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/attakai.png");

	//==============================================================================================================
	//	�n��j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 3300;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitSE = "�q�b�g1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffSE = "�t�B�j�b�V����";
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::EXPLOSION;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.5f, 10);
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fGuardKnockBackPower = 0.5f;	//	��߂�
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->fComboRate = 0.99f;
	
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->width = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 25;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(0.0f, 18, 0);

	//==============================================================================================================
	//	�n�サ�Ⴊ��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 2300;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitSE = "�q�b�g1";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffSE = "�t�B�j�b�V����";
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->WhiffEffectType = EFFECT_TYPE::EXPLOSION;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->bFinish = true;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->AntiGuard = ANTIGUARD_ATTACK::ALL_BREAK;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.5f, 10);
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->GuardRecoveryFrame = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fGuardKnockBackPower = 0.5f;	//	��߂�
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->fComboRate = 0.99f;
	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
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
	// ����`��
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->width = 20;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->height = 25;
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(0.5f, 18, 0);

	// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
	FOR((int)SKILL_ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// new����ĂȂ�������X���[
											//for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE) { m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// �A�b�p�[�̋�U��t���[������
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;


	// �G�t�F�N�g
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
	// ���N���X�̍X�V
	Base::Update(bControl);

	// �A���O���Đݒ�
	m_pObj->SetAngle(3.14f);
	m_pObj->Update();




	// �G�t�F�N�g
	m_pBomEf->Update();
	m_fDangerRate = Math::Clamp((float)(m_CurrentActionFrame) / (float)(m_iActiveStartFrame), 0, 1.0f);
	m_pBomAddEf->GetPanel()->GetPic()->SetAlpha((int)(m_fDangerRate * 255.0f));
	m_pBomAddEf->Update();

	m_pBomEf->SetPos(m_pos + Vector3(0, 17, -6));
	m_pBomAddEf->SetPos(m_pos + Vector3(0, 17, -6));

	// ���Ԃ����ɂꑁ���Ȃ�
	if (m_fDangerRate >= 0.45f)
	{
		m_pBomEf->Update();
		m_pBomAddEf->Update();

		// ���񂾂�傫���Ȃ�
		m_pBomEf->AddAdd3DScale(0.1f);
		m_pBomAddEf->AddAdd3DScale(0.1f);


	}
	if (m_fDangerRate >= 0.75f)
	{
		m_pBomEf->Update();
		m_pBomAddEf->Update();

		// ���񂾂�傫���Ȃ�
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

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (!m_bActive) return;


	// �O�����ɐi��
	if (m_ActionType == SKILL_ACTION_TYPE::LAND)
	{
		// ���肪�o���u�ԓ���
		if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] != FRAME_STATE::ACTIVE)
		{
			// �ǔ�
			const float fAbjust = 0.5f;
			if (m_pPlayer->GetTargetPlayer()->GetPos().x + fAbjust > m_pos.x)
			{
				m_move.x += 0.04f;
			}
			if (m_pPlayer->GetTargetPlayer()->GetPos().x - fAbjust < m_pos.x)
			{
				m_move.x -= 0.04f;
			}

			// �������G�t�F�N�g
			if (m_CurrentActionFrame % 8 == 0)
			{
				ParticleManager::EffectRunSmoke(m_pos, (m_dir == DIR::LEFT));
			}
		}
	}

	// ���C
	Math::Clamp(m_move.x, -2.7f, 2.7f);
	
	// �ړ��l���Z
	m_pos.x += m_move.x;	
	if (abs(m_move.x) > 0.25f)
	{
		m_move.x *= 0.985f;
	}




}

void Stand::Attakai::Render(tdnShader * shader, char * name)
{
	Stand::Base::Render(shader, name);

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (m_bActive == false && m_fScale <= 0.0f) return;
	
	// �A�N�e�B�u(�����j������`�悵�Ȃ�
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] != FRAME_STATE::ACTIVE)
	{
		// ���e
		m_pBomEf->Render3D();
		m_pBomAddEf->Render3D();
	}

	tdnText::Draw(100, 100, 0xffff00ff, "%f", m_fDangerRate);

}

void Stand::Attakai::Action(SKILL_ACTION_TYPE type)
{

	// �Ƃ肠�����A�v���C���[�Ɠ������W
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.675f);
	if (m_pPlayer->GetDir() == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 11.0f;

	// (TODO)2/4 ���͂����Ń��[�V������ݒ�
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
		// �����Ēn�ʂɏ���
		m_pos.y = 0.0f;

		// ����̂�����W��
		m_pos.x = m_pPlayer->GetTargetPlayer()->GetPos().x - m_pPlayer->GetTargetPlayer()->GetDirVecX() * 5;

	}
	if (type == SKILL_ACTION_TYPE::AERIAL)
	{
		// �n�ʂɏ���
		m_pos.y = 0.0f;

	}
	m_pObj->SetPos(m_pos);
	m_pObj->Update();


	// ���N���X�̃A�N�V����
	// �󒆂͂��ׂĂ��Ⴊ�݋Z��
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

	
	// �A�N�e�B�u���ɂȂ�t���[��
	for (int i = 0;; i++)
	{
		if (m_ActionFrameList[(int)m_ActionType][i] == FRAME_STATE::ACTIVE)
		{
			m_iActiveStartFrame = i;
			break;
		}
	}

	// �ړ��ʏ�����
	m_move = VECTOR_ZERO;
 

	// �G�t�F�N�g
	m_pBomEf   ->Action(m_pos + Vector3(0, 17, -6));
	m_pBomAddEf->Action(m_pos + Vector3(0, 17, -6));

	m_pBomEf->SetAdd3DScale(0);
	m_pBomAddEf->SetAdd3DScale(0);

}
