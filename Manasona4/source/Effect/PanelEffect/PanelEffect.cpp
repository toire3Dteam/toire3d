#include "PanelEffect.h"

//BaseEffect::BaseEffect(char* name, int size, int panelNum, int flame, int upCount, bool isRoop = false)
//{
//	m_picSize = size;
//	m_pic = new AnimationPanel(name, size, size, panelNum, flame, upCount, isRoop);
//	m_pos.x = m_pos.y = 0.0f;
//
//}
BasePanelEffect::BasePanelEffect()
{
	m_picSize = 256;
	//m_pic = new AnimationPanel("Data/Effect/plusEffect.png", 256, 256, 15, 1, 4, false);
	m_pic = nullptr;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;

	state = RS::ADD;
}

BasePanelEffect::~BasePanelEffect()
{
	SAFE_DELETE(m_pic);
}

void BasePanelEffect::Update()
{
	// エフェクト
	m_pic->Update();
}

void BasePanelEffect::Render()
{
	m_pic->Render((int)m_pos.x - (m_picSize / 2), (int)m_pos.y - (m_picSize / 2), state);

}

void BasePanelEffect::Render3D()
{
	//tdnRenderState::Filter(FALSE);
	m_pic->GetPic()->SetScale(50.0f);
	m_pic->Render3D(m_pos, state);
	m_pic->GetPic()->SetScale(1);
	//tdnRenderState::Filter(TRUE);
}

void BasePanelEffect::Action(int x , int y)
{
	// エフェクト発動
	m_pic->Action();

	m_pos.x = (float)x;
	m_pos.y = (float)y;

}

void BasePanelEffect::Action(Vector3 pos)
{
	// エフェクト発動
	m_pic->Action();

	m_pos = pos;

}

void BasePanelEffect::Stop()
{
	// エフェクト止める
	m_pic->Stop();


}

