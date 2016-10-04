#include "TDNLIB.h"
#include "../BaseEntity/Message/Message.h"
#include "camera.h"
#include "EffectCamera.h"
#include "../Stage/Stage.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"

//*****************************************************************************************************************************
//
//		�J�����N���X
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//	�������E���
//*****************************************************************************************************************************
//------------------------------------------------------
//	������
//------------------------------------------------------
Camera::Camera() :BaseGameEntity(ENTITY_ID::CAMERA_MGR),	// �G���e�B�e�BID�̓o�^
m_angle(Vector3(0, 0, 0)), m_pos(Vector3(0, 20, -100)), m_ipos(m_pos), m_target(Vector3(0, 0, 0)), m_OrgPos(Vector3(0,0,0)),
m_NumPlayer(-1), m_pPlayerPosReferences(nullptr)
{
	//	�r���[�ݒ�
	tdnView::Init();
	m_projection.fovY = D3DX_PI / 4;
	m_projection.Near = .1f;
	m_projection.Far = 2000.0f;

	m_MaxDist = 25.0f;
	m_MinDist = 22.0f;

	m_pEffectCamera = new EffectCamera(this);

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<Camera>(this);
	m_pStateMachine->SetGlobalState(GlobalCameraState::GetInstance());	// �O���[�o���X�e�[�g�̐ݒ�
}

void Camera::InitializeResult()
{
	if (!m_pStateMachine->GetCurrentState()) m_pStateMachine->SetCurrentState(EffectCameraState::GetInstance());
	m_pEffectCamera->Start(0);
}

void Camera::SetPlayersPos()
{
	// �v���C���[���擾
	m_NumPlayer = PlayerMgr->GetNumPlayer();

	// �X�}�u���J�����p�Ƀv���C���[�B�̍��W�̃A�h���X���擾����
	if (m_pPlayerPosReferences) delete[] m_pPlayerPosReferences;	// �����̃o�b�t�@�̉��
	m_pPlayerPosReferences = new Vector3*[m_NumPlayer];
	FOR(m_NumPlayer)
	{
		// �v���C���[�̍��W���Q�Ƃ���
		m_pPlayerPosReferences[i] = PlayerMgr->GetPlayer(i)->GetPosAddress();
	}
}

void Camera::SetStageCameraInfo(char *path)
{
	std::fstream ifs(path);
	MyAssert(ifs, "�X�}�u���J�����̃e�L�X�g�p�X�Ԉ���Ă��I");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	// �J�����^�C�v�ǂݍ���
	ifs >> skip;
	char cType[16];
	ifs >> cType;
	m_CameraData.bSumabura = (strcmp(cType, "SUMABURA") == 0);	// �X�}�u�����ǂ���

	// XY�͈̔�
	ifs >> skip;
	ifs >> m_CameraData.FullRange.x;
	ifs >> m_CameraData.FullRange.y;

	// YZ�̉������
	ifs >> skip;
	ifs >> m_CameraData.FullZ;
	ifs >> skip;
	ifs >> m_CameraData.FullY;

	// XY�̍ő�l
	ifs >> skip;
	ifs >> m_CameraData.MoveMax.x;
	ifs >> m_CameraData.MoveMax.y;

	// XY�̍ŏ��l
	ifs >> skip;
	ifs >> m_CameraData.MoveMin.x;
	ifs >> m_CameraData.MoveMin.y;

	// �X�}�u���J�����X�e�[�W�Ȃ�
	if (m_CameraData.bSumabura)
		m_pStateMachine->SetCurrentState(SumaburaCameraState::GetInstance());
	else assert(0);
}

//------------------------------------------------------
//	���
//------------------------------------------------------
Camera::~Camera()
{
	if(m_pPlayerPosReferences) delete[] m_pPlayerPosReferences;
	delete m_pStateMachine;
	delete m_pEffectCamera;
}

