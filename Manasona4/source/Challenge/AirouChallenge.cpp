#include "AirouChallenge.h"

//+-------------------------------
// �A�C���\�`�������W
//+-------------------------------

AirouChallenge1::AirouChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�����U��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 120;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("�u�A�ŃR���{�v�́u�U���v�𑱂��邱�ƂŁA\n�y�R���{�z���q�����Ă����V�X�e���ł��B\n����ɋ߂Â��A���{�^����A�ł��Ă݂܂��傤�I");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n���{�^����A�ł��邾���Łu�X�L���v�Ɍq����A\nSP�Q�[�W������΁A�����I�Ɂu�K�E�Z�v�܂Ōq���Ă���܂�\n���߂Ă̕��́A�܂��u�A�ŃR���{�v�Ő���Ă݂܂��傤�B");

	// �^�X�N�Z�b�g
	AddTaskData("�W�����v�U���|�󒆂Ł�");
	AddTaskData("�U���|��");
	AddTaskData("�ǌ��|��");
	AddTaskData("�X�L���|��");
	AddTaskData("�K�E�Z�|��");

	Init();
}

void AirouChallenge1::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// ���ꂼ��̃N���A����
void AirouChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		AERIAL_ATTACK = 0,
		ATTACK = 1,
		ATTACK2 = 2,
		SKIL = 3,
		OVER_DRIVE = 4,

	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{
		// �󒆍U���𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// ��i�ڂ𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 0)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ATTACK);
			}

		}

		// ��i�ڂ𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 1)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ATTACK2);
			}

		}

		// �O�i�ڂ𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 2)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(SKIL);
			}

		}

		// �K�E�Z�X�e�[�g�ɂ�����N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(OVER_DRIVE);
			}

		}

	}
}

