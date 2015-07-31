#include<windows.h>
#include<assert.h>

#include	"OKB.h"

OwatasoKeyBoard::OwatasoKeyBoard()
{
	// �L�[�z�񏉊���
	for (int i = 0; i < 256; i++)
		info[i] = on_frame[i] = 0;
}

void OwatasoKeyBoard::Update()
{
	// �擾�ł������ǂ������m�F & info�ɃL�[���͏��i�[
	BOOL ret = GetKeyboardState(info);

	// �擾�ł��Ȃ������狭���I��(����~�܂�悤�ł���΁A�R�����g�A�E�g���Ă�������)
	assert(ret);

	//=========================
	//	�S�L�[

	for (int index = 0; index < 240; index++){	// 240�ȍ~�A�L�v�ȃL�[�͂Ȃ��Ǝv���c

		// �Ή��L�[��������Ă���Ȃ�
		if (info[index] & 0x80)
			on_frame[index]++;		// ���̔ԍ��̓��̓t���[�������Z

		else
			on_frame[index] = 0;	// �����łȂ��Ȃ�t���[��0�ɂ���(������Ă��Ȃ�)
	}


	//=========================
	// ��{�L�[�̂�
	//BYTE key_list[] =
	//{
	//	KB_ENTER, KB_SPACE,
	//	KB_0, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9,
	//	KB_A, KB_B, KB_C, KB_D, KB_E, KB_F, KB_G, KB_H, KB_I, KB_J, KB_K, KB_L, KB_M, KB_N, KB_O, KB_P, KB_Q, KB_R, KB_S, KB_T, KB_U, KB_V, KB_W, KB_X, KB_Y, KB_Z,
	//	KB_COMMA, KB_DOT, KB_HYPHEN, KB_COLON, KB_SEMICORON, KB_UP, KB_DOWN, KB_LEFT, KB_RIGHT,
	//	0x00
	//};

	//for (int index = 0; key_list[index] != 0x00; index++){

	//	// �Ή��L�[��������Ă���Ȃ�
	//	if (info[key_list[index]] & 0x80)
	//		on_frame[key_list[index]]++;		// ���̔ԍ��̓��̓t���[�������Z

	//	else
	//		on_frame[key_list[index]] = 0;	// �����łȂ��Ȃ�t���[��0�ɂ���(������Ă��Ȃ�)

	//}


}

BYTE OwatasoKeyBoard::AnyTRG()
{
	//=========================
	//	�S�L�[

	for (int index = 0; index < 240; index++)	// 240�ȍ~�A�L�v�ȃL�[�͂Ȃ��Ǝv���c
	{
			// �����̃L�[���͎��Ԃ�1�t���[���ڂ������炻���Ԃ�
		if (on_frame[info[index]] == 1)
			return index;
	}

	//=========================
	// ��{�L�[�̂�
	//BYTE key_list[] =
	//{
	//	KB_ENTER, KB_SPACE,
	//	KB_0, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9,
	//	KB_A, KB_B, KB_C, KB_D, KB_E, KB_F, KB_G, KB_H, KB_I, KB_J, KB_K, KB_L, KB_M, KB_N, KB_O, KB_P, KB_Q, KB_R, KB_S, KB_T, KB_U, KB_V, KB_W, KB_X, KB_Y, KB_Z,
	//	KB_COMMA, KB_DOT, KB_HYPHEN, KB_COLON, KB_SEMICORON,
	//	0x00
	//};

	//for (int index = 0; key_list[index] != 0x00; index++){

	//	// �����̃L�[���͎��Ԃ�1�t���[���ڂ������炻���Ԃ�
	//	if (on_frame[key_list[index]] == 1)
	//		return key_list[index];
	//}

	// ���̃t���[���ŐV���������ꂽ�L�[�͂Ȃ������̂�0(�L�[�R�[�h�Ȃ�)��Ԃ�
	return 0x00;

}


static OwatasoKeyBoard *key_board;

//*****************************************************************************************************************************
//		���͊֘A�������E���
//*****************************************************************************************************************************
void OKB_Init()
{
	key_board = new OwatasoKeyBoard;
}

void OKB_Release()
{
	if (key_board){
		delete key_board;
		key_board = nullptr;
	}
}


//*****************************************************************************************************************************
//		�X�V
//*****************************************************************************************************************************

void OKB_Update()
{
	key_board->Update();
}


//*****************************************************************************************************************************
//		�L�[�A�N�Z�X�֐�
//*****************************************************************************************************************************

UINT KeyBoard(BYTE KeyCode){ return key_board->on_frame[KeyCode]; }
bool KeyBoardTRG(BYTE KeyCode, UINT frame) { return (key_board->on_frame[KeyCode] == frame); }

BYTE KeyBoardAnyTRG(){ return key_board->AnyTRG(); }