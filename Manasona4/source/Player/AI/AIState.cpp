#include "AI.h"
#include "AIState.h"
#include "../../Player/BasePlayer.h"
#include "../../Player/PlayerManager.h"
#include "../BasePlayerState.h"

#define MyPlayer (pPerson->GetBasePlayer())
#define TargetPlayer (pPerson->GetTargetPlayer())

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
		//pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
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
		//pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
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
	// �_���[�W��H�������
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// ���A���[�h��
		pPerson->GetFSM()->ChangeState(AIState::Recovery::GetInstance());
	}

	// �����ɕ߂܂�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true)
	{
		// �����������f�X�e�[�g��
		pPerson->GetFSM()->ChangeState(AIState::ThrowTech::GetInstance());
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
	// �������U�����󂯂ē|���Œ��Ȃ��
	if (
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// ���A�{�^��
		pPerson->PushInputList(PLAYER_INPUT::B);
	}

	// �n��ɕt������W�����v����
	if (MyPlayer->isLand())
	{
		// �W�����v
		pPerson->PushInputList(PLAYER_INPUT::UP);
	}

}

void AIState::PracticeJump::Exit(AI * pPerson)
{
}

void AIState::PracticeJump::Render(AI * pPerson)
{

#ifdef _DEBUG

	tdnText::Draw(1000, 580, 0xffffffff, "AI�W�����v");

#endif // _DEBUG

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
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);

}

void AIState::PracticeLand::Execute(AI * pPerson)
{
	
	// �������U�����󂯂ē|���Œ��Ȃ��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// ���A�{�^��
		pPerson->PushInputList(PLAYER_INPUT::B);
	}

	// �|��Ȃ��m�b�N�o�b�N�Z���󂯂��Ȃ�
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true)
	{
		pPerson->m_bPracticeGuardFlag = true;
		pPerson->m_iPracticeGuardFrame = 0;
	}
	
	// ���K�p�K�[�h�t���O�������Ă���@
	if (pPerson->m_bPracticeGuardFlag == true /*&& MyPlayer->GetRecoveryFrame() <= 0�@�K�[�h�d�����܂ނ̂łȂ�*/)
	{
		pPerson->m_iPracticeGuardFrame++;	
		
		// �K�[�h������K�[�h��������t���[����������i���肪���������܂ŃK�[�h�j
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == true)
		{
			// ����̍U�������t���[�����̂Ƃ����Z�b�g
			// if (TargetPlayer->isActiveFrame() == true)
			pPerson->m_iPracticeGuardFrame = 10;
		}

		// ���t���[����葱����
		if (pPerson->m_iPracticeGuardFrame >= 20) // ���t���[�����������Ȃ����ƃK�[�h����
		{
			// ���K�K�[�h�t���O�Ɨ��K�K�[�h�t���[����������
			pPerson->m_bPracticeGuardFlag = false;
			pPerson->m_iPracticeGuardFrame = 0;
		}

		// ���肪�U���U������
		if (TargetPlayer->isAttackState() == true)
		{

			// ����Ƌt�������o�[������
			if (MyPlayer->GetDir() == DIR::LEFT)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}

			// ���i�Ȃ�
			if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
			{
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
			}

		}

	}


}

void AIState::PracticeLand::Exit(AI * pPerson)
{
}

void AIState::PracticeLand::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI�n��");

#endif // _DEBUG

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
//				���K�p�U���X�e�[�g
/*******************************************************/

AIState::PracticeAttack* AIState::PracticeAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeAttack instance;
	return &instance;
}

void AIState::PracticeAttack::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);
	pPerson->m_iPracticeAttackIntervalFrame = 0;

}

