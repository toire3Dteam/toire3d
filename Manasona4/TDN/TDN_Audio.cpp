//*****************************************************************************************************************************
#include	"TDNLIB.h"
// acm�֐�(MP3���̈��k�����f�[�^�̉𓀂��s��)�p�̃C���N���[�h
#include<mmreg.h>
#include<msacm.h>


HRESULT result_sound;

//**************************************************************************************************************
//
//		�T�E���h�o�b�t�@
//
//**************************************************************************************************************
const int tdnSoundBuffer::READBYTE = 1024;	// �����̒l���傫���Ȃ�ɂ�ēǂݍ��ݑ��x�͏オ�邪�A���ΓI�Ƀ������I�[�o�[�̉\�����オ��

//**************************************************************************************************************
//
//**************************************************************************************************************
tdnSoundBuffer::tdnSoundBuffer(LPDIRECTSOUND8 lpDS, LPCSTR filename, bool b3D)
{
	/*	WAV�t�@�C���̃��[�h	*/
	lpWBuf = LoadFile((char*)filename, &size, &wfx);
	/*	���[�h���s	*/
	if (lpWBuf == nullptr){
		return;
	}

	/* �t�@�C�����ۑ� */
	sprintf_s(wav_file_path, sizeof(wav_file_path), "%s", filename);

	Initialize(lpDS, lpWBuf, size, &wfx, b3D);
}

void tdnSoundBuffer::Initialize(LPDIRECTSOUND8 lpDS, unsigned char* data, DWORD size, LPWAVEFORMATEX fmt, bool b3D)
{
	DSBUFFERDESC	dsbd{};
	LPVOID			lpbuf1, lpbuf2;
	DWORD			dwbuf1, dwbuf2;

	size = size;
	wfx = *fmt;
	lpWBuf = data;

	lpBuf3D = nullptr;
	lpBuf = nullptr;

	/* �񎟃o�b�t�@�쐬	*/
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	if (b3D) dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D | DSBCAPS_CTRLFX;	// CTRL_FX���A�T�E���h����ɕK�v�ȃt���O��ON�ɂ���(CTRL3D��ON�ɂ����CTRLPAN���g���Ȃ�)
	else dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = &wfx;

	/* �{����DIRECTSOUNDBUFFER���g���Ă������ABUFFER8�ɐ؂�ւ��邽�߂ɁA��UBUFFER����遨����BUFFER��QueriInterface�̗����BUFFER8���쐬 */
	LPDIRECTSOUNDBUFFER lpWork;
	lpDS->CreateSoundBuffer(&dsbd, &lpWork, nullptr);
	lpWork->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&lpBuf);
	lpWork->Release();	// �p�ς�

	MyAssert(!b3D || result_sound != E_INVALIDARG || dsbd.lpwfxFormat->nChannels != 2, "3D�T�E���h������X�e���I�����͎g���Ȃ���I\n���m�����ɕϊ����Ă�");
	//if (b3D && result == E_INVALIDARG && dsbd.lpwfxFormat->nChannels == 2)

	if (result_sound != DS_OK)
	{
		return;
	}

	lpBuf3D = nullptr;
	if (b3D == true)
	{
		/* �T�E���h�o�b�t�@����3D�o�b�t�@�ɕϊ�&�쐬 */
		lpBuf->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&lpBuf3D);
		/* 3D�p�����[�^������ */
		this->SetAll3D(DS3D_DEFAULTMAXDISTANCE, DS3D_DEFAULTMINDISTANCE, Vector3(0, 0, 0), Vector3(0, 0, -1), DS3D_MAXCONEANGLE, DS3D_DEFAULTCONEOUTSIDEVOLUME, Vector3(0, 0, 0));
	}

	/* �񎟃o�b�t�@�̃��b�N	*/
	lpBuf->Lock(0, size, &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);
	/* �����f�[�^�̐ݒ�	*/
	memcpy(lpbuf1, lpWBuf, dwbuf1);
	if (dwbuf2 != 0) memcpy(lpbuf2, lpWBuf + dwbuf1, dwbuf2);
	/* �����f�[�^�̉��	*/
	GlobalFree(lpWBuf);
	/* �񎟃o�b�t�@�̃��b�N����	*/
	lpBuf->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2);

	/* ���쏉���� */
	PlayCursor = 0;
	BufferSize = size;
	format = wfx;
}

void tdnSoundBuffer::Create_and_copy(LPDIRECTSOUND8 lpDS, LPCSTR filename, bool b3D, tdnSoundBuffer **buffers, int dst, int count)
{
	DSBUFFERDESC	dsbd{};
	LPVOID			lpbuf1, lpbuf2;
	DWORD			dwbuf1, dwbuf2;


	//===========================================================
	//		�R�s�[���쐬
	//===========================================================
	buffers[dst]->lpBuf3D = nullptr;
	buffers[dst]->lpBuf = nullptr;

	/*	WAV�t�@�C���̃��[�h	*/
	buffers[dst]->lpWBuf = buffers[dst]->LoadWAV(filename, &buffers[dst]->size, &buffers[dst]->wfx);

	/* �t�@�C�����ۑ� */
	sprintf_s(buffers[dst]->wav_file_path, sizeof(buffers[dst]->wav_file_path), "%s", filename);

	/* �񎟃o�b�t�@�쐬	*/
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	if (b3D) dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D | DSBCAPS_CTRLFX;
	else dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX;
	dsbd.dwBufferBytes = buffers[dst]->size;
	dsbd.lpwfxFormat = &buffers[dst]->wfx;
	
	LPDIRECTSOUNDBUFFER lpWork;
	result_sound = lpDS->CreateSoundBuffer(&dsbd, &lpWork, nullptr);

	MyAssert(result_sound != DSERR_BUFFERTOOSMALL, "CreateSoundBuffer:[DSERR_BUFFER_TOSMALL]\n�G���[�t�@�C����:%s\n�Ƃ�܍Đ����Ԃ𒷂����Ă݂�", filename);

	result_sound = lpWork->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffers[dst]->lpBuf);	// LPDIRECTSOUNDBUFFER��LPDIRECTSOUNDBUFFER8
	lpWork->Release();	// �p�ς�

	MyAssert(!b3D || result_sound != E_INVALIDARG || dsbd.lpwfxFormat->nChannels != 2, "3D�T�E���h������X�e���I�����͎g���Ȃ���I\n���m�����ɕϊ����Ă�");

	if (result_sound != DS_OK)
	{
		return;
	}

	buffers[dst]->lpBuf3D = nullptr;
	if (b3D)
	{
		buffers[dst]->lpBuf->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&buffers[dst]->lpBuf3D);
		buffers[dst]->SetAll3D(DS3D_DEFAULTMAXDISTANCE, DS3D_DEFAULTMINDISTANCE, Vector3(0, 0, 0), Vector3(0, 0, -1), DS3D_MAXCONEANGLE, DS3D_DEFAULTCONEOUTSIDEVOLUME, Vector3(0, 0, 0));
	}

	/* �񎟃o�b�t�@�̃��b�N	*/
	buffers[dst]->lpBuf->Lock(0, buffers[dst]->size, &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);
	/* �����f�[�^�̐ݒ�	*/
	memcpy(lpbuf1, buffers[dst]->lpWBuf, dwbuf1);
	if (dwbuf2 != 0) memcpy(lpbuf2, buffers[dst]->lpWBuf + dwbuf1, dwbuf2);
	/* �����f�[�^�̉��	*/
	//GlobalFree(lpWBuf);	// ���̕����R�����g�A�E�g����̂ɑS�Ă��l�܂��Ă�
	/* �񎟃o�b�t�@�̃��b�N����	*/
	buffers[dst]->lpBuf->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2);

	/* ���쏉���� */
	buffers[dst]->PlayCursor = 0;
	buffers[dst]->BufferSize = buffers[dst]->size;
	buffers[dst]->format = buffers[dst]->wfx;


	//===========================================================
	//		�R�s�[��쐬
	//===========================================================
	for (int no = dst + 1; no < dst + count; no++)
	{

		DSBUFFERDESC	dsbd2{};
		LPVOID			lpbuf1 = nullptr, lpbuf2 = nullptr;
		DWORD			dwbuf1 = 0, dwbuf2 = 0;

		buffers[no]->lpBuf3D = nullptr;
		buffers[no]->lpBuf = nullptr;

		/* �񎟃o�b�t�@�쐬	*/
		dsbd2.dwSize = sizeof(DSBUFFERDESC);
		if (b3D == true) dsbd2.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D | DSBCAPS_CTRLFX;
		else dsbd2.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX;
		dsbd2.dwBufferBytes = buffers[dst]->size;
		dsbd2.lpwfxFormat = &buffers[dst]->wfx;

		LPDIRECTSOUNDBUFFER lpWork2;
		result_sound = lpDS->CreateSoundBuffer(&dsbd2, &lpWork2, nullptr);

		if (result_sound == E_OUTOFMEMORY)
		{
			MessageBox(0, "wav�t�@�C�����ǂݍ��܂ꂷ���ă�����������Ȃ��Ȃ��Ă��B�Z�b�g�̐������炷���Awav�t�@�C���̒�����Z�����Ă�", nullptr, MB_OK);
			assert(0);
			return;
		}

		result_sound = lpWork2->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffers[no]->lpBuf);	// LPDIRECTSOUNDBUFFER��LPDIRECTSOUNDBUFFER8
		lpWork2->Release();	// �p�ς�

		buffers[no]->lpBuf3D = nullptr;
		if (b3D)
		{
			result_sound = buffers[no]->lpBuf->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*)&buffers[no]->lpBuf3D);
			buffers[no]->SetAll3D(DS3D_DEFAULTMAXDISTANCE, DS3D_DEFAULTMINDISTANCE, Vector3(0, 0, 0), Vector3(0, 0, -1), DS3D_MAXCONEANGLE, DS3D_DEFAULTCONEOUTSIDEVOLUME, Vector3(0, 0, 0));
		}

		/* �񎟃o�b�t�@�̃��b�N	*/
		buffers[no]->lpBuf->Lock(0, buffers[dst]->size, &lpbuf1, &dwbuf1, &lpbuf2, &dwbuf2, 0);
		/* �����f�[�^�̐ݒ�	*/
		memcpy(lpbuf1, buffers[dst]->lpWBuf, dwbuf1);
		if (dwbuf2 != 0) memcpy(lpbuf2, buffers[dst]->lpWBuf + dwbuf1, dwbuf2);
		/* �����f�[�^�̉��	*/
		//GlobalFree(buffers[no].lpWBuf);
		/* �񎟃o�b�t�@�̃��b�N����	*/
		buffers[no]->lpBuf->Unlock(lpbuf1, dwbuf1, lpbuf2, dwbuf2);

		/* ���쏉���� */
		buffers[no]->PlayCursor = 0;
		buffers[no]->BufferSize = buffers[dst]->size;
		buffers[no]->format = buffers[dst]->wfx;
	}

	/* �R�s�[�������f�[�^�̉��	*/
	GlobalFree(buffers[dst]->lpWBuf);
}

