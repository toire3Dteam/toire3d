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
int sceneSelect::m_iSelectStageNo = 0;
int sceneSelect::m_iSelectBGMNo = 0;

//******************************************************************
//		�������E���
//******************************************************************
sceneSelect::sceneSelect() :BaseGameEntity(ENTITY_ID::SCENE_SELECT) {}
bool sceneSelect::Initialize()
{
	// �Z���N�g�i���o�[
	//m_iSelectNo = 0;

	for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	{
		m_pPic[i] = nullptr;
	}

	m_pPic[PIC_TYPE::BG] = new tdn2DAnim("DATA/UI/CharacterSelect/BG.png");
	m_pPic[PIC_TYPE::BG]->OrderNone();
	m_pPic[PIC_TYPE::BG]->Action();

	m_pPic[PIC_TYPE::TITLE] = new tdn2DAnim("DATA/UI/CharacterSelect/title.png");
	m_pPic[PIC_TYPE::TITLE]->OrderMoveAppeared(16, 0, -200);

	m_pPic[PIC_TYPE::BLACK_LINE] = new tdn2DAnim("DATA/UI/CharacterSelect/BlackLine.png");
	m_pPic[PIC_TYPE::BLACK_LINE]->OrderMoveAppeared(24, 1280, 0);

	m_pPic[PIC_TYPE::INFO_PLATE] = new tdn2DAnim("DATA/UI/CharacterSelect/Information.png");
	m_pPic[PIC_TYPE::INFO_PLATE]->OrderMoveAppeared(24, 0, 592 + 200);

	m_iRectAlpha = 0;

	// Select�}�l�[�W���[
	m_pSelectUIMgr = new SelectUIManager();
	//m_pMenuUIMgr->ChangeSelectNo(m_iSelectNo);
	//m_pMenuUIMgr->Action();

	//m_pCtrlSelectUI = new ControllerSelectUI();

	// Stage&BGM
	m_tagSecondSelect.bBGMSelect = false;
	m_tagSecondSelect.iX = 293;
	m_tagSecondSelect.iY = 192;
	m_tagSecondSelect.pPlate = new tdn2DObj("Data/UI/CharacterSelect/SecondSelect/secondSelectPlate.png");
	m_tagSecondSelect.pStagePlate = new tdn2DObj("Data/UI/CharacterSelect/SecondSelect/StagePlate.png");
	m_tagSecondSelect.pBGMPlate = new tdn2DObj("Data/UI/CharacterSelect/SecondSelect/BGMPlate.png");
	m_tagSecondSelect.pStagePic = new tdn2DObj("Data/UI/CharacterSelect/SecondSelect/StagePic.png");

	m_tagSecondSelect.pArrow = new tdn2DObj("Data/UI/CharacterSelect/SecondSelect/arrow.png");
	
	m_tagSecondSelect.pStageLeftRip = new tdn2DAnim("Data/UI/CharacterSelect/SecondSelect/arrow.png");
	m_tagSecondSelect.pStageRightRip = new tdn2DAnim("Data/UI/CharacterSelect/SecondSelect/arrow.png");
	m_tagSecondSelect.pBGMLeftRip = new tdn2DAnim("Data/UI/CharacterSelect/SecondSelect/arrow.png");
	m_tagSecondSelect.pBGMRightRip = new tdn2DAnim("Data/UI/CharacterSelect/SecondSelect/arrow.png");
	m_tagSecondSelect.pStageLeftRip->OrderRipple(12, 1.0f, 0.08f);
	m_tagSecondSelect.pStageRightRip->OrderRipple(12, 1.0f, 0.08f);
	m_tagSecondSelect.pStageRightRip->SetTurnOver(true);
	m_tagSecondSelect.pBGMLeftRip->OrderRipple(12, 1.0f, 0.08f);
	m_tagSecondSelect.pBGMRightRip->OrderRipple(12, 1.0f, 0.08f);
	m_tagSecondSelect.pBGMRightRip->SetTurnOver(true);
	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneSelect>(this);
	m_pStateMachine->SetCurrentState(SceneSelectState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�

	// BGM���X�g���[�~���O�Đ�
	bgm->PlayStreamIn("DATA/Sound/BGM/System/CharaSelect.ogg");

	return true;
}

sceneSelect::~sceneSelect()
{

	for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pPic[i]);
	}

	bgm->StopStreamIn();
	SAFE_DELETE(m_pSelectUIMgr);
	//SAFE_DELETE(m_pCtrlSelectUI);
	delete m_pStateMachine;

	// 
	SAFE_DELETE(m_tagSecondSelect.pPlate);
	SAFE_DELETE(m_tagSecondSelect.pStagePlate);
	SAFE_DELETE(m_tagSecondSelect.pBGMPlate);
	SAFE_DELETE(m_tagSecondSelect.pStagePic);
	SAFE_DELETE(m_tagSecondSelect.pArrow);
	SAFE_DELETE(m_tagSecondSelect.pStageLeftRip);
	SAFE_DELETE(m_tagSecondSelect.pStageRightRip);
	SAFE_DELETE(m_tagSecondSelect.pBGMLeftRip);
	SAFE_DELETE(m_tagSecondSelect.pBGMRightRip);
}				

