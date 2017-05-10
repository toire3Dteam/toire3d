#include "TDNLIB.h"

//*****************************************************************************************************************************
//
//				管理
//
//*****************************************************************************************************************************
//------------------------------------------------------
//		静的メンバ変数
//------------------------------------------------------
LPDIRECTINPUT8 tdnInputManager::m_lpDI = nullptr;
int tdnInputManager::m_NumDevice = 0;
DIDEVICEINSTANCE tdnInputManager::m_DeviceInstances[tdnInputEnum::INPUT_DEVICE_MAX];
KEY_CONFIG_DATA tdnInputManager::m_tagConfigDatas[tdnInputEnum::INPUT_DEVICE_MAX];
IDirectInputDevice8* tdnInputManager::m_pMouse;
std::vector<KEY_CONFIG_DATA> tdnInputManager::m_vConfigList;

DWORD tdnInputManager::m_dwNumForceFeedbackAxis = 0;

// 振動オン
//#define VIBRATION_ON

//------------------------------------------------------
//		コントローラー列挙
//------------------------------------------------------
BOOL CALLBACK tdnInputManager::EnumDeviceCallback(const DIDEVICEINSTANCE* pdidi, VOID* pContext)
{
	//	情報のコピー
	memcpy(&m_DeviceInstances[m_NumDevice], pdidi, sizeof(DIDEVICEINSTANCE));

	/*
	•DI8DEVTYPE_GAMEPAD
	ゲームパッド。次のサブタイプが定義される。
	◦DI8DEVTYPEGAMEPAD_LIMITED
	アクション マップ対象のデバイス オブジェクトの最小数を指定しないゲームパッド。

	◦DI8DEVTYPEGAMEPAD_STANDARD
	アクション マップ対象のデバイス オブジェクトの最小数を指定しない標準ゲームパッド。

	◦DI8DEVTYPEGAMEPAD_TILT
	コントローラの姿勢から x 軸と y 軸のデータを報告するゲームパッド。


	•DI8DEVTYPE_JOYSTICK
	ジョイスティック。次のサブタイプが定義される。
	◦DI8DEVTYPEJOYSTICK_LIMITED
	アクション マップ対象のデバイス オブジェクトの最小数を指定しないジョイスティック。

	◦DI8DEVTYPEJOYSTICK_STANDARD
	アクション マップ対象のデバイス オブジェクトの最小数を指定する標準ジョイスティック。
	*/

	//if (device_instances[num_device].dwDevType & DI8DEVTYPE_GAMEPAD)
	//{
	//	if (device_instances[num_device].dwDevType & DI8DEVTYPEGAMEPAD_STANDARD)
	//		int i = 0;
	//}
	//if (device_instances[num_device].dwDevType & DI8DEVTYPE_JOYSTICK)
	//{
	//	if (device_instances[num_device].dwDevType & DI8DEVTYPEJOYSTICK_STANDARD)
	//		int i = 0;
	//}


	/*
	tszInstanceName
	インスタンスの登録名。たとえば、"Joystick 1"。
	*/

	// コンフィグcontroller検索
	bool find(false);
	for(auto it : m_vConfigList)
	{
		// 一致してたら
		if (strcmp(m_DeviceInstances[m_NumDevice].tszInstanceName, it.tszPadName) == 0)
		{
			find = true;

			// controller情報登録
			memcpy_s(&m_tagConfigDatas[m_NumDevice], sizeof(KEY_CONFIG_DATA), &it, sizeof(KEY_CONFIG_DATA));
			break;
		}
	}

	// 見つからなかったので新規登録
	if (!find)
	{
		KEY_CONFIG_DATA l_tagConfigData;

		// controllerの名前
		strcpy_s(l_tagConfigData.tszPadName, MAX_PATH, m_DeviceInstances[m_NumDevice].tszInstanceName);
		
		// controllerのコンフィグ
		l_tagConfigData.tagPadSet.AllDatas[0] = AXIS_X;
		l_tagConfigData.tagPadSet.AllDatas[1] = AXIS_Y;
		l_tagConfigData.tagPadSet.AllDatas[2] = AXIS_Z;
		l_tagConfigData.tagPadSet.AllDatas[3] = AXIS_RZ;
		l_tagConfigData.tagPadSet.AllDatas[4] = 3;
		l_tagConfigData.tagPadSet.AllDatas[5] = 4;
		l_tagConfigData.tagPadSet.AllDatas[6] = 1;
		l_tagConfigData.tagPadSet.AllDatas[7] = 2;
		l_tagConfigData.tagPadSet.AllDatas[8] = 5;
		l_tagConfigData.tagPadSet.AllDatas[9] = 7;
		l_tagConfigData.tagPadSet.AllDatas[10] = 11;
		l_tagConfigData.tagPadSet.AllDatas[11] = 6;
		l_tagConfigData.tagPadSet.AllDatas[12] = 8;
		l_tagConfigData.tagPadSet.AllDatas[13] = 12;
		l_tagConfigData.tagPadSet.AllDatas[14] = 10;
		l_tagConfigData.tagPadSet.AllDatas[15] = 9;

		// controller情報登録
		memcpy_s(&m_tagConfigDatas[m_NumDevice], sizeof(KEY_CONFIG_DATA), &l_tagConfigData, sizeof(KEY_CONFIG_DATA));

		// リスト追加
		m_vConfigList.push_back(l_tagConfigData);

		// テキストにも追加します
		std::ofstream ofs("DATA/Input/key_config.txt", std::ios::out | std::ios::ate | std::ios::app);
		ofs << "NAME: " << m_DeviceInstances[m_NumDevice].tszInstanceName << "\n";
		ofs << "3 4 1 2 5 7 11 6 8 12 10 9\n\n";
	}

	//	最大
	if (++m_NumDevice >= tdnInputEnum::INPUT_DEVICE_MAX) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}