tdnSoundBuffer::~tdnSoundBuffer()
{
	if (lpBuf != nullptr)lpBuf->Release();
	if (lpBuf3D != nullptr) lpBuf3D->Release();
}

//**************************************************************************************************************
//		�v�`�u�t�@�C���̓ǂݍ���
//**************************************************************************************************************
LPBYTE tdnSoundBuffer::LoadFile(LPSTR fname, LPDWORD size, LPWAVEFORMATEX wfx)
{
	//	�t�@�C���ǂݍ���
	char* ext = &fname[lstrlen(fname) - 4];
	if (lstrcmpi(ext, ".wav") == 0) return LoadWAV(fname, size, wfx);
	else if (lstrcmpi(ext, ".owd") == 0) return LoadOWD(fname, size, wfx);
	else return nullptr;
}

LPBYTE tdnSoundBuffer::LoadWAV(LPCSTR fname, LPDWORD size, LPWAVEFORMATEX wfx)
{
	// �o�C�i���ǂݍ���
	std::ifstream infs(fname, std::ios::binary);
	MyAssert(infs, "�G���[�t�@�C����[%s]\n����:wav�t�@�C���������Ă��Ȃ����Awav�t�@�C�������Ԉ���Ă����", fname);

	char chunkID[4];
	int ChunkSize;
	BYTE *buf{};

	/* RIFF�`�����N�N�� */
	infs.read(chunkID, 4);
	MyAssert(
		chunkID[0] == 'R'&&
		chunkID[1] == 'I'&&
		chunkID[2] == 'F'&&
		chunkID[3] == 'F',
		"�G���[�t�@�C����[%s]\n����:wav�t�@�C���������Ă��Ȃ����Awav�t�@�C�������Ԉ���Ă����", fname);

	infs.read((char*)&ChunkSize, 4);		// RIFF�`�����N�w�b�_�̃T�C�Y
	infs.read(chunkID, 4);					// �t�H�[�}�b�g�^�C�v
	MyAssert(
		chunkID[0] == 'W'&&
		chunkID[1] == 'A'&&
		chunkID[2] == 'V'&&
		chunkID[3] == 'E',
		"�G���[�t�@�C����[%s]\n����:����wav����Ȃ�", fname);

	/* �t�H�[�}�b�g�`�����N�N�� */
	infs.read(chunkID, 4);


	// ��Broadcast Wave Format���m�F
	if (chunkID[0] == 'b' &&
		chunkID[1] == 'e' &&
		chunkID[2] == 'x' &&
		chunkID[3] == 't')
	{
		/* bext chunk �Ƃ́@�uBroad Audio Extension chunk�v�ŁABWF�t�@�C���Ǝ��̃`�����N�B���O�I�ɂ��Ԃ�g������ˁH�@�����A�����ł͎g��Ȃ��̂œǂݔ�΂����� */

		// �t�H�[�}�b�g�`�����N�܂œǂݔ�΂�
		const char FMTCHUNKID[5] = "fmt ";
		int i = 0;
		while (i < 4)
		{
			infs.read(&chunkID[i], 1);
			i = (chunkID[i] == FMTCHUNKID[i]) ? i + 1 : 0;
		}
	}


	MyAssert(
		chunkID[0] == 'f'&&
		chunkID[1] == 'm'&&
		chunkID[2] == 't',
		"�G���[�t�@�C����[%s]\n����:�t�H�[�}�b�g�`�����N���ŃG���[", fname);

	infs.read((char*)&ChunkSize, 4);		// �t�H�[�}�b�g�`�����N�w�b�_�̃T�C�Y

	// �t�H�[�}�b�g�z���グ
	//infs.read((char*)wfx, sizeof(WAVEFORMATEX));
	infs.read((char*)&wfx->wFormatTag, 2);				// �t�H�[�}�b�gID
	infs.read((char*)&wfx->nChannels, 2);				// �`�����l����
	infs.read((char*)&wfx->nSamplesPerSec, 4);			// �T���v�����O���[�g
	infs.read((char*)&wfx->nAvgBytesPerSec, 4);			// �f�[�^���x byte/sec
	infs.read((char*)&wfx->nBlockAlign, 2);				// �u���b�N�T�C�Y
	infs.read((char*)&wfx->wBitsPerSample, 2);			// �T���v��������̃r�b�g��
	//infs.read((char*)&wfx->cbSize, 2);					// �悭�킩���

	MyAssert(
		wfx->wFormatTag == WAVE_FORMAT_PCM,
		"�G���[�t�@�C����[%s]\n����:�t�H�[�}�b�g��wav����Ȃ�", fname);

	/* �f�[�^�`�����N�N�� */

	// �`�����NID����
	const char DATACHUNKID[5] = "data";
	for (int i = 0; i < 4; i++)
	{
		do
		{
			infs.read(&chunkID[i], 1);
		} while (chunkID[i] != DATACHUNKID[i]);
	}

	MyAssert(
		chunkID[0] == 'd'&&
		chunkID[1] == 'a'&&
		chunkID[2] == 't'&&
		chunkID[3] == 'a',
		"�G���[�t�@�C����[%s]\n����:�f�[�^�`�����N���ŃG���[�B�t�H�[�}�b�g�̂��ꂪ������������Ȃ�", fname);

	infs.read((char*)size, 4);				// �f�[�^�T�C�Y
	buf = (LPBYTE)GlobalAlloc(LPTR, *size);

	// ���f�[�^�z���グ
	DWORD remain = *size; // �������ނׂ��c��̃o�C�g��
	BYTE work[READBYTE]; 

	// xBytes���ǂݍ���(�������p���N�����Ȃ��悤��)
	for (int i = 0; remain > 0; i++)
	{
		int readSize = min(READBYTE, remain);
		infs.read((char*)work, readSize);
		remain -= readSize;

		memcpy(&buf[i*READBYTE], work, readSize);
	}

	return buf;
}

LPBYTE tdnSoundBuffer::LoadOWD(LPCSTR fname, LPDWORD size, LPWAVEFORMATEX wfx)
{
	// �o�C�i���ǂݍ���
	std::ifstream infs(fname, std::ios::binary);

	BYTE *buf = nullptr;

	/* �o�[�W�����`�F�b�N */
	BYTE ver;
	infs.read((char*)&ver, 1);

	if (ver == 0)
	{
		/* �t�H�[�}�b�g�z���グ */
		infs.read((char*)wfx, sizeof(WAVEFORMATEX));

		/* WAV�T�C�Y�擾 */
		infs.read((char*)size, 4);
		buf = (LPBYTE)GlobalAlloc(LPTR, *size);

		/* �f�[�^�z���グ */
		DWORD remain = *size; // �������ނׂ��c��̃o�C�g��
		BYTE work[READBYTE];

		// xBytes���ǂݍ���(�������p���N�����Ȃ��悤��)
		for (int i = 0; remain > 0; i++)
		{
			int readSize = min(READBYTE, remain);
			infs.read((char*)work, readSize);
			remain -= readSize;

			memcpy(&buf[i*READBYTE], work, readSize);
		}
	}


	else if (ver == 1)
	{

	}

	return buf;
}

