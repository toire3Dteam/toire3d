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

/**************************/
// �t�@���N�V����
/**************************/

// ��ԋ߂��|����v���C���[
bool SearchNearPlayer(AI * pPerson)
{
	enum{ NOT_TARGET = -1 };

	//�߂��v���C���[���^�[�Q�b�g��
	float length2 = FLT_MAX;
	int targetNo = NOT_TARGET;
	for (int i = 0; i < PlayerMgr->GetNumPlayer(); i++)
	{
		// �������g�͂͂���
		if (MyPlayer->GetDeviceID() == PlayerMgr->GetPlayer(i)->GetDeviceID())
		{
			continue;
		}
		// �����`�[�����͂���
		if (MyPlayer->GetSide() == PlayerMgr->GetPlayer(i)->GetSide())
		{
			continue;
		}

		if (PlayerMgr->GetPlayer(i)->isInvincible())
		{
			continue;
		}

		//if (PlayerMgr->GetPlayer(i)->isDownState())
		//{
		//	continue;
		//}

		float length = Math::Length(PlayerMgr->GetPlayer(i)->GetPos(), MyPlayer->GetPos());
		if (length < length2)
		{
			length2 = length;
			targetNo = i;
		}
	}

	if (targetNo ==  NOT_TARGET)
	{
		// �ʖڂ݂���
		// �^�[�Q�b�g�ɂȂ��v���C���[�͂��Ȃ������悤�ł���..
		return false;
	}
	else
	{	
		// �^�[�Q�b�g�ƂƂ��Ă��̃v���C���[���Z�b�g
		pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
		return true;
	}

}

// ��ԋ߂����G���̃v���C���[
bool SearchNearInvinciblePlayer(AI * pPerson)
{
	enum{ NOT_TARGET = -1 };

	//�߂��v���C���[���^�[�Q�b�g��
	float length2 = FLT_MAX;
	int targetNo = NOT_TARGET;
	for (int i = 0; i < PlayerMgr->GetNumPlayer(); i++)
	{
		// �������g�͂͂���
		if (MyPlayer->GetDeviceID() == PlayerMgr->GetPlayer(i)->GetDeviceID())
		{
			continue;
		}

		// �����`�[�����͂���
		if (MyPlayer->GetSide() == PlayerMgr->GetPlayer(i)->GetSide())
		{
			continue;
		}


		if (PlayerMgr->GetPlayer(i)->isInvincible() == false)
		{
			continue;
		}

		//if (PlayerMgr->GetPlayer(i)->isDownState()==false)
		//{
		//	continue;
		//}

			float length = Math::Length(PlayerMgr->GetPlayer(i)->GetPos(), MyPlayer->GetPos());
			if (length < length2)
			{
				length2 = length;
				targetNo = i;
			}
		

	}

	if (targetNo == NOT_TARGET)
	{
		// �ʖڂ݂���
		// �^�[�Q�b�g�ɂȂ��v���C���[�͂��Ȃ������悤�ł���..
		return false;
	}
	else
	{
		// �^�[�Q�b�g�ƂƂ��Ă��̃v���C���[���Z�b�g
		pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
		return true;
	}

}


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
	//tdnText::Draw(420, 610, 0xffffffff, "AI����[�΂邷�ā[��");
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
//					�v���N�e�B�X�O���[�o���X�e�[�g
/*******************************************************/

AIState::PracticeGlobal* AIState::PracticeGlobal::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeGlobal instance;
	return &instance;
}

void AIState::PracticeGlobal::Enter(AI * pPerson)
{

}

void AIState::PracticeGlobal::Execute(AI * pPerson)
{

}

void AIState::PracticeGlobal::Exit(AI * pPerson)
{
}

void AIState::PracticeGlobal::Render(AI * pPerson)
{
	//tdnText::Draw(420, 610, 0xffffffff, "AI����[�΂邷�ā[��");
}

bool AIState::PracticeGlobal::OnMessage(AI * pPerson, const Message & msg)
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
	// 
	if (MyPlayer->isLand())
	{
		// �W�����v
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
//				���K�p�n�ʃX�e�[�g
/*******************************************************/

AIState::PracticeLand * AIState::PracticeLand::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeLand instance;
	return &instance;
}

