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
const int BasePlayer::c_OVERDRIVE_MAX_TIME = 480;	// �o�����؂��܂ł̎���

const int BasePlayer::c_THROW_ESCAPE_FRAME = 15;	// �����ʂ��̗P�\�t���[��
const int BasePlayer::c_THROW_MISS_FRAME = 30;	// �����O�����X�̃t���[��(�S�L�������ʂ��낤�Ƃ����l��)
const int BasePlayer::c_THROW_RELEASE_FRAME = 15;	// ���������ŁA�p�V���ĂȂ��Ă�Ԃ̃t���[��(������S�L�������ʂ��낤)
const int BasePlayer::c_WINNER_TIME = 180;
const float BasePlayer::c_GUARD_DISTANCE = 32.0f;
const int BasePlayer::c_FIRST_HIT_ADD_DAMAGE = 300;	// ���i�q�b�g���Z�_���[�W

// �L�����N�^�[��{���ǂݍ���
void BasePlayer::LoadCharacterParam(LPSTR filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "parameter�̃e�L�X�g�����ĂȂ�");

	std::string sDirectory(tdnFile::GetDirectoryPath(filename));

	char skip[128];	// �ǂݔ�΂��p�ϐ�

	// ���b�V��
	char path[MAX_PATH];
	ifs >> skip;
	ifs >> path;
	assert(!m_pDefaultObj);	// ������null����Ȃ��Ȃ炨������
	m_pDefaultObj = new iex3DObj((char*)(sDirectory + "/" + path).c_str());
	m_pDefaultObj->SetPos(m_vPos);
	m_pDefaultObj->Update();

	// �K�E�p���b�V��
	ifs >> skip;
	ifs >> path;
	assert(!m_pHHDOFObj);	// ������null����Ȃ��Ȃ炨������
	m_pHHDOFObj = new iex3DObj((char*)(sDirectory + "/" + path).c_str());
	m_pHHDOFObj->SetAngle(PI);
	m_pHHDOFObj->Update();


	// 2P�J���[�e�N�X�`��(���݂���)
	//ifs >> skip;
	//ifs >> path;
	//if (m_side == SIDE::RIGHT)
	//{
	//	m_pDefaultObj->SetTexture(tdnTexture::Load((char*)(sDirectory + "/" + path).c_str()), 0);
	//	m_pHHDOFObj->SetTexture(tdnTexture::Load((char*)(sDirectory + "/" + path).c_str()), 0);
	//}

	// �{�̂̔���
	ifs >> skip;
	ifs >> m_tagCharacterParam.HitSquare.width;
	ifs >> m_tagCharacterParam.HitSquare.height;
	ifs >> m_tagCharacterParam.HitSquare.pos.y;

	// �ő呬�x(�_�b�V�����x
	ifs >> skip;
	ifs >> m_tagCharacterParam.fMaxSpeed;

	// �󒆃_�b�V�����x
	ifs >> skip;
	ifs >> m_tagCharacterParam.fAerialDashSpeed;

	// �ő�HP
	ifs >> skip;
	ifs >> m_tagCharacterParam.iMaxHP;
	m_iHP = m_tagCharacterParam.iMaxHP;

	// ��������ۑ�
	memcpy_s(&m_tagOrgCharacterParam, sizeof(CharacterParam), &m_tagCharacterParam, sizeof(CharacterParam));
}