//-------------------------------------------------------------
//	MP3
//-------------------------------------------------------------
LPBYTE tdnSoundBuffer::LoadMP3(LPCSTR fname, LPDWORD size, LPWAVEFORMATEX wfx)
{
	// �o�C�i���ǂݍ���
	std::ifstream infs(fname, std::ios::binary);
	MyAssert(infs, "�G���[�t�@�C����[%s]\n����:MP3�t�@�C�������Ԉ���Ă��邩�A�t�@�C�������݂��Ȃ���", fname);

	BYTE *buf = nullptr;

	// �t���[���w�b�_�͂S�o�C�g(�R�Q�r�b�g�j�ŏo���Ă���B�̂ŁADWORD�ł����؂�ɓǂ�ŕ����Ă���
	DWORD receive;
	infs.read((char*)&receive, 32);

	WORD frame_synchronism = (receive >> 21) & 0x000007ff;	// �t���[�������B�I�[��1�r�b�g(11�r�b�g)
	MyAssert(frame_synchronism == 0x000007ff, "�G���[�t�@�C����[%s]\n����:�`����MP3�ł����A���g��MP3�łȂ��\��������܂�", fname);

	BYTE ver = (receive >> 19) & 0x00000003;	// MPEG Audio �̃o�[�W����ID(2�r�b�g)

	enum class MPEG_VERSION
	{
		MPEG_V2_5,	// �o�[�W����2.5
		RESERVE,	// �\��炵��
		MPEG_V2,	// �o�[�W����2
		MPEG_V1		// �o�[�W����1
	}mpeg_ver;
	switch (ver & 0x03)
	{
	case 0x00:mpeg_ver = MPEG_VERSION::MPEG_V2_5; break;
	case 0x01:mpeg_ver = MPEG_VERSION::RESERVE; break;
	case 0x02:mpeg_ver = MPEG_VERSION::MPEG_V2; break;
	case 0x03:mpeg_ver = MPEG_VERSION::MPEG_V1; break;
	}


	BYTE layer			= (receive >> 17)	& 0x00000003;	// ���C���[�̎��(2�r�b�g)
	BOOL CRCerror		= (receive >> 16)	& 0x00000001;	// CRC�G���[�����ɂ��ی�(1�r�b�g)
	BYTE bit_rate		= (receive >> 12)	& 0x0000000f;	// �r�b�g���[�g(4�r�b�g)
	BYTE sampling_rate	= (receive >> 10)	& 0x0000003;	// �T���v�����O���[�g(2�r�b�g)
	BOOL padding		= (receive >> 9)	& 0x00000001;	// �p�f�B���O(1�r�b�g)	0:�Ȃ� 1:����
	BOOL private_bit	= (receive >> 8)	& 0x00000001;	// �v���C�x�[�g�r�b�g�̎g�p�s�g�p(1�r�b�g) 0:�s�g�p 1:�g�p
	BYTE channel_mode	= (receive >> 6)	& 0x0000003;	// �`�����l�����[�h(2�r�b�g)
	BYTE mode_extention = (receive >> 4)	& 0x0000003;	// ���[�h�G�N�X�e���V�����B�`�����l�����[�h��01�̏ꍇ�ɗL���A���̏ꍇ0�ɐݒ�(2�r�b�g)
	BOOL copy_light		= (receive >> 3)	& 0x0000001;	// ���쌠(1�r�b�g)	0:�ی�Ȃ� 1:�ی삠��
	BOOL original		= (receive >> 2)	& 0x00000001;	// �I���W�i��(1�r�b�g)	0:�R�s�[ 1:�I���W�i��
	BYTE emphasis		= receive			& 0x00000003;	// �G���t�@�V�X�B�M�������̈��(2�r�b�g)

	/*
	MPEG Audio �̃o�[�W����ID
	[00] - MPEG v2.5 / [01] - (�\��)
	[10] - MPEG v2 / [11] - MPEG v1

	���C���[�̎��
	[00] - (�\��) / [01] - Layer3
	[10] - Layer2 / [11] - Layer1

	CRC�G���[�����ɂ��ی�
	[0] - �ی삠��B���̏ꍇ�A16�r�b�g��CRC���t���[���w�b�_�̌�ɕt�������
	[1] - �ی�Ȃ�

	*/

	// �T���v�����O���[�g�\
	static const int sampring_rate_list[3][3]=
	{
		{ 44100, 22050, 11025 },
		{ 48000, 24000, 12000 },
		{ 32000, 16000, 8000 }
	};

	// �r�b�g���[�g�\(����������)
	static const int bit_rate_list[14][5] =
	{
		{ 32,	32,		32,		32,		8 },
		{ 64,	48,		40,		48,		16 },
		{ 96,	56,		48,		56,		24 },
		{ 128,	64,		56,		64,		32 },
		{ 160,	80,		64,		80,		40 },
		{ 192,	96,		80,		96,		48 },
		{ 224,	112,	96,		112,	56 },
		{ 256,	128,	112,	128,	64 },
		{ 288,	160,	128,	144,	80 },
		{ 320,	192,	160,	160,	96 },
		{ 352,	224,	192,	176,	112 },
		{ 384,	256,	224,	192,	128 },
		{ 416,	320,	256,	224,	144 },
		{ 448,	384,	320,	256,	160 }
	};


	// �T���v�����O���[�g�ݒ�
	DWORD samplesPerSec;
	{
		int column(0), row(0);
		// ���X�g�̗�
		switch (mpeg_ver)
		{
		case MPEG_VERSION::MPEG_V1:column = 0; break;
		case MPEG_VERSION::MPEG_V2:column = 1; break;
		case MPEG_VERSION::MPEG_V2_5:column = 2; break;
		}

		// ���X�g�̍s
		switch (sampling_rate & 0x03)
		{
		case 0x00:row = 0; break;
		case 0x01:row = 1; break;
		case 0x02:row = 2; break;
		case 0x03:break;	// �u�\��v�炵���B�悭�������
		}

		samplesPerSec = sampring_rate_list[row][column];			// �T���v�����O���[�g
	}

	// �r�b�g���[�g�ݒ�
	DWORD bitRate;
	{
		int column(0), row(0);
		// ���X�g�̗�
		column = 4;		// ���󃊃X�g�̉E�[�̐����ȊO�̐������܂����Ȃ����g��Ȃ������Ȃ̂ō��͉E�[���w��

		// ���X�g�̍s
		switch (bit_rate & 0x0f)
		{
		case 0x01:row = 0; break;
		case 0x02:row = 1; break;
		case 0x03:row = 2; break;
		case 0x04:row = 3; break;
		default:assert(0); break;	// �܂��r�b�g���[�g��40�ȏ���Ă���̂��E�E�E�H
		}
		bitRate = bit_rate_list[row][column];				// �T���v��������̃r�b�g��
	}

	MPEGLAYER3WAVEFORMAT    mwf;        // Source MP3

	mwf.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
	mwf.wfx.nChannels = (channel_mode & 0x3) ? 1 : 2;
	mwf.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
	mwf.wfx.nBlockAlign = 1;
	mwf.wfx.wBitsPerSample = 0;
	mwf.wfx.nSamplesPerSec = samplesPerSec;
	mwf.wfx.nAvgBytesPerSec = bitRate * 1000 / 8;

	mwf.wID = MPEGLAYER3_ID_MPEG;
	mwf.fdwFlags = padding ? MPEGLAYER3_FLAG_PADDING_ON : MPEGLAYER3_FLAG_PADDING_OFF;

	mwf.nFramesPerBlock = 1;

	DWORD frameSize;

	// MPEG�o�[�W����1
	if (mpeg_ver == MPEG_VERSION::MPEG_V1)
	{
		frameSize = (DWORD)(144000 * bitRate / samplesPerSec) + padding;
	}
	// MPEG�o�[�W����2�E�o�[�W����2.5
	else
	{
		frameSize = (DWORD)(72000 * bitRate / samplesPerSec) + padding;
	}
	mwf.nBlockSize = (WORD)(frameSize * mwf.nFramesPerBlock);
	mwf.nCodecDelay = 0x571;

	
	wfx->wFormatTag = WAVE_FORMAT_PCM;

	// �t�H�[�}�b�g�ϊ����\���ǂ������ׂ܂��B
	MMRESULT MMresult = acmFormatSuggest(
		NULL,
		(LPWAVEFORMATEX)&mwf,
		wfx,
		sizeof(WAVEFORMATEX),
		ACM_FORMATSUGGESTF_WFORMATTAG);
	MyAssert(MMresult == MMSYSERR_NOERROR, "�G���[: MP3�ϊ���acmFormatSuggest�֐�");

	// �X�g���[���n���h��
	HACMSTREAM has;

	// ���t�H�[�}�b�g�ϊ��X�g���[���I�[�v���I�I�I
	MMresult = acmStreamOpen(
		&has,								// �ϊ��ɗp����X�g���[���n���h���ւ̃|�C���^�[�ł��B�X�g���[�����I�[�v�������ƃn���h�����߂�܂��B
		NULL,								// �ϊ����s���h���C�o�[���w�肵�܂��BNULL���w�肷��ƁA�V�X�e���͍ł��K���Ǝv����h���C�o�[���������܂��B
		(LPWAVEFORMATEX)&mwf,				// �ϊ����̃E�F�[�u�t�H�[�}�b�g���w�肵�܂��BacmFormatSuggest()�ɓn�������̂Ɠ������̂��g���܂��B
		wfx,								// �ϊ���̃E�F�[�u�t�H�[�}�b�g���w�肵�܂��BacmFormatSuggest()�ɂ���Đݒ肳�ꂽ���̂��g���܂��B
		NULL,								// �ϊ�����ŗp����t�B���^�[���w�肵�܂��B�t�H�[�}�b�g�ϊ��̎��͗p���Ȃ��̂�NULL���w�肵�܂��B
		(DWORD_PTR)tdnSystem::GetWindow(),	// �ϊ������񓯊��ōs���ꍇ�A�A�v���P�[�V�����͕ϊ������̏I����m�邱�Ƃ��ł��܂��B
		NULL,								// �R�[���o�b�N�֐��œn����郆�[�U�[�C���X�^���X�f�[�^�ł��B�R�[���o�b�N�֐��ȊO�̕����ł͎g���܂���̂�NULL���w�肵�܂��B
		CALLBACK_WINDOW);					// �ϊ��X�g���[�����J�����߂̃t���O�ňȉ��̂��̂���`����Ă��܂��B
	MyAssert(MMresult == MMSYSERR_NOERROR, "�G���[: MP3�ϊ���acmStreamOpen�֐�");


	// �ϊ��O��̃T�C�Y�v�Z
	MMresult = acmStreamSize(
		has,						// �X�g���[���n���h��
		mwf.nBlockSize,				// �ϊ��O�̃T�C�Y
		size,						// �ϊ���̃T�C�Y
		ACM_STREAMSIZEF_SOURCE);

	// ACMSTREAMHEADER�\���̂���������B
	ACMSTREAMHEADER ash{};
	ash.cbStruct = sizeof(ACMSTREAMHEADER);

	acmStreamPrepareHeader(
		has,		// �X�g���[���n���h��
		&ash,		// ��������ACMSTREAMHEADER�\���̂̃A�h���X�B
		0			// �\��ς݂Ȃ̂�0���w�肷��炵��
		);


	// ���t�H�[�}�b�g�ϊ��I�I
	acmStreamConvert(
		has,						// �X�g���[���n���h��
		&ash,						// ACMSTREAMHEADER�\���̂̃A�h���X�BacmStreamPrepareHeader�ŏ�������K�v������
		ACM_STREAMCONVERTF_END);	// ���������Ȃ����ߕs�������A���ꂪ������ۂ�
	// MyAssert(ash.cbDstLengthUsed > 0, "�G���[: MP3�ϊ���acmStreamConvert�֐�");	// �񓯊��̏ꍇ�̂݁A����Ă����̃G���[�`�F�b�N�͕s�̗p


	/* �f�[�^�z���グ */
	buf = (LPBYTE)GlobalAlloc(LPTR, *size);
	DWORD remain = *size; // �������ނׂ��c��̃o�C�g��
	BYTE work[READBYTE];

	// xBytes���ǂݍ���(�������p���N�����Ȃ��悤��)
	for (int i = 0; remain > 0; i++)
	{
		int readSize = min(READBYTE, remain);
		memcpy(work, &ash.pbDst[i*READBYTE], readSize);
		remain -= readSize;

		memcpy(&buf[i*READBYTE], work, readSize);
	}

	// SCMDTREAMHEADER�̊J��
	acmStreamUnprepareHeader(
		has,	// �X�g���[���n���h��
		&ash,	// �������ACMSTREAMHEADER�\���̂̃A�h���X�B
		0		// �\��ς݂Ȃ̂�0���w�肷��炵��
		);

	// �t�H�[�}�b�g�ϊ��X�g���[�������
	acmStreamClose(has, 0);

	return buf;
}


