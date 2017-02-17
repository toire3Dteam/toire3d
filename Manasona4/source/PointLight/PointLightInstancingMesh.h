#pragma once
#include "TDNLIB.h"

//+-----------------------------------------------------
//	インスタンシング用に拡張
//+-----------------------------------------------------

struct PLSINSTDATA
{

	//FLOAT _11, _12, _13, _14;
	//FLOAT _21, _22, _23, _24;
	//FLOAT _31, _32, _33, _34;
	//FLOAT _41, _42, _43, _44;
	Matrix wmat;

	FLOAT r, g, b, power;
	FLOAT x, y, z, range;
	//FLOAT power, range;
};

struct InstancingData
{
	Vector3 vPos;
	float fScale;// ←レンジ

	Vector3 vColor;
	float	fPower;
	//Vector3 vVpos;	// ビュー
	//float	fRange;

	InstancingData();
};

class PointLightInstancingMesh :public iexMesh
{
private:
	IDirect3DVertexBuffer9* m_lpVertexBuf;
	IDirect3DIndexBuffer9* m_lpIndexBuf;
	IDirect3DVertexBuffer9* m_pTransMatrixBuf;
	IDirect3DVertexDeclaration9 *m_pDecl;

	// 面・頂点数
	u32 m_iNumIndices;
	u32 m_iNumVertices;

	// 最大インスタンシング数
	int m_iMaxInstancingNum;
	//Matrix*	m_pWMatrix;
	PLSINSTDATA* m_pPLSData;


	InstancingData* m_tagInstancingData;

public:
	PointLightInstancingMesh(char* filename, int iMaxInstancingNum = 1);
	~PointLightInstancingMesh();

	//void Update(Matrix* mat,int numMat);
	void InstancingUpdate();
	void InstancingRender();

	// インスタンシング
	int GetMaxInstancingNum() { return m_iMaxInstancingNum; }

	// アクセサ
	InstancingData* GetInstancingData(int iIndex) 
	{ 
	#ifdef _DEBUG
		MyAssert((iIndex < m_iMaxInstancingNum), "上限を超えています。");
	#endif // _DEBUG
		return &m_tagInstancingData[iIndex]; 
	}

	void SetInstancingData(int iIndex, InstancingData eInstancingData)
	{
	#ifdef _DEBUG
		MyAssert((iIndex < m_iMaxInstancingNum), "上限を超えています。");
	#endif // _DEBUG
		m_tagInstancingData[iIndex] = eInstancingData;
	}

};