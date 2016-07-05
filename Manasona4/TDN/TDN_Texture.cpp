#include	"TDNLIB.h"

//*****************************************************************************
//
//		�e�N�X�`���֘A
//
//*****************************************************************************

Texture2D*	lpLastTexture;										// (?)�Ȃɂ���H
struct tdnTexture::TEXINFO tdnTexture::TexInfo[MAX_TEXTURE];	// �e�N�X�`���̏����i�[����z��

//	������
void	tdnTexture::Initialize()
{
	// IDirect3DTexture9��������
	for (int i = 0; i<MAX_TEXTURE; i++)
	{
		if (TexInfo[i].lpTexture) TexInfo[i].lpTexture->Release();	// �e�N�X�`���̏�񂪓����Ă�����Release
		TexInfo[i].lpTexture = NULL;								// IDirect3DTexture9��0��
		TexInfo[i].UseCount = 0;									// �g�p�J�E���g��0��
	}
}

//	�ǂݍ���
Texture2D* tdnTexture::Load(const char* filename, int flag)
{
	int			TexNo;
	HRESULT		hr;
	D3DFORMAT	fmt = D3DFMT_UNKNOWN;

	enum { NEW_TEXTURE = -1 };
	TexNo = NEW_TEXTURE;

	//	���łɓǂݍ��܂�Ă���e�N�X�`�����𒲂ׂ�
	for (int i = 0; i<MAX_TEXTURE; i++)
	{
		if (!TexInfo[i].lpTexture) continue;						// �e�N�X�`���̒��ɉ��������Ă��Ȃ��Ȃ�continue�Ŗ߂�
		if (lstrcmpi(TexInfo[i].filename, filename) != 0) continue;	// �w�啶���Ə���������ʂ����x�Ƀt�@�C�����̔�r�����Ĉ�v���Ă��Ȃ��Ȃ�continue�Ŗ߂�
		TexNo = i;													// ��x�ǂݍ��񂾃e�N�X�`��������Ȃ炻�̔z��ԍ����L�^���ă��[�v����o��
		//if (TexInfo[(MAX_TEXTURE - 1)].lpTexture){ MessageBox(0, "�ۑ��ł���ő�̃e�N�X�`�����܂œ��B���܂���!", "Texture", MB_OK); }
		break;
	}

	//	NEW_TEXTURE�̂܂܂�������V�K�ǂݍ��� 
	if (TexNo == NEW_TEXTURE)
	{
		if (flag == 1) fmt = D3DFMT_R8G8B8;

		//	�󂢂Ă�z�������
		for (TexNo = 0; TexNo<MAX_TEXTURE; TexNo++) if (!TexInfo[TexNo].lpTexture) break;
		
		//	�t�@�C�����ۑ�
		CopyMemory(TexInfo[TexNo].filename, filename, strlen(filename) + 1);

		//�@�ǂݍ��񂾉摜�̏����t�@�C��������炤
		D3DXGetImageInfoFromFile(filename, &TexInfo[TexNo].info);

		//	�e�N�X�`���ǂݍ���
		hr = D3DXCreateTextureFromFileEx(
			tdnSystem::GetDevice(),		// �r�f�I�J�[�h�̃f�o�C�X 
			filename,					// �t�@�C���̈ʒu
			TexInfo[TexNo].info.Width,	// ���Ŏ擾�����T�C�Y���g�p
			TexInfo[TexNo].info.Height,	// 
			1,							// �~�b�v�}�b�v�̃��x��
			0,							// �e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w�肷��B�~�b�v�}�b�v�����������ɂ��Ă�
			TexInfo[TexNo].info.Format,	// �摜�̃T�[�t�F�C�X�t�H�[�}�b�g���w��@�擾�����t�H�[�}�b�g���̗p
			D3DPOOL_DEFAULT,			// �o�b�t�@��ێ����郁����
			D3DX_FILTER_POINT,			// ���`���
			D3DX_FILTER_POINT,			// �~�b�v�}�b�v�ɂ����`��Ԃ�
			0x00000000,					// �����ɂ���F���w��ł���B���Ȃ��ꍇ��NULL(0)
			NULL,						// ���̉摜�̏����i�[����D3DXIMAGE_INFO�\���̂ւ̃|�C���^��n���B���ŕۑ����Ă���̂ł���Ȃ�
			NULL,						// �F�֌W�H
			&TexInfo[TexNo].lpTexture	// �e�N�X�`���ւ̃|�C���^
			);

		//�@�e�N�X�`�����Ȃ�������
		if (FAILED(hr)) return NULL;
	}
	TexInfo[TexNo].UseCount++;			// ���̃e�N�X�`���𗘗p���Ă���J�E���^������₷
	return TexInfo[TexNo].lpTexture;
}

