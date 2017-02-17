#include "TDNLIB.h"
#include "BattleLoading.h"
#include "BattleLoadingState.h"
#include "../Fade/Fade.h"
#include "../BaseEntity/Message/Message.h"
#include "Data\SelectData.h"
#include "system\System.h"

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
	Fade::Set(Fade::FLAG::FADE_IN, 64);

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

	// �������o
	//pMain->FirstAction();

	pMain->m_pImages[BattleLoading::IMAGE::BLACK_LINE].Action();
	

}

void BattleLoadingState::Intro::Execute(BattleLoading *pMain)
{

	// 1P�̃X���C�h��
	pMain->GetFSM()->ChangeState(BattleLoadingState::Slide1P::GetInstance());

}

void BattleLoadingState::Intro::Exit(BattleLoading *pMain) 
{

}

void BattleLoadingState::Intro::Render(BattleLoading *pMain)
{


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
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].Action();

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
	
	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::Slide2P::Execute(BattleLoading *pMain)
{
	// �X�V
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

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


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 180)
	{
		// ����
		pMain->GetFSM()->ChangeState(BattleLoadingState::FadeChangeStep::GetInstance());
		return;
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
	Fade::Set(Fade::FLAG::FADE_OUT, 8);

	// �^�C�}�[������
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::FadeChangeStep::Execute(BattleLoading *pMain)
{

	// �X�V

	// �L����
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Update();


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 24)
	{
		// ����
		//pMain->GetFSM()->ChangeState(BattleLoadingState::Intro::GetInstance());
		//return;
	}

}

void BattleLoadingState::FadeChangeStep::Exit(BattleLoading *pMain)
{
	// (TODO)
	//pMain->m_pImages[BattleLoading::IMAGE::F_BLACK_CIRCLE].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_BACK].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_VERSUS].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_FRAME].End();

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









