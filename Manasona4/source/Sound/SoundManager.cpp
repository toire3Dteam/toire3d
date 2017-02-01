#include	"TDNLIB.h"
#include	"SoundManager.h"


//����������������������������������������������������������������������������������������������������������������
//
//								�T�E���h�Ăяo��ID�ꗗ														�@����
//
//����������������������������������������������������������������������������������������������������������������

//==============================================================================================
//				S	E
//==============================================================================================
const SE_Manager::DATA all_dataS[]
{
	//{ "�ʏ�1�i�q�b�g",			"DATA/Sound/SE/Battle/attack1.wav",	2, false },
	//{ "�ʏ�2�i�q�b�g",			"DATA/Sound/SE/Battle/attack2.wav",	2, false },
	//{ "�ʏ�3�i�q�b�g",			"DATA/Sound/SE/Battle/attack3.wav",	2, false },
	{ "�q�b�g1",					"DATA/Sound/SE/P4U2/hit1.wav",	2, false },
	{ "�q�b�g2",					"DATA/Sound/SE/P4U2/hit2.wav",	2, false },
	{ "�q�b�g3",					"DATA/Sound/SE/P4U2/hit3.wav",	8, false },
	{ "�q�b�g4",					"DATA/Sound/SE/BlazBlue/hit1.wav", 2, false },
	{ "�q�b�g5",					"DATA/Sound/SE/BlazBlue/hit2.wav", 2, false },
	{ "�q�b�g6",					"DATA/Sound/SE/BlazBlue/hit3.wav", 2, false },
	{ "�P�c�h����1",				"DATA/Sound/SE/Battle/Aniki/dram1.wav", 8, false },
	{ "�g�X�~�^�}",		"DATA/Sound/SE/Battle/plasma.wav",				1, false },
	{ "�a��1",						"DATA/Sound/SE/P4U2/cut2.wav", 2, false },
	{ "�a��2",						"DATA/Sound/SE/BlazBlue/cut1.wav", 6, false },
	{ "������q�b�g",				"DATA/Sound/SE/Battle/gutyo.wav", 8, false },
	{ "��q�[�z�[�q�b�g",			"DATA/Sound/SE/Battle/suikomi.wav", 1, false },
	//{ "�󒆉��U���q�b�g",			"DATA/Sound/SE/Battle/hit2.wav", 2, false },
	//{ "�S�u���C�N",				"DATA/Sound/SE/Battle/all_break.wav",	2, false },
	//{ "�`���[�W",					"DATA/Sound/SE/Battle/charge.wav",2, false },
	//{ "�`���[�W���܂�",			"DATA/Sound/SE/Battle/fill.wav",2, false },
	{ "�t�B�j�b�V����",				"DATA/Sound/SE/Battle/finish_base.wav",2, false },
	//{ "�t�B�j�b�V����",				"DATA/Sound/SE/Battle/finish2.wav",	2, false },
	{ "�t�B�j�b�V����",				"DATA/Sound/SE/Battle/finish3.wav",	2, false },
	{ "�������Ƃ�",					"DATA/Sound/SE/Battle/smack_down.wav",	2, false },
	{ "�W�����v",					"DATA/Sound/SE/BlazBlue/jump.wav",	2, false },
	{ "��U��1",					"DATA/Sound/SE/BlazBlue/whiff1.wav", 2, false },
	{ "��U��2",					"DATA/Sound/SE/BlazBlue/whiff2.wav", 2, false },
	//{ "��U��3",					"DATA/Sound/SE/BlazBlue/whiff3.wav", 2, false },
	{ "��U��4",					"DATA/Sound/SE/BlazBlue/whiff4.wav", 2, false },
	{ "��U��5",					"DATA/Sound/SE/BlazBlue/whiff5.wav", 2, false },
	{ "��U��6",					"DATA/Sound/SE/BlazBlue/whiff6.wav", 2, false },
	{ "�A�C���[�h����",				"DATA/Sound/SE/BlazBlue/drill.wav", 2, false },
	{ "�G�X�P�[�v",					"DATA/Sound/SE/Battle/escape.wav",	2, false },
	{ "�K�[�h��",					"DATA/Sound/SE/BlazBlue/guard1.wav", 8, false },
	{ "�K�[�h��",					"DATA/Sound/SE/BlazBlue/guard2.wav", 4, false },
	{ "�K�[�h��",					"DATA/Sound/SE/BlazBlue/guard3.wav", 4, false },
	{ "�󒆃_�b�V��",				"DATA/Sound/SE/BlazBlue/aerial_dash.wav", 2, false },
	{ "�J�E���^�[����",				"DATA/Sound/SE/Battle/counter_success.wav",	1, false },
	//{ "�u���[�L1",				"DATA/Sound/SE/Battle/brake1.wav",	2, false },
	{ "�_�b�V��",					"DATA/Sound/SE/Battle/dash.wav", 2, false },
	{ "�y���\�i����",				"DATA/Sound/SE/Battle/kyuin.wav", 2, false },
	{ "�͂�",						"DATA/Sound/SE/Battle/wriff_catch.wav", 1, false },
	{ "�͂ݐ���",					"DATA/Sound/SE/Battle/catch.wav", 1, false },
	{ "��������",					"DATA/Sound/SE/Battle/throw_escape.wav", 1, false },	
	{ "�I�[�o�[�h���C�u�J�n",		"DATA/Sound/SE/BlazBlue/over_drive.wav", 2, false },
	{ "�I�[�o�[�h���C�u�I��",		"DATA/Sound/SE/BlazBlue/over_drive_end.wav", 2, false },
	{ "�K�E����",					"DATA/Sound/SE/P4U2/special1.wav", 2, false },
	{ "���K�E����",					"DATA/Sound/SE/P4U2/special2.wav", 2, false },
	{ "���U���g1",					"DATA/Sound/SE/Result/result1.wav", 1, false },
	{ "���U���g2",					"DATA/Sound/SE/Result/result2.wav", 1, false },
	{ "���U���g3",					"DATA/Sound/SE/Result/result3.wav", 1, false },

	{ "�^�C�g���X�^�[�g",			"DATA/Sound/SE/System/start.wav", 1, false },
	{ "�J�[�\��1",					"DATA/Sound/SE/P4U2/cursor.wav", 2, false },
	{ "�J�[�\��2",					"DATA/Sound/SE/P4U2/cursor2.wav", 2, false },
	{ "����1",						"DATA/Sound/SE/P4U2/decide.wav", 2, false },
	{ "�L�����Z��1",				"DATA/Sound/SE/P4U2/cancel.wav", 2, false },
	{ "�X�e�b�v�N���A",				"DATA/Sound/SE/P4U2/gagin.wav", 3, false },
	{ "�I�[���N���A",				"DATA/Sound/SE/P4U2/clear.wav", 1, false },
	{ "�w��",						"DATA/Sound/SE/P4U2/money.wav", 2, false },
	{ "�g���t�B�[",					"DATA/Sound/SE/System/trophy.wav", 1, false },

	{ "�t�F�C�g�C�Y�^�[�j���O",		"DATA/Sound/SE/BlazBlue/aerial_dash.wav",				1, false },
	{ "���x��",						"DATA/Sound/SE/Battle/appear.wav",				1, false },
	{ "�A�N�V����",					"DATA/Sound/SE/Battle/fill.wav",				1, false },

	//{ "�t�F�C�g�C�Y�^�[�j���O",		"DATA/Sound/SE/BlazBlue/voice/the_wheel.wav",	1, false },
	//{ "���x��",						"DATA/Sound/SE/BlazBlue/voice/revel.wav",		1, false },
	//{ "����",						"DATA/Sound/SE/BlazBlue/voice/one.wav",			1, false },
	//{ "�c�[",						"DATA/Sound/SE/BlazBlue/voice/two.wav",			1, false },
	//{ "�X���[",						"DATA/Sound/SE/BlazBlue/voice/three.wav",		1, false },
	//{ "�A�N�V����",					"DATA/Sound/SE/BlazBlue/voice/action.wav",		1, false },
};