//------------------------------------------------------
//	初期化
//------------------------------------------------------
void tdnInputManager::Initialize()
{
	// コンフィグ情報読み込み
	LoadConfig();

	m_lpDI = nullptr;
	MyAssert(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDI, nullptr) == DI_OK, "DirectInputの初期化でエラー");
	m_lpDI->Initialize(GetModuleHandle(nullptr), DIRECTINPUT_VERSION);

	// マウスデバイス初期化
	m_lpDI->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);
	// データ形式を保存
	m_pMouse->SetDataFormat(&c_dfDIMouse2);
	// 強調レベルの設定(非排他)
	m_pMouse->SetCooperativeLevel(tdnSystem::GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	m_NumDevice = 0;

	// コンフィグ初期化
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++) ZeroMemory(&m_tagConfigDatas[i], sizeof(KEY_CONFIG_DATA));

	//	ゲームパッドの列挙(この中でIDの振り分けしてます)
	m_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDeviceCallback, m_lpDI, DIEDFL_ATTACHEDONLY
#ifdef VIBRATION_ON
	| DIEDFL_FORCEFEEDBACK
#endif
	);
}

//------------------------------------------------------
//	コンフィグデータ読み込み
//------------------------------------------------------
void tdnInputManager::LoadConfig()
{
	// 既存バッファ解放
	m_vConfigList.clear();

	// ファイルオープン
	std::ifstream ifs("DATA/Input/key_config.txt");
	if (!ifs) return;

	while (!ifs.eof())
	{
		// リストにセットする用の変数
		KEY_CONFIG_DATA l_tagConfigData;

		// 名前まで読み飛ばし
		char yomitobashi[64];
		{
			do
			{
				ifs >> yomitobashi;
			} while (strcmp(yomitobashi, "NAME:") != 0 && !ifs.eof());
			ifs.read(yomitobashi, 1);	// TDN空白
		}

		if (ifs.eof()) return;

		// ファイルの読み込みが空白で区切りやがるので、1バイトずつ読まないといけない
		{
			int i(0);
			while (!ifs.eof())
			{
				char c;
				ifs.read(&c, 1);

				// 終端マーク
				if (c == '\n')
				{
					l_tagConfigData.tszPadName[i] = '\0';
					break;
				}

				l_tagConfigData.tszPadName[i++] = c;
			}
		}

		// キー配置読み込み
		l_tagConfigData.tagPadSet.AllDatas[0] = AXIS_X, l_tagConfigData.tagPadSet.AllDatas[1] = AXIS_Y, l_tagConfigData.tagPadSet.AllDatas[2] = AXIS_Z, l_tagConfigData.tagPadSet.AllDatas[3] = AXIS_RZ;
		for (int i = 4; i < 16; i++)
		{
			int n;
			ifs >> n;
			l_tagConfigData.tagPadSet.AllDatas[i] = (BYTE)n;
		}

		// リストにセット
		m_vConfigList.push_back(l_tagConfigData);
	}
}

//------------------------------------------------------
//		軸列挙
//------------------------------------------------------
BOOL CALLBACK tdnInputManager::EnumAxes(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	// 軸範囲を設定（-1000～1000）
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -tdnInputEnum::STICK_WIDTH;
	diprg.lMax = +tdnInputEnum::STICK_WIDTH;
	if (FAILED(((LPDIRECTINPUTDEVICE8)pvRef)->SetProperty(DIPROP_RANGE, &diprg.diph))) return DIENUM_STOP;

#ifdef VIBRATION_ON
	DWORD *pdwNumForceFeedbackAxis = (DWORD*)pvRef;
	if ((lpddoi->dwFlags & DIDOI_FFACTUATOR) != 0) (*pdwNumForceFeedbackAxis)++;
#endif

	return DIENUM_CONTINUE;
}

