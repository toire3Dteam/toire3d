#include "AI.h"
#include "AIState.h"
#include "../../Player/BasePlayer.h"
#include "../../Player/PlayerManager.h"
#include "../BasePlayerState.h"
#include "Stand\Stand.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "Shot\BaseShot.h"
#include "Shot\ShotManager.h"


#define MyPlayer (pPerson->GetBasePlayer())
#define TargetPlayer (pPerson->GetTargetPlayer())

//void PushButton(AI* pPerson, PLAYER_INPUT input)
//{
//	pPerson->PushInputList(input);
//}

// (TODO)�X�^���h�ɑ΂��Ă̏����≓�����̋Z�ɑ΂��Ă̓��������

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

// ����Ǝ�����X�̋���
float GetRange(AI * pPerson)
{
	// ����
	//if (fabsf(MyPlayer->GetPos().x - (TargetPlayer->GetPos().x +
	//	TargetPlayer->GetAttackData()->pCollisionShape->pos.x * TargetPlayer->GetDirVecX()))
	//		> BasePlayer::c_GUARD_DISTANCE)
	//{
	//	return false;				// �K�[�h��������
	//}
	//else
	//{
	//	return true;
	//}
	
	return abs(TargetPlayer->GetPos().x - MyPlayer->GetPos().x);
}

// ����̍U���Ǝ�����X�̋���
float GetRangeVsAttack(AI * pPerson)
{
	// ���肪�U�����Ă�����B
	if (TargetPlayer->isAttackState() == true)
	{
		return  (fabsf(MyPlayer->GetPos().x - (TargetPlayer->GetPos().x +
			TargetPlayer->GetAttackData()->pCollisionShape->pos.x * TargetPlayer->GetDirVecX())));
	}
	else
	{
		return 0.0f;
	}
}

// ����X�^���h�Ǝ�����X�̋���
float GetRangeVsStand(AI * pPerson)
{

	float l_fRange = 0.0f;

	// �X�^���h
	//if (TargetPlayer->GetStand()->isActive() &&
	//	TargetPlayer->GetStand()->GetAttackData() &&
	//	TargetPlayer->GetStand()->isAttackFrame())
	//{
	//	// ����
	//	return l_fRange = fabsf(TargetPlayer->GetStand()->GetPos().x - MyPlayer->GetPos().x);
	//}
	//else
	//{
	//	l_fRange = 0.0f;
	//}	

	if (TargetPlayer->GetStand()->isActive() == true)
	{
		l_fRange = TargetPlayer->GetStand()->GetPos().x
			+ TargetPlayer->GetStand()->GetAttackData()->pCollisionShape->pos.x * ((TargetPlayer->GetStand()->GetDir() == DIR::LEFT) ? -1.0f : 1.0f);

	}else
	{
		l_fRange = 0.0f;
	}

	return abs(l_fRange - MyPlayer->GetPos().x);

}

// ����̉������Ǝ�����X�̋���
//float GetRangeVsShot(AI * pPerson)
//{
//	float l_fRange = 0.0f;
//
//	if (TargetPlayer->GetStand()->isActive() == true)
//	{
//		l_fRange = TargetPlayer->GetStand()->GetPos().x;
//
//	}
//	else
//	{
//		l_fRange = 0.0f;
//	}
//
//	return abs(l_fRange - MyPlayer->GetPos().x);
//
//}

// ��Փx���Ⴂ�ǂ�������X�e�[�g�@���̂���������ڂ�
bool EasyTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		�K�[�h
	//+----------------------------
	// ���肪�U�����Ă�����B
	if (TargetPlayer->isAttackState() == true)
	{
		if (tdnRandom::Get(0, 100) <= 20)
		{
			// �K�[�h
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;

		}else
		{
			// �������Ȃ��@����悤
			pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			return true;
		}

	}


	// ���肪���G����Ȃ�������
	//if (TargetPlayer->GetInvincibleTime() <= 0)
	{
		// ���������^�C�}�[�������莞�Ԃ��߂��Ă���s�����ł���
		if (pPerson->GetWaitFrame() >= 60)
		{

			// �W�����v���Ă݂�
			if (tdnRandom::Get(0, 100) <= 15)// ���̊m����
			{
				pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
				return true;

			}

			// ���ɉ������Ă݂�@�i�������ɗ����̂ɂ��ǂ�̂��j
			if (tdnRandom::Get(0, 100) <= 30)// ���̊m����
			{
				pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
				return true;
			}

			//+---------------------------------------
			// ���肪�n��E�󒆂ɂ��邩�œ�����ς���
			//+---------------------------------------
			if (pPerson->GetTargetPlayer()->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
				return true;
			}

		}
	}

	return false;
}


// ��Փx�����ʂ̒ǂ�������X�e�[�g�@�܂��܂�
bool NormalTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		�K�[�h
	//+----------------------------
	// ���肪�U�����Ă�����B
	if (TargetPlayer->isAttackState() == true)
	{
		// 5%�̊m���Ŗ��G
		if (tdnRandom::Get(0, 100) <= 5)
		{
			// ���G�Z(���o�[�T������)
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
			return true;

		}
		else if (tdnRandom::Get(0, 100) <= 30)
		{
			// �K�[�h
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;

		}
		else
		{
			// �������Ȃ��@����悤
			pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			return true;
		}

	}


	// ���肪���G����Ȃ�������
	if (TargetPlayer->GetInvincibleTime() <= 0)
	{

		// ���������^�C�}�[�������莞�Ԃ��߂��Ă���s�����ł���
		if (pPerson->GetWaitFrame() >= 40)
		{
			// �ア�X�e�[�g�@�m�[�}���ł������̊m���Ŏc���Ă���
			{
				// �W�����v���Ă݂�
				if (tdnRandom::Get(0, 100) <= 10)// ���̊m����
				{
					pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
					return true;
				}

				// ���ɉ������Ă݂�@�i�������ɗ����̂ɂ��ǂ�̂��j
				if (tdnRandom::Get(0, 100) <= 25)// ���̊m����
				{
					pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
					return true;
				}
			}

			// �����X�e�[�g
			if (tdnRandom::Get(0, 100) <= 20)// ���̊m���Ŕ��
			{
				// �����肪�_�E�����Ă��Ȃ��Ƃ����A���ł��Ȃ��Ƃ�
				if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
				{
					pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
					return true;
				}
			}

			// �󒆍U��
			if (tdnRandom::Get(0, 100) <= 15)// ���̊m���Ŕ��
			{
				// �O��󒆕����U�������܂��Ă��Ȃ�������
				if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAttack::GetInstance()) != true)
				{
					pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
					return true;
				}
			}

			//+---------------------------------------
			// ���肪�n��E�󒆂ɂ��邩�œ�����ς���
			//+---------------------------------------
			if (pPerson->GetTargetPlayer()->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
				return true;
			}

		}
	}
	return false;
}