// �A�N�V�����t���[�����ǂݍ���
void BasePlayer::LoadAttackFrameList(LPSTR filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�A�^�b�N�t���[���̃e�L�X�g�����ĂȂ�");

	// ���|�C���^�^�̔�r�́u�����Ă���A�h���X�����������ǂ����v(�������񎩑̂̔�r�����Ă���Ȃ�)�Ŕ��f����̂ŁA�Ӑ}��������͂��Ȃ�
	static const std::map<std::string, BASE_ACTION_STATE> IDList
	{
		{ "[[�G�X�P�[�v]]", BASE_ACTION_STATE::ESCAPE },
		{ "[[�o�b�N�X�e�b�v]]", BASE_ACTION_STATE::BACKSTEP },
		{ "[[�������A�o��]]", BASE_ACTION_STATE::OVERDRIVE_ONEMORE },
		{ "[[�X�^���h����]]", BASE_ACTION_STATE::STAND },
		{ "[[�o�[�X�g�o��]]", BASE_ACTION_STATE::OVERDRIVE_BURST },
		{ "[[�ʏ�U��1�i��]]", BASE_ACTION_STATE::RUSH1 },
		{ "[[�ʏ�U��2�i��]]", BASE_ACTION_STATE::RUSH2 },
		{ "[[�ʏ�U��3�i��]]", BASE_ACTION_STATE::RUSH3 },
		{ "[[�΋�U��]]", BASE_ACTION_STATE::ANTI_AIR },
		{ "[[���i�U��]]", BASE_ACTION_STATE::DOKKOI_ATTACK },
		{ "[[���Ⴊ�݉��i�U��]]", BASE_ACTION_STATE::SQUAT_ATTACK },
		{ "[[�����΂炢�U��]]", BASE_ACTION_STATE::DOWN_ATTACK },
		{ "[[�󒆍U��]]", BASE_ACTION_STATE::AERIAL_ATTACK },
		{ "[[�󒆉��U��]]", BASE_ACTION_STATE::AERIALDROP },
		{ "[[�t�؂�U��]]", BASE_ACTION_STATE::INVINCIBLE_ATTACK },
		{ "[[�L�����ŗL�n��j���[�g����]]", BASE_ACTION_STATE::SKILL },
		{ "[[�L�����ŗL2]]", BASE_ACTION_STATE::SKILL2 },
		{ "[[�L�����ŗL�n�サ�Ⴊ��]]", BASE_ACTION_STATE::SKILL_SQUAT },
		{ "[[�L�����ŗL�󒆃j���[�g����]]", BASE_ACTION_STATE::SKILL_AERIAL },
		{ "[[�L�����ŗL�󒆉�]]", BASE_ACTION_STATE::SKILL_AERIALDROP },
		{ "[[�͂�]]", BASE_ACTION_STATE::THROW },
		{ "[[��������]]", BASE_ACTION_STATE::THROW_SUCCESS },
		{ "[[�K�E�Z]]", BASE_ACTION_STATE::HEAVEHO_DRIVE },
		{ "[[���K�E�Z]]", BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW }
	};

	while (!ifs.eof())
	{
		// ID�ǂݍ���
		char read[64];
		ifs >> read;

		// �G���[�`�F�b�N
		MyAssert(read[0] == '[', "�U���f�[�^�̃e�L�X�g������Ă�");
		//MyAssert(IDList.count(ID) != 0, "�U���f�[�^��ID���ԈႦ�Ă���\��\n%s", ID);

		std::string ID(read);

		// �n���A�����A�d���t���[����ǂݍ���
		int count(0);
		int iActionType((int)IDList.at(ID));

		FOR((int)FRAME_STATE::END)
		{
			int frame;
			ifs >> frame;
			for (int j = 0; j < frame; j++)
			{
				m_ActionFrameList[iActionType][count++] = (FRAME_STATE)i;
				MyAssert(count < 256, "�A�N�V�����t���[���̍��v��255�ȉ��ɂ��Ă�������");
			}
		}
		// �I�[(�S��END�ŏ��������Ă邩����v�����ǈꉞ)
		m_ActionFrameList[iActionType][count] = FRAME_STATE::END;
	}

	// �t���[���J�E���g
	{
		FOR(FRAME_MAX - 1) m_ActionFrameList[(int)BASE_ACTION_STATE::FRAMECOUNT][i] = FRAME_STATE::START;
		m_ActionFrameList[(int)BASE_ACTION_STATE::FRAMECOUNT][FRAME_MAX - 1] = FRAME_STATE::END;
	}
}

void BasePlayer::LoadAttackDatas(LPSTR filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�U���f�[�^�̃e�L�X�g�����ĂȂ�");

	static const std::map<LPSTR, BASE_ACTION_STATE> IDList
	{
		{ "[[�ʏ�U��1�i��]]",				BASE_ACTION_STATE::RUSH1 },
		{ "[[�ʏ�U��2�i��]]",				BASE_ACTION_STATE::RUSH2 },
		{ "[[�ʏ�U��3�i��]]",				BASE_ACTION_STATE::RUSH3 },
		{ "[[�΋�U��]]",					BASE_ACTION_STATE::ANTI_AIR },
		{ "[[���i�U��]]",					BASE_ACTION_STATE::DOKKOI_ATTACK },
		{ "[[���Ⴊ�݉��i�U��]]",			BASE_ACTION_STATE::SQUAT_ATTACK },
		{ "[[�����΂炢�U��]]",				BASE_ACTION_STATE::DOWN_ATTACK },
		{ "[[�󒆍U��]]",					BASE_ACTION_STATE::AERIAL_ATTACK },
		{ "[[�󒆉��U��]]",					BASE_ACTION_STATE::AERIALDROP },
		{ "[[�t�؂�U��]]",					BASE_ACTION_STATE::INVINCIBLE_ATTACK },
		{ "[[�L�����ŗL�n��j���[�g����]]", BASE_ACTION_STATE::SKILL },
		{ "[[�L�����ŗL�n�サ�Ⴊ��]]",		BASE_ACTION_STATE::SKILL_SQUAT },
		{ "[[�L�����ŗL�󒆃j���[�g����]]", BASE_ACTION_STATE::SKILL_AERIAL },
		{ "[[�L�����ŗL�󒆉�]]",			BASE_ACTION_STATE::SKILL_AERIALDROP },
		{ "[[�͂�]]",						BASE_ACTION_STATE::THROW },
		{ "[[��������]]",					BASE_ACTION_STATE::THROW_SUCCESS },
		{ "[[�K�E�Z]]",						BASE_ACTION_STATE::HEAVEHO_DRIVE },
		{ "[[���K�E�Z]]",					BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW }
	};

	while (!ifs.eof())
	{
		// �ǂݔ�΂��p�ϐ�
		//char skip[128];

		// ID�ǂݍ���
		{
			char ID[64];
			ifs >> ID;	// �ǂݔ�΂��p�����Ǎė��p

			// �G���[�`�F�b�N
			MyAssert(ID[0] == '[', "�U���f�[�^�̃e�L�X�g������Ă�");
			MyAssert(IDList.count(ID) != 0, "�U���f�[�^��ID���ԈႦ�Ă���\��\n%s", ID);
		}

		// �����ɂ���G���A�͒n��q�b�g�Ƌ󒆃q�b�g�Ƃ��֌W�Ȃ����ʂ̏��
		//int HitScore;				// ���Z�����X�R�A
		//int damage;					// �^����_���[�W
		//bool bHit;					// �����������ǂ���(�����i�q�b�g�h�~�p)���i�q�b�g���Ăق����U�����ƁA�܂��l����K�v������
		//bool bHitSuccess;			// �U�����K�[�h���ꂸ�ɓ����������ǂ���
		//int WhiffDelayFrame;		// ���t���[����ɋ�U���SE���Đ����邩(WhiffSE���g��Ȃ�(nullptr)�Ȃ�ʂɐݒ肵�Ȃ��Ă�����)
		//EFFECT_TYPE HitEffectType;	// �G�t�F�N�g�̃^�C�v
		//LPCSTR HitSE;				// ���������Ƃ��ɖ炷SE
		//LPCSTR WhiffSE;				// ��U��SE
		//EFFECT_TYPE WhiffEffectType;	// �U��G�t�F�N�g
		//int pierceLV;				// �ђʃ��x��
		//bool bAntiAir;				// �΋�U�����ǂ����@(�ǉ�)
		//bool bFinish;				// �t�B�j�b�V���ɂȂ�U�����ǂ���
		//ANTIGUARD_ATTACK AntiGuard;	// �K�[�h��˂��j��U���̃^�C�v
		//ATTACK_ATTRIBUTE attribute;	// �U���̑���(�e�A�����A�Ō�)
		//SHAKE_CAMERA_INFO ShakeCameraInfo;	// �J�����U���p�\����
		//int GuardRecoveryFrame;		// �K�[�h���ꂽ�Ƃ��Ɂu�K�[�h���Ă��鑊��ɗ^����v�d������
		//float fGuardKnockBackPower;	// �K�[�h�������Ƃ��̂������
		//float fComboRate;			// �R���{�␳(0.8�Ȃ玟�̍U����0.8�{�̕␳��������A�␳�͊|�����킳���)
		//
		//
		//CollisionShape::Square *pCollisionShape;	// �������蔻��`��(�l�p�ɂ���)
	}
}

/*
�R���X�g���N�^�ɂ����āA�������q���X�g�Ƃ��Ċe�����o�[�̃f�t�H���g�R���X�g���N�^���Ăяo���`��
�umemberVariableName()�v�ŋL�q���邱�Ƃɂ��A
�g���̒l�^�Aenum�^��|�C���^�^�����łȂ��A
�Œ蒷�z���POD�^�\���̂����ׂă[���N���A�ł��܂�

�i�������z���POD�\���̂̃����o�[���[���ȊO�̐��ŏ���������ɂ́A
��q����悤��C++11��initializer list/uniform initialization�Ƀt���Ή������R���p�C�����K�v�ɂȂ�܂��j�B
*/
BasePlayer::BasePlayer(SIDE side, const SideData &data) :
BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + (int)side)),

// m_tagCharacterParam{} ���Ƃ���Ȃ��A�E�g
m_tagCharacterParam(CharacterParam{}),
m_tagOrgCharacterParam(CharacterParam{}),

m_vPos(0,0,0),
m_vMove(0,0,0),
m_bMoveUpdate(true),

m_fAngleY(0),
m_dir(DIR::LEFT),
m_TargetDir(DIR::LEFT),

//m_ActionFrameList(),
//m_ActionDatas(),
m_eActionState(BASE_ACTION_STATE::NO_ACTION),
m_iCurrentActionFrame(0),
m_iRushStep(0),
m_eSkillActionType(SKILL_ACTION_TYPE::NO_ACTION),

m_bLand(true),
m_bSquat(false),
m_bAerialJump(true),
m_bAerialDash(false),
m_iAerialDashFrame(0),

//m_RecoveryDamageCount(),
m_iRecoveryFrame(0),
m_iHitStopFrame(0),// �������o�O�̌����@�Ђ��Ƃ��g�b�v�������̂��Y��B
m_bDown(false),

m_eGuardState(GUARD_STATE::NO_GUARD),
m_bGuardSuccess(false),

m_iInvincibleLV(0),
m_iInvincibleTime(0),
m_fInvincibleColRate(0),
m_iInvincibleColRateFlame(0),
m_bInvincibleColRateUpFlag(false),
m_bEscape(false),

//m_iMotionNumbers(),

m_iHP(0),
m_fDamageRate(0),

m_iScore(0),
m_iCollectScore(0),

//m_iInputList(),
//m_wCommandHistory(),
m_wAheadCommand(0),

m_pStand(nullptr),

m_pTargetPlayer(nullptr),

m_pDefaultObj(nullptr),
m_pHHDOFObj(nullptr),
m_pObj(nullptr),
m_pSpeedLine(nullptr),
m_pFacePic(nullptr),

