#include	"TDNLIB.h"
#include	"../source/resource.h"

/********************************************/
//	tdnSystem
/********************************************/

/* ���̐錾 */
HINSTANCE tdnSystem::hinst;				// �A�v���P�[�V�������̂�\���Ă���ω����Ȃ����̂悤�ȁu�l�v
HWND	tdnSystem::Window;				// �E�B���h�E�n���h��// �P�̃E�B���h�E��\���u�l�v�ł�
BOOL	tdnSystem::WindowActive =FALSE; // �E�B���h�E���~�߂�t���O
LPDIRECT3D9	tdnSystem::lpD3D;			// DirectX9�ɃA�N�Z�X���邽�߂̃|�C���^			
LPDEVICE	tdnSystem::Device;			// �r�f�I�J�[�h�ɃA�N�Z�X���邽�߂�3D�f�o�C�X
D3DPRESENT_PARAMETERS tdnSystem::d3dpp;	// �f�B�X�v���C�̃p�����[�^�[ �t���X�N���[���Ȃǂ�
ID3DXLine* tdnSystem::pLine;			// ID3DXLine �C���^�[�t�F�C�X�̓e�N�X�`���[�����ꂽ�O�p�`���g�p�������`����������܂��B
RECT	tdnSystem::ScreenSize;			// �X�N���[���̃T�C�Y
D3DFORMAT tdnSystem::ScreenFormat;		// �X�N���[���̃t�H�[�}�b�g
BOOL tdnSystem::FullScreen;				// �t���X�N���[���Ȃ̂��H
ENDIAN tdnSystem::endian;				// �G���f�B�A��(�r�b�Oor���g��)

/* �O���[�o���ϐ� */
//	�ϊ��s��
Matrix		matView;					// �r���[�s��
Matrix		matProjection;				// �v���W�F�N�V�����s��

// �E�B���h�E�v���V�[�W��(Winmain�̗l�ȑ���)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��ɒʂ��Ă�
	// �E�C���h�E���瑗���Ă��郁�b�Z�[�W���L���b�`���ď�������֐��̎��D�R�[���o�b�N�֐��ł���B
	switch (message){
	case WM_CREATE:/*�E�B���h�E���Ăяo���ꂽ�Ƃ���1�x��������*/
		break;
	case WM_ACTIVATEAPP:
		if (wParam){
			tdnSystem::SetWindowActive(TRUE);
		}
		else
		{
			tdnSystem::SetWindowActive(FALSE);
		}
		break;
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;// Esc�L�[����������E�B���h�E�����
		case VK_F1:		tdnSystem::OpenDebugWindow(); return 0;		// �R���\�[����ʂ��o��
		}
		break;
	case WM_MOUSEWHEEL:	// �}�E�X�z�C�[���C�x���g
	{
						  static short mouse_wheel_delta = 0;
						  mouse_wheel_delta += GET_WHEEL_DELTA_WPARAM(wParam);
						  if (mouse_wheel_delta >= 120){
							  mouse_wheel_delta = 0;
							  tdnMouse::SetWheel(WHEEL_FLAG::UP);
						  }
						  else if (mouse_wheel_delta <= 120){
							  mouse_wheel_delta = 0;
							  tdnMouse::SetWheel(WHEEL_FLAG::DOWN);
						  }
	}
		break;

	case WM_DROPFILES:	// �h���b�O&�h���b�v�C�x���g
	{
							//char filename[256] = {};
							//DRAG_AND_DROP::Get_file_path(filename, 256, wParam);
							break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);// �����ŏ������Ȃ����b�Z�[�W��Windows�̕��ŏ������Ă��炤

}

