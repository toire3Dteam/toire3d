#pragma once

#include "../BaseEntity/Message/Message.h"

// �O���錾
class BasePlayer;


//*****************************************************************************
//
//		�R�}���h�̃Z�[�u�f�[�^
//
//*****************************************************************************
class CommandSaver
{
public:
	//------------------------------------------------------
	//	�R�}���h�v���E�I��
	//------------------------------------------------------
	void Start(BasePlayer *pPlayer);
	void End();
	void Write(LPCSTR lpcFilename);

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	void Initialize()
	{
		m_pRefPlayer = nullptr; 
		m_vCommandBuffer.clear();
	}

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	// �v���t���[�����擾
	unsigned int GetFrame(){ return m_vCommandBuffer.size(); }
	bool isAction() { return (m_pRefPlayer != nullptr); }
private:

	//------------------------------------------------------
	//	�ϐ�
	//------------------------------------------------------
	BasePlayer *m_pRefPlayer;				// �Q�Ƃ���v���C���[
	std::vector<WORD> m_vCommandBuffer;		// �R�}���h�ۑ��p
};


//*****************************************************************************
//
//		�R�}���h
//
//*****************************************************************************
class CommandManager
{
public:
	//------------------------------------------------------
	//	���̎擾
	//------------------------------------------------------
	static CommandManager *GetInstance(){ static CommandManager instance; return &instance; }

	//------------------------------------------------------
	//	�ۑ������R�}���h����
	//------------------------------------------------------
	void Action(LPCSTR lpcCommandFilePath, SIDE side = SIDE::LEFT)
	{
		m_tagDatas[(int)side].iCurrentFrame = 0;

		// �R�}���h�����[�h����
		if (!LoadCommand(lpcCommandFilePath, side))return;

		m_bAction = true;
		m_bEnd = false;
	}

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	void Initialize()
	{
		m_bAction = false;
		m_bEnd = false;

		FOR((int)SIDE::ARRAY_MAX)
		{
			m_tagDatas[i].Initialize();

			// �Z�[�u���񏉊���
			if (!m_pCommandSaver[i])
			{
				m_pCommandSaver[i] = new CommandSaver;
				m_pCommandSaver[i]->Initialize();
			}
		}
	}

	//------------------------------------------------------
	//	���
	//------------------------------------------------------
	void Release()
	{
		FOR((int)SIDE::ARRAY_MAX)
		{
			SAFE_DELETE(m_pCommandSaver[i]);
			m_tagDatas[i].Release();
		}
	}

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update(SIDE side)
	{
		// �������ĂȂ�������o�Ă������I�I
		if (!m_bAction || m_bEnd) return;

		// �R�}���h�Đ��J�[�\����i�߂�
		if (++m_tagDatas[(int)side].iCurrentFrame >= m_tagDatas[(int)side].iNumCommand)
		{
			m_bAction = false;
			m_bEnd = true;
		}
	}

	//------------------------------------------------------
	//	��~
	//------------------------------------------------------
	void Stop()
	{
		m_bAction = false;
		m_bEnd = false;
	}

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	// ���ݍĐ����Ă���J�[�\���̃R�}���h
	WORD GetCommand(SIDE side){ return (m_tagDatas[(int)side].iCurrentFrame < m_tagDatas[(int)side].iNumCommand && m_tagDatas[(int)side].paCommandBuffer) ? m_tagDatas[(int)side].paCommandBuffer[m_tagDatas[(int)side].iCurrentFrame] : 0; }
	// �R�}���h�Đ��̏I���t���O
	bool isAction(){ return m_bAction; }
	bool isEnd(){ return m_bEnd; }

	//------------------------------------------------------
	//	�R�}���h�̃Z�[�u�֘A
	//------------------------------------------------------
	void SaveUpdate(SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->Update(); }
	void SaveStart(BasePlayer *pPlayer, SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->Start(pPlayer); }
	void SaveEnd(SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->End(); }
	void SaveWrite(LPCSTR lpcFileName, SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->Write(lpcFileName); }
	int GetSaveFrame(SIDE side = SIDE::LEFT){ return m_pCommandSaver[(int)side]->GetFrame(); }
	bool SaveIsAction(SIDE side = SIDE::LEFT){ return m_pCommandSaver[(int)side]->isAction(); }

private:
	//------------------------------------------------------
	//	�R�}���h�̃��[�h
	//------------------------------------------------------
	bool LoadCommand(LPCSTR lpcCommandFilePath, SIDE side)
	{
		FILE *fp;
		if (fopen_s(&fp, lpcCommandFilePath, "rb") != 0)
		{
			MyAssert(0, "�R�}���h�̃Z�[�u�f�[�^�������Ă��܂���B\n%s", lpcCommandFilePath);	// �܂��~�܂邱�Ƃ͂Ȃ��Ǝv�����c
			return false;
		}

		// �t���[�����ǂݍ���
		int l_iNumCommand;
		fread_s((LPSTR)&l_iNumCommand, sizeof(int), sizeof(int), 1, fp);
		if (l_iNumCommand <= 0)
		{
			MyAssert(0, "�R�}���h������������");
			return false;
		}

		// �R�}���h�o�b�t�@�̊m��
		m_tagDatas[(int)side].Partition(l_iNumCommand);

		// �R�}���h�f�[�^�ǂݍ���
		for (int i = 0; i < l_iNumCommand; i++)
		{
			fread_s((LPSTR)&m_tagDatas[(int)side].paCommandBuffer[i], sizeof(WORD), sizeof(WORD), 1, fp);
		}

		// �t�@�C������
		fclose(fp);

		return true;
	}

	//------------------------------------------------------
	//	�ϐ�
	//------------------------------------------------------
	struct Data
	{
		int iNumCommand;			// �R�}���h��(�ۑ����Ă���t���[��)
		int iCurrentFrame;		// �Đ��ʒu
		WORD *paCommandBuffer;	// �R�}���h���i�[����ϒ��|�C���^(�l�͕����肫���Ă���̂Ń|�C���^�ɂ���)
		Data() :iNumCommand(0), iCurrentFrame(0), paCommandBuffer(nullptr){}
		~Data(){ SAFE_DELETE(paCommandBuffer); }
		void Initialize()
		{
			iNumCommand = 0;
			iCurrentFrame = 0;
			SAFE_DELETE(paCommandBuffer); 
		}
		void Release(){ SAFE_DELETE(paCommandBuffer); }
		void Partition(int num)
		{
			if (paCommandBuffer) delete paCommandBuffer;
			paCommandBuffer = new WORD[num];
			iNumCommand = num;
		}
	}m_tagDatas[(int)SIDE::ARRAY_MAX];
	bool m_bAction;
	bool m_bEnd;

	CommandSaver *m_pCommandSaver[(int)SIDE::ARRAY_MAX];	// �R�}���h���Z�[�u�����̎���

	//------------------------------------------------------
	//	�V���O���g���̍�@
	//------------------------------------------------------
	CommandManager()
	{
		m_pCommandSaver[0] = nullptr;
		m_pCommandSaver[1] = nullptr;
		Initialize(); 
	}
	CommandManager(const CommandManager&){}
	CommandManager &operator =(const CommandManager&){}
};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define CommandMgr (CommandManager::GetInstance())