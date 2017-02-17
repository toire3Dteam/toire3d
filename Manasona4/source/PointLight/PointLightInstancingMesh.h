#pragma once
#include "TDNLIB.h"

//+-----------------------------------------------------
//	�C���X�^���V���O�p�Ɋg��
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
	float fScale;// �������W

	Vector3 vColor;
	float	fPower;
	//Vector3 vVpos;	// �r���[
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

	// �ʁE���_��
	u32 m_iNumIndices;
	u32 m_iNumVertices;

	// �ő�C���X�^���V���O��
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

	// �C���X�^���V���O
	int GetMaxInstancingNum() { return m_iMaxInstancingNum; }

	// �A�N�Z�T
	InstancingData* GetInstancingData(int iIndex) 
	{ 
	#ifdef _DEBUG
		MyAssert((iIndex < m_iMaxInstancingNum), "����𒴂��Ă��܂��B");
	#endif // _DEBUG
		return &m_tagInstancingData[iIndex]; 
	}

	void SetInstancingData(int iIndex, InstancingData eInstancingData)
	{
	#ifdef _DEBUG
		MyAssert((iIndex < m_iMaxInstancingNum), "����𒴂��Ă��܂��B");
	#endif // _DEBUG
		m_tagInstancingData[iIndex] = eInstancingData;
	}

};