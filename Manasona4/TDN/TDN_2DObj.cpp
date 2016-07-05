#include	"TDNLIB.h"

// �摜�̕�Ԃւ̑Ώ��̂��߂̃T�C�Y����
// �e�N�X�`����̃e�N�Z���͂��̂ǐ^�񒆂��؂�̗ǂ��������W�ɂȂ��Ă��邽�߁A
//�u0.5�v�Ƃ����̂̓e�N�Z���ƃe�N�Z���̒��x���ڂɑ�������ʒu�ɂȂ�
static const float abjustSize = 0.5f;

/****************************************************/
//	2DObj
/****************************************************/

// �t�@�C������摜�ǂݍ���
tdn2DObj::tdn2DObj(const char* fileName):m_bLoadTexture(true)
{
	// ������
	m_width = m_height = 0;
	lpSurface = NULL;
	lpTexture = NULL;
	
	// �e�N�X�`���̓ǂݍ���
	lpTexture = tdnTexture::Load(fileName);
	if (lpTexture == NULL)
	{
		//MessageBox(0, "�t�@�C������e�N�X�`���̓ǂݍ��݂Ɏ��s", "2DObj", MB_OK);
		MyAssert(0, "�G���[: �t�@�C������e�N�X�`���̓ǂݍ��݂Ɏ��s�B\n%s", fileName);	// �G���[�o���Ă�t�@�C�����̓���
		return;
	}

	/***************************************/
	// �ǂݍ��񂾃e�N�X�`������T�C�Y��ۑ�
	D3DSURFACE_DESC sd;						// �T�[�t�F�C�X���L�q����B
	lpTexture->GetLevelDesc(0, &sd);		// �e�N�X�`���̏��擾
	m_width = sd.Width;						// �����o�ϐ��ɃT�C�Y�ۑ� 
	m_height = sd.Height;					

	// ���C���T�[�t�F�C�X�̎擾(�ۑ�)
	lpTexture->GetSurfaceLevel(0, &lpSurface);

	// �X�e�[�^�X������
	scale = 1.0f;
	angle = 0.0f;
	color = 0xFFFFFFFF;
	centerX = centerY = 0.0f;
	isTurnOver = false;
	isShiftCenter = false;

}

// �������[����摜�ǂݍ���
tdn2DObj::tdn2DObj(const char* IDName, const char* pArchiveName):m_bLoadTexture(true)
{
	// ������
	m_width = m_height = 0;
	lpSurface = NULL;
	lpTexture = NULL;

	// �e�N�X�`���̓ǂݍ���
	lpTexture = tdnTexture::LoadMemory(IDName, pArchiveName);
	if (lpTexture == NULL)
	{
		MessageBox(0, "�������[����e�N�X�`���̓ǂݍ��݂Ɏ��s\n�q���g:�啶���������̓��͂̍��͊֌W�Ȃ�", "2DObj", MB_OK);
		return;
	}

	/***************************************/
	// �ǂݍ��񂾃e�N�X�`������T�C�Y��ۑ�
	D3DSURFACE_DESC sd;						// �T�[�t�F�C�X���L�q����B
	lpTexture->GetLevelDesc(0, &sd);		// �e�N�X�`���̏��擾
	m_width = sd.Width;						// �����o�ϐ��ɃT�C�Y�ۑ� 
	m_height = sd.Height;

	// ���C���T�[�t�F�C�X�̎擾(�ۑ�)
	lpTexture->GetSurfaceLevel(0, &lpSurface);

	// �X�e�[�^�X������
	scale = 1.0f;
	angle = 0.0f;
	color = 0xFFFFFFFF;
	centerX = centerY = 0.0f;
	isTurnOver = false;
	isShiftCenter = false;

}

