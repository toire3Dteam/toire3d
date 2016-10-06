#include "TDNLIB.h"
#include "BasePlayer.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "AI\AI.h"
#include "../Effect/Particle.h"
#include "../system/System.h"
#include "../PointLight/PointLight.h"

//_/_/_/_/_/_/__/_/__/_/__/_/__
// �萔
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// �ړ����I����đҋ@�ɖ߂�ړ��l���C��
const float BasePlayer::c_FRONT_BRAKE_LINE = .55f;	// ���蒆�ɑ���𗣂��ău���[�L�Ɉڍs����܂ł̈ړ��l���C��
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 2.2f;

const int BasePlayer::c_RECOVERY_FLAME = 8;			// ���J�o���[�X�e�[�g�ɂ��鎞��

const int BasePlayer::c_OVERDRIVE_MAX_GAGE = 100;	// �o���Q�[�W�̍ő�l
const int BasePlayer::c_OVERDRIVE_MAX_TIME = 360;	// �o�����؂��܂ł̎���

const int BasePlayer::c_THROW_ESCAPE_FRAME = 8;	// �����ʂ��̗P�\�t���[��
const int BasePlayer::c_THROW_MISS_FRAME = 30;	// �����O�����X�̃t���[��(�S�L�������ʂ��낤�Ƃ����l��)
const int BasePlayer::c_THROW_RELEASE_FRAME = 15;	// ���������ŁA�p�V���ĂȂ��Ă�Ԃ̃t���[��(������S�L�������ʂ��낤)
// �A�N�V�����t���[�����ǂݍ���
void BasePlayer::LoadAttackFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�A�^�b�N�t���[���̃e�L�X�g�����ĂȂ�");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	FOR((int)BASE_ACTION_STATE::END)
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
				MyAssert(count < 256, "�A�N�V�����t���[���̍��v��255�ȉ��ɂ��Ă�������");
			}
		}
		// �I�[
		m_ActionFrameList[i][count] = FRAME_STATE::END;
	}
}

BasePlayer::BasePlayer(int deviceID, TEAM team, bool bAI) :m_bAI(bAI), m_team(team), BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO), m_bLand(false), m_bAerialJump(true), m_ActionState(BASE_ACTION_STATE::NO_ACTION),
m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentActionFrame(0), m_RecoveryFlame(0), m_bEscape(false), m_score(0), m_CollectScore(0), m_pAI(nullptr),
m_recoveryCount(0), m_HitStopFrame(0),// �������o�O�̌����@�Ђ��Ƃ��g�b�v�������̂��Y��B
m_InvincibleColRate(0), m_InvincibleColRateFlame(0), m_bInvincibleColRateUpFlag(false),
m_OverDriveGage(0), m_bOverDrive(false), m_OverDriveFrame(0), m_OverDriveType(OVERDRIVE_TYPE::BURST),
m_bMoveUpdate(true), m_bThrowSuccess(false),
m_bGuard(false), m_GuardFollowFrame(0),
m_ThrowPlayerID(ENTITY_ID::ID_ERROR),
m_MaxHP(0), m_HP(0),
m_bGameTimerStopFlag(false), m_HeavehoStopTimer(0)
{
	// �Ƃ肠�����A���R�C����
	m_pStand = new Stand::Mokoi(this);


	// �f�t�H���g�ݒ�
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// �G�t�F�N�g�}�l�[�W���[
	m_PanelEffectMGR = new PanelEffectManager();
	m_UVEffectMGR	 = new UVEffectManager();

	// ������
	memset(m_InputList, 0, sizeof(m_InputList));
	ZeroMemory(&m_jump, sizeof(Jump));
	ZeroMemory(&m_RushAttack, sizeof(RushAttack));
	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<BasePlayer>(this);
	m_pStateMachine->SetGlobalState(BasePlayerState::Global::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance());
	m_pStateMachine->SetPreviousState(BasePlayerState::Wait::GetInstance());

	// AI���ǂ����Ł@����
	//if(bAI) m_pAI = new AI(m_deviceID, this);
	m_pAI = nullptr;

	// ID�ō��W�̕���
	switch (deviceID)
	{
	case 0:
		m_pos.Set(-50, 20, 0);
		break;

	case 1:
		m_pos.Set(-25, 30, 0);
		break;

	case 2:
		m_pos.Set(25, 40, 0);
		break;

	case 3:
		m_pos.Set(50, 50, 0);
		break;
	}

	m_angleY = DIR_ANGLE[(int)m_dir];

	// �����Ă钆�ɋ����Ă邩����Ə�����
	m_RecoveryDamageCount.clear();


	m_pComboUI = new ComboUI(&m_RecoveryFlame);

}

