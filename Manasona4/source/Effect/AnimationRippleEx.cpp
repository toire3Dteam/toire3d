
#include "AnimationRippleEx.h"

AnimationRippleEx::AnimationRippleEx(char* name, int EndFlame, int alphaNearFlame, int alphaFarFlame, float StartScale, float ScaleMove, bool IsRoop)
{
	obj = new tdn2DObj(name);

	//pos = VECTOR_ZERO;
	isAction = false;
	nowFlame = 0;
	endFlame = EndFlame;

	alphaNear = alphaNearFlame;
	alphaFar = alphaFarFlame;
	alpha = 0.0f;

	// �X�P�[��
	scale = 1.0f;
	startScale = StartScale;
	scaleMove = ScaleMove;

	isRoop = IsRoop; //���[�v���邩
}

AnimationRippleEx::~AnimationRippleEx()
{
	delete obj;
}

void AnimationRippleEx::Action()
{
	isAction = true;
	nowFlame = 0;
	scale = startScale;// �X�P�[���������ʒu��
}

void AnimationRippleEx::Stop()
{
	isAction = false;
	nowFlame = 0;
}

void AnimationRippleEx::Update()
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I


	// �t���[���X�V
	nowFlame++;
	if (isRoop == true)// ���[�v��������alphaFar�̑O�̂Ƃ��Ń��[�v�������鏈��
	{
		if (nowFlame >= (alphaFar - 1))
		{
			nowFlame = (alphaFar - 1);
			return;
		}
	}
	// �G���h�t���[���܂ŗ�����I���
	if (nowFlame >= endFlame)
	{
		isAction = false;
	}

	// �g�嗦�X�V
	scale += scaleMove;


	// �����x�X�V

	//�����_��Min~Max�͈̔͂ɗ}����@�Q�`�O
	auto Clamp = [](float val, float Min, float Max){
		return min(Max, max(val, Min));
	};
	// ����A�[�̑O�����Ŕ����ς���
	if (nowFlame >= alphaNear)
	{
		// 100-100=0  100-50=50   0/50
		float A = ((float)endFlame - nowFlame);
		float B = ((float)endFlame - alphaFar);
		alpha = A / B;
	}
	else
	{
		// �ŏ���
		alpha = (float)nowFlame / (float)alphaNear;//   0/30=0   60/30=2   1-(0~1)  
	}

	alpha = Clamp(alpha, 0.0f, 1.0f);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�

	obj->SetScale(scale);
	obj->SetARGB((int)(alpha * 255), 255, 255, 255);

}

void AnimationRippleEx::Render(Vector3 pos, DWORD dwFlag)
{
	if (isAction == false)
	{
		int i = 0;
		return;//���s����ĂȂ��Ȃ�o�Ă��I�I
	}

	obj->Render3D(pos, dwFlag);
}

void AnimationRippleEx::Render(float x, float y, DWORD dwFlag)
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	obj->Render((int)x, (int)y, dwFlag);
}