//------------------------------------------------------
//	デバイス取得
//------------------------------------------------------
LPDIRECTINPUTDEVICE8 tdnInputManager::CreateDevice(int no)
{
	HRESULT	hr;
	LPDIRECTINPUTDEVICE8	lpDevice;

	//	デバイス生成
	hr = m_lpDI->CreateDevice(m_DeviceInstances[no].guidInstance, &lpDevice, NULL);
	if (FAILED(hr)) return nullptr;

	// フォーマット設定(キーボードとかジョイスティックとか)
	if (lpDevice->SetDataFormat(&c_dfDIJoystick2) != DI_OK) return nullptr;

	// 強調レベル設定
	/*
	DISCL_FOREGROUND	アプリケーションが非アクティブ状態になると自動的にデバイスの入力を停止する こっちを採用
	DISCL_BACKGROUND	常に入力を続ける

	DISCL_EXCLUSIVE(排他占有)	アクセス権を解放するまで強調レベルを設定した以外のウィンドウは制御できない(ジョイパッドならこれだが、マウスとかキーボードでやると最悪OS終了できない危険性がある)
	DISCL_NOEXCLUSIVE(非排他占有)	↑の逆
	*/
	if (lpDevice->SetCooperativeLevel(tdnSystem::GetWindow(), DISCL_FOREGROUND | DISCL_EXCLUSIVE) != DI_OK) return nullptr;


	// 情報設定
	DIPROPDWORD	dipdw;
	ZeroMemory(&dipdw, sizeof(dipdw));
	dipdw.diph.dwSize = sizeof(dipdw);				// 使用される構造体のサイズ
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);	// DIPROPHEADER構造体のサイズ
	dipdw.diph.dwObj = 0;							// デバイスを対象とする時はいつも0
	dipdw.diph.dwHow = DIPH_DEVICE;					// 対象（ここではデバイス）
	dipdw.dwData = DIPROPAUTOCENTER_OFF;			// 自動センタリング無効

	/*
	DIPROP_AUTOCENTER
	デバイス オブジェクトが自動センタリングを行うかどうかを指定する。
	この設定値は、個別のオブジェクトではなくデバイス全体に適用されるので、割り当てられた DIPROPDWORD 構造体の dwHow メンバは、DIPH_DEVICE でなければならない。
	dwData メンバには、次のいずれかの値を指定できる。
	DIPROPAUTOCENTER_OFF : ユーザーがデバイスを離しても、自動的にセンタリングを行わない。フォース フィードバックを使うアプリケーションは、エフェクトの再生前に自動センタリングを無効にする必要がある。
	DIPROPAUTOCENTER_ON : ユーザーがデバイスを離すと、自動的にセンタリングを行う。
	自動センタリング機能をサポートしていないデバイスもある。
	*/

	// 要するに振動とかのエフェクトを円滑に行えるようにセンタリング設定をOFFにしている
	lpDevice->SetProperty(
		DIPROP_AUTOCENTER,
		&dipdw.diph);

	// 各軸設定
	lpDevice->EnumObjects(
		EnumAxes,
#ifdef VIBRATION_ON
		(LPVOID)&m_dwNumForceFeedbackAxis,
#else
		lpDevice,
#endif
		DIDFT_AXIS);

#ifdef VIBRATION_ON
	if (m_dwNumForceFeedbackAxis > 2) m_dwNumForceFeedbackAxis = 2;
#endif

	// 入力制御開始
	m_pMouse->Acquire();
	lpDevice->Acquire();

	return lpDevice;
}

bool tdnInputManager::CreateEffect(LPDIRECTINPUTEFFECT pEffect, LPDIRECTINPUTDEVICE8 lpDevice)
{
	DWORD           rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
	LONG            rglDirection[2] = { 1 , 1 };
	DICONSTANTFORCE cf;
	DIEFFECT        eff{};

	eff.dwSize = sizeof(DIEFFECT);
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.dwDuration = INFINITE;
	eff.dwSamplePeriod = 0;
	eff.dwGain = DI_FFNOMINALMAX;
	eff.dwTriggerButton = DIEB_NOTRIGGER;
	eff.dwTriggerRepeatInterval = 0;
	eff.cAxes = m_dwNumForceFeedbackAxis;
	eff.rgdwAxes = rgdwAxes;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;

	if (FAILED(lpDevice->CreateEffect(GUID_ConstantForce, &eff, &pEffect, NULL)))return false;

	return true;
}

