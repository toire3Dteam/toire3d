#include "Effect2DManager.h"

/******************
*	Effect2DManager
*******************/

Effect2DManager* Effect2DManager::m_pInstance = nullptr;

/*******************************/
// インスタンス
/*******************************/
Effect2DManager& Effect2DManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Effect2DManager();
	}
	return *m_pInstance;
}

void Effect2DManager::Release()
{
	SAFE_DELETE(m_pInstance);
}

/*********************************/
//			初期化・解放
/*********************************/
Effect2DManager::Effect2DManager()
{
	m_iEffectFrame = 0;
	m_iMaxEffectFrame = 30;
	m_iSelectNodeNo = 0;

	// スクリーンのサイズ
	m_iScreenWidth = 256;
	m_iScreenHeight = 256;
	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};

	AddNode();
}

Effect2DManager::~Effect2DManager()
{
	for (int i = 0; i < (int)m_pEffectNode.size(); i++)
	{
		SAFE_DELETE(m_pEffectNode[i]);
	}
	
	//　データを空に
	m_pEffectNode.clear();

	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);
}

/*********************************/
//			更新・描画
/*********************************/
void Effect2DManager::Update()
{
	// フレーム更新
	CalcFrame();

	CalcSelectNo();
	
	// ノードの数ループ
	for (int i = 0; i < (int)m_pEffectNode.size(); i++)
	{
		m_pEffectNode[i]->Update(m_iEffectFrame);
	}
}

void Effect2DManager::Render()
{	
	//+----------------------------------------------------------
	// まずは今のサーフェイス・ビュー・デプスステンシルを保存
	//+----------------------------------------------------------
	Surface* saveSurface;
	D3DVIEWPORT9 saveViewPort;
	Surface* saveStencilSurface;
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	tdnSystem::GetDevice()->GetViewport(&saveViewPort);
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveStencilSurface);


	//+-----------------------------------------------------
	// サーフェイス・ビュー・デプスステンシルを切り替え
	//+-----------------------------------------------------
	D3DVIEWPORT9 vp = { 0, 0, m_iScreenWidth, m_iScreenHeight, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	m_pScreen->RenderTarget();
	{
		//　画面クリア
		tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		// ノードの数ループ
		for (int i = 0; i < (int)m_pEffectNode.size(); i++)
		{
			// 画面の真ん中を原点として描画
			m_pEffectNode[i]->Render((m_iScreenWidth / 2), (m_iScreenHeight/ 2));

		}

	}
	

		//+-----------------------------------------------------
		// サーフェイス・ビュー・デプスステンシルを復元
		//+-----------------------------------------------------
		tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//レンダーターゲットの復元		
		tdnSystem::GetDevice()->SetViewport(&saveViewPort);//ビューポートの復元
		tdnSystem::GetDevice()->SetDepthStencilSurface(saveStencilSurface);	//ステンシルバッファの復元														


		// エフェクトスクリーン描画
		m_pScreen->Render(0, 0);

		//tdnText::Draw(400, 20, 0xffffffff, "ゲームフレーム_%d", m_iEffectFrame);
		//tdnText::Draw(400, 50, 0xffffffff, "セレクトNo_%d", m_iSelectNodeNo);
		//for (int i = 0; i < (int)m_pEffectNode.size(); i++)
		//{
		//	std::string text;
		//	text = "名前：";
		//	text += m_pEffectNode[i]->m_sName;

		//	if (m_iSelectNodeNo == i)
		//		tdnText::Draw(800, 20 + i * 32, 0xffff00ff, text.c_str());
		//	else
		//		tdnText::Draw(800, 20 + i * 32, 0xffffffff, text.c_str());
		//}

}

void Effect2DManager::CalcFrame()
{
	// フレーム更新
	if (++m_iEffectFrame >= m_iMaxEffectFrame)m_iEffectFrame = 0;	

}

