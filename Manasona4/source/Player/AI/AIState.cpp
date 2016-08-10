#include "AI.h"
#include "AIState.h"
#include "../../Player/BasePlayer.h"
#include "../../Player/PlayerManager.h"
#include "../BasePlayerState.h"

#define MyPlayer (pPerson->GetBasePlayer())

//void PushButton(AI* pPerson, PLAYER_INPUT input)
//{
//	pPerson->PushInputList(input);
//}

/*******************************************************/
//					�O���[�o���X�e�[�g
/*******************************************************/

AIState::Global * AIState::Global::GetInstance()
{
	// �����ɕϐ������
	static AIState::Global instance;
	return &instance;
}

void AIState::Global::Enter(AI * pPerson)
{
}

void AIState::Global::Execute(AI * pPerson)
{
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == true
		)
	{
		// ���A���[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Recovery::GetInstance());
	}
}

void AIState::Global::Exit(AI * pPerson)
{
}

void AIState::Global::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI����[�΂邷�ā[��");
}

bool AIState::Global::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//				���K�p�W�����v�X�e�[�g
/*******************************************************/

AIState::PracticeJump * AIState::PracticeJump::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeJump instance;
	return &instance;
}

void AIState::PracticeJump::Enter(AI * pPerson)
{
}

void AIState::PracticeJump::Execute(AI * pPerson)
{
	if (MyPlayer->isLand())
	{
		// �W�����v�{�^�������Ă�
		pPerson->PushInputList(PLAYER_INPUT::C);

	}

}

void AIState::PracticeJump::Exit(AI * pPerson)
{
}

void AIState::PracticeJump::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�W�����v");
}

bool AIState::PracticeJump::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//				�ǂ�������X�e�[�g
/*******************************************************/

AIState::Chase * AIState::Chase::GetInstance()
{
	// �����ɕϐ������
	static AIState::Chase instance;
	return &instance;
}

void AIState::Chase::Enter(AI * pPerson)
{
	//	�N�������������邩�l����

	//�߂�������[���[�Ƃʂ��u
	float length2 = 114514810.0f;
	int targetno = 0;
	for (int i = 0; i < PlayerMgr->GetNumPlayer(); i++)
	{
		if (MyPlayer->GetDeviceID() == PlayerMgr->GetPlayer(i)->GetDeviceID())
		{
			continue;
		}

		if (PlayerMgr->GetPlayer(i)->isDownState())
		{
			continue;
		}

		float length= Math::Length(PlayerMgr->GetPlayer(i)->GetPos(), MyPlayer->GetPos());
		if (length<length2)
		{
			length2 = length;
			targetno = i;
		}
	}
	
	pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetno));



	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	if (TargetPos.x > MyPos.x)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);

	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);


	}
}

void AIState::Chase::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	if (TargetPos.x > MyPos.x)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);

	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);


	}

	// ���܂ŋ߂Â�����
	if (Math::Length(TargetPos, MyPos) < 22)
	{
		// �n�ォ�󒆂�
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
			return;
		}
		else
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return;
		}
	}
	

}

void AIState::Chase::Exit(AI * pPerson)
{
}

void AIState::Chase::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�W�����v");
}

bool AIState::Chase::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}





/*******************************************************/
//			���b�V���A�^�b�N�X�e�[�g
/*******************************************************/

AIState::RushAtack * AIState::RushAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::RushAtack instance;
	return &instance;
}

void AIState::RushAtack::Enter(AI * pPerson)
{
	pPerson->PushInputList(PLAYER_INPUT::B);
}

void AIState::RushAtack::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ���܂ŗ��ꂽ��
	if (
		//Math::Length(TargetPos, MyPos) > 6

		// �v���C���[�̃��b�V���A�^�b�N�X�e�[�g�I�����Ă���
		!MyPlayer->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance())
		)
	{
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// �U���I�I
	//if ()
	{
		pPerson->PushInputList(PLAYER_INPUT::B);

	}

}

void AIState::RushAtack::Exit(AI * pPerson)
{
}

void AIState::RushAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "���b�V���U��");
}

bool AIState::RushAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}






/*******************************************************/
//			���A�X�e�[�g
/*******************************************************/

AIState::Recovery * AIState::Recovery::GetInstance()
{
	// �����ɕϐ������
	static AIState::Recovery instance;
	return &instance;
}

void AIState::Recovery::Enter(AI * pPerson)
{
	
}

void AIState::Recovery::Execute(AI * pPerson)
{
	// ���A�@�����̃{�^���������ςȂ�
	pPerson->PushInputList(PLAYER_INPUT::C);

	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == false &&
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == false &&
		MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == false)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	}

}

void AIState::Recovery::Exit(AI * pPerson)
{
}

void AIState::Recovery::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "���J�o���[");
}

bool AIState::Recovery::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			�΋󏀔��X�e�[�g
/*******************************************************/

AIState::SetAnitiAir * AIState::SetAnitiAir::GetInstance()
{
	// �����ɕϐ������
	static AIState::SetAnitiAir instance;
	return &instance;
}

void AIState::SetAnitiAir::Enter(AI * pPerson)
{
	// ���Ⴊ�݉������ςȂ�
	pPerson->PushInputList(PLAYER_INPUT::DOWN);
}

void AIState::SetAnitiAir::Execute(AI * pPerson)
{
	// ���Ⴊ�݉������ςȂ�
	pPerson->PushInputList(PLAYER_INPUT::DOWN);

	// ���Ⴊ�݂���߂锻��
	if (pPerson->GetTargetPlayer()->isLand())
	{
		// ����͒n��ɂ���̂ő΋����߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �΋�͈͓�
	if (
		//Math::Length(TargetPos, MyPos) < 24
		abs(TargetPos.y - MyPos.y) < 24
		)
	{
		// ����炪���Ⴊ�݃��[�h�ɂȂ��Ă���
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()) == true)
		{
			// �΋󔭓��I
			pPerson->GetFSM()->ChangeState(AIState::AnitiAir::GetInstance());

		}
	}



}

void AIState::SetAnitiAir::Exit(AI * pPerson)
{
}

void AIState::SetAnitiAir::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SetAnitiAir");
}

bool AIState::SetAnitiAir::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}



/*******************************************************/
//			�΋�X�e�[�g
/*******************************************************/

AIState::AnitiAir * AIState::AnitiAir ::GetInstance()
{
	// �����ɕϐ������
	static AIState::AnitiAir  instance;
	return &instance;
}

void AIState::AnitiAir ::Enter(AI * pPerson)
{
	// �΋�U��
	pPerson->PushInputList(PLAYER_INPUT::B);
}

void AIState::AnitiAir ::Execute(AI * pPerson)
{
	
	// �ǐՃ��[�h�ɖ߂�
	pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());

}

void AIState::AnitiAir ::Exit(AI * pPerson)
{
}

void AIState::AnitiAir ::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AnitiAir");
}

bool AIState::AnitiAir ::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}