//**************************************************************************************************************
//	�Đ��Ǘ�
//**************************************************************************************************************
//-------------------------------------------------------------
//	�Đ�	
//-------------------------------------------------------------
void tdnSoundBuffer::Play(bool loop, DWORD cursor)
{
	lpBuf->Stop();
	lpBuf->SetCurrentPosition(cursor);
	//	���[�v�Đ�
	if (loop) lpBuf->Play(0, 0, DSBPLAY_LOOPING);
	//	�m�[�}���Đ�
	else	   lpBuf->Play(0, 0, 0);

	//	lpBuf->SetFrequency(8000);
	PlayCursor = 0;
	loop_flag = loop;
}

//-------------------------------------------------------------
//	��~	
//-------------------------------------------------------------
void tdnSoundBuffer::Stop()
{
	lpBuf->Stop();
}

void tdnSoundBuffer::Pause()
{
	lpBuf->GetCurrentPosition(&PlayCursor, nullptr);
	lpBuf->Stop();
}
void tdnSoundBuffer::PauseOff()
{
	lpBuf->SetCurrentPosition(PlayCursor);
	//	���[�v�Đ�
	if (loop_flag) lpBuf->Play(0, 0, DSBPLAY_LOOPING);
	//	�m�[�}���Đ�
	else	   lpBuf->Play(0, 0, 0);

	PlayCursor = 0;
}

//-------------------------------------------------------------
//	�{�����[���ύX
//-------------------------------------------------------------
void tdnSoundBuffer::SetVolume(int volume)
{
	lpBuf->SetVolume(volume);
}

//-------------------------------------------------------------
//	�{�����[���Q�b�^�[
//-------------------------------------------------------------
int	tdnSoundBuffer::GetVolume()
{
	LONG ret;
	lpBuf->GetVolume(&ret);
	return ret;
}

//-------------------------------------------------------------
//	�X�e���I(���E��)�֌W
//-------------------------------------------------------------
void tdnSoundBuffer::SetPan(int pan)
{
	lpBuf->SetPan(pan);
}

int tdnSoundBuffer::GetPan()
{
	LONG ret;
	lpBuf->GetPan(&ret);
	return ret;
}


//-------------------------------------------------------------
//	���g���֌W(�Đ����x�E�s�b�`)
//-------------------------------------------------------------
void tdnSoundBuffer::SetFrequency(int pitch)
{
	lpBuf->SetFrequency(pitch);
}

int tdnSoundBuffer::GetFrequency()
{
	DWORD ret;
	lpBuf->GetFrequency(&ret);
	return ret;
}

//-------------------------------------------------------------
//	�Đ����x(��̎�����Ă邾��)
//-------------------------------------------------------------
void tdnSoundBuffer::SetSpeed(float speed)
{
	DWORD frequency = (DWORD)(format.nSamplesPerSec*speed);
	lpBuf->SetFrequency(frequency);
}

float tdnSoundBuffer::GetSpeed()
{
	DWORD work;
	lpBuf->GetFrequency(&work);

	return ((float)work / format.nSamplesPerSec);
}


//-------------------------------------------------------------
//	�Đ��`�F�b�N	
//-------------------------------------------------------------
bool tdnSoundBuffer::isPlay()
{
	DWORD	dwAns;
	lpBuf->GetStatus(&dwAns);
	return ((dwAns&DSBSTATUS_PLAYING) != 0) ? true : false;
}


//-------------------------------------------------------------
//	�Đ��ʒu�֌W
//-------------------------------------------------------------
DWORD tdnSoundBuffer::GetPlayCursor()
{
	/* �Đ��ʒu(�f�[�^�ʓI��) */
	DWORD ret;
	lpBuf->GetCurrentPosition(&ret, nullptr);

	return ret;
}

void tdnSoundBuffer::SetPlayCursor(DWORD new_position)
{
	lpBuf->SetCurrentPosition(new_position);
}

DWORD tdnSoundBuffer::GetPlayFrame()
{
	/* �Đ��ʒu(�t���[�����Z) */
	return (GetPlayCursor() / (format.nAvgBytesPerSec / 60));
}

int tdnSoundBuffer::GetPlaySecond()
{
	/* �Đ��ʒu(�b���Z) */
	return (GetPlayCursor() / format.nAvgBytesPerSec);
}

DWORD tdnSoundBuffer::GetPlayMSecond()
{
	/* �Đ��ʒu(�~���b���Z) */
	return (int)(GetPlayCursor() / (format.nAvgBytesPerSec * .001f));
}

void tdnSoundBuffer::SetPlaySecond(int sec)
{
	DWORD set = sec * format.nAvgBytesPerSec;
	lpBuf->SetCurrentPosition(set);
}

DWORD tdnSoundBuffer::GetSize()
{
	return BufferSize;
}

int tdnSoundBuffer::GetLengthSecond()
{
	return (BufferSize / format.nAvgBytesPerSec);
}

//-------------------------------------------------------------
//	3D�T�E���h�֌W
//-------------------------------------------------------------
void tdnSoundBuffer::SetDist(float max_dist, float min_dist)
{
	lpBuf3D->SetMaxDistance(max_dist, DS3D_DEFERRED), lpBuf3D->SetMinDistance(min_dist, DS3D_DEFERRED);
}
void tdnSoundBuffer::SetPos(const Vector3 &pos)
{
	lpBuf3D->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED);
}
void tdnSoundBuffer::SetFront(const Vector3 &front)
{
	lpBuf3D->SetConeOrientation(front.x, front.y, front.z, DS3D_DEFERRED);
}
void tdnSoundBuffer::SetRange(int degreeIn)
{
	lpBuf3D->SetConeAngles(degreeIn, (DS3D_MAXCONEANGLE - degreeIn), DS3D_DEFERRED);
}
void tdnSoundBuffer::SetOutRange_volume(int out_vol)
{
	lpBuf3D->SetConeOutsideVolume(out_vol, DS3D_DEFERRED);
}
void tdnSoundBuffer::SetMove(const Vector3 &move)
{
	lpBuf3D->SetVelocity(move.x, move.y, move.z, DS3D_DEFERRED);
}
void tdnSoundBuffer::SetAll3D(float max_dist, float min_dist, const Vector3 &pos, const Vector3 &front, int degreeIn, int out_vol, const Vector3 &move)
{
	DS3DBUFFER set{};
	set.dwSize = sizeof(DS3DBUFFER);

	/*
	DS3DMODE_HEADRELATIVE
	�T�E���h �p�����[�^(�ʒu�E���x�E����) �́A���X�i�[�̃p�����[�^�ɑ΂��đ��ΓI�Ȃ��̂ł���B
	���̃��[�h�ł́A���X�i�[�̃p�����[�^���ω�����ƃT�E���h�̐�΃p�����[�^�͎����I�ɍX�V�����̂ŁA
	���΃p�����[�^�͈��ɕۂ����B

	DS3DMODE_NORMAL 
	�W���̏����B����̓f�t�H���g�̃��[�h�ł���B
	*/
	set.dwMode = DS3DMODE_HEADRELATIVE;

	set.flMaxDistance = max_dist;
	set.flMinDistance = min_dist;
	set.vPosition = D3DXVECTOR3(pos.x, pos.y, pos.z);
	set.vConeOrientation = D3DXVECTOR3(front.x, front.y, front.z);
	set.dwInsideConeAngle = degreeIn;
	set.dwOutsideConeAngle = (DS3D_MAXCONEANGLE - degreeIn);
	set.lConeOutsideVolume = out_vol;
	set.vVelocity = D3DXVECTOR3(move.x, move.y, move.z);

	lpBuf3D->SetAllParameters(&set, DS3D_DEFERRED);
}

//-------------------------------------------------------------
//	�G�t�F�N�g
//-------------------------------------------------------------
void tdnSoundBuffer::SetFX(DXA_FX flag)
{
	bool isPlay(this->isPlay());
	// ���t���~���A�G�t�F�N�g��S�폜����(���t���͐ݒ�s�炵��)
	if (isPlay)this->Pause();
	lpBuf->SetFX(0, nullptr, nullptr);
	if (flag == DXA_FX::OFF)
	{
		if (isPlay)this->PauseOff();
		return;
	}

	// �G�t�F�N�g�\���̐ݒ�
	DSEFFECTDESC ed{};
	ed.dwSize = sizeof(DSEFFECTDESC);

	switch (flag)
	{
	case DXA_FX::CHORUS:ed.guidDSFXClass = GUID_DSFX_STANDARD_CHORUS;
		break;
	case DXA_FX::COMPRESSOR:ed.guidDSFXClass = GUID_DSFX_STANDARD_COMPRESSOR;
		break;
	case DXA_FX::DISTORTION:ed.guidDSFXClass = GUID_DSFX_STANDARD_DISTORTION;
		break;
	case DXA_FX::ECHO:ed.guidDSFXClass = GUID_DSFX_STANDARD_ECHO;
		break;
	case DXA_FX::FLANGER:ed.guidDSFXClass = GUID_DSFX_STANDARD_FLANGER;
		break;
	case DXA_FX::GARGLE:ed.guidDSFXClass = GUID_DSFX_STANDARD_GARGLE;
		break;
	case DXA_FX::ENVREVERB:ed.guidDSFXClass = GUID_DSFX_STANDARD_I3DL2REVERB;
		break;
	case DXA_FX::PARAMEQ:ed.guidDSFXClass = GUID_DSFX_STANDARD_PARAMEQ;
		break;
	case DXA_FX::WAVESREVERB:ed.guidDSFXClass = GUID_DSFX_WAVES_REVERB;
		break;
	}

	// DirectSound�ɓn��
	result_sound = lpBuf->SetFX(1, &ed, nullptr);

	MyAssert(result_sound == S_OK || format.wBitsPerSample == 16 || flag != DXA_FX::WAVESREVERB, "�~���[�W�b�N���o�[�u�G�t�F�N�g�̐ݒ��16bit�̃I�[�f�B�I�t�H�[�}�b�g�݂̂ł�");

	// �Đ�
	if (isPlay)this->PauseOff();
}

//**************************************************************************************************************
//
//		�X�g���[���T�E���h
//
//**************************************************************************************************************