m_pPanelEffectMGR(nullptr),
m_pUVEffectMGR(nullptr),
m_pThrowMark(nullptr),

m_OverDriveGage(0),
m_bOverDrive(false),
m_OverDriveFrame(0),
m_OverDriveType(OVERDRIVE_TYPE::ONEMORE),

m_bWillPower(false),
m_iSpGettingFrame(0),

m_iDeviceID(data.iDeviceID),
m_side(side),
m_pStateMachine(nullptr),
m_pAI(nullptr),
m_bAI(data.bAI),

m_iDashFrame(0),

m_iHeavehoStopTimer(0),
m_iHeaveHoDriveOverFlowFrame(0),
m_bLastOneWin(false),

m_fOrangeColRate(0),
m_fMagentaColRate(0),

m_bGameTimerStopFlag(false),

m_iWinNum(0),

m_bNotOverDrive(false),

m_pCutinPic(nullptr),
m_pName("None")
{
	// �X�^���h
	switch (data.ePartner)
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

	// �G�t�F�N�g�}�l�[�W���[
	m_pPanelEffectMGR = new PanelEffectManager();
	m_pUVEffectMGR	 = new UVEffectManager();
	m_pThrowMark = new tdn2DAnim("Data/Effect/ThrowMark.png");
	m_pThrowMark->OrderShrink(6, 1.0f, 2.5f);


	// ������
	memset(m_iInputList, 0, sizeof(m_iInputList));
	ZeroMemory(&m_jump, sizeof(Jump));
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
		m_vPos.Set(-20, 0, 0);
		m_TargetDir = DIR::RIGHT;
		break;

	case (int)SIDE::RIGHT:
		m_vPos.Set(20, 0, 0);
		m_TargetDir = DIR::LEFT;
		break;
	}

	m_fAngleY = DIR_ANGLE[(int)m_dir];

	m_RecoveryDamageCount.clear();

	// ���̓��X�g������
	memset(m_iInputList, 0, sizeof(m_iInputList));

	// �R�}���h����������
	memset(m_wCommandHistory, (int)PLAYER_COMMAND_BIT::NEUTRAL, sizeof(m_wCommandHistory));

	// ���[�V����������(�L�������Ƃ̏�������ݒ肵������Reset�֐����L�����N�^�[�ŗL���[�h��艺�ɏ������ƂɁ����[�V�����̏�����������O�Ƀ��[�V������ݒ肵�Ă��܂������Ⴀ�悱���őS��0�ɂ��悤���ŏ��̑ҋ@���[�V������0�ł��邱�Ƃ��O��̃R�[�h�ɂȂ��Ă��܂�)
	memset(m_iMotionNumbers, 0, sizeof(m_iMotionNumbers));
	
	// �A�N�V�����t���[�����X�g������(memset�ł��ƂȂ����ςȒl�������Ă�)
	FOR((int)BASE_ACTION_STATE::END)
	{
		m_ActionDatas[i] = ActionData{};
		for (int j = 0; j < FRAME_MAX; j++) m_ActionFrameList[i][j] = FRAME_STATE::END;
	}

	//tdnDebug::OutPutLog("[�������`�F�b�N}\n");
	//tdnDebug::OutPutLog("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, ");
}

void BasePlayer::Reset()
{
	/* [int�^]�̏����� */

	/* 0�ŏ����� */
	m_iInvincibleLV =
	m_iInvincibleTime =
	m_iCurrentActionFrame =
	m_iRecoveryFrame =
	m_iScore =
	m_iCollectScore =
	m_iHitStopFrame =
	m_iInvincibleColRateFlame =
	//m_OverDriveGage =			// ���E���h���p��
	m_OverDriveFrame =
	m_iHP =
	m_iHeavehoStopTimer =
	m_iHeaveHoDriveOverFlowFrame =
	m_iAerialDashFrame =
	m_iRushStep = 
	m_wAheadCommand =
	m_iDashFrame=
	m_iSpGettingFrame = 
	0;

	m_iHP = m_tagCharacterParam.iMaxHP;	// �L�������Ƃ�HP

	/* [float�^]�̏����� */

	/* 0�ŏ����� */
	m_fInvincibleColRate =
	0.0f;

	/* 1�ŏ����� */
	m_fDamageRate =
	1.0f;


	/* [bool�^]�̏����� */

	/* true�ŏ������]�[�� */
	m_bAerialJump =
	m_bMoveUpdate =
	true;

	/* false�ŏ������]�[�� */
	m_bEscape =
	m_bInvincibleColRateUpFlag =
	m_bOverDrive =
	m_bGameTimerStopFlag =
	m_bAerialDash =
	m_bLand=
	m_bSquat=
	m_bWillPower=
	m_bDown=
	m_bNotOverDrive=
	m_bGuardSuccess=
	false;


	/* [�|�C���^�^]�̏�����(new���Ă��͋ɗ͂��̂܂�) */
	//m_pTargetPlayer = nullptr;
	if (m_pDefaultObj) m_pDefaultObj->SetMotion(m_iMotionNumbers[(int)MOTION_TYPE::WAIT]);
	m_pObj = m_pDefaultObj;
	//m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance()); 
	m_pStateMachine->ChangeState(BasePlayerState::Wait::GetInstance());			// [11/28] ���Z�b�g���͂����X�e�[�g���ݒ肳��Ă���̃`�F���W��
	m_pStateMachine->SetPreviousState(BasePlayerState::Wait::GetInstance());

	/* [�񋓌^]�̏����� */
	m_eSkillActionType = SKILL_ACTION_TYPE::NO_ACTION;
	m_eActionState = BASE_ACTION_STATE::NO_ACTION;
	m_OverDriveType = OVERDRIVE_TYPE::BURST;
	m_eGuardState = GUARD_STATE::NO_GUARD;

	/* [�\����]�̏����� */
	memcpy_s(&m_tagCharacterParam, sizeof(CharacterParam), &m_tagOrgCharacterParam, sizeof(CharacterParam));
	m_vMove.Set(0, 0, 0);

	// ID�ō��W�̕���
	switch ((int)m_side)
	{
	case (int)SIDE::LEFT:
		m_vPos.Set(-20, 0, 0);
		m_TargetDir = DIR::RIGHT;
		break;

	case (int)SIDE::RIGHT:
		m_vPos.Set(20, 0, 0);
		m_TargetDir = DIR::LEFT;
		break;
	}


	/* [�z��]�̏����� */
	memset(m_iInputList, 0, sizeof(m_iInputList));
	memset(m_wCommandHistory, (int)PLAYER_COMMAND_BIT::NEUTRAL, sizeof(m_wCommandHistory));



	// �R���{�␳������
	m_RecoveryDamageCount.clear();

	// �p�[�g�i�[�̃Q�[�W��������
	m_pStand->SetStandGage(0);
	m_pStand->SetStandStock(0);

}

void BasePlayer::InitAI(AI_TYPE type)
{
	// AI�t���O���AAI��new����ĂȂ�������
	//if (m_pAI) return;
	// ����AI�̎�ރ^�C�v
	if (m_bAI) m_pAI = new AI(m_side, this, type);
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pDefaultObj);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_pStand);
	SAFE_DELETE(m_pPanelEffectMGR);
	SAFE_DELETE(m_pUVEffectMGR);
	SAFE_DELETE(m_pThrowMark);
	SAFE_DELETE(m_pAI);
	//SAFE_DELETE(m_pComboUI);
	SAFE_DELETE(m_pHHDOFObj);
	SAFE_DELETE(m_pFacePic);
	SAFE_DELETE(m_pSpeedLine);
	SAFE_DELETE(m_pCutinPic);
}

