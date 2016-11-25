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
#include "../DeferredEx/DeferredEx.h"
#include "../Data/SelectData.h"

//_/_/_/_/_/_/__/_/__/_/__/_/__
// �萔
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// �ړ����I����đҋ@�ɖ߂�ړ��l���C��
const int	BasePlayer::c_RUSH_AHEAD_START = 5;		// ���b�V�����̐�s���͎�t�J�n�t���[��
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 1.9f;

const int BasePlayer::c_RECOVERY_FLAME = 32;			// ���J�o���[�X�e�[�g�ɂ��鎞��

const int BasePlayer::c_OVERDRIVE_MAX_GAGE = 100;	// �o���Q�[�W�̍ő�l
const int BasePlayer::c_OVERDRIVE_MAX_TIME = 420;	// �o�����؂��܂ł̎���

const int BasePlayer::c_THROW_ESCAPE_FRAME = 8;	// �����ʂ��̗P�\�t���[��
const int BasePlayer::c_THROW_MISS_FRAME = 30;	// �����O�����X�̃t���[��(�S�L�������ʂ��낤�Ƃ����l��)
const int BasePlayer::c_THROW_RELEASE_FRAME = 15;	// ���������ŁA�p�V���ĂȂ��Ă�Ԃ̃t���[��(������S�L�������ʂ��낤)
const int BasePlayer::c_WINNER_TIME = 180;
const float BasePlayer::c_GUARD_DISTANCE = 32.0f;

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

BasePlayer::BasePlayer(SIDE side, const SideData &data) :m_bAI(data.bAI), m_deviceID(data.iDeviceID), m_side(side), BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + (int)side)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_pHitSquare(new CollisionShape::Square), m_pDefaultObj(nullptr),
m_pObj(nullptr),
m_move(VECTOR_ZERO), m_bLand(false),m_bSquat(false), m_bAerialJump(true), m_bAerialDash(false),m_iAerialDashFrame(0), m_ActionState(BASE_ACTION_STATE::NO_ACTION),
m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentActionFrame(0), m_RecoveryFlame(0), m_bEscape(false), m_score(0), m_CollectScore(0), m_pAI(nullptr),
m_HitStopFrame(0),// �������o�O�̌����@�Ђ��Ƃ��g�b�v�������̂��Y��B
m_InvincibleColRate(0), m_InvincibleColRateFlame(0), m_bInvincibleColRateUpFlag(false),
m_OverDriveGage(0), m_bOverDrive(false), m_OverDriveFrame(0), m_OverDriveType(OVERDRIVE_TYPE::BURST),
m_bMoveUpdate(true),
m_MaxHP(0), m_HP(0),
m_bGameTimerStopFlag(false), m_HeavehoStopTimer(0), m_HeaveHoDriveOverFlowFrame(0),
m_WinNum(0), m_GuardState(GUARD_STATE::NO_GUARD),
m_pFacePic(nullptr), m_pTargetPlayer(nullptr), m_pSpeedLine(nullptr), m_SkillActionType(SKILL_ACTION_TYPE::MAX),
m_fOrangeColRate(0), m_fMagentaColRate(0),
m_pCutinPic(nullptr), m_pName("None")
{
	// �X�^���h
	switch (data.partner)
	{
	case PARTNER::MOKOI:
		m_pStand = new Stand::Mokoi(this);
		break;
	case PARTNER::MAYA:
		m_pStand = new Stand::Maya(this);
		break;
	default:
		assert(0);
		break;
	}


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
	switch ((int)m_side)
	{
	case (int)SIDE::LEFT:
		m_pos.Set(-20, 0, 0);
		m_TargetDir = DIR::RIGHT;
		break;

	case (int)SIDE::RIGHT:
		m_pos.Set(20, 0, 0);
		m_TargetDir = DIR::LEFT;
		break;
	}

	m_angleY = DIR_ANGLE[(int)m_dir];

	// �����Ă钆�ɋ����Ă邩����Ə�����
	m_RecoveryDamageCount.clear();


	//m_pComboUI = new ComboUI(&m_RecoveryFlame);

	// �R�}���h����������
	memset(m_CommandHistory, (int)PLAYER_COMMAND_BIT::NEUTRAL, sizeof(m_CommandHistory));


}