void Effect2DManager::CalcSelectNo()
{
	// 一周回転
	if (m_iSelectNodeNo>= (int)m_pEffectNode.size())
	{
		m_iSelectNodeNo = 0;
	}
	if (m_iSelectNodeNo <= -1)
	{
		m_iSelectNodeNo = (int)m_pEffectNode.size() - 1;
	}


}

// エフェクト追加
void Effect2DManager::AddNode()
{
	Effect2D* data;
	data = new  Effect2D();

	// 配列に加える
	m_pEffectNode.push_back(data);

}

//	ノード削除
void Effect2DManager::EraseNode(int no)
{
	// エラー処理
	if (no >= (int)m_pEffectNode.size())
	{
		//  DATAがないので消せない
		MyAssert(0,"いねえよ");
		return;
	}

	// まずはnewしたデータを消す
	SAFE_DELETE(m_pEffectNode[no]);

	// リストから削除
	m_pEffectNode.erase(m_pEffectNode.begin() + no);

}
void Effect2DManager::EraseNode()
{
	// まずはnewしたデータを消す
	SAFE_DELETE(m_pEffectNode[m_iSelectNodeNo]);

	// リストから削除
	m_pEffectNode.erase(m_pEffectNode.begin() + m_iSelectNodeNo);

}

bool Effect2DManager::Save(const char* fileName)
{
	m_saveData.open(fileName, std::ios::binary);
	if (m_saveData.fail())
	{
		return false;
	}

	if (!m_saveData.is_open())
	{
		return false;
	}

	// 画面サイズを取得
	m_saveData.write((char*)&m_iScreenWidth, sizeof(int)); 
	m_saveData.write((char*)&m_iScreenHeight, sizeof(int)); 

	int MAX_SIZE= (int)m_pEffectNode.size();// 最大
	m_saveData.write((char*)&MAX_SIZE, sizeof(int)); // 

	// エフェクトノードデータ
	for (int i = 0; i < MAX_SIZE; i++)
	{

		// 必要なノードのデータ
		m_saveData.write((char*)&m_pEffectNode[i]->m_iFrame, sizeof(int));
		m_saveData.write((char*)&m_pEffectNode[i]->m_vPos, sizeof(Vector2));
		m_saveData.write((char*)&m_pEffectNode[i]->m_fScale, sizeof(float));
		m_saveData.write((char*)&m_pEffectNode[i]->m_fAngle, sizeof(float));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sCol, sizeof(COLOR_INT));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sStart, sizeof(EffecrParam));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sMiddle, sizeof(EffecrParam));
		m_saveData.write((char*)&m_pEffectNode[i]->m_sEnd, sizeof(EffecrParam));
		m_saveData.write((char*)&m_pEffectNode[i]->m_bMiddleFlag, sizeof(bool));
		m_saveData.write((char*)&m_pEffectNode[i]->m_bRenderFlag, sizeof(bool));

		m_saveData.write(m_pEffectNode[i]->m_sName, sizeof(m_pEffectNode[i]->m_sName)); // 名前
																							
		m_saveData.write(m_pEffectNode[i]->m_sListName, sizeof(m_pEffectNode[i]->m_sListName)); // リストネーム

	}


	m_saveData.close();

	return true;
}

