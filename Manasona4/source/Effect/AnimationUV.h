#pragma once
#include "TDNLIB.h"
#include "../system/System.h"

/*
*UV�A�j���[�V�����N���X
*/

/* �g���� */
/* �EUV�A�j���[�V�����������������b�V����ǂݍ���				*/
/* �E�X�V��Update�֐��E�`���Render�֐������ꂼ��u��			*/
/* �E�Ō��Action�֐������s�����UV�A�j���[�V�������n�܂�܂��B	*/

class AnimationUV
{
public:
	// �ߋ��̎d�l
	// ���b�V���̖��O�@1�t���[���ɓ���X��Y�@�I���t���[���@�Ō�ɓ����ɂȂ邩�̃t���O�@�����ɂȂ��Ă����n�܂�̃t���[���@���[�v�����邩
	//AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool AlphaFlag = false, int AlphaNear = 0, bool IsRoop = false);
	
	// ���݂̎d�l
	AnimationUV(char* name, float moveTU, float moveTV, int EndFlame,bool IsRoop = false);
	AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool IsRoop, int AlphaNear, int AlphaFar);
	~AnimationUV();

	void Action();		// �A�j�����s
	void Stop();		// �A�j����~

	void ActionRoop();  // ���[�v�A�j�����s
	void StopRoop();	// ���[�v�A�j���X�g�b�v

	void Update(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 angle = Vector3(0.0f, 0.0f, 0.0f), float scale = 1.0f);	// �X�V
	void Update(Vector3 pos ,Vector3 angle , Vector3 scale);			// �X�V�X�P�[����vector�^�ɕς�������
	void Render();			// �`��
	void Render_ADD();			// ��Z�`��

	// �Q�b�^�[�E�Z�b�^�[
	iexMesh* GetObj(){ return obj; }

	bool IsAction() { return isAction; }
	bool IsEnd() { return isEnd; }
private:
	iexMesh*	obj;
	float		tu, tv;			// ���t���[���̉��c��UV�̍��W
	float		moveTu, moveTv;	// ���t���[���̉��c��UV�̓���
	int			endFlame;		// �`��I��̃t���[��
	int			nowFlame;		// ���̃t���[��

	bool		isAction;		// �A�j�������s���Ă��邩
	bool		isRoop;			// ���[�v�G�t�F�N�g��

	bool		alphaFlag;		// �Ō㓧���ɂ���t���O
	int			alphaNear;		// ��������Ȃ��Ȃ�@�n�߂�t���[��
	int			alphaFar;		// ��������Ȃ��Ȃ�@�I���t���[���@
	float		alpha;			// �����x

	bool		isEnd;		// �����
};