// ��Փx�������ǂ�������X�e�[�g�@�l�X�ȑI����������
bool HardTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		�K�[�h�E�؂�ւ���
	//+----------------------------
	// ���肪�U�����Ă�����B
	if (TargetPlayer->isAttackState() == true)
	{
		int ram = tdnRandom::Get(0, 100);

		// 15%�̊m���Ŗ��G
		if (ram <= 15)
		{
			// ���G�Z(���o�[�T������)
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
			return true;

		}
		else if (ram <= 40)
		{
			// ����X�e�[�g
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return true;
		}
		else if (ram <= 70)
		{
			// �K�[�h
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;
		}
		else
		{
			// �������Ȃ��@����悤
			pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			return true;
		}
	}


	// ���肪���G����Ȃ�������
	if (TargetPlayer->GetInvincibleTime() <= 0)
	{

		// �����X�e�[�g
		if (tdnRandom::Get(0, 100) <= 20)// ���̊m���Ŕ��
		{
			// �����肪�_�E�����Ă��Ȃ��Ƃ����A���ł��Ȃ��Ƃ�
			if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)// ���̊m���Ŕ��
		{
			// �O��󒆕����U�������܂��Ă��Ȃ�������
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAttack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)//	���̊m���Ŕ��
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return  true;
		}

		if (tdnRandom::Get(0, 100) <= 20)//	���̊m���Ŕ��
		{
			// ���G���s�i�t�؂�΍�j
			pPerson->GetFSM()->ChangeState(AIState::MightyWark::GetInstance());
			return true;
		}

		//+---------------------------------------
		// ���肪�n��E�󒆂ɂ��邩�œ�����ς���
		//+---------------------------------------
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
			return  true;
		}
		else
		{
			// ���肪�󒆂ɂ���ꍇ�͑����Ă��悤�������Ăł�������ꏊ�ɍs��
			//float  nearLen = MyPlayer->AIAttackRange();	// �����̃L�����N�^�[�̃����W

			//TargetPlayer->GetRecoveryFrame

			// 2����1�̊m���ő΋�U��
			if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return true;
			}
			else
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return true;
			}

		}
	}

	return false;
}



// ��Փx���ŋ��̒ǂ�������X�e�[�g�@�ŋ�
bool YokoeTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		�K�[�h�E�؂�ւ���
	//+----------------------------
	// ���肪�U�����Ă�����B
	if (TargetPlayer->isAttackState() == true)
	{
		int ram = tdnRandom::Get(0, 100);

		// 40%�̊m���Ŗ��G
		if (ram <= 40)
		{
			// ���G�Z(���o�[�T������)
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
			return true;

		}
		else if (ram <= 65)
		{
			// ����X�e�[�g
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return true;
		}
		else
		{
			// �K�[�h
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;
		}
	}

	// ����̃X�^���h���������Ă���	
	if (TargetPlayer->GetStand()->isActive() &&
		BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetStand()->GetAttackData()->pCollisionShape->width >= GetRangeVsStand(pPerson))//
	{
		int l_iRam = tdnRandom::Get(0, 100);
		if (l_iRam <= 35)
		{
			// ����X�e�[�g
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return true;
		}
		else
		{
			// �K�[�h
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;
		}

	}

	// ���肪���G����Ȃ�������
	if (TargetPlayer->GetInvincibleTime() <= 0)
	{

		// �����X�e�[�g
		if (tdnRandom::Get(0, 100) <= 20)// ���̊m���Ŕ��
		{
			// �����肪�_�E�����Ă��Ȃ��Ƃ����A���ł��Ȃ��Ƃ�
			if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 5)// ���̊m���Ŕ��
		{
			// �O��󒆕����U�������܂��Ă��Ȃ�������
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAttack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)//	���̊m���Ŕ��
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return  true;
		}

		if (tdnRandom::Get(0, 100) <= 25)//	���̊m���Ŕ��
		{
			// ���G���s�i�t�؂�΍�j
			pPerson->GetFSM()->ChangeState(AIState::MightyWark::GetInstance());
			return true;
		}

		//+---------------------------------------
		// ���肪�n��E�󒆂ɂ��邩�œ�����ς���
		//+---------------------------------------
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
			return  true;
		}
		else
		{
			// ���肪�󒆂ɂ���ꍇ�͑����Ă��悤�������Ăł�������ꏊ�ɍs��
			//float  nearLen = MyPlayer->AIAttackRange();	// �����̃L�����N�^�[�̃����W


			// ���i��Փx�ɑ����镨�j����҂̎��߂ɂ���Α΋󓖂Ă�}�����m����

			// 2����1�̊m���ő΋�U��
			//if (tdnRandom::Get(0, 2) == 0)
			//{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return true;
			//}
			//else
			//{
			//	pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
			//	return true;
			//}

		}
	}
	return false;
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
		// �Ȃ������X�e�[�g�ɂ܂��ڍs���ĂȂ�������
		if (pPerson->GetFSM()->isInState(*AIState::ThrowTech::GetInstance()) == false)
		{
			// �����������f�X�e�[�g��
			pPerson->GetFSM()->ChangeState(AIState::ThrowTech::GetInstance());
			return;
		}
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
	// �����ɕ߂܂�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true)
	{
		// �ݒ�œ�����������悤�ɂ��Ă�����
		if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech == ENEMY_THROW_TECH_TYPE::OK)
		{
			// ��������
			pPerson->PushInputList(PLAYER_INPUT::A);
		}
	}


	// �K�[�h�̐ݒ�
	switch ((ENEMY_GUARD_TYPE)SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard)
	{
	case ENEMY_GUARD_TYPE::NO:
		// �K�[�h���Ȃ�
		pPerson->m_bPracticeGuardFlag = false;
		break;
	case ENEMY_GUARD_TYPE::WAY:
		// �r������K�[�h

		break;
	case ENEMY_GUARD_TYPE::ALL:
		// �S���K�[�h�������ꍇ��ɃK�[�h��������
		pPerson->m_bPracticeGuardFlag = true;
		break;
	default:
		MyAssert(0, "���̃^�C�v�͂Ȃ�");
		break;
	}


	// �������U�����󂯂ē|���Œ��Ȃ��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// �󂯐g
		switch ((ENEMY_TECH_TYPE)SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech)
		{
		case ENEMY_TECH_TYPE::NO:
			// �����{�^���������Ȃ�

			break;
		case ENEMY_TECH_TYPE::LAND:
			if (MyPlayer->isLand() == true)
			{
				// ���A�{�^��
				pPerson->PushInputList(PLAYER_INPUT::B);
			}
			break;
		case ENEMY_TECH_TYPE::AIR:
			if (MyPlayer->isLand() == false)
			{
				// ���A�{�^��
				pPerson->PushInputList(PLAYER_INPUT::B);
			}
			break;
		case ENEMY_TECH_TYPE::ALL:
			// ���A�{�^��
			pPerson->PushInputList(PLAYER_INPUT::B);
			break;
		default:
			MyAssert(0, "���̃^�C�v�͂Ȃ�");
			break;
		}


	}

	// �|��Ȃ��m�b�N�o�b�N�Z���󂯂��Ȃ�
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true)
	{
		pPerson->m_bPracticeGuardFlag = true;
		pPerson->m_iPracticeGuardFrame = 0;
	}

	// ���K�p�K�[�h�t���O�������Ă���+�n��ɂ�����@
	if (pPerson->m_bPracticeGuardFlag == true &&
		MyPlayer->isLand() == true && 
		MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == false)
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
		



		// ����̃X�^���h���������Ă���	
		if (TargetPlayer->GetStand()->isActive() &&
			TargetPlayer->GetStand()->isCanGuardStand() &&
			BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetStand()->GetAttackData()->pCollisionShape->width  >= GetRangeVsStand(pPerson))//
		{
			// ���ݒ�ɂ��K�[�h��؂�ւ��邩�ς��Ȃ����ݒ肷��
			if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch == ENEMY_GUARD_SWITCH_TYPE::OK)
			{
				// ����Ƌt�������o�[������
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}

				// ���i�Ȃ�
				if (TargetPlayer->GetStand()->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
				{
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
				}

			}
			else
			{

				// ����Ƌt�������o�[������
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}


			}// ���i�����i��

		}// ����X�^���h�ɑ΂���K�[�h


		// �������̔���
		for (auto it : *ShotMgr->GetList(pPerson->GetTargetPlayer()->GetSide()))
		{
			float fRange = fabsf((it)->GetPos().x - pPerson->GetBasePlayer()->GetPos().x);

			// �������Z�Ƃ̋������k�܂��
			if (BasePlayer::c_GUARD_ADD_RANGE + (it)->GetAttackData()->pCollisionShape->width  > fRange)
			{
				// ���ݒ�ɂ��K�[�h��؂�ւ��邩�ς��Ȃ����ݒ肷��
				if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch == ENEMY_GUARD_SWITCH_TYPE::OK)
				{
					// ����Ƌt�������o�[������
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}

					// ���i�Ȃ�
					if ((it)->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
					{
						pPerson->PushInputList(PLAYER_INPUT::DOWN);
					}

				}
				else
				{

					// ����Ƌt�������o�[������
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}


				}// ���i�����i��

			}


		}// �������̔���

		 // ���肪�U���U�����炩��
		 // 	����̋Z���߂��ɂ�����
		if (TargetPlayer->isAttackState() == true &&
			BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetAttackData()->pCollisionShape->width >= GetRangeVsAttack(pPerson))// (TODO)
		{
			// ������������n�W�N(�����ɑ΂��Ĉړ����Ă��܂�����)
			if (TargetPlayer->GetActionState() != BASE_ACTION_STATE::THROW)
			{

				// ���ݒ�ɂ��K�[�h��؂�ւ��邩�ς��Ȃ����ݒ肷��
				if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch == ENEMY_GUARD_SWITCH_TYPE::OK)
				{
					// ����Ƌt�������o�[������
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
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
				else
				{

					// ����Ƌt�������o�[������
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}


				}// ���i�����i��

			}// ������������n�W�N

		}// �v���C���[�ɑ΂���K�[�h

	}// ���K�p�K�[�h�t���O

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

	// �K�[�h��Ԃ���Ȃ�������
	if (pPerson->m_bPracticeGuardFlag == false)
	{
		// �n��ɕt������W�����v����
		if (MyPlayer->isLand())
		{
			// �W�����v
			pPerson->PushInputList(PLAYER_INPUT::UP);
		}
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
//				���K�p�n�ʂ��Ⴊ�݃X�e�[�g
/*******************************************************/

AIState::PracticeSquat * AIState::PracticeSquat::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeSquat instance;
	return &instance;
}

