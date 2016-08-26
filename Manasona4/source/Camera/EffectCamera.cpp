#include "TDNLIB.h"
#include "camera.h"
#include "EffectCamera.h"


//*****************************************************************************************************************************
//
//		�������������J�����N���X
//
//*****************************************************************************************************************************

//=============================================================================================
//		��	��	��
EffectCamera::EffectCamera(Camera *me) :m_pCamera(me), m_CurrentScriptID(0), m_CurrentFrame(0), m_PatternCursor(0)
{
	std::ifstream ifs("DATA/Camera/script.txt");
	MyAssert(ifs, "�J�����X�N���v�g�����ĂȂ���I");

	while (!ifs.eof())
	{
		TimeLineCamera *set = new TimeLineCamera;

		char skip[128];	// �ǂݔ�΂��p�ϐ�

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
				ifs >> set->pPatterns[i].pPosArray[j].x;
				ifs >> set->pPatterns[i].pPosArray[j].y;
				ifs >> set->pPatterns[i].pPosArray[j].z;

				// �����_
				ifs >> set->pPatterns[i].pTargetArray[j].x;
				ifs >> set->pPatterns[i].pTargetArray[j].y;
				ifs >> set->pPatterns[i].pTargetArray[j].z;

				// roll�l
				ifs >> set->pPatterns[i].pRollArray[j];
			}

			// �Œ肩�ǂ���
			set->pPatterns[i].fix = (set->pPatterns[i].NumPoints == 1);
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
	// �J�����t���[���X�V+�p�^�[�����ł̏I���t���[�����B������
	if ((m_CurrentFrame += m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].FrameSpeed) > m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame)
	{
		m_CurrentFrame = 0;

		// ���̃p�^�[���ֈڍs�A�����p�^�[��������������I��
		if (++m_PatternCursor >= m_list[m_CurrentScriptID]->NumPattern)
		{
			m_pCamera->OffEffectCamera();	// �X�N���v�g����
			return;
		}
	}

	// �J�������W�X�V
	Vector3 pos, target;
	float roll;
	GetTimeLineCameraPos(&pos, &target, &roll);

	// �v���C���[���W�n�ɂ���
	Rot2D(m_pCamera->m_angle.y, &pos.z, &pos.x);
	pos += m_pCamera->m_OrgPos;
	target += m_pCamera->m_OrgPos;

	// �J�����ɃZ�b�g
	m_pCamera->m_pos = pos;
	m_pCamera->m_target = target;
	m_pCamera->m_angle.z = roll;
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
}
//
//=============================================================================================



void EffectCamera::GetTimeLineCameraPos(Vector3 *OutPos, Vector3 *OutTarget, float *OutRoll)
{
	if (m_PatternCursor == -1) return;
	if (m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].fix)
	{
		*OutPos = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pPosArray[0];
		*OutTarget = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pTargetArray[0];
		*OutRoll = m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pRollArray[0];
		return;
	}

	const float percentage = m_CurrentFrame / m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].EndFrame;	// ���݃t���[����percentage

	// �x�W�G�v�Z�֐��Ɋۓ���
	Math::Bezier(
		OutPos,																	// �󂯎M
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pPosArray,					// �n�_�A���ԁA�I�_�������Ă���W�z��
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// �̗v�f��
		percentage
		);

	Math::Bezier(
		OutTarget,																	// �󂯎M
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pTargetArray,				// �n�_�A���ԁA�I�_�������Ă���W�z��
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// �̗v�f��
		percentage
		);

	Math::Bezier(
		OutRoll,																// �󂯎M
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].pRollArray,				// �n�_�A���ԁA�I�_�������Ă���W�z��
		m_list[m_CurrentScriptID]->pPatterns[m_PatternCursor].NumPoints,					// �̗v�f��
		percentage
		);
}