void BasePlayer::Reset()
{
	/* [int�^]�̏����� */

	/* 0�ŏ����� */
	m_InvincibleLV =
	m_InvincibleTime =
	m_CurrentActionFrame =
	m_RecoveryFlame =
	m_score =
	m_CollectScore =
	m_HitStopFrame =
	m_InvincibleColRateFlame =
	//m_OverDriveGage =			// ���E���h���p��
	m_OverDriveFrame =
	m_MaxHP = m_HP =
	m_HeavehoStopTimer =
	m_HeaveHoDriveOverFlowFrame =
	m_iAerialDashFrame =
	m_AheadCommand =
	0;

	/* [float�^]�̏����� */

	/* 0�ŏ����� */
	m_InvincibleColRate =
	0;


	/* [bool�^]�̏����� */

	/* true�ŏ������]�[�� */
	m_bAerialJump =
	m_bMoveUpdate =
	true;

	/* false�ŏ������]�[�� */
	m_bEscape =
	m_bInvincibleColRateUpFlag =
	m_bOverDrive =
	//m_bThrowSuccess =
	//m_bGuard =
	m_bGameTimerStopFlag =
	m_bAerialDash =
	m_bLand=
	m_bSquat=
	false;


	/* [�|�C���^�^]�̏�����(new���Ă��͋ɗ͂��̂܂�) */
	//m_pTargetPlayer = nullptr;
	m_pObj = m_pDefaultObj;
	if (m_pObj) m_pObj->SetMotion(m_MotionNumbers[(int)MOTION_TYPE::WAIT]);
	m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance());
	m_pStateMachine->SetPreviousState(BasePlayerState::Wait::GetInstance());

	/* [�񋓌^]�̏����� */
	m_ActionState = BASE_ACTION_STATE::NO_ACTION;
	m_OverDriveType = OVERDRIVE_TYPE::BURST;
	m_GuardState = GUARD_STATE::NO_GUARD;

	// ID�ō��W�̕���
	switch ((int)m_side)
	{
	case (int)SIDE::LEFT:
		m_pos.Set(-20, 0, 0);
		m_TargetDir = DIR::RIGHT;
		break;

	case (int)SIDE::RIGHT:
		m_pos.Set(20, 0, 0);
		m_TargetDir = DIR::LEFT;
		break;
	}


	m_move.Set(0, 0, 0);

	/* �C���v�b�g���X�g�̏����� */
	memset(m_InputList, 0, sizeof(m_InputList));
}

void BasePlayer::InitAI()
{
	// AI�t���O���AAI��new����ĂȂ�������
	//if (m_pAI) return;
	if (m_bAI) m_pAI = new AI(m_side, this);
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pDefaultObj);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_pStand);
	SAFE_DELETE(m_PanelEffectMGR);
	SAFE_DELETE(m_UVEffectMGR);
	SAFE_DELETE(m_pAI);
	//SAFE_DELETE(m_pComboUI);
	SAFE_DELETE(m_pHHDOFObj);
	SAFE_DELETE(m_pFacePic);
	SAFE_DELETE(m_pSpeedLine);
	SAFE_DELETE(m_pCutinPic);
	delete m_pHitSquare;
}

