#include "LocusEffect.h"

//+------------------------
//	���̎c��
//+------------------------
LocusEffect::LocusEffect(const char* fileName, const int frameMax, const int separateNum)
{
	m_pReferencePosL = nullptr;
	m_pReferencePosR = nullptr;

	m_pTex = new tdn2DObj(fileName);

	m_iFrameMax = frameMax;
	m_iSeparate = separateNum;

	// �O�Ղ̍ő吔�擾
	m_iLocusMax = (m_iFrameMax - 1)*(m_iSeparate - 1) + m_iFrameMax;

	// �O�Ղ̍ő吔���z��𐶐�
	m_pPosL = new Vector3[m_iLocusMax];
	m_pPosR = new Vector3[m_iLocusMax];
	memset(m_pPosL, 0, sizeof(m_pPosL));
	memset(m_pPosR, 0, sizeof(m_pPosR));

	m_pTU = new float[m_iLocusMax];

	// ���Ő�������TU�̔z��ɓ��Ԋu��UV���W���w��
	for (int i = 0; i < m_iLocusMax; i++)
	{
		m_pTU[i] = i*(1.0f / (float(m_iLocusMax) - 1.0f));
	}

	// �����ݒ�
	m_lCol = 0xffffffff;
	m_bAction = false;
}

LocusEffect::~LocusEffect()
{
	SAFE_DELETE(m_pTex);
	SAFE_DELETE_ARRAY(m_pPosL);
	SAFE_DELETE_ARRAY(m_pPosR);
	SAFE_DELETE_ARRAY(m_pTU);

}

void LocusEffect::Update(const Vector3& posL, const Vector3& posR)
{
	if (m_bAction == false)return;

	// �O�Օ����菇�ŉ�
	// �z��Ɋi�[����Ă�����W���Ō���ɂ��炵�Ă���
	for (int i = (m_iLocusMax - 1); i > 0; i--)
	{
		m_pPosL[i] = m_pPosL[i - 1];
		m_pPosR[i] = m_pPosR[i - 1];
	}

	// ���ݎ󂯎�������W�͈�Ԏ�O�̔z��Ɋi�[
	m_pPosL[0] = posL;
	m_pPosR[0] = posR;

}

void LocusEffect::Update()
{
	if (m_bAction == false)return;

	// �O�Օ����菇�ŉ�
	// �z��Ɋi�[����Ă�����W���Ō���ɂ��炵�Ă���
	for (int i = (m_iLocusMax - 1); i > 0; i--)
	{
		m_pPosL[i] = m_pPosL[i - 1];
		m_pPosR[i] = m_pPosR[i - 1];
	}

	// ���ݎ󂯎�������W�͈�Ԏ�O�̔z��Ɋi�[
	m_pPosL[0] = *m_pReferencePosL;
	m_pPosR[0] = *m_pReferencePosR;
}

void LocusEffect::Render()
{
	if (m_bAction == false)return;

	// �J�����OOFF��
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �O�Օ��|���S������
	LVERTEX vertex[4];
	for (int i = 0; i < m_iLocusMax - 1; i++)// [?]-1�ɂ��Ȃ��ƕ\�����o�O��
	{
		vertex[0].x = m_pPosL[i].x;
		vertex[0].y = m_pPosL[i].y;
		vertex[0].z = m_pPosL[i].z;
		vertex[0].color = m_lCol;
		vertex[0].tu = m_pTU[i];
		vertex[0].tv = 0.0f;

		vertex[1].x = m_pPosR[i].x;
		vertex[1].y = m_pPosR[i].y;
		vertex[1].z = m_pPosR[i].z;
		vertex[1].color = m_lCol;
		vertex[1].tu = m_pTU[i];
		vertex[1].tv = 1.0f;

		vertex[2].x = m_pPosL[i + 1].x;
		vertex[2].y = m_pPosL[i + 1].y;
		vertex[2].z = m_pPosL[i + 1].z;
		vertex[2].color = m_lCol;
		vertex[2].tu = m_pTU[i + 1];
		vertex[2].tv = 0.0f;

		vertex[3].x = m_pPosR[i + 1].x;
		vertex[3].y = m_pPosR[i + 1].y;
		vertex[3].z = m_pPosR[i + 1].z;
		vertex[3].color = m_lCol;
		vertex[3].tu = m_pTU[i + 1];
		vertex[3].tv = 1.0f;

		//3D��ԂɃ|���S���`��I
		tdnPolygon::Render3D(vertex, 2, m_pTex, RS::COPY_NOZ);

	}

	// �J�����O�ݒ��߂�
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void LocusEffect::Action(const Vector3& posL, const Vector3& posR)
{
	// �S�Ă̔z��Ɏ󂯎�������W�ŏ�����
	for (int i = 0; i < m_iLocusMax; i++)
	{
		m_pPosL[i] = posL;
		m_pPosR[i] = posR;
	}

	// �A�N�V�����t���O�𗧂Ă�
	m_bAction = true;
}

void LocusEffect::Action(Vector3* posL, Vector3* posR)
{
	m_pReferencePosL = posL;
	m_pReferencePosR = posR;

	// �S�Ă̔z��Ɏ󂯎�������W�ŏ�����
	for (int i = 0; i < m_iLocusMax; i++)
	{
		m_pPosL[i] = *m_pReferencePosL;
		m_pPosR[i] = *m_pReferencePosR;
	}

	// �A�N�V�����t���O�𗧂Ă�
	m_bAction = true;
}

void LocusEffect::Stop()
{
	m_bAction = false;
}

