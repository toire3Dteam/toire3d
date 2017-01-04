
#include	"AnimationUV.h"
#include "system\System.h"

/*
*
*	UV�A�j���[�V�����N���X
*
*/

// �ߋ��̎d�l
//AnimationUV::AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool AlphaFlag, int AlphaNear, bool IsRoop)
//{
//	obj = new iexMesh(name);
//
//	tu = 0.0f; tv = 0.0f;
//	moveTu = moveTU, moveTv = moveTV;
//	endFlame = EndFlame;
//	nowFlame = 0;			// �����t���[����0��
//	isAction = false;		// �A�j�����s���邩
//
//	// ��
//	alphaFlag = AlphaFlag;
//	alphaNear = AlphaNear;
//	alpha = 1.0f;
//
//	isRoop = IsRoop;
//}

// �Ȉ�ver
AnimationUV::AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool IsRoop )
{

	obj = new iexMesh(name);

	tu = 0.0f; tv = 0.0f;
	moveTu = moveTU, moveTv = moveTV;
	endFlame = EndFlame;
	nowFlame = 0;			// �����t���[����0��
	isAction = false;		// �A�j�����s���邩

	// ��
	alphaFlag = false;
	alphaNear = 0;
	alphaFar = EndFlame; // �K��
	alpha = 1.0f;

	isRoop = IsRoop;

	isEnd = false;

	// �I�u�W�F�N�g�X�e�[�^�X
	m_scale = 1.0f;
	m_startScale = 1.0f;
	m_endScale = 1.0f;

    m_angle = VECTOR_ZERO;
    m_startAngle = VECTOR_ZERO;
    m_endAngle = VECTOR_ZERO;
}

// ���l��
AnimationUV::AnimationUV(char* name, float moveTU, float moveTV, int EndFlame, bool IsRoop, int AlphaNear, int AlphaFar)
{
	obj = new iexMesh(name);

	tu = 0.0f; tv = 0.0f;
	moveTu = moveTU, moveTv = moveTV;
	endFlame = EndFlame;
	nowFlame = 0;			// �����t���[����0��
	isAction = false;		// �A�j�����s���邩

	// ��
	alphaFlag = true;
	alphaNear = AlphaNear;
	alphaFar = AlphaFar;
	alpha = 1.0f;

	isRoop = IsRoop;

	isEnd = false;

	// �I�u�W�F�N�g�X�e�[�^�X
	m_scale = 1.0f;
	m_startScale  = 1.0f;
	m_endScale   =	1.0f;

	// [10/07��]�A���O��������
	m_angle = VECTOR_ZERO;
	m_startAngle = VECTOR_ZERO;
	m_endAngle = VECTOR_ZERO;
}


AnimationUV::~AnimationUV()
{
	delete obj;
}


void  AnimationUV::Action()
{
	isAction = true;//�N��
	tu = 0.0f; tv = 0.0f;// ������uv�ɖ߂�
	nowFlame = 0;// �ŏ��̃t���[����

	isEnd = false;
}

void AnimationUV::Stop()
{
	isAction = false;//�~�߂�
	nowFlame = 0;
};

void AnimationUV::ActionRoop()	// ���[�v�A�j�����s
{
	isAction = true;		// �N��
	tu = 0.0f; tv = 0.0f;	// ������uv�ɖ߂�
	nowFlame = 0;			// �ŏ��̃t���[����
	isRoop = true;			// ���[�v
}

// ���[�v�X�g�b�v
void AnimationUV::StopRoop()	// ���[�v�A�j���X�g�b�v
{
	isRoop = false;				// ���[�v�X�g�b�v
	nowFlame = alphaFar + 1;	// �����ɂȂ镔���փ��[�v
}

