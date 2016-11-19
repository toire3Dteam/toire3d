#pragma once
#include "TDNLIB.h"
#include "SelectUI.h"

//+-----------------------------------
//	�L�����N�^�[�Z���N�g�}�l�[�W���[
//+-----------------------------------

class SelectUIManager
{
public:
	SelectUIManager();
	~SelectUIManager();

	void  Update();
	void  Render();

	void FirstAction();// �ŏ��̉��o

	// �����I��������
	bool IsOK()
	{
		if (m_pLeftSide->IsOK() == true &&
			m_pRightSide->IsOK() == true)
		{
			return true;
		}
		else
		{
			return false;
		}

	};

	// ���������݂��̑I�������L�����ƃp�[�g�i�[���擾����A�N�Z�T
	int GetSelectCharacter(SIDE side) 
	{
		if (side == SIDE::LEFT)
		{
			return m_pLeftSide->GetSelectCharacter();
		}
		else
		{
			return m_pRightSide->GetSelectCharacter();
		}
	}

	int GetSelectPartner(SIDE side)
	{
		if (side == SIDE::LEFT)
		{
			return m_pLeftSide->GetSelectPartner();
		}
		else
		{
			return m_pRightSide->GetSelectPartner();
		}
	}


private:
	// (TODO) ���T�C�h���@SelectUI
	SelectUI* m_pLeftSide;
	SelectUI* m_pRightSide;


	// ���ʂ̉摜
	// �摜
	//enum PIC_TYPE
	//{
	//	BG,
	//	TITLE,
	//	BLACK_LINE,
	//	INFO_PLATE,
	//	//INFO_BOX_LEFT,
	//	//INFO_BOX_RIGHT,
	//	ARRAY_END,
	//};
	//tdn2DAnim* m_pPic[PIC_TYPE::ARRAY_END];

	// �L�����N�^�[���̃A�C�R��
	tdn2DAnim* m_pCharacterIcon[(int)CHARACTER::END];


};