void BasePlayer::Update(PLAYER_UPDATE flag)
{
	// �f�o�b�O
	if (KeyBoardTRG(KB_G))
	{
		// 50��
		m_OverDriveGage += 50;
	}
	if (KeyBoardTRG(KB_J))
	{
		m_HP = m_MaxHP;
	}

	// �o���̍X�V
	OverDriveUpdate();

	// �K�[�h�G�t�F�N�g�X�V
	GuardEffectUpdate();

	// 1more�o�����Ă�����X�^���h�̓������~�߂�
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// �X�^���h�X�V
		m_pStand->Update((flag != PLAYER_UPDATE::CONTROL_NO));
	}

	// ���G���Ԃ̍X�V
	if (KeyBoardTRG(KB_H))
	{
		SetInvincible(40, 1);
	}
	InvincibleUpdate();

	// ���F�̍X�V
	ColorUpdate();

	// ���͎�t
	if (flag != PLAYER_UPDATE::CONTROL_NO)
	{
		if (m_bAI)	AIControl();
		else		Control();
	}
	else
	{
		memset(m_InputList, 0, sizeof(m_InputList));
		memset(m_CommandHistory, 0, sizeof(m_CommandHistory));
	}

	// ���q�b�g�X�g�b�v��
	if (m_HitStopFrame > 0)
	{
		m_HitStopFrame--;

		// ����s���͎�t(�������u�Ԃ����擾�ɕύX)
		AheadCommandUpdate();
	}
	// �q�b�g�X�g�b�v�������܂œ����Ȃ�
	else
	{
		// ���d�����Ԃ̃f�N�������g
		if (m_RecoveryFlame > 0)
			m_RecoveryFlame--;

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


		// �������̌�����ݒ�
		// ����if���ɓ����Ă��ȊO��������U�����
		if (!m_pStateMachine->isInState(*BasePlayerState::Skill::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Run::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::DownFall::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Jump::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialJump::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Fall::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialDash::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialBackDash::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::FinishAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialAttack::GetInstance())
			)
		{
			m_dir = m_TargetDir;

			// �A���O����ԏ���
			const float AnglePercentage = (m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance())) ? .9f : .65f;

			if (m_pStateMachine->isInState(*BasePlayerState::Wait::GetInstance()) || 
				m_pStateMachine->isInState(*BasePlayerState::Intro::GetInstance()))
			{
				m_angleY = DIR_ANGLE[(int)m_dir];
				m_angleY += (m_dir == DIR::LEFT) ? PI * -.1f : PI * .1f;
			}

			// �����Ă鎞�ȊO�Ȃ�␳
			else if (!m_pStateMachine->isInState(*BasePlayerState::Win::GetInstance()))
			{
				m_angleY = m_angleY * AnglePercentage + DIR_ANGLE[(int)m_dir] * (1 - AnglePercentage);
			}
		}

		// ���͎�t��ɃX�e�[�g�}�V���X�V
		if (flag != PLAYER_UPDATE::NO_FSM) m_pStateMachine->Update();

		//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
		//else m_move.x = 0;

		// �������@�Ȃ��̃o�O�́@�������Ƃ����ĂȂ����炨����

		// �����̐���
		MoveUpdate();

		// ���b�V���̍X�V
		m_pObj->Animation();
		m_pObj->SetAngle(m_angleY);	// ���E�̃A���O���̃Z�b�g
		m_pObj->SetPos(m_pos);
		m_pObj->Update();
	}


	// �G�t�F�N�g�}�l�[�W���[�X�V (�q�b�g�X�g�b�v����)
	m_PanelEffectMGR->Update();
	m_UVEffectMGR->Update();
	
	// �R���{UI
	//m_pComboUI->Update();

}

void BasePlayer::UpdateDrive()
{
	// �����n��
	m_bLand = true;

	// �X�s�[�h���C���̍X�V
	m_pSpeedLine->Update();

	// �o���̍X�V
	OverDriveUpdate();

	// �K�[�h�G�t�F�N�g�X�V
	GuardEffectUpdate();

	// 1more�o�����Ă�����X�^���h�̓������~�߂�
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// �X�^���h�X�V
		m_pStand->Update(false);
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
					AddEffectAction(m_pos + Vector3(0, 5, -3), m_ActionDatas[(int)m_ActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_ActionState].pAttackData->WhiffSE;
					// ��U��SE�����Ă���
					if (SE_ID)
					{
						// �f�B���C�t���[���o�߂�����Đ�
						se->Play((LPSTR)SE_ID);
					}
				}

			}

			// ���[�V�����̃t���[�����X�V�I
			m_CurrentActionFrame++;

			// �t���[���Ō�܂ōĐ�������
			if (m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame] == FRAME_STATE::END)
			{
				// ���A�N�V�����X�e�[�g����
				m_ActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}

		// ���͎�t��ɃX�e�[�g�}�V���X�V
		m_pStateMachine->Update();

		// �����̐���
		//Move();
		m_move.Set(0, 0, 0);

		// ���b�V���̍X�V
		m_pObj->Animation();
		m_pObj->SetAngle(m_angleY);	// (TODO)���͌Œ�
		m_pObj->SetPos(m_pos);	// ���_�Œ�
		m_pObj->Update();
	}

	// �G�t�F�N�g�}�l�[�W���[�X�V (�q�b�g�X�g�b�v����)
	m_PanelEffectMGR->Update();
	m_UVEffectMGR->Update();

	// �R���{UI
	//m_pComboUI->Update();

}