void BasePlayer::InitAI()
{
	// AI�t���O���AAI��new����ĂȂ�������
	//if (m_pAI) return;
	if (m_bAI) m_pAI = new AI(m_deviceID, this);
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_pStand);
	SAFE_DELETE(m_PanelEffectMGR);
	SAFE_DELETE(m_UVEffectMGR);
	SAFE_DELETE(m_pAI);
	SAFE_DELETE(m_pComboUI);

	delete m_pHitSquare;
}

void BasePlayer::Update(bool bControl)
{
	// �o���̍X�V
	OverDriveUpdate();

	// �K�[�h�G�t�F�N�g�X�V
	GuardEffectUpdate();

	// 1more�o�����Ă�����X�^���h�̓������~�߂�
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// �X�^���h�X�V
		m_pStand->Update();
	}

	// ���G���Ԃ̍X�V
	if (KeyBoardTRG(KB_H))
	{
		SetInvincible(240, 1);
	}
	InvincibleUpdate();


	// ���d�����Ԃ̃f�N�������g
	if (m_RecoveryFlame > 0)
		m_RecoveryFlame--;

	// ���q�b�g�X�g�b�v��
	if (m_HitStopFrame > 0)
	{
		m_HitStopFrame--;
	}
	// �q�b�g�X�g�b�v�������܂œ����Ȃ�
	else
	{
		// �A�N�V�����t���[���̍X�V
		if (isFrameAction())
		{
			// �U���t���[���Ȃ�
			if (m_ActionDatas[(int)m_ActionState].isAttackData())
			{
				// ���̌o�ߎ��Ԃƃf�B���C�t���[���ɂȂ�����
				if (m_ActionDatas[(int)m_ActionState].pAttackData->WhiffDelayFrame == m_CurrentActionFrame)
				{
					// (A���)�����ōU�����肪���������u�Ԃ�����Ă��Ă�
					
					// �U��G�t�F�N�g�����i���j�I
					AddEffectAction(m_pos + Vector3(0, 5, -3) , m_ActionDatas[(int)m_ActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_ActionState].pAttackData->WhiffSE;
					// ��U��SE�����Ă���
					if (SE_ID)
					{
						// �f�B���C�t���[���o�߂�����Đ�
						se->Play((LPSTR)SE_ID);
					}
				}


				//// �����n���E�����̊Ԃɂ��̍U�����u�΋�v���ʂ������Ă����ꍇ
				//// �΋�t���O��ON�ɂ��邻��ȊO��OFF��
				//if ((m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE ||
				//	m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::START)
				//	&& m_ActionDatas[(int)m_ActionState].pAttackData->bAntiAir == true	)
				//{
				//	m_move.y += 0.35f;
				//}
				//else
				//{

				//}

			}
			//else
			//{
			//	// �悤�킩���̂ł������ł��ꉞ�΋�OFF�ɂ��Ƃ�

			//}

			
			// ActionFlame���t���[����
			//if (m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE || 
			//	m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::START)
			//{
			//	// �U���t���[���Ȃ�(���ꂪ�Ȃ�������U���̃f�[�^���Ȃ����̂����Ă��܂�)
			//	if (m_ActionDatas[(int)m_ActionState].isAttackData())
			//	{
			//		// ���ǂ̋Ƃ̃X�e�[�g���킩��
			//		if (m_ActionDatas[(int)m_ActionState].pAttackData->bAntiAir == true)
			//		{
			//			m_move.y += 0.35f;
			//		}
			//	}
			//	
			//}

			// ���[�V�����̃t���[�����X�V�I
			m_CurrentActionFrame++;

			// �t���[���Ō�܂ōĐ�������
			if (m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame] == FRAME_STATE::END)
			{
				// ���A�N�V�����X�e�[�g����
				m_ActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}

		// ���͎�t
		if (bControl)
		{
			if (m_bAI)	AIControl();
			else		Control();
		}

		// ���͎�t��ɃX�e�[�g�}�V���X�V
		m_pStateMachine->Update();

		//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
		//else m_move.x = 0;

		// �������@�Ȃ��̃o�O�́@�������Ƃ����ĂȂ����炨����

		// �����̐���
		Move();

		// ���b�V���̍X�V
		m_pObj->Animation();
		m_pObj->SetAngle(m_angleY);	// ���E�̃A���O���̃Z�b�g
		m_pObj->SetPos(m_pos);
		m_pObj->Update();
	}

	// �A���O����ԏ���
	const float AnglePercentage = (m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance())) ? .9f : .65f;
	m_angleY = m_angleY * AnglePercentage + DIR_ANGLE[(int)m_dir] * (1 - AnglePercentage);

	// �G�t�F�N�g�}�l�[�W���[�X�V (�q�b�g�X�g�b�v����)
	m_PanelEffectMGR->Update();
	m_UVEffectMGR->Update();
	
	// �R���{UI
	m_pComboUI->Update();

}

