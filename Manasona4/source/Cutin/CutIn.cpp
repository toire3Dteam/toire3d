#include "CutIn.h"
#include "BaseEntity\Message\MessageDispatcher.h"

//+-----------------------
//	�J�b�g�C�����o�^�C�v
//+-----------------------

CutInType::Airou::Airou() 
{
	m_pPic.PicClean();
	m_pPic.pic = new tdn2DAnim("Data/UI/Game/CutIn/Airou.png");
	m_pPic.pic->OrderNone();
	//m_pPic.pic->OrderJump(24, 1, 0.1f);

	m_pPicShink.PicClean();
	m_pPicShink.pic = new tdn2DAnim("Data/UI/Game/CutIn/Airou.png");
	m_pPicShink.pic->OrderShrink(8, 1, 2);

	m_pPicBackRip.PicClean();
	m_pPicBackRip.pic = new tdn2DAnim("Data/UI/Game/CutIn/Airou.png");
	m_pPicBackRip.pic->OrderRipple(14, 1, 0.035f);

	m_pPicBackRip2.PicClean();
	m_pPicBackRip2.pic = new tdn2DAnim("Data/UI/Game/CutIn/Airou.png");
	m_pPicBackRip2.pic->OrderRipple(14, 1, 0.035f);
	
	m_pBackCol= new tdn2DObj("Data/UI/Game/CutIn/AirouBack.png");
}

CutInType::Airou::~Airou()
{
	SAFE_DELETE(m_pPic.pic);
	SAFE_DELETE(m_pPicShink.pic);
	SAFE_DELETE(m_pPicBackRip.pic);
	SAFE_DELETE(m_pPicBackRip2.pic);	
	SAFE_DELETE(m_pBackCol);
}


void CutInType::Airou::Update()
{
	m_pPic.pic->Update();
	m_pPicShink.pic->Update();
	m_pPicBackRip.pic->Update();
	m_pPicBackRip2.pic->Update();
}

void CutInType::Airou::Render()
{
	m_pBackCol->Render(0, 0);
	 
	m_pPicBackRip2.pic->Render((int)m_pPicBackRip2.pos.x, (int)m_pPicBackRip2.pos.y,RS::ADD);
	m_pPicBackRip.pic->Render((int)m_pPicBackRip.pos.x, (int)m_pPicBackRip.pos.y, RS::ADD);
	//m_pPic.pic->Render((int)m_pPic.pos.x, (int)m_pPic.pos.y);
	m_pPicShink.pic->Render((int)m_pPicShink.pos.x, (int)m_pPicShink.pos.y);
}

void CutInType::Airou::Action()
{
	Base::Action();
	m_pPic.pic->Action();
	m_pPicShink.pic->Action();

	m_pPicBackRip.pic->Action(12);
	m_pPicBackRip2.pic->Action(20);
	
}


//+--------------------
//	�J�b�g�C��
//+--------------------
 
CutIn  *CutIn::m_pInstance = nullptr;

CutIn::CutIn() :BaseGameEntity(ENTITY_ID::CUTIN_MGR)
{
	m_pScreen = new tdn2DObj(1280, 720, TDN2D::RENDERTARGET);
	m_fRate = 0.0f;
	m_fScale = 1.0f;
	m_iFrame = 0;
	m_bActionFlag = false;
	m_bRenderFlag = false;

	// ���ꂼ��̉��o��ǂݍ���
	for (int i = 0; i < (int)CUTIN_TYPE_NAME::ARRAY_END; i++)
	{
		switch ((CUTIN_TYPE_NAME)i)
		{
		case CUTIN_TYPE_NAME::AIROU:
			m_pCutinType[i] = new CutInType::Airou();
			break;
		default:
			assert(0);// "����ȃG�t�F�N�g�͂Ȃ�"
			break;
		}

	}

	m_eSelectType = CUTIN_TYPE_NAME::AIROU;

	// �����Őݒ��ς����
	m_iAlphaNearFrame = 4;
	m_iAlphaFarFrame = 38;
	m_iEndFrame = 45;

	m_eSaveID = ENTITY_ID::ID_ERROR;

}

CutIn::~CutIn()
{
	SAFE_DELETE(m_pScreen);

	for (int i = 0; i < (int)CUTIN_TYPE_NAME::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pCutinType[i]);
	}
}

void CutIn::Update()
{
	if (m_bActionFlag == false)return;
	m_bRenderFlag = true;

	// �t���[���X�V	
	if (++m_iFrame >= m_iEndFrame) { m_bActionFlag = false; }// �Ō�܂ōs���ΏI�� 

	// �����x�̍X�V
	if (m_iFrame >= m_iAlphaNearFrame)
	{
		m_fRate = (float)(m_iEndFrame - m_iFrame) / (float)(m_iEndFrame - m_iAlphaFarFrame);
		
		// ���X�P�[���g�� �����ɂȂ�Ȃ���g�債�ď����鉉�o
		if (m_fRate < 1.0f)
		{
			m_fScale += 0.1f;
		}		
	}
	else
	{
		m_fRate = (float)m_iFrame / (float)m_iAlphaNearFrame;  
	}
	m_fRate = Math::Clamp(m_fRate, 0.0f, 1.0f);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�
	m_pScreen->SetAlpha((int)(255 * m_fRate));

	// �L�������̉��o
	m_pCutinType[(int)m_eSelectType]->Update();

	// �����ɂȂ�u�Ԃɑ����Ƀ��b�Z�[�W�𑗂�
	if (m_iFrame ==  m_iAlphaFarFrame)
	{
		MsgMgr->Dispatch(0, ENTITY_ID::CUTIN_MGR, m_eSaveID,MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START, nullptr);
	}

}

void CutIn::Render()
{	
	if (m_bRenderFlag == false)return;
	
	Surface* saveSurface;
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	
	// �J�b�g�C���p�ɐ؂�ւ�
	m_pScreen->RenderTarget(0);
	{
		// ��ʃN���A
		tdnView::Clear();

		// �L�������̉��o
		m_pCutinType[(int)m_eSelectType]->Render();
	}
	
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//�����_�[�^�[�Q�b�g�̕���

	m_pScreen->SetScale(m_fScale);
	m_pScreen->Render(0, 0);
}

void CutIn::Action(CUTIN_TYPE_NAME type)
{
	m_fRate = 0.0f;
	m_fScale = 1.0f;	// �傫�������ɖ߂�
	m_iFrame = 0;

	m_bActionFlag = true;
	m_bRenderFlag = false;

	m_eSelectType = type;
	m_pCutinType[(int)m_eSelectType]->Action();

}

bool CutIn::HandleMessage(const Message & msg)
{
	switch (msg.Msg)
	{
	case CUTIN_ACTION:
	{
		CUTIN_TYPE_NAME *data = (CUTIN_TYPE_NAME*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		CUTIN_TYPE_NAME cutinType = *data;
	
		// �������ۑ�
		m_eSaveID = msg.Sender;

		Action(cutinType);
		return true;
	}
		break;
	default:
		break;
	}

	return false;
}
