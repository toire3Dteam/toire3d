#include "TipsCard.h"
#include "Sound\SoundManager.h"

TipsCard::TipsCard(LPSTR string, bool bChoice, bool bOK)
{
	// ����
	m_pString = string;
	m_pCardPic = new tdn2DAnim("DATA/UI/Menu/TipsCard/TipsCard.png");
	m_pCardPic->OrderStretch(8, 1.0f, 0.0f);
	m_iAlpha = 0;
	m_eStep = STEP::INTRO;
	m_eSelectState = SELECT_STATE::HOLD;
	m_iWaitTimer = 0;
	m_bChoice = bChoice;
	m_vCardPos.x = 227;
	m_vCardPos.y = 252;
	m_pChoiceOKPic = new tdn2DAnim("DATA/UI/Menu/TipsCard/selectPlate.png");
	m_pChoiceOKPic->OrderJump(6, 1, 0.05f);
	m_pChoiceCancelPic = new tdn2DAnim("DATA/UI/Menu/TipsCard/selectPlate.png");
	m_pChoiceCancelPic->OrderJump(6, 1, 0.05f);

	// OK
	if (bChoice == true)
	{
		m_bOK = bOK;
	}else
	{
		m_bOK = true;
	}

	m_bRender = false;
}

TipsCard::~TipsCard()
{
	SAFE_DELETE(m_pCardPic);
	SAFE_DELETE(m_pChoiceOKPic);
	SAFE_DELETE(m_pChoiceCancelPic);
}

void TipsCard::Update(int DeviceID)
{
	if (m_bRender == false)return;

	switch (m_eStep)
	{
	case TipsCard::INTRO:
		m_iWaitTimer++;
		if (m_iWaitTimer > 6)
		{
			m_eStep = TipsCard::STEP::EXECUTE;
			//m_pChoicePic->Action();
		}

		// �J�[�h�̃A���t�@
		m_pCardPic->SetAlpha(255);
		break;
	case TipsCard::EXECUTE:
	{
		m_iAlpha += (255 / 7);
		if (m_iAlpha >= 255)
		{
			m_iAlpha = 255;
		}

		// �J�[�h�̃A���t�@
		m_pCardPic->SetAlpha(255);

		/*****************/
		// �R���g���[��
		/*****************/

		//// �p�b�h�̐�
		//int NumDevice(tdnInputManager::GetNumDevice());

		//int count = NumDevice;

		//// �p�b�h�����������ĂȂ��Ƃ��͂P��
		//if (NumDevice == 0)	count = 1;

		//for (int i = 0; i < count; i++)
		//{

			// �I�����������
			if (m_bChoice)
			{


				if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
				{
					if (m_bOK == true)
					{
						m_pChoiceCancelPic->Action();
						m_bOK = false;
					}
					else
					{
						m_pChoiceOKPic->Action();
						m_bOK = true;
					}

					// SE
					se->Play("�J�[�\��1");

					return;
				}
				if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
				{
					if (m_bOK == true)
					{
						m_pChoiceCancelPic->Action();
						m_bOK = false;
					}
					else
					{
						m_pChoiceOKPic->Action();
						m_bOK = true;
					}

					// SE
					se->Play("�J�[�\��1");
					
					return;
				}
			}

			// ����
			if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
			{
				if (m_bOK == true)
				{
					m_eSelectState = SELECT_STATE::OK;
					
					// SE
					se->Play("����1");
				}
				else
				{
					m_eSelectState = SELECT_STATE::CANCEL;
					
					// SE
					se->Play("�L�����Z��1");
				}



				End();

				return;
			}

			// �~
			if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
			{
				if (m_bChoice)
				{
					m_eSelectState = SELECT_STATE::CANCEL;
				}
				else
				{
					m_eSelectState = SELECT_STATE::OK;
				}

				// SE
				se->Play("�L�����Z��1");

				End();

				return;
			}

		//}// NumDevice�����For��

	}	break;
	case TipsCard::END:
		m_iAlpha -= (255 / 6);
		if (m_iAlpha <= 0)
		{
			m_iAlpha = 0;
			m_bRender = false;// �I��
		}

		// �J�[�h�̃A���t�@
		m_pCardPic->SetAlpha(m_iAlpha);

		break;
	default:
		break;
	}

	m_pCardPic->Update();
	m_pChoiceOKPic->Update();
	m_pChoiceCancelPic->Update();
}

void TipsCard::Render()
{
	if (m_bRender == false)return;

	// �F
	DWORD col = ARGB(m_iAlpha, 255, 255, 255);
	//m_pCardPic->SetARGB(col);


	m_pCardPic->Render((int)m_vCardPos.x, (int)m_vCardPos.y);
	
	// �����`��
	//tdnFont::RenderString(m_pString, "HG�޼��E", 26, 340, 287, 0xffffffff, RS::COPY);
	tdnFont::RenderStringCentering(m_pString, "HG�޼��E", 24, (1280 / 2) + 22 , 287, col, RS::COPY);

	// �I���������邩�Ȃ���
	if (m_bChoice == true)
	{
		// �F
		m_pChoiceOKPic->SetAlpha(m_iAlpha);
		m_pChoiceOKPic->Render((int)m_vCardPos.x, (int)m_vCardPos.y + 126, 820, 32, 0, (m_bOK * 32), 820, 32);
		
		m_pChoiceCancelPic->SetAlpha(m_iAlpha);
		m_pChoiceCancelPic->Render((int)m_vCardPos.x, (int)m_vCardPos.y + 159, 820, 32, 0, 32 - (m_bOK * 32), 820, 32);

		tdnFont::RenderStringCentering("OK", "HGS�n�p�p�޼��UB", 26, (1280 / 2) + 16,
			(int)m_vCardPos.y + 126 + 4, ARGB(m_iAlpha, 0, 0, 0), RS::COPY);

		tdnFont::RenderStringCentering("�L�����Z��", "HG�޼��E", 26, (1280 / 2) + 16,
			(int)m_vCardPos.y + 159 + 4, ARGB(m_iAlpha, 0, 0, 0), RS::COPY);

	}else
	{
		// �F
		m_pChoiceOKPic->SetAlpha(m_iAlpha);
		m_pChoiceOKPic->Render((int)m_vCardPos.x, (int)m_vCardPos.y + 159, 820, 32, 0, (m_bOK * 32), 820, 32);

		tdnFont::RenderStringCentering("OK", "HGS�n�p�p�޼��UB", 26, (1280 / 2) + 16,
			(int)m_vCardPos.y + 159 + 4 , ARGB(m_iAlpha, 0, 0, 0), RS::COPY);

	}

}

void TipsCard::Action()
{
	m_bRender = true;

	m_iAlpha = 0;
	m_eStep = STEP::INTRO;
	m_eSelectState =SELECT_STATE::HOLD;
	m_iWaitTimer = 0;
	m_pCardPic->Action();
}

void TipsCard::End()
{
	m_eStep = STEP::END;
}