void BasePlayer::Move() 
{
	// �ړ��ʍX�V
	if (
		//m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false
		m_bMoveUpdate
		)
	{	// �y���\�i����������Ȃ�������ړ�
		m_move.y -= c_GRAVITY;
		//if (m_move.y <= -3.0f) { m_move.y = -3.0f; } // �����鑬�x��}��
		m_move.y = max(-2.75f, m_move.y);// �����鑬�x��}��

		// ��C��R(x�̒l�����X�Ɍ��炵�Ă���)
		if (m_bLand)
		{
			//if (m_move.x > 0)		m_move.x = max(m_move.x - .055f, 0);
			//else if (m_move.x < 0)	m_move.x = min(m_move.x + .055f, 0);
			m_move.x *= 0.92f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
		}
		else
		{
			//if (m_move.x > 0)		m_move.x = max(m_move.x - .035f, 0);
			//else if (m_move.x < 0)	m_move.x = min(m_move.x + .035f, 0);
			m_move.x *= 0.98f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
		}

		// ���E��Move�l
		//m_move.x = Math::Clamp(m_move.x, -m_maxSpeed, m_maxSpeed);
	}

	// �y���\�i�������ړ�
	else
	{
		m_move *= 0.8f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
	}

}

// ���G�̐���
void BasePlayer::InvincibleUpdate()
{
	// ���������̂����O
	enum 
	{
		FLASH_SPEED = 15
	};

	// ���G���Ԃ̍X�V
	if (m_InvincibleTime > 0)
	{
		//m_InvincibleLV = (--m_InvincibleTime <= 0) ? 0 : m_InvincibleLV;
		m_InvincibleTime--;

		
		m_InvincibleColRateFlame++;
		m_InvincibleColRateFlame = m_InvincibleColRateFlame % FLASH_SPEED;
		
		// �グ�����̃t���O�؂�ւ�
		if (m_InvincibleColRateFlame == 0)
		{
			m_bInvincibleColRateUpFlag = (m_bInvincibleColRateUpFlag == false) ? true : false;
		}

		// �グ�t���O��ON�Ȃ炾�񂾂񔒂�
		if (m_bInvincibleColRateUpFlag == false)
		{
			m_InvincibleColRate = ((float)(m_InvincibleColRateFlame) / FLASH_SPEED);
		}
		else
		{
			m_InvincibleColRate = 1.0f - ((float)(m_InvincibleColRateFlame) / FLASH_SPEED);
		}
	

		// ���@�F�̒��� (�����̓Z���X�̖��)
		m_InvincibleColRate *= 0.25f;	// ������Ɣ���
		m_InvincibleColRate += 0.1f;    // ���Ǔ_�ł��Ȃ���Ԃł������ۂ�

	}
	else
	{
		// �^�C����0�ȉ��ɒB�����炨���܂��@������
		m_InvincibleLV = 0;
		m_InvincibleTime = 0;

		m_InvincibleColRate = 0.0f;
		m_InvincibleColRateFlame = 0;
		m_bInvincibleColRateUpFlag = false;
		
	}

}