//*****************************************************************************************************************************
//		�X�V
//*****************************************************************************************************************************
//------------------------------------------------------
//		�X�V
//------------------------------------------------------
void Camera::Update()
{
	//	���[�h���Ƃ̏���
	m_pStateMachine->Update();

	// ���W�ƒ����_�ݒ�(�O���[�o���X�e�[�g�ł��)
	//tdnView::Set(m_pos, m_target);

	// ���e�ݒ�
	tdnView::SetProjection(m_projection.fovY, m_projection.Near, m_projection.Far);
}

//*****************************************************************************************************************************
//	�`�揀��
//*****************************************************************************************************************************
void Camera::Activate()
{
	tdnView::Activate();
	tdnView::Clear();

	//Text::Draw(32, 64, 0xffffff00, "CameraFar : %.1f", projection.Far);
	//Text::Draw(32, 128, 0xffffff00, "CameraFov : %.2f", projection.fovY);

	//if (KeyBoard(KB_NUMPAD4)) m_angle.z += .05f;
	//if (KeyBoard(KB_NUMPAD6)) m_angle.z -= .05f;
	//if (KeyBoard(KB_3)) projection.fovY -= .01f;
	//if (KeyBoard(KB_4)) projection.fovY += .01f;
}

bool Camera::HandleMessage(const Message &msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void Camera::OnEffectCamera(int id)
{
	//m_pStateMachine->SetCurrentState();	// �G�t�F�N�g�J�����ɐ؂�ւ�
	m_pStateMachine->ChangeState(EffectCameraState::GetInstance());
	m_pEffectCamera->Start(id);											// �G�t�F�N�g�J�����X�^�[�g
}

void Camera::OffEffectCamera()
{
	//m_pStateMachine->SetCurrentState(m_pStateMachine->GetPreviousState());	// 1�O�̃X�e�[�g�ɖ߂�
	m_pStateMachine->RevertToPreviousState();
	m_angle.z = 0;
}


//*****************************************************************************************************************************
//
//		�J�����U��
GlobalCameraState::ShakeData::ShakeData() :power(0), MaxPower(0),frame(0), MaxFrame(0), ShakedPos(0, 0, 0), ShakedTarget(0.0f, 0.0f, 0.0f),
rate(5.0f)	// �U���̋����(rate * 0�`1.0f)
{}



void GlobalCameraState::ShakeData::Start(float power, unsigned int frame)
{
	this->power = MaxPower = (power * rate);
	this->frame = MaxFrame = frame;
}

void GlobalCameraState::ShakeData::Update(Camera *camera)
{
	ShakedTarget = camera->m_target;
	ShakedPos = camera->m_pos;

	if (MaxFrame == 0) return;

	if (frame > 0)
	{
		power = MaxPower * (frame / (float)MaxFrame);
		const float x(tdnRandom::Get(-1.0f, 1.0f) * power), y(tdnRandom::Get(-1.0f, 1.0f) * power);
		ShakedPos.x += x;
		ShakedPos.y += y;
		ShakedTarget.x += x;
		ShakedTarget.y += y;

		if (--frame <= 0)
		{
			MaxFrame = 0;
			power = MaxPower = 0.0f;
		}
	}
}
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//		�O���[�o���X�e�[�g
void GlobalCameraState::Enter(Camera *pCamera)
{

}

void GlobalCameraState::Execute(Camera *pCamera)
{
	// �J�����U���X�V
	m_ShakeData.Update(pCamera);

	// ���W�ƒ����_�̐ݒ�	���O���[�o���X�e�[�g�ł��
	tdnView::Set(pCamera->m_pos, m_ShakeData.ShakedTarget, pCamera->m_angle.z);
}

// �o��
void GlobalCameraState::Exit(Camera *pCamera)
{

}

bool GlobalCameraState::OnMessage(Camera *pCamera, const Message &msg)
{

	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SHAKE_CAMERA:	// �U�����b�Z�[�W
		// �I���W�i���\���̂��󂯎��
		SHAKE_CAMERA_INFO *info = (SHAKE_CAMERA_INFO*)msg.ExtraInfo;
		m_ShakeData.Start(info->ShakePower, info->ShakeFrame);
		break;
	}

	return false;
}
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//		�X�}�u���J����
void SumaburaCameraState::Enter(Camera *pCamera)
{

}

