#pragma once

//********************************************************************************************************************
//
//		�N���X�̐錾
//
//********************************************************************************************************************
//		�T�E���h�}�l�[�W���[(�O���[�o�����̂Ŋ֐��Ăяo��)
//****************************************************************************************************************

class SoundManager
{
public:
	// �֐�
	static void Initialize();
	static void Release();
	static void Update();
	static void SetListenerPos(const Vector2 &pos);

	static void SetBaseVolume(int BGMVol, int SEVol, int VoiceVol);
};

class iexSoundBase;
class tdnSoundSE;
class tdnSoundBGM;

class SE_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*�^�Ŕԍ���U�蕪����

	tdnSoundSE *play_manager;	// iexSound
	Vector2 m_ListenerPos;		// ��������l�̍��W


	int Play_in(int data_num, bool loop);
	int Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop);

public:

	//===============================================
	//	���f�[�^
	//===============================================
	struct	DATA{
		LPSTR id;					// �Ăяo���ۂɎg�p���鎯�ʃR�[�h
		LPSTR file_name;			// �t�@�C���p�X
		int play_simultaneously;	// ����̉��𕡐��Đ�����ő�l
		bool b3D;					// 3D�T�E���h�t���O
	};


	//===============================================
	//	�������Ɖ��
	//===============================================
	SE_Manager(){}
	~SE_Manager();
	void Initialize();

	//===============================================
	//	�X		�V
	//===============================================
	void Update();


	//===============================================
	//	��		��
	//===============================================
	bool isPlay(LPSTR _ID, int no){ return play_manager->isPlay(ID[_ID], no); }
	int Play(LPSTR _ID, bool loop = false);																			// �ȈՔ�																								// �{�����[���ݒ��
	int Play(LPSTR _ID, const Vector2 &pos, const Vector2 &move = Vector2(0, 0), bool loop = false);				// 3D�ݒ��
	void SetTone(LPSTR _ID, int tone);
	void Stop(LPSTR _ID, int no);																					// Play�ŕԂ��Ă������l������
	void Stop_all();																								// �S���~�߂�
	void SetFX(LPSTR _ID, int no, DXA_FX flag);
	void SetPos(LPSTR _ID, int no, const Vector2 &pos);
	void SetBaseVolume(float volume){ play_manager->SetBaseVolume(volume); }
	void SetListener(const Vector2 &pos);			// ���X�i�[���
};

class BGM_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*�^�Ŕԍ���U�蕪����

	tdnSoundBGM *play_manager;	// iexSound

	// �X�g���[�~���O�p
	tdnStreamSound *m_pStreamSound;
	bool m_bPlayStream;

	void Play_in(int data_num, bool loop);
	void Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop);

public:

	//===============================================
	//	���f�[�^
	//===============================================
	struct	DATA{
		LPSTR id;					// �Ăяo���ۂɎg�p���鎯�ʃR�[�h
		LPSTR file_name;			// �t�@�C���p�X
		bool b3D;
	};


	//===============================================
	//	�������Ɖ��
	//===============================================
	void Initialize();
	~BGM_Manager();

	//===============================================
	//	�X		�V
	//===============================================
	void Update();


	//===============================================
	//	��		��
	//===============================================
	void Play(LPSTR _ID, bool loop = true);																											// �ȈՔ�																								// �{�����[���ݒ��
	void Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front = Vector3(0, 0, -1), const Vector3 &move = Vector3(0, 0, 0), bool loop = true);	// 3D�ݒ��
	void Stop(LPSTR _ID);																															// Play�ŕԂ��Ă������l������
	void Stop_all();																																// �S���~�߂�

	void Fade_in(LPSTR _ID, float fade_speed, bool loop = true);
	void Fade_out(LPSTR _ID, float fade_speed);
	void Cross_fade(LPSTR inID, LPSTR outID, float fade_speed = .0075f, bool loop = true);
	void Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop);
	bool isPlay(LPSTR _ID);
	void SetSpeed(LPSTR _ID, float speed){ play_manager->SetSpeed(ID[_ID], speed); }
	void SetBaseVolume(float volume)
	{
		play_manager->SetBaseVolume(volume); 

		if (m_bPlayStream)
		{
			m_pStreamSound->SetVolume(volume);
		}
	}
	void SetVolume(LPSTR _ID, float volume){ play_manager->SetVolume(ID[_ID], volume); }
	void SetListener(const Vector2 &pos, const Vector2 &move);											// ���X�i�[���
	tdnStreamSound *PlayStream(LPSTR filename){ return play_manager->PlayStream(filename); }
	tdnStreamSound *PlayStream(LPSTR filename, BYTE mode, int param){ return play_manager->PlayStream(filename, mode, param); }
	void PlayStreamIn(LPSTR filename, bool bFadeIn = false)
	{
		if (m_bPlayStream) m_pStreamSound->Stop();
		if (bFadeIn)m_pStreamSound = play_manager->PlayStream(filename, STR_FADEIN, 8);
		else m_pStreamSound = play_manager->PlayStream(filename);
		m_bPlayStream = true;
	}
	void StopStreamIn(bool bFadeOut = false)
	{
		if (m_bPlayStream)
		{
			if (bFadeOut)m_pStreamSound->SetMode(STR_FADEOUT, 48);
			else m_pStreamSound->Stop();
			m_bPlayStream = false;
		}
	}
};


enum class MY_MUSIC_ID
{
	SELECT,		// �Z���N�g
	SENJO,		// ���
	SYUTEN,		// �I�_
	A,		// A�X�e�[�W
	MAX
};

class MyMusicManager
{
public:
	MyMusicManager(MY_MUSIC_ID id);
	~MyMusicManager();

	// �I���ȍ\����
	struct MyMusic
	{
		std::string MusicName;	// �Ȗ�
		std::string path;		// ���̃t�@�C���ւ̃p�X
		int percent;			// �Đ���
		MyMusic(char *MusicName, char *path, int percent) :MusicName(MusicName), path(path), percent(percent){}
	};

	std::vector<MyMusic*> *GetList(){ return &m_list; }	// ���X�g�擾
	void Play();// �I���Ȃ��烉���_���ōĐ�
	void PlayHeaveHo();	// �q�[�z�[�h���C�u���y�Đ�
private:

	std::vector<MyMusic*> m_list;	// �I���Ȃ��i�[���郊�X�g
	tdnStreamSound *m_pStreamSound;	// �I���ȍĐ�����X�g���[���T�E���h
	tdnStreamSound *m_pHeaveHoStream;;	// �Ё[�ف[�Đ��p

	bool m_bPlay;	// 2�d�Đ��h�~�p
	bool m_bHeaveHoPlay;	// �q�[�z�[�Đ����Ă���ӂ�gu

};

//===============================================
//	����(�֐��Ăяo���͂������g��)
//===============================================
extern SE_Manager *se;
extern BGM_Manager *bgm;