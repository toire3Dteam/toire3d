#include "TDNLIB.h"

// CoInitializeEx�g���p
#include<objbase.h>
#define _WIN32_DCOM

// VMR9�g���p
//#include<Vmr9.h>


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/�g����ł̃���_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
/*

1:
LIBCMTD.LIB ���d��������
�u�����J�v���u�R�}���h���C���v���u�ǉ��̃I�v�V�����v��
/NODEFAULTLIB:LIBCMTD.LIB

2:
�u�����J�v���u�S�ʁv���u�ǉ��̃��C�u�����f�B���N�g���v��
./TDN/DirectShow/

*/

//#pragma comment (lib, "msvcr120.lib")

//****************************************************************************************
//		������
//****************************************************************************************
void tdnMovieManager::Initialize()
{
	// �C���^�[�t�F�[�X������
	// �����COM�R���|�[�l���g��������ł̂���@�ł��āA����Ɏ��s�����COM�������܂���B����ɂ���ē����ŏ������ɕK�v�ȏ��X����������Ă��܂��B
	MyAssert(!FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)), "�G���[: tdnMovie�̃C���^�[�t�F�[�X������");
}

void tdnMovieManager::Release()
{
	// COM�I��
	CoUninitialize();

	//VMR9AllocationInfo *lpAllocInfo;
}