bool Effect2DManager::Load(const char* fileName)
{
	// 一度全てのノードをクリア
	for (int i = 0; i < (int)m_pEffectNode.size(); i++)
	{
		SAFE_DELETE(m_pEffectNode[i]);
	}

	m_iEffectFrame = 0;

	//　データを空に
	m_pEffectNode.clear();


	// アーカイブファイルを読み込む
	std::ifstream LoadData; // input file stream
	LoadData.open(fileName, std::ios::binary);
	if (LoadData.fail())
	{
		return false;
	}


	// 画面サイズを取得
	int sizeX = 0, sizeY = 0;
	LoadData.read((char*)&sizeX,	sizeof(int));
	LoadData.read((char*)&sizeY,	sizeof(int));
	ChangeScreenSize(sizeX, sizeY);

	int MAX_SIZE = 0;// 最大数
	LoadData.read((char*)&MAX_SIZE, sizeof(int));// 最大数取得

	for (int i = 0; i < MAX_SIZE; i++)
	{
		Effect2D* data;
		data = new  Effect2D();

		// 必要なノードのデータ
		LoadData.read((char*)&data->m_iFrame,	sizeof(int));
		LoadData.read((char*)&data->m_vPos,		sizeof(Vector2));
		LoadData.read((char*)&data->m_fScale,	sizeof(float));
		LoadData.read((char*)&data->m_fAngle,	sizeof(float));
		LoadData.read((char*)&data->m_sCol,		sizeof(COLOR_INT));
		LoadData.read((char*)&data->m_sStart,	sizeof(EffecrParam));
		LoadData.read((char*)&data->m_sMiddle,	sizeof(EffecrParam));
		LoadData.read((char*)&data->m_sEnd,		sizeof(EffecrParam));
		LoadData.read((char*)&data->m_bMiddleFlag, sizeof(bool));
		LoadData.read((char*)&data->m_bRenderFlag, sizeof(bool));

		LoadData.read(data->m_sName, sizeof(data->m_sName)); // 名前

		LoadData.read(data->m_sListName, sizeof(data->m_sListName)); // リスト名前

		if (strcmp(data->m_sName,"") != 0)
		{
			data->LoadTexture(data->m_sName);
		}
		

		// 配列に加える
		m_pEffectNode.push_back(data);

	}


	return true;
}

void Effect2DManager::ChangeScreenSize(const int SizeX, const int SizeY)
{	
	// 一度消去
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);

	m_iScreenWidth = SizeX;
	m_iScreenHeight = SizeY;

	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};

}

void Effect2DManager::ChangeScreenSize256()
{
	// 一度消去
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);

	m_iScreenWidth = 256;
	m_iScreenHeight = 256;

	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};

}
void Effect2DManager::ChangeScreenSize512()
{
	// 一度消去
	SAFE_DELETE(m_pScreen);
	SAFE_RELEASE(m_pStencilSurface);

	m_iScreenWidth = 512;
	m_iScreenHeight = 512;

	m_pScreen = new tdn2DObj(m_iScreenWidth, m_iScreenHeight, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(m_iScreenWidth, m_iScreenHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};
}


bool  Effect2DManager::OutputPng( char* fileName, const int interval)
{
	int roopNum = m_iMaxEffectFrame / interval;

	//+-----------------------------------------------------
	// まずは今のサーフェイス・ビュー・デプスステンシルを保存
	//+-----------------------------------------------------
	Surface* saveSurface;
	D3DVIEWPORT9 saveViewPort;
	Surface* saveStencilSurface;
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	tdnSystem::GetDevice()->GetViewport(&saveViewPort);
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveStencilSurface);


	//+-----------------------------------------------------
	// サーフェイス・ビュー・デプスステンシルを切り替え
	//+-----------------------------------------------------
	D3DVIEWPORT9 vp = { 0, 0, m_iScreenWidth, m_iScreenHeight, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	m_pScreen->RenderTarget();
	{
	
		for (int i = 0; i < roopNum; i++)
		{
			//　画面クリア
			tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

			std::string text;
			text = fileName;
			char*	ext = &fileName[lstrlen(fileName) - 4];
			if (lstrcmpi(ext, ".png") == 0)
			{
				// .pngを消す
				text.pop_back();
				text.pop_back();
				text.pop_back();
				text.pop_back();
			}

			text += std::to_string(i);
			text += ".png";		

			int len = text.length();
			char *calcFileName;
			calcFileName = new char[len + 1];
			memcpy(calcFileName, text.c_str(), len + 1);


			// 何コマ目を描画するか
			int frameNum = i*interval;

			// ノードの数ループ
			for (int i = 0; i < (int)m_pEffectNode.size(); i++)
			{
				m_pEffectNode[i]->Update(frameNum);
				m_pEffectNode[i]->Render((m_iScreenWidth / 2), (m_iScreenHeight / 2));
			}

			D3DXSaveTextureToFile(calcFileName, D3DXIFF_PNG, m_pScreen->GetTexture(), NULL);

			delete calcFileName;
		}

	}


	//+----------------------------------------------------------
	// サーフェイス・ビュー・デプスステンシルを復元
	//+----------------------------------------------------------
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//レンダーターゲットの復元		
	tdnSystem::GetDevice()->SetViewport(&saveViewPort);//ビューポートの復元
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveStencilSurface);	//ステンシルバッファの復元														



	return true;
}



