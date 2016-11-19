#include "SelectUI.h"
#include "SelectUIState.h"


static int adjustY = +24;
static float g_scale = 0.9f;


SelectUI::SelectUI(SIDE side, ENTITY_ID id, int DeviceID) :BaseGameEntity(id)
{
	// �x���^�C�}�[
	m_iWaitFrame = 0;

	// ��UI���E�����T�C�h�Ȃ̂����󂯎��@���o���ς��̐�Ɏ擾
	m_eSide = side;

	// �����삷��R���g���[���[�̃f�o�C�X
	m_iDeviceID = DeviceID;

	// �L�����̑I��ԍ�������
	m_iSelectCharacterNo = 0;
	m_iSelectPartnerNo = 0;

	// �L�����N�^�[�̉摜
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_tagCharaPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/airou.png");
			m_tagCharaPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/airou.png");
			m_tagCharaInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/airou.png");
			m_tagCharaName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/name/airou.png");
			m_pCharaIconRip[i] = new tdn2DAnim("Data/UI/CharacterSelect/icon/airou.png");

			break;
		case CHARACTER::TEKI:
			m_tagCharaPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/teki.png");
			m_tagCharaPicRip[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Character/teki.png");
			m_tagCharaInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/teki.png");
			m_tagCharaName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/name/teki.png");
			m_pCharaIconRip[i] = new tdn2DAnim("Data/UI/CharacterSelect/icon/teki.png");

			break;
		default:
			MyAssert(0, "����ȃf�[�^�͂Ȃ�");
			break;
		}

		// ��
		m_tagCharaName[i].pPic->SetScale(g_scale);
		//m_tagCharaPic[i].pPic->SetScale(g_scale);
	}
	InitCharcterPos();

	// �p�[�g�i�[�̉摜
	for (int i = 0; i < (int)PARTNER::END; i++)
	{
		switch ((PARTNER)i)
		{
		case PARTNER::MOKOI:
			m_tagPartnerPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/Mokoi.png");
			m_tagPartnerInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/mokoi.png");
			m_tagPartnerName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Name/mokoi.png");
			m_tagPartneSirclePic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/MokoiSircle.png");

			break;
		case PARTNER::MAYA:
			m_tagPartnerPic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/Maya.png");
			m_tagPartnerInfo[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Info/maya.png");
			m_tagPartnerName[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Name/maya.png");
			m_tagPartneSirclePic[i].pPic = new tdn2DAnim("Data/UI/CharacterSelect/Partner/MayaSircle.png");

			break;
		default:
			MyAssert(0, "����ȃf�[�^�͂Ȃ�");
			break;
		}

		// ��
		m_tagPartnerName[i].pPic->SetScale(g_scale);
	}

	InitPartnerPos();
	
	// ���̑�
	m_tagInfoBox.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/Info/InfoBox.png");
	
	m_tagNamePlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/name/namePlate.png");
	m_tagAndPlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/name/andPlate.png");
	m_tagPartnerPlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/name/PartnerPlate.png");
	m_tagPartnerSirclePlate.pPic = new tdn2DAnim("DATA/UI/CharacterSelect/Partner/PartnerSircleFont.png");
	m_fPartnerPlateAngle = 0.0f;
	
	// ��
	m_tagNamePlate.pPic->SetScale(g_scale);
	m_tagAndPlate.pPic->SetScale(g_scale);
	m_tagPartnerPlate.pPic->SetScale(g_scale);
	


	if (m_eSide == SIDE::LEFT)
	{
		m_tagInfoBox.iX = 0;
		m_tagInfoBox.iY = 354 + adjustY;

		m_tagNamePlate.iX = 0;
		m_tagNamePlate.iY = 200 + adjustY;

		m_tagAndPlate.iX = 0;
		m_tagAndPlate.iY = 200 + adjustY;

		m_tagPartnerPlate.iX = 0;
		m_tagPartnerPlate.iY = 200 + adjustY;

		m_tagPartnerSirclePlate.iX = 315 ;
		m_tagPartnerSirclePlate.iY = 38 + adjustY;

		m_tagInfoBox.pPic->OrderMoveAppeared(14, m_tagInfoBox.iX - 384, m_tagInfoBox.iY);

		m_tagNamePlate.pPic->OrderMoveAppeared(14, m_tagNamePlate.iX -384, m_tagNamePlate.iY);
		m_tagAndPlate.pPic->OrderMoveAppeared(8, m_tagAndPlate.iX -384, m_tagAndPlate.iY);
		m_tagPartnerPlate.pPic->OrderMoveAppeared(8, m_tagPartnerPlate.iX -384, m_tagPartnerPlate.iY);
		m_tagPartnerSirclePlate.pPic->OrderMoveAppeared(12, m_tagPartnerSirclePlate.iX - 512, m_tagPartnerSirclePlate.iY);
	}
	else
	{
		m_tagInfoBox.iX = 899;
		m_tagInfoBox.iY = 354 + adjustY;

		m_tagNamePlate.iX = 890;
		m_tagNamePlate.iY = 200 + adjustY;

		m_tagAndPlate.iX = 890;
		m_tagAndPlate.iY = 200 + adjustY;

		m_tagPartnerPlate.iX = 890;
		m_tagPartnerPlate.iY = 200 + adjustY;

		m_tagPartnerSirclePlate.iX = 709 ;
		m_tagPartnerSirclePlate.iY = 38 + adjustY;

		m_tagInfoBox.pPic->OrderMoveAppeared(14, m_tagInfoBox.iX + 384, m_tagInfoBox.iY);

		m_tagNamePlate.pPic->OrderMoveAppeared(14, m_tagNamePlate.iX + 384, m_tagNamePlate.iY);
		m_tagAndPlate.pPic->OrderMoveAppeared(8, m_tagAndPlate.iX + 384, m_tagAndPlate.iY);
		m_tagPartnerPlate.pPic->OrderMoveAppeared(8, m_tagPartnerPlate.iX + 384, m_tagPartnerPlate.iY);
		m_tagPartnerSirclePlate.pPic->OrderMoveAppeared(12, m_tagPartnerSirclePlate.iX + 512, m_tagPartnerSirclePlate.iY);

	}

	// �I���J�[�\��
	if (m_eSide == SIDE::LEFT)
	{
		m_pSelect.pPic = new tdn2DAnim("Data/UI/CharacterSelect/Icon/select1p.png");
		
	}else 
	{
		m_pSelect.pPic = new tdn2DAnim("Data/UI/CharacterSelect/Icon/select2p.png");
	}
	m_pSelect.pPic->OrderShake(INT_MAX - 1 , 0, 8, 8);
	m_pSelect.pPic->Action();
	m_pSelect.iX = 0;
	m_pSelect.iY = 0;

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<SelectUI>(this);
	m_pStateMachine->SetCurrentState(SelectUIState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�


}

void SelectUI::InitCharcterPos()
{
	// �L�����N�^�[�̉摜
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{

		// ��[�p�[�g�i�[]���T�C�h��������
		if (m_eSide == SIDE::LEFT)
		{
			// �|�W�V�����ݒ�
			// �L�����G
			m_tagCharaPic[i].iX = 0;
			m_tagCharaPic[i].iY = 0;

			// �L�����G�g��
			m_tagCharaPicRip[i].iX = 0;
			m_tagCharaPicRip[i].iY = 0;

			// �L�����̐����G
			m_tagCharaInfo[i].iX = 0;
			m_tagCharaInfo[i].iY = 354 + adjustY;

			// �L�����̖��O
			m_tagCharaName[i].iX = 0;
			m_tagCharaName[i].iY = 200 + adjustY;

			// �A�j���[�V����
			// �����G
			m_tagCharaPic[i].pPic->OrderMoveAppeared(12, m_tagCharaPic[i].iX - 384, m_tagCharaPic[i].iY);
		}
		else // �E�T�C�h�������� 
		{

			// �|�W�V�����ݒ�
			// �L�����G
			m_tagCharaPic[i].iX = 899;
			m_tagCharaPic[i].iY = 0;

			// �L�����G�g��
			m_tagCharaPicRip[i].iX = 899;
			m_tagCharaPicRip[i].iY = 0;

			// �L�����̐����G
			m_tagCharaInfo[i].iX = 899;
			m_tagCharaInfo[i].iY = 354 + adjustY;

			// �L�����̖��O
			m_tagCharaName[i].iX = 890;
			m_tagCharaName[i].iY = 200 + adjustY;

			// �A�j���[�V����
			// �����G
			m_tagCharaPic[i].pPic->OrderMoveAppeared(12, m_tagCharaPic[i].iX + 384, m_tagCharaPic[i].iY);

		}

		// ���ʃA�j��
		// �g��
		m_tagCharaPicRip[i].pPic->OrderRipple(14, 1.0f, 0.01f);

		// ����
		m_tagCharaInfo[i].pPic->OrderMoveAppeared(12, m_tagCharaInfo[i].iX + 384 / 12, m_tagCharaInfo[i].iY);

		// ���O
		m_tagCharaName[i].pPic->OrderMoveAppeared(8, m_tagCharaName[i].iX - 100, m_tagCharaName[i].iY + 10);

		// �A�C�R���̔g��p
		m_pCharaIconRip[i]->OrderRipple(14, 1.0f, 0.02f);
	}
}

void SelectUI::InitPartnerPos()
{
	// �p�[�g�i�[�̉摜
	for (int i = 0; i < (int)PARTNER::END; i++)
	{

		// ��[�p�[�g�i�[]�A�j���[�V����
		// ���T�C�h��������
		if (m_eSide == SIDE::LEFT)
		{
			m_tagPartnerPic[i].iX = 127;
			m_tagPartnerPic[i].iY = 265 + adjustY;

			m_tagPartnerInfo[i].iX = 0;
			m_tagPartnerInfo[i].iY = 354 + adjustY;

			m_tagPartnerName[i].iX = 0;
			m_tagPartnerName[i].iY = 200 + adjustY;
	
			m_tagPartneSirclePic[i].iX = 315 ;
			m_tagPartneSirclePic[i].iY = 38 + adjustY;
			m_tagPartneSirclePic[i].pPic->OrderMoveAppeared(12, m_tagPartneSirclePic[i].iX - 384, m_tagPartneSirclePic[i].iY);

		}
		else // �E�T�C�h�������� 
		{
			m_tagPartnerPic[i].iX = 1027;
			m_tagPartnerPic[i].iY = 265 + adjustY;

			m_tagPartnerInfo[i].iX = 899;
			m_tagPartnerInfo[i].iY = 354 + adjustY;

			m_tagPartnerName[i].iX = 890;
			m_tagPartnerName[i].iY = 200 + adjustY;

			m_tagPartneSirclePic[i].iX = 709;
			m_tagPartneSirclePic[i].iY = 38 + adjustY;
			m_tagPartneSirclePic[i].pPic->OrderMoveAppeared(12, m_tagPartneSirclePic[i].iX + 384, m_tagPartneSirclePic[i].iY);

		}

		// ���ʃA�j���[�V����
		m_tagPartnerPic[i].pPic->OrderMoveAppeared(8, m_tagPartnerPic[i].iX - 32, m_tagPartnerPic[i].iY);
		
		// ����
		m_tagPartnerInfo[i].pPic->OrderMoveAppeared(12, m_tagPartnerInfo[i].iX + 384 / 12, m_tagPartnerInfo[i].iY);

		// ���O
		m_tagPartnerName[i].pPic->OrderMoveAppeared(8, m_tagPartnerName[i].iX - 100, m_tagPartnerName[i].iY + 10);

	
	}
}

SelectUI::~SelectUI()
{
	FOR((int)CHARACTER::END)
	{
		SAFE_DELETE(m_tagCharaPic[i].pPic);
		SAFE_DELETE(m_tagCharaPicRip[i].pPic);
		SAFE_DELETE(m_pCharaIconRip[i]);
		SAFE_DELETE(m_tagCharaInfo[i].pPic);
		SAFE_DELETE(m_tagCharaName[i].pPic);

	}

	FOR((int)PARTNER::END)
	{
		SAFE_DELETE(m_tagPartnerPic[i].pPic);
		SAFE_DELETE(m_tagPartnerInfo[i].pPic);
		SAFE_DELETE(m_tagPartnerName[i].pPic);
		SAFE_DELETE(m_tagPartneSirclePic[i].pPic);
	}

	SAFE_DELETE(m_tagInfoBox.pPic);
	SAFE_DELETE(m_tagNamePlate.pPic);
	SAFE_DELETE(m_tagAndPlate.pPic);
	SAFE_DELETE(m_tagPartnerPlate.pPic);
	SAFE_DELETE(m_tagPartnerSirclePlate.pPic);

	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_pSelect.pPic);
}

void SelectUI::Update()
{
	// �L�����N�^�[�n�̍X�V
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		m_tagCharaPic[i].pPic->Update();
		m_tagCharaPicRip[i].pPic->Update();
		m_tagCharaInfo[i].pPic->Update();
		m_tagCharaName[i].pPic->Update();
		m_pCharaIconRip[i]->Update();
	}

	// �p�[�g�i�[�n�̍X�V
	for (int i = 0; i < (int)PARTNER::END; i++)
	{	
		m_tagPartnerPic[i].pPic->Update();
		m_tagPartnerInfo[i].pPic->Update();
		m_tagPartnerName[i].pPic->Update();
		m_tagPartneSirclePic[i].pPic->Update();

	}

	// �C���t�H�{�b�N�X
	m_tagInfoBox.pPic->Update();

	// ���O�n
	m_tagNamePlate.pPic->Update();
	m_tagAndPlate.pPic->Update();
	m_tagPartnerPlate.pPic->Update();
	
	if (SIDE::LEFT == m_eSide)
	{
		m_fPartnerPlateAngle += 0.002f;
	}else
	{
		m_fPartnerPlateAngle -= 0.002f;
	}
	m_tagPartnerSirclePlate.pPic->SetAngle(m_fPartnerPlateAngle);
	m_tagPartnerSirclePlate.pPic->Update();
	
	// �Z���N�g�X�V
	m_pSelect.pPic->Update();

	// ���X�e�[�g�}�V���X�V
	m_pStateMachine->Update();
	return;

}

