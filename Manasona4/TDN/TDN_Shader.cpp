#include	"TDNLIB.h"

/********************************************/
//	tdnShader	
/********************************************/

// (��)��Ŗ��O�� TransMat N S H �Ȃǂ��@GetParameterByName�Ŏw�肵�Ēu��

// ID3DXEffect
// https://msdn.microsoft.com/ja-jp/library/cc372622.aspx

/**
*	��D3DXCreateEffectFromFile
*	1.�f�o�C�X�@2.�t�@�C�����̃|�C���^�@
*   3.�v���v���Z�b�T�}�N���̒�`�@4.�I�v�V�����C���^�[�t�F�C�X�|�C���^�@
*	5.Flag(�R���p�C���I�v�V����)�@6.Pool�̈����ANULL�ɂ���Ƌ��L����Ȃ�
*�@ 7.�R���p�C�����ꂽ�G�t�F�N�g���i�[���ꂽ�o�b�t�@�ւ̃|�C���^��Ԃ��@8.�R���p�C���G���[		
*	
**/

/* �������E��� */
tdnShader::tdnShader(char* filename)
{
	LPDEVICE lpDevice = tdnSystem::GetDevice();

	// �t�@�C�����`�F�b�N
	char fileName[256];
	if (filename[strlen(filename) - 3] != '.') sprintf(fileName, "%s.fx", filename);
	else sprintf(fileName, "%s", filename);

	// �V�F�[�_�[�̓ǂݍ���
	HRESULT	hr;					// �G���[��
	LPD3DXBUFFER pErr = NULL;	// �R���p�C���G���[�̏ڍׂ��i�[
	hr = D3DXCreateEffectFromFile(lpDevice, fileName, NULL, NULL, 0, NULL, &pShader, &pErr);
	if (FAILED(hr))
	{
		// �t�@�C�������ԈႦ�Ă����ꍇ�ApErr��NULL�ɂȂ��Ă��܂��A�A�N�Z�X�ᔽ���������Ă��܂��̂ŁA�΍�
		MyAssert(pErr, "�G���[: �V�F�[�_�[�t�@�C�������ԈႦ�Ă��܂�\n�G���[�t�@�C����%s", filename);

		// �o�͂ɃG���[�\��
		char*	szBuffer = new char[pErr->GetBufferSize() + 32 ];
		sprintf(szBuffer, "\terrors: %s\n", (char*)pErr->GetBufferPointer());
		OutputDebugString(szBuffer);
		delete[] szBuffer;
		return;
	}

	// ��b�ϐ��̓ǂݍ���
	hmWVP = pShader->GetParameterByName(NULL, "WVPMatrix");	//�e�\���̃p�����[�^�̃n���h��NULL�ōŏ�K�@�p�����[�^�� 
	htexDecale = pShader->GetParameterByName(NULL, "Texture");
}

tdnShader::~tdnShader()
{
	pShader->Release();
}


/*********************/
// �p�����[�^�[�ݒ�
/*********************/

void tdnShader::SetWVPMatrix(Matrix* wvpMat){ pShader->SetMatrix(hmWVP, wvpMat); };
void tdnShader::SetWVPMatrix(Matrix& wvpMat){ pShader->SetMatrix(hmWVP, &wvpMat); };

void tdnShader::SetDecaleTexture(Texture2D* decaleTex){ pShader->SetTexture(htexDecale, decaleTex); }
void tdnShader::SetDecaleTexture(Texture2D& decaleTex){ pShader->SetTexture(htexDecale, &decaleTex); }

void tdnShader::SetValue(char* name, Texture2D* tex){ pShader->SetTexture(name, tex); }
void tdnShader::SetValue(char* name, Texture2D& tex){ pShader->SetTexture(name, &tex); }

void tdnShader::SetValue(char* name, tdn2DObj* tex){ pShader->SetTexture(name, tex->GetTexture()); }
void tdnShader::SetValue(char* name, tdn2DObj& tex){ pShader->SetTexture(name, tex.GetTexture()); }

void tdnShader::SetValue(char* name, Matrix* mat){ pShader->SetMatrix(name, mat); }
void tdnShader::SetValue(char* name, Matrix& mat){ pShader->SetMatrix(name, &mat); }

void tdnShader::SetValue(char* name, D3DXVECTOR4* v){ pShader->SetVector(name, v); }
void tdnShader::SetValue(char* name, D3DXVECTOR4& v){ pShader->SetVector(name, &v); }

void tdnShader::SetValue(char* name, Vector3* v){ pShader->SetFloatArray(name, (float*)v, 3); }
void tdnShader::SetValue(char* name, Vector3& v){ pShader->SetFloatArray(name, (float*)&v, 3); }


void tdnShader::SetValue(char* name, float f){ pShader->SetFloat(name, f); }
// void tdnShader::SetValue(char* name, float2 f){ pShader->SetFloatArray(name, f, 2); }// Float2
void tdnShader::SetValue(char* name, int d){ pShader->SetInt(name, d); }
void tdnShader::SetValue(char* name, DWORD d){ pShader->SetValue(name, &d, 4); }






