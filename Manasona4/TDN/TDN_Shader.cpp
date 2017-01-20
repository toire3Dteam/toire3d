#include	"TDNLIB.h"

/********************************************/
//	tdnShader	
/********************************************/

// (∵)後で名前と TransMat N S H などを　GetParameterByNameで指定して置く

// ID3DXEffect
// https://msdn.microsoft.com/ja-jp/library/cc372622.aspx

/**
*	■D3DXCreateEffectFromFile
*	1.デバイス　2.ファイル名のポインタ　
*   3.プリプロセッサマクロの定義　4.オプションインターフェイスポインタ　
*	5.Flag(コンパイルオプション)　6.Poolの引数、NULLにすると共有されない
*　 7.コンパイルされたエフェクトが格納されたバッファへのポインタを返す　8.コンパイルエラー		
*	
**/

/* 初期化・解放 */
tdnShader::tdnShader(char* filename, bool iscfx)
{
	LPDEVICE lpDevice = tdnSystem::GetDevice();

	// ファイル名チェック
	char fileName[256];
	if (iscfx)
	{
		sprintf(fileName, "%s", filename);
	}
	else
	{
		if (filename[strlen(filename) - 3] != '.') sprintf(fileName, "%s.fx", filename);
		else sprintf(fileName, "%s", filename);
	}



	// シェーダーの読み込み
	HRESULT	hr;					// エラー報告
	LPD3DXBUFFER pErr = NULL;	// コンパイルエラーの詳細を格納
	
	// コンパイル済みのファイルを読み込む？
	if (iscfx)
	{
		hr = D3DXCreateEffectFromFile(lpDevice, fileName, NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &pShader, &pErr);
	}
	else
	{
		hr = D3DXCreateEffectFromFile(lpDevice, fileName, NULL, NULL, 0, NULL, &pShader, &pErr);

	}

	if (FAILED(hr))
	{
		// ファイル名を間違えていた場合、pErrはNULLになってしまい、アクセス違反が発生してしまうので、対策
		MyAssert(pErr, "エラー: シェーダーファイル名を間違えています\nエラーファイル名%s", filename);

		// 出力にエラー表示
		char*	szBuffer = new char[pErr->GetBufferSize() + 32 ];
		sprintf(szBuffer, "\terrors: %s\n", (char*)pErr->GetBufferPointer());
		OutputDebugString(szBuffer);
		delete[] szBuffer;
		return;
	}

	// 基礎変数の読み込み
	hmWVP = pShader->GetParameterByName(NULL, "WVPMatrix");	//親構造体パラメータのハンドルNULLで最上階　パラメータ名 
	htexDecale = pShader->GetParameterByName(NULL, "Texture");
}

tdnShader::~tdnShader()
{
	pShader->Release();
}


/*********************/
// パラメーター設定
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

void tdnShader::SetMaskScreen(tdn2DObj* maskScreen)
{
	tdnShader::SetValue("MaskTex", maskScreen);
	//pShader->SetValue("MaskTex", maskScreen->get);
}