// �����_�[�^�[�Q�b�g(�`���)�쐬
tdn2DObj::tdn2DObj(UINT width, UINT height, FMT2D fmtFlag):m_bLoadTexture(true)
{
	//	��񏉊���
	m_width = m_height = 0;
	lpSurface = NULL;
	lpTexture = NULL;
	D3DFORMAT	fmt = D3DFMT_R8G8B8;
	u32			usage = 0;
	D3DPOOL		pool = D3DPOOL_MANAGED;

	
	// fmtFlag�Ńt�H�[�}�b�g�w��
	switch (fmtFlag)
	{
	case RENDERTARGET:
		usage = D3DUSAGE_RENDERTARGET;
		fmt = tdnSystem::GetScreenFormat();
		pool = D3DPOOL_DEFAULT;
		break;
	case FLOAT1:
		usage = D3DUSAGE_RENDERTARGET;
		fmt = D3DFMT_R32F;
		pool = D3DPOOL_DEFAULT;
		break;
	case FLOAT2:
		usage = D3DUSAGE_RENDERTARGET;
		fmt = D3DFMT_G32R32F;
		pool = D3DPOOL_DEFAULT;
		break;
	case USEALPHA:
		usage = D3DUSAGE_RENDERTARGET;
		fmt = D3DFMT_A8R8G8B8;
		pool = D3DPOOL_DEFAULT;
		break;
	case SYSTEMMEM:
		fmt = D3DFMT_A8R8G8B8;
		pool = D3DPOOL_SYSTEMMEM;
		break;
	case HDR:
		usage = D3DUSAGE_RENDERTARGET;
		fmt = D3DFMT_A16B16G16R16F;
		pool = D3DPOOL_DEFAULT;
		break;
	case SYSTEMMEM_HDR:
		fmt = D3DFMT_R32F;
		pool = D3DPOOL_SYSTEMMEM;
		break;
	default:
		MessageBox(0, "�t�H�[�}�b�g�̐ݒ�Ɏ��s", "2DObj", MB_OK);
		break;
	}

	// ���̐ݒ�Ńe�N�X�`���쐬
	D3DXCreateTexture(tdnSystem::GetDevice(), width, height, 0, usage, fmt, pool, &lpTexture);
	if (lpTexture == NULL)
	{
		MessageBox(0, "D3DXCreateTexture�̐ݒ�Ɏ��s", "2DObj", MB_OK);
		return;
	}

	/***************************************/
	// �ǂݍ��񂾃e�N�X�`������T�C�Y��ۑ�
	D3DSURFACE_DESC sd;						// �T�[�t�F�C�X���L�q����B
	lpTexture->GetLevelDesc(0, &sd);		// �e�N�X�`���̏��擾
	m_width = sd.Width;						// �����o�ϐ��ɃT�C�Y�ۑ� 
	m_height = sd.Height;

	// ���C���T�[�t�F�C�X�̎擾(�ۑ�)
	lpTexture->GetSurfaceLevel(0, &lpSurface);

	// �X�e�[�^�X������
	scale = 1.0f;
	angle = 0.0f;
	color = 0xFFFFFFFF;
	centerX = centerY = 0.0f;
	isTurnOver = false;
	isShiftCenter = false;

}

// �e�N�X�`���Q�ƌ^
tdn2DObj::tdn2DObj(Texture2D *texture) : lpTexture(texture), m_bLoadTexture(false), scale(1), angle(0), color(0xffffffff), centerX(0), centerY(0), isTurnOver(false), isShiftCenter(false)
{
	MyAssert(texture, "�G���[: tdn2DObj�̃R���X�g���N�^�A�e�N�X�`������B");

	/***************************************/
	// �ǂݍ��񂾃e�N�X�`������T�C�Y��ۑ�
	D3DSURFACE_DESC sd;						// �T�[�t�F�C�X���L�q����B
	lpTexture->GetLevelDesc(0, &sd);		// �e�N�X�`���̏��擾
	m_width = sd.Width;						// �����o�ϐ��ɃT�C�Y�ۑ� 
	m_height = sd.Height;

	// ���C���T�[�t�F�C�X�̎擾(�ۑ�)
	lpTexture->GetSurfaceLevel(0, &lpSurface);
}

//	���
tdn2DObj::~tdn2DObj(){
	if (lpSurface){
		lpSurface->Release();
	}
	if (lpTexture && m_bLoadTexture){
		tdnTexture::Release(lpTexture);
	}
	
}

/*******************************/
//	�����_�[�^�[�Q�b�g�؂�ւ�
/*******************************/
void tdn2DObj::RenderTarget(int index)
{
	tdnSystem::GetDevice()->SetRenderTarget(index, lpSurface);
}

