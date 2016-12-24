#include "ComboUI.h"

//ComboUI::ComboUI()
//{
//	m_num = new Number("DATA/UI/Combo/Number.png", 128, Number::NUM_KIND::COMBO);
//	m_damageNum = new Number("DATA/UI/Combo/damageNumber.png", 32, Number::NUM_KIND::NORMAL);
//	//m_num->SetCol(Number::RGB(,255,255,255));
//
//	// �R���{
//	m_iCount = 0;
//	m_iMaxCount = 0;
//
//	m_iDamage = 0;
//	m_iMaxDamage = 0;
//
//	// 
//	m_frontPic = new tdn2DObj("DATA/UI/Combo/front.png");
//	m_backPic = new tdn2DObj("DATA/UI/Combo/back.png");
//
//	m_gageFramePic = new tdn2DObj("DATA/UI/Combo/comboGageFrame.png");
//	m_gageBackPic = new tdn2DObj("DATA/UI/Combo/comboGageFrameBack.png");
//	m_gagePic = new tdn2DObj("DATA/UI/Combo/comboGage.png");
//
//	m_pPlayerData = nullptr;
//	m_iRecoveryFrame = 0;
//	m_iMaxRecoveryFrame = 100;
//
//	// �J�E���^�[
//	m_tagCounter.pPic = new tdn2DAnim("Data/UI/Combo/Counter.png");
//	m_tagCounter.pPic->OrderAlphaMove(120, 2, 120 - 8);
//	
//
//}

ComboUI::ComboUI(BasePlayer* PlayerData)
{
	m_num = new Number("DATA/UI/Combo/Number.png", 128, Number::NUM_KIND::COMBO);
	m_damageNum = new Number("DATA/UI/Combo/damageNumber.png", 32, Number::NUM_KIND::NORMAL);
	//m_num->SetCol(Number::RGB(,255,255,255));
	// �R���{
	m_iCount = 0;
	m_iMaxCount = 0;

	m_iDamage = 0;
	m_iNowDamage = 0;
	m_iComboDamage = 0;
	m_iMaxDamage = 0;

	// 
	m_frontPic = new tdn2DObj("DATA/UI/Combo/front.png");
	m_backPic = new tdn2DObj("DATA/UI/Combo/back.png");

	m_gageFramePic = new tdn2DObj("DATA/UI/Combo/comboGageFrame.png");
	m_gageBackPic = new tdn2DObj("DATA/UI/Combo/comboGageFrameBack.png");
	m_gagePic = new tdn2DObj("DATA/UI/Combo/comboGage.png");

	m_pPlayerData = PlayerData;
	m_iRecoveryFrame = 0;
	m_iMaxRecoveryFrame = 100;

	m_bRenderFlag = false;
	m_iRenderFlagWaitFrame = 0;

	m_bGuardFlag = false;

	// �J�E���^�[
	m_tagCounter.pPic = new tdn2DAnim("Data/UI/Combo/Counter.png");
	m_tagCounter.pPic->OrderAlphaMove(90, 4, 90 - 8);
	
	m_eSide = m_pPlayerData->GetSide();
	if (m_eSide == SIDE::LEFT)
	{
		m_vPos.x = (int)100;
		m_vPos.y = (int)200;
	}else
	{
		m_vPos.x = (int)1050;
		m_vPos.y = (int)200;
	}
	m_tagCounter.iAddX = 0;

}

ComboUI::~ComboUI()
{
	SAFE_DELETE(m_num);
	SAFE_DELETE(m_damageNum);

	SAFE_DELETE(m_frontPic);
	SAFE_DELETE(m_backPic)

	SAFE_DELETE(m_gageFramePic);
	SAFE_DELETE(m_gageBackPic);
	SAFE_DELETE(m_gagePic);

	SAFE_DELETE(m_tagCounter.pPic);

}

void ComboUI::Update()
{
	MyAssert(m_iMaxRecoveryFrame != 0, "�ő�l��0����0�Ŋ��邱�ƂɂȂ�");

	// �����̍X�V
	m_num->Update();

	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);

	// 0�ȏ� �K�[�h���Ă��珉�������Ȃ�
	if (rate <= 0.0f || m_bGuardFlag == true)
	{
		// ���[�g��0�Ȃ��2�b��UI������
		m_iRenderFlagWaitFrame++;
		if (m_iRenderFlagWaitFrame > 120)
		{
			m_bRenderFlag = false;
			m_iCount = 0;
			m_iDamage = 0;
		}

	}
	else
	{
		m_bRenderFlag = true;
		m_iRenderFlagWaitFrame = 0;

	}

	// �X�V
	m_tagCounter.pPic->Update();
	// �J�E���^�[�̓���
	if (m_eSide == SIDE::LEFT)
	{
		m_tagCounter.iAddX += 256 / 8;
		if (m_tagCounter.iAddX >= 0)
		{
			m_tagCounter.iAddX = 0;
		}
	}
	else
	{
		m_tagCounter.iAddX -= 256 / 8;
		
		if (m_tagCounter.iAddX <= 0)
		{
			m_tagCounter.iAddX = 0;
		}
	}

}

