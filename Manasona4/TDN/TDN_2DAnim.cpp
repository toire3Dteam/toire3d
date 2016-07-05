#include	"TDNLIB.h"


/****************************************************/
//	2DObj�����b�v���ėl�X�ȓ���@�\���������o
/****************************************************/

// �A�N�V�����`�F�b�N
bool AnimAction::Base::ActionCheck() 
{

	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	if (m_bActionFlag == false) return false;
	
	// ?
	// �f�B���C�^�C�}�[��0�ɂȂ�܂Œʂ��Ȃ�
	if (m_iDelayFlame > 0)
	{
		m_iDelayFlame--;
		return false;
	}
	
	return true; //����
}

/******************************************/
// �A�j���p�`��
/******************************************/
// 2D
void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	
	pic->Render(x, y, dwFlags);
}

void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(x, y, shader, name);
}

void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(x, y, w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::Base::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(x, y, w, h, tx, ty, tw, th, shader, name);
}

// 3D
void AnimAction::Base::Render3D(tdn2DObj* pic, float x, float y, float z, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render3D(x, y, z, dwFlags);
}

void AnimAction::Base::Render3D(tdn2DObj* pic, Vector3 pos, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render3D(pos, dwFlags);
}

void AnimAction::Base::Render3D(tdn2DObj* pic, float x, float y, float z, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render3D(x, y, z, w, h, tx, ty, tw, th, dwFlags);
}

void AnimAction::Base::Render3D(tdn2DObj* pic, Vector3 pos, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render3D(pos, w, h, tx, ty, tw, th, dwFlags);
}



/**********************/
// �g��
/**********************/

AnimAction::Ripple::Ripple(int endFlame, float startScale, float moveScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startscale = startScale;
	m_movescale = moveScale;

}

AnimAction::Ripple::~Ripple()
{

}

void AnimAction::Ripple::Update(tdn2DObj * pic)
{
	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	//if (m_bActionFlag == false)return;
	if (ActionCheck() == false)return;

	// �t���[���X�V
	m_nowFlame++;
	// �G���h�t���[���܂ŗ�����I���
	if (m_nowFlame >= m_endFlame)
	{
		m_bActionFlag = false;
		m_bEndFlag = true; // �I��t���OON
		// �g�嗦�X�V
		//pic->SetScale(m_startscale);
	}

	// �A���t�@����
	float alpha = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)  
	alpha = 1.0f - alpha;
	pic->SetARGB((int)(alpha * 255), 255, 255, 255);

	// �g�嗦�X�V
	pic->SetScale(pic->GetScale() + m_movescale);


}

void AnimAction::Ripple::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // ���s�t���OOn

	// ������
	m_nowFlame = 0;
	pic->SetScale(m_startscale);

}


/******************************************/
// �ړ����Ȃ���o��
/******************************************/

AnimAction::MoveAppeared::MoveAppeared(int endflame, int startX, int startY)
{
	m_nowFlame = 0;
	m_endFlame= endflame;

	m_startX = startX;
	m_startY = startY;

	m_rate = 0.0f;
}

AnimAction::MoveAppeared::~MoveAppeared()
{

}

void AnimAction::MoveAppeared::Update(tdn2DObj * pic)
{
	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	if (ActionCheck() == false)return;

	// �t���[���X�V
	m_nowFlame++;
	// �G���h�t���[���܂ŗ����炻�̂܂ܑҋ@
	if (m_nowFlame >= m_endFlame)
	{
		//m_bActionFlag = false;
		m_nowFlame = m_endFlame;

		m_bEndFlag = true; // �I��t���OON
	}
	//else
	//{

	//}

	// �A���t�@����
	m_rate = (float)m_nowFlame / (float)m_endFlame;//   0~�Ȃɂ���
	Math::Clamp(m_rate, 0.0f, 1.0f);//   0~1.0f��

	pic->SetARGB((int)(m_rate * 255), 255, 255, 255);

	

}

void AnimAction::MoveAppeared::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);

	// ������
	m_nowFlame = 0;
	m_rate = 0.0f;
}

// 2D
void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(
		(int)(Math::Blend(m_rate,(float)m_startX, (float)x)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		dwFlags);
}

void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(
		(int)(Math::Blend(m_rate, (float)m_startX, (float)x)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		shader, name);
	//����������
}