// �E�B���h�E��������
BOOL tdnSystem::InitWindow(HINSTANCE hInstance, int nCmdShow, char* AppTitle, DWORD ScreenMode, BOOL isFullScreen)
{
	// �X�N���[���T�C�Y���擾
	ScreenSize = GetScreenRect(ScreenMode); // ���[�h�ɍ��킹�T�C�Y���擾

	// �t���X�N���[���ɂ���t���O��ۑ�
	FullScreen = isFullScreen;

	// ��D3D�I�u�W�F�N�g�̍쐬
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	std::vector<D3DDISPLAYMODE> DModes;	// PC�̃f�B�X�v���C���[�h��ۑ�����ϒ��z��
	D3DDISPLAYMODE    d3dspMode;		// PC�̃f�B�X�v���C���[�h���i�[

	// PC�ɉ���ނ̃f�B�X�v���C�����邩���ׂ�
	int DISPLAY_COUNT = lpD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);

	// �f�B�X�v���C���[�h�𒲂ׂ�
	for (int i = 0; i < DISPLAY_COUNT; ++i) {
		lpD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &d3dspMode);
		// �f�B�X�v���C���[�h���ϒ��z��ɋL��
		DModes.push_back(d3dspMode);
	}


	if (FullScreen == TRUE) // �܂��t���X�N���[���ɂł���𑜓x���p�\�R���ɂ��邩�T��
	{
		for (int i = 0; i < DISPLAY_COUNT; i++)
		{
			if (DModes[i].Width == ScreenSize.right && DModes[i].Height == ScreenSize.bottom &&
				DModes[i].RefreshRate == 60)// �t���X�N���[���̃T�C�Y�⃊�t���b�V�����[�g�̐ݒ�
			{
				// (��) �����ŃT�C�Y�ۑ��Ȃ�Ȃ�Ȃ�B
				// �����ɒʂ��Ă���ł�Handle�G���[���o���ꍇ�̓f���A�����j�^�[�Ȃǂ��Ă�\���������B
				break;// �𑜓x��v->OK!
			}
			else if (i == (DISPLAY_COUNT - 1))// �Ō�܂Ŋm���߂Ă���v����𑜓x���Ȃ�������
			{
				MessageBox(0, "�t���X�N���[���ɂ��邽�߂̉𑜓x�����Ȃ��̃p�\�R����\n�ݒ肳��Ă��Ȃ������̂ŃE�B���h�E���[�h�Ɉڍs���܂��B", "System", MB_OK);
				FullScreen = false;	//�@�����I�ɃE�B���h�E���[�h��
			}
		}
	}


	// �E�B���h�E�N���XEX�̐ݒ�
	WNDCLASSEX wcex;											// �E�B���h�E�N���XEX�Ƃ́A�E�B���h�E�̐����ɕK�v�ȏ������\����
	wcex.cbSize = sizeof(WNDCLASSEX);							// �\���̂ɕK�v�ȃo�C�g����sizeof(WNDCLASSEX)���w�肷��B
	wcex.style = CS_HREDRAW | CS_VREDRAW;						// �E�B���h�E�̃X�^�C����ݒ肵�܂��A�r�b�g���Z�q�ŃX�^�C����g�ݍ��킹��
	wcex.lpfnWndProc = WindowProc;								// �E�B���h�E�v���V�[�W���ւ̊֐��|�C���^��ݒ肵�܂�
	wcex.cbClsExtra = 0;										// ���̍\���̂Ɋ��蓖�Ă�]���ȃ�������ݒ肵�A0�ŏ��������܂�
	wcex.cbWndExtra = 0;										// �E�B���h�E�C���X�^���X(���ۂ̃E�B���h�E)�Ɋ��蓖�Ă��� �]���ȃ�������ݒ肵�A0�ŏ���������܂�
	wcex.hInstance = hInstance;									// ���̃N���X�̃E�B���h�E�v���V�[�W�����܂ރA�v���P�[�V���� �C���X�^���X�n���h����ݒ肵�܂��B
	wcex.hIcon = LoadIcon(hInstance, LPSTR(IDI_ICON1));			// �A�C�R���̃n���h����ݒ肵�܂��B
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					// �J�[�\���̃n���h����ݒ肵�܂��B 
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// �w�i�̃n���h����ݒ肵�܂��B 
	wcex.lpszMenuName = NULL;									// ���j���[���\�[�X�̖��O��ݒ肵�܂�
	wcex.lpszClassName = "classname";							// �E�B���h�E�N���X�̖��O��ݒ肵�܂�
	wcex.hIconSm = NULL;										// �X���[���A�C�R���̃n���h��

	// �ݒ肵����RegisterClassEx�œo�^!
	RegisterClassEx(&wcex);

	// �傫�����ς��̂ňڂ�
	RECT WindowSize = ScreenSize;

	// �ݒ肵������ۂɃE�B���h�E�����܂��B�@�߂�l�ɃE�B���h�E�����ʂ��邽�߂̃n���h�����A���Ă��܂��B
	// �t���X�N���[���ŕ`�悷�邩�H
	if (FullScreen == false)
	{
		// (?) AdjustWindowRectEx�̍ŏ��ȊO���Ӗ����Ȃ��Ă��Ȃ��H
		AdjustWindowRectEx(&WindowSize, // �N���C�A���g�̈�̍��W���������\���̂ւ̃|�C���^�@�t���[�����݂̑傫����
			WS_OVERLAPPEDWINDOW,		// �E�B���h�E�X�^�C��
			FALSE,						// ���j���[�������ǂ����̎w��
			NULL);						// �g���E�B���h�E�X�^�C��

		const	LONG	WindowCoordX = 1920 / 2 - 1280 / 2;
		const	LONG	WindowCoordY = 1080 / 2 - 720 / 2;

		Window = CreateWindowEx(WS_EX_ACCEPTFILES, wcex.lpszClassName, AppTitle, WS_SYSMENU,	// 1.�g���E�B���h�E�X�^�C���@2.�o�^�����N���X���@3.�E�B���h�E���@4.�E�B���h�E�X�^�C��
			WindowCoordX, WindowCoordY, WindowSize.right - WindowSize.left, WindowSize.bottom - WindowSize.top,		// �E�B���h�E�̏����ʒu��T�C�Y
			NULL, NULL, hInstance, NULL);														// NULL�ɂ͓��ɈӖ��͂Ȃ��A�v���P�[�V�����̃C���X�^���X�n���h����������

	}
	else // �t���X�N���[��
	{

		Window = CreateWindow(wcex.lpszClassName, AppTitle, WS_POPUP,
			0, 0, WindowSize.right, WindowSize.bottom,
			NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);

	}

	ShowWindow(Window, nCmdShow);	// �w�肳�ꂽ�E�B���h�E�̕\����Ԃ�ݒ肵�܂��B
	UpdateWindow(Window);			// ���̃E�B���h�E�̃N���C�A���g�̈���X�V���܂�

	// D3D������
	if (!InitD3D())
	{
		MessageBox(0, "D3D���������s�B", "System", MB_OK);
	}

	// �G���f�B�A���`�F�b�N
	auto EndianCheck = [](){ int bit = 1; return (*(char*)&bit) ? LITTLE_ENDIAN : BIG_ENDIAN; };
	endian = EndianCheck();

	return TRUE;
}