void tdnInputManager::SetConfig(const KEY_CONFIG_DATA &tagNewConfigData, int no)
{
	// 上書き
	memcpy_s(&m_tagConfigDatas[no].tagPadSet, sizeof(PADSET), &tagNewConfigData.tagPadSet, sizeof(PADSET));
	tdnInput::PadAsign(tagNewConfigData.tagPadSet, no);

	std::ofstream ofs("DATA/Input/key_config.txt");

	for (auto it : m_vConfigList)
	{
		if (strcmp(it.tszPadName, tagNewConfigData.tszPadName) == 0)
		{
			// 上書き
			memcpy_s(&it.tagPadSet, sizeof(PADSET), &tagNewConfigData.tagPadSet, sizeof(PADSET));
		}

		// controllerの名前
		ofs << "NAME: " << it.tszPadName << "\n";

		// コンフィグ情報
		int out;
		out = it.tagPadSet.A;
		ofs << out << " ";
		out = it.tagPadSet.B;
		ofs << out << " ";
		out = it.tagPadSet.C;
		ofs << out << " ";
		out = it.tagPadSet.D;
		ofs << out << " ";
		out = it.tagPadSet.L1;
		ofs << out << " ";
		out = it.tagPadSet.L2;
		ofs << out << " ";
		out = it.tagPadSet.L3;
		ofs << out << " ";
		out = it.tagPadSet.R1;
		ofs << out << " ";
		out = it.tagPadSet.R2;
		ofs << out << " ";
		out = it.tagPadSet.R3;
		ofs << out << " ";
		out = it.tagPadSet.START;
		ofs << out << " ";
		out = it.tagPadSet.SELECT;
		ofs << out << "\n\n";
	}
}

//*****************************************************************************************************************************
//				入力デバイス
//*****************************************************************************************************************************
const int tdnInputDevice::default_key_map[20] = 
{
	'W',		// ↑
	'S',		// ↓
	'A',		// ←
	'D',		// →
	'Z',		// KEY_A
	'X',		// KEY_B
	'C',		// KEY_C
	'V',		// KEY_D
	'F',		// KEY_L1
	'G',		// KEY_R1
	'R',		// KEY_L2
	'T',		// KEY_R2
	'4',		// KEY_L3
	'5',		// KEY_R3
	VK_RETURN,	// KEY_START
	VK_SPACE	// KEY_SELECT
};

/* キーボードとジョイスティックのコンフィグが若干違う？キーボード:L1,R1,L2,R2,L3,R3 ジョイ:L1,L2,L3,R1,R2,R3 */
const int tdnInputDevice::default_joy_map[20] = 
{ AXIS_X, AXIS_Y, AXIS_Z, AXIS_RZ,
	2,	// KEY_A
	3,	// KEY_B
	1,	// KEY_C
	4,	// KEY_D
	5,	// KEY_L1
	7,	// KEY_L2
	11,	// KEY_L3
	6,	// KEY_R1
	8,	// KEY_R2
	12,	// KEY_R3
	9,	// KEY_START
	10	// KEY_SELECT
};

//------------------------------------------------------
//	初期化
//------------------------------------------------------
tdnInputDevice::tdnInputDevice(int n) :lpDevice(nullptr), m_pEffect(nullptr)
{
	if (n != -1){
		lpDevice = tdnInputManager::CreateDevice(n);
#ifdef VIBRATION_ON
		if (lpDevice) InitVibration();
#endif
	}

	// デフォルト設定
	memcpy(key_map, default_key_map, sizeof(key_map));
	memcpy(joy_map, default_joy_map, sizeof(joy_map));

	// キー初期化
	pad_axisX = pad_axisY = pad_axisX2 = pad_axisY2 = 0;
	for (int i = 0; i<20; i++) key_info[i] = 0;
}
void tdnInputDevice::InitVibration()
{
	tdnInputManager::CreateEffect(m_pEffect, lpDevice);
}

