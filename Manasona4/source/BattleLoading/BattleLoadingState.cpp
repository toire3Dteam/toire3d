#include "TDNLIB.h"
#include "BattleLoading.h"
#include "BattleLoadingState.h"
#include "../Fade/Fade.h"
#include "../BaseEntity/Message/Message.h"
#include "Data\SelectData.h"
#include "system\System.h"
#include "system\FrameworkEx.h"


int GetCharaType(SIDE eSide)
{
	int l_iNo = 0;

	if (eSide == SIDE::LEFT)
	{
		l_iNo = (int)SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter;
	}
	else
	{
		l_iNo = (int)SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter;
	}

	return l_iNo;
}

/*******************************************************/
//					�J�n�X�e�[�g
/*******************************************************/

void BattleLoadingState::Intro::Enter(BattleLoading *pMain)
{
	// �t�F�[�h������
	// Fade::Set(Fade::FLAG::FADE_IN, 64);

	for (int i = 0; i < BattleLoading::IMAGE::ARRAY_END; i++)
	{
		pMain->m_pImages[i].End();
	}
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		pMain->m_tagSlideChara1P[i].End();
		pMain->m_tagSlideChara2P[i].End();
		pMain->m_tagFinalChara1P[i].End();
		pMain->m_tagFinalChara2P[i].End();
	}

	// �ŏ��͍�
	pMain->m_fFadeRate = 1.0f;

	// �������o
	//pMain->m_pImages[BattleLoading::IMAGE::BLUE_RING].Action();
	pMain->m_pImages[BattleLoading::IMAGE::BLACK_LINE].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].Action();

	
	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;

}

void BattleLoadingState::Intro::Execute(BattleLoading *pMain)
{


	// 
	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 18)
	{
		pMain->m_fFadeRate -= 0.1f;
	}

	if (pMain->m_iSceneFrame == 18)
	{
		pMain->m_pImages[BattleLoading::IMAGE::I_INTRO_LIGHT].Action();
	}

	//if (pMain->m_fFadeRate <= 0.0f)
	if (pMain->m_iSceneFrame >= 46)
	{
		// 1P�̃X���C�h��
		pMain->GetFSM()->ChangeState(BattleLoadingState::Slide1P::GetInstance());
		return;
	}
}

void BattleLoadingState::Intro::Exit(BattleLoading *pMain) 
{
	pMain->m_pImages[BattleLoading::IMAGE::I_INTRO_LIGHT].End();

}

void BattleLoadingState::Intro::Render(BattleLoading *pMain)
{

	// ������
	pMain->m_pIntroBlackBack->Render(0, 0);


#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
#endif // _DEBUG

}

bool BattleLoadingState::Intro::OnMessage(BattleLoading *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					1P�X���C�h�X�e�[�g
/*******************************************************/

void BattleLoadingState::Slide1P::Enter(BattleLoading *pMain)
{
	// ����̃t���[���Ȃǂ̃C���X�g
	pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_VS].Action();

	// 1PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_1PLAYER].Action();
	//pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].vPos = VECTOR2_ZERO;

	// �L����
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Action();

	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::Slide1P::Execute(BattleLoading *pMain)
{
	// �X�V
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	// �X�s�[�h���C��
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].vPos.x += 20.0f;

	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 120)
	{
		// 2P�̃X���C�h��
		pMain->GetFSM()->ChangeState(BattleLoadingState::Slide2P::GetInstance());
		return;
	}

}

void BattleLoadingState::Slide1P::Exit(BattleLoading *pMain)
{
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].End();
	
	// PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_1PLAYER].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].End();


}

void BattleLoadingState::Slide1P::Render(BattleLoading *pMain)
{
	// �`��
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Render();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Render();

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Slide1P");
#endif // _DEBUG

}