void AIState::PracticeSquat::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);

}

void AIState::PracticeSquat::Execute(AI * pPerson)
{	
	// ���K�p�K�[�h�t���O
	if (pPerson->m_bPracticeGuardFlag == true/*&& MyPlayer->GetRecoveryFrame() <= 0�@�K�[�h�d�����܂ނ̂łȂ�*/)
	{
		// ���肪�U���U���Ă��邩
		if (TargetPlayer->isAttackState() == true &&
			BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetAttackData()->pCollisionShape->width  >= GetRangeVsAttack(pPerson))// (TODO)
		{
			// ���ݒ�ɂ��K�[�h��؂�ւ��邩�ς��Ȃ���
			if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch 
				== ENEMY_GUARD_SWITCH_TYPE::OK)
			{
				// ���Ⴊ�܂Ȃ�

			}
			else
			{
				// ��ɂ��Ⴊ�ݏ��
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
			}

		}
		else
		{
			// ��ɂ��Ⴊ�ݏ��
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
		}

	}
	else
	{
		// ��ɂ��Ⴊ�ݏ��
		pPerson->PushInputList(PLAYER_INPUT::DOWN);
	}



}

void AIState::PracticeSquat::Exit(AI * pPerson)
{
}

void AIState::PracticeSquat::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI�n�ʂ��Ⴊ��");

#endif // _DEBUG

}

bool AIState::PracticeSquat::OnMessage(AI * pPerson, const Message & msg)
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
//				���K�p�K�[�h�X�e�[�g
/*******************************************************/

AIState::PracticeGuard * AIState::PracticeGuard::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeGuard instance;
	return &instance;
}

void AIState::PracticeGuard::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);

}

void AIState::PracticeGuard::Execute(AI * pPerson)
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
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
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

void AIState::PracticeGuard::Exit(AI * pPerson)
{
}

void AIState::PracticeGuard::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI���K�K�[�h");

#endif // _DEBUG

}

bool AIState::PracticeGuard::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}

/*******************************************************/
//				���G���荞�݃`�F�b�N�p�X�e�[�g
/*******************************************************/

AIState::InterruptInvincible * AIState::InterruptInvincible::GetInstance()
{
	// �����ɕϐ������
	static AIState::InterruptInvincible instance;
	return &instance;
}

void AIState::InterruptInvincible::Enter(AI * pPerson){}

void AIState::InterruptInvincible::Execute(AI * pPerson)
{
	// �������U�����󂯂ē|���Œ��Ȃ��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// ���A�{�^��
		pPerson->PushInputList(PLAYER_INPUT::B);

		pPerson->m_bPracticeGuardFlag = false;
		pPerson->m_iGuardFrame = 0;
	}

	// �K�[�h��
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()))
	{
		// �܂�����̍U�����K�[�h���Ă��Ȃ����(���[�V���������K�[�h���Ă���)
		if (!pPerson->m_bPracticeGuardFlag)
		{
			// �K�[�h���J�o���[�t���[����0�ȏ�A���Ȃ킿����̍U�����󂯂���t���O��ON�ɂ���
			pPerson->m_bPracticeGuardFlag = (MyPlayer->GetRecoveryFrame() > 0);
		}

		// �K�[�h��1�x�ł��󂯂��ꍇ��if��
		else
		{
			// 2�t���[���P�ʂŖ��G�U���{�^����A�ł���
			if (++pPerson->m_iGuardFrame % 2) pPerson->PushInputList(PLAYER_INPUT::L2);
		}
	}

	// ���G�U����U���Ă���A���傤�ق����肹���Ɓ@
	else if (MyPlayer->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()))
	{
		pPerson->m_bPracticeGuardFlag = false;
		pPerson->m_iGuardFrame = 0;
	}

	// ���肪�U���U������
	if (TargetPlayer->isAttackState() == true)
	{
		// ����Ƌt�������o�[������
		if (MyPlayer->GetTargetDir() == DIR::LEFT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}

		// ��{���i�K�[�h
		if (TargetPlayer->GetAttackData()->AntiGuard != ANTIGUARD_ATTACK::UP_ATTACK)
		{
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
		}
	}
}

void AIState::InterruptInvincible::Exit(AI * pPerson)
{
}