static int	NumStream = 0;
static bool ForceEnd = false;

//**************************************************************************************************************
//		�Ǘ��X���b�h
//**************************************************************************************************************
DWORD WINAPI ThreadIIDX(LPDWORD lpdwParam)
{
	DWORD	param;
	tdnStreamSound*	lpStream;

	lpStream = (tdnStreamSound*)(lpdwParam);
	for (;;){
		if (lpStream == nullptr) break;
		param = WaitForMultipleObjects(3, lpStream->hEvent, FALSE, 100);

		if (ForceEnd == true) param = -1;

		switch (param){
		case WAIT_OBJECT_0:		if (lpStream->type == TYPE_WAV) lpStream->SetBlockWAV(1);
			if (lpStream->type == TYPE_OGG) lpStream->SetBlockOGG(1);
			break;
		case WAIT_OBJECT_0 + 1:	if (lpStream->type == TYPE_WAV) lpStream->SetBlockWAV(0);
			if (lpStream->type == TYPE_OGG) lpStream->SetBlockOGG(0);
			break;

		case WAIT_TIMEOUT:		switch (lpStream->GetMode()){
		case STR_NORMAL:		break;
		case STR_FADEIN:
		{
			float rate((lpStream->GetVolume() + lpStream->GetParam()) / 255.0f);
			lpStream->SetVolume(rate);
			if (lpStream->GetVolume() >= 255) lpStream->SetMode(STR_NORMAL, 0);
		}
			break;
		case STR_FADEOUT:
		{
			float rate((lpStream->GetVolume() - lpStream->GetParam()) / 255.0f);
			lpStream->SetVolume(rate);
			if (lpStream->GetVolume() <= 0){
				lpStream->Stop();
				delete lpStream;
				ExitThread(TRUE);
				return 0;
			}
		}
			break;
		}
								break;

		default:
			lpStream->Stop();
			delete lpStream;
			ExitThread(TRUE);
			return 0;

		}
	}
	return 0;
}

//**************************************************************************************************************
//
//**************************************************************************************************************
tdnStreamSound::tdnStreamSound(LPDIRECTSOUND lpDS, LPSTR filename, BYTE mode, int param)
{
	NumStream++;

	LPSTR	work;
	BOOL	bInit;
	//	�X�g���[���t�@�C�����J��
	work = &filename[lstrlen(filename) - 4];
	if (lstrcmpi(work, ".wav") == 0) bInit = SetWAV(lpDS, filename);
	if (lstrcmpi(work, ".ogg") == 0) bInit = SetOGG(lpDS, filename);

	lpStream->SetCurrentPosition(0);
	if (mode != STR_FADEIN) SetVolume(1);
	else SetVolume(0);
	/*	�Ǘ��X���b�h�̍쐬	*/
	hStrThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadIIDX, this, 0, &dwThreadId);
	if (hStrThread == nullptr) return;
	/*	�Đ��J�n	*/
	lpStream->Play(0, 0, DSBPLAY_LOOPING);

	this->mode = mode;
	this->param = param;
}



tdnStreamSound::~tdnStreamSound()
{
	if (lpStream != nullptr){
		if (type == TYPE_OGG) ov_clear(&vf);
		else if (hStrFile != nullptr) fclose(hStrFile);

		if (lpStrNotify != nullptr) lpStrNotify->Release();
		lpStrNotify = nullptr;
		/*	�o�b�t�@�J��	*/
		if (lpStream != nullptr) lpStream->Release();
		lpStream = nullptr;
	}

	NumStream--;
}

//**************************************************************************************************************
//
//**************************************************************************************************************
//-------------------------------------------------------------
//	�u���b�N�쐬
//-------------------------------------------------------------
BOOL	tdnStreamSound::OGGRead(LPBYTE dst, unsigned long size)
{
	DWORD	remain = size;
	char*	dstPtr = (char*)dst;
	while (remain > 0){
		long actualRead = ov_read(&vf, dstPtr, remain, 0, 2, 1, nullptr);
		//�I�[�`�F�b�N
		if (actualRead <= 0){
			if (ov_pcm_seek(&vf, 0)) return FALSE;
		}
		remain -= actualRead;
		dstPtr += actualRead;
	}
	return TRUE;
}

BOOL	tdnStreamSound::SetBlockOGG(int block)
{
	LPBYTE	blk1, blk2;
	DWORD	bs1, bs2;

	DWORD	CurPos;

	CurPos = StrSize - StrPos;
	//	�o�b�t�@���b�N
	lpStream->Lock((rate * 4 * STRSECOND)*block, rate * 4 * STRSECOND, (LPVOID*)&blk1, &bs1, (LPVOID*)&blk2, &bs2, 0);
	//	�u���b�N�P�ւ̃f�[�^�Z�b�g
	OGGRead(blk1, bs1);
	StrPos += bs1;
	//	�u���b�N�Q�ւ̃f�[�^�Z�b�g
	if (blk2){
		OGGRead(blk2, bs2);
		StrPos += bs2;
	}

	lpStream->Unlock(blk1, bs1, blk2, bs2);
	return TRUE;
}


BOOL	tdnStreamSound::SetBlockWAV(int block)
{
	LPBYTE	blk1, blk2;
	DWORD	bs1, bs2, work;

	DWORD	CurPos;

	CurPos = StrSize - StrPos;
	lpStream->Lock((rate * 4 * STRSECOND)*block, rate * 4 * STRSECOND, (LPVOID*)&blk1, &bs1, (LPVOID*)&blk2, &bs2, 0);

	work = fread(blk1, bs1, 1, hStrFile);
	StrPos += work;
	if (work < bs1){
		/*	�����߂�	*/
		fseek(hStrFile, LoopPtr, SEEK_SET);
		work = fread(blk1 + work, bs1 - work, 1, hStrFile);
		StrPos = work;
	}

	if (blk2){
		work = fread(blk2, bs2, 1, hStrFile);
		StrPos += work;
		if (work < bs2){
			/*	�����߂�	*/
			fseek(hStrFile, LoopPtr, SEEK_SET);
			work = fread(blk2 + work, bs2 - work, 1, hStrFile);
			StrPos = work;
		}
	}

	lpStream->Unlock(blk1, bs1, blk2, bs2);
	return TRUE;
}


//**************************************************************************************************************
//	�f�[�^�Ǘ�
//**************************************************************************************************************
void tdnStreamSound::Initialize(LPDIRECTSOUND lpDS, int rate)
{
	/*	�������`�F�b�N	*/
	if (lpDS == nullptr) return;
	/*	�v�`�u�d�t�H�[�}�b�g������	*/
	WAVEFORMATEX wfx{};
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;			/*	stereo	*/
	wfx.nSamplesPerSec = rate;
	wfx.wBitsPerSample = 16;			/*	16bit	*/
	wfx.nBlockAlign = 4;
	wfx.nAvgBytesPerSec = rate * 4;
	/* �񎟃o�b�t�@�̏�����	*/
	DSBUFFERDESC dsbd{};
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = rate * 4 * STRSECOND * 2;
	dsbd.lpwfxFormat = &wfx;
	if (lpDS->CreateSoundBuffer(&dsbd, &lpStream, nullptr) != DS_OK) return;
	lpStream->SetFormat(&wfx);

	if (lpStream->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&lpStrNotify) != DS_OK) return;
	/*	�ʒu�C�x���g�쐬	*/
	hEvent[0] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	hEvent[1] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	hEvent[2] = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	pn[0].dwOffset = 0;
	pn[0].hEventNotify = hEvent[0];
	pn[1].dwOffset = rate * 4 * STRSECOND;
	pn[1].hEventNotify = hEvent[1];

	pn[2].dwOffset = DSBPN_OFFSETSTOP;
	pn[2].hEventNotify = hEvent[2];

	if (lpStrNotify->SetNotificationPositions(3, pn) != DS_OK) return;

	this->rate = rate;
	return;
}


BOOL	tdnStreamSound::SetWAV(LPDIRECTSOUND lpDS, char* filename)
{
	HMMIO			hMMIO = nullptr;		/*	�t�@�C���n���h��	*/
	MMCKINFO		ckinfo, ckparent;	/*	RIFF�`�����N���	*/
	LRESULT			ptr;

	/* �I�[�v��	*/
	hMMIO = mmioOpen(filename, nullptr, MMIO_ALLOCBUF | MMIO_READ);
	mmioDescend(hMMIO, &ckparent, nullptr, 0);
	/*	���������`�����N�ɐN��		*/
	ckinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMMIO, &ckinfo, &ckparent, MMIO_FINDCHUNK);
	//	���݈ʒu�擾
	ptr = mmioSeek(hMMIO, 0, SEEK_CUR);
	/*	�t�@�C���A�N�Z�X�I��	*/
	mmioClose(hMMIO, 0);
	if (ptr == -1) return FALSE;

	/*	�t�@�C���I�[�v��	*/
	hStrFile = fopen(filename, "rb");
	if (hStrFile == nullptr) return FALSE;
	//	�X�g���[�����ݒ�
	StrPos = 0;
	LoopPtr = ptr;

	fseek(hStrFile, 0L, SEEK_END);
	StrSize = ftell(hStrFile) - LoopPtr;

	StrSize = GetFileSize(hStrFile, nullptr) - LoopPtr;
	//	�t�@�C���V�[�N
	fseek(hStrFile, LoopPtr, SEEK_SET);

	type = TYPE_WAV;
	Initialize(lpDS, 22050);

	/*	�Đ�����	*/
	SetBlockWAV(0);

	return TRUE;
}

//
//	OGG�p�X�g���[��������
//

BOOL	tdnStreamSound::SetOGG(LPDIRECTSOUND lpDS, char* filename)
{
	//	�t�@�C���I�[�v�� 
	hStrFile = fopen(filename, "rb");
	if (hStrFile == nullptr) return FALSE;

	//Ogg���J��
	ov_open(hStrFile, &vf, nullptr, 0);

	//�V�[�N�\?
	if (!ov_seekable(&vf)){
		ov_clear(&vf);
		return FALSE;
	}

	//���ݒ�
	const vorbis_info *info = ov_info(&vf, -1);
	Initialize(lpDS, info->rate);

	type = TYPE_OGG;
	StrPos = 0;
	StrSize = (DWORD)ov_pcm_total(&vf, -1);

	/*	�Đ�����	*/
	SetBlockOGG(0);

	return TRUE;
}


