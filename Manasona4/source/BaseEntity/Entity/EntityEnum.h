#pragma once

//********************************************************
//	�Q�[����̒萔����
//********************************************************

#define GAME_PERSON_MAX 64		//�Q�[���o�Ă���l�̍ő吔

#define GAME_ID_MAX 500

namespace GAME_ID
{
	enum _ENUM_ENTITY_ID
	{
		ID_ERROR,					//�G���[ID

		PERSON_MNG,					// �}�l�[�W���[��ID 
		JUDGE_MNG,					// �W���b�W�}�l�[�W���[ID
		CAMERA_MGR,					// �J�����}�l�[�W���[ID

		id_person_first,			//�l��ID�̏���
		id_person_max = id_person_first + GAME_PERSON_MAX,	//�l�̍ő�ID+1

	};
}
typedef GAME_ID::_ENUM_ENTITY_ID	ENTITY_ID;	//�Q�[����ID

