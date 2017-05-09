#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneMain.h"
#include "SceneVS.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Window/OptionWindow.h"	// �K�v�ȃE�B���h�E
#include "Window/SoundWindow.h"	// 
#include "Window/GameWindow.h"
#include "Window/SystemWindow.h"
#include "Window/ChallengeWindow.h"
#include "Window/ChallengeSelectWindow.h"
#include "../Scene/sceneResult.h"
#include	"Trophy\TrophyManager.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "Data\PlayerData.h"
#include "Data\CommandData.h"
#include "KeyConfig\KeyConfig.h"

//#include "sceneResult.h"
//#include "SceneSwitch/SceneSwitch.h"
//+------------------------
//	�V�[�����j���[
//+------------------------
int sceneMenu::m_iSelectNo(0);	// �Z���N�g�i���o�[(static�ɂ��邱�Ƃɂ���āA�O��I���������[�h�ɍ��킹���܂܃��j���[�V�[�����n�܂�)
int sceneMenu::m_iCtrlDevice(0);// ���삵�Ă����f�o�C�X
//int sceneMenu::m_iTrainingCtrlDevice(0);// �g���[�j���O�ɓ������v���C���[��ID
//******************************************************************
//		�������E���
//******************************************************************
sceneMenu::sceneMenu() :BaseGameEntity(ENTITY_ID::SCENE_MENU) {}
bool sceneMenu::Initialize()
{	
	// �Z���N�g�i���o�[
	//m_iSelectNo = 0;
	m_iTrainingCtrlDevice = 0;

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
			m_pTips[i] = new TipsCard("�`���[�g���A�����n�߂܂����H", true, true);
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
			m_pWindow[i] = new OptionWindow(Vector2(500, 50));
			break;
		case WINDOW_TYPE::SYSTEM:
			m_pWindow[i] = new SystemWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::GAME:
			m_pWindow[i] = new GameWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::SOUND:
			m_pWindow[i] = new SoundWindow(Vector2(200, 250));
			break;
		case WINDOW_TYPE::CHALLENGE:
			m_pWindow[i] = new ChallengeWindow(Vector2(450, 50));
			break;
		case WINDOW_TYPE::CHALLENGE_SELECT:
			m_pWindow[i] = new ChallengeSelectWindow(Vector2(150, 250));
			break;
		default:
			MyAssert(0, "����ȃE�B���h�E�͂Ȃ�");
			break;
		}
	}
	// �L�[�R���t�B�O
	for (int i = 0; i < (int)SIDE::ARRAY_MAX; i++)
	{
		m_pKeyWindow[i] = new KeyConfig();
	}


	// �ŏ��̈��ڂ̃g���t�B�̉���m�F
	TrophyMgr->CheakPlayCount(PlayerDataMgr->m_PlayerInfo.PlayCount);

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneMenu>(this);
	m_pStateMachine->SetCurrentState(SceneMenuState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�

	// BGM���X�g���[�~���O�Đ�
	bgm->PlayStreamIn("DATA/Sound/BGM/System/ModeSelect.ogg");

	// �|�[�Y�������l�̃f�o�C�XID
	m_iPauseDeviceID = 0;
	m_eCommandSide = SIDE::LEFT;// ���͎g���Ă��Ȃ�

	// (TODO)��
	CommandMgr->SetReplayFlag(false);

	return true;
}