void SumaburaCameraState::Execute(Camera *pCamera)
{
	float MAX_X = -1000, MAX_Y = -1000, MIN_X = +1000, MIN_Y = +1000;
	//�␳���݂̕�
	FOR(pCamera->m_NumPlayer)
	{
		//�ő�X
		if (MAX_X<pCamera->m_pPlayerPosReferences[i]->x + 10)
		{
			MAX_X = pCamera->m_pPlayerPosReferences[i]->x + 10;
		}
		//�ŏ�X
		if (MIN_X>pCamera->m_pPlayerPosReferences[i]->x - 10)
		{
			MIN_X = pCamera->m_pPlayerPosReferences[i]->x - 10;
		}

		//�ő�Y
		if (MAX_Y<pCamera->m_pPlayerPosReferences[i]->y + 10)
		{
			MAX_Y = pCamera->m_pPlayerPosReferences[i]->y + 10;
		}
		//�ŏ�Y
		if (MIN_Y>pCamera->m_pPlayerPosReferences[i]->y - 10)
		{
			MIN_Y = pCamera->m_pPlayerPosReferences[i]->y - 10;
		}
	}

	MAX_X = Math::Clamp(MAX_X, pCamera->m_CameraData.MoveMin.x, pCamera->m_CameraData.MoveMax.x);
	MAX_Y = Math::Clamp(MAX_Y, pCamera->m_CameraData.MoveMin.y, pCamera->m_CameraData.MoveMax.y);

	//�@���ɕ������߂�̂Ɛ^�񒆂����߂�
	float senter_X, senter_Y;
	float range_X, range_Y;
	range_X = abs(MAX_X - MIN_X);//����X�̕�
	range_Y = abs(MAX_Y - MIN_Y);//����Y�̕�

	senter_X = (MAX_X + MIN_X) / 2;//���SX�擾
	senter_Y = (MAX_Y + MIN_Y) / 2;//���SY�擾

	float percent_X = 0.0f, percent_Y = 0.0f;

	percent_X = range_X / pCamera->m_CameraData.FullRange.x;
	percent_Y = range_Y / pCamera->m_CameraData.FullRange.y;

	//�@�p�[�Z���g�擾
	float percent = 0.0f;
	if (percent_X > percent_Y)percent = percent_X;
	else percent = percent_Y;

	//Z�l
	pCamera->m_ipos.z = (pCamera->m_CameraData.FullZ * percent) - 40;//�p�[�Z���g�ɂ���ăJ�������߂Â����� �|40�͕␳

	//�@������X��Y
	float max_move_x = pCamera->m_CameraData.MoveMax.x * (1.0f - percent);
	float max_move_y = pCamera->m_CameraData.MoveMax.y * (1.0f - percent);
	float min_move_x = pCamera->m_CameraData.MoveMin.x * (1.0f - percent);
	float min_move_y = pCamera->m_CameraData.MoveMin.y * (1.0f - percent);

	//�@�܂��͒��S��
	pCamera->m_ipos.x = senter_X;
	pCamera->m_ipos.y = senter_Y;
	pCamera->m_itarget.x = senter_X;
	pCamera->m_itarget.y = senter_Y;

	//�␳
	if (max_move_x <= pCamera->m_ipos.x)
	{
		pCamera->m_ipos.x = max_move_x;
		pCamera->m_itarget.x = max_move_x;
	}
	if (min_move_x >= pCamera->m_ipos.x)
	{
		pCamera->m_ipos.x = min_move_x;
		pCamera->m_itarget.x = min_move_x;
	}
	if (max_move_y <= pCamera->m_ipos.y)
	{
		pCamera->m_ipos.y = max_move_y;
		pCamera->m_itarget.y = max_move_y;
	}
	if (min_move_y >= pCamera->m_ipos.y)
	{
		pCamera->m_ipos.y = min_move_y;
		pCamera->m_itarget.y = min_move_y;
	}


	//Y�@�␳��
	//ipos.y += 5;
	//itarget.y += 5;

	//Y�l �␳
	pCamera->m_ipos.y += (pCamera->m_CameraData.FullY * percent) + 5;//�p�[�Z���g�ɂ���ăJ�������߂Â����� +���͕␳
	pCamera->m_itarget.y += (pCamera->m_CameraData.FullY * percent);

	// ���
	pCamera->m_pos = pCamera->m_pos * 0.9f + pCamera->m_ipos * 0.1f;
	pCamera->m_target = pCamera->m_target * .95f + pCamera->m_itarget * .05f;
}