void AIState::PracticeLand::Enter(AI * pPerson)
{
	pPerson->SetPracticeGuardFrame(120);
	pPerson->SetPracticeGuardFlag(true);
}

void AIState::PracticeLand::Execute(AI * pPerson)
{
	
	// �������U�����󂯂Ă���Œ��Ȃ��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == true
		)
	{
		// ���A�{�^��
		pPerson->PushInputList(PLAYER_INPUT::A);
		pPerson->SetPracticeGuardFrame(0);
		pPerson->SetPracticeGuardFlag(false);
	}


	//  �܂��K�[�h�t���O�������Ă��Ȃ����J�o���[�H�ł��Ă�����
	if (pPerson->GetPracticeGuardFlag() == false)
	{
		// �m�b�N�o�b�N�E�󒆃��J�o���[�E�n�ナ�J�o���[���O�̃X�e�[�g�ɂȂ��Ă�����K�[�h����
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true ||
			MyPlayer->GetFSM()->isPrevState(*BasePlayerState::AerialRecovery::GetInstance()) == true ||
			MyPlayer->GetFSM()->isPrevState(*BasePlayerState::LandRecovery::GetInstance()) == true)
		{
			pPerson->PushInputList(PLAYER_INPUT::R1);// �K�[�h����
			pPerson->SetPracticeGuardFlag(true);
		}
	}
	else
	{
		pPerson->AddPracticeGuardFrame(1);
		if (pPerson->GetPracticeGuardFrame() < 60)
		{
			pPerson->PushInputList(PLAYER_INPUT::R1);// �K�[�h����
			//pPerson->SetPracticeGuardFrame(0);
			//pPerson->SetPracticeGuardFlag(false);
		}

	}

}

void AIState::PracticeLand::Exit(AI * pPerson)
{
}

void AIState::PracticeLand::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�n��");
}

bool AIState::PracticeLand::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}

/*******************************************************/
//				�ҋ@�X�e�[�g
/*******************************************************/

AIState::Wait * AIState::Wait::GetInstance()
{
	// �����ɕϐ������
	static AIState::Wait instance;
	return &instance;
}

void AIState::Wait::Enter(AI * pPerson)
{

}

void AIState::Wait::Execute(AI * pPerson)
{

}

void AIState::Wait::Exit(AI * pPerson)
{
}

void AIState::Wait::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI��������");
}

bool AIState::Wait::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//				�Ԋu���J����X�e�[�g
/*******************************************************/

AIState::Space * AIState::Space::GetInstance()
{
	// �����ɕϐ������
	static AIState::Space instance;
	return &instance;
}

void AIState::Space::Enter(AI * pPerson)
{
	//	�N���瓦���邩�l����
	if (SearchNearInvinciblePlayer(pPerson) == false)
	{
		// ����ς�S��������
		//pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	}

	// �E�F�C�g�^�C�}�[������
	pPerson->SetWaitFrame(0);

}

void AIState::Space::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();


	enum 
	{
		SPACE_LENGTH = 50,// �����鋗��
		SPACE_LENGTH_FAR = 75// ���������Ă���߂Â��鋗��
	};

	if (Math::Length(TargetPos, MyPos) > SPACE_LENGTH_FAR)
	{
		// �ړ� (�߂Â�)
		if (TargetPos.x > MyPos.x)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
	}
	// ���܂ŗ��ꂽ��
	else if (Math::Length(TargetPos, MyPos) > SPACE_LENGTH)
	{
		// �A�N�V�����ł�����(WaitState)�܂Œʂ��Ȃ�
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true ||
			MyPlayer->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()) == true)
		{
			int random = tdnRandom::Get(0, 2);
			if (random == 1 || random == 2)
			{
				// ���ł݂�
				pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
				return;
			}
			else
			{

				// ������Ă݂� 
				//pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());	
				// ���߃K�[�h���Č���
				//pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				// �K���ɉ���������

				return;

			}

		}
	
	}
	else
	{
		// �ړ� (������)
		if (TargetPos.x < MyPos.x)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
	}


	/***************************/
	//	���������Ď��̃X�e�[�g��
	/***************************/
	pPerson->AddWaitFrame(1);			// �҂��^�C�}�[�X�V
	if (pPerson->GetWaitFrame() >= 20)	// 30�b�ԔS�����Ēǂ�������
	{
		pPerson->SetWaitFrame(0); // ������

		//	�N�����������邩�l����
		if (SearchNearPlayer(pPerson) == true)// ���`�F�C�X�̍ŏ��̏������܂߂�����^�[�Q�b�g���Ă邯�ǁ@bool�ł̎g�����Ƃ��Ă��֗��Ȃ̂ō̗p
		{
			// ������
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}
}

