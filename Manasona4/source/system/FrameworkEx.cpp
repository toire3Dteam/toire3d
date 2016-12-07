#include "FrameworkEx.h"
#include "../Scene/sceneLoading.h"
#include	"../Sound/SoundManager.h"

// ���̉�
FrameworkEx* FrameworkEx::m_pInstance = nullptr;

// �~���b�ł�FPS
#define FPS_1 16.66667
#define FPS_2 33.33333
#define FPS_60 1000

/************************/
//	�������E���
/************************/
FrameworkEx::FrameworkEx()
{
	// �}�C�N���b�܂Œ��ׂ�̂ɕK�v�ȍ\����
	LARGE_INTEGER count;

	// 1�b������̃N���b�N��
	QueryPerformanceFrequency(&count);  // 1�b������N���b�N�����擾
	m_dFreq= (double)count.QuadPart;	// 1�b������N���b�N�J�E���g���i���g��
  
	// �~���b�ō��̃t���[�����l�ߍ���
	QueryPerformanceCounter(&count);		 // �N���b�N�J�E���g��	
	m_dFlameTime = (double)count.QuadPart * 1000 / m_dFreq;// �����N���b�N����{���x���������_�ɕϊ�
	
	// �\�L�p
	m_iFPS = 0;
	m_iRenderFPS = 0;

	m_iWorkFPS = 0;
	m_iWorkRenderFPS = 0;

	// �`��E�X�V�t���O
	m_bRenderFlag = true;
	m_bUpdateFlag = true;
	m_bSlowFlag = false;

	// �Q�[���̃V�[��
	m_scene = NULL;

	// ���[�r�[�t���O
	m_bMoviePlay = false;
}

FrameworkEx::~FrameworkEx()
{
	SAFE_DELETE(m_scene);
}

FrameworkEx * FrameworkEx::GetInstance()
{
	if (FrameworkEx::m_pInstance == nullptr)
	{
		m_pInstance = new FrameworkEx();
	}

	return m_pInstance;
}

void FrameworkEx::Release()
{
	SAFE_DELETE(m_pInstance);
}

/************************/
//	�������E���
/************************/
bool FrameworkEx::Update()
{
	// �}�C�N���b�܂Œ��ׂ�̂ɕK�v�ȍ\����
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);	// �N���b�N�J�E���g���擾	

	// ��b���̕ϐ�
	static 	double	oneSecond =  (double)count.QuadPart * 1000 / m_dFreq;//�@�ŏ��̎��Ԃɍ��킹�� 

	// ���񌻍݂̎��Ԃ��X�V		 	
	m_dCurrentTime = (double)count.QuadPart * 1000 / m_dFreq;
	
	// ��1fps�ɖ����Ȃ��X�V�����Ȃ�
	if (m_dCurrentTime < m_dFlameTime+FPS_1) return false;
	
	//	�o�ߎ���
	double	intervalTime = m_dCurrentTime - m_dFlameTime;

	// 2�b�ȏ㗣��Ă����炷�������x�ōX�V��������̂Ŏ��Ԃ����킷
	if (intervalTime > (FPS_60 * 2))
	{
		m_dFlameTime = m_dCurrentTime;	// �Q�[�����̂��~�܂�悤�ɂ��Ă���Ȃ炠�܂�֌W�Ȃ�����
	}

	// ���̍X�V��2FPS�ȏォ�����Ă��܂����ꍇ �`����~�߂čX�V��������
	if (intervalTime > FPS_1 * 2) m_bRenderFlag = false;
	else m_bRenderFlag = true;
	m_bRenderFlag = true;

	// �V����1FPS�����Z
	// ���̐��l��G�邱�Ƃɂ��1�t���[���̒�����ς����
	m_dFlameTime += FPS_1;


	// ��b�Ԃɂǂꂾ���X�V����Ă��邩�L��
	if (oneSecond < m_dCurrentTime)
	{
		m_iFPS	=	m_iWorkFPS;				// 1�b�ԂɍX�V������
		m_iRenderFPS = m_iWorkRenderFPS;	// 1�b�Ԃɕ`�悵����
		
		m_iWorkFPS = m_iWorkRenderFPS = 0;	// ������

		oneSecond += FPS_60;				// 1�b������p��1�b���Z
	}

	// FPS�J�E���g���Z
	m_iWorkFPS++;

	// �V�X�e���X�V
	tdnInput::Update();
	SoundManager::Update();


	if (FPSCtrl() == true)return true;// FPS�̑���

	// �V�[���̍X�V
	if (m_scene != NULL)
	{
		// �V�[���̍X�V
		m_scene->Update();	
		
		// ���悪�Đ����Ă�����`�悳���Ȃ�
		return true;
	}

	return true;
}

void FrameworkEx::Render()
{
	// �`��t���O�������Ă�����`��
	if (m_bRenderFlag == false)return;

	//�y�`��J�n�z
	tdnSystem::BeginScene();



	//	�V�[���`��
	if (m_scene != NULL) m_scene->Render();
	


#ifdef _DEBUG



	//	�t���[���\��
	tdnText::Draw(tdnSystem::GetScreenSize().right - 128, tdnSystem::GetScreenSize().bottom-36, 0xff00ffff, "FPS %d/%d", m_iFPS, m_iRenderFPS);

#endif
	// �X�g�b�v�E�H�b�`�̕`��
	tdnStopWatch::DrawResult(300, 0);
	//�y�`��I���z
	tdnSystem::EndScene();

	// RenderFPS�J�E���g���Z
	m_iWorkRenderFPS++;
}


// FPS�̑���
bool FrameworkEx::FPSCtrl()
{
	if (KeyBoardTRG(KB_1))
	{
		if (m_bUpdateFlag == false)m_bUpdateFlag = true;
		else m_bUpdateFlag = false;
	}

	// �X�V�t���O�������Ă�����X�V
	if (m_bUpdateFlag == false)
	{
		if (KeyBoardTRG(KB_2)) /*	�X�V���ʂ�	*/;
		else return true;
	}

	if (KeyBoardTRG(KB_3))
	{
		if (m_bSlowFlag == false)
		{
			m_bSlowFlag = true;
		} else
		{
			m_bSlowFlag = false;
		}
	}
	if (m_bSlowFlag == true)
	{
		m_dFlameTime += FPS_1 + FPS_1 + FPS_1;
	}

	return false;
}

//*****************************************************************************************************************************
//		�V�[���̐؂�ւ�
//*****************************************************************************************************************************
void FrameworkEx::ChangeScene(BaseScene* newScene, bool bLoadingScene)
{
	//	���݂̃V�[�����
	if (m_scene != nullptr) delete m_scene;
	//	�V�[���̐؂�ւ���������

	// ���[�f�B���O��ʂ���
	if (bLoadingScene)
	{
		m_scene = new sceneLoading(newScene);
		m_scene->Initialize();	// �����ŁA�J�ڐ��Initialize���Ă�ł���B
	}

	// ���[�f�B���O��ʂȂ�
	else
	{
		m_scene = newScene;

		// Loading�ł�2�dInitialize�j�~
		if (m_scene->m_bLoad) return;

		m_scene->Initialize();
	}
}