/*******************************/
//	�`��
/*******************************/
void tdn2DObj::Render(int x, int y, u32 dwFlags)
{

	// ���_���
	TLVERTEX	v[4];

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// Z��0
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = 0.0f;

	// ���_����g��
	v[0].sx = v[2].sx = (float)(m_width * -0.5f - centerX) * scale;
	v[1].sx = v[3].sx = (float)(m_width * 0.5f - centerX) * scale;
	v[0].sy = v[1].sy = (float)(m_height * -0.5f - centerY) * scale;
	v[2].sy = v[3].sy = (float)(m_height * 0.5f - centerY) * scale;


	//v[0].sx = v[2].sx = (float)x;
	//v[1].sx = v[3].sx = (float)(x + this->m_width) - abjustSize; //0.5f���炷
	//v[0].sy = v[1].sy = (float)y;
	//v[2].sy = v[3].sy = (float)(y + this->m_height) - abjustSize;

	// ���_�����]
	for (DWORD i = 0; i < 4; i++){
		const float xrot = v[i].sx;
		const float yrot = v[i].sy;
		v[i].sx = xrot * cos(angle) + yrot * sin(angle);
		v[i].sy = -xrot * sin(angle) + yrot * cos(angle);
	}


	// �ړ�
	float transX = 0.0f;
	float transY = 0.0f;
	if (isShiftCenter)
	{
		// ���̂܂܈ړ�
		transX = (float)(x);
		transY = (float)(y);
	}
	else
	{
		// ���̈ʒu�ɖ߂��Ĉړ�
		transX = (float)(x + centerX + (m_width * 0.5f));
		transY = (float)(y + centerY + (m_height * 0.5f));
	}

	v[0].sx += transX;
	v[2].sx += transX;
	v[1].sx += transX - abjustSize; //0.5f���炷
	v[3].sx += transX - abjustSize; //0.5f���炷	
	v[0].sy += transY;
	v[1].sy += transY;
	v[2].sy += transY - abjustSize;
	v[3].sy += transY - abjustSize;


	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v[0].tu = v[2].tu = (float)abjustSize / (float)this->m_width;// �ق�̏���UV���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[1].tu = v[3].tu = 1.0;
		else
			v[1].tu = v[3].tu = 1.0f - (float)abjustSize / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[1].tu = v[3].tu = 0.0f;//
		else
			v[1].tu = v[3].tu = (float)abjustSize / (float)this->m_width;//

		v[0].tu = v[2].tu = 1.0;
	}
	// �c��uv���W�͕ς�炸
	v[0].tv = v[1].tv = (float)abjustSize / (float)this->m_height;// 
	// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v[2].tv = v[3].tv = 1.0;
	else
		v[2].tv = v[3].tv = 1.0f - (float)abjustSize / (float)this->m_height;


	// �|���S���̐F
	v[0].color = v[1].color = v[2].color = v[3].color = color;

	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//���Z��?

	// ���̏��Ń|���S���쐬!!
	tdnPolygon::Render2D(v, 2, this, dwFlags);


	// ���ɖ߂�

}

void tdn2DObj::Render(int x, int y, tdnShader* shader, char* name)
{

	// ���_���
	TLVERTEX	v[4];

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// Z��0
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = 0.0f;

	// ���_����g��
	v[0].sx = v[2].sx = (float)(m_width * -0.5f - centerX) * scale;
	v[1].sx = v[3].sx = (float)(m_width * 0.5f - centerX) * scale;
	v[0].sy = v[1].sy = (float)(m_height * -0.5f - centerY) * scale;
	v[2].sy = v[3].sy = (float)(m_height * 0.5f - centerY) * scale;

	// ���_�����]
	for (DWORD i = 0; i < 4; i++){
		const float xrot = v[i].sx;
		const float yrot = v[i].sy;
		v[i].sx = xrot * cos(angle) + yrot * sin(angle);
		v[i].sy = -xrot * sin(angle) + yrot * cos(angle);
	}


	// �ړ�
	float transX = 0.0f;
	float transY = 0.0f;
	if (isShiftCenter)
	{
		// ���̂܂܈ړ�
		transX = (float)(x);
		transY = (float)(y);
	}
	else
	{
		// ���̈ʒu�ɖ߂��Ĉړ�
		transX = (float)(x + centerX + (m_width * 0.5f));
		transY = (float)(y + centerY + (m_height * 0.5f));
	}

	v[0].sx += transX;
	v[2].sx += transX;
	v[1].sx += transX - abjustSize; //0.5f���炷
	v[3].sx += transX - abjustSize; //0.5f���炷	
	v[0].sy += transY;
	v[1].sy += transY;
	v[2].sy += transY - abjustSize;
	v[3].sy += transY - abjustSize;


	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v[0].tu = v[2].tu = (float)abjustSize / (float)this->m_width;// �ق�̏���UV���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[1].tu = v[3].tu = 1.0;
		else
			v[1].tu = v[3].tu = 1.0f - (float)abjustSize / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[1].tu = v[3].tu = 0.0f;//
		else
			v[1].tu = v[3].tu = (float)abjustSize / (float)this->m_width;//

		v[0].tu = v[2].tu = 1.0;
	}
	// �c��uv���W�͕ς�炸
	v[0].tv = v[1].tv = (float)abjustSize / (float)this->m_height;// 
	// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v[2].tv = v[3].tv = 1.0;
	else
		v[2].tv = v[3].tv = 1.0f - (float)abjustSize / (float)this->m_height;


	// �|���S���̐F
	v[0].color = v[1].color = v[2].color = v[3].color = color;

	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//���Z��?

	// ���̏��Ń|���S���쐬!!
	tdnPolygon::Render2D(v, 2, this, shader, name);

}

