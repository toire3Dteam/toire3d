#include "Effect2DManager.h"

/******************
*	Effect2DManager
*******************/

Effect2DManager* Effect2DManager::m_pInstance = nullptr;

/*******************************/
// �C���X�^���X
/*******************************/
Effect2DManager& Effect2DManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Effect2DManager();
	}
	return *m_pInstance;
}

void Effect2DManager::Release()
{
	SAFE_DELETE(m_pInstance);
}

/*********************************/
//			�������E���
/*********************************/
Effect2DManager::Effect2DManager()
{
	m_iEffectFrame = 0;
	m_iMaxEffectFrame = 30;
	m_iSelectNodeNo = 0;

	// �X�N���[���̃T�C�Y
	m_iScreenWidth = 256;
	m_iScreenHeight = 256;
	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};

	AddNode();
}

Effect2DManager::~Effect2DManager()
{
	for (int i = 0; i < (int)m_pEffectNode.size(); i++)
	{
		SAFE_DELETE(m_pEffectNode[i]);
	}
	
	//�@�f�[�^�����
	m_pEffectNode.clear();

	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);
}

/*********************************/
//			�X�V�E�`��
/*********************************/
void Effect2DManager::Update()
{
	// �t���[���X�V
	CalcFrame();

	CalcSelectNo();
	
	// �m�[�h�̐����[�v
	for (int i = 0; i < (int)m_pEffectNode.size(); i++)
	{
		m_pEffectNode[i]->Update(m_iEffectFrame);
	}
}

void Effect2DManager::Render()
{	
	//+----------------------------------------------------------
	// �܂��͍��̃T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V����ۑ�
	//+----------------------------------------------------------
	Surface* saveSurface;
	D3DVIEWPORT9 saveViewPort;
	Surface* saveStencilSurface;
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	tdnSystem::GetDevice()->GetViewport(&saveViewPort);
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveStencilSurface);


	//+-----------------------------------------------------
	// �T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V����؂�ւ�
	//+-----------------------------------------------------
	D3DVIEWPORT9 vp = { 0, 0, m_iScreenWidth, m_iScreenHeight, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	m_pScreen->RenderTarget();
	{
		//�@��ʃN���A
		tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		// �m�[�h�̐����[�v
		for (int i = 0; i < (int)m_pEffectNode.size(); i++)
		{
			// ��ʂ̐^�񒆂����_�Ƃ��ĕ`��
			m_pEffectNode[i]->Render((m_iScreenWidth / 2), (m_iScreenHeight/ 2));

		}

	}
	

		//+-----------------------------------------------------
		// �T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V���𕜌�
		//+-----------------------------------------------------
		tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//�����_�[�^�[�Q�b�g�̕���		
		tdnSystem::GetDevice()->SetViewport(&saveViewPort);//�r���[�|�[�g�̕���
		tdnSystem::GetDevice()->SetDepthStencilSurface(saveStencilSurface);	//�X�e���V���o�b�t�@�̕���														


		// �G�t�F�N�g�X�N���[���`��
		m_pScreen->Render(0, 0);

		//tdnText::Draw(400, 20, 0xffffffff, "�Q�[���t���[��_%d", m_iEffectFrame);
		//tdnText::Draw(400, 50, 0xffffffff, "�Z���N�gNo_%d", m_iSelectNodeNo);
		//for (int i = 0; i < (int)m_pEffectNode.size(); i++)
		//{
		//	std::string text;
		//	text = "���O�F";
		//	text += m_pEffectNode[i]->m_sName;

		//	if (m_iSelectNodeNo == i)
		//		tdnText::Draw(800, 20 + i * 32, 0xffff00ff, text.c_str());
		//	else
		//		tdnText::Draw(800, 20 + i * 32, 0xffffffff, text.c_str());
		//}

}

void Effect2DManager::CalcFrame()
{
	// �t���[���X�V
	if (++m_iEffectFrame >= m_iMaxEffectFrame)m_iEffectFrame = 0;	

}