// �o���̐���
void BasePlayer::OverDriveUpdate()
{
	// �o�����Ă��邩
	if (m_bOverDrive == false)
	{
		m_OverDriveGage++;
		// �Q�[�W���ő�܂ŗ��܂�����
		if (m_OverDriveGage >= c_OVERDRIVE_MAX_GAGE)
		{
			m_OverDriveGage = c_OVERDRIVE_MAX_GAGE;
		}

	}
	else // �o����
	{
		// �o���̎�ނɉ�����
		switch (m_OverDriveType)
		{
		case OVERDRIVE_TYPE::ONEMORE:

			// �I�[���̃p�[�e�B�N��
			ParticleManager::EffectOverDrive(Vector3(GetPos().x, GetPos().y, -0.5f));	// �኱��O

			break;
		case OVERDRIVE_TYPE::BURST:

			break;
		default:
			break;
		}

		m_OverDriveFrame--;
		if (m_OverDriveFrame <= 0) // �o�����Ԃ�0�ɂȂ�����o���I���
		{
			m_OverDriveFrame = 0;
			m_bOverDrive = false;
		}
	}

}

void BasePlayer::ActionOverDrive(OVERDRIVE_TYPE type)
{
	// �o���̎��
	m_OverDriveType = type;

	// �A�N�V����
	m_bOverDrive = true;
	
	m_OverDriveGage = 0; //���
	m_OverDriveFrame = c_OVERDRIVE_MAX_TIME;

	// �W�����v�̌���������
	m_bAerialJump = true;
}

void BasePlayer::Control()
{
	// �������L�[����
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_InputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_deviceID);
	}

}

void BasePlayer::AIControl()
{
	//static int frame(0);
	//if (++frame > 90)
	//{
	//	frame = 0;
	//	m_InputList[(int)PLAYER_INPUT::C] = 3;
	//	//m_InputList[(int)PLAYER_INPUT::LEFT] = 3;
	//}

	////static int frame2(0);
	////if (++frame2 > 65)
	////{
	////	frame2 = 0;
	////	m_InputList[(int)PLAYER_INPUT::B] = 3;
	////	m_InputList[(int)PLAYER_INPUT::DOWN] = 3;
	////}
	//m_InputList[(int)PLAYER_INPUT::LEFT] = 1;	// A��������


	// AI�̃X�e�[�g�}�V�����X�V
	m_pAI->Update();

}

void BasePlayer::UpdatePos()
{
	// (TODO)�q�b�g�X�g�b�v���ĂȂ�������
	if (m_HitStopFrame <= 0)
	{
		//if (m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false)// �y���\�i����������Ȃ�������ړ�
		{
			// ���W�X�V
			m_pos += m_move;
		}
	}
}