// �T�C�Y�w���2D�`��
void tdn2DObj::Render(int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	// ���_���
	TLVERTEX	v[4];

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// ���_����g��
	v[0].sx = v[2].sx = (float)(w * -0.5f - centerX) * scale;
	v[1].sx = v[3].sx = (float)(w * 0.5f - centerX) * scale;
	v[0].sy = v[1].sy = (float)(h * -0.5f - centerY) * scale;
	v[2].sy = v[3].sy = (float)(h * 0.5f - centerY) * scale;

	// ���_�����]
	for (DWORD i = 0; i < 4; i++){
		const float xrot = v[i].sx;
		const float yrot = v[i].sy;
		v[i].sx = xrot * cos(angle) + yrot * sin(angle);
		v[i].sy = -xrot * sin(angle) + yrot * cos(angle);
	}

	// �ړ�
	float transX = 0.0f;
	float transY = 0.0f;
	if (isShiftCenter)
	{
		// ���̂܂܈ړ�
		transX = (float)(x);
		transY = (float)(y);
	}
	else
	{
		// ���̈ʒu�ɖ߂��Ĉړ�
		transX = (float)(x + centerX + (w * 0.5f));
		transY = (float)(y + centerY + (h * 0.5f));
	}

	v[0].sx += transX;
	v[2].sx += transX;
	v[1].sx += transX - abjustSize; //0.5f���炷
	v[3].sx += transX - abjustSize; //0.5f���炷	
	v[0].sy += transY;
	v[1].sy += transY;
	v[2].sy += transY - abjustSize;
	v[3].sy += transY - abjustSize;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v[0].tu = v[2].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);// �ق�̏���UV���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[1].tu = v[3].tu = (float)(tx + tw) / (float)this->m_width;
		else
			v[1].tu = v[3].tu = (float)((tx + tw) - abjustSize) / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		if (angle == 0.0f)
			v[1].tu = v[3].tu = (float)(tx) / ((float)this->m_width);//
		else
			v[1].tu = v[3].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);//

		v[0].tu = v[2].tu = (float)((tx + tw) - (float)abjustSize) / ((float)this->m_width);
	}

	// �c��uv���W�͕ς�炸
	v[0].tv = v[1].tv = (float)(ty + abjustSize) / (float)this->m_height;// 
	/// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v[2].tv = v[3].tv = (float)((ty + th)) / (float)this->m_height;
	else
		v[2].tv = v[3].tv = (float)((ty + th) - abjustSize) / (float)this->m_height;


	// �|���S���̐F
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//���Z��?	
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = 0.0f;// Z��0

	// ���̏��Ń|���S���쐬!!
	tdnPolygon::Render2D(v, 2, this, dwFlags);

}

void tdn2DObj::Render(int x, int y, int w, int h, int tx, int ty, int tw, int th, tdnShader* shader, char* tech)
{
	// ���_���
	TLVERTEX	v[4];

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// ���_����g��
	v[0].sx = v[2].sx = (float)(w * -0.5f - centerX) * scale;
	v[1].sx = v[3].sx = (float)(w * 0.5f - centerX) * scale;
	v[0].sy = v[1].sy = (float)(h * -0.5f - centerY) * scale;
	v[2].sy = v[3].sy = (float)(h * 0.5f - centerY) * scale;

	// ���_�����]
	for (DWORD i = 0; i < 4; i++){
		const float xrot = v[i].sx;
		const float yrot = v[i].sy;
		v[i].sx = xrot * cos(angle) + yrot * sin(angle);
		v[i].sy = -xrot * sin(angle) + yrot * cos(angle);
	}

	// �ړ�
	float transX = 0.0f;
	float transY = 0.0f;
	if (isShiftCenter)
	{
		// ���̂܂܈ړ�
		transX = (float)(x);
		transY = (float)(y);
	}
	else
	{
		// ���̈ʒu�ɖ߂��Ĉړ�
		transX = (float)(x + centerX + (w * 0.5f));
		transY = (float)(y + centerY + (h * 0.5f));
	}

	v[0].sx += transX;
	v[2].sx += transX;
	v[1].sx += transX - abjustSize; //0.5f���炷
	v[3].sx += transX - abjustSize; //0.5f���炷	
	v[0].sy += transY;
	v[1].sy += transY;
	v[2].sy += transY - abjustSize;
	v[3].sy += transY - abjustSize;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v[0].tu = v[2].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);// �ق�̏���UV���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[1].tu = v[3].tu = (float)(tx + tw) / (float)this->m_width;
		else
			v[1].tu = v[3].tu = (float)((tx + tw) - abjustSize) / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		if (angle == 0.0f)
			v[1].tu = v[3].tu = (float)(tx) / ((float)this->m_width);//
		else
			v[1].tu = v[3].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);//

		v[0].tu = v[2].tu = (float)((tx + tw) - (float)abjustSize) / ((float)this->m_width);
	}

	// �c��uv���W�͕ς�炸
	v[0].tv = v[1].tv = (float)(ty + abjustSize) / (float)this->m_height;// 
	/// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v[2].tv = v[3].tv = (float)((ty + th)) / (float)this->m_height;
	else
		v[2].tv = v[3].tv = (float)((ty + th) - abjustSize) / (float)this->m_height;


	// �|���S���̐F
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//���Z��?	
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = 0.0f;// Z��0

	// ���̏��Ń|���S���쐬!!
	tdnPolygon::Render2D(v, 2, this, shader, tech);


}

