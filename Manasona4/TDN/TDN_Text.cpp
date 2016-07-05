#include	"TDNLIB.h"

LPD3DXFONT tdnText::font;

void tdnText::Init(){
	HFONT hf;
	D3DXFONT_DESC fd;

	hf = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hf, sizeof(LOGFONT), (LPSTR)&fd);
	fd.Height = 32;
	fd.Width = 0;
	fd.Italic = 0;
	fd.CharSet = SHIFTJIS_CHARSET;
	strcpy(fd.FaceName, "ƒƒCƒŠƒI");

	D3DXCreateFontIndirect(tdnSystem::GetDevice(), &fd, &font);
}

void tdnText::CleanUpModule(){
	font->Release();
}

void tdnText::Draw(int x, int y, DWORD color, const char * _Format, ...){
	const int strsize = 1024;
	char str[strsize];
	ZeroMemory(str, sizeof(char)* strsize);
	va_list list;
	va_start(list, _Format);
	vsprintf(str, _Format, list);
	va_end(list);

	RECT rec;
	rec.left = x;
	rec.top = y;
	rec.right = tdnSystem::GetScreenSize().right;
	rec.bottom = tdnSystem::GetScreenSize().bottom;

	font->DrawText(NULL, str, -1, &rec, DT_LEFT | DT_WORDBREAK, color);
	return;
}