void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(
		(int)(Math::Blend(m_rate, (float)m_startX, (float)x)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::MoveAppeared::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	if (m_bActionFlag == false)return;
	if (m_iDelayFlame > 0)return;
	pic->Render(
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		(int)(Math::Blend(m_rate, (float)m_startY, (float)y)),
		w, h, tx, ty, tw, th, shader, name);
}

/**********************/
// �V�F�C�N
/**********************/

AnimAction::Shake::Shake(int endFlame, float x, float y, int cycle)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_shakeX = x / 2.0f;
	m_shakeY = y / 2.0f;
	m_cycle = cycle;
	m_powerX = x / cycle; //10/1=10 10/2=5;
	m_powerY = y / cycle; //10/1=10 10/2=5;
	
	m_orgX = x;
	m_orgY = y;

	m_bTrunOver = false;
}

AnimAction::Shake::~Shake()
{

}

void AnimAction::Shake::Update(tdn2DObj * pic)
{
	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	//if (m_bActionFlag == false)return;
	if (ActionCheck() == false)return;

	// �t���[���X�V
	m_nowFlame++;
	// �G���h�t���[���܂ŗ�����I���
	if (m_nowFlame >= m_endFlame)
	{
		m_bActionFlag = false;
		m_bEndFlag = true; // �I��t���OON
		m_shakeX = (m_orgX / 2.0f);
		m_shakeY = (m_orgY / 2.0f);
		return;
	}

	// �T�C�N�������@���̃T�C�N���܂łɍő哞�B�_�ɂ���
	//float rate = m_shakeX / m_cycle; //10/1=10 10/2=5

	// 0�ɂȂ�����t���O�`�F���W�I
	if (m_nowFlame%m_cycle == 0)
	{
		if (m_bTrunOver)
		{
			m_bTrunOver = false;
		}
		else
		{
			m_bTrunOver = true;
		}
	}
	
	// �X�V
	if (m_bTrunOver)
	{
		// �X�V
		m_shakeX -= m_powerX;
		m_shakeY -= m_powerY;
		if (m_shakeX <= 0)m_shakeX = 0;
		if (m_shakeY <= 0)m_shakeY = 0;
	}
	else
	{
		// �X�V
		m_shakeX += m_powerX;
		m_shakeY += m_powerY;

		if (m_shakeX >= m_orgX)m_shakeX = m_orgX;
		if (m_shakeY >= m_orgY)m_shakeY = m_orgY;
	}


}

void AnimAction::Shake::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // ���s�t���OOn

	// ������
	m_nowFlame = 0;

}

// 2D
void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	pic->Render(x+(int)(m_shakeX-(m_orgX/2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), dwFlags);
}

void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	pic->Render(x + (int)(m_shakeX - (m_orgX / 2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), shader, name);
}

void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	pic->Render(x + (int)(m_shakeX - (m_orgX / 2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::Shake::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	pic->Render(x + (int)(m_shakeX - (m_orgX / 2.0f)), y + (int)(m_shakeY - (m_orgY / 2)), w, h, tx, ty, tw, th, shader, name);
}





/**********************/
// ��яo��
/**********************/

AnimAction::Jump::Jump(int endFlame, float startScale, float maxScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startscale = startScale;

	m_maxScale = maxScale;
	m_moveScale = maxScale / endFlame; //10/1=10 10/2=5;

//	m_orgAlpha = 1.0f;
//	m_alpha = 1.0f;
}

AnimAction::Jump::~Jump()
{

}

void AnimAction::Jump::Update(tdn2DObj * pic)
{
	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	if (ActionCheck() == false)return;

	// �t���[���X�V
	m_nowFlame++;
	// �G���h�t���[���܂ŗ�����I���
	//if (m_nowFlame >= m_endFlame)
	//{
	//	m_bActionFlag = false;
	//	// �g�嗦�X�V
	//	//pic->SetScale(m_startscale);
	//}

	// �A���t�@����
	//float rate = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)  


	if (m_nowFlame >= m_endFlame)
	{
		// ������
		pic->SetScale(pic->GetScale() - m_moveScale);

		// �X�^�[�g�������������Ȃ�����I��
		if (pic->GetScale() <= m_startscale)
		{
			// ���ǂ�
			pic->SetScale(m_startscale);
			m_bEndFlag = true; // �I��t���OON
			//m_bActionFlag = false;
			//m_alpha = (float)m_nowFlame / (float)m_endFlame;
			//m_alpha=
			return;
		}


	}
	else
	{		
		// �傫��
		pic->SetScale(pic->GetScale() + m_moveScale);
	
	//	m_alpha = (float)m_nowFlame / (float)m_endFlame;
	}




}

void AnimAction::Jump::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);

	// ������
	m_nowFlame = 0;
	pic->SetScale(m_startscale);

}


// 2D
void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, u32 dwFlags)
{
	//pic->SetARGB(255, 255, 255, 255);
	pic->Render(x, y, dwFlags);
	
	//pic->SetARGB((int)(m_alpha * 255), 255, 255, 255);
	//pic->Render(x, y, RS::ADD);

}