void Effect2DManager::CalcSelectNo()
{
	// �����]
	if (m_iSelectNodeNo>= (int)m_pEffectNode.size())
	{
		m_iSelectNodeNo = 0;
	}
	if (m_iSelectNodeNo <= -1)
	{
		m_iSelectNodeNo = (int)m_pEffectNode.size() - 1;
	}


}

// �G�t�F�N�g�ǉ�
void Effect2DManager::AddNode()
{
	Effect2D* data;
	data = new  Effect2D();

	// �z��ɉ�����
	m_pEffectNode.push_back(data);

}

//	�m�[�h�폜
void Effect2DManager::EraseNode(int no)
{
	// �G���[����
	if (no >= (int)m_pEffectNode.size())
	{
		//  DATA���Ȃ��̂ŏ����Ȃ�
		MyAssert(0,"���˂���");
		return;
	}

	// �܂���new�����f�[�^������
	SAFE_DELETE(m_pEffectNode[no]);

	// ���X�g����폜
	m_pEffectNode.erase(m_pEffectNode.begin() + no);

}
void Effect2DManager::EraseNode()
{
	// �܂���new�����f�[�^������
	SAFE_DELETE(m_pEffectNode[m_iSelectNodeNo]);

	// ���X�g����폜
	m_pEffectNode.erase(m_pEffectNode.begin() + m_iSelectNodeNo);

}

bool Effect2DManager::Save(const char* fileName)
{
	m_saveData.open(fileName, std::ios::binary);
	if (m_saveData.fail())
	{
		return false;
	}

	if (!m_saveData.is_open())
	{
		return false;
	}

	// ��ʃT�C�Y���擾
	m_saveData.write((char*)&m_iScreenWidth, sizeof(int)); 
	m_saveData.write((char*)&m_iScreenHeight, sizeof(int)); 

	int MAX_SIZE= (int)m_pEffectNode.size();// �ő�
	m_saveData.write((char*)&MAX_SIZE, sizeof(int)); // 

	// �G�t�F�N�g�m�[�h�f�[�^
	for (int i = 0; i < MAX_SIZE; i++)
	{

		// �K�v�ȃm�[�h�̃f�[�^
		m_saveData.write((char*)&m_pEffectNode[i]->m_iFrame, sizeof(int));
		m_saveData.write((char*)&m_pEffectNode[i]->m_vPos, sizeof(Vector2));
		m_saveData.write((char*)&m_pEffectNode[i]->m_fScale, sizeof(float));
		m_saveData.write((char*)&m_pEffectNode[i]->m_fAngle, sizeof(float));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sCol, sizeof(COLOR_INT));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sStart, sizeof(EffecrParam));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sMiddle, sizeof(EffecrParam));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sEnd, sizeof(EffecrParam));
		m_saveData.write((char*)&m_pEffectNode[i]->m_bMiddleFlag, sizeof(bool));
		m_saveData.write((char*)&m_pEffectNode[i]->m_bRenderFlag, sizeof(bool));

		m_saveData.write(m_pEffectNode[i]->m_sName, sizeof(m_pEffectNode[i]->m_sName)); // ���O
																							
		m_saveData.write(m_pEffectNode[i]->m_sListName, sizeof(m_pEffectNode[i]->m_sListName)); // ���X�g�l�[��

	}


	m_saveData.close();

	return true;
}

