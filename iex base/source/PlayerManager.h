#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

//******************************************************************************
//
//
//		Player�Ǘ��N���X(Singleton)
//
//
//******************************************************************************

class PlayerManager
{
private:

	/* �v���C���[���� */
	BasePlayer **players;

	/* �v���C�l�� */
	static int max_player;


	PlayerManager(){}
	PlayerManager(const PlayerManager &pm){}
	PlayerManager &operator=(const PlayerManager &pm){}

public:


	//=========================================
	//	�C���X�^���X�擾
	static PlayerManager *GetInstance()
	{
		static PlayerManager instance;
		return &instance;
	}


	//=========================================
	//	�������Ɖ��
	void Initialize();
	void Release();


	//=========================================
	//	�X�V�ƕ`��
	void Update();
	void Render();

};


#endif