// (��) ���D3D�̏�����������->�t���X�N���[�� 
// DirectX��������
BOOL tdnSystem::InitD3D()
{
	//lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	// �A�_�v�^�́u���݁v�̃f�B�X�v���C�̃T�C�Y���擾����(FullScreen�p)
	D3DDISPLAYMODE d3ddm;
	lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);// �A�_�v�^�̌��݂̃f�B�X�v���C ���[�h���擾����B���͎g���Ă��Ȃ�


	if (FullScreen) // �t���X�N���[��
	{
		// D3D�f�o�C�X(�E�B���h�E)�p�����[�^�̐ݒ�
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Flags = 0;											// 
		d3dpp.BackBufferHeight = ScreenSize.bottom;					// �o�b�N�o�b�t�@�̕�
		d3dpp.BackBufferWidth = ScreenSize.right;					// �o�b�N�o�b�t�@�̍���
		d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐��@������1�łȂ���΂����Ȃ�
		d3dpp.hDeviceWindow = Window;								// �E�B���h�E�@�t���X�N���[���̏ꍇ�̓J�o�[�E�B���h�E��
		d3dpp.EnableAutoDepthStencil = TRUE;						// TRUE�Ȃ�[�x�X�e���V���o�b�t�@���쐬����
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// �f�o�C�X���쐬���鎩���[�x�X�e���V���T�[�t�F�C�X�̃t�H�[�}�b�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �A�_�v�^�̃��t���b�V�����[�g�@D3DPRESENT�@
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				// _NONE->������̃��x���̃t���V�[���}���`�T���v�����O�����p�ł��Ȃ�
		d3dpp.MultiSampleQuality = 0;								// �T���v���i�����x��

		d3dpp.Windowed = FALSE;										// TRUE�ŃE�B���h�E�@FALSE�Ńt���X�N���[��
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �X���b�v�`�F�[���̒�` D3DSWAPEFFECT_DISCARD
		d3dpp.BackBufferFormat = d3ddm.Format;						// �t���X�N���[���ŕ`�悷��Ȃ�d3ddm����擾����Format���g�p����B
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// �f�B�X�v���C �A�_�v�^�ɂ��X�N���[���̃��t���b�V�����[�g,���̒l��0�łȂ���΂Ȃ�Ȃ�
	}
	else // �E�B���h�E���[�h
	{
		// D3D�f�o�C�X(�E�B���h�E)�p�����[�^�̐ݒ�
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Flags = 0;											// 
		d3dpp.BackBufferHeight = ScreenSize.bottom;					// �o�b�N�o�b�t�@�̕�
		d3dpp.BackBufferWidth = ScreenSize.right;					// �o�b�N�o�b�t�@�̍���
		d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐��@������1�łȂ���΂����Ȃ�
		d3dpp.hDeviceWindow = Window;								// �E�B���h�E�@�t���X�N���[���̏ꍇ�̓J�o�[�E�B���h�E��
		d3dpp.EnableAutoDepthStencil = TRUE;						// TRUE�Ȃ�[�x�X�e���V���o�b�t�@���쐬����
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// �f�o�C�X���쐬���鎩���[�x�X�e���V���T�[�t�F�C�X�̃t�H�[�}�b�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �A�_�v�^�̃��t���b�V�����[�g�@D3DPRESENT�@
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				// _NONE->������̃��x���̃t���V�[���}���`�T���v�����O�����p�ł��Ȃ�
		d3dpp.MultiSampleQuality = 0;								// �T���v���i�����x��

		d3dpp.Windowed = TRUE;										// TRUE�ŃE�B���h�E�@FALSE�Ńt���X�N���[��
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �X���b�v�`�F�[���̒�` D3DSWAPEFFECT_DISCARD
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					// D3DFMT_UNKNOWN->���݂̃f�B�X�v���C���[�h�t�H�[�}�b�g���g��
	}

	//	�f�o�C�X�쐬
	D3DCAPS9 Caps;
	ZeroMemory(&Caps, sizeof(Caps));
	//	HAL�`�F�b�N
	//D3DDEVTYPE_HAL->�n�[�h�E�F�A�ɂ�郉�X�^�[���B�V�F�[�f�B���O�́A�\�t�g�E�F�A�A�n�[�h�E�F�A�A�܂��̓g�����X�t�H�[���ƃ��C�e�B���O�̑g�ݍ��킹�ɂ���Ď��s����܂��B
	if (SUCCEEDED(lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps)))
	{
		// ������OK�������灝
		// �n�[�h�E�F�A�ɂ�钸�_�������w�肵�܂��B �� �}���`�X���b�h�����S�ɍs��(�p�t�H�[�}���X�͒Ⴂ)
		if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &Device)))
		{
			// ���Ɏ��s������
			// �\�t�g�E�F�A����уn�[�h�E�F�A�ɂ��~�b�N�X�̒��_�������w�肵�܂��B �Ɓ@�}���`�X���b�h�����S�ɍs��(�p�t�H�[�}���X�͒Ⴂ)
			if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &Device)))
			{
				// ���Ɏ��s������
				// �\�t�g�E�F�A�ɂ�钸�_�������w�肵�܂��Ɓ@�}���`�X���b�h�����S�ɍs��(�p�t�H�[�}���X�͒Ⴂ)
				if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &Device)))
				{
					return FALSE;	// �����Q�[
				}
			}
		}
	}
	else 
	{
		ZeroMemory(&Caps, sizeof(Caps));
		// Direct3D �@�\���u�\�t�g�E�F�A�v�Ɏ�������܂��B�������A���t�@�����X���X�^���C�U�[�́A�\�ȏꍇ�A�����CPU ���߂𗘗p���܂��B
		lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &Caps);
		if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &Device)))
			return FALSE;
	}
	
	// ��ʃo�b�N�o�b�t�@�t�H�[�}�b�g��ۑ�
	ScreenFormat = d3dpp.BackBufferFormat;	// FMT2D::RENDERTARGET

	// ���̑�
	// ID3DXLine�C���^�[�t�F�C�X�̐���
	D3DXCreateLine(tdnSystem::Device, &pLine);

	return TRUE;
}