bool Effect2DManager::Load(const char* fileName)
{
	// ��x�S�Ẵm�[�h���N���A
	for (int i = 0; i < (int)m_pEffectNode.size(); i++)
	{
		SAFE_DELETE(m_pEffectNode[i]);
	}

	m_iEffectFrame = 0;

	//�@�f�[�^�����
	m_pEffectNode.clear();


	// �A�[�J�C�u�t�@�C����ǂݍ���
	std::ifstream LoadData; // input file stream
	LoadData.open(fileName, std::ios::binary);
	if (LoadData.fail())
	{
		return false;
	}


	// ��ʃT�C�Y���擾
	int sizeX = 0, sizeY = 0;
	LoadData.read((char*)&sizeX,	sizeof(int));
	LoadData.read((char*)&sizeY,	sizeof(int));
	ChangeScreenSize(sizeX, sizeY);

	int MAX_SIZE = 0;// �ő吔
	LoadData.read((char*)&MAX_SIZE, sizeof(int));// �ő吔�擾

	for (int i = 0; i < MAX_SIZE; i++)
	{
		Effect2D* data;
		data = new  Effect2D();

		// �K�v�ȃm�[�h�̃f�[�^
		LoadData.read((char*)&data->m_iFrame,	sizeof(int));
		LoadData.read((char*)&data->m_vPos,		sizeof(Vector2));
		LoadData.read((char*)&data->m_fScale,	sizeof(float));
		LoadData.read((char*)&data->m_fAngle,	sizeof(float));
		LoadData.read((char*)&data->m_sCol,		sizeof(COLOR_INT));
		LoadData.read((char*)&data->m_sStart,	sizeof(EffecrParam));
		LoadData.read((char*)&data->m_sMiddle,	sizeof(EffecrParam));
		LoadData.read((char*)&data->m_sEnd,		sizeof(EffecrParam));
		LoadData.read((char*)&data->m_bMiddleFlag, sizeof(bool));
		LoadData.read((char*)&data->m_bRenderFlag, sizeof(bool));

		LoadData.read(data->m_sName, sizeof(data->m_sName)); // ���O

		LoadData.read(data->m_sListName, sizeof(data->m_sListName)); // ���X�g���O

		if (strcmp(data->m_sName,"") != 0)
		{
			data->LoadTexture(data->m_sName);
		}
		

		// �z��ɉ�����
		m_pEffectNode.push_back(data);

	}


	return true;
}

void Effect2DManager::ChangeScreenSize(const int SizeX, const int SizeY)
{	
	// ��x����
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);

	m_iScreenWidth = SizeX;
	m_iScreenHeight = SizeY;

	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};

}

void Effect2DManager::ChangeScreenSize256()
{
	// ��x����
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);

	m_iScreenWidth = 256;
	m_iScreenHeight = 256;

	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};

}
void Effect2DManager::ChangeScreenSize512()
{
	// ��x����
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);

	m_iScreenWidth = 512;
	m_iScreenHeight = 512;

	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};
}


bool  Effect2DManager::OutputPng( char* fileName, const int interval)
{
	int roopNum = m_iMaxEffectFrame / interval;

	//+-----------------------------------------------------
	// �܂��͍��̃T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V����ۑ�
	//+-----------------------------------------------------
	Surface* saveSurface;
	D3DVIEWPORT9 saveViewPort;
	Surface* saveStencilSurface;
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	tdnSystem::GetDevice()->GetViewport(&saveViewPort);
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveStencilSurface);


	//+-----------------------------------------------------
	// �T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V����؂�ւ�
	//+-----------------------------------------------------
	D3DVIEWPORT9 vp = { 0, 0, m_iScreenWidth, m_iScreenHeight, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	m_pScreen->RenderTarget();
	{
	
		for (int i = 0; i < roopNum; i++)
		{
			//�@��ʃN���A
			tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

			std::string text;
			text = fileName;
			char*	ext = &fileName[lstrlen(fileName) - 4];
			if (lstrcmpi(ext, ".png") == 0)
			{
				// .png������
				text.pop_back();
				text.pop_back();
				text.pop_back();
				text.pop_back();
			}

			text += std::to_string(i);
			text += ".png";		

			int len = text.length();
			char *calcFileName;
			calcFileName = new char[len + 1];
			memcpy(calcFileName, text.c_str(), len + 1);


			// ���R�}�ڂ�`�悷�邩
			int frameNum = i*interval;

			// �m�[�h�̐����[�v
			for (int i = 0; i < (int)m_pEffectNode.size(); i++)
			{
				m_pEffectNode[i]->Update(frameNum);
				m_pEffectNode[i]->Render((m_iScreenWidth / 2), (m_iScreenHeight / 2));
			}

			D3DXSaveTextureToFile(calcFileName, D3DXIFF_PNG, m_pScreen->GetTexture(), NULL);

			delete calcFileName;
		}

	}


	//+----------------------------------------------------------
	// �T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V���𕜌�
	//+----------------------------------------------------------
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//�����_�[�^�[�Q�b�g�̕���		
	tdnSystem::GetDevice()->SetViewport(&saveViewPort);//�r���[�|�[�g�̕���
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveStencilSurface);	//�X�e���V���o�b�t�@�̕���														



	return true;
}