//==============================================================================================
//				B	G	M
//==============================================================================================
const BGM_Manager::DATA all_dataB[]
{
	{ "END", nullptr, false }
};



//**************************************************************************************************************
//
//		�T�E���h�Ǘ��N���X(winmain��framework�Ŏg������)
//
//**************************************************************************************************************
//=============================================================================================
//		��	��	��
void SoundManager::Initialize()
{
	se = new SE_Manager;
	se->Initialize();
	bgm = new BGM_Manager;
	bgm->Initialize();
	voice = new VOICE_Manager;
	voice->Initialize();

	// �{�����[���̓ǂݍ���
	//std::ifstream ifs("DATA/Sound/volume.txt");
	//MyAssert(ifs, "�G���[: �{�����[���ݒ�e�L�X�g�������ĂȂ�");
	//
	//// �{�����[���ǂݍ���
	//char skip[8];	// �ǂݔ�΂��p
	//ifs >> skip;
	//ifs >> m_SEVolume;
	//ifs >> skip;
	//ifs >> m_BGMVolume;
}

//=============================================================================================
//		��		��
void SoundManager::Release()
{
	SAFE_DELETE(se);
	SAFE_DELETE(bgm);
	SAFE_DELETE(voice);
}

//=============================================================================================
//		�X�V
void SoundManager::Update()
{
	se->Update();
	bgm->Update();
}