void AIState::InterruptInvincible::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI���G���荞�݃`�F�b�N�p");

#endif // _DEBUG

}

bool AIState::InterruptInvincible::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//				���K�p���i�U���X�e�[�g
/*******************************************************/

AIState::PracticeDokkoiAttack* AIState::PracticeDokkoiAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeDokkoiAttack instance;
	return &instance;
}

void AIState::PracticeDokkoiAttack::Enter(AI * pPerson)
{
	pPerson->m_iPracticeAttackIntervalFrame = 60;

}

void AIState::PracticeDokkoiAttack::Execute(AI * pPerson)
{

	// ��莞�Ԃ���������U��
	pPerson->m_iPracticeAttackIntervalFrame++;
	if (pPerson->m_iPracticeAttackIntervalFrame >= 90)
	{


		// ��������ɕ���
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		if (MyPlayer->GetRecoveryFrame() <= 0 &&	// �������U���I�������
			MyPlayer->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) == false)
		{
			if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
			else
			{
				// ����̉��ɓ�������
				pPerson->m_iPracticeAttackIntervalFrame = 0;
				// ���i�U��
				pPerson->PushInputList(PLAYER_INPUT::R1);
			}
		}




	}


}

void AIState::PracticeDokkoiAttack::Exit(AI * pPerson)
{
}

void AIState::PracticeDokkoiAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AIPracticeDokkoiAttack���K");

#endif // _DEBUG

}

bool AIState::PracticeDokkoiAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//				���K�p���i�U���X�e�[�g
/*******************************************************/

AIState::PracticeDownAttack* AIState::PracticeDownAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::PracticeDownAttack instance;
	return &instance;
}

void AIState::PracticeDownAttack::Enter(AI * pPerson)
{
	pPerson->m_iPracticeAttackIntervalFrame = 60;

}

void AIState::PracticeDownAttack::Execute(AI * pPerson)
{

	// ��莞�Ԃ���������U��
	pPerson->m_iPracticeAttackIntervalFrame++;
	if (pPerson->m_iPracticeAttackIntervalFrame >= 90)
	{

		// ��������ɕ���
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		if (MyPlayer->GetRecoveryFrame() <= 0)// �������U���I�������
		{
			if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
			else
			{
				// ����̉��ɓ�������
				pPerson->m_iPracticeAttackIntervalFrame = 0;
				// ���i�U��
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
				pPerson->PushInputList(PLAYER_INPUT::C);
			}
		}

	}


}

void AIState::PracticeDownAttack::Exit(AI * pPerson)
{
}

void AIState::PracticeDownAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AIPracticeDownAttack���K");

#endif // _DEBUG

}

bool AIState::PracticeDownAttack::OnMessage(AI * pPerson, const Message & msg)
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
		// �U����
		//pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
		//return;

		// �m��  (�l���Ⴂ�قǐ��\�̗ǂ��Z���ł�)
		int l_iStrong= tdnRandom::Get(0, 100);
		// ��Փx�ɂ��␳�l
		switch (pPerson->m_eAIType)
		{
		case AI_TYPE::CPU_EASY:
			l_iStrong += 50;
			break;
		case AI_TYPE::CPU_NORMAL:
			l_iStrong += 25;
			break;
		case AI_TYPE::CPU_HARD:
			l_iStrong += 0;
			break;
		case AI_TYPE::CPU_YOKOE:
			l_iStrong += -25;
			break;
		default:
			break;
		}

		// ���肪�N���オ��U���킹�Ă�����
		if (TargetPlayer->isAttackState() == true &&
			TargetPlayer->AIAttackRange() > GetRange(pPerson))
		{
			// ��%�̊m���Ŗ��G
			if (l_iStrong <= 30)
			{
				// ���G�Z(���o�[�T������)
				pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
				return;
			} 		
			else if (l_iStrong <= 60)	// ��%�̊m���ŃK�[�h
			{
				// �K�[�h
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}

		}

		// �ō���Փx����
		if (pPerson->m_eAIType == AI_TYPE::CPU_YOKOE)
		{
			// ����̃X�^���h���������Ă���	
			if (TargetPlayer->GetStand()->isActive() &&
				30 >= GetRangeVsStand(pPerson))//
			{
				if (l_iStrong <= 25)
				{
					// ����X�e�[�g
					pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
					return;
				}
				else
				{
					// �K�[�h
					pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
					return;
				}
			}
		}

		// ���p�[�g�i�[�Q�[�W�����܂��Ă���
		// ����Ƀ_���[�W���ʂ�ꍇ
		// ��m���Ńp�[�g�i�[����
			if (TargetPlayer->GetInvincibleTime() <= 0 &&
				TargetPlayer->isAttackState() == false && 
				MyPlayer->GetStand()->GetStandStock() >= 1)
		{
			// �m�� 
			int l_iPartnerPow = 0;
			// ��Փx�ɂ��␳�l
			switch (pPerson->m_eAIType)
			{
			case AI_TYPE::CPU_EASY:
				l_iPartnerPow = 5;
				break;
			case AI_TYPE::CPU_NORMAL:
				l_iPartnerPow = 15;
				break;
			case AI_TYPE::CPU_HARD:
				l_iPartnerPow = 25;
				break;
			case AI_TYPE::CPU_YOKOE:
				l_iPartnerPow = 30;
				break;
			default:
				break;
			}

			if (tdnRandom::Get(0, 100) <= l_iPartnerPow)
			{
				// �p�[�g�i�[�������[�h��
				pPerson->GetFSM()->ChangeState(AIState::PartnerAttack::GetInstance());
				return;
			}
		}

		// ����Ƀ_���[�W���ʂ�@���@��m���ŃX�L���Z���o���Ă݂�
		if (TargetPlayer->GetInvincibleTime() <= 0 && 
			TargetPlayer->isAttackState() == false)
		{
			if (tdnRandom::Get(0, 100) <= 7)
			{
				// �X�L�����[�h��
				pPerson->GetFSM()->ChangeState(AIState::SkillAttack::GetInstance());
				return;
			}
		}

		//if (pPerson->m_eAIType == AI_TYPE::CPU_EASY) 
		//{
		//	// 3����1��
		//	if (tdnRandom::Get(0, 3) <= 0)
		//	{
		//		// ���֕���
		//		pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
		//		return;
		//	}
		//}
		if (pPerson->m_eAIType == AI_TYPE::CPU_EASY||
			pPerson->m_eAIType == AI_TYPE::CPU_NORMAL)
		{
			// 5����1��
			if (tdnRandom::Get(0, 5) <= 0)
			{
				// �O����������Ă��Ȃ�������
				if (pPerson->GetFSM()->isPrevState(*AIState::BackWark::GetInstance()) != true)
				{
					// ���֕���
					pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
					return;
				}
			}
		}

		// �󒆂��n��ǂ���Œǂ������邩
		if (tdnRandom::Get(0, 100) <= 20)
		{
			// �C�[�W�[���[�h�Ȃ��̉�
			if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)
			{
				// �O����ł��Ȃ�������
				if (pPerson->GetFSM()->isPrevState(*AIState::SimpleJump::GetInstance()) != true)
				{
					// �W�����v���[�h��
					pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
					return;
				}
				else
				{
					// �ǐՃ��[�h��
					pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
					return;
				}

			}
			else
			{
				// �󒆒ǐՃ��[�h��
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return;
			}

		}
		else
		{
			// �����C�[�W�[���[�h�Ȃ�
			if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)
			{	
				// �O����ł��Ȃ�������
				if (pPerson->GetFSM()->isPrevState(*AIState::SimpleJump::GetInstance()) != true)
				{
					// 4����1��
					if (tdnRandom::Get(0, 4) <= 0)
					{
						// ���֕���
						pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
						return;
					}
				}
			}

			// �ǐՃ��[�h��
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

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
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	//+---------------------------------------------------
	// �ߋ����U����������Ȃ��قǑ��肪����Ă���
	//+---------------------------------------------------
	// ���ݍ��ݎ�����
	if (MyPlayer->GetJump()->bHold == true)
	{
		if (abs(TargetPlayer->GetPos().x - MyPlayer->GetPos().x)
			>= MyPlayer->AIAttackRange())
		{
			// ��������ɃL�[����
			if (TargetPlayer->GetPos().x > MyPlayer->GetPos().x)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}
		}
	}

	//+-----------------------------------------
	//	�I������
	//+-----------------------------------------
	// �W�����v�̓��ݍ��݂̓J�E���g���Ȃ�
	if (MyPlayer->GetJump()->bHold == false)
	{
		// �n��ɒ��n������ ���@�ҋ@�X�e�[�g�ɂ����
		if (MyPlayer->isLand() && MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// AI�ҋ@�ɖ߂�
			pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
			return;
		}
	}

}

void AIState::SimpleJump::Exit(AI * pPerson)
{
}

void AIState::SimpleJump::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI�V���v���W�����v");
#endif // _DEBUG

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
	// ���X�^���h���o���Ă��Ȃ����������(2/4)
	if (pPerson->GetTargetPlayer()->isAttackState() == false &&
		TargetPlayer->GetStand()->isActive() == false)
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

	// �m��  (�l���Ⴂ�قǐ��\�̗ǂ��Z���ł�)
	int l_iRamdom = tdnRandom::Get(0, 100);
	// ��Փx�ɂ��␳�l
	switch (pPerson->m_eAIType)
	{
	case AI_TYPE::CPU_EASY:
		l_iRamdom += 45;
		break;
	case AI_TYPE::CPU_NORMAL:
		l_iRamdom += 25;
		break;
	case AI_TYPE::CPU_HARD:
		l_iRamdom += 0;
		break;
	case AI_TYPE::CPU_YOKOE:
		l_iRamdom += -70;
		break;
	default:
		break;
	}
	//if(���[���[��������)
	//{
	//}else
	//{

	// ���肪�U���U������
	if (TargetPlayer->isAttackState() == true)
	{
		if (l_iRamdom <= 20) // ���̊m���Ŋ��S�K�[�h
		{
			// ����Ƌt�������o�[������
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
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
		else  if (l_iRamdom <= 50) 
		{
			// ���Ⴊ�݃K�[�h
			// ����Ƌt�������o�[������
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}

			if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
			{
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
			}
		}
		else 
		{
			// �����K�[�h
			// ����Ƌt�������o�[������
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}
		}
	}

	// �X�^���h�p�[�g�i�[�ɑ΂���K�[�h
	if (pPerson->m_eAIType == AI_TYPE::CPU_YOKOE)
	{

		// ����̃X�^���h���������Ă���	
		if (TargetPlayer->GetStand()->isActive() &&
			30 >= GetRangeVsStand(pPerson))//
		{
			if (l_iRamdom <= 20) // ���̊m���Ŋ��S�K�[�h
			{
				// ����Ƌt�������o�[������
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}

				// ���i�Ȃ�
				if (TargetPlayer->GetStand()->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
				{
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
				}
			}
			else  if (l_iRamdom <= 50)
			{
				// ���Ⴊ�݃K�[�h
				// ����Ƌt�������o�[������
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}

				if (TargetPlayer->GetStand()->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
				{
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
				}
			}
			else
			{
				// �����K�[�h
				// ����Ƌt�������o�[������
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}
			}

		}// ����X�^���h�ɑ΂���K�[�h
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

	// ���������^�C�}�[������
	pPerson->m_iWaitFrame = 0;
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
		len = MyPlayer->AIRunAttackRange();	// �����̃L�����N�^�[�̃����W
	else len = MyPlayer->AIAttackRange();	// �����̃L�����N�^�[�̃����W
	
	//+-------------------------------------------------
	//	���̋����ň��܂ŋ߂Â�����@(�����_��)�ōU�� (1210) x�̋����ɕύX 
	//+-------------------------------------------------

	if (Math::Length(TargetPos.x, MyPos.x) < len)
	{
		bool bReturn = false;

		// AI�̃^�C�v�ɂ�肱����ύX
		switch (pPerson->m_eAIType)
		{
		case AI_TYPE::CPU_EASY:
			bReturn = EasyTypeChaseBrain(pPerson);
			break;
		case AI_TYPE::CPU_NORMAL:
			bReturn = NormalTypeChaseBrain(pPerson);
			break;
		case AI_TYPE::CPU_HARD:
			bReturn = HardTypeChaseBrain(pPerson);
			break;
		case AI_TYPE::CPU_YOKOE:
			bReturn = YokoeTypeChaseBrain(pPerson);
			break;
		default:
			break;
		}
	
		// ���^�[���t���O�������Ă���
		if (bReturn == true)return;
		
	}
	

	//+-----------------------------------------
	// �_�b�V������@(12/10) �ړ�����
	//+-----------------------------------------
	int iDushRange = 25;
	if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)iDushRange = 60;
	if (pPerson->GetAIType() == AI_TYPE::CPU_NORMAL)iDushRange = 40;

	if (Math::Length(TargetPos, MyPos) >= iDushRange)		// ���̃p�����[�^�[������ɐ؂�ւ��鋗��
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
		if (TargetPos.x > MyPos.x + 8)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else if (TargetPos.x < MyPos.x - 8)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
		else// ������̂ɂ��肬��܂Ŕ������Ƃ��̏���
		{
			// �ҋ@�ɖ߂낤�@�����܂ŋ߂Â��ĉ������Ȃ��̂�
			// ���Ȃ�₳������Փx 
			// ����ڂ��U����
			// �����͂܂���łȂ�Ƃ����܂�
			//pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			//return;
			
			//pPerson->m_iWaitFrame += 100;

		}

		//if (TargetPos.x > MyPos.x )	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		//else 	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	}

		
	/*******************************************/
	//	�����ǂ����������Ȃ��悤�̐V�����s��
	/*******************************************/
	pPerson->m_iChaseFrame++;			// �ǂ������^�C�}�[�X�V
	if (pPerson->m_iChaseFrame >= 300)	// 5�b�ԔS�����Ēǂ�������
	{
		pPerson->m_iChaseFrame = 0; // ������
	
		// �ҋ@��
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// ���������^�C�}�[�X�V
	pPerson->m_iWaitFrame++;
}

void AIState::Chase::Exit(AI * pPerson)
{
}

void AIState::Chase::Render(AI * pPerson)
{

#ifdef _DEBUG
	tdnText::Draw(420, 610, 0xffffffff, "AI�`�F�C�X");
	tdnText::Draw(420, 660, 0xffffffff, "AI�ǂ������Ă�v���C���[No%d", pPerson->GetTargetPlayer()->GetDeviceID());
	tdnText::Draw(420, 700, 0xffffffff, "����%.2f", Math::Length(TargetPlayer->GetPos(), MyPlayer->GetPos()));


