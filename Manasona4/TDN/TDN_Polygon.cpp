#include	"TDNLIB.h"

/********************************************/
//	tdnPolygon
//	2DObj��2D�E3D�S�Ẵ|���S���������ō��
/********************************************/

// http://rudora7.blog81.fc2.com/blog-entry-422.html

//DrawPrimitive�֐��E�E�E���f�����O�t�@�C���ō쐬����A���I�ɕύX����K�v�̖����A���_���������I�u�W�F�N�g�B
//DrawPrimitiveUp�֐��E�E�E�|�C���g�X�v���C�g��|���ȂǁA���I�ɕύX�����_�������Ȃ��I�u�W�F�N�g�B
//http://nutti2.seesaa.net/article/236658336.html

//DrawPrimitive
//���_�f�[�^�������ς������ČĂяo���I�[�o�[�w�b�h�������ł��A�e�N�X�`���Ƃ��̃p�����[�^���ʂɂ��郁�b�V���̕`��Ɏg���B
//DrawPrimitiveUP
//���_�f�[�^�̏�����������r�I�e�ՂŃe�N�X�`���Ȃǂ̃p�����[�^�����ʂ��₷���|���n�Ɏg���B
//http://d.hatena.ne.jp/arCtan/20091101/1257083345

//(��)DrawIndexedPrimitive���g�����Ȃ����悤�ɂȂ�Ώ������x���V���E�ɂȂ��

//*****************************************************************************
//	�R�c�|���S��
//*****************************************************************************
//------------------------------------------------------
//		�Œ�@�\��	
//------------------------------------------------------

void	tdnPolygon::Render3D(LPLVERTEX lpVertex, int Num, tdn2DObj* lpObj, u32 dwFlags)
{
	LPDEVICE	lpDevice = tdnSystem::GetDevice();// �����̂�

	//	(?)���[���h�s��ݒ� �P��f�o�C�X�̃g�����X�t�H�[���֘A�X�e�[�g��ݒ肵�܂��B 2 ���݂̕Ԋ҂��C������Matrix
	Matrix	mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	lpDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mat);

	//	�����_�[�X�e�[�g�X�V
	if (lpObj) tdnRenderState::Set(dwFlags , NULL, lpObj->GetTexture());
	else		tdnRenderState::Set(dwFlags , NULL, NULL);

	// Direct3D �̃��C�e�B���O��L���ɂ���ɂ� TRUE�A�����ɂ���ɂ� FALSE 
	lpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//	�����_�����O
	lpDevice->SetFVF(D3DFVF_LVERTEX);// �I���W�i���J�X�^��FVF�}�N���Œ��_�t�H�[�}�b�g�̓��e��`�B����B
	lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num, lpVertex, sizeof(LVERTEX));

	lpDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

// �s��𑗂�悤
void tdnPolygon::Render3D(LPLVERTEX lpVertex, int Num, tdn2DObj* lpObj, u32 dwFlags, D3DXMATRIX mat)// �ǉ�
{
	LPDEVICE	lpDevice = tdnSystem::GetDevice();
	//	���[���h�s��ݒ�
	// Matrix	mat( 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 );
	lpDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mat);

	//	�����_�[�X�e�[�g�X�V
	if (lpObj) tdnRenderState::Set(dwFlags, NULL, lpObj->GetTexture());
	else		tdnRenderState::Set(dwFlags, NULL, NULL);

	lpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//	�����_�����O
	lpDevice->SetFVF(D3DFVF_LVERTEX);// �I���W�i���J�X�^��FVF�}�N�� 
	lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num, lpVertex, sizeof(LVERTEX));

	lpDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//------------------------------------------------------
//		�V�F�[�_�[��
//------------------------------------------------------
void	tdnPolygon::Render3D(LPLVERTEX lpVertex, int Num, tdn2DObj* lpObj, tdnShader* shader, char* name)
{
	LPDEVICE	lpDevice = tdnSystem::GetDevice();
	
	// ���[�J��-�ˉe�ϊ��s��
	Matrix m = matView * matProjection;
	shader->SetWVPMatrix(&m);									//WVP�𑗂�

	lpDevice->SetFVF(D3DFVF_LVERTEX);							// ���_�t�H�[�}�b�g�̓��e��`�B����B
	if (lpObj) shader->SetDecaleTexture(lpObj->GetTexture());	// �V�F�[�_�[����Texture�𑗂�

	// ���V�F�[�_�[�w�J�n�x
	u32 pass = shader->Begin(name);
	for (u32 p = 0; p<pass; p++)
	{
		// ���V�F�[�_�[�w�p�X�J�n�x
		shader->BeginPass(p);
		shader->CommitChanges();
		//	�����_�����O
		lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num, lpVertex, sizeof(LVERTEX));
		// ���V�F�[�_�[�w�p�X�I���x
		shader->EndPass();
	}
	// ���V�F�[�_�[�w�I���x
	shader->End();
}