void AIState::PracticeAttack::Execute(AI * pPerson)
{
	// ��莞�Ԃ���������U��
	pPerson->m_iPracticeAttackIntervalFrame++;
	if (pPerson->m_iPracticeAttackIntervalFrame >= 60)
	{
		pPerson->m_iPracticeAttackIntervalFrame = 0;
		// �U��
		pPerson->PushInputList(PLAYER_INPUT::C);

	}
	

	// �������U�����󂯂ē|���Œ��Ȃ��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// ���A�{�^��
		pPerson->PushInputList(PLAYER_INPUT::B);
	}

	//// �|��Ȃ��m�b�N�o�b�N�Z���󂯂��Ȃ�
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true)
	//{
	//	pPerson->m_bPracticeGuardFlag = true;
	//	pPerson->m_iPracticeGuardFrame = 0;
	//}

	//// ���K�p�K�[�h�t���O�������Ă���@
	//if (pPerson->m_bPracticeGuardFlag == true /*&& MyPlayer->GetRecoveryFrame() <= 0�@�K�[�h�d�����܂ނ̂łȂ�*/)
	//{
	//	pPerson->m_iPracticeGuardFrame++;

	//	// �K�[�h������K�[�h��������t���[����������i���肪���������܂ŃK�[�h�j
	//	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == true)
	//	{
	//		// ����̍U�������t���[�����̂Ƃ����Z�b�g
	//		// if (TargetPlayer->isActiveFrame() == true)
	//		pPerson->m_iPracticeGuardFrame = 10;
	//	}

	//	// ���t���[����葱����
	//	if (pPerson->m_iPracticeGuardFrame >= 20) // 40�t���[�����������Ȃ����ƃK�[�h����
	//	{
	//		// ���K�K�[�h�t���O�Ɨ��K�K�[�h�t���[����������
	//		pPerson->m_bPracticeGuardFlag = false;
	//		pPerson->m_iPracticeGuardFrame = 0;
	//	}

	//	// ���肪�U���U������
	//	if (TargetPlayer->isAttackState() == true)
	//	{

	//		// ����Ƌt�������o�[������
	//		if (MyPlayer->GetDir() == DIR::LEFT)
	//		{
	//			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//		}
	//		else
	//		{
	//			pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//		}

	//		// ���i�Ȃ�
	//		if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
	//		{
	//			pPerson->PushInputList(PLAYER_INPUT::DOWN);
	//		}

	//	}

	//}

}

void AIState::PracticeAttack::Exit(AI * pPerson)
{
}

void AIState::PracticeAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI�U�����K");

#endif // _DEBUG

}

bool AIState::PracticeAttack::OnMessage(AI * pPerson, const Message & msg)
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
	
	// �ҋ@���[�V�����ɂȂ�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// �ǐՃ��[�h��
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
}

void AIState::Wait::Exit(AI * pPerson)
{
}

void AIState::Wait::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI��������");

#endif // _DEBUG

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
	BasePlayer* b = pPerson->GetTargetPlayer();
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
	pPerson->PushInputList(PLAYER_INPUT::UP);
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
	
	// ��𔭓�������
	pPerson->PushInputList(PLAYER_INPUT::B);

}