//****************************************************************************************
//		������
//****************************************************************************************
tdnMovie::tdnMovie(char *cFilename, bool bSound, bool bLoop) :m_pGraphBuilder(nullptr), m_pMediaControl(nullptr), m_pMediaEvent(nullptr), m_pMediaPosition(nullptr),
m_state(FILTER_STATE::State_Stopped), m_bLoop(bLoop)
{
	IBaseFilter	*pSourceFilter(nullptr);	// �\�[�X�t�B���^�[
	IPin *pFSrcPinOut(nullptr);				// �\�[�X�t�B���^�[��OUT�s��

	// FilterGraph�C���^�[�t�F�[�X����
	MyAssert(!FAILED(CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGraphBuilder)), "�G���[: tdnMovie��FilterGraph�C���^�[�t�F�[�X����");

	// �e�N�X�`�������_���\������
	HRESULT hr;
	TextureRenderer *pRenderer = new TextureRenderer(nullptr, &hr); // ���[�N
	pRenderer->SetDevice(tdnSystem::GetDevice());
	MyAssert(!FAILED(hr) && pRenderer, "�G���[: �e�N�X�`�������_���\�̏�����");

	// �e�N�X�`�������_���\�������_�t�B���^�[�ɂ�����
	m_pTextureRenderer = pRenderer;
	/* �� */
	MyAssert(!FAILED(m_pGraphBuilder->AddFilter(m_pTextureRenderer, L"TextureRenderer")), "�G���[: �����_�t�B���^�[�ǉ�");

	/*
	// �����_�t�B���^�[�����E�ǉ�
	MyAssert(!FAILED(CoCreateInstance(CLSID_VideoMixingRenderer9, nullptr, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&m_pVMR9)), "�G���[: tdnMovie�̃����_�t�B���^�[����");
	m_pGraphBuilder->AddFilter(m_pVMR9, L"VMR9");

	// �E�C���h�E���X���[�h�ɂ���
	IVMRFilterConfig *pVMRConfig;
	MyAssert(!FAILED(m_pVMR9->QueryInterface(IID_IVMRFilterConfig9, (LPVOID*)&pVMRConfig)),"�G���[: tdnMovie��VMR9�̃C���^�[�t�F�[�X");
	MyAssert(!FAILED(pVMRConfig->SetRenderingMode(VMRMode_Windowless)),"�G���[: tdnMovie�̃E�B���h�E�ݒ�");
	pVMRConfig->Release();// �p�ς�

	// �`��E�B���h�E�̎w��
	IVMRWindowlessControl *pVMRWndControl;
	MyAssert(!FAILED(m_pVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndControl)), "�G���[: tdnMovie��VMR9�C���^�[�t�F�[�X");
	MyAssert(!FAILED(pVMRWndControl->SetVideoClippingWindow(tdnSystem::GetWindow())), "�G���[: tdnMovie�̕`��E�B���h�E�ݒ�");
	pVMRWndControl->Release();// �p�ς�
	*/

	// �\�[�X�t�B���^�[�����E�ǉ�
	// char �� BSTR
	LPSTR lstr = cFilename;
	int blen = (int)MultiByteToWideChar(CP_ACP, 0, lstr, strlen(lstr), nullptr, 0);
	BSTR bFilename;
	bFilename = SysAllocStringLen(nullptr, blen);
	MultiByteToWideChar(CP_ACP, 0, lstr, strlen(lstr), bFilename, blen);
	MyAssert(!FAILED(m_pGraphBuilder->AddSourceFilter(bFilename, L"SourceFilter", &pSourceFilter)), "�G���[: tdnMovie�̃\�[�X�t�B���^�[����");

	/* AVI�Ƃ�MPEG1�Ƃ�VFW�Ƃ� */
	if (SUCCEEDED(pSourceFilter->FindPin(L"Output", &pFSrcPinOut)))
	{
		// �T�E���h���Đ�����ꍇ
		if (bSound)
		{
			// �\�[�X�t�B���^�[�s����`��H�q����̂��H
			MyAssert(!FAILED(m_pGraphBuilder->Render(pFSrcPinOut)), "�G���[: tdnMovie�̃\�[�X�t�B���^�A�E�g�v�b�g");
		}

		// �T�E���h�Đ����Ȃ��ꍇ
		else
		{
			// �e�N�X�`�������_���\�̃C���v�b�g�s��
			IPin *pFTRPinIn;

			// �����_���\�t�B���^�[�̃C���v�b�g����
			/* �� */
			MyAssert(!FAILED(m_pTextureRenderer->FindPin(L"In",&pFTRPinIn)), "�G���[: tdnMovie�̃����_���\�t�B���^�[�̃C���v�b�g����");

			// �\�[�X�t�B���^�[�̃A�E�g�v�b�g�������_�[�t�B���^�[�̃C���v�b�g�ɂȂ���I�I
			MyAssert(!FAILED(m_pGraphBuilder->Connect(pFSrcPinOut, pFTRPinIn)), "�G���[: tdnMovie�̃t�B���^�[�ڑ�");
		}
	}

	/* WMV�Ƃ�MPEG2�Ƃ� */
	else
	{
		IBasicAudio *pAudio;
		MyAssert(!FAILED(m_pGraphBuilder->QueryInterface(IID_IBasicAudio,(LPVOID*)&pAudio)), "�G���[: tdnMovie�̃I�[�f�B�I�C���^�[�t�F�[�X");
		MyAssert(!FAILED(m_pGraphBuilder->RenderFile(bFilename, nullptr)), "�G���[: tdnMovie�̃����_�[�t�@�C��");

		// �T�E���h�Đ��t���O�ɂ���ĉ��̑傫����ς���(-10000�Ŗ���)
		pAudio->put_Volume(bSound ? 0 : -10000);
	}

	/*
	// �t�B���^�[�ڑ���`���Ă���邭�񐶐�
	MyAssert(!FAILED(CoCreateInstance(CLSID_CaptureGraphBuilder2, nullptr, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (LPVOID*)&m_pCaptureGraphBuilder2)),"�G���[: tdnMovie��ICaptureGraphBuilder2����");
	// �C���^�[�t�F�[�X�؂�ւ�
	m_pCaptureGraphBuilder2->SetFiltergraph(m_pGraphBuilder);

	// �t�B���^�[�ڑ�
	m_pCaptureGraphBuilder2->RenderStream(0, 0, m_pSourceFilter, 0, m_pVMR9);

	// �I�[�f�B�I�ڑ�(�����Ȃ��Ɩ����̓���ɂȂ�)
	m_pCaptureGraphBuilder2->RenderStream(0, &MEDIATYPE_Audio, m_pSourceFilter, 0, 0);

	// �t�B���^�O���t������������ɁAVMR9�ł̓����_�����O��̃E�B���h�E�ɑ΂��ē���̏o�̓T�C�Y��ݒ肷��K�v������܂��B����͂���@������܂��̂ŁA���̒ʂ�Ƀv���O�������܂��B
	// �`��̈�̐ݒ�i�ڑ���łȂ��ƃG���[�ɂȂ�j
	LONG W, H;
	RECT SrcR, DestR;
	MyAssert(!FAILED(pVMRWndControl->GetNativeVideoSize(&W, &H, nullptr, nullptr)),"�G���[: tdnMovie�̕`��̈�̐ݒ�");
	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(tdnSystem::GetWindow(), &DestR);
	pVMRWndControl->SetVideoPosition(&SrcR, &DestR);
	pVMRWndControl->Release();// �p�ς�
	*/

	// ���f�B�A�Ǘ��C���^�[�t�F�[�X����
	MyAssert(!FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&m_pMediaControl)), "�G���[: tdnMovie�̃��f�B�A�C���^�[�t�F�[�X����");

	// ���f�B�A�C�x���g�C���^�[�t�F�[�X����
	MyAssert(!FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (LPVOID*)&m_pMediaEvent)), "�G���[: tdnMovie�̃��f�B�A�C���^�[�t�F�[�X����");

	// �Đ��ʒu�C���^�[�t�F�[�X����
	MyAssert(!FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*)&m_pMediaPosition)), "�G���[: tdnMovie�̍Đ��ʒu�C���^�[�t�F�[�X����");

	// �O���t�X�^�[�g(�Ӗ�����ܕ������ĂȂ�)
	m_pMediaControl->Run();
	// �e�N�X�`���̎擾
	m_pTexture = m_pTextureRenderer->GetTexture();
	/* �� */
	m_pTextureRenderer->GetVideoDesc(&m_lWidth, &m_lHeight, &m_lPitch);

	D3DSURFACE_DESC desc;
	/* �� ������ */
	m_pTexture->GetLevelDesc(0, &desc);

	// �e�N�X�`����2�̗ݏ�ł����m�ۂł��Ȃ��ꍇ�ɉߏ蕪��\�����Ȃ����߂�UV�v�Z
	m_fu = (FLOAT)m_lWidth / (FLOAT)desc.Width;
	m_fv = (FLOAT)m_lHeight / (FLOAT)desc.Height;

	m_pMediaControl->Stop();

	// Graph�𐶐�
	//MyAssert(!FAILED(m_pMediaControl->RenderFile(bFilename)),"�G���[: tdnMovie�̃O���t����");
	// �E�C���h�E�ɂ�������(����������Ȃ��ƁA����ɓ���p�̃E�B���h�E�����������)
	//m_pVideoWindow->put_Owner((OAHWND)tdnSystem::GetWindow());
	//m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	//m_pVideoWindow->SetWindowPosition(0, 0, 1280, 720);
}

