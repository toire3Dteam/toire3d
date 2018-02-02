#include "TDNLIB.h"
#include "system/FrameworkEx.h"
#include "sceneLoading.h"
#include "LoadSceneThread.h"

//=============================================================================================
//		��	��	��	��	�J	��
sceneLoading::sceneLoading(BaseScene *newScene) :BaseScene(),m_pNewScene(newScene),

// �X���b�h�֘A�������]�[��
//m_thread(&sceneLoading::ThreadFunction, this, newScene),	// �R���X�g���N�^�ŃX���b�h�𐶐� �������o�֐����w�肷��ꍇ�́A��������this�����邱�ƁB
//m_bThread(true),											// �X���b�h�t���O

// 0�������]�[��
m_pImage{},
m_pGauge{},
m_AnimeCount{},
m_AnimeFrame{},
m_fLoadGaugePercent{},
m_fLoadAngle{}
{

}

bool sceneLoading::Initialize()
{
	m_pImage = new tdn2DObj("DATA/UI/Loading/loadPic.png");
	m_pGauge = new tdn2DObj("DATA/UI/Loading/loading_gauge.png");
	m_AnimeCount = m_AnimeFrame = 0;
	m_fLoadGaugePercent = 0;
	m_fLoadAngle = 0.0f;

	// �����[�f�B���O����FPS�𗎂Ƃ�
	MainFrameEX->SetFPSMode(FPS_MODE::FPS_45);

	// ���X���b�h����
	LoadSceneThreadMgr->Initialize(m_pNewScene);

	//bThread = true;
	//_beginthread(ThreadFunction, 0, (void*)m_newScene);

	return true;
}

sceneLoading::~sceneLoading()
{
	// ���X���b�h���
	LoadSceneThreadMgr->Release();

	MainFrameEX->SetFPSMode(FPS_MODE::NORMAL);

	SAFE_DELETE(m_pImage);
	SAFE_DELETE(m_pGauge);
}
//=============================================================================================

//=============================================================================================
//		�X			�V
void sceneLoading::Update()
{
	// �摜�A�j���[�V��������
	static const int AnimeSpeed = 10;
	if (++m_AnimeFrame > AnimeSpeed)
	{
		m_AnimeFrame = 0;
		if (++m_AnimeCount > 5) m_AnimeFrame = 0;
	}

	m_fLoadAngle -= 0.1f;
	m_pImage->SetAngle(m_fLoadAngle);

	// ���[�h������ԏ���
	m_fLoadGaugePercent = m_fLoadGaugePercent*.5f + m_pNewScene->m_fLoadPercentage*.5f;

	// ���[�h�̃R�����g
	strcpy_s(m_LoadComment, 256, m_pNewScene->m_LoadComment);

	// �����[�h���I�������A�V�[�����`�F���W
	if (LoadSceneThreadMgr->isInitializeEnd())
	{
		MainFrameEX->ChangeScene(m_pNewScene);
	}
}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneLoading::Render()
{
	tdnView::Clear();

	//tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom , RS::COPY, 0xff000000);
	//m_pImage->Render(720, 540, 512, 64, 0, m_AnimeCount * 64, 512, 64);
	
	m_pImage->Render(1280 - 256 + 32, 720 - 256 + 32);

	// �Q�[�W���g
	//m_pGauge->Render(720, 620, (int)(m_fLoadGaugePercent * 512), 64, 0, 64, (int)(m_fLoadGaugePercent * 512), 64);
	// �Q�[�W�g
	//m_pGauge->Render(720, 620, 512, 64, 0, 0, 512, 64);

	// ���[�h�̃R�����g
	tdnText::Draw(320, 620, 0xffffffff, "%s", m_LoadComment);
}
//
//=============================================================================================