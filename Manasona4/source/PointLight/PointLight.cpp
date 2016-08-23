#include "PointLight.h"
#include "../DeferredEx/DeferredEx.h"


/*************************/
// �@�|�C���g���C�g�}�l�[�W���[
/*************************/

PointLightManager* PointLightManager::m_pInstance = nullptr;

/*************************/
//	�������E���
/*************************/
PointLightManager::PointLightManager()
{
	//m_PointLightData.reserve(64);

}

PointLightManager::~PointLightManager()
{
	//for (auto it : m_PointLightData)
	//{
	//	SAFE_DELETE(it);
	//}
	m_PointLightData.clear();
}

/*************************/
//	���ԁE���
/*************************/
PointLightManager* PointLightManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new PointLightManager();
	}

	return m_pInstance;
}

void PointLightManager::Release()
{
	SAFE_DELETE(m_pInstance);
}

/*************************/
//	�X�V�E�`��
/*************************/

void PointLightManager::Update()
{
	// ������
	DeferredManagerEx.ClearPLSdata();

	// List
	for (auto it = m_PointLightData.begin(); it != m_PointLightData.end();)
	{
		float rate = 0.0f;

		// ����A�[�̑O�����Ŕ����ς���
		if ((*it).alphaNear >= (*it).alphaNear)
		{
			float A = (float)((*it).endFlame - (*it).nowFlame);
			float B = (float)((*it).endFlame - (*it).alphaFar);
			rate = A / B;
		}
		else
		{
			rate = (float)(*it).nowFlame / (float)(*it).alphaNear;// 
		}

		rate = Math::Clamp(rate, 0.0f, 1.0f);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�

		// �����Ń|�C���g���C�g�ɋl�߂�
		DeferredManagerEx.AddPLSdata((*it).pos, (*it).color, (*it).range, ((*it).maxPower*rate));

		// �t���[���X�V
		(*it).nowFlame++;
		if ((*it).nowFlame >= (*it).endFlame)
		{
			// ��ɏ���
			//SAFE_DELETE((*it));
			// ����ɍX�V�����
			it = m_PointLightData.erase(it);

		}else
		{
			it++;
		}
	}




}

void PointLightManager::SendPointLight()
{



}

void PointLightManager::AddPointLight
(Vector3 pos, Vector3 color, float range,
float maxPower, int endFlame, int alphaNear, int alphaFar)
{
	PointLightData data;
	data.pos = pos;
	data.color = color;
	data.range = range;
	data.maxPower = maxPower;
	data.endFlame = endFlame;
	data.nowFlame = 0;// ���݂̃t���[��
	data.alphaNear = alphaNear;
	data.alphaFar = alphaFar;

	m_PointLightData.push_back(data);
}



