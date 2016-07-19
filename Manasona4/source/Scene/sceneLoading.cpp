#include	"TDNLIB.h"
#include	"system/FrameworkEx.h"
#include	"sceneLoading.h"

//=============================================================================================
//		�O	��	�[	�o	��	��	��
bool sceneLoading::isThread;


//=============================================================================================
//		��	��	��	��	�J	��
bool sceneLoading::Initialize(BaseScene *newScene)
{
	m_pImage = new tdn2DObj("DATA/UI/Loading/loading.png");
	m_pGauge = new tdn2DObj("DATA/UI/Loading/loading_gauge.png");
	m_AnimeCount = m_AnimeFrame = 0;
	m_fLoadGaugePercent = 0;

	//	�ʃX���b�h�쐬
	//���̃V�[���̃|�C���^�͌�Ŏg���̂�newScene�ɕۑ����Ă����B
	m_newScene = newScene;

	isThread = true;
	_beginthread(Thread_funk, 0, (void*)m_newScene);

	return true;
}

sceneLoading::~sceneLoading()
{
	delete m_pImage;
	delete m_pGauge;
}
//=============================================================================================


//=============================================================================================
//		�X	��	�b	�h	��	��
void sceneLoading::Thread_funk(void *arg)
{
	BaseScene* scene = (BaseScene*)arg;

	scene->Initialize();	// �������Ŗ{�̂�scene���K�v�Ȏ��Ԃ������鏉�������s��
	scene->m_bLoad = true;	// ���̎��_�Ń��[�h�t���O��ON�B�V�[����؂�ւ���ۂ̓ǂݍ��݂����Ȃ��悤�ɂ���

	//	�X���b�h�I������
	isThread = false;
	_endthread();
}


//
//=============================================================================================

//=============================================================================================
//		�X			�V
bool sceneLoading::Update()
{
	// �摜�A�j���[�V��������
	static const int AnimeSpeed = 10;
	if (++m_AnimeFrame > AnimeSpeed)
	{
		m_AnimeFrame = 0;
		if (++m_AnimeCount > 5) m_AnimeFrame = 0;
	}

	// ���[�h������ԏ���
	m_fLoadGaugePercent = m_fLoadGaugePercent*.5f + m_newScene->m_fLoadPercentage*.5f;

	//���[�h���I�������A�V�[�����`�F���W
	if (!isThread)
	{
		MainFrameEx->ChangeScene(m_newScene);
	}

	return true;
}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneLoading::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom , RS::COPY, 0xff000000);
	m_pImage->Render(720, 540, 512, 64, 0, m_AnimeCount * 64, 512, 64);

	// �Q�[�W���g
	m_pGauge->Render(720, 620, (int)(m_fLoadGaugePercent * 512), 64, 0, 64, (int)(m_fLoadGaugePercent * 512), 64);
	// �Q�[�W�g
	m_pGauge->Render(720, 620, 512, 64, 0, 0, 512, 64);
}
//
//=============================================================================================