#pragma once
#include "TDNLIB.h"

struct COLOR_INT
{
	int a, r, g, b;
	void Clear() { a = r = g = b = 255; };
};

// �p�����[�^�[
struct EffecrParam
{
	int			frame;
	Vector2		pos;
	float		scale;
	float		angle;
	COLOR_INT	col;

	void Clear()
	{
		frame = 0;
		pos.x = pos.y = 0;
		scale = 1.0f;
		angle = 0.0f;
		col.Clear();
	}
};

/*****************************/
//
/*****************************/
class Effect2D
{
public:
	Effect2D();
	~Effect2D();

	void LoadTexture(char* fileName);
	void Update(int gameFrame);
	void Render();
	void Render(int screenX, int screenY);

public:
	// �I�u�W�F
	tdn2DObj* m_pObj;

	// �p�����[�^
	int		m_iFrame;
	//int		m_iMaxFrame;
	Vector2 m_vPos;
	float	m_fScale;
	float	m_fAngle;
	COLOR_INT m_sCol;

	// �t���[�����̐ݒ�
	EffecrParam m_sStart;
	EffecrParam m_sMiddle;
	EffecrParam m_sEnd;
	
	bool m_bMiddleFlag;

	// ���O
	char m_sListName[256];

	// ���
	char m_sName[256];

	// �`�悷�邩���Ȃ���
	bool m_bRenderFlag;

private:
	//void FrameUpdate();
	void CalcParam();

};