void BasePlayer::Render()
{
	// �X�^���h�`��
	m_pStand->Render(shaderM, "Persona");

	// ���G�̔��F�`�J�`�J���[�g�𑗂�
	shaderM->SetValue("g_InvincibleColRate", m_InvincibleColRate);
	m_pObj->Render(shaderM, "PlayerToon");

	// ���G�Ȃ���Z�ŏd�˂ĕ`��
	if (m_InvincibleTime > 0) m_pObj->Render(RS::ADD);

	// �����Ō��݂̃X�e�[�g�}�V���̏�Ԃ��m�F
	if (m_deviceID == 0)
	{
		m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��
		
		if (m_pAI != nullptr)
		{
			 m_pAI->GetFSM()->Render();// AI�X�e�[�g�}�V���ł̕`��
		}
	}


	// �G�t�F�N�g�}�l�[�W���[�`��
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();

#ifdef _DEBUG
	// ����̕`��
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), m_pHitSquare, sizeof(CollisionShape::Square));
	square.pos += m_pos;

	Vector3 wv[3];	// ���[���h�o�[�e�b�N�X
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// �X�N���[���o�[�e�b�N�X
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* �U������̕`�� */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
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

	// �f�o�b�O
	tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "�X�^���h�Q�[�W : %d", m_pStand->GetStandGage());
	tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "�X�^���h�X�g�b�N: %d", m_pStand->GetStandStock());
	
	tdnText::Draw(32 + m_deviceID * 250, 430, 0xff00ff80, "OD �Q�[�W: %d", m_OverDriveGage);
	tdnText::Draw(32 + m_deviceID * 250, 460, 0xffff8000, "OD�c�莞��: %d", m_OverDriveFrame);


	Vector2 pos2d = Math::WorldToScreen(m_pos);
	
	//if (m_deviceID == 1 || m_deviceID == 2)
	{
		tdnText::Draw((int)pos2d.x, (int)pos2d.y - 150, 0xff00ff80, "C_Score : %d", m_CollectScore);
		tdnText::Draw((int)pos2d.x, (int)pos2d.y-100, 0xffff8000, "%dP!!", m_deviceID);

	}


}

void BasePlayer::Render(tdnShader* shader, char* name)
{
	// �X�^���h�`��
	m_pStand->Render(shader, name);

	// ���G�̔��F�𑗂�
	shader->SetValue("g_InvincibleColRate", m_InvincibleColRate);
	// �f�o�b�O
	tdnText::Draw(332 + m_deviceID * 350, 260, 0xff00ff80, "m_InvColRate : %.2f", m_InvincibleColRate);

	m_pObj->Render(shader, name);
	// ���G�Ȃ���Z�ŏd�˂ĕ`��
	if (m_InvincibleTime > 0) m_pObj->Render(RS::ADD);

	if (m_deviceID == 3)
	{
		m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��
		m_pAI->GetFSM()->Render();// AI�X�e�[�g�}�V���ł̕`��
	}
	

	// �G�t�F�N�g�}�l�[�W���[�`��
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();

#ifdef _DEBUG
	// ����̕`��
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), m_pHitSquare, sizeof(CollisionShape::Square));
	square.pos += m_pos;

	Vector3 wv[3];	// ���[���h�o�[�e�b�N�X
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// �X�N���[���o�[�e�b�N�X
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* �U������̕`�� */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
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

	// �f�o�b�O
	tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "�X�^���h�Q�[�W : %d", m_pStand->GetStandGage());
	tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "�X�^���h�X�g�b�N: %d", m_pStand->GetStandStock());

}

void BasePlayer::RenderDeferred()
{
	// �X�^���h�`��
	m_pStand->Render(shaderM, "G_Buffer");

	m_pObj->Render(shaderM,"G_Buffer");
}

void BasePlayer::RenderUI()
{
	// �R���{UI
	m_pComboUI->Render(100 + (m_deviceID * 950), 200);

}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

