#include	"TDNLIB.h"


/********************************************/
//	tdnView
/********************************************/

/* ���̐錾 */
Vector tdnView::Pos;				// �J�����̌��_
Vector tdnView::Target;			// �J�����̃^�[�Q�b�g
D3DVIEWPORT9 tdnView::Viewport;	// 3D��`�悷��Ƃ��́@�\������Window��ʂ̉����Əc���@��O�Ɖ��@�@
float tdnView::FovY;				// ����p
float tdnView::Near;				// �r���[�|�[�g�̎�O
float tdnView::Far;				// ��ԉ��̈ʒu (�����ق�Z�t�@�C�e�B���O�������Ȃ�)
float tdnView::Aspect;				// 

/*************/
//	�N���A
/*************/
/*
*   Clear�֐�
*	�N���A����Ώۂ��w��ł���B�g�ݍ��킹�Ďg����
*   ���
*	D3DCLEAR_STENCIL  �X�e���V�� �o�b�t�@���N���A���āAStencil �p�����[�^�̒l�ɂ���  
*	D3DCLEAR_TARGET  �����_�����O �^�[�Q�b�g���N���A���āAColor �p�����[�^�̐F�ɂ���  
*	D3DCLEAR_ZBUFFER  �[�x�o�b�t�@���N���A���āAZ �p�����[�^�̒l�ɂ���  
*	NULL�ɂ���ƑS��������
*/

// ���E�N���A
void tdnView::Clear(DWORD color, bool bClearZ)
{
	DWORD flag = D3DCLEAR_TARGET;					// �F���N���A
	if (bClearZ == true) flag |= D3DCLEAR_ZBUFFER;	// Z�t���O��TRUE�Ȃ�Z�o�b�t�@������

	// ��ʃN���A
	tdnSystem::GetDevice()->Clear(0, NULL, flag, color, 1.0f, 0);// 0 NULL�őS���N���A

}
// Z�o�b�t�@�̂݃N���A
void tdnView::ClearZ()
{
	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
}


/*************/
//	���_�ݒ�
/*************/
//	���_�E�����_�ݒ�
void tdnView::Set(const Vector& pos, const Vector& target)
{
	Pos = pos;
	Target = target;
}

//	�ʒu�E�p�x�ݒ�
void tdnView::Set(float x, float y, float z, float ax, float ay, float az)
{
	float sx = sinf(ax);
	float sy = sinf(ay);
	float cx = cosf(ax);
	float cy = cosf(ay);

	Vector	p, t;
	p.x = x;
	p.y = y;
	p.z = z;
	t.x = x + (cx*sy);
	t.y = y + (-sx);
	t.z = z + (cx*cy);

	Set(p, t);
}


/***********************************************/
//	���e���ʐݒ� Window�ɉf���o������ς��邾��
/***********************************************/
void tdnView::SetViewport()
{
	Viewport.X = Viewport.Y = 0;
	Viewport.Width = tdnSystem::GetScreenSize().right;
	Viewport.Height =  tdnSystem::GetScreenSize().bottom;
	Viewport.MaxZ = 1.0f; 
	Viewport.MinZ = 0.0f;	

	Aspect = (float)Viewport.Width / (float)Viewport.Height;
}
void tdnView::SetViewport(int x, int y, int w, int h)
{
	Viewport.X = x;
	Viewport.Y = y;
	Viewport.Width = w;
	Viewport.Height = h;
	Viewport.MaxZ = 1.0f;
	Viewport.MinZ = 0.0f;

	Aspect = (float)w / (float)h;
}

/***********************************************/
//	���e�ݒ� �@�`�悷���O�Ɖ������߂邾��
/***********************************************/
void tdnView::SetProjection(float FovY, float Near, float Far)
{
	if (Viewport.Width == 0)
	{
		Viewport.Width = tdnSystem::GetScreenSize().right;
		Viewport.Height = tdnSystem::GetScreenSize().bottom;
	}
	SetProjection(FovY, Near, Far, (float)Viewport.Width / (float)Viewport.Height);
}

void tdnView::SetProjection(float _FovY, float _Near, float _Far, float _asp)
{
	FovY = _FovY;
	Near = _Near;
	Far = _Far;
	Aspect = _asp;
}

//*****************************************************************************
//		�r���[�ϊ��E�v���W�F�N�V�����ϊ��E�r���[�|�[�g�K�p
//*****************************************************************************
void tdnView::Activate()
{
	// �s��
	Matrix matV, matP;

	//	�r���[�s��ݒ�
	Vector p, t;
	p = Pos;
	t = Target;
	
	Math::LookAtLH(matV, p, t, Vector3(.0f, 1.0f, .0f));

	//	�r���[�|�[�g�ݒ�
	if (Viewport.Width != 0) tdnSystem::GetDevice()->SetViewport(&Viewport);

	//	���e�s��ݒ�
	Math::PerspectiveLH(matP, FovY, Aspect, Near, Far);

	//	�O���[�o���ւ̓K�p
	::matProjection = matP;
	::matView = matV;

	//	DirectX�ݒ� �P��f�o�C�X�̃g�����X�t�H�[���֘A�X�e�[�g��ݒ肷��B
	tdnSystem::GetDevice()->SetTransform(D3DTS_PROJECTION, &matP);
	tdnSystem::GetDevice()->SetTransform(D3DTS_VIEW, &matV);
}


