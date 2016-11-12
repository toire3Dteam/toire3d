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

	m_pic3DScale = 50.0f;// �����l�@�RD�ŕ`�悷��Ƃ��̑傫��

	state = RS::ADD;

	m_iDelayFrame = 0;
}

BasePanelEffect::~BasePanelEffect()
{
	SAFE_DELETE(m_pic);
}

void BasePanelEffect::Update()
{
	m_iDelayFrame--;// �f�B���C�t���[�������炷
	if (m_iDelayFrame > 0)
	{
			return;
	}
	m_iDelayFrame = 0;

	// �G�t�F�N�g
	m_pic->Update();
}

void BasePanelEffect::Render()
{
	if (m_iDelayFrame > 0)	return;
	
	m_pic->Render((int)m_pos.x - (m_picSize / 2), (int)m_pos.y - (m_picSize / 2), state);

}

void BasePanelEffect::Render3D()
{
	if (m_iDelayFrame > 0)	return;

	//tdnRenderState::Filter(FALSE);
	m_pic->GetPic()->SetScale(m_pic3DScale);// �e�����L���Ă���X�P�[���ɐݒ�
	m_pic->Render3D(m_pos, state);
	m_pic->GetPic()->SetScale(1);
	//tdnRenderState::Filter(TRUE);
}

void BasePanelEffect::Action(int x , int y, int delayFrame)
{
	// �G�t�F�N�g����
	m_pic->Action();

	m_pos.x = (float)x;
	m_pos.y = (float)y;

	m_iDelayFrame = delayFrame;
}

void BasePanelEffect::Action(Vector3 pos, int delayFrame)
{
	// �G�t�F�N�g����
	m_pic->Action();

	m_pos = pos;

	m_iDelayFrame = delayFrame;
}

void BasePanelEffect::Stop()
{
	// �G�t�F�N�g�~�߂�
	m_pic->Stop();


}

