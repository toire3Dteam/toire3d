#include	"PointLightInstancingMesh.h"
#include	"system/system.h"

//+-----------------------------------------------------
//	PL用インスタンシングメッシュ
//+-----------------------------------------------------

PointLightInstancingMesh::PointLightInstancingMesh(char* filename, int iMaxInstancingNum) :iexMesh(filename)
{
	m_iMaxInstancingNum = iMaxInstancingNum;
	m_tagInstancingData = new InstancingData[m_iMaxInstancingNum];
	//m_pWMatrix = new Matrix[m_iMaxInstancingNum];
	m_pPLSData = new PLSINSTDATA[m_iMaxInstancingNum];

	// 行列初期化
	for (int i = 0; i < m_iMaxInstancingNum; i++)
	{
		m_tagInstancingData[i].vPos = VECTOR_ZERO;
		m_tagInstancingData[i].fScale = 1.0f;

		D3DXMatrixIdentity(&m_pPLSData[i].wmat);
		m_pPLSData[i].wmat._11 = m_tagInstancingData[i].fScale;
		m_pPLSData[i].wmat._22 = m_tagInstancingData[i].fScale;
		m_pPLSData[i].wmat._33 = m_tagInstancingData[i].fScale;
		m_pPLSData[i].wmat._41 = m_tagInstancingData[i].vPos.x;
		m_pPLSData[i].wmat._42 = m_tagInstancingData[i].vPos.y;
		m_pPLSData[i].wmat._43 = m_tagInstancingData[i].vPos.z;

		//m_pPLSData[i].a = 1.0f;
		m_pPLSData[i].r = 1.0f;
		m_pPLSData[i].g = 1.0f;
		m_pPLSData[i].b = 1.0f;
		m_pPLSData[i].power = 1.0f;

		m_pPLSData[i].x = 0.0f;
		m_pPLSData[i].y = 0.0f;
		m_pPLSData[i].z = 0.0f;
		m_pPLSData[i].range = 1.0f;

	}


	lpMesh->GetVertexBuffer(&m_lpVertexBuf);
	lpMesh->GetIndexBuffer(&m_lpIndexBuf);

	// 頂点宣言作成
	D3DVERTEXELEMENT9 declElems[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },//	LPos
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 }, //		法線  
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },	//	 カラー
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	//	 UV
			{ 1,  sizeof(float) * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	//	 UV
			{ 1,  sizeof(float) * 4, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	//	 UV
			{ 1,  sizeof(float) * 8, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	//	 UV
			{ 1,  sizeof(float) * 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	//	 UV
			
			{ 1,  sizeof(float) * 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },	//	 EXT
			{ 1,  sizeof(float) * 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6 },	//	 EXT
			//{ 1,  sizeof(float) * 23, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 7 },	//	 EXT

		D3DDECL_END()
	};
	//D3DVERTEXELEMENT9 declInst[] = {
	//	{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	//	 UV
	//	{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	//	 UV
	//	{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	//	 UV
	//	{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	//	 UV
	//	D3DDECL_END()
	//};
	m_pDecl = 0;
	tdnSystem::GetDevice()->CreateVertexDeclaration(declElems, &m_pDecl);
	tdnSystem::GetDevice()->CreateVertexBuffer(sizeof(PLSINSTDATA)*m_iMaxInstancingNum, 0, 0, D3DPOOL_MANAGED, &m_pTransMatrixBuf, 0);
	m_iNumIndices  = lpMesh->GetNumFaces();
	m_iNumVertices = lpMesh->GetNumVertices();


}

PointLightInstancingMesh::~PointLightInstancingMesh()
{
	m_lpVertexBuf->Release();
	m_lpIndexBuf->Release();
	m_pDecl->Release();

	SAFE_DELETE(m_tagInstancingData);
	//SAFE_DELETE(m_pWMatrix);
	SAFE_DELETE(m_pPLSData);


}
//void PointLightInstancingMesh::Update(Matrix* mat, int numMat)
//{
//	void* pBuf;
//	m_pTransMatrixBuf->Lock(0,0,&pBuf,0);
//	memcpy(pBuf,mat,sizeof(PLSINSTDATA)*numMat);
//	m_pTransMatrixBuf->Unlock();
//}

void PointLightInstancingMesh::InstancingUpdate()
{
	// 行列生成
	for (int i = 0; i < m_iMaxInstancingNum; i++)
	{
		D3DXMatrixIdentity(&m_pPLSData[i].wmat);
		m_pPLSData[i].wmat._11 = m_tagInstancingData[i].fScale;
		m_pPLSData[i].wmat._22 = m_tagInstancingData[i].fScale;
		m_pPLSData[i].wmat._33 = m_tagInstancingData[i].fScale;
		m_pPLSData[i].wmat._41 = m_tagInstancingData[i].vPos.x;
		m_pPLSData[i].wmat._42 = m_tagInstancingData[i].vPos.y;
		m_pPLSData[i].wmat._43 = m_tagInstancingData[i].vPos.z;

		//m_pPLSData[i].a = 1.0f;
		m_pPLSData[i].r = m_tagInstancingData[i].vColor.x;
		m_pPLSData[i].g = m_tagInstancingData[i].vColor.y;
		m_pPLSData[i].b = m_tagInstancingData[i].vColor.z;
		m_pPLSData[i].power = m_tagInstancingData[i].fPower;

		m_pPLSData[i].x = m_tagInstancingData[i].vPos.x;
		m_pPLSData[i].y = m_tagInstancingData[i].vPos.y;
		m_pPLSData[i].z = m_tagInstancingData[i].vPos.z;
		m_pPLSData[i].range = m_tagInstancingData[i].fScale;// fRange
		
	}

	// ↑で生成した行列に書き換える
	void* l_pBuf;
	m_pTransMatrixBuf->Lock(0, 0, &l_pBuf, 0);
	memcpy(l_pBuf, m_pPLSData, sizeof(PLSINSTDATA)*m_iMaxInstancingNum);
	m_pTransMatrixBuf->Unlock();

}

void PointLightInstancingMesh::InstancingRender()
{
	
	DEVICE* device = tdnSystem::GetDevice();

	device->SetVertexDeclaration(m_pDecl);

	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_iMaxInstancingNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA|1);

	device->SetStreamSource(0, m_lpVertexBuf, 0, sizeof(MESHVERTEX2));
	device->SetStreamSource(1,m_pTransMatrixBuf,0,sizeof(PLSINSTDATA));

	device->SetIndices(m_lpIndexBuf);

	//	シェーダーの適用
	u32 pass = shaderM->Begin("PointLightSphereInstancing");
	for (u32 p = 0; p < pass; p++)
	{
		// パスのレンダリング
		shaderM->BeginPass(p);

		// テクスチャ指定
		shaderM->SetDecaleTexture(lpTexture[0]);

		// テクスチャ指定
		shaderM->CommitChanges();
		
		//	レンダリング
		HRESULT hr= tdnSystem::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
			m_iNumVertices, 0, m_iNumIndices);

		shaderM->EndPass();
	}
	shaderM->End();


	tdnSystem::GetDevice()->SetStreamSourceFreq(0,1);
	tdnSystem::GetDevice()->SetStreamSourceFreq(1,1);
}


// 初期化
InstancingData::InstancingData()
{
	vPos = VECTOR_ZERO;
	fScale = 1.0f;

	vColor = Vector3(1.0f, 1.0f, 1.0f);
	//fRange = 1.0f;
	//vVpos = VECTOR_ZERO;
	fPower = 1.0f;

}