void AIState::Escape::Execute(AI * pPerson)
{

	// �G�X�P�[�v��܂��͑ҋ@�ɂȂ��Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true||
		MyPlayer->GetFSM()->isPrevState(*BasePlayerState::Escape::GetInstance()) == true)
	{
		// �ҋ@�֖߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
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
//				�K�[�h�X�e�[�g	(1211) ����
/*******************************************************/

AIState::Guard * AIState::Guard::GetInstance()
{
	// �����ɕϐ������
	static AIState::Guard instance;
	return &instance;
}

void AIState::Guard::Enter(AI * pPerson)
{

	pPerson->m_iGuardFrame = 0;

}

void AIState::Guard::Execute(AI * pPerson)
{

	// �K�[�h�t���[���X�V
	pPerson->m_iGuardFrame++;

	// �K�[�h������K�[�h��������t���[����������i���肪���������܂ŃK�[�h�j
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == true)
	{
		// ����̍U�������t���[�����̂Ƃ����Z�b�g
		pPerson->m_iGuardFrame = 10;
	}

	// ���t���[����葱����
	if (pPerson->m_iGuardFrame >= 20) // ���t���[�����������Ȃ����ƃK�[�h����
	{
		pPerson->m_iGuardFrame = 0;

		// AI�ҋ@�X�e�[�g��
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());

		//�iTODO�j�؂�ւ����Z


	}
	
	//	�iTODO�j�t�H���[�̒l���~����
	// ���肪�U�����Ă��Ȃ�������
	if (pPerson->GetTargetPlayer()->isAttackState() == false)
	{
		// ����菭�Ȃ����t���[���ŏI��
		if (pPerson->m_iGuardFrame >= 10)
		{
			pPerson->m_iGuardFrame = 0;
			// AI�ҋ@�X�e�[�g��
			pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());

			//�iTODO�j�؂�ւ����Z


		}

	}


	//+-----------------------------------------
	//		�K�[�h�̏���
	//+-----------------------------------------

	//if(���[���[��������)
	//{
	//}else
	//{

	// ���肪�U���U������
	if (TargetPlayer->isAttackState() == true)
	{

		// ����Ƌt�������o�[������
		if (MyPlayer->GetDir() == DIR::LEFT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}

		// ���i�Ȃ�
		if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
		{
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
		}

	}


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
//				�ǂ�������X�e�[�g -> �ߋ����h��
/*******************************************************/

AIState::Chase * AIState::Chase::GetInstance()
{
	// �����ɕϐ������
	static AIState::Chase instance;
	return &instance;
}

void AIState::Chase::Enter(AI * pPerson)
{

	// �ǂ������^�C�}�[������
	pPerson->m_iChaseFrame = 0;

}

