#pragma once
#include "TDNLIB.h"


/**********************************/
//	　ポイントライトマネージャー
/**********************************/

class PointLightManager
{
private:
	PointLightManager();
	~PointLightManager();
	
	PointLightManager(const PointLightManager&){}
	PointLightManager& operator=(const PointLightManager&){}

	static PointLightManager* m_pInstance;
public:
	//this is a シングルトン
	static PointLightManager* GetInstance();
	static void Release();

	void Update();
	void SendPointLight();
	void AddPointLight(Vector3 pos, Vector3 color, float range, float maxPower, int endFlame, int alphaNear, int alphaFar);

private:

	/*************************/
	//	PointLight用
	/*************************/
	typedef struct
	{
		Vector3 pos;
		Vector3 color;
		float range;

		// フレーム
		float maxPower;	
		int endFlame;
		int nowFlame;
		int alphaNear;
		int alphaFar;

	} PointLightData;
	//std::vector<PointLightData> m_PointLightData;
	// 数字のデータ
	std::list<PointLightData> m_PointLightData;
	std::list<PointLightData*>::iterator m_it;

};

#define PointLightMgr PointLightManager::GetInstance()
