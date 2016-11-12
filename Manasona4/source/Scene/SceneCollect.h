#pragma once

namespace PlayerData
{
struct Infomation;
}

class sceneCollect : public BaseScene
{
public:

	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	bool Initialize();
	~sceneCollect();

	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	void Update();
	void Render();

private:

	enum IMAGE
	{
		BACK,
		SCROLL_BAR,
		SCROLL_ARROW,
		MAX
	};
	tdn2DObj *m_pImages[IMAGE::MAX];
	
	// �v���C���
	PlayerData::Infomation *m_pPlayerInfo;


	// �X�N���[������
	tdn2DObj* m_pScrollBar;
	tdn2DObj* m_pScrollArrow;

	// �X�N���[��
	float m_iScrollHeight;	// �ő�̍���
	float m_iScrollScreenSize;// ��̃X�N���[����
	float m_iScrollPos;

};