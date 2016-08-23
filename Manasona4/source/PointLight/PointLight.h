#pragma once
#include "TDNLIB.h"


/**********************************/
//	�@�|�C���g���C�g�}�l�[�W���[
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
	//this is a �V���O���g��
	static PointLightManager* GetInstance();
	static void Release();

	void Update();
	void SendPointLight();
	void AddPointLight(Vector3 pos, Vector3 color, float range, float maxPower, int endFlame, int alphaNear, int alphaFar);

private:

	/*************************/
	//	PointLight�p
	/*************************/
	typedef struct
	{
		Vector3 pos;
		Vector3 color;
		float range;

		// �t���[��
		float maxPower;	
		int endFlame;
		int nowFlame;
		int alphaNear;
		int alphaFar;

	} PointLightData;
	//std::vector<PointLightData> m_PointLightData;
	// �����̃f�[�^
	std::list<PointLightData> m_PointLightData;
	std::list<PointLightData*>::iterator m_it;

};

#define PointLightMgr PointLightManager::GetInstance()