void AIState::Space::Exit(AI * pPerson)
{

}

void AIState::Space::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�X�y�[�X�󂯂Ċԍ��������");
}

bool AIState::Space::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//				������ԃX�e�[�g
/*******************************************************/

AIState::SimpleJump * AIState::SimpleJump::GetInstance()
{
	// �����ɕϐ������
	static AIState::SimpleJump instance;
	return &instance;
}

void AIState::SimpleJump::Enter(AI * pPerson)
{
	// �W�����v�{�^������
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::SimpleJump::Execute(AI * pPerson)
{

	// �W�����v�̓��ݍ��݂̓J�E���g���Ȃ�
	if (MyPlayer->GetJump()->bHold == false)
	{
		// �n��ɒ��n������ ���@�ҋ@�X�e�[�g�ɂ����
		if (MyPlayer->isLand() && MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// �ǂ������ɖ߂�
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}
	}

}

void AIState::SimpleJump::Exit(AI * pPerson)
{
}

void AIState::SimpleJump::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�V���v���W�����v");
}

bool AIState::SimpleJump::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//				����X�e�[�g
/*******************************************************/

AIState::Escape * AIState::Escape::GetInstance()
{
	// �����ɕϐ������
	static AIState::Escape instance;
	return &instance;
}

void AIState::Escape::Enter(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �K�[�h�{�ړ��Ł@��𔭓�������
	pPerson->PushInputList(PLAYER_INPUT::R1);
	// ����̏ꏊ�ɉ�����
	if (TargetPos.x < MyPos.x) pPerson->PushInputList(PLAYER_INPUT::LEFT);
	else pPerson->PushInputList(PLAYER_INPUT::RIGHT);


}

void AIState::Escape::Execute(AI * pPerson)
{
	// �G�X�P�[�v��܂��͑ҋ@�ɂȂ��Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true||
		MyPlayer->GetFSM()->isPrevState(*BasePlayerState::Escape::GetInstance()) == true)
	{
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
	
}

void AIState::Escape::Exit(AI * pPerson)
{
}

void AIState::Escape::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI���");
}

bool AIState::Escape::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//				�K�[�h�X�e�[�g
/*******************************************************/

AIState::Guard * AIState::Guard::GetInstance()
{
	// �����ɕϐ������
	static AIState::Guard instance;
	return &instance;
}

void AIState::Guard::Enter(AI * pPerson)
{
	// �K�[�h�{�^������
	pPerson->PushInputList(PLAYER_INPUT::R1);
}

void AIState::Guard::Execute(AI * pPerson)
{
	//�@�K�[�h�{�^��������������
	pPerson->PushInputList(PLAYER_INPUT::R1);
	
	// �K�[�h��܂��͑ҋ@�ɂȂ��Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true ||
		MyPlayer->GetFSM()->isPrevState(*BasePlayerState::Guard::GetInstance()) == true)
	{
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
	
	// ��x�̃K�[�h���ԁ@�ō��ł�8�t���[���͍d��

	// 8�t���[�����ɉ�����h��p�����𔻒f
	if (pPerson->GetGuardFrame() % 8 == 0)
	{
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		float len = MyPlayer->AIAtackRange();

		/***********************/
		// ���or�h��
		/***********************/
		// ���肪�U���X�e�[�g�Ȃ�
		if (pPerson->GetTargetPlayer()->isAttackState())
		{
			if (tdnRandom::Get(0, 1) == 0)
			{
				// ���܂ő��肪�߂��ɂ���
				if (Math::Length(TargetPos, MyPos) < len)
				{
					// ������Ă悯��
					pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
					return;
				}
			}

		}
	}


	// 40�t���[���܂ł����獡�x�͑��肪�U�����Ă��Ȃ�������K�[�h������
	if (pPerson->GetGuardFrame() > 40)
	{
		pPerson->SetGuardFrame(0);// ���Z�b�g

		// ���肪�U�����Ă��Ȃ�������
		if (pPerson->GetTargetPlayer()->isAttackState()==false)
		{
			// �K�[�h����
			pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
			return;

		}

	}

	pPerson->AddGuardFrame(1);//++


}

void AIState::Guard::Exit(AI * pPerson)
{
}

void AIState::Guard::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIGuard");
}

