#pragma once
#include "MenuIcon.h"

//+--------------------------
//	���j���[UI�}�l�[�W���[
//+--------------------------
 
// �A�C�R���̎��
enum class MENU_ICON_TYPE
{
	TUTORIAL,
	BATTLE,
	TRAINING,
	COLLECT,
	OPTION,
	ARRAY_END
};



class MenuUIManager
{
public:
	MenuUIManager();
	~MenuUIManager();

	void Update();
	void Move();
	void Render();
	void Action();

	void ActionFall();	// ����A�N�V����[�I����2��]
	void ActionUp();	// �o��A�N�V����[�I����1��]

	void InfoStop();	// ��������S�ď���
	void InfoAction();	// ��������\��

	// �Z���N�g�i���o�[�ύX
	//void AddSelectNo(int add) { m_iSelectNo += add; }
	//void SetSelectNo(int set) { m_iSelectNo = set; }
	void ChangeSelectNo(int setSelectNo);

	// �A�N�Z�T
	int GetMaxIcon() { return (int)MENU_ICON_TYPE::ARRAY_END; }

	MENU_ICON_TYPE GetIconType()
	{
		return (MENU_ICON_TYPE)m_iSelectNo;
	}

private:
	
	BaseMenuIcon* m_pIcon[(int)MENU_ICON_TYPE::ARRAY_END];

	int m_iSelectNo;	// �Z���N�g�i���o�[

	float m_fIconWidth;		// �A�C�R���̉���
	float m_fIconHeight;	// �A�C�R���̏c��

	// ��
	tdn2DObj* m_pblueLine;
	int m_iBlueLineY;
	int m_iNextBlueLineY;

	tdn2DAnim* m_pBlackLine;

	// �����g
	tdn2DAnim* m_pInfo;
	int m_iInfoY;

	// ��
	tdn2DObj* pExp;

	// ����
	tdn2DObj* m_pMove;
	tdnMovie*  m_pMoveMgr;

	
};