// ���]
void tdn2DObj::RenderReversal(int x, int y, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	// ���_���
	TLVERTEX	v[4];

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// ���_����g��
	v[1].sx = v[3].sx = (float)(w * -0.5f - centerX) * scale;
	v[0].sx = v[2].sx = (float)(w * 0.5f - centerX) * scale;
	v[0].sy = v[1].sy = (float)(h * -0.5f - centerY) * scale;
	v[2].sy = v[3].sy = (float)(h * 0.5f - centerY) * scale;

	// ���_�����]
	for (DWORD i = 0; i < 4; i++) {
		const float xrot = v[i].sx;
		const float yrot = v[i].sy;
		v[i].sx = xrot * cos(angle) + yrot * sin(angle);
		v[i].sy = -xrot * sin(angle) + yrot * cos(angle);
	}

	// �ړ�
	float transX = 0.0f;
	float transY = 0.0f;
	if (isShiftCenter)
	{
		// ���̂܂܈ړ�
		transX = (float)(x);
		transY = (float)(y);
	}
	else
	{
		// ���̈ʒu�ɖ߂��Ĉړ�
		transX = (float)(x + centerX + (w * 0.5f));
		transY = (float)(y + centerY + (h * 0.5f));
	}

	v[1].sx += transX;
	v[3].sx += transX;
	v[0].sx += transX - abjustSize; //0.5f���炷
	v[2].sx += transX - abjustSize; //0.5f���炷	
	v[0].sy += transY;
	v[1].sy += transY;
	v[2].sy += transY - abjustSize;
	v[3].sy += transY - abjustSize;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v[1].tu = v[3].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);// �ق�̏���UV���W�����炷

																					 // ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v[0].tu = v[2].tu = (float)(tx + tw) / (float)this->m_width;
		else
			v[0].tu = v[2].tu = (float)((tx + tw) - abjustSize) / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		if (angle == 0.0f)
			v[0].tu = v[2].tu = (float)(tx) / ((float)this->m_width);//
		else
			v[0].tu = v[2].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);//

		v[1].tu = v[4].tu = (float)((tx + tw) - (float)abjustSize) / ((float)this->m_width);
	}

	// �c��uv���W�͕ς�炸
	v[0].tv = v[1].tv = (float)(ty + abjustSize) / (float)this->m_height;// 
																		 /// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v[2].tv = v[3].tv = (float)((ty + th)) / (float)this->m_height;
	else
		v[2].tv = v[3].tv = (float)((ty + th) - abjustSize) / (float)this->m_height;


	// �|���S���̐F
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//���Z��?	
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = 0.0f;// Z��0

												 // ���̏��Ń|���S���쐬!!
	tdnPolygon::Render2D(v, 2, this, dwFlags);


}