void BasePlayer::Update(PLAYER_UPDATE flag)
{
	//bool bOutLog(false);

	// �f�o�b�O
	if (KeyBoardTRG(KB_8))
	{
		// 50��
		m_OverDriveGage += 50;
		AddEffectAction(m_vPos, EFFECT_TYPE::WILL_POWER);
	}
	if (KeyBoardTRG(KB_9))
	{
		m_iHP = m_tagCharacterParam.iMaxHP;
	}
	if (KeyBoardTRG(KB_0))
	{
		m_iHP = 3000;
		//bOutLog = true;
	}

	//if (bOutLog) tdnStopWatch::Start();
	// �o���̍X�V
	OverDriveUpdate();

	// �K�[�h�G�t�F�N�g�X�V
	GuardEffectUpdate();

	// �����l�����p�̍X�V
	WillPowerUpdate(flag);

	// 1more�o�����Ă�����X�^���h�̓������~�߂�
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// �X�^���h�X�V
		m_pStand->Update((flag == PLAYER_UPDATE::CONTROL_OK));
	}

	// ���G���Ԃ̍X�V
	if (KeyBoardTRG(KB_H))
	{
		SetInvincible(40, 1);
	}
	InvincibleUpdate();

	// ���F�̍X�V
	ColorUpdate();

	//if (bOutLog)
	//{
	//	tdnStopWatch::End();
	//	tdnDebug::OutPutLog("�v���C���[%s:\n�X�V1�ɂ����������� %.1lf\n",(m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
	//}

	//if (bOutLog) tdnStopWatch::Start();

	// ���͎�t
	if (flag != PLAYER_UPDATE::CONTROL_NO)
	{
		// 0�t���[���ڂɃR�}���h�t���O������̂ŁA����̑O�ɃR�}���h�r�b�g���X�g����ɉ����グ��

		// �܂��A�����O�̂��ۑ�
		WORD temp(m_wCommandHistory[0]);

		for (int i = 1; i < c_COMMAND_FRAME_MAX; i++)
		{
			// �J�艺������
			//for (int i2 = i + 1; i2 < c_COMMAND_FRAME_MAX; i2++)
			{
				WORD save(m_wCommandHistory[i]);
				m_wCommandHistory[i] = temp;

				temp = save;
			}
		}

		// �ŏ�neutral�ɖ߂�(��0�t���[���ڂ���)
		m_wCommandHistory[0] &= 0;

		if (m_bAI)	AIControl();
		else		Control();
	}
	else
	{
		memset(m_iInputList, 0, sizeof(m_iInputList));
		memset(m_wCommandHistory, 0, sizeof(m_wCommandHistory));
	}

	//if (bOutLog)
	//{
	//	tdnStopWatch::End();
	//	tdnDebug::OutPutLog("�v���C���[%s:\n���͂ɂ����������� %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
	//}

	// ���q�b�g�X�g�b�v��
	if (m_iHitStopFrame > 0)
	{
		m_iHitStopFrame--;

		// ����s���͎�t(�������u�Ԃ����擾�ɕύX)
		AheadCommandUpdate();
	}
	// �q�b�g�X�g�b�v�������܂œ����Ȃ�
	else
	{
		// ���d�����Ԃ̃f�N�������g
		if (m_iRecoveryFrame > 0)
			m_iRecoveryFrame--;
		else m_fDamageRate = 1.0f;	// (TODO�R���{�؂��1�t���[���ŕ␳���؂�Ă�̂ɃR���{������(�A�C���[1,2,��,�X�L������3400���ł�))�R���{�؂ꂽ��A�_���[�W���[�g��߂�

		// �A�N�V�����t���[���̍X�V
		if (isFrameAction())
		{
			// �U���t���[���Ȃ�
			if (m_ActionDatas[(int)m_eActionState].isAttackData())
			{
				// ���̌o�ߎ��Ԃƃf�B���C�t���[���ɂȂ�����
				if (m_ActionDatas[(int)m_eActionState].pAttackData->WhiffDelayFrame == m_iCurrentActionFrame)
				{
					// (A���)�����ōU�����肪���������u�Ԃ�����Ă��Ă�
					
					// �U��G�t�F�N�g�����i���j�I
					AddEffectAction(m_vPos + Vector3(0, 5, -3) , m_ActionDatas[(int)m_eActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_eActionState].pAttackData->WhiffSE;
					// ��U��SE�����Ă���
					if (SE_ID)
					{
						// �f�B���C�t���[���o�߂�����Đ�
						se->Play((LPSTR)SE_ID);
					}
				}

				// ���蕜���t���[��
				if (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::RECOVERY_HIT)
				{
					// ���胊�Z�b�g
					GetAttackData()->bHit = GetAttackData()->bHitSuccess = false;
				}

			}

			// ���[�V�����̃t���[�����X�V�I
			m_iCurrentActionFrame++;

			// �t���[���Ō�܂ōĐ�������
			if (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::END)
			{
				// ���A�N�V�����X�e�[�g����
				m_eActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}



		// �������̌�����ݒ�
		// ����if���ɓ����Ă��ȊO��������U�����
		if (!m_pStateMachine->isInState(*BasePlayerState::Skill::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::SquatAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::DownAttack::GetInstance()) &&
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
			!m_pStateMachine->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) &&
			!m_pStateMachine->isInState(*BasePlayerState::AerialAttack::GetInstance())
			)
		{
			m_dir = m_TargetDir;

			// �A���O����ԏ���
			const float AnglePercentage = (m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance())) ? .9f : .65f;

			if (m_pStateMachine->isInState(*BasePlayerState::Wait::GetInstance()) || 
				m_pStateMachine->isInState(*BasePlayerState::Intro::GetInstance()))
			{
				m_fAngleY = DIR_ANGLE[(int)m_dir];
				m_fAngleY += (m_dir == DIR::LEFT) ? PI * -.1f : PI * .1f;
			}

			// �����Ă鎞�ȊO�Ȃ�␳
			else if (!m_pStateMachine->isInState(*BasePlayerState::Win::GetInstance()))
			{
				m_fAngleY = m_fAngleY * AnglePercentage + DIR_ANGLE[(int)m_dir] * (1 - AnglePercentage);
			}
		}

		//if (bOutLog) tdnStopWatch::Start();

		// ���͎�t��ɃX�e�[�g�}�V���X�V
		if (flag != PLAYER_UPDATE::NO_FSM) m_pStateMachine->Update();

		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("�v���C���[%s:\n�X�e�[�g�}�V���X�V�ɂ����������� %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

		// �����̐���
		MoveUpdate();

		// ���b�V���̍X�V
		//if (bOutLog) tdnStopWatch::Start();
		if (!m_pStateMachine->isInState(*BasePlayerState::KnockBack::GetInstance()))m_pObj->Animation();
		else if (m_iRecoveryFrame < 30)m_pObj->Animation();
		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("�v���C���[%s:\n���b�V��Animation()�ɂ����������� %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

		//if (bOutLog) tdnStopWatch::Start();
		m_pObj->SetAngle(m_fAngleY);	// ���E�̃A���O���̃Z�b�g
		m_pObj->SetPos(m_vPos);
		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("�v���C���[%s:\n���b�V��SetPos()�Ƃ��ɂ����������� %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

		//if (bOutLog) tdnStopWatch::Start();

		// ���[�V������Ԃ��|����̂����A�U�����[�V�������኱�x��Ă��܂��̂ŁA�U�����[�V�����̂Ƃ��͕�Ԃ��|���Ȃ�
		const float slerp((isAttackState()) ? 0 : .5f);
		m_pObj->Update();
		//if (bOutLog)
		//{
		//	tdnStopWatch::End();
		//	tdnDebug::OutPutLog("�v���C���[%s:\n���b�V��Update()�ɂ����������� %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
		//}

	}

	//if (bOutLog) tdnStopWatch::Start();

	// �G�t�F�N�g�}�l�[�W���[�X�V (�q�b�g�X�g�b�v����)
	m_pPanelEffectMGR->Update();
	m_pUVEffectMGR->Update();
	m_pThrowMark->Update();

	//if (bOutLog)
	//{
	//	tdnStopWatch::End();
	//	tdnDebug::OutPutLog("�v���C���[%s:\n�G�t�F�N�g�X�V�ɂ����������� %.1lf\n", (m_side == SIDE::LEFT) ? "LEFT" : "RIGHT", tdnStopWatch::GetResult());
	//}
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
	if (m_iRecoveryFrame > 0)
		m_iRecoveryFrame--;

	// ���q�b�g�X�g�b�v��
	if (m_iHitStopFrame > 0)
	{
		m_iHitStopFrame--;
	}
	// �q�b�g�X�g�b�v�������܂œ����Ȃ�
	else
	{
		// �A�N�V�����t���[���̍X�V
		if (isFrameAction())
		{
			// �U���t���[���Ȃ�
			if (m_ActionDatas[(int)m_eActionState].isAttackData())
			{
				// ���̌o�ߎ��Ԃƃf�B���C�t���[���ɂȂ�����
				if (m_ActionDatas[(int)m_eActionState].pAttackData->WhiffDelayFrame == m_iCurrentActionFrame)
				{
					// (A���)�����ōU�����肪���������u�Ԃ�����Ă��Ă�

					// �U��G�t�F�N�g�����i���j�I
					AddEffectAction(m_vPos + Vector3(0, 5, -3), m_ActionDatas[(int)m_eActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_eActionState].pAttackData->WhiffSE;
					// ��U��SE�����Ă���
					if (SE_ID)
					{
						// �f�B���C�t���[���o�߂�����Đ�
						se->Play((LPSTR)SE_ID);
					}
				}

			}

			// ���[�V�����̃t���[�����X�V�I
			m_iCurrentActionFrame++;

			// �t���[���Ō�܂ōĐ�������
			if (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::END)
			{
				// ���A�N�V�����X�e�[�g����
				m_eActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}

		// ���͎�t��ɃX�e�[�g�}�V���X�V
		m_pStateMachine->Update();

		// �����̐���
		//Move();
		m_vMove.Set(0, 0, 0);

		// ���b�V���̍X�V
		m_pObj->Animation();
		m_pObj->SetAngle(m_fAngleY);	// (TODO)���͌Œ�
		m_pObj->SetPos(m_vPos);	// ���_�Œ�
		m_pObj->Update();
	}

	// �G�t�F�N�g�}�l�[�W���[�X�V (�q�b�g�X�g�b�v����)
	m_pPanelEffectMGR->Update();
	m_pUVEffectMGR->Update();

	// �R���{UI
	//m_pComboUI->Update();

}

void BasePlayer::MoveUpdate() 
{
	if (m_bAerialDash)
	{
		m_vMove.y *= .8f;
		if (m_iAerialDashFrame-- > 0)
		{
			//// �ړ�
			//if (m_dir == DIR::RIGHT)
			//{
			//	// ������傢
			//	m_vMove.Set(1.5f, 0, 0);
			//}
			//else
			//{
			//	// ������傢
			//	m_vMove.Set(-1.5f, 0, 0);
			//}
		}
		else m_bAerialDash = false;

		//m_vMove *= 0.8f;	// ����(A���:���̒l�̓L�����ŗL�̒l)

		return;
	}
	// �ړ��ʍX�V
	if (
		//m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false
		m_bMoveUpdate
		)
	{	// �y���\�i����������Ȃ�������ړ�
		m_vMove.y -= c_GRAVITY;
		//if (m_vMove.y <= -3.0f) { m_vMove.y = -3.0f; } // �����鑬�x��}��
		m_vMove.y = max(-2.75f, m_vMove.y);// �����鑬�x��}��

		// ��C��R(x�̒l�����X�Ɍ��炵�Ă���)
		if (m_bLand)
		{
			//if (m_vMove.x > 0)		m_vMove.x = max(m_vMove.x - .055f, 0);
			//else if (m_vMove.x < 0)	m_vMove.x = min(m_vMove.x + .055f, 0);
			m_vMove.x *= 0.92f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
		}
		else
		{
			//if (m_vMove.x > 0)		m_vMove.x = max(m_vMove.x - .035f, 0);
			//else if (m_vMove.x < 0)	m_vMove.x = min(m_vMove.x + .035f, 0);
			//m_vMove.x *= 0.98f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
		}

		// ���E��Move�l
		//m_vMove.x = Math::Clamp(m_vMove.x, -m_maxSpeed, m_maxSpeed);
	}

	// �y���\�i�������ړ�
	else
	{
		m_vMove *= 0.8f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
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
	if (m_iInvincibleTime > 0)
	{
		//m_InvincibleLV = (--m_InvincibleTime <= 0) ? 0 : m_InvincibleLV;
		m_iInvincibleTime--;

		
		m_iInvincibleColRateFlame++;
		m_iInvincibleColRateFlame = m_iInvincibleColRateFlame % FLASH_SPEED;
		
		// �グ�����̃t���O�؂�ւ�
		if (m_iInvincibleColRateFlame == 0)
		{
			m_bInvincibleColRateUpFlag = (m_bInvincibleColRateUpFlag == false) ? true : false;
		}

		// �グ�t���O��ON�Ȃ炾�񂾂񔒂�
		if (m_bInvincibleColRateUpFlag == false)
		{
			m_fInvincibleColRate = ((float)(m_iInvincibleColRateFlame) / FLASH_SPEED);
		}
		else
		{
			m_fInvincibleColRate = 1.0f - ((float)(m_iInvincibleColRateFlame) / FLASH_SPEED);
		}
	

		// ���@�F�̒��� (�����̓Z���X�̖��)
		m_fInvincibleColRate *= 0.25f;	// ������Ɣ���
		m_fInvincibleColRate += 0.1f;    // ���Ǔ_�ł��Ȃ���Ԃł������ۂ�

	}
	else
	{
		// �^�C����0�ȉ��ɒB�����炨���܂��@������
		m_iInvincibleLV = 0;
		m_iInvincibleTime = 0;

		m_fInvincibleColRate = 0.0f;
		m_iInvincibleColRateFlame = 0;
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

			// ���I�[���̃p�[�e�B�N��
			ParticleManager::EffectOverDrive(Vector3(GetPos().x, GetPos().y, -0.5f));	// �኱��O

			break;
		case OVERDRIVE_TYPE::BURST:
			// ���F���I�[���̃p�[�e�B�N��
			ParticleManager::EffectOverDriveBurst(Vector3(GetPos().x, GetPos().y, -0.5f));	// �኱��O

			break;
		default:
			break;
		}

		// �������o�����̂Ƃ��̓Q�[�W�����܂�Ȃ���!!!!!!!
		m_OverDriveGage = 0;

		m_OverDriveFrame--;
		// ���̃o�[�X�g�Ȃ�Q�[�W����������
		if (m_OverDriveType== OVERDRIVE_TYPE::BURST)
		{
			m_OverDriveFrame -= 1;
		}

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

	// �������L�[����
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_iInputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_iDeviceID);

		// �����Ă���I
		if (m_iInputList[i] & 0x01)
		{
			m_wCommandHistory[0] |= (int)c_BitList[i];
		}
	}

	// �X�e�B�b�N�̎擾
	float x, y;
	tdnInput::GetAxisXYf(&x, &y, m_iDeviceID);
	
	static const float gosa(.2f);
	if (x > .5f - gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::RIGHT;
	}
	else if (x < -.5f + gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::LEFT;
	}
	if (y > .5f - gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::DOWN;
	}
	else if (y < -.5f + gosa)
	{
		m_wCommandHistory[0] |= (int)PLAYER_COMMAND_BIT::UP;
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
	if (m_iHitStopFrame <= 0)
	{
		//if (m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false)// �y���\�i����������Ȃ�������ړ�
		{
			// ���W�X�V
			m_vPos += m_vMove;
		}
	}
}

// ����{�`��
void BasePlayer::Render()
{
	// �X�^���h�`��
	m_pStand->Render(shaderM, "Persona");

	// �V�F�[�_�֑���f�[�^
	ShaderSendParam();


	// �悾��30�R�X�g
	m_pObj->Render(shaderM, "PlayerToon");
	
	// ���G�Ȃ���Z�ŏd�˂ĕ`��
	//if (m_iInvincibleTime > 0) m_pObj->Render(RS::ADD);
	
	// �����Ō��݂̃X�e�[�g�}�V���̏�Ԃ��m�F
	if (m_iDeviceID == 0)
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
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();
	m_pPanelEffectMGR->Render();	
	Vector2 vScreenPos = Math::WorldToScreen(m_vPos);// (TODO)���̃|�W�V�����̍��W���g��
	m_pThrowMark->Render((int)vScreenPos.x - 56, (int)vScreenPos.y - 324, RS::COPY_NOZ);

	

#ifdef _DEBUG
	// �����Ō��݂̃X�e�[�g�}�V���̏�Ԃ��m�F
	if (m_iDeviceID == 0)
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


		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::LEFT)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::RIGHT)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::UP)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::DOWN)
		{
			c = "��";
		}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "��";
		//}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "��";
		//}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "�^";
		}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "�_";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		{
			c = "�~";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::B)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::C)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::D)
		{
			c = "��";
		}
		
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L1)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R1)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L2)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R2)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R3)
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
		tdnFont::RenderString(c, "���C���I", 32, 10 + (i % 64) * 36, 610 + (i / 32 * 32) + ((int)m_side * 32), 0xffffffff, RS::COPY);

	}

	// ����̕`��
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), &m_tagCharacterParam.HitSquare, sizeof(CollisionShape::Square));
	square.pos += m_vPos;

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
			square.pos += m_vPos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}

	Vector2 pos2d = Math::WorldToScreen(m_vPos);
	if (m_bAI)
	{
		tdnText::Draw((int)pos2d.x - 20, (int)pos2d.y - 128, 0xffffffff, "�d��: %d", m_iRecoveryFrame);
	}

