#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneMain.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"

//#include "sceneResult.h"
//#include "SceneSwitch/SceneSwitch.h"
//SceneSwitchPrev* prevEF;
//+------------------------
//	�V�[�����j���[
//+------------------------
int sceneMenu::m_iSelectNo(0);	// �Z���N�g�i���o�[(static�ɂ��邱�Ƃɂ���āA�O��I���������[�h�ɍ��킹���܂܃��j���[�V�[�����n�܂�)

//******************************************************************
//		�������E���
//******************************************************************
sceneMenu::sceneMenu() :BaseGameEntity(ENTITY_ID::SCENE_MENU) {}
bool sceneMenu::Initialize()
{
	//prevEF = new SceneSwitchPrev();

	// �Z���N�g�i���o�[
	//m_iSelectNo = 0;

	// UI�}�l�[�W���[
	m_pMenuUIMgr = new MenuUIManager();
	m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	m_pMenuUIMgr->Action();

	m_pCtrlSelectUI = new ControllerSelectUI();


	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneMenu>(this);
	m_pStateMachine->SetCurrentState(SceneMenuState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�

	// BGM���X�g���[�~���O�Đ�
	bgm->PlayStreamIn("DATA/Sound/BGM/System/CharaSelect.ogg");

	return true;
}

sceneMenu::~sceneMenu()
{
	bgm->StopStreamIn();
	delete m_pMenuUIMgr;
	SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;
}

//******************************************************************
//		����
//******************************************************************

void sceneMenu::Update()
{
	//if (KeyBoardTRG(KB_F))
	//{
	//	prevEF->Action();
	//}
	//prevEF->Update();
	//if (prevEF->IsEnd())
	//{
	//	MainFrameEx->ChangeScene(new sceneResult);
	//	return;
	//}

	// �t�F�[�h
	Fade::Update();

	{
		// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
		m_pStateMachine->Update();
		return;
	}
}

//******************************************************************
//		�`��
//******************************************************************

void sceneMenu::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	
	// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
	m_pStateMachine->Render();

	// �t�F�[�h
	Fade::Render();
	
	//prevEF->Render();
}

