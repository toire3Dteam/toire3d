#pragma once
#include "TDNLIB.h"

//+------------------------
//	���̎c��
//+------------------------

class LocusEffect
{
public:
	LocusEffect(const char* fileName, const int frameMax, const int separateNum = 2);
	~LocusEffect();

	void Update(const Vector3& posL, const Vector3& posR);
	void Update();
	void Render();

	void Action(const Vector3& posL, const Vector3& posR);
	void Action(Vector3* posL, Vector3* posR);
	void Stop();

	// �A�N�Z�T
	void SetCol(DWORD col) { m_lCol = col; };
private:

	// �Q�Ɨp
	Vector3* m_pReferencePosL;
	Vector3* m_pReferencePosR;

	// ���W�̔z��
	Vector3* m_pPosL;
	Vector3* m_pPosR;

	int m_iFrameMax;
	int m_iSeparate;
	int m_iLocusMax;

	tdn2DObj* m_pTex;
	float* m_pTU;
	
	DWORD m_lCol;

	bool m_bAction;

};