//**************************************************************************************************************
//	�Đ��Ǘ�
//**************************************************************************************************************

void tdnStreamSound::Stop()
{
	if (lpStream == nullptr) return;
	if (hStrFile == nullptr) return;

	lpStream->Stop();
}

void tdnStreamSound::SetVolume(float volume)
{
	int		vol;
	if (lpStream == nullptr) return;
	/*	���ʃZ�b�g	*/
	volume = Math::Clamp(volume, 0, 1);
	vol = (int)(255 * volume);
	this->volume = vol;
	vol -= 255;
	vol = (vol*vol * 100 / (255 * 255));
	lpStream->SetVolume(-vol*vol);
}

void tdnStreamSound::SetMode(BYTE mode, int param)
{
	this->mode = mode;
	this->param = param;
}




//**************************************************************************************************************
//
//		�T�E���h�}�l�[�W�����
//
//**************************************************************************************************************

//=============================================================================================
//		��	��	��
tdnSoundBase::tdnSoundBase()
{
	hWndWAV = tdnSystem::GetWindow();
	CoInitialize(nullptr);
	//	DirectSound�̏�����
	if (DirectSoundCreate8(nullptr, &lpDS, nullptr) != DS_OK){
		lpDS = nullptr;
		return;
	}

	lpDS->SetCooperativeLevel(hWndWAV, DSSCL_PRIORITY);

	lpPrimary = nullptr;
	DSBUFFERDESC dsbd{};
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	lpDS->CreateSoundBuffer(&dsbd, &lpPrimary, nullptr);

	// 3D���X�i�[�쐬
	lp3DListener = nullptr;
	result_sound = lpPrimary->QueryInterface(IID_IDirectSound3DListener, (LPVOID *)&lp3DListener);

	if (result_sound == E_INVALIDARG)	// �v���V�[�W���̌Ăяo���A�܂��͈������s���炵��
	{
		assert(0);
	}

	this->SetListenerAll(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(0, 0, 0));
	this->UpdateListener();

	WAVEFORMATEX wfx{};
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = 22050;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	lpPrimary->SetFormat(&wfx);

}
//
//=============================================================================================


//=============================================================================================
//		��		��
tdnSoundBase::~tdnSoundBase()
{
	//	�o�b�t�@�̉��
	//for (int i = 0; i < WavNum; i++) for (UINT j = 0; j < buffer[i].size(); j++)
	//{
	//	SAFE_DELETE(buffer[i][j]);
	//}

	//	Direct Sound���
	if (lpPrimary != nullptr) lpPrimary->Release();
	if (lpDS != nullptr) lpDS->Release();

	lpDS = nullptr;
	lpPrimary = nullptr;
}
//
//=============================================================================================

//**************************************************************************************************************
//
//		�T�E���h�}�l�[�W��(SE�p)
//
//**************************************************************************************************************

//=============================================================================================
//		��	��	��
tdnSoundSE::tdnSoundSE():m_fBaseVolume(1)
{
	for (int i = 0; i < WavNum; i++)data[i].clear();
}
//
//=============================================================================================


//=============================================================================================
//		��		��
tdnSoundSE::~tdnSoundSE()
{
	// �o�b�t�@���
	for (int i = 0; i < WavNum; i++)
	{
		for (UINT j = 0; j < data[i].size(); j++)
		{
			delete data[i][j]->buffer;
			delete data[i][j];
		}
	}
}
//
//=============================================================================================

//=============================================================================================
//		wave�t�@�C���Z�b�g
void tdnSoundSE::Set(int ID, int num_of_play_simultaneously, LPCSTR filename, bool b3D)
{
	//	�������`�F�b�N
	assert(lpDS);
	//	�����̃o�b�t�@�̉��
	if (ID < (int)data[ID].size())
	{
		for (UINT i = 0; i < data[i].size(); i++) SAFE_DELETE(data[ID][i]);
		data[ID].clear();
	}
	assert(filename);

	if (num_of_play_simultaneously == 1)
	{	// 1�����Ȃ��Ȃ�R�s�[����Ӗ��Ȃ�����Ȃ��I
		SEData *set = new SEData;
		set->buffer = new tdnSoundBuffer(lpDS, filename, b3D);
		set->b3D = b3D;
		data[ID].push_back(set);
	}
	else
	{
		tdnSoundBuffer **work = new tdnSoundBuffer*[num_of_play_simultaneously];
		// �����Đ��\����WAV�t�@�C�����[�h
		for (int i = 0; i < num_of_play_simultaneously; i++)
		{
			SEData *set = new SEData;
			set->buffer = new tdnSoundBuffer();
			set->b3D = b3D;
			data[ID].push_back(set);
			work[i] = data[ID][i]->buffer;	// �A�h���X��n��
		}
		tdnSoundBuffer::Create_and_copy(lpDS, filename, b3D, work, 0, num_of_play_simultaneously);	// ���work�ɃA�h���X��n�����̂ł����ł���buffer[ID]�ɂ͍���Ă���
		delete[] work;
		assert(data[ID][0]->buffer->GetBuf());
	}
}
//
//=============================================================================================

//=============================================================================================
//		��		��
int tdnSoundSE::Play(int ID, bool loop)
{
	//	�������`�F�b�N
	assert(lpDS);
	//	�f�[�^�������I�I
	assert(data[ID].size() != 0);

	for (UINT play_no = 0; play_no < data[ID].size(); play_no++)
	{
		// �Đ����ĂȂ����炢�ł�514��Ԃ̐l������
		if (!data[ID][play_no]->buffer->isPlay())
		{	// ���������I
			data[ID][play_no]->buffer->SetVolume((int)(m_fBaseVolume * 10000) - 10000);// ����{�{�����[����ݒ�
			data[ID][play_no]->buffer->Play(loop);
			return play_no;
		}
	}
	
	// �S���Đ���Ԃ������̂ŁA�Đ����s
	return TDNSOUND_PLAY_NONE;
}

int tdnSoundSE::Play(int ID, const Vector3 &pos, const Vector3 &move, bool loop)
{
	MyAssert(data[ID][0]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");

	//	�������`�F�b�N
	assert(lpDS);
	//	�f�[�^�������I�I
	assert(data[ID].size() != 0);

	for (UINT play_no = 0; play_no < data[ID].size(); play_no++)
	{
		// �Đ����ĂȂ����炢�ł�514��Ԃ̐l������
		if (!data[ID][play_no]->buffer->isPlay())
		{	// ���������I
			data[ID][play_no]->buffer->SetPos(pos);
			data[ID][play_no]->buffer->SetMove(move);
			data[ID][play_no]->buffer->Play(loop);
			return play_no;
		}
	}

	// �S���Đ���Ԃ������̂ŁA�Đ����s
	return TDNSOUND_PLAY_NONE;
}

int tdnSoundSE::Play(int ID, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	MyAssert(data[ID][0]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");

	//	�������`�F�b�N
	assert(lpDS);
	//	�f�[�^�������I�I
	assert(data[ID].size() != 0);

	for (UINT play_no = 0; play_no < data[ID].size(); play_no++)
	{
		// �Đ����ĂȂ����炢�ł�514��Ԃ̐l������
		if (!data[ID][play_no]->buffer->isPlay())
		{	// ���������I
			data[ID][play_no]->buffer->SetAll3D(DS3D_DEFAULTMAXDISTANCE, DS3D_DEFAULTMINDISTANCE, pos, front, DS3D_DEFAULTCONEANGLE, DS3D_DEFAULTCONEOUTSIDEVOLUME, move);
			data[ID][play_no]->buffer->Play(loop);
			return play_no;
		}
	}

	// �S���Đ���Ԃ������̂ŁA�Đ����s
	return TDNSOUND_PLAY_NONE;
}
//
//=============================================================================================

//=============================================================================================
//		��		�~
void tdnSoundSE::Stop(int ID, int no)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->Stop();
}
void tdnSoundSE::AllStop()
{
	assert(lpDS);
	FOR(WavNum) for (auto it : data[i])if (it->buffer->isPlay())it->buffer->Stop();
}
//
//=============================================================================================

//=============================================================================================
//		�{�����[��(-10000�`0)
void tdnSoundSE::SetVolume(int ID, int volume)
{
	if(!lpDS)return;
	if(data[ID].empty())return;
	if (data[ID][0]->b3D)return;	// 3D�T�E���h�ɏ�����C���Ă���̂ł����瑤�ŉ��͂�����Ȃ�
	for (auto it : data[ID]) it->buffer->SetVolume(volume);	// ID���S���ݒ肵�Ă邪�A�e���ݒ肵�����ꍇ�́A�܂����܂��B
}
void tdnSoundSE::SetVolume(int ID, float volume)
{
	SetVolume(ID, (int)(-7500 * volume));
}
void tdnSoundSE::SetBaseVolume(float fVolume)
{
	FOR(WavNum) SetVolume(i, fVolume);
}
int	tdnSoundSE::GetVolume(int ID)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	return data[ID][0]->buffer->GetVolume();
}
//
//=============================================================================================

//=============================================================================================
//		�X�e���I(-10000�`0�`10000)
void	tdnSoundSE::SetPan(int ID, int pan)
{
	if (data[ID][0]->b3D)return;	// 3D�T�E���h�ɏ�����C���Ă���̂ł����瑤�ŉ��͂�����Ȃ�
	assert(lpDS);
	assert(data[ID].size() != 0);
	for (UINT i = 0; i < data[ID].size(); i++) data[ID][i]->buffer->SetPan(pan);
}
int		tdnSoundSE::GetPan(int ID)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	return data[ID][0]->buffer->GetPan();
}
//
//=============================================================================================

//=============================================================================================
//		���g��
void tdnSoundSE::SetFrequency(int ID, int frequency)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	for (UINT i = 0; i < data[ID].size(); i++) data[ID][i]->buffer->SetFrequency(frequency);
}
void tdnSoundSE::SetFrequency(int ID, int no, int frequency)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetFrequency(frequency);
}
int	tdnSoundSE::GetFrequency(int ID, int no)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	return data[ID][no]->buffer->GetFrequency();
}
//
//=============================================================================================

//=============================================================================================
//		�Đ����x(���g���������Ă邾��)
void tdnSoundSE::SetSpeed(int ID, float speed)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	for (UINT i = 0; i < data[ID].size(); i++) data[ID][i]->buffer->SetSpeed(speed);
}

