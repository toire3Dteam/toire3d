#include "BaseEffect.h"

//BaseEffect::BaseEffect(char* name, int size, int panelNum, int flame, int upCount, bool isRoop = false)
//{
//	m_picSize = size;
//	m_pic = new AnimationPanel(name, size, size, panelNum, flame, upCount, isRoop);
//	m_pos.x = m_pos.y = 0.0f;
//
//}
BaseEffect::BaseEffect()
{
	m_picSize = 256;
	//m_pic = new AnimationPanel("Data/Effect/plusEffect.png", 256, 256, 15, 1, 4, false);
	m_pic = nullptr;
	m_pos.x = m_pos.y = 0.0f;

	state = RS::ADD;
}

BaseEffect::~BaseEffect()
{
	SAFE_DELETE(m_pic);
}

void BaseEffect::Update()
{
	// エフェクト
	m_pic->Update();
}

void BaseEffect::Render()
{
	m_pic->Render((int)m_pos.x - (m_picSize / 2), (int)m_pos.y - (m_picSize / 2), state);

}

void BaseEffect::Action(int x , int y)
{
	// エフェクト発動
	m_pic->Action();

	m_pos.x = (float)x;
	m_pos.y = (float)y;

}

void BaseEffect::Stop()
{
	// エフェクト止める
	m_pic->Stop();


}