void AnimationUV::Update(Vector3 pos)
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// UV�X�V
	tu += moveTu;
	tv += moveTv;

	//if (isRoop == false)//���[�v����Ȃ�������
	{
		// �t���[���X�V
		nowFlame++;
		if (isRoop == true)// ���[�v��������alphaFar�̑O�̂Ƃ��Ń��[�v�������鏈��
		{
			if (nowFlame >= (alphaFar - 1))
			{
				nowFlame = (alphaFar-1);
			}
		}

		if (nowFlame >= endFlame)
		{
			isAction = false;

			// �ǉ�
			isEnd = true;
		}
	}

	// �����x�X�V
	if (alphaFlag == true)
	{
		//�����_��Min~Max�͈̔͂ɗ}����@�Q�`�O
		auto Clamp = [](float val, float Min, float Max){
			return min(Max, max(val, Min));
		};

		// 0�������@1���s�����ɂ���悤�ɐݒ�
		//float A = (endFlame - nowFlame);
		//float B =(endFlame - alphaNear);
		//alpha = A / B;
		//alpha = Clamp(alpha, 0.0f, 1.0f);

		// ����A�[�̑O�����Ŕ����ς���
		if (nowFlame >= alphaNear)
		{		
		// 100-100=0  100-50=50   0/50
		float A = (float)(endFlame - nowFlame);
		float B = (float)(endFlame - alphaFar);
		alpha = A / B;
		//alpha = Clamp(alpha, 0.0f, 1.0f);

		}
		else
		{
		// �ŏ���
		alpha = (float)nowFlame / (float)alphaNear;//   0/30=0   60/30=2   1-(0~1)  

		}

		//alpha = (alphaFar - nowFlame) / (alphaFar - alphaNear);
		alpha = Clamp(alpha, 0.0f, 1.0f);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�

		
	}

	// �X�P�[���X�V
	float scareRate = (float)(nowFlame) / (float)(endFlame); // �ŏ��Ƀ��[�g���o��
	float calcScare = (m_startScale * (1.0f - scareRate)) + (m_endScale * scareRate);

    // ��]�X�V
    float angleRate = (float)(nowFlame) / (float)(endFlame); // �ŏ��Ƀ��[�g���o��
    Vector3 calcAngle = (m_startAngle * (1.0f - angleRate)) + (m_endAngle * angleRate);


	// ���b�V���̍X�V
	obj->SetScale(calcScare);
    obj->SetAngle(calcAngle);
	obj->SetPos(pos);
	obj->Update();


}

void AnimationUV::Update(Vector3 pos, Vector3 angle, Vector3 scale)
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// UV�X�V
	tu += moveTu;
	tv += moveTv;

	if (isRoop == false)//���[�v����Ȃ�������
	{
		// �t���[���X�V
		nowFlame++;
		if (nowFlame >= endFlame)
		{
			isAction = false;
		}
	}

	// �����x�X�V
	if (alphaFlag == true)
	{
		//�����_��Min~Max�͈̔͂ɗ}����@�Q�`�O
		auto Clamp = [](float val, float Min, float Max){
			return min(Max, max(val, Min));
		};
		// ����A�[�̑O�����Ŕ����ς���
		if (nowFlame >= alphaNear)
		{
			// 100-100=0  100-50=50   0/50
			float A = (float)(endFlame - nowFlame);
			float B = (float)(endFlame - alphaFar);
			alpha = A / B;
			//alpha = Clamp(alpha, 0.0f, 1.0f);

		}
		else
		{
			// �ŏ���
			alpha = (float)nowFlame / (float)alphaNear;//   0/30=0   60/30=2   1-(0~1)  

		}

		//alpha = (alphaFar - nowFlame) / (alphaFar - alphaNear);
		alpha = Clamp(alpha, 0.0f, 1.0f);//�w�肳�ꂽ�l�� 0 �` 1 �͈̔͂ɃN�����v���܂�


	}

	// ���b�V���̍X�V
	obj->SetScale(scale);
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->Update();

}

void AnimationUV::Render()
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// �`��̑O�ɏ��𑗂�
	//�@�V�F�[�_�[�ɕ`�斈UV���W�𑗂�
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// �����x

	// �`��
	obj->Render(shaderM, "uvAnime");


}

void AnimationUV::Render_ADD()
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// �`��̑O�ɏ��𑗂�
	//�@�V�F�[�_�[�ɕ`�斈UV���W�𑗂�
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// �����x

	// �`��
	obj->Render(shaderM, "uvAnime_add");

}

void AnimationUV::Render_ADD_NOZ()
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// �`��̑O�ɏ��𑗂�
	//�@�V�F�[�_�[�ɕ`�斈UV���W�𑗂�
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// �����x

	// �`��
	obj->Render(shaderM, "uvAnime_add_noz");

}

// �K�[�h�p�`��
void AnimationUV::Render_Guard()			
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	// �`��̑O�ɏ��𑗂�
	//�@�V�F�[�_�[�ɕ`�斈UV���W�𑗂�
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// �����x

	// �`��
	obj->Render(shaderM, "uvAnime_guard");

}

// �G���A�̕Ǘp
void AnimationUV::Render_AreaWall()
{
	if (isAction == false)return;//���s����ĂȂ��Ȃ�o�Ă��I�I

	 // �`��̑O�ɏ��𑗂�
	 //�@�V�F�[�_�[�ɕ`�斈UV���W�𑗂�
	shaderM->SetValue("tuAnime", tu);
	shaderM->SetValue("tvAnime", tv);
	shaderM->SetValue("alphaUV", alpha);// �����x

										// �`��
	obj->Render(shaderM, "uvAnime_areaWall");
}


// �g��A�j��
void AnimationUV::ScaleAnimation(float StartScale, float EndScale)
{
	m_startScale = StartScale;
	m_endScale = EndScale;
}

// ��]�A�j��
void AnimationUV::AngleAnimation(Vector3 StartAngle, Vector3 EndAngle)
{
    m_startAngle = StartAngle;
    m_endAngle = EndAngle;
}
