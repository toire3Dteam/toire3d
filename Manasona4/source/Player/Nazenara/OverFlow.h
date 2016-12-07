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

		// �������ԃJ�E���g
		if (
			//++m_iCreateTimer > c_CREATE_INTERVAL || 
			KeyBoardTRG(KB_ENTER)
			)
		{
			// �搶��
			m_iCreateTimer = 0;
			const float angle(rand() % 628 * .01f);
			m_NazeList.push_back(new Naze(Vector3(sinf(angle) * c_CREATE_RADIUS, 0, cosf(angle) * c_CREATE_RADIUS), m_pObj));
		}
		for (auto it : m_NazeList) it->Update();
	}
	void Render(){ for (auto it : m_NazeList) it->Render(); }

	void Set(const Vector3 positions[], int num)
	{
		// �����̍��W�Ɂ���Z�b�g
		FOR(num)m_NazeList.push_back(new Naze(positions[i], m_pObj));
	}

private:

	class Naze
	{
	private:
		Vector3 vPos;			// ���W
		float fAlpha;			// �A���t�@�̒l
		const float fAngleY;	// Y�A���O��
		iex3DObj *pObj;			// �Q�Ƃ��邾�����b�V��
		const float c_SPEED = .1f;

	public:
		Naze(const Vector3 &pos, iex3DObj *obj) :vPos(pos), pObj(obj), fAngleY(atan2f(-vPos.x, -vPos.z)), fAlpha{}{}
		void Update()
		{
			if (fAlpha < 1)
			{
				fAlpha = min(1, fAlpha + .05f);
			}

			if (vPos.x * vPos.x + vPos.z * vPos.z > 16)
			{
				// ���_�Ɍ����ď��X�ɐi��ł���
				vPos.x += sinf(fAngleY) * c_SPEED;
				vPos.z += cosf(fAngleY) * c_SPEED;
			}
		}
		void Render()
		{
			pObj->SetPos(vPos);
			pObj->SetAngle(fAngleY);
			pObj->Update();
			pObj->Render(RS::COPY, fAlpha);
		}
	};

	int m_iCreateTimer;					// ��������
	const int  c_CREATE_INTERVAL = 180;	// �����Ԋu
	const float c_CREATE_RADIUS = 60;	// �������a

	iex3DObj *m_pObj;				// �惁�b�V��
	Nazenara *m_pNazenara;
	std::list<Naze*> m_NazeList;	// �悭��肷��
};