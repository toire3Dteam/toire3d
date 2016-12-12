#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneMain.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Window\OptionWindow.h"	// �K�v�ȃE�B���h�E
#include "Window\SoundWindow.h"	// 
#include "../Scene/sceneResult.h"

//#include "sceneResult.h"
//#include "SceneSwitch/SceneSwitch.h"
//+------------------------
//	�V�[�����j���[
//+------------------------
int sceneMenu::m_iSelectNo(0);	// �Z���N�g�i���o�[(static�ɂ��邱�Ƃɂ���āA�O��I���������[�h�ɍ��킹���܂܃��j���[�V�[�����n�܂�)
int sceneMenu::m_iCtrlDevice(0);// ���삵�Ă����f�o�C�X

//******************************************************************
//		�������E���
//******************************************************************
sceneMenu::sceneMenu() :BaseGameEntity(ENTITY_ID::SCENE_MENU) {}
bool sceneMenu::Initialize()
{	
	// �Z���N�g�i���o�[
	//m_iSelectNo = 0;

	// UI�}�l�[�W���[
	m_pMenuUIMgr = new MenuUIManager(m_iSelectNo);
	m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	m_pMenuUIMgr->Action();

	m_pCtrlSelectUI = new ControllerSelectUI();


	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		switch ((TIPS_TYPE)i)
		{
		case TIPS_TYPE::TUTORIAL:
			m_pTips[i] = new TipsCard("�`���[�g���A�����n�߂܂����H", true);
			break;
		default:
			MyAssert(0, "����ȃq���g�͂Ȃ�");
			break;
		}
	}
	
	// �E�B���h�E��
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		switch ((WINDOW_TYPE)i)
		{
		case WINDOW_TYPE::OPTION:
			m_pWindow[i] = new OptionWindow(Vector2(500, 250));
			break;
		case WINDOW_TYPE::SOUND:
			m_pWindow[i] = new SoundWindow(Vector2(800, 250));
			break;
		default:
			MyAssert(0, "����ȃE�B���h�E�͂Ȃ�");
			break;
		}
	}


	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneMenu>(this);
	m_pStateMachine->SetCurrentState(SceneMenuState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�

	// BGM���X�g���[�~���O�Đ�
	bgm->PlayStreamIn("DATA/Sound/BGM/System/ModeSelect.ogg");

	return true;
}

sceneMenu::~sceneMenu()
{
	bgm->StopStreamIn();
	delete m_pMenuUIMgr;
	SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;
	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTips[i]);
	}
	// �E�B���h�E��
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pWindow[i]);
	}
}

//******************************************************************
//		����
//******************************************************************

void sceneMenu::Update()
{

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		m_pTips[i]->Update(m_iCtrlDevice);
	}

	// �E�B���h�E��
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Update();
	}

	// �t�F�[�h
	Fade::Update();

	{
		// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
		m_pStateMachine->Update();

#ifdef DEBUG
		// �G���^�[�Ń��[�v
		if (KeyBoard(KB_ENTER) == 2)
		{
			ResultData data;
			data.eWinnerSide = SIDE::RIGHT;
			data.iMaxDamage = 2500;
			data.iRemainingHP = 80;
			data.iElapsedTime = 35;

			MainFrameEx->ChangeScene(new sceneResult(data));
			return;
		}

#endif // DEBUG

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

	// �E�B���h�E��
	for (int i = 0; i < (int)WINDOW_TYPE::ARRAY_END; i++)
	{
		m_pWindow[i]->Redner();
	}

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		m_pTips[i]->Render();
	}
	

	// �����s����
	//std::vector<int> v;
	//int N = 1000 * 100;
	//for (int i = 0; i < N; i++)
	//{
	//	v.push_back(i);
	//}


	// �t�F�[�h
	Fade::Render();
	
}

