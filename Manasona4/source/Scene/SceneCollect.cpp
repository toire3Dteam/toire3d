#include "TDNLIB.h"
#include "../Sound/SoundManager.h"
#include "system/FrameworkEx.h"
#include "../Data/PlayerData.h"
#include "SceneMenu.h"
#include "SceneCollect.h"
#include "../Fade/Fade.h"


//=============================================================================================
//		��	��	��	��	�J	��
bool sceneCollect::Initialize()
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	m_fLoadPercentage = .5f;	// ���[�h����

	// �摜������
	m_pImages[IMAGE::BACK] = new tdn2DObj("DATA/UI/Collect/bg.png");
	m_pImages[IMAGE::SCROLL_BAR] = new tdn2DObj("DATA/UI/Collect/ScrollBar.png");
	m_pImages[IMAGE::SCROLL_ARROW] = new tdn2DObj("DATA/UI/Collect/ScrollArrow.png");
	m_fLoadPercentage = 1.0f;	// ���[�h����

	m_pPlayerInfo = &PlayerDataMgr->m_PlayerInfo;

	bgm->PlayStreamIn("DATA/Sound/BGM/System/Collect.ogg");

	// �X�N���[���o�[
	m_iScrollHeight = 1440;
	m_iScrollScreenSize= 720;

	return true;
}

sceneCollect::~sceneCollect()
{
	bgm->StopStreamIn();
	FOR(IMAGE::MAX) delete m_pImages[i];
}
//=============================================================================================


//=============================================================================================
//		�X			�V
void sceneCollect::Update()
{
	static bool bEnd(false);

	// �t�F�[�h
	Fade::Update();

	if (bEnd)
	{
		// �t�F�[�h�I�������A��
		if (Fade::isFadeStop())
		{
			bEnd = false;
			MainFrameEx->ChangeScene(new sceneMenu);
		}
	}

	// �X�y�[�X�L�[�Ŕ�����
	else if (KEY(KEYCODE::KEY_A, 0) == 3)
	{
		//se->Play("�߂�");

		// �t�F�[�h�A�E�g�ݒ�
		Fade::Set(Fade::FLAG::FADE_OUT, 4);
		bEnd = true;
	}
}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneCollect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// �w�i
	m_pImages[IMAGE::BACK]->Render(0, 0);

	tdnPolygon::Rect((int)(1280 * .25f) / 2, 96, (int)(1280 * .75f), (int)(720 * .75f), RS::COPY, 0xff808080);

	// �v���C��
	tdnText::Draw(400, 400, 0xffffffff, "�v���C��: %d", m_pPlayerInfo->PlayCount);

	// �v���C����
	const int minutes(m_pPlayerInfo->PlayTime % 60), hour(m_pPlayerInfo->PlayTime / 60);
	tdnText::Draw(400, 460, 0xffffffff, "�v���C����: %d���� %d��", hour, minutes);

	// �R�C��
	tdnText::Draw(400, 520, 0xffffffff, "�R�C��: %d", m_pPlayerInfo->coin);

	// �X�N���[���o�[
	m_pImages[IMAGE::SCROLL_BAR]->Render(0, 0);
	float rate= (m_iScrollScreenSize / m_iScrollHeight);
	m_pImages[IMAGE::SCROLL_ARROW]->Render(0, 0,64, (int)(512* rate), 0, 0, 64, 512);

	// �t�F�[�h
	Fade::Render();
}
//
//=============================================================================================