// ���
void tdnSystem::Release()
{
	SAFE_RELEASE(Device);
	SAFE_RELEASE(lpD3D);
	SAFE_RELEASE(pLine);
	////�N���X���
	//UnregisterClass("classname", hinst);
}

//*****************************************************************************
//		�E�B���h�E���b�Z�[�W�����֐�
//*****************************************************************************
//BOOL tdnSystem::Message()
//{
//	MSG msg;
//	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))// ���b�Z�[�W�������TRUE
//	{
//		if (!GetMessage(&msg, NULL, 0, 0)) return FALSE;
//		TranslateMessage(&msg);// ���b�Z�[�W���͂��v���O�����ŏ����ł���`���ɕϊ�
//		DispatchMessage(&msg);// WindowProc���Ăт����A���b�Z�[�W�������s���B
//	}
//
//
//}

//*****************************************************************************
//		���[�h�ʃX�N���[���T�C�Y�擾
//*****************************************************************************
RECT tdnSystem::GetScreenRect(DWORD mode)
{
	RECT rc;
	rc.left = rc.top = 0;

	switch (mode){
	case SCREEN800x450:		rc.right = 800;  rc.bottom = 450;	break;
	case SCREEN1280x720:	rc.right = 1280; rc.bottom = 720;	break;
	case SCREEN1920x1080:	rc.right = 1920; rc.bottom = 1080;	break;
	case SCREEN800x600:		rc.right = 800; rc.bottom = 600;	break;
	default:
		rc.right = (mode >> 16);
		rc.bottom = (mode & 0xFFFF);
		MessageBox( 0, "�X�N���[���T�C�Y�̃��[�h����������", NULL, MB_OK );
		break;
	}

	return rc;
}


//*****************************************************************************
//		
//		IEX�ɓ����Ă�����B�F���g���Ă���̂Ȃ�c���Ă����܂��B
//		�f�o�b�O�p�E�B���h�E
//
//*****************************************************************************
#include <stdio.h>
static FILE* DebugFP = NULL;

void tdnSystem::OpenDebugWindow()
{
#ifdef _DEBUG
	AllocConsole();
	freopen_s(&DebugFP, "CON", "w", stdout);
	freopen_s(&DebugFP, "CON", "r", stdin);		// ���̕����Ȃ��ƃ��[�U�[����̓��͂��󂯕t�����Ȃ�
#endif
}

void tdnSystem::CloseDebugWindow()
{
#ifdef _DEBUG
	if (DebugFP)
	{
		fclose(DebugFP);
		FreeConsole();
	}
#endif
}

void tdnSystem::printf(const char* format, ...)
{
#ifdef _DEBUG
	va_list arg;
	va_start(arg, format);

	vprintf(format, arg);

	va_end(arg);
#endif
}
