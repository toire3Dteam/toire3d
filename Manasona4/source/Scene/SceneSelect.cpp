#include "TDNLIB.h"
#include "sceneSelect.h"
#include "sceneSelectState.h"
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
//int sceneSelect::m_iSelectNo(0);	// �Z���N�g�i���o�[(static�ɂ��邱�Ƃɂ���āA�O��I���������[�h�ɍ��킹���܂܃��j���[�V�[�����n�܂�)

//******************************************************************
//		�������E���
//******************************************************************
sceneSelect::sceneSelect() :BaseGameEntity(ENTITY_ID::SCENE_SELECT) {}
bool sceneSelect::Initialize()
{
	// �Z���N�g�i���o�[
	//m_iSelectNo = 0;

	// Select�}�l�[�W���[
	m_pSelectUIMgr = new SelectUIManager();
	//m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	//m_pMenuUIMgr->Action();

	//m_pCtrlSelectUI = new ControllerSelectUI();

	
	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneSelect>(this);
	m_pStateMachine->SetCurrentState(SceneSelectState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�

	// BGM���X�g���[�~���O�Đ�
	bgm->PlayStreamIn("DATA/Sound/BGM/System/CharaSelect.ogg");

	return true;
}

sceneSelect::~sceneSelect()
{


	bgm->StopStreamIn();
	SAFE_DELETE(m_pSelectUIMgr);
	//SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;


}

//******************************************************************
//		����
//******************************************************************

void sceneSelect::Update()
{

	// �Z���N�g�}�l�[�W���[
	m_pSelectUIMgr->Update();

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

void sceneSelect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// �Z���N�g�}�l�[�W���[
	m_pSelectUIMgr->Render();

	// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
	m_pStateMachine->Render();


	// �t�F�[�h
	Fade::Render();
	
}