//*****************************************************************************
//	�Q�c�|���S��
//*****************************************************************************
//------------------------------------------------------
//		�Œ�@�\��	
//------------------------------------------------------
void	tdnPolygon::Render2D(LPTLVERTEX v, int Num, tdn2DObj* lpObj, u32 dwFlags)
{
	LPDEVICE	lpDevice = tdnSystem::GetDevice();

	//	�����_�[�X�e�[�g�X�V
	if (lpObj) tdnRenderState::Set(dwFlags, NULL, lpObj->GetTexture());
	else		tdnRenderState::Set(dwFlags, NULL, NULL);

	//	�����_�����O	
	lpDevice->SetFVF(D3DFVF_TLVERTEX);											// ���_�t�H�[�}�b�g�̓��e��`�B����B
	lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num, v, sizeof(TLVERTEX));
}

//------------------------------------------------------
//		�V�F�[�_�[��
//------------------------------------------------------
void	tdnPolygon::Render2D(LPTLVERTEX v, int Num, tdn2DObj* lpObj, tdnShader* shader, char* name)
{
	LPDEVICE	lpDevice = tdnSystem::GetDevice();
	
	lpDevice->SetFVF(D3DFVF_TLVERTEX);							// ���_�t�H�[�}�b�g�̓��e��`�B����B
	if (lpObj) shader->SetDecaleTexture(lpObj->GetTexture());	// �V�F�[�_�[����Texture�𑗂�

	// ���V�F�[�_�[�w�J�n�x
	u32 pass = shader->Begin(name);
	for (u32 p = 0; p<pass; p++)
	{
		// ���V�F�[�_�[�w�p�X�J�n�x
		shader->BeginPass(p);
		shader->CommitChanges();// BeginPass�`EndPass���ŃZ�b�^�[��ʂ��Ēl��ύX�����ۂɌĂяo���K�v������֐��ł�
		//	�����_�����O
		lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num, v, sizeof(TLVERTEX));
		// ���V�F�[�_�[�w�p�X�I���x
		shader->EndPass();
	}
	// ���V�F�[�_�[�w�I���x
	shader->End();
}

//*****************************************************************************
//	��`�`��
//*****************************************************************************
//------------------------------------------------------
//		�Œ�@�\��	
//------------------------------------------------------
void	tdnPolygon::Rect(int DstX, int DstY, int DstW, int DstH, u32 dwFlags, COLOR color, float z)
{
	TLVERTEX	v[4];

	v[0].sx = v[2].sx = (FLOAT)DstX;
	v[1].sx = v[3].sx = (FLOAT)(DstX + DstW);

	v[0].sy = v[1].sy = (FLOAT)DstY;
	v[2].sy = v[3].sy = (FLOAT)(DstY + DstH);

	v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	Render2D(v, 2, NULL, dwFlags);
}

//------------------------------------------------------
//		�V�F�[�_�[��
//------------------------------------------------------
void	tdnPolygon::Rect(int DstX, int DstY, int DstW, int DstH, tdnShader* shader, char* name, COLOR color, float z)
{
	TLVERTEX	v[4];

	v[0].sx = v[2].sx = (FLOAT)DstX;
	v[1].sx = v[3].sx = (FLOAT)(DstX + DstW);

	v[0].sy = v[1].sy = (FLOAT)DstY;
	v[2].sy = v[3].sy = (FLOAT)(DstY + DstH);

	v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	Render2D(v, 2, NULL, shader, name);
}


//*****************************************************************************
//	���`��
//*****************************************************************************
void tdnPolygon::DrawLine(float x1, float y1, float x2, float y2, DWORD color, float size)
{
	D3DXVECTOR2 v[2] =
	{
		{ x1, y1 },
		{ x2, y2 },
	};

	tdnSystem::GetLine()->SetWidth(size);
	tdnSystem::GetLine()->Begin();
	tdnSystem::GetLine()->Draw(v, 2, color);
	tdnSystem::GetLine()->End();

}

void tdnPolygon::DrawLine3D(Vector3 pos1, Vector3 pos2, DWORD color, float size, u32 dwFlag)
{
	VECTOR_LINE v[2] =
	{
		{ pos1.x, pos1.y, pos1.z, color },
		{ pos2.x, pos2.y, pos2.z, color },
	};


	LPDEVICE	lpDevice = tdnSystem::GetDevice();
	//	���[���h�s��ݒ�
	Matrix	mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	lpDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mat);

	tdnRenderState::Set(dwFlag & 0x0F, NULL, NULL);

	lpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//	�����_�����O
	lpDevice->SetFVF(D3DFVF_LVERTEX);// (D3DFVF_XYZ)
	lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(VECTOR_LINE));

	//lpDevice->SetRenderState(D3DRS_LIGHTING, TRUE);



}


//*****************************************************************************
//	�~(2D)�`��
//*****************************************************************************
//------------------------------------------------------
//		�Œ�@�\��	
//------------------------------------------------------
void	tdnPolygon::Circle2D(int cx, int cy, float r, u32 dwFlags, COLOR color, float z)
{
}

//------------------------------------------------------
//		�V�F�[�_�[��
//------------------------------------------------------
void	tdnPolygon::Circle2D(int cx, int cy, float r, tdnShader* shader, char* name, COLOR color, float z)
{
}