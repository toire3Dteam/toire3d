#pragma once

//********************************************************
//	�Q�[����̒萔����
//********************************************************

#define GAME_PLAYER_MAX 2		//�Q�[���o�Ă���l�̍ő吔

#define GAME_ID_MAX 500

namespace GAME_ID
{
	enum _ENUM_ENTITY_ID
	{
		ID_ERROR,					//�G���[ID
		SCENE_MAIN,					// �V�[�����C��
		SCENE_MENU,					// �V�[�����j���[
		SCENE_SELECT,				// �V�[���L�����Z��
		SCENE_COLLECT,				// �V�[���R���N�g
		SCENE_TITLE,				// �V�[���^�C�g��
		PLAYER_MGR,					// �}�l�[�W���[��ID 
		JUDGE_MNG,					// �W���b�W�}�l�[�W���[ID
		CAMERA_MGR,					// �J�����}�l�[�W���[ID
		UI_MGR,						// UI�}�l�[�W���[
		CUTIN_MGR,					// �J�b�g�C���}�l�[�W���[
		ROUND_CALL_MGR,				// ���E���h�R�[���}�l�[�W���[
		SHOT_MGR,					// �V���b�g�}�l�[�W���[
		SELECT_UI_LEFT,				// �L�����N�^�[�Z���N�g��UI��
		SELECT_UI_RIGHT,			// �L�����N�^�[�Z���N�g��UI�E
		TROPHY_MGR,					// �g���t�B�[�}�l�[�W���[
		PICTURE_MGR,				// �s�N�`���[�}�l�[�W���[
		MOVIE_MGR,					// ���[�r�[�}�l�[�W���[
		ID_PLAYER_FIRST,			//�l��ID�̏���
		ID_PLAYER_MAX = ID_PLAYER_FIRST + GAME_PLAYER_MAX,	//�l�̍ő�ID+1

		ID_AI_FIRST,			//AI��ID�̏���
		ID_AI_MAX = ID_AI_FIRST + GAME_PLAYER_MAX,	//AI�̍ő�ID+1

	};
}
typedef GAME_ID::_ENUM_ENTITY_ID	ENTITY_ID;	//�Q�[����ID