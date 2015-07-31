#include	"iextreme.h"

#include	"BasePlayer.h"
#include	"Airou.h"
#include	"PlayerManager.h"




//******************************************************************************
//
//		�O���[�o���ϐ�
//
//******************************************************************************

int PlayerManager::max_player = 0;		// static�����o�ϐ��̎���




//******************************************************************************
//
//		�������Ɖ��
//
//******************************************************************************
void PlayerManager::Initialize()
{
	PlayerManager::max_player = 4;	// ��

	players = new BasePlayer*[PlayerManager::max_player];
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		players[i] = new Airou(i);		// ���̓A�C���[
	}
}

void PlayerManager::Release()
{
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		SAFE_DELETE(players[i]);
	}
	delete[] players;		// �R�������Ȃ��ƃ��������[�N����B
}


//******************************************************************************
//
//		�X		�V
//
//******************************************************************************
void PlayerManager::Update()
{
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		players[i]->Update(i);

		if (players[i]->isDied())	// ����ł��畜�������Ă�����
			players[i]->Respawn(i);
	}
}


//******************************************************************************
//
//		�`		��
//
//******************************************************************************
void PlayerManager::Render()
{
	for (int i = 0; i < PlayerManager::max_player; i++)
	{
		players[i]->Render();
	}
}