//------------------------------------------------------
//	解放
//------------------------------------------------------
tdnInputDevice::~tdnInputDevice()
{
	if (lpDevice){
		lpDevice->Unacquire();	// アクセス権解放
		lpDevice->Release();
	}
	if (m_pEffect){
		m_pEffect->Stop();
		m_pEffect->Release();
	}
}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void tdnInputDevice::Update()
{
	// スティック初期化
	pad_axisX = pad_axisX2 = pad_axisY = pad_axisY2 = 0;

	if (lpDevice)
	{
		//------------------------------------------------------
		//	ゲームパッド
		//------------------------------------------------------
		// アクセス権が万が一入ってなかったら
		if (FAILED(lpDevice->Poll())){
			HRESULT hr;
			do { hr = lpDevice->Acquire(); } while (hr == DIERR_INPUTLOST);	// アクセス権の再取得
			lpDevice->Poll();
		}

		// パッド情報構造体
		DIJOYSTATE2 js;

		if (lpDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js) == DI_OK){
			//	軸状態保存
			long	axis[6];
			axis[0] = js.lX;	// スティックx軸
			axis[1] = js.lY;	// スティックy軸
			axis[2] = js.lZ;	// スロットル制御を表す場合が多い。ジョイスティックにこの軸がない場合、値はゼロである。 
			axis[3] = js.lRx;	// x 軸回転。ジョイスティックにこの軸がない場合、値はゼロである
			axis[4] = js.lRy;	// y 軸回転。ジョイスティックにこの軸がない場合、値はゼロである
			axis[5] = js.lRz;	// z 軸回転 (方向舵と呼ばれる)。ジョイスティックにこの軸がない場合、値はゼロである。

			//	軸設定
			pad_axisX = axis[joy_map[0]];	//	左スティックＸ軸
			pad_axisY = axis[joy_map[1]];	//	左スティックＹ軸
			pad_axisX2 = axis[joy_map[2]];	//	右スティックＸ軸
			pad_axisY2 = axis[joy_map[3]];	//	右スティックＹ軸

			//	上下左右キー設定
			u32	angle = 8;
			u8	povkey[9] = { 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x00 };
			//	８方向取得
			if (LOWORD(js.rgdwPOV[0]) != 0xFFFF) angle = js.rgdwPOV[0] / 4500;
			for (int dir = 0; dir < 4; dir++)
			{
				if (povkey[angle] & (0x01 << dir)){
					if (joy_info[dir] & 0x01) joy_info[dir] = 1; else joy_info[dir] = 3;
				}
				else {
					if (joy_info[dir] & 0x01) joy_info[dir] = 2; else joy_info[dir] = 0;
				}
			}

			//	ボタン(0,1,2,3はスティックなので省く)
			for (int i = 4; i < 16; i++){
				if (js.rgbButtons[joy_map[i]] & 0x80){
					if (joy_info[i] & 0x01) joy_info[i] = 1; else joy_info[i] = 3;
				}
				else {
					if (joy_info[i] & 0x01) joy_info[i] = 2; else joy_info[i] = 0;
				}
			}
		}
	}

	//------------------------------------------------------
	//	キーボード
	//------------------------------------------------------
	//	キーによる左スティック設定
	if (KeyBoard(KB_UP))	pad_axisY = -tdnInputEnum::STICK_WIDTH;
	if (KeyBoard(KB_DOWN))	pad_axisY = tdnInputEnum::STICK_WIDTH;
	if (KeyBoard(KB_LEFT))	pad_axisX = -tdnInputEnum::STICK_WIDTH;
	if (KeyBoard(KB_RIGHT))	pad_axisX = tdnInputEnum::STICK_WIDTH;
	//	軸正規化
	int	work = pad_axisX*pad_axisX + pad_axisY*pad_axisY;
	if (work > tdnInputEnum::STICK_WIDTH * tdnInputEnum::STICK_WIDTH)
	{
		work = (int)sqrtf((float)work);
		pad_axisX = pad_axisX * tdnInputEnum::STICK_WIDTH / work;
		pad_axisY = pad_axisY * tdnInputEnum::STICK_WIDTH / work;
	}

	//	キーによる右スティック設定
	if (KeyBoard(KB_NUMPAD8)) pad_axisY2 = -tdnInputEnum::STICK_WIDTH;
	if (KeyBoard(KB_NUMPAD2)) pad_axisY2 = tdnInputEnum::STICK_WIDTH;
	if (KeyBoard(KB_NUMPAD4)) pad_axisX2 = -tdnInputEnum::STICK_WIDTH;
	if (KeyBoard(KB_NUMPAD6)) pad_axisX2 = tdnInputEnum::STICK_WIDTH;
	//	軸正規化
	work = pad_axisX2*pad_axisX2 + pad_axisY2*pad_axisY2;
	if (work > tdnInputEnum::STICK_WIDTH * tdnInputEnum::STICK_WIDTH)
	{
		work = (int)sqrtf((float)work);
		pad_axisX2 = pad_axisX2 * tdnInputEnum::STICK_WIDTH / work;
		pad_axisY2 = pad_axisY2 * tdnInputEnum::STICK_WIDTH / work;
	}

	//	ボタン情報設定
	for (int i = 0; i < 16; i++)
	{
		if (lpDevice && joy_info[i] != 0x00) key_info[i] = joy_info[i];
		else {
			work = (KeyBoard(key_map[i])) ? 1 : 0;
			(key_info[i] & 0x01) ? (key_info[i] = (work) ? 1 : 2) : (key_info[i] = (work) ? 3 : 0);
		}
	}
}