bool AIState::Guard::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//				�ǂ�������X�e�[�g
/*******************************************************/
//�������イ�ɂ�����󒆂��������ɂ����Ă���������
AIState::Chase * AIState::Chase::GetInstance()
{
	// �����ɕϐ������
	static AIState::Chase instance;
	return &instance;
}

void AIState::Chase::Enter(AI * pPerson)
{

	// �ǂ������^�C�}�[������
	pPerson->SetChaseFrame(0);

	//	�N�����������邩�l����
	if (SearchNearPlayer(pPerson) == false)
	{
		// �G���������Ȃ�����
		pPerson->GetFSM()->ChangeState(AIState::Space::GetInstance());
	}

	//Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	//Vector3 MyPos = MyPlayer->GetPos();

	//if (TargetPos.x > MyPos.x)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//}
	//else
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//}
}

void AIState::Chase::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	//// HIT���Ă�����
	//if (MyPlayer->GetAttackData()->bHit == false) // �����̃A�^�b�N�f�[�^�̃Q�b�^�[���킢
	//{
	//	return;
	//}

	// �������󒆂ɕ����Ă�����@�����Ă闝�R�[�����J�o���[�Ƃ�
	if (MyPlayer->isLand() == false)
	{
		pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
		return;
	}

	/***********************************************************/
	//pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
	//return;
	/***********************************************************/

	//�ӂ��ɂ����゠�[��AI�ł����낤���Ȃ�����
	// �����Ă邩�����Ă��邩�� ���iTODO�j�L�����N�^�[���Ƀ����W��ς���
	float len = 0;
	if (MyPlayer->GetFSM()->isInState
		(*BasePlayerState::Run::GetInstance()) == true)  len = MyPlayer->AIRunAtackRange();// ���Ő��l��������
	else len = MyPlayer->AIAtackRange();
	
	// ���܂ŋ߂Â�����
	if (Math::Length(TargetPos, MyPos) < len)
	{
		if (tdnRandom::Get(0, 2) == 0)// �Ȃ��̎��₯�ǂ��������₩�炱�̂܂܂�
		{
			// �O��󒆍U�������܂��Ă��Ȃ�������
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDropAtack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return;
			}
		}

		// �ǂ݂̉�� ���邫�ҋ@||�����Ŋm�����Ȃ�
		if (MyPlayer->GetFSM()->isInState
			(*BasePlayerState::Wait::GetInstance()) == true||
			MyPlayer->GetFSM()->isInState
			(*BasePlayerState::Walk::GetInstance()) == true)
		{
			// �A���Ŗh�䂷��̖h�~
			if (pPerson->GetFSM()->isPrevState(*AIState::Guard::GetInstance()) != true)
			{
				//4���̂P
				if (tdnRandom::Get(0, 3) == 3)
				{
					pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
					return;
				}
			}
		}


		// �n�ォ�󒆂�
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			// ���E�̐U�����
			if (TargetPos.x > MyPos.x)
			{
				// �������Ă��������ς���܂ŉE�{�^��������
				if (MyPlayer->GetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					// 3����1�̊m���ŃX�L���U��
					if (tdnRandom::Get(0, 2) == 0)
					{
						pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
						return;
					}
					else
					{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					}

				}

			}
			else
			{
				//�@�����l�Ɍ�����ς���
				if (MyPlayer->GetDir() == DIR::RIGHT)
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}		
				else
				{
					// 3����1�̊m���ŃX�L���U��
					if (tdnRandom::Get(0, 2) == 0)
					{
						pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
						return;
					}
					else
					{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					}
				}
			}
		}
		else
		{
			// ���i��Փx�ɑ����镨�j����҂̎��߂ɂ���Α΋󓖂Ă�}�����m����
			
			// 2����1�̊m���ő΋�U��
			if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return;
			}
			else if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
			else
			{
				pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
				return;
			}
		}
	}
	

	// �_�b�V������
	if (Math::Length(TargetPos, MyPos) > 50)
	{
		// �����Ă���ꍇ
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()) == true||
			MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// �����Ă͗����@���̓����ő���ɂ�����
			if (TargetPos.x > MyPos.x)
			{
				if (MyPlayer->GetInputList(PLAYER_INPUT::RIGHT) == 0)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
			}
			else 
			{
				if (MyPlayer->GetInputList(PLAYER_INPUT::LEFT) == 0)
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}				
			}

		}
		else if(MyPlayer->GetFSM()->isInState(*BasePlayerState::Run::GetInstance()) == true)
		{
			// ����X�e�[�g�ɂȂ��Ă���̂Ō�͂����ǂ����
			// ���E�̈ړ�
			if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

		}
		//MyPlayer->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	
	}
	else 
	{
		// ���E�̈ړ�
		if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	}

		
	/********************/
	//	�S���h�~
	/********************/
	pPerson->AddChaseFrame(1);			// �ǂ������^�C�}�[�X�V
	if (pPerson->GetChaseFrame() >= 60)	// 60�b�ԔS�����Ēǂ�������
	{
		pPerson->SetChaseFrame(0); // ������
	
		// ���������߂���T��
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}



}