sceneMenu::~sceneMenu()
{
	// �g���t�B�[�̉��o���~�߂�
	TrophyMgr->Stop();

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
	
	// �L�[�R���t�B�O
	for (int i = 0; i < (int)SIDE::ARRAY_MAX; i++)
	{
		SAFE_DELETE(m_pKeyWindow[i]);
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

	// �g���t�B�[
	TrophyMgr->Update();

	{
		// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
		m_pStateMachine->Update();


		if (KeyBoardTRG(KB_SPACE))
		{
			CommandMgr->SetReplayFlag(true);

			// �o�C�i��
			FILE *fp;
			MyAssert(fopen_s(&fp, "DATA/Replay/SelectData.bin", "rb") == 0, "�f�f�h��(��])\n�Z�[�u�f�[�^�ǂݍ��݂Ɏ��s�����I");	// �܂��~�܂邱�Ƃ͂Ȃ��Ǝv�����c

																											// ���E���h��
			int l_iRoundNum;
			fread_s((LPSTR)&l_iRoundNum, sizeof(int), sizeof(int), 1, fp);
			PlayerDataMgr->m_ConfigData.iRoundNumType = l_iRoundNum;

			// ��������
			int l_iRoundTime;
			fread_s((LPSTR)&l_iRoundTime, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iRoundTime = l_iRoundTime;

			// �X�e�[�W
			int l_iStage;
			fread_s((LPSTR)&l_iStage, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->eStage = (STAGE)l_iStage;

			// �Ȃ̔ԍ�
			int l_iBattleMusicID;
			fread_s((LPSTR)&l_iBattleMusicID, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iBattleMusicID = l_iBattleMusicID;

			FOR((int)SIDE::ARRAY_MAX)
			{
				// �g�p�����
				int l_iCharacter;
				fread_s((LPSTR)&l_iCharacter, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].eCharacter = (CHARACTER)l_iCharacter;
				// �g�p�p�[�g�i�[
				int l_iPartner;
				fread_s((LPSTR)&l_iPartner, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].ePartner = (PARTNER)l_iPartner;
			}

			// �t�@�C������
			fclose(fp);

			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

			// �`���[�g���A���ł͂Ȃ�
			SelectDataMgr->Get()->bTutorial = false;

			// �g���[�j���O�ł��Ȃ�
			SelectDataMgr->Get()->bTraining = false;

			// �`�������W�ł��Ȃ�
			SelectDataMgr->Get()->bChallenge = false;
			MainFrameEX->ChangeScene(new sceneVS);
			return;
		}

#ifdef DEBUG


		// �G���^�[�Ń��[�v
		if (KeyBoard(KB_ENTER) == 2)
		{
			ResultData data;
			data.eWinnerSide = SIDE::LEFT;
			data.iMaxDamage = 2500;
			data.iRemainingHP = 80;
			data.iElapsedTime = 35;

			MainFrameEX->ChangeScene(new sceneResult(data));
			return;
		}

		if (KeyBoardTRG(KB_SPACE))
		{
			CommandMgr->SetReplayFlag(true);

			// �o�C�i��
			FILE *fp;
			MyAssert(fopen_s(&fp, "DATA/Replay/SelectData.bin", "rb") == 0, "�f�f�h��(��])\n�Z�[�u�f�[�^�ǂݍ��݂Ɏ��s�����I");	// �܂��~�܂邱�Ƃ͂Ȃ��Ǝv�����c

			// ���E���h��
			int l_iRoundNum;
			fread_s((LPSTR)&l_iRoundNum, sizeof(int), sizeof(int), 1, fp);
			PlayerDataMgr->m_ConfigData.iRoundNumType = l_iRoundNum;

			// ��������
			int l_iRoundTime;
			fread_s((LPSTR)&l_iRoundTime, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iRoundTime = l_iRoundTime;

			// �X�e�[�W
			int l_iStage;
			fread_s((LPSTR)&l_iStage, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->eStage = (STAGE)l_iStage;

			// �Ȃ̔ԍ�
			int l_iBattleMusicID;
			fread_s((LPSTR)&l_iBattleMusicID, sizeof(int), sizeof(int), 1, fp);
			SelectDataMgr->Get()->iBattleMusicID = l_iBattleMusicID;

			FOR((int)SIDE::ARRAY_MAX)
			{
				// �g�p�����
				int l_iCharacter;
				fread_s((LPSTR)&l_iCharacter, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].eCharacter = (CHARACTER)l_iCharacter;
				// �g�p�p�[�g�i�[
				int l_iPartner;
				fread_s((LPSTR)&l_iPartner, sizeof(int), sizeof(int), 1, fp);
				SelectDataMgr->Get()->tagSideDatas[i].ePartner = (PARTNER)l_iPartner;
			}

			// �t�@�C������
			fclose(fp);

			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

			// �`���[�g���A���ł͂Ȃ�
			SelectDataMgr->Get()->bTutorial = false;

			// �g���[�j���O�ł��Ȃ�
			SelectDataMgr->Get()->bTraining = false;

			// �`�������W�ł��Ȃ�
			SelectDataMgr->Get()->bChallenge = false;
			MainFrameEX->ChangeScene(new sceneMain);
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
	for (int i = 0; i < (int)SIDE::ARRAY_MAX; i++)
	{
		// �L�[�E�B���h�E
		m_pKeyWindow[i]->Render();
	}

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE::ARRAY_END; i++)
	{
		m_pTips[i]->Render();
	}
	
	// �g���t�B�[
	TrophyMgr->Render();

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

