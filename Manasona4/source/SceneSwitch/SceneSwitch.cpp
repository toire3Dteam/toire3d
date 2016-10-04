#include "SceneSwitch.h"

SceneSwitch::SceneSwitch()
{
	for (int i = 0; i < PIC_MAX; i++)
	{
		std::string text;
		text = "Data/Result/SceneSwitch/";

		text += std::to_string(i);
		text += ".png";

		int len = text.length();
		char *cha;
		cha = new char[len + 1];
		memcpy(cha, text.c_str(), len + 1);

		m_Pic[i] = new tdn2DObj(cha);
		delete cha;
	}

	m_bActionFlag = false;
	m_iFrame = 0;
	m_iCount = 0;
}

SceneSwitch::~SceneSwitch()
{
	for (int i = 0; i < PIC_MAX; i++)
	{
		delete m_Pic[i];
	}
}

void SceneSwitch::Update()
{
	if (m_bActionFlag == false) return;

	if (++m_iFrame >= 6)
	{
		m_iFrame = 0;
		m_iCount++;

		// ç≈å„Ç‹Ç≈çsÇØÇŒèIóπ
		if (PIC_MAX-1 < m_iCount)
		{
			m_bActionFlag = false;
		}
	}	

}

void SceneSwitch::Render()
{
	if (m_bActionFlag == false) return;

	m_Pic[m_iCount]->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
}

void SceneSwitch::Action()
{
	m_bActionFlag = true;
	m_iFrame = 0;
	m_iCount = 0;
}