void AIState::Chase::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �������󒆂ɕ����Ă�����@�󒆒ǂ������X�e�[�g��
	if (MyPlayer->isLand() == false)
	{
		pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
		return;
	}

	/***********************************************************/
	//pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
	//return;
	/***********************************************************/

	// �����Ă邩�����Ă��邩�� ���iTODO�j�L�����N�^�[���Ƀ����W��ς���
	float len = 0;
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Run::GetInstance()) == true)
		len = MyPlayer->AIRunAtackRange();	// �����̃L�����N�^�[�̃����W
	else len = MyPlayer->AIAtackRange();	// �����̃L�����N�^�[�̃����W
	
	//+-------------------------------------------------
	//	���̋����ň��܂ŋ߂Â�����@(�����_��)�ōU�� (1210) x�̋����ɕύX �W�����v���ĕ�������_�b�V���ɂȂ�̂͗���
	//+-------------------------------------------------
	if (Math::Length(TargetPos.x, MyPos.x) < len &&
		TargetPlayer->GetInvincibleTime() <= 0)
	{
		// 0%~100%�̊Ԃ̎�����܂�
		//int iRandom = tdnRandom::Get(0, 100);
		
		// �����X�e�[�g
		if (tdnRandom::Get(0, 100) <= 105)// ���̊m���Ŕ��
		{
			// �����肪�_�E�����Ă��Ȃ��Ƃ����A���ł��Ȃ��Ƃ�
			if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
				return;
			}
			return;
		}

		// ���G�Z�X�e�[�g
		if (tdnRandom::Get(0, 100) <= 15)// ���̊m���Ŕ��
		{
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAtack::GetInstance());
			return;
		}

		// ����X�e�[�g
		if (tdnRandom::Get(0, 100) <= 15)// ���̊m���Ŕ��
		{
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return;
		}


		// �K�[�h�X�e�[�g
		if (tdnRandom::Get(0, 100) <= 15)// ���̊m���Ŕ��
		{
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return;
		}	

		if (tdnRandom::Get(0, 100) <=	15)// ���̊m���Ŕ��
		{
			// �O��󒆕����U�������܂��Ă��Ȃ�������
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAtack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)//	���̊m���Ŕ��
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return;
		}

		// ���傤�͂����܂�
		//	�������͂��[�ǂƂ�������ւ�Ƃ͂�����ςȂ��Ȃǂ̎�̉�

		//// �ǂ݂̉�� ���邫�ҋ@ �����Ŋm�����Ȃ�
		//if (MyPlayer->GetFSM()->isInState
		//	(*BasePlayerState::Wait::GetInstance()) == true||
		//	MyPlayer->GetFSM()->isInState
		//	(*BasePlayerState::Walk::GetInstance()) == true)
		//{
		//	// �A���Ŗh�䂷��̖h�~
		//	if (pPerson->GetFSM()->isPrevState(*AIState::Guard::GetInstance()) != true)
		//	{
		//		//4���̂P
		//		if (tdnRandom::Get(0, 3) == 3)
		//		{
		//			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
		//			return;
		//		}
		//	}
		//}


		// ���肪�n��E�󒆂ɂ��邩�œ�����ς���
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			// ���E�̐U�����
			if (TargetPos.x > MyPos.x)
			{
				//// �������Ă��������ς���܂ŉE�{�^��������
				//if (MyPlayer->GetDir() == DIR::LEFT)
				//{
				//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				//}
				//else
				//{
					// 3����1�̊m���ŃX�L���U��
					//if (tdnRandom::Get(0, 2) == 0)
					//{
					//	pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
					//	return;
					//}
					//else
					//{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					//}

				//}

			}
			else
			{
				////�@�����l�Ɍ�����ς���
				//if (MyPlayer->GetDir() == DIR::RIGHT)
				//{
				//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
				//}		
				//else
				//{
					// 3����1�̊m���ŃX�L���U��
					//if (tdnRandom::Get(0, 2) == 0)
					//{
					//	pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
					//	return;
					//}
					//else
					//{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					//}
				//}
			}
		}
		else
		{
			// ���肪�󒆂ɂ���ꍇ�͑����Ă��悤�������Ăł�������ꏊ�ɍs��
			//float  nearLen = MyPlayer->AIAtackRange();	// �����̃L�����N�^�[�̃����W


			// ���i��Փx�ɑ����镨�j����҂̎��߂ɂ���Α΋󓖂Ă�}�����m����
			
			// 2����1�̊m���ő΋�U��
			//if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return;
			}
			//else
			//{
			//	pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
			//	return;
			//}

		}
	}
	

	//+-----------------------------------------
	// �_�b�V������@(12/10) �ړ�����
	//+-----------------------------------------	
	if (Math::Length(TargetPos, MyPos) >= 25)		// ���̃p�����[�^�[������ɐ؂�ւ��鋗��
	{
		// �����Ă���ꍇ
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()) == true||
			MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// �����Ă͗����@���̓����ő���ɂ�����
			if (TargetPos.x > MyPos.x)
			{
				// ����񉟂� �_�b�V���̂��߂�
				pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
			}else 
			{
				// ����񉟂� �_�b�V���̂��߂�
				pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
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
	else // �߂���������|��
	{
		// ���E�̈ړ�
		if (TargetPos.x > MyPos.x + 7)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else if (TargetPos.x < MyPos.x - 7)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
		else// ������̂ɂ��肬��܂Ŕ������Ƃ��̏���
		{

		}

	}

		
	/****************************/
	//	�~�S���h�~�@���V�����s��
	/****************************/
	pPerson->m_iChaseFrame++;			// �ǂ������^�C�}�[�X�V
	if (pPerson->m_iChaseFrame >= 60)	// 60�b�ԔS�����Ēǂ�������
	{
		pPerson->m_iChaseFrame = 0; // ������
	
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
	tdnText::Draw(420, 700, 0xffffffff, "����%.2f", Math::Length(TargetPlayer->GetPos(), MyPlayer->GetPos()));


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


AIState::ChaseAir * AIState::ChaseAir::GetInstance()
{
	// �����ɕϐ������
	static AIState::ChaseAir instance;
	return &instance;
}

void AIState::ChaseAir::Enter(AI * pPerson)
{
	// �W�����v
	pPerson->PushInputList(PLAYER_INPUT::UP);

}

void AIState::ChaseAir::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ���E�̈ړ�
	//if (TargetPos.x > MyPos.x + 13)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//else if (TargetPos.x < MyPos.x - 13)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//// �����ł̔�т����h�~�i�Î~������j
	//else if (MyPlayer->GetMove().x < -0.3f)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//}
	//else if (MyPlayer->GetMove().x > 0.3f)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//}

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
			// �󒆉��U���Ő���
			pPerson->GetFSM()->ChangeState(AIState::AerialDestructAtack::GetInstance());
			return;
		}

	}
	
	//+-----------------------------------------------------
	//	���肪�����ɂ��čU���ł������n���Ă��܂������̏���
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
	// �U���J�n�A�ŏ��̈�i�ڂ�U��ŁI�I�I
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::RushAtack::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �v���C���[�̃��b�V���A�^�b�N�X�e�[�g�I�����Ă���
	// [1210] GetAtactData�ŃA�T�[�g���N����̂Œǉ��@��Ńo�[�X�g���Ă݂邩
	if (!MyPlayer->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) ||
		!MyPlayer->isAttackState())/*!MyPlayer->isAttackState()*/
	{
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// �U���I�I

	// ��i�ڂ��q�b�g��������
	if (MyPlayer->GetRushStep() == 0 && 
		MyPlayer->GetAttackData()->bHit == true	)
	{
		// ��i�ڍU���I
		pPerson->PushInputList(PLAYER_INPUT::C);
	}

	// ��i�ڂ��q�b�g��������
	if (MyPlayer->GetRushStep() == 1 &&
		MyPlayer->GetAttackData()->bHit == true)
	{	
		// (��Փx��)��������ɃK�[�h����Ă������
		if (TargetPlayer->GetGuardState() == GUARD_STATE::DOWN_GUARD)
		{
			// ���i
			pPerson->PushInputList(PLAYER_INPUT::R1);

		}
		else if (TargetPlayer->GetGuardState() == GUARD_STATE::UP_GUARD)
		{
			// ������
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
			pPerson->PushInputList(PLAYER_INPUT::R1);
		}
		else
		{
			// �Ō�O�i�ڍU���I
			pPerson->PushInputList(PLAYER_INPUT::C);
		}

	}



	// �O�i�ڂ��q�b�g����||�Q�[�W50%�ȏ㗭�܂��Ă���
	if (MyPlayer->GetRushStep() == 2 &&
		MyPlayer->GetAttackData()->bHit == true &&
		MyPlayer->GetOverDriveGage() >= 50.0f)
	{
		// �K�E�Z�����I�I
		pPerson->PushInputList(PLAYER_INPUT::C);
	}

	// �����L �Ԋu���󂯂��������̂ō��͂����
	//static int Flame = 0;
	//Flame++;
	////if (Flame > 5)
	//{
	//	Flame = 0;
	//	if (MyPlayer->GetDir()== DIR::RIGHT)
	//	{
	//		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//	}
	//	else
	//	{
	//		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//	}
	//	pPerson->PushInputList(PLAYER_INPUT::B);

	//}

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
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == false)
	{
			// �E�F�C�g�ɖ߂�
			pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());		
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
//			�͂܂ꂽ�i���������j�X�e�[�g	��
/*******************************************************/

AIState::ThrowTech * AIState::ThrowTech::GetInstance()
{
	// �����ɕϐ������
	static AIState::ThrowTech instance;
	return &instance;
}

void AIState::ThrowTech::Enter(AI * pPerson)
{
	// ��������
	pPerson->PushInputList(PLAYER_INPUT::A);
}

void AIState::ThrowTech::Execute(AI * pPerson)
{
	// �ҋ@�X�e�[�g�ɖ߂�����AI�̃X�e�[�g���ҋ@��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// �E�F�C�g�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	}
}

void AIState::ThrowTech::Exit(AI * pPerson)
{
}

void AIState::ThrowTech::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "�����ʂ��^�C��");
}