//------------------------------------------------------
//	振動発動
//------------------------------------------------------
void tdnInputDevice::StartVibration()
{
#ifdef VIBRATION_ON
	if (m_pEffect) m_pEffect->Start(0, 1);
#endif
}


//------------------------------------------------------
//	パッド設定
//------------------------------------------------------
void tdnInputDevice::PadAsign(const PADSET &padset)
{
	//	左スティック
	joy_map[0] = padset.lx;
	joy_map[1] = padset.ly;
	//	右スティック
	joy_map[2] = padset.rx;
	joy_map[3] = padset.ry;
	//	メインボタン
	joy_map[KEY_A] = padset.A - 1;
	joy_map[KEY_B] = padset.B - 1;
	joy_map[KEY_C] = padset.C - 1;
	joy_map[KEY_D] = padset.D - 1;
	//	ＬＲキー
	joy_map[KEY_L1] = padset.L1 - 1;
	joy_map[KEY_L2] = padset.L2 - 1;
	joy_map[KEY_L3] = padset.L3 - 1;

	joy_map[KEY_R1] = padset.R1 - 1;
	joy_map[KEY_R2] = padset.R2 - 1;
	joy_map[KEY_R3] = padset.R3 - 1;
	//	スタート・セレクト
	joy_map[KEY_START] = padset.START - 1;
	joy_map[KEY_SELECT] = padset.SELECT - 1;
}

//*****************************************************************************************************************************
//		アクセス関数群
//*****************************************************************************************************************************
tdnInputDevice* tdnInput::device[tdnInputEnum::INPUT_DEVICE_MAX] = { nullptr };

//------------------------------------------------------
//	初期化
//------------------------------------------------------
void tdnInput::Initialize()
{
	// この中でcontrollerの列挙とかコンフィグの読み込みとか
	tdnInputManager::Initialize();

	// デバイス初期化
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++)
	{
		device[i] = new tdnInputDevice(i);
	}

	// 読み込んだキーコンフィグを設定
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++)
	{
		device[i]->PadAsign(tdnInputManager::GetConfigDatas(i)->tagPadSet);
	}

	OKB_Init();
}

//------------------------------------------------------
//	解放
//------------------------------------------------------
void tdnInput::Release()
{
	// デバイス解放
	tdnInputManager::Release();
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++) SAFE_DELETE(device[i]);
	OKB_Release();
}

//------------------------------------------------------
//	リセット
//------------------------------------------------------
void tdnInput::Reset()
{
	// 解放
	tdnInputManager::Release();
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++) SAFE_DELETE(device[i]);

	// この中でcontrollerの列挙とかコンフィグの読み込みとか
	tdnInputManager::Initialize();

	// デバイス初期化
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++)
	{
		device[i] = new tdnInputDevice(i);
	}

	// 読み込んだキーコンフィグを設定
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++)
	{
		device[i]->PadAsign(tdnInputManager::GetConfigDatas(i)->tagPadSet);
	}
}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void tdnInput::Update()
{
	for (int i = 0; i < tdnInputEnum::INPUT_DEVICE_MAX; i++)
	{
		device[i]->Update();
	}
	OKB_Update();
}

//------------------------------------------------------
//	微量の傾きを排除したスティック傾き情報
//------------------------------------------------------

void tdnInput::GetAxisXYf(float *outX, float *outY, int no)
{
	*outX = (float)device[no]->GetAxisX() / tdnInputEnum::STICK_WIDTH, *outY = (float)device[no]->GetAxisY() / tdnInputEnum::STICK_WIDTH;

	//// 絶対値処理(2乗sqrtfとどっちが早いんだろう?)
	//float X = (*outX < 0) ? *outX * -1 : *outX;
	//float Y = (*outY < 0) ? *outY * -1 : *outY;
	//
	//float dist = *outX + *outY;

	//// 誤差排除
	//if (dist < tdnInputEnum::MIN_MOVE_STICK)
	//{
	//	*outX = 0;
	//	*outY = 0;
	//}

	//// 正規化(これをしないと斜め移動が早くなってしまう)
	//if (dist > 1.0f)
	//{
	//	*outX /= dist;
	//	*outY /= dist;
	//}
}
void tdnInput::GetAxisXY2f(float *outX, float *outY, int no)
{
	*outX = (float)device[no]->GetAxisX2() / tdnInputEnum::STICK_WIDTH, *outY = (float)device[no]->GetAxisY2() / tdnInputEnum::STICK_WIDTH;

	//// 絶対値処理(2乗sqrtfとどっちが早いんだろう?)
	//*outX = (AX < 0) ? AX*-1 : AX;
	//*outY = (AY < 0) ? AY*-1 : AY;
	//
	//float dist = *outX + *outY;

	//// 誤差排除
	//if (dist < tdnInputEnum::MIN_MOVE_STICK)
	//{
	//	*outX = 0;
	//	*outY = 0;
	//}

	//// 正規化(これをしないと斜め移動が早くなってしまう)
	//if (dist > 1.0f)
	//{
	//	*outX /= dist;
	//	*outY /= dist;
	//}
}