void tdnSoundSE::SetSpeed(int ID, int no, float speed)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetSpeed(speed);
}
//
//=============================================================================================

//=============================================================================================
//		�Đ����(�Đ���or��~��)
bool tdnSoundSE::isPlay(int ID, int no)
{
	assert(lpDS);
	assert(data[ID].size() != 0);
	return data[ID][no]->buffer->isPlay();
}
//
//=============================================================================================

//=============================================================================================
//		3D�T�E���h�ł̕�������l���ݒ�
/*
DS3D_DEFERRED
�A�v���P�[�V������ IDirectSound3DListener8::CommitDeferredSettings ���\�b�h���Ăяo���܂ŁA
�ݒ�͓K�p����Ȃ��B�����̐ݒ��ύX������A1 ��̍Čv�Z�ł�����K�p�ł���B

DS3D_IMMEDIATE
�ݒ�𒼂��ɓK�p���A�V�X�e���͂��ׂĂ� 3D �T�E���h �o�b�t�@�� 3D ���W���Čv�Z����B
*/
void tdnSoundBase::SetListenerPos(const Vector3 &pos)
{
	lp3DListener->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED);
}
void tdnSoundBase::SetListenerOrientation(const Vector3 &front, const Vector3 &up)
{
	lp3DListener->SetOrientation(front.x, front.y, front.z, up.x, up.y, up.z, DS3D_DEFERRED);
}
void tdnSoundBase::SetListenerMove(const Vector3 &move)
{
	lp3DListener->SetVelocity(move.x, move.y, move.z, DS3D_DEFERRED);
}
void tdnSoundBase::SetListenerAll(const Vector3 &pos, const Vector3 &front, const Vector3 &up, const Vector3 &velocity)
{
	DS3DLISTENER set{};
	set.dwSize = sizeof(DS3DLISTENER);

	// ���X�i�[���ݒ�
	set.vPosition = D3DXVECTOR3(pos.x, pos.y, pos.z);
	set.vOrientFront = D3DXVECTOR3(front.x, front.y, front.z);
	set.vOrientTop = D3DXVECTOR3(up.x, up.y, up.z);
	set.vVelocity = D3DXVECTOR3(velocity.x, velocity.y, velocity.z);

	// �T�E���h�v�Z���ݒ�(�f�t�H���g�l)
	set.flDistanceFactor = DS3D_DEFAULTDISTANCEFACTOR;	// �x�N�g���P�ʂɂ����郁�[�g����
	set.flDopplerFactor = DS3D_DEFAULTDOPPLERFACTOR;	// �h�b�v���[���ʂɂ��Ă̒l
	set.flRolloffFactor = 0.01f;	// �����ɂ�錸���ɂ��Ă̒l

	lp3DListener->SetAllParameters(&set, DS3D_DEFERRED);
}
void tdnSoundBase::UpdateListener()
{
	result_sound = lp3DListener->CommitDeferredSettings();
	assert(result_sound == DS_OK);
}
//
//=============================================================================================

//=============================================================================================
//		3D�T�E���h�ł̉������ݒ�
void tdnSoundSE::SetDist(int ID, int no, float max_dist, float min_dist)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetDist(max_dist, min_dist);
}
void tdnSoundSE::SetPos(int ID, int no, const Vector3 &pos)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetPos(pos);
}
void tdnSoundSE::SetFront(int ID, int no, const Vector3 &front)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetFront(front);
}
void tdnSoundSE::SetRange(int ID, int no, int degreeIn)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetRange(degreeIn);
}
void tdnSoundSE::SetOutRange_volume(int ID, int no, int out_vol)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetOutRange_volume(out_vol);
}
void tdnSoundSE::SetMove(int ID, int no, const Vector3 &move)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetMove(move);
}
void tdnSoundSE::SetAll3D(int ID, int no, float max_dist, float min_dist, const Vector3 &pos, const Vector3 &front, int degreeIn, int out_vol, const Vector3 &move)
{
	MyAssert(data[ID][no]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data[ID].size() != 0);
	data[ID][no]->buffer->SetAll3D(max_dist, min_dist, pos, front, degreeIn, out_vol, move);
}
//
//=============================================================================================

//=============================================================================================
//		�G�t�F�N�g�Z�b�g
void tdnSoundSE::SetFX(DXA_FX flag)
{
	for (int i = 0; i < WavNum; i++)for (UINT j = 0; j < data[i].size(); j++)data[i][j]->buffer->SetFX(flag);
}
void tdnSoundSE::SetFX(int ID, DXA_FX flag)
{
	for (UINT i = 0; i < data[ID].size(); i++)data[ID][i]->buffer->SetFX(flag);
}
//
//=============================================================================================


//**************************************************************************************************************
//
//		�T�E���h�}�l�[�W��(BGM�p)
//
//**************************************************************************************************************

//=============================================================================================
//		��	��	��
tdnSoundBGM::tdnSoundBGM() :m_fBaseVolume(1)
{
	data.clear();
	Fade_funk[(int)MODE::NONE] = &tdnSoundBGM::None;
	Fade_funk[(int)MODE::FADE_IN] = &tdnSoundBGM::In;
	Fade_funk[(int)MODE::FADE_OUT] = &tdnSoundBGM::Out;
}
//
//=============================================================================================


//=============================================================================================
//		��		��
tdnSoundBGM::~tdnSoundBGM()
{
	// �o�b�t�@���
	for (UINT i = 0; i < data.size(); i++)
	{
		delete data[i]->buffer;
		delete data[i];
	}
	data.clear();
}
//
//=============================================================================================

//=============================================================================================
//		�X		�V
void tdnSoundBGM::Update()
{
	for (UINT i = 0; i < data.size(); i++)
	{
		(this->*Fade_funk[(int)data[i]->fade_mode])(i);	// ���[�h����
	}
}

void tdnSoundBGM::None(int no){}
void tdnSoundBGM::In(int no)
{
	// �{�����[���グ�Ă���
	if ((data[no]->volume += data[no]->fade_speed) >= 1.0f)
	{
		// �t�F�[�h���������I
		data[no]->volume = 1.0f;
		data[no]->fade_mode = MODE::NONE;
	}
	const int vol = (int)(MinVolume * (1.0f - data[no]->volume));
	data[no]->buffer->SetVolume(vol);
}
void tdnSoundBGM::Out(int no)
{
	// �{�����[�������Ă���
	if ((data[no]->volume -= data[no]->fade_speed) <= 0)
	{
		// �t�F�[�h���������I
		data[no]->volume = 0;
		data[no]->fade_mode = MODE::NONE;
		data[no]->buffer->Stop();
	}
	const int vol = (int)(MinVolume * (1.0f - data[no]->volume));
	data[no]->buffer->SetVolume(vol);
}
//
//=============================================================================================

//=============================================================================================
//		wave�t�@�C���Z�b�g
void tdnSoundBGM::Set(int ID, char* filename, bool b3D)
{
	//	�������`�F�b�N
	assert(lpDS);
	//	�����̃o�b�t�@�̉��
	if (ID < (int)data.size())SAFE_DELETE(data[ID]->buffer);

	assert(filename);

	// ���ݒ�
	tagBGMData *set = new tagBGMData;
	set->b3D = b3D;
	set->buffer = new tdnSoundBuffer(lpDS, filename, b3D);
	set->fade_mode = MODE::NONE;
	set->fade_speed = 0;
	set->volume = 1;
	data.push_back(set);
	assert(data[ID]->buffer->GetBuf());
}
void tdnSoundBGM::Set(int ID, BYTE *wav_data, DWORD size, LPWAVEFORMATEX wfx, bool b3D)
{
	//	�������`�F�b�N
	assert(lpDS);
	//	�����̃o�b�t�@�̉��
	if (ID < (int)data.size())SAFE_DELETE(data[ID]->buffer);

	// ���ݒ�
	tagBGMData *set = new tagBGMData;
	set->b3D = b3D;
	set->buffer = new tdnSoundBuffer();
	set->buffer->Initialize(lpDS, wav_data, size, wfx, b3D);
	set->fade_mode = MODE::NONE;
	set->fade_speed = 0;
	set->volume = 1;
	data.push_back(set);
	assert(data[ID]->buffer->GetBuf());
}
//
//=============================================================================================

//=============================================================================================
//		��		��
void tdnSoundBGM::Play(int ID, bool loop, DWORD cursor)
{
	//	�������`�F�b�N
	assert(lpDS);
	//	�f�[�^�������I�I
	assert(data[ID]->buffer);
	data[ID]->buffer->Play(loop, cursor);
}
void tdnSoundBGM::Play(int ID, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	data[ID]->buffer->SetAll3D(DS3D_DEFAULTMAXDISTANCE, DS3D_DEFAULTMINDISTANCE, pos, front, DS3D_DEFAULTCONEANGLE, DS3D_DEFAULTCONEOUTSIDEVOLUME, move);
	data[ID]->buffer->Play(loop);
}
//
//=============================================================================================


//=============================================================================================
//		��		�~
void tdnSoundBGM::Stop(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	data[ID]->buffer->Stop();
}
void tdnSoundBGM::AllStop()
{
	assert(lpDS);
	for (UINT i = 0; i < data.size(); i++)if (data[i]->buffer->isPlay())data[i]->buffer->Stop();
}
void tdnSoundBGM::Pause(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	data[ID]->buffer->Pause();
}
//
//=============================================================================================

//=============================================================================================
//		�t�F�[�h�֌W
void tdnSoundBGM::FadeIn(int ID, float fade_speed, bool loop, DWORD cursor)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	data[ID]->volume = 0;
	data[ID]->fade_mode = MODE::FADE_IN;
	data[ID]->fade_speed = fade_speed;
	data[ID]->buffer->SetVolume(DSBVOLUME_MIN);
	data[ID]->buffer->Play(loop, cursor);
}
void tdnSoundBGM::FadeOut(int ID, float fade_speed)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	data[ID]->volume = 1.0f;
	data[ID]->fade_mode = MODE::FADE_OUT;
	data[ID]->fade_speed = fade_speed;
	data[ID]->buffer->SetVolume(DSBVOLUME_MAX);
}
void tdnSoundBGM::CrossFade(int inID, int outID, float fade_speed, CROSS_FADE_TYPE type, bool loop)
{
	CrossFade(inID, outID, fade_speed, fade_speed, type, loop);
}
void tdnSoundBGM::CrossFade(int inID, int outID, float in_speed, float out_speed, CROSS_FADE_TYPE type, bool loop)
{
	assert(lpDS);
	assert(data[inID]->buffer && data[outID]->buffer);

	// �t�F�[�h�A�E�g�ݒ�
	this->FadeOut(outID, out_speed);

	// �t�F�[�h�C���ݒ�
	DWORD cursor;
	switch (type)
	{
	case CROSS_FADE_TYPE::NORMAL: cursor = 0;
		break;
	case CROSS_FADE_TYPE::END_OF_ETERNITY: cursor = data[outID]->buffer->GetPlayCursor();
		break;
	}
	this->FadeIn(inID, in_speed, loop, cursor);
}
//
//=============================================================================================