bool AIState::ThrowTech::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}

/*******************************************************/
//			���������X�e�[�g	��
/*******************************************************/

AIState::SetThrow * AIState::SetThrow::GetInstance()
{
	// �����ɕϐ������
	static AIState::SetThrow instance;
	return &instance;
}

void AIState::SetThrow::Enter(AI * pPerson)
{
	// ����̕����ֈړ�����
	if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
}

void AIState::SetThrow::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �΋�͈͓� (�����̍U���͈͓�)
	enum { ABJUST = 3 };
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIAtackRange() / ABJUST))
	{
		// ���������X�e�[�g�ցI
		pPerson->GetFSM()->ChangeState(AIState::Throw::GetInstance());
		return;
	}else
	{
		// ����̕����ֈړ�����
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}

	}
}

void AIState::SetThrow::Exit(AI * pPerson)
{
}

void AIState::SetThrow::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "��������");
}

bool AIState::SetThrow::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			�����X�e�[�g	��
/*******************************************************/

AIState::Throw * AIState::Throw::GetInstance()
{
	// �����ɕϐ������
	static AIState::Throw instance;
	return &instance;
}

void AIState::Throw::Enter(AI * pPerson)
{
	// ����
	pPerson->PushInputList(PLAYER_INPUT::A);
}

void AIState::Throw::Execute(AI * pPerson)
{
	// �ҋ@�X�e�[�g�ɖ߂�����AI�̃X�e�[�g���ҋ@��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// �E�F�C�g�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	}
}

void AIState::Throw::Exit(AI * pPerson)
{
}

void AIState::Throw::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "����");
}