void AIState::Chase::Exit(AI * pPerson)
{
}

void AIState::Chase::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�`�F�C�X");
	tdnText::Draw(420, 660, 0xffffffff, "AI�ǂ������Ă�v���C���[No%d",pPerson->GetTargetPlayer()->GetDeviceID());
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
//				�W�����v���ċ󒆂Œǂ�������X�e�[�g
/*******************************************************/

// (TODO) ���O�����������@�`�F�C�X������ȁ@�����W�����v���Ă邾�� �G�A�[����Ȃ��ăW�����v�H

AIState::ChaseAir * AIState::ChaseAir::GetInstance()
{
	// �����ɕϐ������
	static AIState::ChaseAir instance;
	return &instance;
}

void AIState::ChaseAir::Enter(AI * pPerson)
{
	// �W�����v
	pPerson->PushInputList(PLAYER_INPUT::C);

}

void AIState::ChaseAir::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ���E�̈ړ�
	if (TargetPos.x > MyPos.x + 13)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	else if (TargetPos.x < MyPos.x - 13)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	// �����ł̔�т����h�~�i�Î~������j
	else if (MyPlayer->GetMove().x < -0.3f)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else if (MyPlayer->GetMove().x > 0.3f)
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}

	// ���܂ŋ߂Â�����
	if (Math::Length(TargetPos, MyPos) < 32)
	{
		// �������n�߂��u�Ԃ���
		if (MyPlayer->GetMove().y < -0.0f)
		{
			if (pPerson->GetTargetPlayer()->isLand() == false)
			{
				// ��������󒆂ɂ���Ȃ�󒆃R���{�Ɏ����Ă���
				pPerson->GetFSM()->ChangeState(AIState::AerialAtack::GetInstance());
				return;
			}
		}

		// �����������n�߂���
		if (MyPlayer->GetMove().y < -0.3f)
		{
			// ���i���j�Ńt�B�j�b�V���U��������Ă݂�
			if (MyPlayer->GetCollectScore() >= 10000)
			{
				// �t�B�j�b�V���U��
				pPerson->GetFSM()->ChangeState(AIState::FinishAtack::GetInstance());
				return;
			}
			else
			{
				// �󒆉��U���Ő���
				pPerson->GetFSM()->ChangeState(AIState::AerialDropAtack::GetInstance());
				return;
			}

			
		}
	}
	
	// �W�����v�X�e�[�g��������
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == true)
	{
		// �W�����v�̓��ݍ��݂̓J�E���g���Ȃ�
		if (MyPlayer->GetJump()->bHold == false)
		{
			// ���n������
			if (MyPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
				return;
			}
		}
	}
	else// �����X�e�[�g�Ƃ��H
	{			// ���n������
		if (MyPlayer->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}

	

}

void AIState::ChaseAir::Exit(AI * pPerson)
{
}