// ���G�t�F�N�g�������@�i�S�ẴG�t�F�N�g�̔����ꏊ�j
void BasePlayer::AddEffectAction(Vector3 pos, EFFECT_TYPE effectType)
{
	// ���̃G�t�F�N�g���d�˂邩�͂����ŉ������߂�B

    // �L�����N�^�[�����ɂ�����
    float diaAngle = (m_dir == DIR::RIGHT) ? 0.0f : 3.14f;

	// �W�܂�G�t�F�N�g�̏ꏊ�␳
	Vector3 convAddPos = Vector3(0, -10, 0);

	switch (effectType)
	{
	case EFFECT_TYPE::DAMAGE:
	{
		//m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURN);
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE);
		PointLightMgr->AddPointLight(pos + Vector3(0, 5, 0), Vector3(1.0f, 0.4f, 0.0f), 20, 4, 20, 4, 15);// �|�C���g���C�g�G�t�F�N�g�I
		Vector3 FlyVector(m_move);
		FlyVector.Normalize();
		ParticleManager::EffectHit(pos, FlyVector);
	}
		break;
	case EFFECT_TYPE::WHIFF:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::INEFFECT_MINI);
		
        m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_IMPACT,
            1, 2, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));

	
		break;
	case EFFECT_TYPE::RECOVERY:
		m_PanelEffectMGR->AddEffect(pos + Vector3(0, 5, 0), PANEL_EFFECT_TYPE::ClEAR);


		break;
	case EFFECT_TYPE::PERSONA:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PERSONA,
			1, 2.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectPersonaTrigger(pos);
		PointLightMgr->AddPointLight(pos + Vector3(0, 3, 0) , Vector3(0, .4f, 2.0f), 50, 4, 40, 10, 30);// �|�C���g���C�g�G�t�F�N�g�I

		break;
	case EFFECT_TYPE::DROP_IMPACT:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::IMPACT,
			1.0f, 1.35f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::SHOCK_WAVE,
			1.0f, 1.55f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		break;
	case EFFECT_TYPE::UPPER:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::UPPER,
			1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));

		break;

	case EFFECT_TYPE::FINISH_HIT:
		ParticleManager::EffectFinish(pos);

		break;
	case EFFECT_TYPE::BURST:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::ONEMORE_BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST_START:
	
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST_PREV);

	//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::BURST_BALL, 0.9, 0.75
	//		, Vector3(0, 0, 0), Vector3(0, 0, 0));

	 m_UVEffectMGR->AddEffect(pos+convAddPos, UV_EFFECT_TYPE::CONV, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0));
	 m_UVEffectMGR->AddEffect(pos + convAddPos+Vector3(0,-5,0), UV_EFFECT_TYPE::CONV2, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0),4);
	 m_UVEffectMGR->AddEffect(pos + convAddPos + Vector3(-0.5, -7, 0), UV_EFFECT_TYPE::CONV3, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0),8);
	 m_UVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV4, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0),12);
		break;
	case EFFECT_TYPE::RUN:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_pos, (m_dir != DIR::LEFT));

		break;
	case EFFECT_TYPE::GUARD_BREAK:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::GLASS);

		break;
	default:
		assert(0);	// ����ȃG�t�F�N�g�͑��݂��Ȃ� AddEffectAction()
		break;
	}

}

// �K�[�hEffect
void BasePlayer::GuardEffectAction()
{
	// �K�[�h�G�t�F�N�g����
	m_UVEffectMGR->AddEffectRoop(GetCenterPos(), UV_EFFECT_TYPE::GUARD);

	m_PanelEffectMGR->AddEffect(GetCenterPos()+Vector3(0,0,-5), PANEL_EFFECT_TYPE::GUARD);

}

void BasePlayer::GuardEffectStop()
{
	// �K�[�h�G�t�F�N�g�I��
	m_UVEffectMGR->StopEffectRoop(UV_EFFECT_TYPE::GUARD);
}

void BasePlayer::GuardEffectUpdate()
{
	// �L�����N�^�[�ɒǏ]
	// �K�[�h�G�t�F�N�g�X�V
	m_UVEffectMGR->GetBaseUVEffect(UV_EFFECT_TYPE::GUARD)->SetPos(GetCenterPos());
}