bool AIState::Throw::OnMessage(AI * pPerson, const Message & msg)
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
	//// ���Ⴊ�݉���
	//pPerson->PushInputList(PLAYER_INPUT::DOWN);

}

void AIState::SetAnitiAir::Execute(AI * pPerson)
{
	
	// ���肪�n��ɗ����Ă��炵�Ⴊ�݂���߂锻��
	//if (pPerson->GetTargetPlayer()->isLand())
	//{
	//	// ����͒n��ɂ���̂ő΋����߂�
	//	pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	//	return;
	//}


	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �΋�͈͓� (�����̍U���͈͓�)
	enum { ABJUST = 2 };
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIAtackRange() - ABJUST))
	{
		// ���Ⴊ�݉������ςȂ� +
		// ����̋t�������o�[
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		pPerson->PushInputList(PLAYER_INPUT::DOWN);

		// AI�΋󔭓��X�e�[�g�ցI
		pPerson->GetFSM()->ChangeState(AIState::AnitiAir::GetInstance());
		return;

	}
	else
	{
		// ����̕����ֈړ�����
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
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
	// ����̋t�������o�[
	if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::AnitiAir ::Execute(AI * pPerson)
{
	
	// (�͂���Ă�����?)
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// ������Ԃ֖߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


	// �΋�U����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()) == true)
	{
		if (!MyPlayer->isAttackState())return;
		// HIT���Ă�����L�����Z�����Ĕ�ׁI�I
		if (MyPlayer->GetAttackData()->bHit == true) // �����̃A�^�b�N�f�[�^�̃Q�b�^�[���킢
		{
			if (pPerson->GetTargetPlayer()->isLand()==true)
			{
				// ���肪�����ĂȂ�������
				pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
				return;
			}
			else
			{
				// AI�G���A���R���{�U��������
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
	// ����Ƃ̋���������Ă���������L�[�����
	float l_fXLength = abs(MyPlayer->GetPos().x - TargetPlayer->GetPos().x);
	if (l_fXLength >= 7) 
	{
		// ����̕����֔��
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
	}

	// (1210) �W�����v�͉������ςȂ��Ŕ������邩���񉟂��Ă��Ӗ����Ȃ������̂�
	// �����ŏ����Ă��Ȃ�
	// �W�����v����
	//pPerson->PushInputList(PLAYER_INPUT::UP);
}

void AIState::SetAerialAtack::Execute(AI * pPerson)
{
	// �W�����v�ɍs���܂ŉ������ςȂ�
	
	// �A�^�b�N�X�e�[�g�Ȃ�
	if (MyPlayer->isAttackState())
	{
		// HIT���Ă�����L�����Z�����Ĕ�ׁI�I
		if (MyPlayer->GetAttackData()->bHit == true)
		{
			if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()) == true ||
				MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) == true)
			{
				// ����̕����֔��
				if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				else 
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);

				pPerson->PushInputList(PLAYER_INPUT::UP);
			}
		}
	}


	

	// ����͒n��ɒ��n���Ă��܂�����
	if (pPerson->GetTargetPlayer()->isLand())
	{
		// �΋�U���ł��Ȃ��̂Ŗ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ���E�̈ړ�
	//if (TargetPos.x > MyPos.x+5)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);	
	//else if (TargetPos.x < MyPos.x-5)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

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
		//Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		//Vector3 MyPos = MyPlayer->GetPos();
		//// ���E�̓���(�����]��)
		//if (TargetPos.x > MyPos.x)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		//else						pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}
	// �󒆍U�������I
	// �����W�����v�L�����Z���ł��Ȃ�������Y�Z
	if (MyPlayer->isAerialJump() == true)
	{
		// �U���{�^������
		pPerson->PushInputList(PLAYER_INPUT::C);
	}
	else
	{
		// ���͂ȍU���{�^������
		//MyPlayer->AIHighAtackButton();
		pPerson->PushInputList(PLAYER_INPUT::C);
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
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
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
				//pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());

				// �ҋ@�ɖ߂�
				pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
				return;
			}

		}
	}

	if (MyPlayer->GetActionFrame()==FRAME_STATE::END)
	{
		// �ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
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
//		�� �󒆕����U�� �X�e�[�g�@HIT ->�`�F�C�X -> �n��U����
/*******************************************************/

AIState::AerialDestructAtack * AIState::AerialDestructAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::AerialDestructAtack  instance;
	return &instance;
}