void BasePlayer::MoveUpdate() 
{
	if (m_bAerialDash)
	{
		m_move.y *= .8f;
		if (m_iAerialDashFrame-- > 0)
		{
			//// �ړ�
			//if (m_dir == DIR::RIGHT)
			//{
			//	// ������傢
			//	m_move.Set(1.5f, 0, 0);
			//}
			//else
			//{
			//	// ������傢
			//	m_move.Set(-1.5f, 0, 0);
			//}
		}
		else m_bAerialDash = false;

		//m_move *= 0.8f;	// ����(A���:���̒l�̓L�����ŗL�̒l)

		return;
	}
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
			//m_move.x *= 0.98f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
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
		//m_OverDriveGage++;
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

		// �������o�����̂Ƃ��̓Q�[�W�����܂�Ȃ���!!!!!!!
		m_OverDriveGage = 0;

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

void BasePlayer::ColorUpdate()
{
	// �I�����W�̌��̍X�V
	m_fOrangeColRate -= 0.03f;
	m_fOrangeColRate = max(0.0f, m_fOrangeColRate);

	// �}�[���^�̌��̍X�V
	m_fMagentaColRate -= 0.05f;
	m_fMagentaColRate = max(0.0f, m_fMagentaColRate);

}

void BasePlayer::Control()
{
	// 0�t���[���ڂɃR�}���h�t���O������̂ŁA����̑O�ɃR�}���h�r�b�g���X�g����ɉ����グ��

	// �܂��A�����O�̂��ۑ�
	WORD temp(m_CommandHistory[0]);

	for (int i = 1; i < c_COMMAND_FRAME_MAX; i++)
	{
		// �J�艺������
		//for (int i2 = i + 1; i2 < c_COMMAND_FRAME_MAX; i2++)
		{
			WORD save(m_CommandHistory[i]);
			m_CommandHistory[i] = temp;

			temp = save;
		}
	}

	// �ŏ�neutral�ɖ߂�(��0�t���[���ڂ���)
	m_CommandHistory[0] &= 0;


	// �L�[�{�[�h�\
	static const KEYCODE KeyCodeList[(int)PLAYER_INPUT::MAX] = {
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_UP,
		KEY_DOWN,
		KEY_R1,
		KEY_R2,
		KEY_R3,
		KEY_L1,
		KEY_L2,
		KEY_L3,
		KEY_START,
		KEY_SELECT,
	};

	static const PLAYER_COMMAND_BIT BitList[(int)PLAYER_INPUT::MAX] = 
	{
		PLAYER_COMMAND_BIT::A,
		PLAYER_COMMAND_BIT::B,
		PLAYER_COMMAND_BIT::C,
		PLAYER_COMMAND_BIT::D,
		PLAYER_COMMAND_BIT::RIGHT,
		PLAYER_COMMAND_BIT::LEFT,
		PLAYER_COMMAND_BIT::UP,
		PLAYER_COMMAND_BIT::DOWN,
		PLAYER_COMMAND_BIT::R1,
		PLAYER_COMMAND_BIT::R2,
		PLAYER_COMMAND_BIT::R3,
		PLAYER_COMMAND_BIT::L1,
		PLAYER_COMMAND_BIT::L2,
		PLAYER_COMMAND_BIT::L3,

		// START��SELECT�g�͖���
		PLAYER_COMMAND_BIT::NEUTRAL,
		PLAYER_COMMAND_BIT::NEUTRAL
	};

	// �������L�[����
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_InputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_deviceID);

		// �����Ă���I
		if (m_InputList[i] & 0x01)
		{
			m_CommandHistory[0] |= (int)BitList[i];
		}
	}

	// �X�e�B�b�N�̎擾
	float x, y;
	tdnInput::GetAxisXYf(&x, &y, m_deviceID);
	
	static const float gosa(.2f);
	if (x > .5f - gosa)
	{
		m_CommandHistory[0] |= (int)PLAYER_COMMAND_BIT::RIGHT;
	}
	else if (x < -.5f + gosa)
	{
		m_CommandHistory[0] |= (int)PLAYER_COMMAND_BIT::LEFT;
	}
	if (y > .5f - gosa)
	{
		m_CommandHistory[0] |= (int)PLAYER_COMMAND_BIT::DOWN;
	}
	else if (y < -.5f + gosa)
	{
		m_CommandHistory[0] |= (int)PLAYER_COMMAND_BIT::UP;
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

// ����{�`��
void BasePlayer::Render()
{
	// �X�^���h�`��
	m_pStand->Render(shaderM, "Persona");

	// ���G�̔��F�`�J�`�J���[�g�𑗂�
	shaderM->SetValue("g_InvincibleColRate", m_InvincibleColRate);

	// ���G�Z�̃I�����W�F�𑗂�
	shaderM->SetValue("g_OrangeColRate", m_fOrangeColRate);
		
	// ���G�Z�̃}�[���^�F�𑗂�
	shaderM->SetValue("g_MagentaColRate", m_fMagentaColRate);
	
	// �o�����Ă��邩// �o�����̃J���[
	if (m_bOverDrive == true||
		m_pStateMachine->isInState(*BasePlayerState::HeavehoDrive::GetInstance())||
		m_pStateMachine->isInState(*BasePlayerState::HeavehoDriveOverFlow::GetInstance())
		)
		shaderM->SetValue("g_OverDriveColRate", 1.0f);
	else shaderM->SetValue("g_OverDriveColRate", 0.0f);
		
	m_pObj->Render(shaderM, "PlayerToon");

	// ���G�Ȃ���Z�ŏd�˂ĕ`��
	if (m_InvincibleTime > 0) m_pObj->Render(RS::ADD);

	// �����Ō��݂̃X�e�[�g�}�V���̏�Ԃ��m�F
	if (m_deviceID == 0)
	{
		//tdnText::Draw(30, 320, 0xffffffff, "�t���[��:%d", m_CurrentActionFrame);
		//m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��
		//
		//if (m_pAI != nullptr)
		//{
		//	 m_pAI->GetFSM()->Render();// AI�X�e�[�g�}�V���ł̕`��
		//}
	}


	// �G�t�F�N�g�}�l�[�W���[�`��
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();
	m_PanelEffectMGR->Render();

#ifdef _DEBUG
	// �����Ō��݂̃X�e�[�g�}�V���̏�Ԃ��m�F
	if (m_deviceID == 0)
	{
		m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��

		if (m_pAI != nullptr)
		{
			m_pAI->GetFSM()->Render();// AI�X�e�[�g�}�V���ł̕`��
		}
	}

	// �R�}���h�����̕`��
	FOR(c_COMMAND_FRAME_MAX)
	{
		char *c	= "";


		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::LEFT)
		{
			c = "��";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::RIGHT)
		{
			c = "��";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::UP)
		{
			c = "��";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::DOWN)
		{
			c = "��";
		}
		if (m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
			m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		{
			c = "��";
		}
		if (m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
			m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		{
			c = "��";
		}
		if (m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
			m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "��";
		}
		if (m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
			m_CommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "��";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		{
			c = "�~";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::B)
		{
			c = "��";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::C)
		{
			c = "��";
		}
		if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::D)
		{
			c = "��";
		}
		//if (m_CommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		//{
		//
		//}

		//switch (m_CommandHistory[i])
		//{
		//case PLAYER_INPUT::NEUTRAL:
		//	c = "�E";
		//	break;
		//case PLAYER_INPUT::A:
		//	c = "A";
		//	break;
		//case PLAYER_INPUT::B:
		//	c = "B";
		//	break;
		//case PLAYER_INPUT::C:
		//	c = "C";
		//	break;
		//case PLAYER_INPUT::D:
		//	c = "D";
		//	break;
		//case PLAYER_INPUT::RIGHT:
		//	c = "��";
		//	break;
		//case PLAYER_INPUT::LEFT:
		//	c = "��";
		//	break;
		//case PLAYER_INPUT::UP:
		//	c = "��";
		//	break;
		//case PLAYER_INPUT::DOWN:
		//	c = "��";
		//	break;
		//case PLAYER_INPUT::R1:
		//	c = "R1";
		//	break;
		//case PLAYER_INPUT::R2:
		//	c = "R2";
		//	break;
		//case PLAYER_INPUT::R3:
		//	c = "R3";
		//	break;
		//case PLAYER_INPUT::L1:
		//	c = "L1";
		//	break;
		//case PLAYER_INPUT::L2:
		//	c = "L2";
		//	break;
		//case PLAYER_INPUT::L3:
		//	c = "L3";
		//	break;
		//case PLAYER_INPUT::START:
		//	c = "START";
		//	break;
		//case PLAYER_INPUT::SELECT:
		//	c = "SELECT";
		//	break;
		//case PLAYER_INPUT::MAX:
		//	break;
		//default:
		//	break;
		//}

		//tdnText::Draw(10 + (i % 30) * 30, 640 + (i / 30 * 30), 0xffffffff, "%s", c);
		tdnFont::RenderString(c, "���C���I", 32, 10 + (i % 64) * 30, 640 + (i / 30 * 30), 0xffffffff, RS::COPY);

	}

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
	//tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	//tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	//tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "�X�^���h�Q�[�W : %d", m_pStand->GetStandGage());
	//tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "�X�^���h�X�g�b�N: %d", m_pStand->GetStandStock());
	
	//tdnText::Draw(32 + m_deviceID * 250, 430, 0xff00ff80, "OD �Q�[�W: %d", m_OverDriveGage);
	//tdnText::Draw(32 + m_deviceID * 250, 460, 0xffff8000, "OD�c�莞��: %d", m_OverDriveFrame);


	Vector2 pos2d = Math::WorldToScreen(m_pos);
	
	//if (m_deviceID == 1 || m_deviceID == 2)
	{
		//tdnText::Draw((int)pos2d.x, (int)pos2d.y - 150, 0xff00ff80, "C_Score : %d", m_CollectScore);
		//tdnText::Draw((int)pos2d.x, (int)pos2d.y-100, 0xffff8000, "%dP!!", m_deviceID);

	}


}

void BasePlayer::Render(tdnShader* shader, char* name)
{
	// �X�^���h�`��
	m_pStand->Render(shader, name);

	// ���G�̔��F�𑗂�
	//shader->SetValue("g_InvincibleColRate", m_InvincibleColRate);
	// �f�o�b�O
	//tdnText::Draw(332 + m_deviceID * 350, 260, 0xff00ff80, "m_InvColRate : %.2f", m_InvincibleColRate);

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
	//tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	//tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	//tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "�X�^���h�Q�[�W : %d", m_pStand->GetStandGage());
	//tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "�X�^���h�X�g�b�N: %d", m_pStand->GetStandStock());

}

void BasePlayer::RenderShadow()
{
	m_pObj->Render(shaderM, "ShadowBuf");
}


void BasePlayer::RenderDrive()
{
	m_pObj->Render();

	// �G�t�F�N�g�}�l�[�W���[�`��
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();

	// �X�s�[�h���C���̕`��
	m_pSpeedLine->Render();
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
	//m_pComboUI->Render(100 + (m_deviceID * 950), 200);

}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

// ���G�t�F�N�g�������@�i�S�ẴG�t�F�N�g�̔����ꏊ�j
void BasePlayer::AddEffectAction(Vector3 pos, EFFECT_TYPE effectType, Vector3 AttackVec)
{
	// ���̃G�t�F�N�g���d�˂邩�͂����ŉ������߂�B

	// �U�������������p�x��ς���
	float zAngle = atan2(-AttackVec.x, AttackVec.y);	

	// �L�����N�^�[�����ɂ�����
	float diaAngle = (m_dir == DIR::RIGHT) ? 0.0f : PI;

	// �΂߂ɕ`�悵�Č��₷������p
	//float supportAngleY = (m_dir == DIR::RIGHT) ? -(PI * 0.0625f) : (PI * 0.0625f);
	float supportAngleY = (m_dir == DIR::RIGHT) ? -(PI * 0.08f) : (PI * 0.08f);

	// �W�܂�G�t�F�N�g�̏ꏊ�␳
	Vector3 convAddPos = Vector3(0, -10, 0);

	switch (effectType)
	{
	case EFFECT_TYPE::DAMAGE:
	{
		//m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURN);
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::HIT);
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);

		// �����Move�l�l��
		float z = atan2(-m_move.x, m_move.y);
		//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE, 1, 1, Vector3(0, supportAngleY, z), Vector3(0, supportAngleY, z));

		// �ђʔg
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_IMPACT,
			1, 2, Vector3(0, 0, z), Vector3(0, 0, z));

		// �q�b�g�����O
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_RING, 2, 3, Vector3(0, supportAngleY, z), Vector3(0, supportAngleY, z));

		// 
		PointLightMgr->AddPointLight(pos + Vector3(0, 5, 0), Vector3(1.4f, 0.8f, 0.0f), 20, 4, 20, 4, 15);// �|�C���g���C�g�G�t�F�N�g�I
		Vector3 FlyVector(m_move);
		FlyVector.Normalize();
		ParticleManager::EffectHit(pos, FlyVector);

		
		// �u���\�G�t�F�N�g
		DeferredManagerEx.SetRadialBlur(m_pos, 3.25f);

	}
		break;
	case EFFECT_TYPE::WHIFF:
	{
		Vector2 screenPos;
		screenPos = VECTOR2_ZERO;

		screenPos=Math::WorldToScreen(pos);

		//m_PanelEffectMGR->AddEffect((int)screenPos.x, (int)screenPos.y, PANEL_EFFECT_TYPE::WEAK);
	

		//nt delayTimer = 0;// �`�b�x��
		//_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_CIRCLE,
		//   3.0f, 5.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, -3), delayTimer);

	}	break;
	case EFFECT_TYPE::RECOVERY:
		m_PanelEffectMGR->AddEffect(pos + Vector3(0, 5, 0), PANEL_EFFECT_TYPE::ClEAR);


		break;
	case EFFECT_TYPE::PERSONA:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PERSONA,
			1, 2.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectPersonaTrigger(pos);
		PointLightMgr->AddPointLight(pos + Vector3(0, 3, 0), Vector3(0, .4f, 2.0f), 50, 4, 40, 10, 30);// �|�C���g���C�g�G�t�F�N�g�I

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

		m_UVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0));
		m_UVEffectMGR->AddEffect(pos + convAddPos + Vector3(0, -5, 0), UV_EFFECT_TYPE::CONV2, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 4);
		m_UVEffectMGR->AddEffect(pos + convAddPos + Vector3(-0.5, -7, 0), UV_EFFECT_TYPE::CONV3, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 8);
		m_UVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV4, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 12);
		break;
	case EFFECT_TYPE::RUN:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_pos, (m_dir != DIR::LEFT));

		break;
	case EFFECT_TYPE::GUARD_BREAK:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::GLASS);

		break;
	case EFFECT_TYPE::AIROU_DRILL:
	{
									 int delayTimer = 15;// �`�b�x��
									 m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_DRILL,
										 1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0), delayTimer);

	}	break;
	case EFFECT_TYPE::AIROU_CIRCLE:
	{
									  int delayTimer = 0;// �`�b�x��
									  m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_CIRCLE,
										  3.0f, 5.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, -3), delayTimer);

	}	break;
	case EFFECT_TYPE::GUARD_WAVE:
	{
									// �����Move�l�l��
									float z = atan2(-m_move.x, m_move.y);

									// �K�[�h�E�F�\�u
									m_UVEffectMGR->AddMultipleEffect(pos, UV_EFFECT_MULTIPLE_TYPE::GUARD_WAVE, 0.5f, 0.5f, Vector3(0, -supportAngleY*1.5f, z), Vector3(0, -supportAngleY*1.5f, z));
									m_UVEffectMGR->AddMultipleEffect(pos, UV_EFFECT_MULTIPLE_TYPE::GUARD_GRID, 0.2f, 0.5f, Vector3(0, -supportAngleY*1.5f, z), Vector3(0, -supportAngleY*1.5f, z));

	}	break;
	case EFFECT_TYPE::MULTIPLE_HIT:
	{
		// �q�b�g���b�v
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_UVEffectMGR->AddMultipleEffect(pos, UV_EFFECT_MULTIPLE_TYPE::HIT_RIP, 1.0f, 2.0f, Vector3(ram, -supportAngleY*1.5f, 0), Vector3(ram, -supportAngleY*1.5f, 0));
		
		float slashRam = tdnRandom::Get(-3.14f, 3.14f);
		m_UVEffectMGR->AddMultipleEffect(pos + Vector3(0, 0, -5) , UV_EFFECT_MULTIPLE_TYPE::HIT_SLASH, 2.0f, 4.0f, Vector3(0, 0, slashRam), Vector3(0, 0, slashRam));

		// �u���\�G�t�F�N�g
		DeferredManagerEx.SetRadialBlur(m_pos, 2.55f);

	}	break;
	case EFFECT_TYPE::OVER_DRIVE_ACTION:
	{

		m_PanelEffectMGR->AddEffect
			(pos, PANEL_EFFECT_TYPE::OVER_DRIVE_START);

		// OVER_DRIVE_RING
		m_PanelEffectMGR->AddEffect
			(pos, PANEL_EFFECT_TYPE::OVER_DRIVE, 7);

		// OVER_DRIVE_RING
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::OVER_DRIVE_RING, 0.5f, 3.5f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 7);

	}	break;
	case EFFECT_TYPE::JUMP:
	{
		// �W�����v�G�t�F�N�g
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
		// �W�����v�G�t�F�N�g��
		// �����Move�l�l��
		float z = atan2(-m_move.x, m_move.y);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, z), Vector3(0, 0, z));


	}	break;
	case EFFECT_TYPE::AERIAL_JUMP:
	{

		float diaAngle = (m_move.x >= 0) ? PI / 2 : -PI / 2;

		// �W�����v�G�t�F�N�g
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, diaAngle), Vector3(0, 0, diaAngle));
		// �W�����v�G�t�F�N�g��
		// �����Move�l�l��
		//float z = atan2(-m_move.x, m_move.y);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, diaAngle), Vector3(0, 0, diaAngle));


	}	break;
	case EFFECT_TYPE::INVINCIBLE_ATTACK:
	{
		// �v���b�V���[
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));
	
		// �I�����W�̌�
		m_PanelEffectMGR->AddEffect
			(pos + Vector3(0, 10, -4) , PANEL_EFFECT_TYPE::ORANGE_LOGHT, 0);

		// �I�����W�̃����O
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_UVEffectMGR->AddEffect(pos + Vector3(0, 10, -2), UV_EFFECT_TYPE::ORANGE_BURST, 0.1f, 1.75f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 0);

		// �L�����N�^�[���̂��I�����W��
		m_fOrangeColRate = 1.0f;
	}	break;
	case EFFECT_TYPE::DOKKOI:
	{
		// �v���b�V���[
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// �ǂ������̌�
		m_PanelEffectMGR->AddEffect
			(pos + Vector3(0, 8, -2), PANEL_EFFECT_TYPE::DOKKOI, 0);

		// �L�����N�^�[���̂��}�[���^��
		m_fMagentaColRate = 1.0f;

	}	break;
	case EFFECT_TYPE::MUZZLE_FLASH:
	{
		// �}�Y���t���b�V��
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::MUZZLE_FLASH, 0.90f, 1.0f, Vector3(0, 0, zAngle), Vector3(0, 0, zAngle));

		// ��юU���
		//m_PanelEffectMGR->AddEffect
		//	(pos + Vector3(0, 8, -2), PANEL_EFFECT_TYPE::DOKKOI, 0);

	}	break;
	default:
		MyAssert(0,"����ȃG�t�F�N�g�͑��݂��Ȃ� ");	// ����ȃG�t�F�N�g�͑��݂��Ȃ� AddEffectAction()
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