tdnMovie::~tdnMovie()
{
#ifdef REGISTER_FILTERGRAPH
	// Pull graph from Running Object Table (Debug)
	RemoveFromROT();
#endif

	// ����~�߂�
	if (m_state == FILTER_STATE::State_Running)m_pMediaControl->Stop();

	// �e�N�X�`����������
	m_pGraphBuilder->RemoveFilter(m_pTextureRenderer);

	// DirectShow�֘A�̉��
	if (m_pMediaEvent)		m_pMediaEvent->Release();		// ���f�B�A�C�x���g���
	if (m_pMediaPosition)	m_pMediaPosition->Release();	// �|�W�V�������
	if (m_pMediaControl)	m_pMediaControl->Release();		// ���f�B�A�Ǘ����
	if (m_pGraphBuilder)	m_pGraphBuilder->Release();		// �O���t���
}

//****************************************************************************************
//		�Đ��E��~
//****************************************************************************************
void tdnMovie::Play()
{
	m_pMediaControl->Run();
	m_state = FILTER_STATE::State_Running;
}

void tdnMovie::Stop()
{
	// �����ĂȂ�������o�Ă������I�I�I
	if (m_state != FILTER_STATE::State_Running)return;

	// ��~����
	m_pMediaControl->Stop();
	m_state = FILTER_STATE::State_Stopped;
}










//
//			TextureRenderer
//

//�O���[�o��
TCHAR g_achCopy[] = TEXT("Bitwise copy of the sample");
TCHAR g_achOffscr[] = TEXT("Using offscreen surfaces and StretchCopy()");
TCHAR g_achDynTextr[] = TEXT("Using Dynamic Textures");
TCHAR* g_pachRenderMethod = NULL;