void AIState::AerialDestructAtack::Enter(AI * pPerson)
{

	// �󒆍U��
	pPerson->PushInputList(PLAYER_INPUT::C);
	
}

void AIState::AerialDestructAtack::Execute(AI * pPerson)
{


	// �n��ɒ��n������
	if (MyPlayer->isLand() == true)
	{
		// �����Ă���̂�����ǐՃ��[�h�ɍs�������Ȃ��I
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
}

void AIState::AerialDestructAtack::Exit(AI * pPerson)
{

}

void AIState::AerialDestructAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialDestructAtack");
}

bool AIState::AerialDestructAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			���G�U�� �X�e�[�g	(1211) ��
/*******************************************************/

AIState::InvincibleAtack * AIState::InvincibleAtack::GetInstance()
{
	// �����ɕϐ������
	static AIState::InvincibleAtack  instance;
	return &instance;
}

void AIState::InvincibleAtack::Enter(AI * pPerson)
{
	// ���G�{�^��
	pPerson->PushInputList(PLAYER_INPUT::L2);

}

void AIState::InvincibleAtack::Execute(AI * pPerson)
{
	// FinishAttack�X�e�[�g���I����Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == false)
	{
		// �ǐՃ��[�h�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	//// FinishAttack��
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == true)
	//{
	//	if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
	//	{
	//		// �ǐՃ��[�h�ɖ߂�
	//		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	//		return;
	//	}

	//}
}

void AIState::InvincibleAtack::Exit(AI * pPerson)
{

}

void AIState::InvincibleAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "InvincibleAtack");
}

bool AIState::InvincibleAtack::OnMessage(AI * pPerson, const Message & msg)
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