void ComboUI::Render()
{
	if (m_bRenderFlag == false)return;	// �`�悳���Ȃ�	
	if (m_iCount == 0)return;			// �R���{��0�Ȃ�`�悳���Ȃ�

	m_backPic->Render((int)m_vPos.x, (int)m_vPos.y, RS::COPY_NOZ);
	m_num->Render((int)m_vPos.x, (int)m_vPos.y -14, m_iCount, Number::NUM_KIND::COMBO);// ���l
	m_frontPic->Render((int)m_vPos.x, (int)m_vPos.y, RS::COPY_NOZ);

	int gageX, gageY;
	gageX = 16;
	gageY = 144;

	// �Q�[�W��
	m_gageBackPic->Render((int)m_vPos.x + gageX, (int)m_vPos.y + gageY,RS::COPY_NOZ);
	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);
	if (m_bGuardFlag == true)rate = 0.0f;
	m_gagePic->Render((int)m_vPos.x + gageX, (int)m_vPos.y + gageY, int(128 * rate), 64, 0, 0, int(128 * rate), 64, RS::COPY_NOZ);
	m_gageFramePic->Render((int)m_vPos.x + gageX, (int)m_vPos.y + gageY,RS::COPY_NOZ);

	m_damageNum->Render((int)m_vPos.x + gageX + 64, (int)m_vPos.y + gageY - 30, m_iDamage, Number::NUM_KIND::DAMAGE_SCORE);// ���l

	// �J�E���^�[
	m_tagCounter.pPic->Render((int)m_vPos.x + m_tagCounter.iAddX , (int)m_vPos.y);

}

//void ComboUI::GageUpdate()
//{
//	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);
//	
//	if (rate > 0.0f)
//	{
//		m_bRenderFlag = true;
//	}
//	else
//	{
//		m_bRenderFlag = false;
//	}
//}

void ComboUI::Count(int damage, int maxRecovery, bool bCounterHit)
{
	MyAssert(maxRecovery != 0, "�ő�l��0����0�Ŋ��邱�ƂɂȂ�");


	// �E�F�C�g�^�C�}�[���쓮���Ă邶�傤�����Ȃ�
	if (m_iRenderFlagWaitFrame >= 1)
	{
		// ������
		m_iCount = 0;
		m_iDamage = 0;
	}

	m_num->Action();

	m_bGuardFlag = false;

	m_iCount++;	
	// �ő�R���{����ۑ�
	if (m_iMaxCount <= m_iCount)
	{
		m_iMaxCount = m_iCount;
	}
	
	m_iDamage += damage;

	m_iNowDamage = damage;
	m_iComboDamage = m_iDamage;

	// �ő�R���{����ۑ�
	if (m_iMaxDamage <= m_iDamage)
	{
		m_iMaxDamage = m_iDamage;
	}

	m_iMaxRecoveryFrame = maxRecovery;

	// �J�E���^�[�q�b�g�Ȃ�΃J�E���^�[�̉��o�ǉ�
	if (bCounterHit)
	{
		// �J�E���^�[
		m_tagCounter.pPic->Action();
		if (m_eSide == SIDE::LEFT)
		{
			m_tagCounter.iAddX = -256;
		}
		else
		{
			m_tagCounter.iAddX = +256;
		}
	}

}

//void ComboUI::Count(int damage, int maxRecovery,int* recovery)
//{
//	// �E�F�C�g�^�C�}�[���쓮���Ă邶�傤�����Ȃ�
//	if (m_iRenderFlagWaitFrame >= 1)
//	{
//		// ������
//		m_iCount = 0;
//		m_iDamage = 0;
//	}
//
//	m_num->Action();
//	
//	m_bGuardFlag = false;
//
//	m_iCount++;
//	// �ő�R���{����ۑ�
//	if (m_iMaxCount <= m_iCount)
//	{
//		m_iMaxCount = m_iCount;
//	}
//
//	m_iDamage += damage;
//	// �ő�R���{����ۑ�
//	if (m_iMaxDamage <= m_iDamage)
//	{
//		m_iMaxDamage = m_iDamage;
//	}
//
//	m_iMaxRecoveryFrame = maxRecovery;
//
//	m_pRecoveryFrame = recovery;
//}

void ComboUI::Guard()
{
	m_bGuardFlag = true;
}