#endif

	// �f�o�b�O
	//tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	//tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	//tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "�X�^���h�Q�[�W : %d", m_pStand->GetStandGage());
	//tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "�X�^���h�X�g�b�N: %d", m_pStand->GetStandStock());
	
	//tdnText::Draw(32 + m_deviceID * 250, 430, 0xff00ff80, "OD �Q�[�W: %d", m_OverDriveGage);
	//tdnText::Draw(32 + m_deviceID * 250, 460, 0xffff8000, "OD�c�莞��: %d", m_OverDriveFrame);
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
	if (m_iInvincibleTime > 0) m_pObj->Render(RS::ADD);

	if (m_iDeviceID == 3)
	{
		m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��
		m_pAI->GetFSM()->Render();// AI�X�e�[�g�}�V���ł̕`��
	}
	

	// �G�t�F�N�g�}�l�[�W���[�`��
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();

#ifdef _DEBUG
	// ����̕`��
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), &m_tagCharacterParam.HitSquare, sizeof(CollisionShape::Square));
	square.pos += m_vPos;

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
			square.pos += m_vPos;

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
	m_pPanelEffectMGR->Render3D();
	m_pUVEffectMGR->Render();

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

// �V�F�[�_�ɑ���f�[�^
void BasePlayer::ShaderSendParam()
{
	// [1206] (�œK��)�@10->5
	// [1206] �œK���̂��߃��[�g�̑���񐔂����炷���߂܂Ƃ߂đ��邱�Ƃ�
	// ���̐l�����ĉ�����Ă�̂��킩��Ȃ��Ǝv���̂ŏ��������肵������ǂ��܂��B
	Vector3 l_vColDesc = VECTOR_ZERO;
	Vector3 l_vColDesc2 = VECTOR_ZERO;

	// ���G�̔��F�`�J�`�J���[�g�𑗂�
	//shaderM->SetValue("g_InvincibleColRate", m_fInvincibleColRate);
	l_vColDesc.x = m_fInvincibleColRate;

	// ���G�Z�̃I�����W�F�𑗂�
	//shaderM->SetValue("g_OrangeColRate", m_fOrangeColRate);
	l_vColDesc.y = m_fOrangeColRate;

	// ���G�Z�̃}�[���^�F�𑗂�
	//shaderM->SetValue("g_MagentaColRate", m_fMagentaColRate);
	l_vColDesc.z = m_fMagentaColRate;


	// �o�����Ă��邩// �o�����̃J���[
	if (m_bOverDrive == true ||
		m_pStateMachine->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) ||
		m_pStateMachine->isInState(*BasePlayerState::HeavehoDriveOverFlow::GetInstance())
		)
	{
		//shaderM->SetValue("g_OverDriveColRate", 1.0f);
		//shaderM->SetValue("g_WillPowerRate", 0.0f);
		l_vColDesc2.x = 1.0f;
		l_vColDesc2.y = 0.0f;
	}
	else
	{
		shaderM->SetValue("g_OverDriveColRate", 0.0f);
		l_vColDesc2.x = 0.0f;

		// ������Ԃ�
		if (m_bWillPower == true)
		{
			//shaderM->SetValue("g_WillPowerRate", 1.0f);
			l_vColDesc2.y = 1.0f;
		}
		else
		{
			//shaderM->SetValue("g_WillPowerRate", 0.0f);
			l_vColDesc2.y = 0.0f;
		}

	}

	// �T�C�h�ɉ����ĉ����̐F��ς���B
	//Vector3 vEdgeCol = VECTOR_ZERO;
	if (m_side == SIDE::LEFT)
	{
		//vEdgeCol = Vector3(0.95f, 0.1f, 0.0f);
		l_vColDesc2.z = 1.0f;
	}
	else
	{
		//vEdgeCol = Vector3(0.0f, 0.65f, 1.0f);
		l_vColDesc2.z = 0.0f;
	}
	//shaderM->SetValue("g_EdgeColor", vEdgeCol);

	// �܂Ƃ߂ăV�F�[�_��
	shaderM->SetValue("g_PlayerColDesc", l_vColDesc);
	shaderM->SetValue("g_PlayerColDesc2", l_vColDesc2);

}

