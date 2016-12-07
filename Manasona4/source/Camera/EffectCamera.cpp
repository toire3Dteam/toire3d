#include "TDNLIB.h"
#include "camera.h"
#include "EffectCamera.h"
#include "../BaseEntity/Message/MessageDispatcher.h"


//*****************************************************************************************************************************
//
//		�������������J�����N���X
//
//*****************************************************************************************************************************

//=============================================================================================
//		��	��	��
EffectCamera::EffectCamera(ViewData *pViewData, LPCSTR filename, const Vector3 &OrgPos) :m_pViewData(pViewData), m_CurrentScriptID(0), m_CurrentFrame(0), m_PatternCursor(0), m_EventFrame(0), m_EventCursor(0), m_bAction(false), m_vOrgPos(OrgPos)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�J�����X�N���v�g�����ĂȂ���I");

	while (!ifs.eof())
	{
		TimeLineCamera *set = new TimeLineCamera;


		char skip[128];	// �ǂݔ�΂��p�ϐ�
		// �X�N���v�g���ǂݔ�΂�
		ifs >> skip;

		MyAssert(skip[0] == '[', "�J�����X�N���v�g�̃e�L�X�g������Ă�\n%d�Ԗڕt�߂������������H", m_list.size());

		// �X�N���v�gID�擾
		ifs >> skip;
		ifs >> set->ID;

		// �p�^�[���̌��擾
		ifs >> skip;
		ifs >> set->NumPattern;
		assert(set->NumPattern > 0);
		set->pPatterns = new TimeLineCamera::Pattern[set->NumPattern];

		// �p�^�[���S��
		for (int i = 0; i < set->NumPattern; i++)
		{
			// �I���t���[���擾
			ifs >> skip;
			ifs >> set->pPatterns[i].EndFrame;

			// �t���[�����x�擾
			ifs >> skip;
			ifs >> set->pPatterns[i].FrameSpeed;

			// ���W�ƒ����_�̃x�W�G�_�̌�
			ifs >> skip;
			ifs >> set->pPatterns[i].NumPoints;
			assert(set->pPatterns[i].NumPoints > 0);
			//set->pPatterns[i].pPosArray = new Vector3[set->pPatterns[i].NumPoints];
			//set->pPatterns[i].pTargetArray = new Vector3[set->pPatterns[i].NumPoints];
			set->pPatterns[i].InstanceArrays();	// ��������NuMPoints���̔z��|�C���^�̊m�ۂ��s���Ă���

			for (int j = 0; j < set->pPatterns[i].NumPoints; j++)
			{
				// ���W
				ifs >> set->pPatterns[i].pViewDataArray[j].pos.x;
				ifs >> set->pPatterns[i].pViewDataArray[j].pos.y;
				ifs >> set->pPatterns[i].pViewDataArray[j].pos.z;

				// �����_
				ifs >> set->pPatterns[i].pViewDataArray[j].target.x;
				ifs >> set->pPatterns[i].pViewDataArray[j].target.y;
				ifs >> set->pPatterns[i].pViewDataArray[j].target.z;

				// roll�l
				ifs >> set->pPatterns[i].pViewDataArray[j].roll;
			}

			// �Œ肩�ǂ���
			set->pPatterns[i].fix = (set->pPatterns[i].NumPoints == 1);
		}

	// �J�����U���C�x���g
	int NumShakeEvent(0);
	ifs >> skip;
	ifs >> NumShakeEvent;

	set->PartitioningShakeEvent(NumShakeEvent);

	FOR(NumShakeEvent)
	{
		// �����t���[��
		ifs >> set->pShakeEventArray[i].TriggerFrame;
		// �U���̗�
		ifs >> set->pShakeEventArray[i].ShakeInfo.ShakePower;
		// �U������
		ifs >> set->pShakeEventArray[i].ShakeInfo.ShakeFrame;
	}

		ifs >> skip;	// END�ǂݔ�΂�

		// ���X�g�ɓ˂�����
		m_list.push_back(set);
	}
}

//
//=============================================================================================


//=============================================================================================
//		�J		��
EffectCamera::~EffectCamera()
{
	// ���X�g�̊J��
	for (auto it : m_list) delete it;
}
//
//=============================================================================================


//=============================================================================================
//		�X		�V
Vector3 operator * (const Vector3 &v, const Matrix &m)
{
	Vector3 ret;
	ret.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
	ret.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
	ret.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);
	return ret;
}
void Rot2D(float rad, float *x, float *y)
{
	float ret_x = (*x * cosf(rad)) - (*y * sinf(rad));
	float ret_y = (*x * sinf(rad)) + (*y * cosf(rad));
	*x = ret_x;
	*y = ret_y;
}

void EffectCamera::Update()
{
	// �͂���
	if (!m_bAction) return;

	// �C�x���g���ԍX�V
	m_EventFrame++;
	if (m_EventCursor < m_list[m_CurrentScriptID]->NumShakeEvent)
	{
		if (m_EventFrame == m_list[m_CurrentScriptID]->pShakeEventArray[m_EventCursor].TriggerFrame)
		{
			// �U�����b�Z�[�W���M
			MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &m_list[m_CurrentScriptID]->pShakeEventArray[m_EventCursor].ShakeInfo);
			m_EventCursor++;
		}
	}

	// �J�����t���[���X�V+�p�^�[�����ł̏I���t���[�����B������
	if ((m_CurrentFrame += m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].FrameSpeed) > m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame)
	{
		m_CurrentFrame = 0;

		// ���̃p�^�[���ֈڍs�A�����p�^�[��������������I��
		if (++m_PatternCursor >= m_list[m_CurrentScriptID]->NumPattern)
		{
			//m_pCamera->OffEffectCamera();	// �X�N���v�g����
			m_bAction = false;
			return;
		}
	}

	// �J�������W�X�V
	ViewData data;
	GetTimeLineViewData(&data);

	// �v���C���[���W�n�ɂ���
	//Rot2D(m_pCamera->m_angle.y, &data.pos.z, &data.pos.x);
	data.pos += m_vOrgPos;
	data.target += m_vOrgPos;

	// �J�����ɃZ�b�g
	*m_pViewData = data;
	//m_pCamera->Set(pos, target);
}
//
//=============================================================================================

//=============================================================================================
//		�s	��	�X�^�[�g
void EffectCamera::Start(int ID)
{
	MyAssert(ID >= 0 && ID < (int)m_list.size(), "�G���[: �Ӑ}����Ȃ�ID���ݒ肳��܂����Bby�G�t�F�N�g�J����");
	m_CurrentScriptID = ID;
	m_PatternCursor = 0;
	m_CurrentFrame = 0;
	m_EventFrame = 0;
	m_EventCursor = 0;
	m_bAction = true;
}
//
//=============================================================================================



void EffectCamera::GetTimeLineViewData(ViewData *out)
{
	if (m_PatternCursor == -1) return;
	if (m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].fix)
	{
		*out = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pViewDataArray[0];
		return;
	}

	const float percentage = m_CurrentFrame / m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame;	// ���݃t���[����percentage

	// �x�W�G�v�Z�֐��Ɋۓ���
	Math::Bezier(
		out,																	// �󂯎M
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pViewDataArray,					// �n�_�A���ԁA�I�_�������Ă���W�z��
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// �̗v�f��
		percentage
		);
}