#pragma once

//********************************************************
//	�Q�[����̒萔����
//********************************************************

#define GAME_PLAYER_MAX 4		//�Q�[���o�Ă���l�̍ő吔

#define GAME_ID_MAX 500

namespace GAME_ID
{
	enum _ENUM_ENTITY_ID
	{
		ID_ERROR,					//�G���[ID

		PLAYER_MGR,					// �}�l�[�W���[��ID 
		JUDGE_MNG,					// �W���b�W�}�l�[�W���[ID
		CAMERA_MGR,					// �J�����}�l�[�W���[ID

		ID_PLAYER_FIRST,			//�l��ID�̏���
		ID_PLAYER_MAX = ID_PLAYER_FIRST + GAME_PLAYER_MAX,	//�l�̍ő�ID+1
	};
}
typedef GAME_ID::_ENUM_ENTITY_ID	ENTITY_ID;	//�Q�[����ID