bool BattleLoadingState::Slide1P::OnMessage(BattleLoading *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					2P�X���C�h�X�e�[�g
/*******************************************************/

void BattleLoadingState::Slide2P::Enter(BattleLoading *pMain)
{
	// �L����
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Action();

	// 2PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_2PLAYER].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].vPos = VECTOR2_ZERO;


	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::Slide2P::Execute(BattleLoading *pMain)
{
	// �X�V
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	// �X�s�[�h���C��
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].vPos.x -= 20.0f;

	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 120)
	{
		// �X���C�h�I���
		pMain->GetFSM()->ChangeState(BattleLoadingState::SlideEnd::GetInstance());
		return;
	}

}

void BattleLoadingState::Slide2P::Exit(BattleLoading *pMain)
{
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].End();
	
	// PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_2PLAYER].End();
	//pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK].End();// �܂�2P�̔w�i�c��
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].End();


}

void BattleLoadingState::Slide2P::Render(BattleLoading *pMain)
{
	// �`��
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Render();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Render();

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Slide2P");
#endif // _DEBUG

}

bool BattleLoadingState::Slide2P::OnMessage(BattleLoading *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					�X���C�h�I��X�e�[�g
/*******************************************************/

void BattleLoadingState::SlideEnd::Enter(BattleLoading *pMain)
{
	pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_VS].End();

	// �ŏI�t���[���̉��o�J�n
	pMain->m_pImages[BattleLoading::IMAGE::F_FRAME].Action(6);

	// ����~
	pMain->m_pImages[BattleLoading::IMAGE::BLUE_RING].Action();

	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::SlideEnd::Execute(BattleLoading *pMain)
{

	// �X�V
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	// �X���C�h����UI���͂���
	pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].AddPos(Vector2(0, -4.5));
	pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].AddPos(Vector2(0, 4.5));
	pMain->m_pImages[BattleLoading::IMAGE::S_VS].AddPos(Vector2(0, 4));


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 8)
	{
		// ����
		pMain->GetFSM()->ChangeState(BattleLoadingState::FinalStep::GetInstance());
		return;
	}

}

void BattleLoadingState::SlideEnd::Exit(BattleLoading *pMain)
{
	// �����Ŕw�i����
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK].End();
}

void BattleLoadingState::SlideEnd::Render(BattleLoading *pMain)
{
	

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "SlideEnd");
#endif // _DEBUG

}

bool BattleLoadingState::SlideEnd::OnMessage(BattleLoading *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�ŏI�X�e�[�g
/*******************************************************/

void BattleLoadingState::FinalStep::Enter(BattleLoading *pMain)
{
	// �㉺�̃��C��������
	pMain->m_pImages[BattleLoading::IMAGE::BLACK_LINE].End();

	// �J�n
	pMain->m_pImages[BattleLoading::IMAGE::F_BLACK_CIRCLE].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].fAngle = 0.0f;
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].fAngle = 0.0f;

	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].vPos = VECTOR2_ZERO;

	pMain->m_pImages[BattleLoading::IMAGE::F_BACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_FLASH_BACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_VERSUS].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_FLASH].Action();

	// �L����
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Action();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Action();


	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::FinalStep::Execute(BattleLoading *pMain)
{
	// �A�j���[�V����
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].fAngle += 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].fAngle -= 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].vPos.y += 2.0f;

	//// �X�V
	//pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	//pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();
	//
	//// �X���C�h����UI���͂���
	//pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].AddPos(Vector2(0, -4.5));
	//pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].AddPos(Vector2(0, 4.5));
	//pMain->m_pImages[BattleLoading::IMAGE::S_VS].AddPos(Vector2(0, 4));

	// �L����
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Update();

	// ���̃t���[���܂ŃL����������������
	if (pMain->m_iSceneFrame <= 60)
	{
		pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].vPos.x -= 0.25f;
		pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].vPos.x += 0.25f;
	}

	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 180)
	{
		// ����
		pMain->GetFSM()->ChangeState(BattleLoadingState::FadeChangeStep::GetInstance());
		return;
	}

	// �X�L�b�v�\�Ȃ�
	if (pMain->m_bSkip == true)
	{
		// �p�b�h���X�V
		int NumDevice(tdnInputManager::GetNumDevice());
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			// ���j���[�؂�ւ�
			if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, i) == 3||
				tdnInput::KeyGet(KEYCODE::KEY_A, i) == 3 || 
				tdnInput::KeyGet(KEYCODE::KEY_B, i) == 3 )
			{
				// ����
				pMain->GetFSM()->ChangeState(BattleLoadingState::FadeChangeStep::GetInstance());
				return;

			}
		}
	}
	

}

