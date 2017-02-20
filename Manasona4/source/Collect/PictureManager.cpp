#include "PictureManager.h"
#include "Data\PlayerData.h"
#include "Player\PlayerManager.h"
#include "UI\GameUI.h"
#include "BaseEntity\Message\MessageDispatcher.h"

//+--------------------------
//	�s�N�`���[�̊Ǘ��N���X
//+--------------------------

// ���̂̐錾
//PictureManager* PictureManager::m_pInstance = nullptr;

PictureManager::PictureManager() :BaseGameEntity(ENTITY_ID::PICTURE_MGR)	// �G���e�B�e�BID�o�^
{
	m_eSelectPicture = PICTURE_TYPE::AIROU;

	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		switch ((PICTURE_TYPE)i)
		{
		case PICTURE_TYPE::REST:
			m_pPicture[i] = new RestPicture();
			break;
		case PICTURE_TYPE::GATHERING:
			m_pPicture[i] = new GatheringPicture();
			break;
		case PICTURE_TYPE::STYLISH:
			m_pPicture[i] = new StylishPicture();
			break;
		case PICTURE_TYPE::COOL:
			m_pPicture[i] = new CoolPicture();
			break;
		case PICTURE_TYPE::ALLEY:
			m_pPicture[i] = new AlleyPicture();
			break;
		case PICTURE_TYPE::SAD:
			m_pPicture[i] = new SadPicture();
			break;
		case PICTURE_TYPE::SEXY:
			m_pPicture[i] = new SexyPicture();
			break;
		case PICTURE_TYPE::SEA:
			m_pPicture[i] = new SeaPicture();
			break;
		case PICTURE_TYPE::AIROU:
			m_pPicture[i] = new AirouPicture();
			break;
		case PICTURE_TYPE::TEKI:
			m_pPicture[i] = new TekiPicture();
			break;
		case PICTURE_TYPE::NAZENARABA:
			m_pPicture[i] = new NazenarabaPicture();
			break;
		case PICTURE_TYPE::ARAMITAMA:
			m_pPicture[i] = new AramitamaPicture();
			break;
		case PICTURE_TYPE::ANIKI:
			m_pPicture[i] = new AnikiPicture();
			break;
		case PICTURE_TYPE::MANATU:
			m_pPicture[i] = new Manatu1Picture();
			break;
		case PICTURE_TYPE::SUIKA:
			m_pPicture[i] = new SuikaPicture();
			break;
		case PICTURE_TYPE::MAHUYU:
			m_pPicture[i] = new MahuyuPicture();
			break;
		case PICTURE_TYPE::TOIRE:
			m_pPicture[i] = new ToirePicture();
			break;
		case PICTURE_TYPE::MANATU2:
			m_pPicture[i] = new Manatu2Picture();
			break;
		default:
			m_pPicture[i] = new AirouPicture();
			//MyAssert(0, "���̃^�C�v�͑��݂��Ȃ��B");	
			break;
		}

	}

	// ���b�N�A�C�R��
	m_pRockIcon = new tdn2DObj("Data/UI/Collect/Icon/Rock.png");

}

PictureManager::~PictureManager()
{
	// �g���t�B�[�S���
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pPicture[i]);
	}

	SAFE_DELETE(m_pRockIcon);

}

//+---------------------------------
// �N����
//+---------------------------------

// �I�������񏉊���
void PictureManager::InitExe(PICTURE_TYPE eSelectType)
{
	// 
	m_eSelectPicture = eSelectType;

	// �I�����Ă���C���X�g
	m_pPicture[(int)m_eSelectPicture]->InitExe();
}

//+---------------------------------
// �I��������̉摜�́@�����E�`��
//+---------------------------------
void PictureManager::UpdateExe()
{
	// �I�����Ă���C���X�g
	m_pPicture[(int)m_eSelectPicture]->UpdateExe();

}

void PictureManager::CtrlExe(int iDevice)
{
	// �I�����Ă���C���X�g
	m_pPicture[(int)m_eSelectPicture]->CtrlExe(iDevice);
}

void PictureManager::RenderExe()
{
	// �I�����Ă���C���X�g
	m_pPicture[(int)m_eSelectPicture]->RenderExe();

#ifdef _DEBUG

#endif // _DEBUG

}

void PictureManager::RenderExeInfo(int iPictureType, int iX, int iY)
{
	// �����̃C���X�g�̐���
	m_pPicture[(int)iPictureType]->RenderExeInfo(iX, iY);

}

// �I����������
void PictureManager::RereaseExe()
{
	// �I�����Ă���C���X�g
	m_pPicture[(int)m_eSelectPicture]->RereaseExe();
}

//+---------------------------------
// �A�C�R���̕`��
//+---------------------------------
void PictureManager::RenderIcon(int iPictureType, int iX, int iY)
{
	// ���b�N
	bool l_bRock = true;

	if (PlayerDataMgr->m_SecretData.iAllPic[iPictureType] == 1)
	{
		// �I�����Ă���C���X�g
		m_pPicture[(int)iPictureType]->RenderIcon(iX, iY);
	}
	else
	{
		// ��������ĂȂ��������Ȃ�
		if (m_pPicture[(int)iPictureType]->isRock())
		{
			// ���b�N�C���X�g
			m_pRockIcon->Render((int)m_pPicture[(int)iPictureType]->GetPictureIcon().vPos.x + iX,
				(int)m_pPicture[(int)iPictureType]->GetPictureIcon().vPos.y + iY);
		
		}else
		{
			// �I�����Ă���C���X�g�𖢍w���o�[�W�����ŕ`��
			m_pPicture[(int)iPictureType]->RenderIconNonPurchase(iX, iY);


		}


	}


}

//+---------------------------------
// �A�C�R���̐���
//+---------------------------------
void PictureManager::RenderIconInfo(int iPictureType, int iX, int iY)
{
	// ���b�N ����������Ă���Ȃ�
	if (PlayerDataMgr->m_SecretData.iAllPic[iPictureType] == 1)
	{
		// ���ʂɐ����`��
		m_pPicture[(int)iPictureType]->RenderIconInfo(iX, iY);

	}
	else
	{
		// ��������ĂȂ��������Ȃ�
		if (m_pPicture[(int)iPictureType]->isRock())
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
			m_pPicture[(int)iPictureType]->RenderIconInfo(iX, iY);
		}

	}


}

// �S�C���X�g���b�N
void PictureManager::AllReset()
{
	// 
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		PlayerDataMgr->m_SecretData.iAllPic[i] = 0;
	}

}

bool PictureManager::HandleMessage(const Message & msg)
{
	// ��񕜌�
	PICTURE_TYPE* l_eType = (PICTURE_TYPE*)msg.ExtraInfo;

	// ID���m�F���đ����Ă������b�Z�[�W���d��
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::PICTURE_GET:

		if (PlayerDataMgr->m_SecretData.iAllPic[(int)*l_eType] == 0)
		{
			PlayerDataMgr->m_SecretData.iAllPic[(int)*l_eType] = 1;
			//m_eSelectPicture = *l_eType;
			//m_pPicture[(int)m_eSelectPicture]->Action(20);
			return true;
		}

		break;
	default:

		break;
	}

	return false;
}


int PictureManager::GetPictureOwned()
{
	int l_iCount = 0;
	//
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		if (PlayerDataMgr->m_SecretData.iAllPic[i] == 1)
		{
			l_iCount++;
		}

	}

	return l_iCount;
}
