#include "PointLight.h"
#include "../DeferredEx/DeferredEx.h"


/*************************/
// 　ポイントライトマネージャー
/*************************/

PointLightManager* PointLightManager::m_pInstance = nullptr;

/*************************/
//	初期化・解放
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
//	実態・解放
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
//	更新・描画
/*************************/

void PointLightManager::Update()
{
	// 初期化
	DeferredManagerEx.ClearPLSdata();

	// List
	for (auto it = m_PointLightData.begin(); it != m_PointLightData.end();)
	{
		float rate = 0.0f;

		// α二アーの前か後ろで判定を変える
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

		rate = Math::Clamp(rate, 0.0f, 1.0f);//指定された値を 0 ～ 1 の範囲にクランプします

		// ここでポイントライトに詰める
		DeferredManagerEx.AddPLSdata((*it).pos, (*it).color, (*it).range, ((*it).maxPower*rate));

		// フレーム更新
		(*it).nowFlame++;
		if ((*it).nowFlame >= (*it).endFlame)
		{
			// 先に消す
			//SAFE_DELETE((*it));
			// 勝手に更新される
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
	data.nowFlame = 0;// 現在のフレーム
	data.alphaNear = alphaNear;
	data.alphaFar = alphaFar;

	m_PointLightData.push_back(data);
}



