#pragma once

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
	void Action(LPCSTR lpcCommandFilePath)
	{
		m_iCurrentFrame = 0;

		// �R�}���h�����[�h����
		if (!LoadCommand(lpcCommandFilePath))return;

		m_bAction = true;
		m_bEnd = false;
	}

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	void Initialize()
	{
		m_iCurrentFrame = 0;
		m_iNumCommand = 0;
		m_bAction = false;
		m_bEnd = false;
		SAFE_DELETE(m_paCommandBuffer);

		// �Z�[�u���񏉊���
		if (!m_pCommandSaver) m_pCommandSaver = new CommandSaver;
		m_pCommandSaver->Initialize();
	}

	//------------------------------------------------------
	//	���
	//------------------------------------------------------
	void Release()
	{
		SAFE_DELETE(m_pCommandSaver);
		SAFE_DELETE(m_paCommandBuffer);
	}

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	void Update()
	{
		// �������ĂȂ�������o�Ă������I�I
		if (!m_bAction || m_bEnd) return;

		// �R�}���h�Đ��J�[�\����i�߂�
		if (++m_iCurrentFrame >= m_iNumCommand)
		{
			m_bAction = false;
			m_bEnd = true;
		}
	}

	//------------------------------------------------------
	//	�Q�b�^�[
	//------------------------------------------------------
	// ���ݍĐ����Ă���J�[�\���̃R�}���h
	WORD GetCommand(){ return (m_iCurrentFrame < m_iNumCommand && m_paCommandBuffer) ? m_paCommandBuffer[m_iCurrentFrame] : 0; }
	// �R�}���h�Đ��̏I���t���O
	bool isAction(){ return m_bAction; }
	bool isEnd(){ return m_bEnd; }

	//------------------------------------------------------
	//	�R�}���h�̃Z�[�u�֘A
	//------------------------------------------------------
	void SaveUpdate(){ m_pCommandSaver->Update(); }
	void SaveStart(BasePlayer *pPlayer){ m_pCommandSaver->Start(pPlayer); }
	void SaveEnd(){ m_pCommandSaver->End(); }
	int GetSaveFrame(){ return m_pCommandSaver->GetFrame(); }

private:
	//------------------------------------------------------
	//	�R�}���h�̃��[�h
	//------------------------------------------------------
	bool LoadCommand(LPCSTR lpcCommandFilePath)
	{
		FILE *fp;
		if (fopen_s(&fp, lpcCommandFilePath, "rb") != 0)
		{
			MyAssert(0, "�R�}���h�̃Z�[�u�f�[�^�������Ă��܂���B\n%s", lpcCommandFilePath);	// �܂��~�܂邱�Ƃ͂Ȃ��Ǝv�����c
			return false;
		}

		// �t���[�����ǂݍ���
		fread_s((LPSTR)&m_iNumCommand, sizeof(int), sizeof(int), 1, fp);
		if (m_iNumCommand <= 0)
		{
			MyAssert(0, "�R�}���h������������");
			return false;
		}

		// �R�}���h�o�b�t�@�̊m��
		if (m_paCommandBuffer) delete m_paCommandBuffer;
		m_paCommandBuffer = new WORD[m_iNumCommand];

		// �R�}���h�f�[�^�ǂݍ���
		for (int i = 0; i < m_iNumCommand; i++)
		{
			fread_s((LPSTR)&m_paCommandBuffer[i], sizeof(WORD), sizeof(WORD), 1, fp);
		}

		// �t�@�C������
		fclose(fp);

		return true;
	}

	//------------------------------------------------------
	//	�ϐ�
	//------------------------------------------------------
	int m_iNumCommand;			// �R�}���h��(�ۑ����Ă���t���[��)
	int m_iCurrentFrame;		// �Đ��ʒu
	WORD *m_paCommandBuffer;	// �R�}���h���i�[����ϒ��|�C���^(�l�͕����肫���Ă���̂Ń|�C���^�ɂ���)
	bool m_bAction;
	bool m_bEnd;

	CommandSaver *m_pCommandSaver;	// �R�}���h���Z�[�u�����̎���

	//------------------------------------------------------
	//	�V���O���g���̍�@
	//------------------------------------------------------
	CommandManager() :m_pCommandSaver(nullptr), m_paCommandBuffer(nullptr){ Initialize(); }
	CommandManager(const CommandManager&){}
	CommandManager &operator =(const CommandManager&){}
};

/********************************************/
//	�C���X�^���X�ȗ���
/********************************************/
#define CommandMgr (CommandManager::GetInstance())