#endif // _DEBUG


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

	//+---------------------------------------------------
	// �ߋ����U����������Ȃ��قǑ��肪����Ă���
	//+---------------------------------------------------
	// ���ݍ��ݎ�����
	if (MyPlayer->GetJump()->bHold == true)
	{
		if (abs(TargetPlayer->GetPos().x - MyPlayer->GetPos().x)
			>= MyPlayer->AIAttackRange())
		{
			// ��������ɃL�[����
			if (TargetPlayer->GetPos().x > MyPlayer->GetPos().x)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}
		}
	}

	//+---------------------------------------------------
	// ����Ƃ̋��������ȏ㗣��Ă��ċ󒆃W�����v�̌����������
	// ����ɒn�ʂ��炻��Ȃ�ɗ���Ă���
	//+---------------------------------------------------
	if (abs(TargetPos.x - MyPos.x) >= 35 &&
		MyPlayer->isAerialJump() == true &&
		MyPos.y >= 7.0f)
	{
		// �󒆃_�b�V��
		if (TargetPos.x > MyPos.x)
		{	
			// ����񉟂� �󒆃_�b�V���̂��߂�
			pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
		}
		else
		{
			// ����񉟂� �󒆃_�b�V���̂��߂�
			pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
		}

	}

	//+---------------------------------------------------
	// �U���̏���
	//+---------------------------------------------------
	// �����Ă���
	if (MyPlayer->isLand()==false)
	{
		// ���܂ŋ߂Â�����
		if (Math::Length(TargetPos, MyPos) < 32)
		{
			// �������n�߂��u�Ԃ���
			if (MyPlayer->GetMove().y < 0.5f)
			{
				if (pPerson->GetTargetPlayer()->isLand() == false)
				{
					// ��������󒆂ɂ���Ȃ�󒆃R���{�Ɏ����Ă���
					pPerson->GetFSM()->ChangeState(AIState::AerialAttack::GetInstance());
					return;
				}
			}

			// �����������n�߂���
			if (MyPlayer->GetMove().y < -0.3f)
			{
				// �󒆉��U���Ő���
				pPerson->GetFSM()->ChangeState(AIState::AerialDestructAttack::GetInstance());
				return;
			}

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

AIState::RushAttack * AIState::RushAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::RushAttack instance;
	return &instance;
}

void AIState::RushAttack::Enter(AI * pPerson)
{
	// ���肪�����Ă�����
	if (pPerson->GetTargetPlayer()->isLand() == false)
	{		
		// AI�̃^�C�v�ɂ�肱����ύX
		switch (pPerson->GetAIType())
		{
		case AI_TYPE::CPU_HARD:

			break;
		case AI_TYPE::CPU_YOKOE:
			// �󒆍U����
			//pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
			//return;
			// �΋�
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return;
			break;
		default:
			break;
		}
		
	}
	

	// �U���J�n�A�ŏ��̈�i�ڂ�U��ŁI�I�I
	pPerson->PushInputList(PLAYER_INPUT::C);

	// �񕪂̈�ŉ��i
	if (tdnRandom::Get(0, 1) == 0)
	{
		pPerson->PushInputList(PLAYER_INPUT::DOWN);
	}

}

void AIState::RushAttack::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// �v���C���[���ҋ@�ɂ��ǂ��Ă���{
	// [1210] GetAtactData�ŃA�T�[�g���N����̂Œǉ��@��Ńo�[�X�g���Ă݂邩
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)/*!MyPlayer->isAttackState()*/
	{
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


	// ��Փx�ō��������ꍇ
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
	{

		// ���b�V���U������r���ő��肪��ɉ����Ă�����{
		// �������܂��U�����Ă��Ȃ�������
		if (MyPlayer->isAttackState() == false && 
			TargetPlayer->isAttackState() == true)
		{
			//+----------------------------
			//		�K�[�h�E�؂�ւ���
			//+----------------------------
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return;
		}

	}

	// ��������Ƀ��o�[��|�� ���܂��Ȃ����
	//if (TargetPos.x > MyPos.x)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//}
	//else
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//}


	// �U�����I�I
	if (MyPlayer->isAttackState() == true)
	{

		// ���i���q�b�g��������
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()) == true &&
			MyPlayer->GetAttackData()->bHit == true)
		{
			// ��i�ڍU���I
			pPerson->PushInputList(PLAYER_INPUT::C);
			return;
		}


		// ��i�ڂ��q�b�g��������
		if (MyPlayer->GetRushStep() == 0 &&
			MyPlayer->GetAttackData()->bHit == true)
		{
			// ��i�ڍU���I
			pPerson->PushInputList(PLAYER_INPUT::C);
			return;
		}

		// ��i�ڂ��q�b�g��������
		if (MyPlayer->GetRushStep() == 1 &&
			MyPlayer->GetAttackData()->bHit == true)
		{

			// (��Փx��)��������ɃK�[�h����Ă������
			if (pPerson->GetAIType() == AI_TYPE::CPU_HARD ||
				pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
			{

				if (TargetPlayer->GetGuardState() == GUARD_STATE::DOWN_GUARD)
				{
					// �������Z��������Ȃ��Ȃ�
					if (abs(TargetPos.x - MyPos.x) <= MyPlayer->AIAttackRange())
					{
						// �m���Œ��i�X�e�[�g��
						if (tdnRandom::Get(0, 1) == 0)
						{
							pPerson->GetFSM()->ChangeState(AIState::BackStep::GetInstance());
							return;

						}
						else
						{
							//pPerson->PushInputList(PLAYER_INPUT::R1);
							pPerson->GetFSM()->ChangeState(AIState::DokkoiAttack::GetInstance());
							return;
						}

					}
					else
					{
						// �o�b�N�X�e�b�v
						pPerson->GetFSM()->ChangeState(AIState::BackStep::GetInstance());
						return;

					}
					//pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
					return;

				}
				else if (TargetPlayer->GetGuardState() == GUARD_STATE::UP_GUARD)
				{
					// ������
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
					// ����̕������o�[
					if (MyPlayer->GetPos().x < TargetPlayer->GetPos().x)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}
					pPerson->PushInputList(PLAYER_INPUT::R1);
					return;
				}

			}// ��Փx���̐ݒ�
			

			{
				// �Ō�O�i�ڍU���I
				pPerson->PushInputList(PLAYER_INPUT::C);
				return;
			}

		}



		// �O�i�ڂ��q�b�g����||�Q�[�W50%�ȏ㗭�܂��Ă���
		if (MyPlayer->GetRushStep() == 2 &&
			MyPlayer->GetAttackData()->bHit == true &&
			MyPlayer->GetOverDriveGage() >= 50.0f)
		{
			// �K�[�h����Ă��Ȃ�������
			if (TargetPlayer->GetGuardState() == GUARD_STATE::NO_GUARD)
			{
				// �K�E�Z�����I�I
				pPerson->PushInputList(PLAYER_INPUT::C);
				return;
			}
			else // �K�[�h����Ă�����ҋ@��
			{
				pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
				return;
			}
		}

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