// 3D�`��
void tdn2DObj::Render3D(float x, float y, float z, u32 dwFlags)
{

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// 3D�p���_
	LVERTEX v2[4];

	// �Q�[���́��@�Ɓ@���@���w��
	Vector3 right(matView._11, matView._21, matView._31);// +�Ł��@-�Ł�
	Vector3 up(matView._12, matView._22, matView._32);// +�Ł��@-�Ł�
	right.Normalize();
	up.Normalize();

	// �r���{�[�h�̃x�N�g������]
	float cosan(cosf(angle)), sinan(sinf(angle));
	Vector3 turned_right, turned_up;
	turned_right = right * cosan + up * sinan;

	turned_right = right * cos(angle) + up * sin(angle);
	turned_up = -right * sin(angle) + up * cos(angle);

	// �g�嗦&��
	// v[0].sx = v[2].sx = (float)(m_width * -0.5f - centerX) * scale;
	float maxSize;
	(m_width > m_height) ? ((float)(maxSize = (float)m_width)) : ((float)(maxSize = (float)m_height));
	float sizeX = (float)(m_width / maxSize)* scale;
	float sizeY = (float)(m_height / maxSize)* scale;

	// �|�W�V�����̏����ݒ�			  
	v2[0].x = -(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@�� = ����
	v2[0].y = -(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[0].z = -(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[1].x = +(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[1].y = +(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[1].z = +(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[2].x = -(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[2].y = -(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[2].z = -(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));

	v2[3].x = +(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[3].y = +(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[3].z = +(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));

	// ��](R)
	//for (DWORD i = 0; i < 4; i++){
	//	const float xrot = v2[i].x;
	//	const float yrot = v2[i].y;
	//	const float zrot = v2[i].z;

	//	v2[i].x = xrot * cos(angle) + yrot * sin(angle);
	//	v2[i].y = -xrot * sin(angle) + yrot * cos(angle);
	//}

	// ���_�𕽍s�ړ�(T)	
	v2[0].x += (float)x;
	v2[0].y += (float)y;
	v2[0].z += (float)z;

	v2[1].x += (float)x;
	v2[1].y += (float)y;
	v2[1].z += (float)z;

	v2[2].x += (float)x;
	v2[2].y += (float)y;
	v2[2].z += (float)z;

	v2[3].x += (float)x;
	v2[3].y += (float)y;
	v2[3].z += (float)z;


	// �|���S���̐F
	v2[0].color = v2[1].color = v2[2].color = v2[3].color = color;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v2[0].tu = v2[2].tu = (float)abjustSize / (float)this->m_width;// �ق�̏���UV���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = 1.0f;
		else
			v2[1].tu = v2[3].tu = 1.0f - (float)abjustSize / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = 0.0f;//
		else
			v2[1].tu = v2[3].tu = (float)abjustSize / (float)this->m_width;//

		v2[0].tu = v2[2].tu = 1.0f;
	}
	// �c��uv���W�͕ς�炸
	v2[0].tv = v2[1].tv = (float)abjustSize / (float)this->m_height;// 
	// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v2[2].tv = v2[3].tv = 1.0f;
	else
		v2[2].tv = v2[3].tv = 1.0f - (float)abjustSize / (float)this->m_height;


	//�`��
	tdnPolygon::Render3D(v2, 2, this, dwFlags);

}

void tdn2DObj::Render3D(Vector3 pos, u32 dwFlags)
{

	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// 3D�p���_
	LVERTEX v2[4];

	// �Q�[���́��@�Ɓ@���@���w��
	Vector3 right(matView._11, matView._21, matView._31);// +�Ł��@-�Ł�
	Vector3 up(matView._12, matView._22, matView._32);// +�Ł��@-�Ł�
	right.Normalize();
	up.Normalize();

	// �r���{�[�h�̃x�N�g������]
	float cosan(cosf(angle)), sinan(sinf(angle));
	Vector3 turned_right, turned_up;
	turned_right = right * cosan + up * sinan;

	turned_right = right * cos(angle) + up * sin(angle);
	turned_up = -right * sin(angle) + up * cos(angle);

	// �g�嗦&��
	// v[0].sx = v[2].sx = (float)(m_width * -0.5f - centerX) * scale;
	float maxSize;
	(m_width > m_height) ? ((float)(maxSize = (float)m_width)) : ((float)(maxSize = (float)m_height));
	float sizeX = (float)(m_width / maxSize)* scale;
	float sizeY = (float)(m_height / maxSize)* scale;

	// �|�W�V�����̏����ݒ�			  
	v2[0].x = -(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@�� = ����
	v2[0].y = -(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[0].z = -(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[1].x = +(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[1].y = +(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[1].z = +(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[2].x = -(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[2].y = -(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[2].z = -(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));

	v2[3].x = +(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[3].y = +(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[3].z = +(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));

	// ��](R)
	//for (DWORD i = 0; i < 4; i++){
	//	const float xrot = v2[i].x;
	//	const float yrot = v2[i].y;
	//	const float zrot = v2[i].z;

	//	v2[i].x = xrot * cos(angle) + yrot * sin(angle);
	//	v2[i].y = -xrot * sin(angle) + yrot * cos(angle);
	//}

	// ���_�𕽍s�ړ�(T)	
	v2[0].x += (float)pos.x;
	v2[0].y += (float)pos.y;
	v2[0].z += (float)pos.z;

	v2[1].x += (float)pos.x;
	v2[1].y += (float)pos.y;
	v2[1].z += (float)pos.z;

	v2[2].x += (float)pos.x;
	v2[2].y += (float)pos.y;
	v2[2].z += (float)pos.z;

	v2[3].x += (float)pos.x;
	v2[3].y += (float)pos.y;
	v2[3].z += (float)pos.z;


	// �|���S���̐F
	v2[0].color = v2[1].color = v2[2].color = v2[3].color = color;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v2[0].tu = v2[2].tu = (float)abjustSize / (float)this->m_width;// �ق�̏���UV���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = 1.0f;
		else
			v2[1].tu = v2[3].tu = 1.0f - (float)abjustSize / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = 0.0f;//
		else
			v2[1].tu = v2[3].tu = (float)abjustSize / (float)this->m_width;//

		v2[0].tu = v2[2].tu = 1.0f;
	}
	// �c��uv���W�͕ς�炸
	v2[0].tv = v2[1].tv = (float)abjustSize / (float)this->m_height;// 
	// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v2[2].tv = v2[3].tv = 1.0f;
	else
		v2[2].tv = v2[3].tv = 1.0f - (float)abjustSize / (float)this->m_height;


	//�`��
	tdnPolygon::Render3D(v2, 2, this, dwFlags);

}

void tdn2DObj::Render3D(float x, float y, float z, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// 3D�p���_
	LVERTEX v2[4];

	// �Q�[���́��@�Ɓ@���@���w��
	Vector3 right(matView._11, matView._21, matView._31);// +�Ł��@-�Ł�
	Vector3 up(matView._12, matView._22, matView._32);// +�Ł��@-�Ł�
	right.Normalize();
	up.Normalize();

	// �r���{�[�h�̃x�N�g������]
	float cosan(cosf(angle)), sinan(sinf(angle));
	Vector3 turned_right, turned_up;
	turned_right = right * cosan + up * sinan;
	turned_right = right * cos(angle) + up * sin(angle);
	turned_up = -right * sin(angle) + up * cos(angle);

	// �g�嗦&��
	float maxSize;
	(m_width > m_height) ? ((float)(maxSize = (float)m_width)) : ((float)(maxSize = (float)m_height));
	float sizeX = (float)(w / maxSize)* scale;
	float sizeY = (float)(h / maxSize)* scale;

	// �|�W�V�����̏����ݒ�			  
	v2[0].x = -(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@�� = ����
	v2[0].y = -(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[0].z = -(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[1].x = +(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[1].y = +(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[1].z = +(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[2].x = -(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[2].y = -(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[2].z = -(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));

	v2[3].x = +(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[3].y = +(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[3].z = +(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));


	// ���_�𕽍s�ړ�(T)	
	v2[0].x += (float)x;
	v2[0].y += (float)y;
	v2[0].z += (float)z;

	v2[1].x += (float)x;
	v2[1].y += (float)y;
	v2[1].z += (float)z;

	v2[2].x += (float)x;
	v2[2].y += (float)y;
	v2[2].z += (float)z;

	v2[3].x += (float)x;
	v2[3].y += (float)y;
	v2[3].z += (float)z;


	// �|���S���̐F
	v2[0].color = v2[1].color = v2[2].color = v2[3].color = color;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v2[0].tu = v2[2].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);// �ق�̏���Uv2���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = (float)(tx + tw) / (float)this->m_width;
		else
			v2[1].tu = v2[3].tu = (float)((tx + tw) - abjustSize) / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = (float)(tx) / ((float)this->m_width);//
		else
			v2[1].tu = v2[3].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);//

		v2[0].tu = v2[2].tu = (float)((tx + tw) - (float)abjustSize) / ((float)this->m_width);
	}

	// �c��uv2���W�͕ς�炸
	v2[0].tv = v2[1].tv = (float)(ty + abjustSize) / (float)this->m_height;// 
	/// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v2[2].tv = v2[3].tv = (float)((ty + th)) / (float)this->m_height;
	else
		v2[2].tv = v2[3].tv = (float)((ty + th) - abjustSize) / (float)this->m_height;


	//�`��
	tdnPolygon::Render3D(v2, 2, this, dwFlags);


}

void tdn2DObj::Render3D(Vector3 pos, int w, int h, int tx, int ty, int tw, int th, u32 dwFlags)
{
	// ���ԁ@�g��(S)->��](R)->�ړ�(T)

	// 3D�p���_
	LVERTEX v2[4];

	// �Q�[���́��@�Ɓ@���@���w��
	Vector3 right(matView._11, matView._21, matView._31);// +�Ł��@-�Ł�
	Vector3 up(matView._12, matView._22, matView._32);// +�Ł��@-�Ł�
	right.Normalize();
	up.Normalize();

	// �r���{�[�h�̃x�N�g������]
	float cosan(cosf(angle)), sinan(sinf(angle));
	Vector3 turned_right, turned_up;
	turned_right = right * cosan + up * sinan;
	turned_right = right * cos(angle) + up * sin(angle);
	turned_up = -right * sin(angle) + up * cos(angle);

	// �g�嗦&��
	float maxSize;
	(m_width > m_height) ? ((float)(maxSize = (float)m_width)) : ((float)(maxSize = (float)m_height));
	float sizeX = (float)(w / maxSize)* scale;
	float sizeY = (float)(h / maxSize)* scale;

	// �|�W�V�����̏����ݒ�			  
	v2[0].x = -(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@�� = ����
	v2[0].y = -(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[0].z = -(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[1].x = +(turned_right.x*(sizeX)) + (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[1].y = +(turned_right.y*(sizeX)) + (turned_up.y * (sizeY));
	v2[1].z = +(turned_right.z*(sizeX)) + (turned_up.z * (sizeY));

	v2[2].x = -(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[2].y = -(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[2].z = -(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));

	v2[3].x = +(turned_right.x*(sizeX)) - (turned_up.x * (sizeY));//���@�s���ā@���@= �E��
	v2[3].y = +(turned_right.y*(sizeX)) - (turned_up.y * (sizeY));
	v2[3].z = +(turned_right.z*(sizeX)) - (turned_up.z * (sizeY));


	// ���_�𕽍s�ړ�(T)	
	v2[0].x += (float)pos.x;
	v2[0].y += (float)pos.y;
	v2[0].z += (float)pos.z;

	v2[1].x += (float)pos.x;
	v2[1].y += (float)pos.y;
	v2[1].z += (float)pos.z;

	v2[2].x += (float)pos.x;
	v2[2].y += (float)pos.y;
	v2[2].z += (float)pos.z;

	v2[3].x += (float)pos.x;
	v2[3].y += (float)pos.y;
	v2[3].z += (float)pos.z;


	// �|���S���̐F
	v2[0].color = v2[1].color = v2[2].color = v2[3].color = color;

	// �e�N�X�`�����̍��W(UV)
	if (isTurnOver == false)
	{
		v2[0].tu = v2[2].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);// �ق�̏���Uv2���W�����炷

		// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = (float)(tx + tw) / (float)this->m_width;
		else
			v2[1].tu = v2[3].tu = (float)((tx + tw) - abjustSize) / (float)this->m_width;

	}
	else //���]����Ȃ�
	{
		if (angle == 0.0f)
			v2[1].tu = v2[3].tu = (float)(tx) / ((float)this->m_width);//
		else
			v2[1].tu = v2[3].tu = (float)(tx + (float)abjustSize) / ((float)this->m_width);//

		v2[0].tu = v2[2].tu = (float)((tx + tw) - (float)abjustSize) / ((float)this->m_width);
	}

	// �c��uv2���W�͕ς�炸
	v2[0].tv = v2[1].tv = (float)(ty + abjustSize) / (float)this->m_height;// 
	/// ��]��K�p�����Ȃ�@��Ԃ̂�����C�����邽�ߏ������炷
	if (angle == 0.0f)
		v2[2].tv = v2[3].tv = (float)((ty + th)) / (float)this->m_height;
	else
		v2[2].tv = v2[3].tv = (float)((ty + th) - abjustSize) / (float)this->m_height;


	//�`��
	tdnPolygon::Render3D(v2, 2, this, dwFlags);


}




//-----------------------------------
///	���X�V
//-----------------------------------

// �g��k��
void tdn2DObj::SetScale(float scale)
{
	this->scale = scale;
}

// ��]
void tdn2DObj::SetAngle(float angle)
{
	this->angle = angle;
}

// �F���� 0~255�܂�
void tdn2DObj::SetARGB(BYTE A, BYTE R, BYTE G, BYTE B)
{

	// ARGB��char�^�l���ɓ���Ă���
	color = ARGB(A, R, G, B);
	// �e���_�ɔ��f
	//v[0].color = v[1].color = v[2].color = v[3].color = color;

}

// �F����(int�^) 0~255�܂�
void tdn2DObj::SetARGB(int A, int R, int G, int B)
{
	//�����_��Min~Max�͈̔͂ɗ}����@0~255
	auto Clamp = [](int val, int Min, int Max){
		return min(Max, max(val, Min));
	};

	BYTE a = Clamp(A, 0, 255);
	BYTE r = Clamp(R, 0, 255);
	BYTE g = Clamp(G, 0, 255);
	BYTE b = Clamp(B, 0, 255);

	/// ARGB��char�^�l���ɓ���Ă���
	// DWORD color = ARGB(a, r, g, b);
	color = ARGB(a, r, g, b);
	// �e���_�ɔ��f
	//v[0].color = v[1].color = v[2].color = v[3].color = color;

}

// 16�i���ł̐F����
void tdn2DObj::SetARGB(DWORD ARGB)
{
	color = ARGB;
	// �e���_�ɔ��f
	//v[0].color = v[1].color = v[2].color = v[3].color = color;

}

void tdn2DObj::SetAlpha(BYTE A)
{	
	DWORD orgColor = (color & 0x00ffffff);	
	color = (A << 24) | orgColor;
}

// 
void tdn2DObj::SetAlpha(int A)
{	
	BYTE a = (BYTE)Math::Clamp((float)A, 0, 255);

	DWORD orgColor = (color & 0x00ffffff);	
	color = (a << 24) | orgColor;
}

void tdn2DObj::SetRGB(BYTE R, BYTE G, BYTE B)
{
	DWORD orgColor = (color & 0xff000000);
	color = (R << 16) | (G << 8) | (B) | orgColor;

}

void tdn2DObj::SetRGB(int R, int G, int B)
{
	BYTE r = (BYTE)Math::Clamp((float)R, 0, 255);
	BYTE g = (BYTE)Math::Clamp((float)G, 0, 255);
	BYTE b = (BYTE)Math::Clamp((float)B, 0, 255);

	DWORD orgColor = (color & 0xff000000);
	color = (r << 16) | (g << 8) | (b) | orgColor;
}


void tdn2DObj::SetTurnOver(bool turnFlag)
{

	isTurnOver = turnFlag;

}

void tdn2DObj::SetShiftCenter(bool ShiftFlag)
{
	//�@�摜�����W�̒��S�Ɏ����Ă�����
	isShiftCenter = ShiftFlag;

}

// ��]�̒��S�ύX
void tdn2DObj::SetCenter(float x, float y)
{
	centerX = x;
	centerY = y;
}
