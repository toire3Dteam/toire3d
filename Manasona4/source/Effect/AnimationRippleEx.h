#pragma once
#include "tdnlib.h"
#include "../system/System.h"

class AnimationRippleEx
{
public:
	AnimationRippleEx(char* name, int endFlame, int alphaNearFlame, int alphaFarFlame, float StartScale, float ScaleMove, bool isRoop = false);
	~AnimationRippleEx();

	void Action();
	void Stop();

	void Update();
	void Render(Vector3 pos, DWORD dwFlag);
	void Render(float x, float y, DWORD dwFlag);

	bool isDoing(){ return isAction; }
	tdn2DObj* GetObj(){ return obj; };
private:
	tdn2DObj* obj;
	
	//Vector3 pos;

	int			endFlame;		// �`��I��̃t���[��
	int			nowFlame;		// ���̃t���[��

	int			alphaNear;		// ��������Ȃ��Ȃ�@�n�߂�t���[��
	int			alphaFar;		// ��������Ȃ��Ȃ�@�I���t���[���@
	float		alpha;			// �����x

	float scale;				// �X�P�[��
	float startScale;			// �ŏ��̃X�P�[��
	float scaleMove;			// �X�P�[�����ǂ���������

	bool isAction;
	bool isRoop;				// ���[�v���邩
};