bool  Effect2DManager::OutputPngLine( char* fileName, const int interval, int WidthFrameNum)
{

	int roopNum = (int)ceil((float)m_iMaxEffectFrame / (float)interval);// �����_�؂�グ

	// ���̒���
	int calcWidth = WidthFrameNum;
	if (WidthFrameNum >= roopNum)
	{
		calcWidth = roopNum;
	}
	// �c�̒���
	int calcHeight = (int)ceil((float)roopNum / (float)calcWidth);
	
	// �e�N�X�`���̃T�C�Y���擾
	int lineScreenSizeX, lineScreenSizeY;	
	lineScreenSizeX = m_iScreenWidth*calcWidth;
	lineScreenSizeY = m_iScreenHeight*calcHeight;
 
	// �e�N�X�`���쐬
	m_pLineScreen = new tdn2DObj(lineScreenSizeX, lineScreenSizeY, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(lineScreenSizeX, lineScreenSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pLineStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};


	//+----------------------------------------------------------
	// �܂��͍��̃T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V����ۑ�
	//+----------------------------------------------------------
	Surface* saveSurface;
	D3DVIEWPORT9 saveViewPort;
	Surface* saveStencilSurface;
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	tdnSystem::GetDevice()->GetViewport(&saveViewPort);
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveStencilSurface);


	//+----------------------------------------------------------
	// �T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V����؂�ւ�
	//+----------------------------------------------------------
	D3DVIEWPORT9 vp = { 0, 0, lineScreenSizeX, lineScreenSizeY, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pLineStencilSurface);
	m_pLineScreen->RenderTarget();
	//�@��ʃN���A ����͈��̂�
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	
	{

		for (int i = 0; i < roopNum; i++)
		{
			// ���R�}�ڂ�`�悷�邩
			int frameNum = i*interval;

			int x = i % calcWidth;//  
			int y = i / calcWidth;//  

			// �m�[�h�̐����[�v
			for (int i = 0; i < (int)m_pEffectNode.size(); i++)
			{
				m_pEffectNode[i]->Update(frameNum);
				m_pEffectNode[i]->Render((m_iScreenWidth / 2) +(x * m_iScreenWidth),
					(m_iScreenHeight / 2) + (y * m_iScreenHeight));
			}
			
		}

	}


	std::string text;
	text = fileName;
	
	char*	ext = &fileName[lstrlen(fileName) - 4];
	if (lstrcmpi(ext, ".png") != 0) { text += ".png"; }
	
	int textLen = text.length();
	char *calcFileName;
	calcFileName = new char[textLen + 1];
	memcpy(calcFileName, text.c_str(), textLen + 1);
	
	// Png�摜
	D3DXSaveTextureToFile(calcFileName, D3DXIFF_PNG, m_pLineScreen->GetTexture(), NULL);

	delete calcFileName;

	//+----------------------------------------------------------
	// �T�[�t�F�C�X�E�r���[�E�f�v�X�X�e���V���𕜌�
	//+----------------------------------------------------------
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//�����_�[�^�[�Q�b�g�̕���		
	tdnSystem::GetDevice()->SetViewport(&saveViewPort);//�r���[�|�[�g�̕���
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveStencilSurface);	//�X�e���V���o�b�t�@�̕���														


	// ����
	SAFE_DELETE(m_pLineScreen);
	SAFE_RELEASE(m_pLineStencilSurface);

	return true;
}