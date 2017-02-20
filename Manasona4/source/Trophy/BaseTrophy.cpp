#include "BaseTrophy.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "../Sound/SoundManager.h"

//+------------------------------
//	��{�̃g���t�B�[
//+------------------------------

// �g���t�B�[�f�[�^�̏�����
TROPHY_DATA::TROPHY_DATA()
{
	pIcon = nullptr;
	sTitle = "�Ȃ�";
	sText = "�Ȃ�";
	vPos = VECTOR2_ZERO;
	iAlpha = 1.0f;
}

// ������
BaseTrophy::BaseTrophy()
{
	m_bAction = false;
	m_iActiveFrame = 0;

	m_pPlate = new tdn2DObj("Data/Trophy/Plate.png");
	m_pIconRip = new tdn2DAnim("Data/Trophy/Rip.png");
	m_pIconRip->OrderRipple(42, 1.0f, 0.02f);

	m_iDelayFrame = 0;

	m_vRoomPos = VECTOR2_ZERO;

	//m_bRock = true;
	//m_pRockIcon = new tdn2DObj("Data/Trophy/Rock.png");

	m_bHide = false;
}

BaseTrophy::~BaseTrophy()
{
	SAFE_DELETE(m_tagTrophy.pIcon);
	SAFE_DELETE(m_pPlate);
	SAFE_DELETE(m_pIconRip);
	//SAFE_DELETE(m_pRockIcon);
}

// �X�V�E�`��
void BaseTrophy::Update()
{
	// ���o���n�܂��Ă��Ȃ��������΂�
	if (m_bAction == false)return;

	// �f�B���C������Ȃ�
	if (m_iDelayFrame > 0) m_iDelayFrame--;
	if (m_iDelayFrame > 0)return;

	m_iActiveFrame++;// ���o�t���[���X�V

	if (m_iActiveFrame >= 240)
	{
		m_bAction = false;
	}

	enum STEP
	{
		INTRO = 12,
		END = 220
	};

	// �ŏ�
	if (m_iActiveFrame <= INTRO)
	{
		m_tagTrophy.iAlpha = (float)m_iActiveFrame / (float)INTRO;
		m_tagTrophy.vPos.x += (float)400 / (float)INTRO;
	}
	// ������
	else if (m_iActiveFrame >= END)
	{
		m_tagTrophy.iAlpha += 0.2f;
		m_tagTrophy.vPos.x -= 40;
	}
	else
	{
		if (m_pIconRip->GetAction()->IsEnd() == true || 
			m_pIconRip->GetAction()->IsAction() == false)
		{
			m_pIconRip->Action();
		}

	}

	// �A���t�@
	m_tagTrophy.iAlpha = Math::Clamp(m_tagTrophy.iAlpha, 0.0f, 1.0f);

	// �g��
	m_pIconRip->Update();

}

void BaseTrophy::Render()
{
	// ���o���n�܂��Ă��Ȃ��������΂�
	if (m_bAction == false)return;

	// �f�B���C������Ȃ�
	if (m_iDelayFrame > 0)return;

	int l_iX = (int)m_tagTrophy.vPos.x;
	int l_iY = (int)m_tagTrophy.vPos.y;
	int l_iAlpha = (int)(255 * m_tagTrophy.iAlpha);
	DWORD l_dCol = ARGB(l_iAlpha, 255, 255, 255);

	// �v���[�g
	m_pPlate->SetAlpha(l_iAlpha);
	m_pPlate->Render(l_iX, l_iY);

	// �A�C�R��
	m_tagTrophy.pIcon->SetAlpha(l_iAlpha);
	m_tagTrophy.pIcon->Render(l_iX + 10, l_iY + 4 , 64, 64, 0, 0, 128, 128);

	// �^�C�g��
	//tdnFont::RenderString(m_tagTrophy.sTitle.c_str(), "HG�޼��E",
	//	22, l_iX + 72, l_iY + 6 , 0xffffffff, RS::COPY);
	tdnFont::RenderString("�g���t�B�[����ɓ���܂����I", "HG�޼��E",
		22, l_iX + 82, l_iY + 6, l_dCol, RS::COPY);

	//// �����`��
	//tdnFont::RenderString(m_tagTrophy.sText.c_str(), "HG�޼��E",
	//	19, l_iX + 72, l_iY +36, 0xffffffff, RS::COPY);
	tdnFont::RenderString(m_tagTrophy.sTitle.c_str(), "HG�޼��E",
		19, l_iX + 82, l_iY + 36, l_dCol, RS::COPY);

	// �g��
	m_pIconRip->Render(l_iX + 10, l_iY + 4, 64, 64, 0, 0, 128, 128, RS::ADD);

}

// 
void BaseTrophy::Action(int iDelayFrame)
{
	m_bAction = true;
	m_iActiveFrame = 0;

	// �����ʒu
	m_tagTrophy.vPos.x = -400;
	m_tagTrophy.vPos.y = 48;
	m_tagTrophy.iAlpha = 0.0f;

	// �g�� �r������`�悵�����̂�
	m_pIconRip->Stop();

	m_iDelayFrame = iDelayFrame;

	// SE�̍Đ�
	se->Play("�g���t�B�[");
}

