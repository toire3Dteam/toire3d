#include "TDNLIB.h"
#include "Stand.h"
#include "../Sound/SoundManager.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseEntity\Message\MessageDispatcher.h"

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
	// �X�^���h�Q�[�W�X�V
	if(bControl)StandGageUpdate();

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (!m_bActive) return;

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
	}

	// �t���[���Ō�܂ōĐ�������
	if (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame++] == FRAME_STATE::END)
	{
		// ���A�N�e�B�u���I�t(�{���������甼�����t�F�[�h�A�E�g���g��)
		m_bActive = false;
	}

	// 3D�I�u�W�F�N�g�̍X�V
	m_pObj->Animation();
	m_pObj->SetAngle(DIR_ANGLE[(int)m_dir]);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
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

}

void Stand::Base::Render(tdnShader* shader, char* name)
{

	// �A�N�e�B�u����Ȃ�������o�Ă������I�I
	if (!m_bActive) return;

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
}

/******************************/
//		���R�C
/******************************/
Stand::Mokoi::Mokoi(BasePlayer *pPlayer) :Base(pPlayer)
{
	// ���R�C�̎���
	m_pObj = new iex3DObj("DATA/CHR/Stand/Mokoi/mokoi.IEM");
	
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->damage = 360;
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->height = 18;
	m_pAttackData[(int)SKILL_ACTION_TYPE::LAND]->pCollisionShape->pos.Set(7.5f, 17, 0);

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

void Stand::Mokoi::Action(SKILL_ACTION_TYPE type)
{
	// ���N���X�̃A�N�V����	�����͑S���n��̍U�������ɂ���
	Base::Action(SKILL_ACTION_TYPE::LAND);

	// �Ƃ肠�����A�v���C���[�Ɠ������W
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
//		�}�[��
/******************************/
Stand::Maya::Maya(BasePlayer *pPlayer) :Base(pPlayer)
{
	// �}�[���̎���
	m_pObj = new iex3DObj("DATA/CHR/Stand/Maya/maya.IEM");

	// �ʂ̎���
	m_pBullet = new  MayaShotEffect();/*iexMesh("DATA/UVeffect/Airou/AirouBoost.IMO");*/

	// �A�N�V�����t���[�����[�h����
	LoadActionFrameList("DATA/CHR/Stand/Maya/FrameList.txt");

	/*****************************/
	// �}�[���̊�{�X�e�[�^�X
	m_standStockMAX = 1;
	m_standGageMAX = 60*4;

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
	m_pAttackData[(int)SKILL_ACTION_TYPE::SQUAT]->damage = 800;
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
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
	m_pAttackData[(int)SKILL_ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].HitRecoveryFrame = 45;
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
	delete m_pBullet;
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
		if (m_pPlayer->GetDir() == DIR::LEFT) ShotVec.x *= -1;

		// �}�Y���G�t�F�N�g�����I
		m_pPlayer->AddEffectAction(m_pos + Vector3(0, 4, -2), EFFECT_TYPE::MUZZLE_FLASH, ShotVec);


		// �ʂ̏����i�[
		Shot::Base *pNewShot(new Shot::Maya(m_pPlayer, m_pAttackData[(int)m_ActionType], m_pBullet, m_pos + Vector3(0, 5, 0), ShotVec));
		MsgMgr->Dispatch(0, m_pPlayer->GetID(), ENTITY_ID::SHOT_MGR, MESSAGE_TYPE::ADD_SHOT, pNewShot);
	}
}

void Stand::Maya::Action(SKILL_ACTION_TYPE type)
{
	// ���N���X�̃A�N�V����(�S�����ʌn)
	Base::Action(type);

	// �Ƃ肠�����A�v���C���[�Ɠ������W
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 1.0f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
}