void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, tdnShader * shader, char * name)
{
	pic->Render(x, y, shader, name);
}

void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	pic->Render(x, y, w, h, tx, ty, tw, th, dwFlags);

}

void AnimAction::Jump::Render(tdn2DObj* pic, int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader * shader, char * name)
{
	pic->Render(x, y, w, h, tx, ty, tw, th, shader, name);
}



/**********************/
// �W�܂�p�ɏo��
/**********************/

AnimAction::Shrink::Shrink(int endFlame, float startScale, float maxScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startScale = startScale;
	m_maxScale = maxScale;

	m_nowScale = m_maxScale;

	float scaleRata = maxScale - startScale;

	m_addScale = scaleRata / endFlame;
}

AnimAction::Shrink::~Shrink()
{

}

void AnimAction::Shrink::Update(tdn2DObj * pic)
{
	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	//if (m_bActionFlag == false)return;
	if (ActionCheck() == false)return;

	// �t���[���X�V
	m_nowFlame++;
	// �G���h�t���[���܂ŗ�����I���
	if (m_nowFlame >= m_endFlame)
	{
		m_bEndFlag = true; // �I��t���OON
		//m_bActionFlag = false;
		// �g�嗦�X�V
		//pic->SetScale(m_startscale);
		//return;
	}

	// �A���t�@����
	float alpha = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)
	Math::Clamp(alpha, 0.0f, 1.0f);

	//alpha = 1.0f - alpha;
	pic->SetARGB((int)(alpha * 255), 255, 255, 255);

	// �X�P�[���������Ă���
	m_nowScale -= m_addScale;
	if (m_nowScale <= m_startScale)
	{
		m_nowScale = m_startScale;
	}

	// �g�嗦�X�V
	pic->SetScale(m_nowScale);

}

void AnimAction::Shrink::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // ���s�t���OOn

	// ������
	m_nowFlame = 0;

	m_nowScale = m_maxScale;
	pic->SetScale(m_nowScale);

}


/**********************/
// �傫���Ȃ�
/**********************/

AnimAction::Grow::Grow(int endFlame, float startScale, float moveScale)
{
	m_nowFlame = 0;
	m_endFlame = endFlame;
	m_startscale = startScale;
	m_movescale = moveScale;

}

AnimAction::Grow::~Grow()
{

}

void AnimAction::Grow::Update(tdn2DObj * pic)
{
	// �A�N�V�����t���O�������Ă��Ȃ��ƕԂ�
	//if (m_bActionFlag == false)return;
	if (ActionCheck() == false)return;

	// �t���[���X�V
	m_nowFlame++;
	// �G���h�t���[���܂ŗ�����I���
	if (m_nowFlame >= m_endFlame)
	{
		//m_bActionFlag = false;
		m_bEndFlag = true; // �I��t���OON
						   // �g�嗦�X�V
						   //pic->SetScale(m_startscale);

		return; // �I��
	}

	// �A���t�@����
	float alpha = (float)m_nowFlame / (float)m_endFlame;//   0/30=0   60/30=2   1-(0~1)  
	//alpha = 1.0f - alpha;
	pic->SetARGB((int)(alpha * 255), 255, 255, 255);

	// �g�嗦�X�V
	pic->SetScale(pic->GetScale() + m_movescale);


}

void AnimAction::Grow::Action(tdn2DObj * pic, int delay)
{
	AnimAction::Base::Action(pic, delay);
	//m_bActionFlag = true; // ���s�t���OOn

	// ������
	m_nowFlame = 0;
	pic->SetScale(m_startscale);

}


/******************************************/
// �`����A�j���p�ɉ��H
/******************************************/

void tdn2DAnim::OrderRipple(int endFlame, float startScale, float moveScale)
{
	if (m_pAction != nullptr) delete m_pAction; 
	m_pAction = new AnimAction::Ripple(endFlame, startScale, moveScale); 

}

void tdn2DAnim::OrderMoveAppeared(int endFlame, int startX, int startY)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::MoveAppeared(endFlame, startX, startY);
}

void tdn2DAnim::OrderShake(int endFlame, float shakeX, float shakeY, int cycle)
{	
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Shake(endFlame, shakeX, shakeY, cycle);
}

void tdn2DAnim::OrderJump(int endFlame, float startScale, float addScale)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Jump(endFlame, startScale, addScale);
}
// �W�܂�p�ɏo��
void tdn2DAnim::OrderShrink(int endFlame, float startScale, float maxScale)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Shrink(endFlame, startScale, maxScale);
}

void tdn2DAnim::OrderGrow(int endFlame, float startScale, float moveScale)
{
	if (m_pAction != nullptr) delete m_pAction;
	m_pAction = new AnimAction::Grow(endFlame, startScale, moveScale);

}