void tdnInput::Vibration(int no)
{
#ifdef VIBRATION_ON
	device[no]->StartVibration();
#endif
}

//*****************************************************************************************************************************
//		OKB
//*****************************************************************************************************************************
//------------------------------------------------------
//	初期化
//------------------------------------------------------
OwatasoKeyBoard::OwatasoKeyBoard()
{
	// キー配列初期化
	for (int i = 0; i < 256; i++)
		info[i] = on_frame[i] = 0;
}

//------------------------------------------------------
//	更新
//------------------------------------------------------
void OwatasoKeyBoard::Update()
{
	// 取得できたかどうかを確認 & infoにキー入力情報格納
	BOOL ret = GetKeyboardState(info);

	// 取得できなかったら強制終了(毎回止まるようであれば、コメントアウトしてください)
	assert(ret);

	//=========================
	//	全キー

	for (int index = 0; index < 240; index++){	// 240以降、有益なキーはないと思う…

		// 対応キーが押されているなら
		if (info[index] & 0x80)
			on_frame[index]++;		// その番号の入力フレームを加算

		else
			on_frame[index] = 0;	// そうでないならフレーム0にする(押されていない)
	}


	//=========================
	// 基本キーのみ
	//BYTE key_list[] =
	//{
	//	KB_ENTER, KB_SPACE,
	//	KB_0, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9,
	//	KB_A, KB_B, KB_C, KB_D, KB_E, KB_F, KB_G, KB_H, KB_I, KB_J, KB_K, KB_L, KB_M, KB_N, KB_O, KB_P, KB_Q, KB_R, KB_S, KB_T, KB_U, KB_V, KB_W, KB_X, KB_Y, KB_Z,
	//	KB_COMMA, KB_DOT, KB_HYPHEN, KB_COLON, KB_SEMICORON, KB_UP, KB_DOWN, KB_LEFT, KB_RIGHT,
	//	0x00
	//};

	//for (int index = 0; key_list[index] != 0x00; index++){

	//	// 対応キーが押されているなら
	//	if (info[key_list[index]] & 0x80)
	//		on_frame[key_list[index]]++;		// その番号の入力フレームを加算

	//	else
	//		on_frame[key_list[index]] = 0;	// そうでないならフレーム0にする(押されていない)

	//}


}


//------------------------------------------------------
//	どれか押したか
//------------------------------------------------------
BYTE OwatasoKeyBoard::AnyTRG()
{
	//=========================
	//	全キー

	for (int index = 0; index < 240; index++)	// 240以降、有益なキーはないと思う…
	{
		// 何かのキー入力時間が1フレーム目だったらそれを返す
		if (on_frame[info[index]] == 1)
			return index;
	}

	//=========================
	// 基本キーのみ
	//BYTE key_list[] =
	//{
	//	KB_ENTER, KB_SPACE,
	//	KB_0, KB_1, KB_2, KB_3, KB_4, KB_5, KB_6, KB_7, KB_8, KB_9,
	//	KB_A, KB_B, KB_C, KB_D, KB_E, KB_F, KB_G, KB_H, KB_I, KB_J, KB_K, KB_L, KB_M, KB_N, KB_O, KB_P, KB_Q, KB_R, KB_S, KB_T, KB_U, KB_V, KB_W, KB_X, KB_Y, KB_Z,
	//	KB_COMMA, KB_DOT, KB_HYPHEN, KB_COLON, KB_SEMICORON,
	//	0x00
	//};

	//for (int index = 0; key_list[index] != 0x00; index++){

	//	// 何かのキー入力時間が1フレーム目だったらそれを返す
	//	if (on_frame[key_list[index]] == 1)
	//		return key_list[index];
	//}

	// このフレームで新しく押されたキーはなかったので0(キーコードなし)を返す
	return 0x00;

}


//*****************************************************************************************************************************
//	非クラス領域
//*****************************************************************************************************************************
static OwatasoKeyBoard *key_board(nullptr);

//------------------------------------------------------
//	初期化
//------------------------------------------------------
void OKB_Init()
{
	if(!key_board)key_board = new OwatasoKeyBoard;
}