void AIState::ChaseAir::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI�󒆃`�F�C�X");
}

bool AIState::ChaseAir::OnMessage(AI * pPerson, const Message & msg)
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

	// �����L �Ԋu���󂯂��������̂ō��͂����
	static int Flame = 0;
	Flame++;
	//if (Flame > 5)
	{
		Flame = 0;
		if (MyPlayer->GetDir()== DIR::RIGHT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
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
	// ���Ⴊ�݉���
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
		abs(TargetPos.y - MyPos.y) < 32
		)
	{
		// ����炪���Ⴊ�݃��[�h�ɂȂ��Ă���
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()) == true)
		{
			// �΋󔭓��X�e�[�g�ցI
			pPerson->GetFSM()->ChangeState(AIState::AnitiAir::GetInstance());
			return;

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
	
	// ���Ⴊ�ݍU�����I����Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()) == false)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// ���Ⴊ�ݍU����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()) == true)
	{
		if (!MyPlayer->isAttackState())return;
		// HIT���Ă�����L�����Z�����Ĕ�ׁI�I
		if (MyPlayer->GetAttackData()->bHit == true) // �����̃A�^�b�N�f�[�^�̃Q�b�^�[���킢
		{
			if (pPerson->GetTargetPlayer()->isLand()==true)
			{
				// ���肪�����ĂȂ�������
				//pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
				//return;
			}
			else
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());
				return;
			}

		}
	}


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


/*******************************************************/
//			�󒆍U�������@�X�e�[�g
/*******************************************************/

AIState::SetAerialAtack * AIState::SetAerialAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::SetAerialAtack  instance;
	return &instance;
}

void AIState::SetAerialAtack::Enter(AI * pPerson)
{
	// �W�����v����
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::SetAerialAtack::Execute(AI * pPerson)
{
	// ����͒n��ɒ��n���Ă��܂�����
	if (pPerson->GetTargetPlayer()->isLand())
	{
		// �΋�U���ł��Ȃ��̂Ŗ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ���E�̈ړ�
	if (TargetPos.x > MyPos.x+5)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);	
	else if (TargetPos.x < MyPos.x-5)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	// �΋�͈͓�
	if ( Math::Length(TargetPos, MyPos) < 18 )
	{
		// �����������Ă�Ƃ�
		if (MyPlayer->GetMove().y < -0.0f)
		{
			// �W�����v�E�G���A���W�����v���[�h�ɂȂ��Ă���
			if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == true ||
				MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()) == true)
			{

				// �󒆍U�������X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(AIState::AerialAtack::GetInstance());
				return;

			}
		}
	}

}

void AIState::SetAerialAtack::Exit(AI * pPerson)
{

}

void AIState::SetAerialAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SetAerialAtack");
}

bool AIState::SetAerialAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}

/*******************************************************/
//			�󒆍U�� �X�e�[�g
/*******************************************************/

AIState::AerialAtack * AIState::AerialAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::AerialAtack  instance;
	return &instance;
}

void AIState::AerialAtack::Enter(AI * pPerson)
{
	{
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		// ���E�̓���(�����]��)
		if (TargetPos.x > MyPos.x)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else						pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}

	// �����W�����v�L�����Z���ł��Ȃ�������Y�Z
	if (MyPlayer->isAerialJump() == true)
	{
		// �U���{�^������
		pPerson->PushInputList(PLAYER_INPUT::B);

	}
	else
	{
		//
		// ���͂ȍU���{�^������
		MyPlayer->AIHighAtackButton();

	}
}

void AIState::AerialAtack::Execute(AI * pPerson)
{
	// �����U���h�~
	//if (MyPlayer->isPushInput(PLAYER_INPUT::DOWN) == true)return;

	// �n��ɒ��n���Ă��܂�����
	if (MyPlayer->isLand())
	{
		// �߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// �G���A���A�^�b�N�X�e�[�g�ֈڍs���Ă��Ȃ�������҂�
	if (MyPlayer->GetFSM()->isPrevState(*BasePlayerState::AerialAttack::GetInstance()) == false)
	{
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) == false)
			return;
	}

	// �󒆍U�����I����Ă�����
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) == false)
	//{
	//	// �ǐՃ��[�h�ɖ߂�
	//	pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	//	return;
	//}


//	if (pPerson->GetTargetPlayer()->GetMove().y < 2.0f)
	{
		// HIT����Ζ߂�
		if (!MyPlayer->isAttackState())return;
		if (MyPlayer->GetAttackData()->bHit == true) // �����̃A�^�b�N�f�[�^�̃Q�b�^�[���킢
		{
			// ��i�W�����v���ł���Ȃ������x�G���A����
			if (MyPlayer->isAerialJump() == true)
			{

				// �W�����v���[�h�ɖ߂�
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());
				return;
			}
			else
			{
				// �ǐՃ��[�h�ɖ߂�
				pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
				return;
			}

		}
	}

	if (MyPlayer->GetActionFrame()==FRAME_STATE::END)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}


}

void AIState::AerialAtack::Exit(AI * pPerson)
{

}

void AIState::AerialAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialAtack");
}

bool AIState::AerialAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}

/*******************************************************/
//			�󒆉��U�� �X�e�[�g
/*******************************************************/

AIState::AerialDropAtack * AIState::AerialDropAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::AerialDropAtack  instance;
	return &instance;
}

void AIState::AerialDropAtack::Enter(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();
	// ���E�̓���(�����]��)
	if (TargetPos.x > MyPos.x )	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	else						pPerson->PushInputList(PLAYER_INPUT::LEFT);

	// �󒆉��U��
	pPerson->PushInputList(PLAYER_INPUT::DOWN);
	pPerson->PushInputList(PLAYER_INPUT::B);
	
}

void AIState::AerialDropAtack::Execute(AI * pPerson)
{

	// �n��ɒ��n���Ă��܂�����
	if (MyPlayer->isLand() == true)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// �G���A���A�^�b�N�X�e�[�g�ֈڍs���Ă��Ȃ�������҂�(�󉺕��󂵂��̂ŃR�����g�A�E�g���܂����I)
	//if (MyPlayer->GetFSM()->isPrevState(*BasePlayerState::AerialDropAttack::GetInstance()) == false)
	//{
	//	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialDropAttack::GetInstance()) == false)
	//		return;
	//}


	// HIT����Ζ߂�
	if (!MyPlayer->isAttackState())return;
	if (MyPlayer->GetAttackData()->bHit == true) // �����̃A�^�b�N�f�[�^�̃Q�b�^�[���킢
	{
		// �ǐՃ��[�h�ɖ߂�
		//pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		//return;
	}
	



}

void AIState::AerialDropAtack::Exit(AI * pPerson)
{

}

void AIState::AerialDropAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialDropAtack");
}

bool AIState::AerialDropAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			�t�B�j�b�V���U�� �X�e�[�g
/*******************************************************/

AIState::FinishAtack * AIState::FinishAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::FinishAtack  instance;
	return &instance;
}

void AIState::FinishAtack::Enter(AI * pPerson)
{
	// �t�B�j�b�V���{�^��
	pPerson->PushInputList(PLAYER_INPUT::R2);

}

void AIState::FinishAtack::Execute(AI * pPerson)
{
	// FinishAttack�X�e�[�g���I����Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == false)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// FinishAttack��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == true)
	{
		if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
		{
			// �ǐՃ��[�h�ɖ߂�
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}
}

void AIState::FinishAtack::Exit(AI * pPerson)
{

}

void AIState::FinishAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "FinishAtack");
}

bool AIState::FinishAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}

/*******************************************************/
//			�X�L���U�� �X�e�[�g
/*******************************************************/

AIState::SkillAtack * AIState::SkillAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::SkillAtack  instance;
	return &instance;
}

void AIState::SkillAtack::Enter(AI * pPerson)
{
	// �L�����N�^�[�ŗL�̍U���n�X�L�������X�e�[�g
	MyPlayer->AIHighAtackButton();
}

void AIState::SkillAtack::Execute(AI * pPerson)
{
	// �X�L���X�e�[�g���I����Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) == false)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// �X�L����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) == true)
	{

		// HIT����Ζ߂�
		if (!MyPlayer->isAttackState())return;// �U���n�̃X�L���Ȃ�
		if (MyPlayer->GetAttackData()->bHit == true) // �����̃A�^�b�N�f�[�^�̃Q�b�^�[���킢
		{
			//// ��i�W�����v���ł���Ȃ������x�G���A����
			//if (MyPlayer->isAerialJump() == true)
			//{
			//	// �W�����v���[�h�ɖ߂�
			//	pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());
			//	return;
			//}
		}


		if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
		{
			// �ǐՃ��[�h�ɖ߂�
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}

}

void AIState::SkillAtack::Exit(AI * pPerson)
{

}

void AIState::SkillAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SkillAtack");
}

bool AIState::SkillAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}
