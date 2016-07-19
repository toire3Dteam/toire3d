#include "TDNLIB.h"


/************************/
//		tdnFont
/************************/

/****** 前方宣言 *******/

//	管理用パラメータ									
bool tdnFont::m_IsInitialized = false;
tdnFont::CacheDesc		tdnFont::m_CacheDesc = { 0 }; //キャッシュの色々

/***********************/


/***********************************************/
//		初期化・解放
/***********************************************/
void	tdnFont::Initialize()
{
	//	通常文字キャッシュの初期化
	{
		//	すでに確保されている領域がある場合は破棄
		SAFE_DELETE_ARRAY(m_CacheDesc.textureCacheList);	//	文字テクスチャのリスト
		SAFE_DELETE_ARRAY(m_CacheDesc.referenceDataList);	//	キャッシュの照合用データリスト

															//	キャッシュを保存する領域を確保
		m_CacheDesc.textureCacheList = new tdn2DObj[CacheQty];  // 文字テクスチャのリスト
		m_CacheDesc.referenceDataList = new	RefData[CacheQty];	// キャッシュの照合用データのリスト

																//	キャッシュ分初期化
		for (UINT i = 0; i < CacheQty; i++)
		{
			m_CacheDesc.referenceDataList[i].isEnable = false;// 有効フラグをfalse
		}

		m_CacheDesc.nextUseCacheNum = 0;//	次に使用するキャッシュの番号
	}

	//	初期化フラグを立てる
	m_IsInitialized = true;

}

void	tdnFont::Release()
{
	//	通常キャッシュ
	{
		SAFE_DELETE_ARRAY(m_CacheDesc.textureCacheList);	//	文字テクスチャのリスト
		SAFE_DELETE_ARRAY(m_CacheDesc.referenceDataList);	//	キャッシュの照合用データリスト
		m_CacheDesc.nextUseCacheNum = 0;
	}

	//	初期化フラグをリセット
	m_IsInitialized = false;
}

/***********************************************/
//		文字描画
/***********************************************/

//文字列描画
void	tdnFont::RenderString(LPCSTR string, LPCSTR fontName, int fontSize, int drawX, int  drawY, DWORD color, DWORD RenderFlag)
{
	//	変数の用意
	int	x = drawX;
	int	y = drawY;
	UINT	myByte = 0;
	//	終端文字までループ
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&string[i]);

		//	文字を描画
		int	drawCoordX = x;
		int	drawCoordY = y;
		Vector2	drawedSize = RenderCharacter(&string[i], fontName, fontSize, drawCoordX, drawCoordY, color, RenderFlag);

		//	描画座標をずらす
		x += drawedSize.x;
	}
}


// 一文字描画
Vector2	tdnFont::RenderCharacter(LPCSTR character, LPCSTR fontName, int fontSize, int drawX, int  drawY, DWORD color, DWORD RenderFlag)
{
	//	文字テクスチャ管理パラメータが初期化されているかチェック
	if (!m_IsInitialized)	Initialize();

	//	変数の用意
	tdn2DObj*	pImage;
	UINT	createSize;
	UINT	byteSize = _mbclen((BYTE*)character);

	//	幅と高さを比較して大きいほうを基準にテクスチャを作成
	int		createSizeW = (int)((float)fontSize);
	int		createSizeH = (int)((float)fontSize);
	if (createSizeW > createSizeH)	createSize = createSizeW;
	else							createSize = createSizeH;

	//	使用するテクスチャのアドレス[配列番号]をセット
	pImage = &m_CacheDesc.textureCacheList[SearchCache(character, createSize, fontName)];

	//	空白チェック
	bool	draw(true);
	
	switch (byteSize) 
	{
	case	1:
		if (character[0] == ' ') {	draw = false;	}
		break;
	case	2:
	{
		char	buffer[3] = { character[0], character[1], '\0' };
		if (strcmp(buffer, "　") == 0) {
			draw = false;
		}
	}
	break;
	}

	//	描画
	if (draw)
	{
		pImage->SetARGB(color);
		pImage->Render(drawX, drawY, pImage->GetWidth(), pImage->GetHeight(),
			0, 0, pImage->GetWidth(), pImage->GetHeight(), RenderFlag);
	}

	//	描画した文字のサイズを返す
	return	Vector2((float)pImage->GetWidth(), (float)pImage->GetHeight());
}


//  簡易
void tdnFont::RenderFont2D(LPCSTR _String, int _FontSize, float _DrawX, float _DrawY, DWORD col)
{
	LPSTR	c_FontName = "HGS創英角ﾎﾟｯﾌﾟ体";

	RenderString(_String, c_FontName, _FontSize,
		_DrawX, _DrawY, col, RS::COPY);
}

/****************************************/
//	 2DObjから文字を作るサポート関数
/****************************************/
UINT	tdnFont::SearchCache(LPCSTR chara, UINT size, LPCSTR fontName)
{
	//	使用可能なキャッシュを探す
	UINT	sizeAllow = 0;							//	許容サイズ差
	UINT	byteSize = _mbclen((BYTE*)chara);	//	バイト数
												//	キャッシュ数分ループ
	for (UINT cNum = 0; cNum < CacheQty; cNum++) {
		RefData&	refData = m_CacheDesc.referenceDataList[cNum];
		if (!refData.isEnable)									continue;
		//	バイト数判定
		if (refData.byte != byteSize)							continue;
		//	文字判定（バイト数に応じて分岐）
		if (byteSize != 2) {
			if (refData.chara[0] != chara[0])					continue;
		}
		else {
			if (refData.chara[0] != chara[0])					continue;
			if (refData.chara[1] != chara[1])					continue;
		}
		//	サイズ判定
		if (refData.size - sizeAllow > size)					continue;
		if (refData.size + sizeAllow < size)					continue;
		//	フォント判定
		if (strlen(refData.fontName) == 0) {
			if (strlen(fontName) != 0)						continue;
		}
		else {
			if (strlen(fontName) == 0)						continue;
			if (strcmp(refData.fontName, fontName) != 0)	continue;
		}
		//	判定をクリア
		return	cNum;
	}

	//	新しく文字テクスチャを作成
	UINT	useNum = m_CacheDesc.nextUseCacheNum;
	m_CacheDesc.textureCacheList[useNum].LoadFontTexture(chara, size, fontName);
	//	照合用パラメータを更新
	RefData&	refData = m_CacheDesc.referenceDataList[useNum];
	refData.chara[0] = chara[0];
	refData.chara[1] = chara[1];
	refData.size = size;
	refData.byte = byteSize;
	refData.isEnable = true;
	refData.fontName = fontName;

	//	次に書き換えるキャッシュの番号を更新
	if (++m_CacheDesc.nextUseCacheNum == CacheQty)	m_CacheDesc.nextUseCacheNum = 0;

	//	新しくテクスチャを作成した番号を返す
	return	useNum;
}