//�R���X�g���N�^
TextureRenderer::TextureRenderer(LPUNKNOWN pUnk, HRESULT *phr):
CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), NAME("Texture Renderer"), pUnk, phr),
m_bUseDynamicTextures(FALSE)
{
	// Store and AddRef the texture for our use.
	ASSERT(phr);
	if (phr)
		*phr = S_OK;
}

//�f�X�g���N�^
TextureRenderer::~TextureRenderer()
{
}

//-----------------------------------------------------------------------------
// CheckMediaType: This method forces the graph to give us an R8G8B8 video
// type, making our copy to texture memory trivial.
//-----------------------------------------------------------------------------
HRESULT TextureRenderer::CheckMediaType(const CMediaType *pmt)
{
	HRESULT   hr = E_FAIL;
	VIDEOINFO *pvi = 0;

	CheckPointer(pmt, E_POINTER);

	// Reject the connection if this is not a video type
	if (*pmt->FormatType() != FORMAT_VideoInfo) {
		return E_INVALIDARG;
	}

	// Only accept RGB24 video
	pvi = (VIDEOINFO *)pmt->Format();

	if (IsEqualGUID(*pmt->Type(), MEDIATYPE_Video) &&
		IsEqualGUID(*pmt->Subtype(), MEDIASUBTYPE_RGB24))
	{
		hr = S_OK;
	}

	return hr;
}