// ���o��؂�
void BaseTrophy::Stop()
{
	m_bAction = false;
	m_iActiveFrame = 0;

}

//	�{���p
void BaseTrophy::RenderRoom(int iX, int iY/*, bool bRock*/)
{
	//�@���b�N���Ȃ�
	//if (bRock)
	//{
	//	//m_pRockIcon->SetScale(0.5f);
	//	//m_pRockIcon->Render((int)m_vRoomPos.x + iX, (int)m_vRoomPos.y + iY);
	//
	//}else
	//{
		// �A�C�R��
		m_tagTrophy.pIcon->SetAlpha(255);
		m_tagTrophy.pIcon->Render((int)m_vRoomPos.x + iX, (int)m_vRoomPos.y + iY);
	//}

}

// �^�C�g���E��������
void BaseTrophy::RenderInfo(int iX, int iY)
{
	// �B���g���t�B�[�Ȃ�
	//if (m_bHide)
	//{

	//}
	//else
	{	
		// �^�C�g��
		tdnFont::RenderString(m_tagTrophy.sTitle.c_str(), "HG�޼��E",
			22, iX, iY, 0xffffffff, RS::COPY);
		// ����
		tdnFont::RenderString(m_tagTrophy.sText.c_str(), "HG�޼��E",
			19, iX, iY + 30, 0xffffffff, RS::COPY);

	}


}


//+---------------------------------
//	���v���C�̃g���t�B�[
//+---------------------------------

FirstTrophy::FirstTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/first.png");
	m_tagTrophy.sTitle = "�^�Ẵt�@�C�g�J��";
	m_tagTrophy.sText = "���߂ăQ�[�����v���C����";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	���߂đΐ������
//+---------------------------------
FirstBattleTrophy::FirstBattleTrophy()
{	
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/Battle.png");
	m_tagTrophy.sTitle = "���ΐ�";
	m_tagTrophy.sText = "���߂đΐ킵���B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}


//+---------------------------------
//	�傫���_���[�W���o����
//+---------------------------------
BigDamageTrophy::BigDamageTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/power.png");
	m_tagTrophy.sTitle = "���͂ȃR���{";
	m_tagTrophy.sText = "���̃R���{��5000�_���[�W�ȏ�^�����B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}


//+---------------------------------
//	�ő�HP�ŏ�������
//+---------------------------------
MaxHpFinishTrophy::MaxHpFinishTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/hart.png");
	m_tagTrophy.sTitle = "�p�[�t�F�N�g����";
	m_tagTrophy.sText = "�_���[�W���󂯂��ɏ��������B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}


//+---------------------------------
//	������������
//+---------------------------------
SpeedFinishTrophy::SpeedFinishTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/timer.png");
	m_tagTrophy.sTitle = "�u�E";
	m_tagTrophy.sText = "20�b�ȓ��ŏ��������B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	�g���[�j���O�ɘA�����Ԃ�������
//+---------------------------------
TrainingTimeTrophy::TrainingTimeTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/Plactice.png");
	m_tagTrophy.sTitle = "�w�͉�";
	m_tagTrophy.sText = "�A����5���ȏ�g���[�j���O���[���ŗ��K�����B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	�ō���Փx��COM��|����
//+---------------------------------
PowerfulEnemyWinTrophy::PowerfulEnemyWinTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/medal.png");
	m_tagTrophy.sTitle = "COM�}�X�^�[";
	m_tagTrophy.sText = "�ō���Փx��COM�ɏ������B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	SS�����N�œ|����
//+---------------------------------
SSRankWinTrophy::SSRankWinTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/star.png");
	m_tagTrophy.sTitle = "���S����";
	m_tagTrophy.sText = "SS�����N�ŏ��������B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	�ō���Փx��COM+SS�����N��|����
//+---------------------------------
PowerfulEnemySSRankWinTrophy::PowerfulEnemySSRankWinTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/starMedal.png");
	m_tagTrophy.sTitle = "�^�Ă��ɂ߂���";
	m_tagTrophy.sText = "�ō���Փx��COM��SS�����N�ŏ��������B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	�R���e���c����������������
//+---------------------------------
BuyManyContentTrophy::BuyManyContentTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/item.png");
	m_tagTrophy.sTitle = "�����Ŕ����Ȃ����l������";
	m_tagTrophy.sText = "10��ވȏ�R���e���c����������B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	�`�������W���[�h�Ń^�X�N���N���A
//+---------------------------------
ChallengeTrophy::ChallengeTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/kusuri.png");
	m_tagTrophy.sTitle = "�R���{�}�X�^�[";
	m_tagTrophy.sText = "�`�������W���[�h�ōō���Փx�̃^�X�N���N���A�����B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}

//+---------------------------------
//	�S�Ẵg���t�B�[���W�߂�
//+---------------------------------
CompleteTrophy::CompleteTrophy()
{
	m_tagTrophy.pIcon = new tdn2DObj("Data/Trophy/full.png");
	m_tagTrophy.sTitle = "�^�Ă̔e��";
	m_tagTrophy.sText = "�S�Ẵg���t�B�[����肵���B";

	// �B���g���t�B�[�t���O
	m_bHide = false;
}
