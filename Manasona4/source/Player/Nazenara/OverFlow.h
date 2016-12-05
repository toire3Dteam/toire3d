#pragma once

#include "Nazenara.h"

class OverFlow
{
public:
	OverFlow(Nazenara *pNazenara) :m_pNazenara(pNazenara), m_iCreateTimer(0), m_pObj(new iex3DObj("DATA/CHR/Nazenara/naze_dance.IEM"))
	{
		m_pObj->SetMotion(0);
		m_NazeList.clear();
	}
	~OverFlow()
	{
		delete m_pObj;
		for (auto it = m_NazeList.begin(); it != m_NazeList.end();)
		{
			delete (*it);
			it = m_NazeList.erase(it);
		}
	}
	void Update()
	{
		m_pObj->Animation();

		// 生成時間カウント
		if (
			//++m_iCreateTimer > c_CREATE_INTERVAL || 
			KeyBoardTRG(KB_ENTER)
			)
		{
			// ∵生成
			m_iCreateTimer = 0;
			const float angle(rand() % 628 * .01f);
			m_NazeList.push_back(new Naze(Vector3(sinf(angle) * c_CREATE_RADIUS, 0, cosf(angle) * c_CREATE_RADIUS), m_pObj));
		}
		for (auto it : m_NazeList) it->Update();
	}
	void Render(){ for (auto it : m_NazeList) it->Render(); }

private:

	class Naze
	{
	private:
		Vector3 vPos;			// 座標
		const float fAngleY;	// Yアングル
		iex3DObj *pObj;			// 参照するだけメッシュ
		const float c_SPEED = .1f;

	public:
		Naze(const Vector3 &pos, iex3DObj *obj) :vPos(pos), pObj(obj), fAngleY(atan2f(-vPos.x, -vPos.z)){}
		void Update()
		{
			if (vPos.x * vPos.x + vPos.z * vPos.z > 16)
			{
				// 原点に向けて徐々に進んでいく
				vPos.x += sinf(fAngleY) * c_SPEED;
				vPos.z += cosf(fAngleY) * c_SPEED;
			}
		}
		void Render()
		{
			pObj->SetPos(vPos);
			pObj->SetAngle(fAngleY);
			pObj->Update();
			pObj->Render();
		}
	};

	int m_iCreateTimer;					// 生成時間
	const int  c_CREATE_INTERVAL = 180;	// 生成間隔
	const float c_CREATE_RADIUS = 60;	// 生成半径

	iex3DObj *m_pObj;				// ∵メッシュ
	Nazenara *m_pNazenara;
	std::list<Naze*> m_NazeList;	// ∵くんりすと
};