void BasePlayer::RenderCommandFrame(int x, int y)
{
	// �R�}���h�����̕`��
	FOR(c_COMMAND_FRAME_MAX)
	{
		char *c = "";

		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::LEFT)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::RIGHT)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::UP)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::DOWN)
		{
			c = "��";
		}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "��";
		//}
		//if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
		//	m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::UP))
		//{
		//	c = "��";
		//}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::LEFT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "�^";
		}
		if (m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::RIGHT) &&
			m_wCommandHistory[i] & ((int)PLAYER_COMMAND_BIT::DOWN))
		{
			c = "�_";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::A)
		{
			c = "�~";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::B)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::C)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::D)
		{
			c = "��";
		}

		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L1)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R1)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::L2)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R2)
		{
			c = "��";
		}
		if (m_wCommandHistory[i] & (int)PLAYER_COMMAND_BIT::R3)
		{
			c = "��";
		}

		tdnFont::RenderString(c, "���C���I", 34, x + ((i % 64) * 44), y, 0xffffffff, RS::COPY);
	}

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
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::HIT);
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);

		// �����Move�l�l��
		float z = atan2(-m_vMove.x, m_vMove.y);
		//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE, 1, 1, Vector3(0, supportAngleY, z), Vector3(0, supportAngleY, z));

		// �ђʔg
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_IMPACT,
			1, 2, Vector3(0, 0, z), Vector3(0, 0, z));

		// �q�b�g�����O
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_RING, 2, 3, Vector3(0, supportAngleY, z), Vector3(0, supportAngleY, z));

		// 
		PointLightMgr->AddPointLight(pos + Vector3(0, 5, 0), Vector3(1.4f, 0.8f, 0.0f), 20, 4, 20, 4, 15);// �|�C���g���C�g�G�t�F�N�g�I
		Vector3 FlyVector(m_vMove);
		FlyVector.Normalize();
		ParticleManager::EffectHit(pos, FlyVector);

		
		// �u���\�G�t�F�N�g
		DeferredManagerEx.SetRadialBlur(m_vPos, 3.25f);

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
		m_pPanelEffectMGR->AddEffect(pos + Vector3(0, 5, 0), PANEL_EFFECT_TYPE::ClEAR);


		break;
	case EFFECT_TYPE::PERSONA:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PERSONA,
			1, 2.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectPersonaTrigger(pos);
		PointLightMgr->AddPointLight(pos + Vector3(0, 3, 0), Vector3(0, .4f, 2.0f), 50, 4, 40, 10, 30);// �|�C���g���C�g�G�t�F�N�g�I

		break;
	case EFFECT_TYPE::DROP_IMPACT:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::IMPACT,
			1.0f, 1.35f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::SHOCK_WAVE,
			1.0f, 1.55f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		break;
	case EFFECT_TYPE::UPPER:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::UPPER,
			1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));

		break;

	case EFFECT_TYPE::FINISH_HIT:
		ParticleManager::EffectFinish(pos);

		break;
	case EFFECT_TYPE::BURST:
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST:
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::ONEMORE_BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST_START:

		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST_PREV);

		//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::BURST_BALL, 0.9, 0.75
		//		, Vector3(0, 0, 0), Vector3(0, 0, 0));

		m_pUVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0));
		m_pUVEffectMGR->AddEffect(pos + convAddPos + Vector3(0, -5, 0), UV_EFFECT_TYPE::CONV2, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 4);
		m_pUVEffectMGR->AddEffect(pos + convAddPos + Vector3(-0.5, -7, 0), UV_EFFECT_TYPE::CONV3, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 8);
		m_pUVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV4, 1.5f, 1.5f
			, Vector3(0, 0, 0), Vector3(0, 0, 0), 12);
		break;
	case EFFECT_TYPE::RUN:
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir != DIR::LEFT));

		break;
	case EFFECT_TYPE::BACK_STEP:
	{
		float l_fAngle = (m_dir == DIR::LEFT) ? 0.0f : PI;

		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, l_fAngle, 0), Vector3(0, l_fAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir == DIR::LEFT));

	}	break;
	case EFFECT_TYPE::GUARD_BREAK:
		m_pPanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::GLASS);

		break;
	case EFFECT_TYPE::AIROU_DRILL:
	{
									 int delayTimer = 15;// �`�b�x��
									 m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_DRILL,
										 1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0), delayTimer);

	}	break;
	case EFFECT_TYPE::AIROU_CIRCLE:
	{
									  int delayTimer = 0;// �`�b�x��
									  m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::AIROU_CIRCLE,
										  3.0f, 5.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, -3), delayTimer);

	}	break;
	case EFFECT_TYPE::GUARD_WAVE:
	{
		Vector3 l_Pos = pos;
		l_Pos = pos;
		if (AttackVec.y >= 0.1f)// �������̃x�N�g���Ȃ�
		{
			l_Pos.y += 5; //�@�K�[�h�G�t�F�N�g�����ɏグ��
		}
		// ����Ǝ����̃x�N�g���l���l��
		float z = atan2(-AttackVec.x, AttackVec.y);

		// [12/04] �̂������̗�����Ȃ����߂�Move�l�l��
		l_Pos.x += (m_vMove.x * 7.0f);

		// �K�[�h�E�F�\�u
		m_pUVEffectMGR->AddMultipleEffect(l_Pos, UV_EFFECT_MULTIPLE_TYPE::GUARD_WAVE, 0.5f, 0.5f, Vector3(0, -supportAngleY*1.5f, z), Vector3(0, -supportAngleY*1.5f, z));
		m_pUVEffectMGR->AddMultipleEffect(l_Pos, UV_EFFECT_MULTIPLE_TYPE::GUARD_GRID, 0.2f, 0.5f, Vector3(0, -supportAngleY*1.5f, z), Vector3(0, -supportAngleY*1.5f, z));

		// ����
		PointLightMgr->AddPointLight(l_Pos + Vector3((m_vMove.x * 2.0f) , 1.5f, -1), Vector3(0, 0.35f, 0.65f), 18, 12, 12, 3, 6);// �|�C���g���C�g�G�t�F�N�g�I

		// ��юU��p�[�e�B�N��
		ParticleManager::EffectGuard(l_Pos, AttackVec);

		// [1204] �K�[�h�G�t�F�N�g�u���b�V���A�b�v���� 

	}	break;
	case EFFECT_TYPE::MULTIPLE_HIT:
	{
		// �q�b�g���b�v
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddMultipleEffect(pos, UV_EFFECT_MULTIPLE_TYPE::HIT_RIP, 1.0f, 2.0f, Vector3(ram, -supportAngleY*1.5f, 0), Vector3(ram, -supportAngleY*1.5f, 0));
		
		float slashRam = tdnRandom::Get(-3.14f, 3.14f);
		m_pUVEffectMGR->AddMultipleEffect(pos + Vector3(0, 0, -5) , UV_EFFECT_MULTIPLE_TYPE::HIT_SLASH, 2.0f, 4.0f, Vector3(0, 0, slashRam), Vector3(0, 0, slashRam));

		// �u���\�G�t�F�N�g
		DeferredManagerEx.SetRadialBlur(m_vPos, 2.55f);

	}	break;
	case EFFECT_TYPE::OVER_DRIVE_ACTION:
	{

		m_pPanelEffectMGR->AddEffect
			(pos, PANEL_EFFECT_TYPE::OVER_DRIVE_START);

		// OVER_DRIVE_RING
		m_pPanelEffectMGR->AddEffect
			(pos, PANEL_EFFECT_TYPE::OVER_DRIVE, 7);

		// OVER_DRIVE_RING
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::OVER_DRIVE_RING, 0.5f, 3.5f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 7);

	}	break;
	case EFFECT_TYPE::JUMP:
	{
		// �W�����v�G�t�F�N�g
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
		// �W�����v�G�t�F�N�g��
		// �����Move�l�l��
		float z = atan2(-m_vMove.x, m_vMove.y);
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, z), Vector3(0, 0, z));


	}	break;
	case EFFECT_TYPE::AERIAL_JUMP:
	{

		float diaAngle = (m_vMove.x >= 0) ? PI / 2 : -PI / 2;

		// �W�����v�G�t�F�N�g
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, diaAngle), Vector3(0, 0, diaAngle));
		// �W�����v�G�t�F�N�g��
		// �����Move�l�l��
		//float z = atan2(-m_vMove.x, m_vMove.y);
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, diaAngle), Vector3(0, 0, diaAngle));


	}	break;
	case EFFECT_TYPE::ESCAPE:
	{
		float ecpAngle = (m_dir == DIR::RIGHT) ? PI / 2 : -PI / 2;

		// �W�����v�G�t�F�N�g
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.25f, 0.5f, Vector3(0, 0, ecpAngle), Vector3(0, 0, ecpAngle));
		// �W�����v�G�t�F�N�g��
		// �����Move�l�l��
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_SPEED_LINE, 0.5f, 0.35f, Vector3(0, 0, ecpAngle), Vector3(0, 0, ecpAngle));


		// ��
		m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir != DIR::LEFT));

	}	break;
	case EFFECT_TYPE::INVINCIBLE_ATTACK:
	{
		// �v���b�V���[
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));
	
		// �I�����W�̌�
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 10, -4) , PANEL_EFFECT_TYPE::ORANGE_LIGHT, 0);

		// �I�����W�̃����O
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddEffect(pos + Vector3(0, 10, -2), UV_EFFECT_TYPE::ORANGE_BURST, 0.1f, 1.75f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 0);

		// �L�����N�^�[���̂��I�����W��
		m_fOrangeColRate = 1.0f;
	}	break;
	case EFFECT_TYPE::DOKKOI:
	{
		// �v���b�V���[
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// �ǂ������̌�
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 8, -2), PANEL_EFFECT_TYPE::DOKKOI, 0);

		// �L�����N�^�[���̂��}�[���^��
		m_fMagentaColRate = 1.0f;

	}	break;
	case EFFECT_TYPE::MUZZLE_FLASH:
	{
		// �}�Y���t���b�V��
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::MUZZLE_FLASH, 0.90f, 1.0f, Vector3(0, 0, zAngle), Vector3(0, 0, zAngle));

		// ��юU���
		//m_PanelEffectMGR->AddEffect
		//	(pos + Vector3(0, 8, -2), PANEL_EFFECT_TYPE::DOKKOI, 0);

	}	break;
	case EFFECT_TYPE::THROW:
	{
		// �����̌�
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 8, -4), PANEL_EFFECT_TYPE::INEFFECT_MINI, 0);

	}	break;
	case EFFECT_TYPE::WILL_POWER:
	{
		// �v���b�V���[
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PRESSURE, 0.5f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// �Ԃ���
		m_pPanelEffectMGR->AddEffect
			(pos + Vector3(0, 10, -4), PANEL_EFFECT_TYPE::RED_RIGHT, 0);

		// �Ԃ̃����O
		float ram = tdnRandom::Get(-1.57f, 1.57f);
		m_pUVEffectMGR->AddEffect(pos + Vector3(0, 10, -2), UV_EFFECT_TYPE::RED_BURST, 0.1f, 1.85f,
			Vector3(ram, -supportAngleY*1.5f, 0),
			Vector3(ram, -supportAngleY*1.5f, 0), 0);

		// �Ԃ��g��
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RED_WAVE, 0.75f, 1.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}	break;
	case EFFECT_TYPE::DROP_IMPACT_SUPER:
	{
		// �v���b�V���[
		m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::PRESSURE, 1.5f, 2.0f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		// ��
		m_pUVEffectMGR->AddEffect(m_vPos + Vector3(0, -5, 0) , UV_EFFECT_TYPE::CYCLONE_SHORT, 1.15f, 2.35f);

		// �g��
		m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::WAVE, 1.0f, 1.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}	break;
	case EFFECT_TYPE::CYCLONE_RUN:
	{
		// �_�b�V���G�t�F�N�g
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			1.3f, 1.5f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_vPos, (m_dir != DIR::LEFT));

		// �x�N�g���l���l��
		Vector3 v = m_vMove;
		v.Normalize();
		float z = (m_vMove.x >= 0) ? PI / 2 : -PI / 2;

		// ��
		m_pUVEffectMGR->AddEffect(pos + Vector3(0, 5, 1), UV_EFFECT_TYPE::CYCLONE_SHORT, 1.0f, 1.2f, Vector3(0, 0, z), Vector3(0, 0, z));

		// �g��
		//m_pUVEffectMGR->AddEffect(m_vPos, UV_EFFECT_TYPE::WAVE, 1.0f, 1.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
		// �W�����v�G�t�F�N�g
		m_pUVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::JUMP_WAVE, 0.55f, 0.8f, Vector3(0, 0, z), Vector3(0, 0, z));

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
	m_pUVEffectMGR->AddEffectRoop(GetCenterPos(), UV_EFFECT_TYPE::GUARD);

	m_pPanelEffectMGR->AddEffect(GetCenterPos()+Vector3(0,0,-5), PANEL_EFFECT_TYPE::GUARD);

}

