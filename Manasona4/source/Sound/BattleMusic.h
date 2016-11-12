#pragma once

#include "TDNLIB.h"

class BattleMusicManager
{
public:
	//------------------------------------------------------
	//	���̎擾
	//------------------------------------------------------
	static BattleMusicManager *GetInstance(){ static BattleMusicManager i; return &i; }

	//------------------------------------------------------
	//	�f�X�g���N�^(���X�g�̊J��)
	//------------------------------------------------------
	~BattleMusicManager(){ m_list.clear(); }

	//------------------------------------------------------
	//	AUTO��RANDOM�p
	//------------------------------------------------------
	enum{ AUTO = 0, RANDOM = 1 };

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	// �Ȑ���Ԃ�
	int GetNumMusic(){ return m_NumMusic; }

	// �����̋Ȗ��̔ԍ���Ԃ�
	int GetMusicNo(LPCSTR MusicName)
	{
		FOR((int)m_list.size()) if (tdnFile::GetFileName((LPSTR)m_list[i].c_str(), false) == MusicName) return i;

		MyAssert(0, "����ȃf�[�^�͑��݂��Ȃ�");
		return -1;
	}

	// �����̔ԍ��̋Ȗ���Ԃ�(��ɃZ���N�g��)
	std::string GetMusicName(int ID)
	{
		assert(ID >= 0 && ID < m_NumMusic);
		switch (ID)
		{
		case AUTO:
			return std::string("AUTO");
			break;

		case RANDOM:
			return std::string("RANDOM");
			break;

		default:
			return tdnFile::GetFileName((LPSTR)m_list[ID - 2].c_str(), false);
			break;
		}
	}

	// �����̔ԍ��̋Ȃ̃t�@�C���p�X��Ԃ�
	std::string GetMusicFilePath(int ID)
	{
		assert(ID >= 0 && ID < m_NumMusic);
		switch (ID)
		{
		case AUTO:
			return std::string("DATA/Sound/BGM/battle/tori.ogg");
			break;

		case RANDOM:
			return std::string("DATA/Sound/BGM/battle/" + m_list[tdnRandom::Get(0,m_NumMusic - 3)]);
			break;

		default:
			return std::string("DATA/Sound/BGM/battle/" + m_list[ID - 2]);
			break;
		}

	}
private:

	//------------------------------------------------------
	//	�Ȃ̖��O���t�@�C�����A���̔ԍ����i�[����Ă���
	//------------------------------------------------------
	std::vector<std::string> m_list;

	//------------------------------------------------------
	//	�Ȃ̑���
	//------------------------------------------------------
	int m_NumMusic;

	//------------------------------------------------------
	//	�R���X�g���N�^(�񋓂��Ċi�[)
	//------------------------------------------------------
	BattleMusicManager()
	{
		// �t�H���_�[�񋓗p
		DirectoryInfo info;
		tdnFile::EnumDirectory("DATA/Sound/BGM/battle", &info);

		// �Ȃ̑���
		m_NumMusic = (int)info.FileNames.size();

		// �t�H���_�[�̒���ogg�t�@�C����񋓂��Ċi�[���Ă���
		FOR(m_NumMusic)
		{
			// ogg�t�@�C�����Ƃ��̔ԍ����i�[
			m_list.push_back(info.FileNames[i]);
		}

		// ��AUTO�ƃ����_���p
		m_NumMusic += 2;
	}
	BattleMusicManager(const BattleMusicManager&){}
	BattleMusicManager &operator=(const BattleMusicManager&){}
};

#define BattleMusicMgr (BattleMusicManager::GetInstance())