///********************************************************************************************
//		���X�i�[���ݒ�I
void SoundManager::SetListenerPos(const Vector2 &pos)
{
	se->SetListener(pos);
}

void SoundManager::SetBaseVolume(int BGMVol, int SEVol, int VoiceVol)
{
	// ��{�{�����[���ݒ�
	bgm->SetBaseVolume(BGMVol * .01f);
	se->SetBaseVolume(SEVol * .01f);
	voice->SetBaseVolume(VoiceVol * .01f);
}


//**************************************************************************************************************
//
//		SE�Ǘ��N���X
//
//**************************************************************************************************************


//=============================================================================================
//		��	��	��
void SE_Manager::Initialize()
{
	play_manager = new tdnSoundSE;

	for (int i = 0; i < _countof(all_dataS); i++)
	{
		if (strcmp(all_dataS[i].id, "END") == 0) break;	// �I�[

		ID[all_dataS[i].id] = i;
		play_manager->Set(i, all_dataS[i].play_simultaneously, all_dataS[i].file_name, all_dataS[i].b3D);
	}

	// ���X�i�[�̏����ݒ�
	m_ListenerPos = Vector2(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f);
	//play_manager->SetListenerAll(
	//	Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// ���X�i�[���W(��ʂ̐^�񒆂Ɖ��肷��)
	//	Vector3(1, 0, 0),	// ���ʃx�N�g��
	//	Vector3(0, 1, 0),	// ����x�N�g��
	//	Vector3(0, 0, 0)	// �ړ��l
	//	);
}
//
//=============================================================================================



//=============================================================================================
//		��		��
SE_Manager::~SE_Manager()
{
	delete play_manager;
}
//
//=============================================================================================



