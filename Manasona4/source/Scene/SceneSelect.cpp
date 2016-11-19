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
//	シーンメニュー
//+------------------------
//int sceneSelect::m_iSelectNo(0);	// セレクトナンバー(staticにすることによって、前回選択したモードに合わせたままメニューシーンが始まる)
int sceneSelect::m_iSelectStageNo = 0;
int sceneSelect::m_iSelectBGMNo = 0;

//******************************************************************
//		初期化・解放
//******************************************************************
sceneSelect::sceneSelect() :BaseGameEntity(ENTITY_ID::SCENE_SELECT) {}
bool sceneSelect::Initialize()
{
	// セレクトナンバー
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

	// Selectマネージャー
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
	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneSelect>(this);
	m_pStateMachine->SetCurrentState(SceneSelectState::Intro::GetInstance());	// ステートの設定

	// BGMをストリーミング再生
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
//		処理
//******************************************************************

void sceneSelect::Update()
{
	// 更新
	for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	{
		m_pPic[i]->Update();
	}

	// セレクトマネージャー
	m_pSelectUIMgr->Update();

	// 矢印のアニメ
	m_tagSecondSelect.pStageLeftRip->Update();
	m_tagSecondSelect.pBGMLeftRip->Update();
	m_tagSecondSelect.pStageRightRip->Update();
	m_tagSecondSelect.pBGMRightRip->Update();

	// フェード
	Fade::Update();

	{
		// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
		m_pStateMachine->Update();
		return;
	}
}

//******************************************************************
//		描画
//******************************************************************

void sceneSelect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	m_pPic[PIC_TYPE::BG]->Render(0, 0);

	m_pPic[PIC_TYPE::BLACK_LINE]->Render(0, 0);

	// セレクトマネージャー
	m_pSelectUIMgr->Render();

	// 
	DWORD col = ARGB(m_iRectAlpha, 0, 0, 0);	
	tdnPolygon::Rect(0, 0, 1280, RS::COPY, col);


	// ★ここにステートマシン描画(多分2D関係が多いんじゃないかと)
	m_pStateMachine->Render();


	m_pPic[PIC_TYPE::TITLE]->Render(0, 0);
	m_pPic[PIC_TYPE::INFO_PLATE]->Render(0, 592 + 5);

	// フェード
	Fade::Render();
	
}

void sceneSelect::StageAndBGMRender()
{
	m_tagSecondSelect.pPlate->Render(m_tagSecondSelect.iX, m_tagSecondSelect.iY);
	m_tagSecondSelect.pStagePlate->Render(m_tagSecondSelect.iX, m_tagSecondSelect.iY, 689, 323, 0, 323 - (m_tagSecondSelect.bBGMSelect * 323), 689, 323);
	m_tagSecondSelect.pBGMPlate->Render(m_tagSecondSelect.iX, m_tagSecondSelect.iY, 689, 323, 0, (m_tagSecondSelect.bBGMSelect * 323), 689, 323);

	// ステージのイラスト
	m_tagSecondSelect.pStagePic->Render(512, 244, 256, 128, 0, 0, 256, 128);

	// 矢印
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