//------------------------------------------------------
//	解放
//------------------------------------------------------
void OKB_Release()
{
	if (key_board){
		delete key_board;
		key_board = nullptr;
	}
}


//------------------------------------------------------
//	更新
//------------------------------------------------------

void OKB_Update()
{
	key_board->Update();
}


//*****************************************************************************************************************************
//		キーアクセス関数
//*****************************************************************************************************************************

UINT KeyBoard(BYTE KeyCode){ return key_board->on_frame[KeyCode]; }
bool KeyBoardTRG(BYTE KeyCode, UINT frame) { return (key_board->on_frame[KeyCode] == frame); }

BYTE KeyBoardAnyTRG(){ return key_board->AnyTRG(); }















//*****************************************************************************************************************************
//		マウス
//*****************************************************************************************************************************
//------------------------------------------------------
//	静的変数
//------------------------------------------------------
Vector2 tdnMouse::m_Axis = Vector2(0, 0);
Vector2 tdnMouse::m_Pos = Vector2(0, 0);
int tdnMouse::m_FlagRight = 0;
int tdnMouse::m_FlagLeft = 0;
POINT tdnMouse::m_CurrentPoint;
POINT tdnMouse::m_PrevPoint;
RECT tdnMouse::m_Rc;
int tdnMouse::m_PrevWheel;
int tdnMouse::m_CurrentWheel;
WHEEL_FLAG tdnMouse::m_FlagW = WHEEL_FLAG::NONE;

//------------------------------------------------------
//	初期化
//------------------------------------------------------
void tdnMouse::Initialize(BOOL show)
{
	ShowCursor(show);
}


//------------------------------------------------------
//	更新
//------------------------------------------------------
void tdnMouse::Update()
{
	if (m_CurrentWheel == m_PrevWheel) m_FlagW = WHEEL_FLAG::NONE;
	else
	{
		m_FlagW = (m_CurrentWheel < m_PrevWheel) ? WHEEL_FLAG::DOWN : WHEEL_FLAG::UP;
		m_PrevWheel = m_CurrentWheel;
	}

	static const float Max = 0.6f;
	static const float Min = 0.25f;

	// 前回座標保存
	m_PrevPoint = m_CurrentPoint;
	//ShowCursor(TRUE);			// 勝手にファルスに

	// 取得
	GetCursorPos(&m_CurrentPoint);

	GetWindowRect(tdnSystem::GetWindow(), &m_Rc);
	// 窓位置と縁による調整
	m_Pos.x = (float)(m_CurrentPoint.x - m_Rc.left - 8);
	m_Pos.y = (float)(m_CurrentPoint.y - m_Rc.top - 29);
	////中央オフセット＆正規化
	m_Axis.x = ((float)m_Pos.x - (tdnSystem::GetScreenSize().right / 2)) / (tdnSystem::GetScreenSize().right / 2);
	m_Axis.y = -((float)m_Pos.y - (tdnSystem::GetScreenSize().bottom / 2)) / (tdnSystem::GetScreenSize().bottom / 2);

	// 最大値制御
	if (m_Axis.x > Max)m_Axis.x = Max;
	else if (m_Axis.x < -Max)m_Axis.x = -Max;
	if (m_Axis.y > Max)m_Axis.y = Max;
	else if (m_Axis.y < -Max)m_Axis.y = -Max;

	// 最小値なら0
	float val_x = sqrtf(m_Axis.x * m_Axis.x), val_y = sqrtf(m_Axis.y * m_Axis.y);
	if (val_x < Min) m_Axis.x = 0;
	if (val_y < Min * 1.5f) m_Axis.y = 0;

	// 左クリック
	BOOL work = (KeyBoard(MOUSE_LEFT)) ? 1 : 0;
	(m_FlagLeft & 0x01) ? (m_FlagLeft = (work) ? 1 : 2) : (m_FlagLeft = (work) ? 3 : 0);

	// 右クリック
	work = (KeyBoard(MOUSE_RIGHT)) ? 1 : 0;
	(m_FlagRight & 0x01) ? (m_FlagRight = (work) ? 1 : 2) : (m_FlagRight = (work) ? 3 : 0);

}

void tdnMouse::SetExclusion(bool exclusion)
{
	//	一時的にデバイスへのアクセス権を解放
	tdnInputManager::GetMouseDevice()->Unacquire();

	//	排他
	if (exclusion) tdnInputManager::GetMouseDevice()->SetCooperativeLevel(tdnSystem::GetWindow(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	//	非排他
	else tdnInputManager::GetMouseDevice()->SetCooperativeLevel(tdnSystem::GetWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	//	アクセス権を再取得
	tdnInputManager::GetMouseDevice()->Acquire();
}