void BattleLoadingState::FinalStep::Exit(BattleLoading *pMain)
{
	// (TODO)
	//pMain->m_pImages[BattleLoading::IMAGE::F_BLACK_CIRCLE].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_BACK].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_VERSUS].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_FRAME].End();

}

void BattleLoadingState::FinalStep::Render(BattleLoading *pMain)
{

	// �L����
	//shader2D->SetMaskScreen(pMain->m_pMask2P);
	//pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Render();
	//shader2D->SetMaskScreen(pMain->m_pMask2P);

	// �}�X�N
	pMain->MaskRender(pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)]);
	shader2D->SetMaskScreen(pMain->m_pMask1P);
	pMain->m_pMaskSurface->Render(0, 0,shader2D,"Mask");

	// �}�X�N
	pMain->MaskRender(pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)]);
	shader2D->SetMaskScreen(pMain->m_pMask2P);
	pMain->m_pMaskSurface->Render(0, 0, shader2D, "Mask");


#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "SlideEnd");
#endif // _DEBUG

}

bool BattleLoadingState::FinalStep::OnMessage(BattleLoading *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�t�F�[�h�`�F���W�X�e�[�g
/*******************************************************/

void BattleLoadingState::FadeChangeStep::Enter(BattleLoading *pMain)
{
	// �t�F�[�h
	// Fade::Set(Fade::FLAG::FADE_OUT, 8);

	pMain->m_pImages[BattleLoading::IMAGE::LOADING_CIRCLE].Action();

	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::FadeChangeStep::Execute(BattleLoading *pMain)
{

	// �X�V
	pMain->m_fFadeRate += 0.05f;

	// �A�j���[�V����
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].fAngle += 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].fAngle -= 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].vPos.y += 2.0f;

	// ���[�f�B���O�p
	pMain->m_pImages[BattleLoading::IMAGE::LOADING_CIRCLE].fAngle -= 0.1f;


	// �L����
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Update();


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 28)
	{
		// �����o�I������FPS��߂�
		if (MainFrameEX->GetFPSMode() != FPS_MODE::NORMAL)
		{
			MainFrameEX->SetFPSMode(FPS_MODE::NORMAL);
		}

		// ���o�I��
		pMain->SetEndFlag(true);

		//pMain->GetFSM()->ChangeState(BattleLoadingState::Intro::GetInstance());
		//return;	
	}

}

void BattleLoadingState::FadeChangeStep::Exit(BattleLoading *pMain)
{
	pMain->m_pImages[BattleLoading::IMAGE::LOADING_CIRCLE].End();
}

void BattleLoadingState::FadeChangeStep::Render(BattleLoading *pMain)
{

	// �L����
	//shader2D->SetMaskScreen(pMain->m_pMask2P);
	//pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Render();
	//shader2D->SetMaskScreen(pMain->m_pMask2P);

	// �}�X�N
	pMain->MaskRender(pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)]);
	shader2D->SetMaskScreen(pMain->m_pMask1P);
	pMain->m_pMaskSurface->Render(0, 0, shader2D, "Mask");

	// �}�X�N
	pMain->MaskRender(pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)]);
	shader2D->SetMaskScreen(pMain->m_pMask2P);
	pMain->m_pMaskSurface->Render(0, 0, shader2D, "Mask");


#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "SlideEnd");
#endif // _DEBUG

}

bool BattleLoadingState::FadeChangeStep::OnMessage(BattleLoading *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}