void SelectUI::Render()
{	
	// �����G
	if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()))
	{
		m_tagCharaPic[m_iSelectCharacterNo].pPic->SetARGB(0x77555555);
		m_tagCharaPic[m_iSelectCharacterNo].pPic->Render(m_tagCharaPic[m_iSelectCharacterNo].iX, m_tagCharaPic[m_iSelectCharacterNo].iY);
	}else
	{
		m_tagCharaPic[m_iSelectCharacterNo].pPic->Render(m_tagCharaPic[m_iSelectCharacterNo].iX, m_tagCharaPic[m_iSelectCharacterNo].iY);
	}

	// �����G�g��
	m_tagCharaPicRip[m_iSelectCharacterNo].pPic->Render(m_tagCharaPicRip[m_iSelectCharacterNo].iX, m_tagCharaPicRip[m_iSelectCharacterNo].iY, RS::ADD);

	// �����p�{�b�N�X
	m_tagInfoBox.pPic->Render(m_tagInfoBox.iX, m_tagInfoBox.iY);

	// ����
	m_tagCharaInfo[m_iSelectCharacterNo].pPic->Render(m_tagCharaInfo[m_iSelectCharacterNo].iX, m_tagCharaInfo[m_iSelectCharacterNo].iY);
	
	// �p�[�g�i�[����
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Render(m_tagPartnerInfo[m_iSelectPartnerNo].iX, m_tagPartnerInfo[m_iSelectPartnerNo].iY);

	// ���O�v���[�g
	if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()))
	{		
		m_tagNamePlate.pPic->SetARGB(0xaa666666);
		m_tagNamePlate.pPic->Render(m_tagNamePlate.iX, m_tagNamePlate.iY);
	}
	else
	{
		m_tagNamePlate.pPic->Render(m_tagNamePlate.iX, m_tagNamePlate.iY);
	}

	// ���O
	if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()))
	{
		m_tagCharaName[m_iSelectCharacterNo].pPic->SetARGB(0xaa666666);
		m_tagCharaName[m_iSelectCharacterNo].pPic->Render(m_tagCharaName[m_iSelectCharacterNo].iX, m_tagCharaName[m_iSelectCharacterNo].iY);
	}else
	{
		m_tagCharaName[m_iSelectCharacterNo].pPic->Render(m_tagCharaName[m_iSelectCharacterNo].iX, m_tagCharaName[m_iSelectCharacterNo].iY);

	}
	// �p�[�g�i�[�I���v���[�g1
	m_tagPartnerPlate.pPic->Render(m_tagPartnerPlate.iX, m_tagPartnerPlate.iY);

	// �p�[�g�i�[�I���������̊G
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Render(m_tagPartnerPic[m_iSelectPartnerNo].iX, m_tagPartnerPic[m_iSelectPartnerNo].iY);

	// �p�[�g�i�[�̍ŏI�v���[�g
	m_tagPartnerSirclePlate.pPic->Render(m_tagPartnerSirclePlate.iX, m_tagPartnerSirclePlate.iY);

	// �p�[�g�i�[�I���������̍Ō�̊G
	m_tagPartneSirclePic[m_iSelectPartnerNo].pPic->Render(m_tagPartneSirclePic[m_iSelectPartnerNo].iX, m_tagPartneSirclePic[m_iSelectPartnerNo].iY);

	// �p�[�g�i�[�̖��O�v���[�g
	m_tagAndPlate.pPic->Render(m_tagAndPlate.iX, m_tagAndPlate.iY);

	// �p�[�g�i�[�̖��O
	m_tagPartnerName[m_iSelectPartnerNo].pPic->Render(m_tagPartnerName[m_iSelectPartnerNo].iX, m_tagPartnerName[m_iSelectPartnerNo].iY);

	// �A�C�R��
	m_pCharaIconRip[m_iSelectCharacterNo]->Render(384 + (m_iSelectCharacterNo * 88), 482, RS::ADD);
	if (GetFSM()->isInState(*SelectUIState::FirstStep::GetInstance()))
	{
		m_pSelect.pPic->SetARGB(0xffffffff);
		if (m_eSide== SIDE::LEFT)
		{

			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88)-2, 482);
		}
		else
		{

			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88) + 42, 482);
		}

	}
	else if (GetFSM()->isInState(*SelectUIState::MiddleStep::GetInstance()))
	{
		m_pSelect.pPic->SetARGB(0xaa555555);
		if (m_eSide == SIDE::LEFT)
		{
			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88), 482);

		}else
		{
			m_pSelect.pPic->Render(384 + (m_iSelectCharacterNo * 88) + 42, 482);
		}
	}
	else if (GetFSM()->isInState(*SelectUIState::SecondStep::GetInstance()))
	{
		m_pSelect.pPic->SetARGB(0xffffffff);
		m_pSelect.pPic->Render(m_tagPartnerPlate.iX+88, m_tagPartnerPlate.iY + 76);
	}

	// �������ɃX�e�[�g�}�V���`��
	m_pStateMachine->Render();


}