//******************************************************************
//		����
//******************************************************************

void sceneSelect::Update()
{
	// �X�V
	for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	{
		m_pPic[i]->Update();
	}

	// �Z���N�g�}�l�[�W���[
	m_pSelectUIMgr->Update();

	// ���̃A�j��
	m_tagSecondSelect.pStageLeftRip->Update();
	m_tagSecondSelect.pBGMLeftRip->Update();
	m_tagSecondSelect.pStageRightRip->Update();
	m_tagSecondSelect.pBGMRightRip->Update();

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

	m_pPic[PIC_TYPE::BG]->Render(0, 0);

	m_pPic[PIC_TYPE::BLACK_LINE]->Render(0, 0);

	// �Z���N�g�}�l�[�W���[
	m_pSelectUIMgr->Render();

	// 
	DWORD col = ARGB(m_iRectAlpha, 0, 0, 0);	
	tdnPolygon::Rect(0, 0, 1280, RS::COPY, col);


	// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
	m_pStateMachine->Render();


	m_pPic[PIC_TYPE::TITLE]->Render(0, 0);
	m_pPic[PIC_TYPE::INFO_PLATE]->Render(0, 592 + 5);

	// �t�F�[�h
	Fade::Render();
	
}

void sceneSelect::StageAndBGMRender()
{
	m_tagSecondSelect.pPlate->Render(m_tagSecondSelect.iX, m_tagSecondSelect.iY);
	m_tagSecondSelect.pStagePlate->Render(m_tagSecondSelect.iX, m_tagSecondSelect.iY, 689, 323, 0, 323 - (m_tagSecondSelect.bBGMSelect * 323), 689, 323);
	m_tagSecondSelect.pBGMPlate->Render(m_tagSecondSelect.iX, m_tagSecondSelect.iY, 689, 323, 0, (m_tagSecondSelect.bBGMSelect * 323), 689, 323);

	// �X�e�[�W�̃C���X�g
	m_tagSecondSelect.pStagePic->Render(512, 244, 256, 128, 0, 0, 256, 128);

	// ���
	m_tagSecondSelect.pArrow->SetTurnOver(false);
	m_tagSecondSelect.pArrow->Render(447, 294, 32, 32, 0, 32-(m_tagSecondSelect.bBGMSelect * 32), 32, 32);
	m_tagSecondSelect.pArrow->Render(320, 421, 32, 32, 0, (m_tagSecondSelect.bBGMSelect * 32), 32, 32);
	m_tagSecondSelect.pArrow->SetTurnOver(true);
	m_tagSecondSelect.pArrow->Render(800, 294, 32, 32, 0, 32 - (m_tagSecondSelect.bBGMSelect*32), 32, 32);
	m_tagSecondSelect.pArrow->Render(928, 421, 32, 32, 0, (m_tagSecondSelect.bBGMSelect*32), 32, 32);

	m_tagSecondSelect.pStageLeftRip->Render(447, 294, 32, 32, 0, 32 - (m_tagSecondSelect.bBGMSelect * 32), 32, 32,RS::ADD);
	m_tagSecondSelect.pBGMLeftRip->Render(320, 421, 32, 32, 0, (m_tagSecondSelect.bBGMSelect * 32), 32, 32, RS::ADD);
	m_tagSecondSelect.pStageRightRip->Render(800, 294, 32, 32, 0, 32 - (m_tagSecondSelect.bBGMSelect * 32), 32, 32, RS::ADD);
	m_tagSecondSelect.pBGMRightRip->Render(928, 421, 32, 32, 0, (m_tagSecondSelect.bBGMSelect * 32), 32, 32, RS::ADD);


}

void sceneSelect::FirstAction()
{
	m_pPic[PIC_TYPE::TITLE]->Action();
	m_pPic[PIC_TYPE::BLACK_LINE]->Action(6);
	m_pPic[PIC_TYPE::INFO_PLATE]->Action(6);
	
	m_pSelectUIMgr->FirstAction();

}

