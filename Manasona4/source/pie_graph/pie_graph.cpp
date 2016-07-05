
#include "pie_graph.h"

Pie_graph::Pie_graph(char *filename) : tdn2DObj(filename), percent(0.0f)
{}

Pie_graph::~Pie_graph()
{}

void Pie_graph::Render(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags, COLOR color, float z)
{
	if (percent <= 0.0f)
	{
		percent = 0.0f;
		return;
	}

	if (percent >= 1.0f)
	{
		percent = 1.0f;
		tdn2DObj::SetARGB(color);
		tdn2DObj::Render(DstX, DstY, DstW, DstH, SrcX, SrcY, width, height, dwFlags);
	}

	float DstW_2(DstW*0.5f), DstH_2(DstH*0.5f), width_2(width*0.5f), height_2(height*0.5f);


	if (percent < 0.125f)
	{
		float per(percent * 8);
		TLVERTEX	v[3];
		v[0].sx = v[2].sx = (float)(DstX + DstW_2);
		v[1].sx = (float)(DstX + DstW_2 + DstW_2 * per);
		v[0].sy = v[1].sy = (float)DstY;
		v[2].sy = (float)(DstY + DstH_2);

		v[0].tu = v[2].tu = (float)(SrcX +  width_2) / (float)this->m_height;
		v[1].tu = (float)(SrcX + width_2 + width_2 * per) / (float)this->m_width;
		v[0].tv = v[1].tv = (float)SrcY / (float)this->m_height;
		v[2].tv = (float)(SrcY + height_2) / (float)this->m_height;

		v[0].color = v[1].color = v[2].color = color;
		v[0].sz = v[1].sz = v[2].sz = z;
		v[0].rhw = v[1].rhw = v[2].rhw = 1.0f;

		tdnPolygon::Render2D(v, 1, this, dwFlags);
	}
	else if (percent < 0.375f)
	{
		float per((percent - 0.125f) * 4.0f);
		TLVERTEX	v[4];
		v[0].sx = v[2].sx = (float)(DstX + DstW_2);
		v[1].sx = v[3].sx = (float)(DstX + DstW);
		v[0].sy = v[1].sy = (float)DstY;
		v[2].sy = (float)(DstY + DstH_2);
		v[3].sy = (float)(DstY + DstH * per);

		v[0].tu = v[2].tu = (float)(SrcX + width_2) / (float)this->m_width;
		v[1].tu = v[3].tu = (float)(SrcX + width) / (float)this->m_width;
		v[0].tv = v[1].tv = (float)SrcY / (float)this->m_height;
		v[2].tv = (float)(SrcY + height_2) / (float)this->m_height;
		v[3].tv = (float)(SrcY + height * per) / (float)this->m_height;

		v[0].color = v[1].color = v[2].color = v[3].color = color;
		v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
		v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

		tdnPolygon::Render2D(v, 2, this, dwFlags);
	}
	else if (percent < 0.625f)
	{
		float per((percent - 0.375f) * 4.0f);
		TLVERTEX	v[5];
		v[0].sx = v[2].sx = (float)(DstX + DstW_2);
		v[1].sx = v[3].sx = (float)(DstX + DstW);
		v[4].sx = (float)(DstX + DstW - DstW * per);
		v[0].sy = v[1].sy = (float)DstY;
		v[2].sy = (float)(DstY + DstH_2);
		v[3].sy = v[4].sy = (float)(DstY + DstH);

		v[0].tu = v[2].tu = (float)(SrcX + width_2) / (float)this->m_width;
		v[1].tu = v[3].tu = (float)(SrcX + width) / (float)this->m_width;
		v[4].tu = (float)(SrcX + width - width * per) / (float)this->m_width;
		v[0].tv = v[1].tv = (float)SrcY / (float)this->m_height;
		v[2].tv = (float)(SrcY + height_2) / (float)this->m_height;
		v[3].tv = v[4].tv = (float)(SrcY + height) / (float)this->m_height;

		v[0].color = v[1].color = v[2].color = v[3].color = v[4].color = color;
		v[0].sz = v[1].sz = v[2].sz = v[3].sz = v[4].sz = z;
		v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = v[4].rhw = 1.0f;

		tdnPolygon::Render2D(v, 3, this, dwFlags);
	}
	else if (percent < 0.875f)
	{
		float per((percent - 0.625f) * 4.0f);
		TLVERTEX	v[6];
		v[0].sx = v[2].sx = (float)(DstX + DstW_2);
		v[1].sx = v[3].sx = (float)(DstX + DstW);
		v[4].sx = v[5].sx = (float)DstX;
		v[0].sy = v[1].sy = (float)DstY;
		v[2].sy = (float)(DstY + DstH_2);
		v[3].sy = v[5].sy = (float)(DstY + DstH);
		v[4].sy = (float)(DstY + DstH - DstH * per);

		v[0].tu = v[2].tu = (float)(SrcX + width_2) / (float)this->m_width;
		v[1].tu = v[3].tu = (float)(SrcX + width) / (float)this->m_width;
		v[4].tu = v[5].tu = (float)SrcX / (float)this->m_width;
		v[0].tv = v[1].tv = (float)SrcY / (float)this->m_height;
		v[2].tv = (float)(SrcY + height_2) / (float)this->m_height;
		v[3].tv = v[5].tv = (float)(SrcY + height) / (float)this->m_height;
		v[4].tv = (float)(SrcY + height - height * per) / (float)this->m_height;

		v[0].color = v[1].color = v[2].color = v[3].color = v[4].color = v[5].color = color;
		v[0].sz = v[1].sz = v[2].sz = v[3].sz = v[4].sz = v[5].sz = z;
		v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = v[4].rhw = v[5].rhw = 1.0f;

		tdnPolygon::Render2D(v, 4, this, dwFlags);
	}
	else
	{
		float per((percent - 0.875f) * 8.0f);
		TLVERTEX	v[7];
		v[0].sx = v[2].sx = (float)(DstX + DstW_2);
		v[1].sx = v[3].sx = (float)(DstX + DstW);
		v[4].sx = v[5].sx = (float)DstX;
		v[6].sx = (float)(DstX + DstW_2 * per);
		v[0].sy = v[1].sy = v[4].sy = v[6].sy = (float)DstY;
		v[2].sy = (float)(DstY + DstH_2);
		v[3].sy = v[5].sy = (float)(DstY + DstH);

		v[0].tu = v[2].tu = (float)(SrcX + width_2) / (float)this->m_width;
		v[1].tu = v[3].tu = (float)(SrcX + width) / (float)this->m_width;
		v[4].tu = v[5].tu = (float)SrcX / (float)this->m_width;
		v[6].tu = (float)(SrcX + width_2 * per) / (float)this->m_width;
		v[0].tv = v[1].tv = v[4].tv = v[6].tv = (float)SrcY / (float)this->m_height;
		v[2].tv = (float)(SrcY + height_2) / (float)this->m_height;
		v[3].tv = v[5].tv = (float)(SrcY + height) / (float)this->m_height;

		v[0].color = v[1].color = v[2].color = v[3].color = v[4].color = v[5].color = v[6].color = color;
		v[0].sz = v[1].sz = v[2].sz = v[3].sz = v[4].sz = v[5].sz = v[6].sz = z;
		v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = v[4].rhw = v[5].rhw = v[6].rhw = 1.0f;

		tdnPolygon::Render2D(v, 4, this, dwFlags);

		TLVERTEX	vv[3] = { v[4], v[6], v[2] };
		tdnPolygon::Render2D(vv, 1, this, dwFlags);
	}
}