// �L�����֘A
void SelectUI::ActionChara()
{
	m_tagCharaPic[m_iSelectCharacterNo].pPic->Action();
	m_tagCharaInfo[m_iSelectCharacterNo].pPic->Action();
	m_tagCharaName[m_iSelectCharacterNo].pPic->Action();
}
// �L�����֘A�v���[�g
void SelectUI::ActionCharaPlate()
{
	m_tagInfoBox.pPic->Action(6);
	m_tagNamePlate.pPic->Action(12);
}

void SelectUI::ActionFirstStep()
{
	// �F�̖��邳��߂�
	//m_tagCharaPic[m_iSelectCharacterNo].pPic->SetARGB(0xffffffff);


}

void SelectUI::StopFirstStep()
{
	//m_tagNamePlate.pPic->Stop();
	
	// �L�����N�^�[�n�̐F�̈Â�
	// �F�̖��邳��߂�
	//m_tagCharaPic[m_iSelectCharacterNo].pPic->SetARGB(0xff666666);

}
// �p�[�g�i�[�֘A
void SelectUI::ActionPartner() 
{
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Action();
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Action();
	m_tagPartnerName[m_iSelectPartnerNo].pPic->Action();
}
// �p�[�g�i�[�֘A�v���[�g
void SelectUI::ActionSecondStep()
{
	// �܂��̓L�����������~�߂�
	m_tagCharaInfo[m_iSelectCharacterNo].pPic->Stop();


	m_tagAndPlate.pPic->Action();
	m_tagPartnerPlate.pPic->Action();
}

void SelectUI::BackSecondStep()
{
	m_tagAndPlate.pPic->Stop();
	m_tagPartnerPlate.pPic->Stop();
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerName[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Stop();
}

void SelectUI::ActionOKStep()
{
	m_tagPartnerPlate.pPic->Stop();
	m_tagPartnerInfo[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerPic[m_iSelectPartnerNo].pPic->Stop();

	// �A�j���[�V����
	m_tagCharaPic[m_iSelectCharacterNo].pPic->Action();
	m_tagPartneSirclePic[m_iSelectPartnerNo].pPic->Action();
	m_tagPartnerSirclePlate.pPic->Action();


}

void SelectUI::BackOKStep()
{
	m_tagPartneSirclePic[m_iSelectPartnerNo].pPic->Stop();
	m_tagPartnerSirclePlate.pPic->Stop();
}

void SelectUI::MiddleAction()
{
	// �����G�ƃA�C�R���� �g�䉉�o
	m_tagCharaPicRip[m_iSelectCharacterNo].pPic->Action();
	m_pCharaIconRip[m_iSelectCharacterNo]->Action();
}