//-----------------------------------------------------------------------------
// SetMediaType: Graph connection has been made. 
//-----------------------------------------------------------------------------
HRESULT TextureRenderer::SetMediaType(const CMediaType *pmt)
{
	HRESULT hr;

	UINT uintWidth = 2;
	UINT uintHeight = 2;

	// Retrive the size of this media type
	D3DCAPS9 caps;
	VIDEOINFO *pviBmp;                      // Bitmap info header
	pviBmp = (VIDEOINFO *)pmt->Format();

	m_lVidWidth = pviBmp->bmiHeader.biWidth;
	m_lVidHeight = abs(pviBmp->bmiHeader.biHeight);
	m_lVidPitch = (m_lVidWidth * 3 + 3) & ~(3); // We are forcing RGB24

	// here let's check if we can use dynamic textures
	ZeroMemory(&caps, sizeof(D3DCAPS9));
	hr = m_pd3dDevice->GetDeviceCaps(&caps);
	if (caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES)
	{
		m_bUseDynamicTextures = TRUE;
	}

	if (caps.TextureCaps & D3DPTEXTURECAPS_POW2
		&& caps.TextureCaps & !D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
	{
		while ((LONG)uintWidth < m_lVidWidth)
		{
			uintWidth = uintWidth << 1;
		}
		while ((LONG)uintHeight < m_lVidHeight)
		{
			uintHeight = uintHeight << 1;
		}
	}
	else
	{
		uintWidth = m_lVidWidth;
		uintHeight = m_lVidHeight;
		m_bUseDynamicTextures = FALSE;
	}

	// Create the texture that maps to this media type
	hr = E_UNEXPECTED;
	if (m_bUseDynamicTextures)
	{
		hr = m_pd3dDevice->CreateTexture(uintWidth, uintHeight, 1, 0,
			D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT,
			&m_pTexture, NULL);
		g_pachRenderMethod = g_achDynTextr;
		if (FAILED(hr))
		{
			m_bUseDynamicTextures = FALSE;
		}
	}
	if (FALSE == m_bUseDynamicTextures)
	{
		hr = m_pd3dDevice->CreateTexture(uintWidth, uintHeight, 1, 0,
			D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
			&m_pTexture, NULL);
		g_pachRenderMethod = g_achCopy;
	}
	if (FAILED(hr))
	{
		return hr;
	}

	// CreateTexture can silently change the parameters on us
	D3DSURFACE_DESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	if (FAILED(hr = m_pTexture->GetLevelDesc(0, &ddsd))) {
		return hr;
	}


	IDirect3DSurface9 *pSurf;

	if (SUCCEEDED(hr = m_pTexture->GetSurfaceLevel(0, &pSurf)))
	{
		pSurf->GetDesc(&ddsd);
	}

	// Save format info
	m_TextureFormat = ddsd.Format;

	if (m_TextureFormat != D3DFMT_X8R8G8B8 &&
		m_TextureFormat != D3DFMT_A1R5G5B5) {
		return VFW_E_TYPE_NOT_ACCEPTED;
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// DoRenderSample: A sample has been delivered. Copy it to the texture.
//-----------------------------------------------------------------------------
HRESULT TextureRenderer::DoRenderSample(IMediaSample * pSample)
{
	BYTE  *pBmpBuffer, *pTxtBuffer; // Bitmap buffer, texture buffer
	LONG  lTxtPitch;                // Pitch of bitmap, texture

	BYTE  * pbS = NULL;
	DWORD * pdwS = NULL;
	DWORD * pdwD = NULL;
	UINT row, col, dwordWidth;

	CheckPointer(pSample, E_POINTER);
	CheckPointer(m_pTexture, E_UNEXPECTED);

	// Get the video bitmap buffer
	pSample->GetPointer(&pBmpBuffer);

	// Lock the Texture
	D3DLOCKED_RECT d3dlr;
	if (m_bUseDynamicTextures)
	{
		if (FAILED(m_pTexture->LockRect(0, &d3dlr, 0, D3DLOCK_DISCARD)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture->LockRect(0, &d3dlr, 0, 0)))
			return E_FAIL;
	}
	// Get the texture buffer & pitch
	pTxtBuffer = static_cast<byte *>(d3dlr.pBits);
	lTxtPitch = d3dlr.Pitch;


	// Copy the bits
	pTxtBuffer += lTxtPitch*(m_lVidHeight - 1);

	if (m_TextureFormat == D3DFMT_X8R8G8B8)
	{
		// Instead of copying data bytewise, we use DWORD alignment here.
		// We also unroll loop by copying 4 pixels at once.
		//
		// original BYTE array is [b0][g0][r0][b1][g1][r1][b2][g2][r2][b3][g3][r3]
		//
		// aligned DWORD array is     [b1 r0 g0 b0][g2 b2 r1 g1][r3 g3 b3 r2]
		//
		// We want to transform it to [ff r0 g0 b0][ff r1 g1 b1][ff r2 g2 b2][ff r3 b3 g3]
		// below, bitwise operations do exactly this.

		dwordWidth = m_lVidWidth / 4; // aligned width of the row, in DWORDS
		// (pixel by 3 bytes over sizeof(DWORD))

		for (row = 0; row< (UINT)m_lVidHeight; row++)
		{
			pdwS = (DWORD*)pBmpBuffer;
			pdwD = (DWORD*)pTxtBuffer;

			for (col = 0; col < dwordWidth; col++)
			{
				pdwD[0] = pdwS[0] | 0xFF000000;
				pdwD[1] = ((pdwS[1] << 8) | 0xFF000000) | (pdwS[0] >> 24);
				pdwD[2] = ((pdwS[2] << 16) | 0xFF000000) | (pdwS[1] >> 16);
				pdwD[3] = 0xFF000000 | (pdwS[2] >> 8);
				pdwD += 4;
				pdwS += 3;
			}

			// we might have remaining (misaligned) bytes here
			pbS = (BYTE*)pdwS;
			for (col = 0; col < (UINT)m_lVidWidth % 4; col++)
			{
				*pdwD = 0xFF000000 |
					(pbS[2] << 16) |
					(pbS[1] << 8) |
					(pbS[0]);
				pdwD++;
				pbS += 3;
			}

			pBmpBuffer += m_lVidPitch;
			pTxtBuffer -= lTxtPitch;
		}// for rows
	}

	if (m_TextureFormat == D3DFMT_A1R5G5B5)
	{
		for (int y = 0; y < m_lVidHeight; y++)
		{
			BYTE *pBmpBufferOld = pBmpBuffer;
			BYTE *pTxtBufferOld = pTxtBuffer;

			for (int x = 0; x < m_lVidWidth; x++)
			{
				*(WORD *)pTxtBuffer = (WORD)
					(0x8000 +
					((pBmpBuffer[2] & 0xF8) << 7) +
					((pBmpBuffer[1] & 0xF8) << 2) +
					(pBmpBuffer[0] >> 3));

				pTxtBuffer += 2;
				pBmpBuffer += 3;
			}

			pBmpBuffer = pBmpBufferOld + m_lVidPitch;
			pTxtBuffer = pTxtBufferOld + lTxtPitch;
		}
	}

	// Unlock the Texture
	if (FAILED(m_pTexture->UnlockRect(0)))
		return E_FAIL;

	return S_OK;
}