void AIState::RushAttack::Exit(AI * pPerson)
{
}

void AIState::RushAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "���b�V���U��");
}

bool AIState::RushAttack::OnMessage(AI * pPerson, const Message & msg)
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
	int l_iRate = tdnRandom::Get(0, 100);	
	
	switch (pPerson->GetAIType())
	{
	case AI_TYPE::CPU_EASY:
		l_iRate += 40;
		break;
	case AI_TYPE::CPU_NORMAL:
		l_iRate += 30;
		break;
	case AI_TYPE::CPU_HARD:
		l_iRate += -30;
		break;
	case AI_TYPE::CPU_YOKOE:
		l_iRate += -50;
		break;
	default:
		break;
	}

	// ���̐��l�������Ȃ瓊����������
	if (l_iRate <= 40)
	{
		// ��������
		pPerson->PushInputList(PLAYER_INPUT::A);
	}
	//else
	//{
	//	// �����������s�B�ҋ@�ɖ߂�
	//	pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	//	return;
	//}


}

void AIState::ThrowTech::Execute(AI * pPerson)
{
	// ���łȂ����������ĂȂ������炱���ő��~��

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

	// �����͈͓� (�����̍U���͈͓�)
	//enum { ABJUST = 2 };
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIThrowRange() /*/ ABJUST*/))
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


	// ��Փx�ō��������ꍇ
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
	{
		// ����̋߂��ɂ��ā������肪�U���U������
		if (MyPlayer->AIAttackRange() >= GetRange(pPerson) &&
			TargetPlayer->isAttackState() == true)
		{
			if (tdnRandom::Get(0, 1) == 0)
			{
				// �K�[�h��
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
			else
			{
				// �����
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
		}


		// ����̃X�^���h���������Ă���	
		if (TargetPlayer->GetStand()->isActive() &&
			30 >= GetRangeVsStand(pPerson))//
		{
			if (tdnRandom::Get(0, 100) <= 20)
			{
				// ����X�e�[�g
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
			else
			{
				// �K�[�h
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
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
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIAttackRange() / 1.5f))
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

	// ��Փx�ō��������ꍇ
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
	{
		// ����̋߂��ɂ��ā������肪�U���U������+�n�ʂɑ��肪���Ă���
		if (MyPlayer->AIAttackRange() >= GetRange(pPerson) &&
			TargetPlayer->isAttackState() == true&&
			TargetPlayer->isLand() == true)
		{
			if (tdnRandom::Get(0, 1) == 0)
			{
				// �K�[�h��
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
			else
			{
				// �����
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
		}


	
			// ����̃X�^���h���������Ă���	
		if (TargetPlayer->GetStand()->isActive() &&
			30 >= GetRangeVsStand(pPerson))//
		{
			if (tdnRandom::Get(0, 100) <= 25)
			{
				// ����X�e�[�g
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
			else
			{
				// �K�[�h
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
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
	pPerson->PushInputList(PLAYER_INPUT::R1);
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
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAttack::GetInstance());
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

AIState::SetAerialAttack * AIState::SetAerialAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::SetAerialAttack  instance;
	return &instance;
}

void AIState::SetAerialAttack::Enter(AI * pPerson)
{
	// ����Ƃ̋���������Ă���������L�[�����
	float l_fXLength = abs(MyPlayer->GetPos().x - TargetPlayer->GetPos().x);
	if (l_fXLength >= 3) 
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

void AIState::SetAerialAttack::Execute(AI * pPerson)
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
		// �U���ł��Ȃ��̂Ŗ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ���E�̈ړ�
	//if (TargetPos.x > MyPos.x+5)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);	
	//else if (TargetPos.x < MyPos.x-5)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	// �U���͈͓�
	if ( Math::Length(TargetPos, MyPos) < 18 )
	{
		// (TODO) ���h���Ƃ��ŏ������Â炢���ɂȂ��Ă�̂Ō�Œ���

		// �����������Ă�Ƃ�
		if (MyPlayer->GetMove().y < -0.0f ||
			MyPlayer->isAerialJump() == false&& MyPlayer->GetMove().y < 1.0f)// ��i�W�����v�ڂȂ痎���O�ɃL�����Z���ł��܂�
		{
			// �W�����v�E�G���A���W�����v���[�h�ɂȂ��Ă���
			if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == true ||
				MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()) == true)
			{

				// �󒆍U�������X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(AIState::AerialAttack::GetInstance());
				return;

			}
		}
	}

}

void AIState::SetAerialAttack::Exit(AI * pPerson)
{

}

void AIState::SetAerialAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SetAerialAttack");
}

bool AIState::SetAerialAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::AerialAttack * AIState::AerialAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::AerialAttack  instance;
	return &instance;
}

void AIState::AerialAttack::Enter(AI * pPerson)
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
		// �Y�̋��͂ȃX�L�������X�e�[�g�ֈړ��I
		pPerson->GetFSM()->ChangeState(AIState::SkillAttack::GetInstance());
		return;
	}
}

void AIState::AerialAttack::Execute(AI * pPerson)
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
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAttack::GetInstance());
				return;
			}
			else
			{
				//pPerson->GetFSM()->ChangeState(AIState::SetAerialAttack::GetInstance());

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

void AIState::AerialAttack::Exit(AI * pPerson)
{

}

void AIState::AerialAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialAttack");
}