// �o��
void SumaburaCameraState::Exit(Camera *pCamera)
{

}

bool SumaburaCameraState::OnMessage(Camera *pCamera, const Message &msg)
{

	// ���b�Z�[�W�^�C�v
	switch ((MESSAGE_TYPE)msg.Msg)
	{
	case MESSAGE_TYPE::EFFECT_CAMERA:
	{
										EFFECT_CAMERA_INFO *eci = (EFFECT_CAMERA_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
										pCamera->m_OrgPos = (eci->pOrgPos) ? *eci->pOrgPos : VECTOR_ZERO;	// ���_���W�����Ă�����
										pCamera->OnEffectCamera(eci->scriptID);								// �G�t�F�N�g�J�����J�n
										return true;
										break;
	}
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}
//
//*****************************************************************************************************************************


//*****************************************************************************************************************************
//
//		�G�t�F�N�g�J����
void EffectCameraState::Enter(Camera *pCamera)
{

}

void EffectCameraState::Execute(Camera *pCamera)
{	
	// �G�t�F�N�g�J�����X�V
	pCamera->m_pEffectCamera->Update();
}

// �o��
void EffectCameraState::Exit(Camera *pCamera)
{

}

bool EffectCameraState::OnMessage(Camera *pCamera, const Message &msg)
{

	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	return true;
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//
//		���R�ݒ�p(�\�t�g�C�}�[�W��)�J����
//void Camera::Mode::Free::Initialize(const Vector3 &pos, const Vector3 &target)
//{
//	dist = 47.0f;
//	me->slerp_percentage = 1.0f;
//	add_pos = VECTOR_ZERO;
//}
//
//void Camera::Mode::Free::Update()
//{
//	if (Get_wheel_flag() == WHEEL_FLAG::UP)
//		dist -= 4.0f;
//	else if (Get_wheel_flag() == WHEEL_FLAG::DOWN)
//		dist += 4.0f;
//
//	if (KeyBoard(MOUSE_RIGHT))
//	{
//		me->angle.y -= Get_mouse_move_x()*.01f;
//		me->angle.x -= Get_mouse_move_y()*.01f;
//	}
//	else if (KeyBoard(MOUSE_CENTAR))
//	{
//		// �ړ��ʍ쐬
//		Vector3 move =
//			Vector3(matView._11, matView._21, matView._31) * -Get_mouse_move_x()*.1f +
//			Vector3(matView._12, matView._22, matView._32) * Get_mouse_move_y()*.1f;
//		add_pos += move;
//	}
//
//	// �p�x�̒l�ɂ��x�N�g�����쐬
//	float ay_x = sinf(me->angle.y);
//	float ay_z = cosf(me->angle.y);
//	float ax_y = sinf(me->angle.x);
//	float ax_z = cosf(me->angle.x);
//
//	Vector3 vec(
//		ay_x * ax_z,
//		ax_y,
//		ay_z * ax_z);
//
//	// �x�N�g���̒�������
//	vec *= this->dist;
//
//
//	// �v���C���[���W�擾
//	Vector3 p_pos = me->my_player->GetCollisionPos();
//
//
//
//	// �p�x�̒l�̃x�N�g���ƃv���C���[����J�����ʒu�Z�o
//	me->pos.x = p_pos.x - vec.x;
//	me->pos.y = p_pos.y - vec.y;
//	me->pos.z = p_pos.z - vec.z;
//
//	me->target = p_pos;
//
//	me->view->Set(me->pos+add_pos, me->target+add_pos);
//}
//
//*****************************************************************************************************************************