// ����������̓ǂݍ���
Texture2D* tdnTexture::LoadMemory(const char* filename, const char* pArchiver)
{
	// �����o�ϐ�
	char* saveBuf;	// �폜�p
	char* seekBuf;	// �V�[�N�p
	int	  bufSize;	// �o�b�t�@�̃T�C�Y

	// �A�[�J�C�u���������ăf�[�^���󂯎��
	saveBuf = tdnUnArchiver::OpenArchiveFile(pArchiver, filename, bufSize);
	seekBuf = saveBuf;	// �V�[�N�Ɉڂ�

	int			TexNo;
	HRESULT		hr;
	D3DFORMAT	fmt = D3DFMT_UNKNOWN;

	enum { NEW_TEXTURE = -1 };
	TexNo = NEW_TEXTURE;

	//	���łɓǂݍ��܂�Ă���e�N�X�`�����𒲂ׂ�
	for (int i = 0; i<MAX_TEXTURE; i++)
	{
		if (!TexInfo[i].lpTexture) continue;						// �e�N�X�`���̒��ɉ��������Ă��Ȃ��Ȃ�continue�Ŗ߂�
		if (lstrcmpi(TexInfo[i].filename, filename) != 0) continue;	// �t�@�C�����̔�r�����Ĉ�v���Ă��Ȃ��Ȃ�continue�Ŗ߂�
		TexNo = i;													// ��x�ǂݍ��񂾃e�N�X�`��������Ȃ炻�̔z��ԍ����L�^���ă��[�v����o��
		//if (TexInfo[(MAX_TEXTURE - 1)].lpTexture){ MessageBox(0, "�ۑ��ł���ő�̃e�N�X�`�����܂œ��B���܂���!", "Texture", MB_OK); }
		break;
	}

	//	NEW_TEXTURE�̂܂܂�������V�K�ǂݍ��� 
	if (TexNo == NEW_TEXTURE)
	{

		//	�󂢂Ă�z�������
		for (TexNo = 0; TexNo<MAX_TEXTURE; TexNo++) if (!TexInfo[TexNo].lpTexture) break;

		//	�t�@�C�����ۑ�
		CopyMemory(TexInfo[TexNo].filename, filename, strlen(filename) + 1);

		//�@�ǂݍ��񂾉摜�̏����w�������[�x����擾
		D3DXGetImageInfoFromFileInMemory(seekBuf, sizeof(char)*bufSize, &TexInfo[TexNo].info);
		
		//	�w�������[�x����e�N�X�`���ǂݍ���
		hr = D3DXCreateTextureFromFileInMemoryEx(
			tdnSystem::GetDevice(),		// �r�f�I�J�[�h�̃f�o�C�X 
			seekBuf,					// �ǂݍ��ރ������[�̈ʒu
			sizeof(char)*bufSize,		// �������[(�o�b�t�@)�̃T�C�Y
			TexInfo[TexNo].info.Width,	// ���Ŏ擾�����T�C�Y���g�p
			TexInfo[TexNo].info.Height,	// 
			1,							// �~�b�v�}�b�v�̃��x��
			0,							// �e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w�肷��B�~�b�v�}�b�v�����������ɂ��Ă�
			TexInfo[TexNo].info.Format,	// �摜�̃T�[�t�F�C�X�t�H�[�}�b�g���w��@�擾�����t�H�[�}�b�g���̗p
			D3DPOOL_DEFAULT,			// �o�b�t�@��ێ����郁����
			D3DX_FILTER_POINT,			// ���`���
			D3DX_FILTER_POINT,			// �~�b�v�}�b�v�ɂ����`��Ԃ�
			0x00000000,					// �����ɂ���F���w��ł���B���Ȃ��ꍇ��NULL(0)
			NULL,						// ���̉摜�̏����i�[����D3DXIMAGE_INFO�\���̂ւ̃|�C���^��n���B���ŕۑ����Ă���̂ł���Ȃ�
			NULL,						// �F�֌W�H
			&TexInfo[TexNo].lpTexture	// �e�N�X�`���ւ̃|�C���^
			);

		//�@�e�N�X�`�����Ȃ�������
		if (FAILED(hr))
		{
			delete[] saveBuf;			// new�����������[�������������
			return NULL;
		}
	}
	TexInfo[TexNo].UseCount++;			// ���̃e�N�X�`���𗘗p���Ă���J�E���^������₷
	
	delete[] saveBuf;					// new�����������[�������������
	return TexInfo[TexNo].lpTexture;


}

//	���
void tdnTexture::Release(Texture2D* lpTexture)
{
	int	TexNo;

	enum { NOT_FIND = -1 };// ������Ȃ�����
	TexNo = NOT_FIND;
	// �����Ă����e�N�X�`�����Ɠ������̂����� 
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (!TexInfo[i].lpTexture) continue;				// �e�N�X�`���̒��ɉ��������Ă��Ȃ��Ȃ�continue�Ŗ߂�
		if (TexInfo[i].lpTexture != lpTexture) continue;	// �e�N�X�`���̏��ƈ�v���Ă��Ȃ��Ȃ�continue�Ŗ߂�
		TexNo = i;											// ��v�����炻�̔z��ԍ����L�^���ďo��
		break;
	}

	// NOT_FIND�ȊO->��������
	if (TexNo != NOT_FIND)
	{
		TexInfo[TexNo].UseCount--;					// ���̃e�N�X�`���𗘗p���Ă���2DObj�̃J�E���^�����炷	
		if (TexInfo[TexNo].UseCount > 0) return;	// �܂����̃e�N�X�`�����g���Ă���2DObj������̂Ȃ�c��
		//	�e�N�X�`������
		tdnSystem::GetDevice()->SetTexture(0, NULL);
		lpLastTexture = NULL;						// (?)�Ȃɂ���H
		//	�e�N�X�`�����
		if (TexInfo[TexNo].lpTexture->Release() != D3D_OK)
		{
			//	������s (?) �Ȃ񂩃V�F�[�_�[���g�����������s����
			//MessageBox(0, "�e�N�X�`���̉���Ɏ��s", "Texture", MB_OK);
		}
		TexInfo[TexNo].lpTexture = NULL;
		TexInfo[TexNo].UseCount = 0;
	}
	else 
	{
		// Load�֐��Ń��[�h����Ă��Ȃ��ꍇ�͌�����Ȃ��̂ŁA�����[�X�̂ݎ��s
		lpTexture->Release();
	}
}