//=============================================================================================
//		�X		�V
void SE_Manager::Update()
{
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		��		��
int SE_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		//play_manager->SetPan(data_num, DSBPAN_CENTER);
		//play_manager->SetVolume(data_num, DSBVOLUME_MAX);
		play_manager->Play(data_num, loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		// �X�e���I�蓮�Őݒ肵�Ă݂�
		static const int DSBPAN_WIDTH = 10000;
		const int pan = (int)((DSBPAN_WIDTH / 8)*((pos.x - m_ListenerPos.x) / (tdnSystem::GetScreenSize().right / 2)));
		const int vol = DSBVOLUME_MAX + (int)((DSBVOLUME_MIN / 32) * (sqrtf((m_ListenerPos.x - pos.x)*(m_ListenerPos.x - pos.x)) / m_ListenerPos.x) + (DSBVOLUME_MIN / 16) * (sqrtf((m_ListenerPos.y - pos.y)*(m_ListenerPos.y - pos.y)) / m_ListenerPos.y));
		play_manager->SetPan(data_num, pan);
		play_manager->SetVolume(data_num, vol);

		return play_manager->Play(data_num, loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play(LPSTR _ID, bool loop)
{
	/*
	std::map<char*, int> map;
	map["HOGE0"] = 0;
	map["HOGE1"] = 1;

	const char* str1 = "HOGE1";
	std::string str2 = "HOGE1";

	int a = map[(char*)str1]			// 1����������
	int b = map[(char*)str2.c_str()];	// 0����������
	*/

	// ���������������Ƃ����ƘA�z�z��͑z���ȏ�ɐM�p�ł��Ȃ��B

	return Play_in(ID[_ID], loop);
}

int SE_Manager::Play(LPSTR _ID, const Vector2 &pos, const Vector2 &move, bool loop)
{
	return Play_in(ID[_ID], pos, move, loop);
}

void SE_Manager::Stop(LPSTR _ID, int no)
{
	play_manager->Stop(ID[_ID], no);
}

void SE_Manager::Stop_all()
{
	play_manager->AllStop();
}

void SE_Manager::SetTone(LPSTR _ID, int tone)
{
	play_manager->SetSpeed(ID[_ID], 1.0f + (float)tone*.048f);
}

void SE_Manager::SetPos(LPSTR _ID, int no, const Vector2 &pos)
{
	// �X�e���I�蓮�Őݒ肵�Ă݂�
	static const int DSBPAN_WIDTH = 10000;
	const int pan = (int)((DSBPAN_WIDTH / 8)*((pos.x - m_ListenerPos.x) / (tdnSystem::GetScreenSize().right / 2)));
	const int vol = DSBVOLUME_MAX + (int)((DSBVOLUME_MIN / 32) * (sqrtf((m_ListenerPos.x - pos.x)*(m_ListenerPos.x - pos.x)) / m_ListenerPos.x) + (DSBVOLUME_MIN / 16) * (sqrtf((m_ListenerPos.y - pos.y)*(m_ListenerPos.y - pos.y)) / m_ListenerPos.y));
	play_manager->SetPan(no, pan);
	play_manager->SetVolume(no, vol);
}

void SE_Manager::SetListener(const Vector2 &pos)
{
	m_ListenerPos = pos;
	//play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	//play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================






//**************************************************************************************************************
//
//		BGM�Ǘ��N���X
//
//**************************************************************************************************************


//=============================================================================================
//		��	��	��
void BGM_Manager::Initialize()
{
	play_manager = new tdnSoundBGM;
	m_bPlayStream = false;

	for (int i = 0;; i++)
	{
		if (strcmp(all_dataB[i].id, "END") == 0) break;	// �I�[

		ID[all_dataB[i].id] = i;
		play_manager->Set(i, all_dataB[i].file_name, all_dataB[i].b3D);
	}

	// ���X�i�[�̏����ݒ�
	play_manager->SetListenerAll(
		Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// ���X�i�[���W(��ʂ̐^�񒆂Ɖ��肷��)
		Vector3(1, 0, 0),	// ���ʃx�N�g��
		Vector3(0, 1, 0),	// ����x�N�g��
		Vector3(0, 0, 0)	// �ړ��l
		);
}
//
//=============================================================================================



//=============================================================================================
//		��		��
BGM_Manager::~BGM_Manager()
{
	delete play_manager;
}
//
//=============================================================================================

//=============================================================================================
//		�X		�V
void BGM_Manager::Update()
{
	play_manager->Update();
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		��		��
void BGM_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->Play(data_num, loop);
	}
}

void BGM_Manager::Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->Play(data_num, pos, front, move, loop);
	}
}

void BGM_Manager::Play(LPSTR _ID, bool loop)
{
	Play_in(ID[_ID], loop);
}

void BGM_Manager::Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	return Play_in(ID[_ID], pos, front, move, loop);
}

void BGM_Manager::Stop(LPSTR _ID)
{
	play_manager->Stop(ID[_ID]);
}

void BGM_Manager::Stop_all()
{
	play_manager->AllStop();
}

void BGM_Manager::Fade_in(LPSTR _ID, float fade_speed, bool loop)
{
	play_manager->FadeIn(ID[_ID], fade_speed, loop);
}

void BGM_Manager::Fade_out(LPSTR _ID, float fade_speed)
{
	play_manager->FadeOut(ID[_ID], fade_speed);
}

void BGM_Manager::Cross_fade(LPSTR inID, LPSTR outID, float fade_speed, bool loop)
{
	play_manager->CrossFade(ID[inID], ID[outID], fade_speed, tdnSoundBGM::CROSS_FADE_TYPE::END_OF_ETERNITY, loop);
}

void BGM_Manager::Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop)
{
	play_manager->CrossFade(ID[inID], ID[outID], in_speed, out_speed, tdnSoundBGM::CROSS_FADE_TYPE::END_OF_ETERNITY, loop);
}