bool AIState::AerialAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::AerialDestructAttack * AIState::AerialDestructAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::AerialDestructAttack  instance;
	return &instance;
}

void AIState::AerialDestructAttack::Enter(AI * pPerson)
{

	// �󒆍U��
	pPerson->PushInputList(PLAYER_INPUT::C);
	
}

void AIState::AerialDestructAttack::Execute(AI * pPerson)
{


	// �n��ɒ��n������
	if (MyPlayer->isLand() == true)
	{
		// �����Ă���̂�����ǐՃ��[�h�ɍs�������Ȃ��I
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
}

void AIState::AerialDestructAttack::Exit(AI * pPerson)
{

}

void AIState::AerialDestructAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialDestructAttack");
}

bool AIState::AerialDestructAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::InvincibleAttack * AIState::InvincibleAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::InvincibleAttack  instance;
	return &instance;
}

void AIState::InvincibleAttack::Enter(AI * pPerson)
{
	// ���G�{�^��
	pPerson->PushInputList(PLAYER_INPUT::L2);

}

void AIState::InvincibleAttack::Execute(AI * pPerson)
{
	// ���G�X�e�[�g���I����Ă�����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) == false)
	{
		//�ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	//// InvincibleAttack��
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) == true)
	//{
	//	if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
	//	{
	//		// �ǐՃ��[�h�ɖ߂�
	//		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	//		return;
	//	}

	//}
}

void AIState::InvincibleAttack::Exit(AI * pPerson)
{

}

void AIState::InvincibleAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "InvincibleAttack");
}

bool AIState::InvincibleAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::SkillAttack * AIState::SkillAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::SkillAttack  instance;
	return &instance;
}

void AIState::SkillAttack::Enter(AI * pPerson)
{
	// �����Ō���̑���Ǝ����̃|�W�V�����ŏo���Z�����߂�
	// ���ݑ��݂����ނ́@[�ߋ���]�@[������]�@[������]�@[�ߋ�����]

	// �L�����N�^�[��X���ł̋������擾
	float l_fRange= Math::Length(TargetPlayer->GetPos().x, MyPlayer->GetPos().x);
	enum
	{
		NEAR_RANGE = 25,
		MIDDLE_RANGE = 40, 
		//FAR_RANGE = 80,
	};

	// �ꏊ�ɔ������R�}���h�����
	bool l_bActionFlag = false;
	if (l_fRange <= NEAR_RANGE)
	{	
		if (MyPlayer->isLand() == true)
		{	
			// �ߋ���
			l_bActionFlag = MyPlayer->AINearSkill();
		}
		else
		{
			// �󒆋ߋ���
			l_bActionFlag = MyPlayer->AIAerialNearSkill();
		}
	}
	else if (l_fRange <= MIDDLE_RANGE)
	{
		// ������
		l_bActionFlag = MyPlayer->AIMiddleSkill();
	}
	else
	{
		// ������
		l_bActionFlag = MyPlayer->AIFarSkill();
	}
	

	// �����ł���Z���Ȃ�����
	if (l_bActionFlag == false)
	{
		// �������Ă͂܂������̋Z���ݒ肳��Ă��Ȃ������ꍇ�ҋ@�ɖ߂�
		//�ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


}

void AIState::SkillAttack::Execute(AI * pPerson)
{
	// �ҋ@�ɖ߂��Ă���
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// �ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::SkillAttack::Exit(AI * pPerson)
{

}

void AIState::SkillAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SkillAttack");
}

bool AIState::SkillAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			���i�Z �X�e�[�g
/*******************************************************/

AIState::DokkoiAttack * AIState::DokkoiAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::DokkoiAttack  instance;
	return &instance;
}

void AIState::DokkoiAttack::Enter(AI * pPerson)
{
	// ���i�Z
	pPerson->PushInputList(PLAYER_INPUT::R1);

	// ������
	pPerson->SetWaitFrame(0);
	pPerson->SetDokkoiDush(false);

}

void AIState::DokkoiAttack::Execute(AI * pPerson)
{
	// �ҋ@�ɖ߂��Ă���
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// �ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// ���i������
	if (MyPlayer->isAttackState() == true && 
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) == true)
	{
		// ����������
		if (MyPlayer->GetAttackData()->bHit == true) 
		{	
			// �������_�b�V����
			pPerson->PushInputList(PLAYER_INPUT::R1);
			// �_�b�V���t���O���Ă�
			pPerson->SetWaitFrame(0);
			pPerson->SetDokkoiDush(true);
		}
	}

	// ���b�V����
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) == true)
	{
		// �_�b�V���t���O�������Ă�����
		if (pPerson->GetDokkoiDush() == true)
		{
			// ���̃t���[���𒴂�����
			pPerson->AddWaitFrame(1);
			if (pPerson->GetWaitFrame() >= 20)
			{
				// �L�����Z����̓��b�V����
				pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
				return;
			}

		}
	}

}

void AIState::DokkoiAttack::Exit(AI * pPerson)
{

}

void AIState::DokkoiAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "DokkoiAttack");
}

