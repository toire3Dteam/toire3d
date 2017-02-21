#include "MovieManager.h"
#include "Data\PlayerData.h"
#include "Player\PlayerManager.h"
#include "UI\GameUI.h"
#include "BaseEntity\Message\MessageDispatcher.h"

//+--------------------------
//	���[�r�[�̊Ǘ��N���X
//+--------------------------

// ���̂̐錾
//MovieManager* MovieManager::m_pInstance = nullptr;

MovieManager::MovieManager() :BaseGameEntity(ENTITY_ID::MOVIE_MGR)	// �G���e�B�e�BID�o�^
{
	m_eSelectMovie = MOVIE_TYPE::OP;

	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		switch ((MOVIE_TYPE)i)
		{
		case MOVIE_TYPE::OP:
			m_pMovie[i] = new OpMovie();
			break;
		case MOVIE_TYPE::OLD_OP:
			m_pMovie[i] = new OldOpMovie();
			break;
		case MOVIE_TYPE::STAFF_ROLL:
			m_pMovie[i] = new StaffRollMovie();
			break;
		default:
			m_pMovie[i] = new OpMovie();
			//MyAssert(0, "���̃^�C�v�͑��݂��Ȃ��B");	
			break;
		}

	}

	// ���b�N�A�C�R��
	m_pRockIcon = new tdn2DObj("Data/UI/Collect/Icon/Rock.png");

}

MovieManager::~MovieManager()
{
	// �g���t�B�[�S���
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pMovie[i]);
	}

	SAFE_DELETE(m_pRockIcon);

}

//+---------------------------------
// �N����
//+---------------------------------

// �I�������񏉊���
void MovieManager::InitExe(MOVIE_TYPE eSelectType)
{
	// 
	m_eSelectMovie = eSelectType;

	// �I�����Ă���C���X�g
	m_pMovie[(int)m_eSelectMovie]->InitExe();
}

//+---------------------------------
// �I��������̉摜�́@�����E�`��
//+---------------------------------
bool MovieManager::UpdateExe()
{
	// �I�����Ă���C���X�g
	return m_pMovie[(int)m_eSelectMovie]->UpdateExe();

}

void MovieManager::CtrlExe(int iDevice)
{
	// �I�����Ă���C���X�g
	m_pMovie[(int)m_eSelectMovie]->CtrlExe(iDevice);
}

void MovieManager::RenderExe()
{
	// �I�����Ă���C���X�g
	m_pMovie[(int)m_eSelectMovie]->RenderExe();

#ifdef _DEBUG

#endif // _DEBUG

}

void MovieManager::RenderExeInfo(int iMovieType, int iX, int iY)
{
	// �����̃C���X�g�̐���
	m_pMovie[(int)iMovieType]->RenderExeInfo(iX, iY);

}

// �I����������
void MovieManager::RereaseExe()
{
	// �I�����Ă���C���X�g
	m_pMovie[(int)m_eSelectMovie]->RereaseExe();
}

//+---------------------------------
// �A�C�R���̕`��
//+---------------------------------
void MovieManager::RenderIcon(int iMovieType, int iX, int iY)
{
	// ���b�N
	bool l_bRock = true;

	if (PlayerDataMgr->m_SecretData.iAllMovie[iMovieType] == 1)
	{
		// �I�����Ă���C���X�g
		m_pMovie[(int)iMovieType]->RenderIcon(iX, iY);
	}
	else
	{
		// ��������ĂȂ��������Ȃ�
		if (m_pMovie[(int)iMovieType]->isRock())
		{
			// ���b�N�C���X�g
			m_pRockIcon->Render((int)m_pMovie[(int)iMovieType]->GetMovieIcon().vPos.x + iX,
				(int)m_pMovie[(int)iMovieType]->GetMovieIcon().vPos.y + iY);
		
		}else
		{
			// �I�����Ă���C���X�g�𖢍w���o�[�W�����ŕ`��
			m_pMovie[(int)iMovieType]->RenderIconNonPurchase(iX, iY);


		}


	}


}

//+---------------------------------
// �A�C�R���̐���
//+---------------------------------
void MovieManager::RenderIconInfo(int iMovieType, int iX, int iY)
{
	// ���b�N ����������Ă���Ȃ�
	if (PlayerDataMgr->m_SecretData.iAllMovie[iMovieType] == 1)
	{
		// ���ʂɐ����`��
		m_pMovie[(int)iMovieType]->RenderIconInfo(iX, iY);

	}
	else
	{
		// ��������ĂȂ��������Ȃ�
		if (m_pMovie[(int)iMovieType]->isRock())
		{
			// �B���p�̃��b�Z�[�W
			// �^�C�g��
			tdnFont::RenderString("���J���R���e���c", "HG�޼��E",
				22, iX, iY, 0xffffffff, RS::COPY);
			// ����
			tdnFont::RenderString("�������Ă��Ȃ��R���e���c�ł��B", "HG�޼��E",
				19, iX, iY + 30, 0xffffffff, RS::COPY);
		}
		else
		{
			// ���ʂɐ����`��
			m_pMovie[(int)iMovieType]->RenderIconInfo(iX, iY);
		}

	}


}

// �S�C���X�g���b�N
void MovieManager::AllReset()
{
	// 
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		PlayerDataMgr->m_SecretData.iAllMovie[i] = 0;
	}

}

bool MovieManager::HandleMessage(const Message & msg)
{
	// ��񕜌�
	MOVIE_TYPE* l_eType = (MOVIE_TYPE*)msg.ExtraInfo;

	// ID���m�F���đ����Ă������b�Z�[�W���d��
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::MOVIE_GET:

		if (PlayerDataMgr->m_SecretData.iAllMovie[(int)*l_eType] == 0)
		{
			PlayerDataMgr->m_SecretData.iAllMovie[(int)*l_eType] = 1;
			//m_eSelectMovie = *l_eType;
			//m_pMovie[(int)m_eSelectMovie]->Action(20);
			return true;
		}

		break;
	default:

		break;
	}

	return false;
}


int MovieManager::GetMovieOwned()
{
	int l_iCount = 0;
	//
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		if (PlayerDataMgr->m_SecretData.iAllMovie[i] == 1)
		{
			l_iCount++;
		}

	}

	return l_iCount;
}