void BGM_Manager::SetListener(const Vector2 &pos, const Vector2 &move)
{
	play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================




//**************************************************************************************************************
//
//		VOICE�Ǘ��N���X
//
//**************************************************************************************************************

void VOICE_Manager::Load(CHARACTER eCharacter)
{
	// �t�@�C���p�X����
	std::string l_sFileName("");
	switch (eCharacter)
	{
	case CHARACTER::ANIKI:
		l_sFileName = "DATA/Sound/Voice/Aniki/list.txt";
	default:
		break;
	}

	// �L�����N�^�[�̃{�C�X�������[�h����
	if (l_sFileName == "") return;

	std::ifstream ifs(l_sFileName);
	MyAssert(ifs, "�{�C�X�̃e�L�X�g�����ĂȂ�");

	static const std::map<std::string, VOICE_TYPE> IDList
	{
		{ "[[�G�X�P�[�v]]",					VOICE_TYPE::ESCAPE },
		{ "[[�������A�o��]]",				VOICE_TYPE::OVERDRIVE_ONEMORE },
		{ "[[�X�^���h����]]",				VOICE_TYPE::STAND },
		{ "[[�o�[�X�g�o��]]",				VOICE_TYPE::OVERDRIVE_BURST },
		{ "[[�ʏ�U��1�i��]]",				VOICE_TYPE::RUSH1 },
		{ "[[�ʏ�U��2�i��]]",				VOICE_TYPE::RUSH2 },
		{ "[[�ʏ�U��3�i��]]",				VOICE_TYPE::RUSH3 },
		{ "[[�΋�U��]]",					VOICE_TYPE::ANTI_AIR },
		{ "[[���i�U��]]",					VOICE_TYPE::DOKKOI_ATTACK },
		{ "[[���Ⴊ�݉��i�U��]]",			VOICE_TYPE::SQUAT_ATTACK },
		{ "[[�����΂炢�U��]]",				VOICE_TYPE::DOWN_ATTACK },
		{ "[[�󒆍U��]]",					VOICE_TYPE::AERIAL_ATTACK },
		{ "[[�t�؂�U��]]",					VOICE_TYPE::INVINCIBLE_ATTACK },
		{ "[[�L�����ŗL�n��j���[�g����]]", VOICE_TYPE::SKILL_LAND },
		{ "[[�L�����ŗL2]]",				VOICE_TYPE::SKILL2 },
		{ "[[�L�����ŗL3]]",				VOICE_TYPE::SKILL3 },
		{ "[[�L�����ŗL4]]",				VOICE_TYPE::SKILL4 },
		{ "[[�L�����ŗL�n�サ�Ⴊ��]]",		VOICE_TYPE::SKILL_SQUAT },
		{ "[[�L�����ŗL�󒆃j���[�g����]]", VOICE_TYPE::SKILL_AERIAL },
		{ "[[�L�����ŗL�󒆉�]]",			VOICE_TYPE::SKILL_AERIALDROP },
		{ "[[�͂�]]",						VOICE_TYPE::THROW },
		{ "[[��������]]",					VOICE_TYPE::THROW_SUCCESS },
		{ "[[�K�E�Z]]",						VOICE_TYPE::HEAVEHO_DRIVE },
		{ "[[�K�E�Z2]]",					VOICE_TYPE::HEAVEHO_DRIVE2 },
		{ "[[���K�E�Z]]",					VOICE_TYPE::HEAVEHO_DRIVE_OVERFLOW },
		{ "[[�N���オ��]]",					VOICE_TYPE::REVERSAL },
		{ "[[�m�b�N�o�b�N]]",				VOICE_TYPE::KNOCK_BACK },
		{ "[[�m�b�N�o�b�N2]]",				VOICE_TYPE::KNOCK_BACK2 },
		{ "[[�m�b�N�_�E��]]",				VOICE_TYPE::KNOCK_DOWN },
		{ "[[�m�b�N�_�E��2]]",				VOICE_TYPE::KNOCK_DOWN2 },
		{ "[[���������炢]]",				VOICE_TYPE::DOWN_FALL },
		{ "[[����]]",						VOICE_TYPE::DIE },
		{ "[[�|������]]",					VOICE_TYPE::INTRO },
		{ "[[����]]",						VOICE_TYPE::WIN }
	};

	int i(0);

	while (!ifs.eof())
	{
		// ID�ǂݍ���
		char read[64];
		ifs >> read;

		// �G���[�`�F�b�N
		MyAssert(read[0] == '[', "�{�C�X�f�[�^�̃e�L�X�g������Ă�");

		std::string ID(read);
		m_mID[(int)eCharacter][IDList.at(ID)] = i;

		// �t�@�C�����ǂݍ���
		char szFileName[MAX_PATH];
		ifs >> szFileName;

		// SE���Z�b�g����
		m_pPlayManager->Set(i, 2, (tdnFile::GetDirectoryPath(l_sFileName.c_str()) + "/" + szFileName).c_str());

		i++;
	}
}

//=============================================================================================
//		��	��	��
void VOICE_Manager::Initialize()
{
	m_pPlayManager = std::make_unique<tdnSoundSE>();

	FOR((int)CHARACTER::END) m_mID[i].clear();
}
void VOICE_Manager::InitializeCharacterVoice(CHARACTER eCharacter)
{
	// ���ɏ������ς�
	if (!m_mID[(int)eCharacter].empty()) return;

	// �L�����N�^�[�̃{�C�X�������[�h����
	Load(eCharacter);
}
//
//=============================================================================================

//=============================================================================================
//		��		��
void VOICE_Manager::PlayIn(int data_num)
{
	m_pPlayManager->Play(data_num);
}

void VOICE_Manager::Play(VOICE_TYPE eType, CHARACTER eCharacter)
{
	// ���[�h����Ă��Ȃ���Δ�����
	if (m_mID[(int)eCharacter].count(eType) == 0) return;

	// �Đ�
	PlayIn(m_mID[(int)eCharacter][eType]);
}
bool VOICE_Manager::isPlay(VOICE_TYPE eType, CHARACTER eCharacter, int no)
{
	// ���[�h����Ă��Ȃ���Δ�����
	if (m_mID[(int)eCharacter].count(eType) == 0) return false;

	return m_pPlayManager->isPlay(m_mID[(int)eCharacter][eType], no);
}
//
//=============================================================================================





//=============================================================================================
//		��		��
SE_Manager *se;
BGM_Manager *bgm;
VOICE_Manager *voice;
//
//=============================================================================================



MyMusicManager::MyMusicManager(MY_MUSIC_ID id) :m_bPlay(false), m_pStreamSound(nullptr), m_pHeaveHoStream(nullptr), m_bHeaveHoPlay(false)
{
	std::string directory = "DATA/Sound/BGM/MyMusic";
	switch (id)
	{
	case MY_MUSIC_ID::SELECT:
		directory += "/Select";
		break;
	case MY_MUSIC_ID::SENJO:
		directory += "/Senjo";
		break;
	case MY_MUSIC_ID::SYUTEN:
		directory += "/Syuten";
		break;
	case MY_MUSIC_ID::A:
		directory += "/A";
		break;
	default:
		assert(0);
		break;
	}

	// �t�@�C���ǂݍ���
	std::ifstream ifs((char*)(directory + "/info.txt").c_str());
	MyAssert(ifs, "�G���[: �I���Ȃ̓ǂݍ��݂Ɏ��s");

	// �I�[�܂�
	while (!ifs.eof())
	{
		char MusicName[256], FileName[256], path[256];
		int percent;

		// �I���ȏ��ǂݎ��
		ifs >> MusicName;
		ifs >> FileName;
		sprintf_s(path, 256, "%s/%s", directory.c_str(), FileName);	// �p�X�쐬
		ifs >> percent;

		// ���X�g�ɓ˂�����
		m_list.push_back(new MyMusic(MusicName, path, percent));
	}

}

MyMusicManager::~MyMusicManager()
{
	for (auto it : m_list) delete it;	// ���X�g����
	
	// �Ȃ̃X�g�b�v
	if(m_bPlay)m_pStreamSound->Stop();	
	if(m_bHeaveHoPlay)m_pHeaveHoStream->Stop();
}

void MyMusicManager::Play()
{
	// 2�d�Đ��h�~
	if (m_bPlay) return;

	// ���\�A���ȃR�[�h�Ȃ̂ŁA�C������������x�ɒ���
	int count(0);
	for (auto &it : m_list)
	{
		// �S�Ȃ̃p�[�Z���g�̐��Ԃ�A�������J�E���g����
		count += it->percent;
	}

	// �����i�[�̈�m��
	int *numbers = new int[count];

	int count2(0);
	for (UINT i = 0; i < m_list.size();i++)
	{
		for (int remain = m_list[i]->percent - 1; remain >= 0; remain--)
		{
			// �����i�[�̈�ɋȂ̔ԍ������Ă���
			numbers[count2++] = i;
		}
	}

	// ���̐����i�[�̈�̒����烉���_���Œ��I�A�������������Đ�
	int r = (true) ? tdnRandom::Get(0, count - 1) : rand() % count;
	m_pStreamSound = bgm->PlayStream((char*)m_list[numbers[r]]->path.c_str());
	m_bPlay = true;	// �Đ��t���OON

	// ���
	delete[] numbers;
}

void MyMusicManager::PlayHeaveHo()
{
	// ���ݗ����Ă��ƒ��K�E��BGM���N���X�t�F�[�h������
	m_pStreamSound->SetMode(STR_FADEOUT, 16);
	m_bPlay = false;

	m_pHeaveHoStream = bgm->PlayStream("DATA/Sound/BGM/battle/HeaveHo.ogg");
	m_bHeaveHoPlay = true;
}