bool  Effect2DManager::OutputPngLine( char* fileName, const int interval, int WidthFrameNum)
{

	int roopNum = (int)ceil((float)m_iMaxEffectFrame / (float)interval);// 小数点切り上げ

	// 横の長さ
	int calcWidth = WidthFrameNum;
	if (WidthFrameNum >= roopNum)
	{
		calcWidth = roopNum;
	}
	// 縦の長さ
	int calcHeight = (int)ceil((float)roopNum / (float)calcWidth);
	
	// テクスチャのサイズを取得
	int lineScreenSizeX, lineScreenSizeY;	
	lineScreenSizeX = m_iScreenWidth*calcWidth;
	lineScreenSizeY = m_iScreenHeight*calcHeight;
 
	// テクスチャ作成
	m_pLineScreen = new tdn2DObj(lineScreenSizeX, lineScreenSizeY, TDN2D::USEALPHA);
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface
		(lineScreenSizeX, lineScreenSizeY, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pLineStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};


	//+----------------------------------------------------------
	// まずは今のサーフェイス・ビュー・デプスステンシルを保存
	//+----------------------------------------------------------
	Surface* saveSurface;
	D3DVIEWPORT9 saveViewPort;
	Surface* saveStencilSurface;
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	tdnSystem::GetDevice()->GetViewport(&saveViewPort);
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveStencilSurface);


	//+----------------------------------------------------------
	// サーフェイス・ビュー・デプスステンシルを切り替え
	//+----------------------------------------------------------
	D3DVIEWPORT9 vp = { 0, 0, lineScreenSizeX, lineScreenSizeY, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pLineStencilSurface);
	m_pLineScreen->RenderTarget();
	//　画面クリア 今回は一回のみ
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	
	{

		for (int i = 0; i < roopNum; i++)
		{
			// 何コマ目を描画するか
			int frameNum = i*interval;

			int x = i % calcWidth;//  
			int y = i / calcWidth;//  

			// ノードの数ループ
			for (int i = 0; i < (int)m_pEffectNode.size(); i++)
			{
				m_pEffectNode[i]->Update(frameNum);
				m_pEffectNode[i]->Render((m_iScreenWidth / 2) +(x * m_iScreenWidth),
					(m_iScreenHeight / 2) + (y * m_iScreenHeight));
			}
			
		}

	}


	std::string text;
	text = fileName;
	
	char*	ext = &fileName[lstrlen(fileName) - 4];
	if (lstrcmpi(ext, ".png") != 0) { text += ".png"; }
	
	int textLen = text.length();
	char *calcFileName;
	calcFileName = new char[textLen + 1];
	memcpy(calcFileName, text.c_str(), textLen + 1);
	
	// Png画像
	D3DXSaveTextureToFile(calcFileName, D3DXIFF_PNG, m_pLineScreen->GetTexture(), NULL);

	delete calcFileName;

	//+----------------------------------------------------------
	// サーフェイス・ビュー・デプスステンシルを復元
	//+----------------------------------------------------------
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//レンダーターゲットの復元		
	tdnSystem::GetDevice()->SetViewport(&saveViewPort);//ビューポートの復元
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveStencilSurface);	//ステンシルバッファの復元														


	// 消去
	SAFE_DELETE(m_pLineScreen);
	SAFE_RELEASE(m_pLineStencilSurface);

	return true;
}