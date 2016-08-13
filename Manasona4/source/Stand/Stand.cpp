#include "TDNLIB.h"
#include "Stand.h"
#include "../Sound/SoundManager.h"

// �U���t���[�����ǂݍ���
void Stand::Base::LoadActionFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�A�N�V�����t���[���̃e�L�X�g�����ĂȂ�");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	FOR((int)ACTION_TYPE::MAX)
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
	// �X�^���h�Q�[�W�X�V
	StandGageUpdate();

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

			LPCSTR SE_ID = m_pAttackData[(int)m_ActionType]->WhiffSE;
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

/******************************/
//		���R�C
/******************************/
Stand::Mokoi::Mokoi(BasePlayer *pPlayer) :Base(pPlayer)
{
	// ���R�C�̎���
	m_pObj = new iex3DObj("DATA/CHR/mokoi/mokoi.IEM");
	//m_pObj->SetTexture(tdnTexture::Load("DATA/CHR/Airou/tex_airou4.png"), 0);	// �_�[�N�A�C���[
	
	// �A�N�V�����t���[�����[�h����
	LoadActionFrameList("DATA/CHR/Mokoi/FrameList.txt");

	/*****************************/
	// ���R�C�̊�{�X�e�[�^�X
	m_standStockMAX = 2;
	m_standGageMAX = 120;


	//==============================================================================================================
	//	�n��j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)ACTION_TYPE::LAND] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::LAND]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::LAND]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::LAND]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::LAND]->HitSE = "�ʏ�3�i�q�b�g";
	m_pAttackData[(int)ACTION_TYPE::LAND]->WhiffSE = "��U��1";
	m_pAttackData[(int)ACTION_TYPE::LAND]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::LAND]->WhiffEffectType = EFFECT_TYPE::UPPER;
	m_pAttackData[(int)ACTION_TYPE::LAND]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::LAND]->ShakeCameraInfo.Set(.75f, 4);

	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(.75f, 2.3f);
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 8;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 8;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 60;
	m_pAttackData[(int)ACTION_TYPE::LAND]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// ����`��
	m_pAttackData[(int)ACTION_TYPE::LAND]->pCollisionShape->width = 14;
	m_pAttackData[(int)ACTION_TYPE::LAND]->pCollisionShape->height = 18;
	m_pAttackData[(int)ACTION_TYPE::LAND]->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	���Ⴊ��
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)ACTION_TYPE::SQUAT] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->HitSE = "�ʏ�3�i�q�b�g";
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->WhiffSE = "��U��1";
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->ShakeCameraInfo.Set(.75f, 4);

	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// ����`��
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pCollisionShape->width = 8;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pCollisionShape->height = 12;
	m_pAttackData[(int)ACTION_TYPE::SQUAT]->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	�󒆃j���[�g����
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)ACTION_TYPE::AERIAL] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->HitSE = "�ʏ�3�i�q�b�g";
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->WhiffSE = "��U��1";
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->ShakeCameraInfo.Set(.75f, 4);

	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// ����`��
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pCollisionShape->width = 8;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pCollisionShape->height = 12;
	m_pAttackData[(int)ACTION_TYPE::AERIAL]->pCollisionShape->pos.Set(2.5f, 17, 0);

	//==============================================================================================================
	//	�󒆉�
	// �n��q�b�g���󒆃q�b�g�����ʂ̏��
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP] = new AttackData;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->HitScore = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->damage = 360;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pierceLV = 0;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->HitSE = "�ʏ�3�i�q�b�g";
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->WhiffSE = "��U��1";
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->HitEffectType = EFFECT_TYPE::DAMAGE;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->bAntiAir = true;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->ShakeCameraInfo.Set(.75f, 4);

	// �n��q�b�g�Ƌ󒆃q�b�g�ŋ������ς�����
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].bBeInvincible = true;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].bBeInvincible = false;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].FlyVector.Set(1.0f, .0f);
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].FlyVector.Set(.5f, 2.1f);
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].hitStopFlame = 4;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::LAND].recoveryFlame = 20;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->places[(int)AttackData::HIT_PLACE::AERIAL].recoveryFlame = 60;
	// ����`��
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pCollisionShape->width = 8;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pCollisionShape->height = 12;
	m_pAttackData[(int)ACTION_TYPE::AERIALDROP]->pCollisionShape->pos.Set(2.5f, 17, 0);

	// ���蔭�������u�Ԃɋ�U��SE���Đ����Ă݂�
	FOR((int)ACTION_TYPE::MAX)
	{
		if (!m_pAttackData[i]) continue;	// new����ĂȂ�������X���[
		for (int place = 0; place < (int)AttackData::HIT_PLACE::MAX; place++) MyAssert((m_pAttackData[i]->places[place].FlyVector.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		MyAssert((m_pAttackData[i]->pCollisionShape->pos.x >= 0), "X�̒l��-�ɂȂ邱�Ƃ͐�΂Ȃ�");
		m_pAttackData[i]->bHit = false;
		for (int frame = 0;; frame++)if (m_ActionFrameList[i][frame] == FRAME_STATE::ACTIVE){ m_pAttackData[i]->WhiffDelayFrame = frame; break; }
	}

	// �A�b�p�[�̋�U��t���[������
	m_pAttackData[(int)ACTION_TYPE::LAND]->WhiffDelayFrame -= 8;
}

void Stand::Mokoi::Action(Stand::ACTION_TYPE type)
{
	// �X�g�b�N������
	m_standStock--;

	m_ActionType = type;		// �Z�̃^�C�v�ݒ�
	m_CurrentActionFrame = 0;	// �t���[�����Z�b�g

	m_pAttackData[(int)m_ActionType]->bHit = false;	// �q�b�g�t���O����

	// �Ƃ肠�����A�A�N�e�B�u�ɂ���
	m_bActive = true;

	// �v���C���[�̌��������炤
	m_dir = m_pPlayer->GetDir();

	// �Ƃ肠�����A�v���C���[�Ɠ������W
	m_pos = m_pPlayer->GetPos();
	Vector3 v(0.5f, 0.0f, 0.5f);
	if (m_dir == DIR::LEFT)v.x *= -1;
	//v.z *= 1.;
	m_pos += v * 5.0f;
	m_pObj->SetMotion(0);
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// �A�b�p�[�G�t�F�N�g�I
	//if(type == ACTION_TYPE::LAND) m_pPlayer->AddEffectAction(m_pPlayer->GetPos(), EFFECT_TYPE::UPPER);
}