void BasePlayer::GuardEffectStop()
{
	// �K�[�h�G�t�F�N�g�I��
	m_pUVEffectMGR->StopEffectRoop(UV_EFFECT_TYPE::GUARD);
}

void BasePlayer::GuardEffectUpdate()
{
	// �L�����N�^�[�ɒǏ]
	// �K�[�h�G�t�F�N�g�X�V
	m_pUVEffectMGR->GetBaseUVEffect(UV_EFFECT_TYPE::GUARD)->SetPos(GetCenterPos());
}

void BasePlayer::WillPowerUpdate(PLAYER_UPDATE updateFlag)
{	
	// ����ł��鎞�Ȃ��
	if (updateFlag != PLAYER_UPDATE::CONTROL_OK) return;	
	
	// HP�̃��[�g���v�Z
	float HpRate = (float)m_iHP/ m_tagCharacterParam.iMaxHP;

	// �̗͂�35%�ȉ��ɂȂ�΍�������
	if (m_bWillPower == false)
	{
		/*******************/
		//	 �����l�����O
		/*******************/

		// ����łȂ���Ԃł���HP��35%�ȏ�Ȃ�
		if (HpRate <= 0.35f &&
			GetFSM()->isInState(*BasePlayerState::Die::GetInstance()) == false /*&& 
			GetFSM()->isInState(*BasePlayerState::Die::GetInstance()) == false*/)
		{
			// ���J�o���[�t���[�����؂ꓮ����悤�ɂȂ����u��+
			// �_�E���t���OFalse�̎����g���K�[
			if (m_iRecoveryFrame <= 0 && m_bDown == false)
			{
				// ��������
				m_bWillPower = true;

				// SP��25��
				m_OverDriveGage += 25;

				// ���o
				AddEffectAction(m_vPos, EFFECT_TYPE::WILL_POWER);
				
				// SP�Q�[�W�㏸�t���[��������
				m_iSpGettingFrame = 0;

				// �I�[������
				m_pUVEffectMGR->AddEffectRoop(GetCenterPos(), UV_EFFECT_TYPE::WILL_POWER_AURA);

			}

		}
	}
	else
	{
		/*******************/
		//	 �����l������
		/*******************/

		// HP��������ƍ������Ȃ��Ȃ�@�܂��́@���񂾂�؂��
		if (HpRate > 0.35f || GetFSM()->isInState(*BasePlayerState::Die::GetInstance()) == true)
		{
			m_bWillPower = false;
		}
		// �I�[���I��
		m_pUVEffectMGR->StopEffectRoop(UV_EFFECT_TYPE::WILL_POWER_AURA);
	
		m_iSpGettingFrame++;
		if (m_iSpGettingFrame >= 60)//1202
		{
			m_iSpGettingFrame = 0;
			m_OverDriveGage += 1;// SP�Q�[�W�𑫂�
		}

	}

	// �L�����N�^�[�ɒǏ]
	// �I�[���G�t�F�N�g�X�V
	m_pUVEffectMGR->GetBaseUVEffect(UV_EFFECT_TYPE::WILL_POWER_AURA)->SetPos(GetCenterPos());

	//throw gcnew System::NotImplementedException();

}