//=============================================================================================
//		�{�����[��(-10000�`0)
void tdnSoundBGM::SetVolume(int ID, int volume)
{
	if (data[ID]->b3D)return;	// 3D�T�E���h�ɏ�����C���Ă���̂ł����瑤�ŉ��͂�����Ȃ�
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetVolume(volume);
}
void tdnSoundBGM::SetVolume(int ID, float volume)
{
	const int vol = (int)(MinVolume * (1.0f - volume));
	SetVolume(ID, vol);
}
int	tdnSoundBGM::GetVolume(int ID)
{
	assert(lpDS);
	assert(data.size() != 0);
	return data[ID]->buffer->GetVolume();
}
//
//=============================================================================================

//=============================================================================================
//		�X�e���I(-10000�`0�`10000)
void tdnSoundBGM::SetPan(int ID, int pan)
{
	if (data[ID]->b3D)return;	// 3D�T�E���h�ɏ�����C���Ă���̂ł����瑤�ŉ��͂�����Ȃ�
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetPan(pan);
}
int	tdnSoundBGM::GetPan(int ID)
{
	assert(lpDS);
	assert(data.size() != 0);
	return data[ID]->buffer->GetPan();
}
//
//=============================================================================================

//=============================================================================================
//		���g��
void tdnSoundBGM::SetFrequency(int ID, int frequency)
{
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetFrequency(frequency);
}
int	tdnSoundBGM::GetFrequency(int ID, int no)
{
	assert(lpDS);
	assert(data.size() != 0);
	return data[ID]->buffer->GetFrequency();
}
//
//=============================================================================================

//=============================================================================================
//		�Đ����x(���g���������Ă邾��)
void tdnSoundBGM::SetSpeed(int ID, float speed)
{
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetSpeed(speed);
}

void tdnSoundBGM::SetSpeed(int ID, int no, float speed)
{
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetSpeed(speed);
}
//
//=============================================================================================

//=============================================================================================
//		�Đ����(�Đ���or��~��)
bool tdnSoundBGM::isPlay(int ID)
{
	assert(lpDS);
	assert(data.size() != 0);
	return data[ID]->buffer->isPlay();
}
//
//=============================================================================================

//=============================================================================================
//		�Đ��J�[�\��
DWORD tdnSoundBGM::GetPlayCursor(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	return data[ID]->buffer->GetPlayCursor();
}
DWORD tdnSoundBGM::GetPlayFrame(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	return data[ID]->buffer->GetPlayFrame();
}
int	tdnSoundBGM::GetPlaySecond(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	return data[ID]->buffer->GetPlaySecond();
}
DWORD tdnSoundBGM::GetPlayMSecond(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	return data[ID]->buffer->GetPlayMSecond();
}

void tdnSoundBGM::SetPlaySecond(int ID, int sec)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	data[ID]->buffer->SetPlaySecond(sec);
}


DWORD tdnSoundBGM::GetSize(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	return data[ID]->buffer->GetSize();
}

int	tdnSoundBGM::GetLengthSecond(int ID)
{
	assert(lpDS);
	assert(data[ID]->buffer);
	return data[ID]->buffer->GetLengthSecond();
}
//
//=============================================================================================

//=============================================================================================
//		3D�T�E���h�ł̉������ݒ�
void tdnSoundBGM::SetDist(int ID, int no, float max_dist, float min_dist)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetDist(max_dist, min_dist);
}
void tdnSoundBGM::SetPos(int ID, int no, const Vector3 &pos)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetPos(pos);
}
void tdnSoundBGM::SetFront(int ID, int no, const Vector3 &front)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetFront(front);
}
void tdnSoundBGM::SetRange(int ID, int no, int degreeIn)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetRange(degreeIn);
}
void tdnSoundBGM::SetOutRange_volume(int ID, int no, int out_vol)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetOutRange_volume(out_vol);
}
void tdnSoundBGM::SetMove(int ID, int no, const Vector3 &move)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetMove(move);
}
void tdnSoundBGM::SetAll3D(int ID, int no, float max_dist, float min_dist, const Vector3 &pos, const Vector3 &front, int degreeIn, int out_vol, const Vector3 &move)
{
	MyAssert(data[ID]->b3D, "ERROR:b3D�t���OOFF�̏�Ԃ�3D�T�E���h���g�p���Ă��܂��BSet��b3D��true�ɂ���Ɖ������܂�");
	assert(lpDS);
	assert(data.size() != 0);
	data[ID]->buffer->SetAll3D(max_dist, min_dist, pos, front, degreeIn, out_vol, move);
}
//
//=============================================================================================

//=============================================================================================
//		�G�t�F�N�g�Z�b�g
void tdnSoundBGM::SetFX(DXA_FX flag)
{
	for (UINT i = 0; i < data.size(); i++)data[i]->buffer->SetFX(flag);
}
void tdnSoundBGM::SetFX(int ID, DXA_FX flag)
{
	data[ID]->buffer->SetFX(flag);
}
//
//=============================================================================================

//**************************************************************************************************************
//	�X�g���[���T�E���h�Ǘ�
//**************************************************************************************************************
tdnStreamSound* tdnSoundBGM::PlayStream(char* filename)
{
	return PlayStream(filename, STR_NORMAL, 0);
}

tdnStreamSound* tdnSoundBGM::PlayStream(char* filename, BYTE mode, int param)
{
	tdnStreamSound*	lpStream;

	//	�������`�F�b�N
	if (lpDS == nullptr) return nullptr;

	lpStream = new tdnStreamSound(lpDS, filename, mode, param);
	if(mode == STR_NORMAL)lpStream->SetVolume(m_fBaseVolume);				// ����{�{�����[���ݒ�
	return lpStream;
}


LPBYTE LoadWavData(LPSTR filename, LPDWORD size, LPWAVEFORMATEX wfx)
{
	HMMIO			hMMIO = nullptr;		/*	�t�@�C���n���h��	*/
	PCMWAVEFORMAT	pwf;				/*	WAV�f�[�^�`��		*/
	MMCKINFO		ckparent, ckinfo;	/*	RIFF�`�����N���	*/
	MMIOINFO		mminfo;				/*	�t�@�C�����		*/
	DWORD			i;
	LPBYTE			buf = nullptr;			/*	�ǂݍ��݃o�b�t�@	*/

	/* �I�[�v��	*/
	MyAssert((hMMIO = mmioOpen(filename, nullptr, MMIO_ALLOCBUF | MMIO_READ)), "�G���[�t�@�C����[%s]\n����:wav�t�@�C���������Ă��Ȃ����Awav�t�@�C�������Ԉ���Ă����", filename);
	if (mmioDescend(hMMIO, &ckparent, nullptr, 0) != 0) goto WAVE_LoadError;
	/*	�v�`�u(RIFF)�t�@�C���`�F�b�N		*/
	if ((ckparent.ckid != FOURCC_RIFF) || (ckparent.fccType != mmioFOURCC('W', 'A', 'V', 'E'))) goto WAVE_LoadError;
	/*	�������`�����N�ɐN��		*/
	ckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hMMIO, &ckinfo, &ckparent, MMIO_FINDCHUNK) != 0) goto WAVE_LoadError;
	if (ckinfo.cksize < sizeof(PCMWAVEFORMAT)) goto WAVE_LoadError;
	/*	�`�����N���烊�[�h	*/
	if (mmioRead(hMMIO, (HPSTR)&pwf, sizeof(pwf)) != sizeof(pwf)) goto WAVE_LoadError;
	if (pwf.wf.wFormatTag != WAVE_FORMAT_PCM) goto WAVE_LoadError;
	/*	WAV�t�H�[�}�b�g�̕ۑ�	*/
	memset(wfx, 0, sizeof(WAVEFORMATEX));
	memcpy(wfx, &pwf, sizeof(pwf));
	/*	�f�[�^�̓ǂݍ���	*/
	if (mmioSeek(hMMIO, ckparent.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1) goto WAVE_LoadError;
	/*	���������`�����N�ɐN��		*/
	ckinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hMMIO, &ckinfo, &ckparent, MMIO_FINDCHUNK) != 0) goto WAVE_LoadError;
	if (mmioGetInfo(hMMIO, &mminfo, 0) != 0) goto WAVE_LoadError;
	/*	�o�b�t�@�T�C�Y�ۑ�	*/
	if (size != nullptr) *size = ckinfo.cksize;
	/*	�v�`�u�p�o�b�t�@�̎擾	*/
	buf = (LPBYTE)GlobalAlloc(LPTR, ckinfo.cksize);
	if (buf == nullptr) goto WAVE_LoadError;
	/*	�f�[�^�̓ǂ݂Ƃ�	*/
	for (i = 0; i < ckinfo.cksize; i++){
		/*	�G���[�`�F�b�N	*/
		if (mminfo.pchNext >= mminfo.pchEndRead){
			if (mmioAdvance(hMMIO, &mminfo, MMIO_READ) != 0) goto WAVE_LoadError;
			if (mminfo.pchNext >= mminfo.pchEndRead) goto WAVE_LoadError;
		}
		*(buf + i) = *((LPBYTE)mminfo.pchNext);
		mminfo.pchNext++;
	}
	/*	�t�@�C���A�N�Z�X�I��	*/
	mmioSetInfo(hMMIO, &mminfo, 0);
	mmioClose(hMMIO, 0);
	return buf;

WAVE_LoadError:	/*	�G���[�I��	*/
	mmioClose(hMMIO, 0);
	if (buf != nullptr) GlobalFree(buf);
	return nullptr;
}