bool AIState::DokkoiAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			�C�[�W�[�A�^�b�N �X�e�[�g
/*******************************************************/

AIState::EasyAttack * AIState::EasyAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::EasyAttack  instance;
	return &instance;
}

void AIState::EasyAttack::Enter(AI * pPerson)
{
	pPerson->SetWaitFrame(0);
}

void AIState::EasyAttack::Execute(AI * pPerson)
{
	pPerson->AddWaitFrame(1);

	float fWaitRate = 70;
	
	// ��Փx�ɂ���ă��[�g����������
	if (pPerson->GetAIType() == AI_TYPE::CPU_NORMAL)fWaitRate = 40;
	if (pPerson->GetAIType() == AI_TYPE::CPU_HARD)fWaitRate = 10;
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)fWaitRate = 5;

	// �����������Ă���
	if (MyPlayer->GetHP()<= TargetPlayer->GetHP())
	{
		// HP�����Ȃ��قǋ����I
		// ���[�U�[�ɓG�����̂��銴�G�ɂ����邽�߂�
		fWaitRate *= (MyPlayer->GetHPPercentage()*0.01f);
	}
	
	if (pPerson->GetWaitFrame() >= (int)fWaitRate)
	{
		// �܂����ꂾ���҂��đ��肪�߂��ɂ�����U��
		if (GetRange(pPerson) <=  MyPlayer->AIAttackRange())
		{
			if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)
			{
				// 3���̈�Ń��b�V����
				if (tdnRandom::Get(0, 100) <= 30)
				{
					// ���b�V����
					pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
					return;
				}
				else
				{
					// �ꔭ�����Ă����
					pPerson->PushInputList(PLAYER_INPUT::C);
					pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
					return;
				}
			}
			else
			{
				// 45%�Ń��b�V����
				if (tdnRandom::Get(0, 100) <= 45)
				{
					// ���b�V����
					pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
					return;
				}
				else
				{
					// �ꔭ�����Ă����
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
					pPerson->PushInputList(PLAYER_INPUT::C);	
					pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
					return;
				}
			}


		}
		else 
		{
			// ����Ă�����X�L���Ŏ��݂Ă݂�
			pPerson->GetFSM()->ChangeState(AIState::SkillAttack::GetInstance());
			return;
		}

	}	
}

void AIState::EasyAttack::Exit(AI * pPerson)
{

}

void AIState::EasyAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "EasyAttack");
#endif // _DEBUG

}

bool AIState::EasyAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			�������X�e�[�g
/*******************************************************/

AIState::BackWark * AIState::BackWark::GetInstance()
{
	// �����ɕϐ������
	static AIState::BackWark  instance;
	return &instance;
}

void AIState::BackWark::Enter(AI * pPerson)
{
	pPerson->SetWaitFrame(0);
}

void AIState::BackWark::Execute(AI * pPerson)
{
	// ���肪�U�����ĂȂ��Ƃ�
	if (TargetPlayer->isAttackState() == false)
	{
		// �Ђ�������ɕ�������
		if (MyPlayer->GetDir() == DIR::LEFT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
	}

	pPerson->AddWaitFrame(1);
	if (pPerson->GetWaitFrame() >= 60)
	{
		// �ҋ@��
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::BackWark::Exit(AI * pPerson)
{

}

void AIState::BackWark::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "BackWark");

#endif // _DEBUG
}

bool AIState::BackWark::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}



/*******************************************************/
//			�ŋ��̕��s (�U�����Ă�����K�[�h��)�X�e�[�g
/*******************************************************/

AIState::MightyWark * AIState::MightyWark::GetInstance()
{
	// �����ɕϐ������
	static AIState::MightyWark  instance;
	return &instance;
}

void AIState::MightyWark::Enter(AI * pPerson)
{
	// ����̕����ɐi��
	if (MyPlayer->GetDir() == DIR::RIGHT)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}

	pPerson->SetWaitFrame(0);
}

void AIState::MightyWark::Execute(AI * pPerson)
{
	// ����̕����ɐi��
	if (MyPlayer->GetDir() == DIR::RIGHT)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}


	// ����̋߂��ɂ��ā������肪�U���U������
	if (MyPlayer->AIAttackRange() >= GetRange(pPerson) && 
		TargetPlayer->isAttackState() == true)
	{
		if (tdnRandom::Get(0, 1) == 0)
		{
			// �K�[�h��
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
		}else
		{
			// �����
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
		}
		return;
	}

	// �������܂�
	pPerson->AddWaitFrame(1);
	if (pPerson->GetWaitFrame() >= 20)
	{
		// �ҋ@��
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::MightyWark::Exit(AI * pPerson)
{

}

void AIState::MightyWark::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "MightyWark");

#endif // _DEBUG
}

bool AIState::MightyWark::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			�o�b�N�X�e�b�v�X�e�[�g
/*******************************************************/

AIState::BackStep * AIState::BackStep::GetInstance()
{
	// �����ɕϐ������
	static AIState::BackStep  instance;
	return &instance;
}

void AIState::BackStep::Enter(AI * pPerson)
{
	pPerson->SetWaitFrame(0);
	
	// �o�b�N�X�e�b�v
	if (TargetPlayer->GetPos().x < MyPlayer->GetPos().x)
	{
		// ����񉟂�
		pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
	}
	else
	{
		// ����񉟂�
		pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
	}

}

void AIState::BackStep::Execute(AI * pPerson)
{
	// �o�b�N�X�e�b�v
	if (TargetPlayer->GetPos().x < MyPlayer->GetPos().x)
	{
		// ����񉟂�
		pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
	}
	else
	{
		// ����񉟂�
		pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
	}

	pPerson->AddWaitFrame(1);
	if (pPerson->GetWaitFrame() >= 20)
	{
		// �ҋ@��
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// �o�b�N�X�e�b�v�ɐ���������I��
	if (MyPlayer->GetFSM()->isPrevState(*BasePlayerState::BackStep::GetInstance()) == true)
	{
		// �ҋ@��
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::BackStep::Exit(AI * pPerson)
{

}

void AIState::BackStep::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "BackStep");

#endif // _DEBUG
}

bool AIState::BackStep::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			�p�[�g�i�[�����U�� �X�e�[�g
/*******************************************************/

AIState::PartnerAttack * AIState::PartnerAttack::GetInstance()
{
	// �����ɕϐ������
	static AIState::PartnerAttack  instance;
	return &instance;
}

void AIState::PartnerAttack::Enter(AI * pPerson)
{
	// ���Q�[�W���܂����܂��Ă��Ȃ�������
	if (MyPlayer->GetStand()->GetStandStock() == 0)
	{
		// �ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// �����Ō���̑���Ǝ����̃|�W�V�����ŏo���Z�����߂�
	// ���ݑ��݂����ނ́@[�ߋ���]�@[������]�@[������]�@[�ߋ�����]

	// �L�����N�^�[��X���ł̋������擾
	float l_fRange = Math::Length(TargetPlayer->GetPos().x, MyPlayer->GetPos().x);
	enum
	{
		NEAR_RANGE = 25,
		MIDDLE_RANGE = 40,
		//FAR_RANGE = 80,
	};

	// �ꏊ�ɔ������R�}���h�����
	bool l_bActionFlag = false;
	if (l_fRange <= NEAR_RANGE)
	{
		// �ߋ���
		switch (MyPlayer->GetStand()->GetPartnerType())
		{
		case PARTNER::MOKOI:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		case PARTNER::MAYA:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		default:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		}

	}
	else if (l_fRange <= MIDDLE_RANGE)
	{
		// ������
		switch (MyPlayer->GetStand()->GetPartnerType())
		{
		case PARTNER::MOKOI:
			break;
		case PARTNER::MAYA:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		case PARTNER::END:
			break;
		default:
			break;
		}

	}
	else
	{
		// ������
		switch (MyPlayer->GetStand()->GetPartnerType())
		{
		case PARTNER::MOKOI:
			break;
		case PARTNER::MAYA:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		case PARTNER::END:
			break;
		default:
			break;
		}
	}


	// �����ł���Z���Ȃ�����
	if (l_bActionFlag == false)
	{
		// �������Ă͂܂������̋Z���ݒ肳��Ă��Ȃ������ꍇ�ҋ@�ɖ߂�
		//�ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


}

void AIState::PartnerAttack::Execute(AI * pPerson)
{
	// �ҋ@�ɖ߂��Ă���
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// �ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::PartnerAttack::Exit(AI * pPerson)
{

}

void AIState::PartnerAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "PartnerAttack");
}

bool AIState::PartnerAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}
	return false;
}
