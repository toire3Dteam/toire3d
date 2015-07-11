
#include "Iex_Mesh2.h"
#include <stdarg.h>

iexMesh2_textures::~iexMesh2_textures()
{
	for (int j = 0; j < sizeof(t) / sizeof(t[0]); j++)
	{
		t[j]->Release();
	}
	delete[] t;
}

void iexMesh2_textures::Create(int number)
{
	t = new Texture2D*[number]{};
}

void iexMesh2_textures::Load(int index, char* filename)
{
	t[index] = iexTexture::Load(filename);
}

void iexMesh2_textures::Create_load(int number, ...)
{
	t = new Texture2D*[number]{};

	va_list list;
	va_start(list, number);

	char* filename(nullptr);
	for (int i = 0; i < number; i++)
	{
		filename = va_arg(list, char*);
		t[i] = iexTexture::Load(filename);
	}

	va_end(list);
}



iexMesh2::iexMesh2()
{
	Texture_set_null();
}

iexMesh2::iexMesh2(char* filename) : iexMesh(filename)
{}

iexMesh2::~iexMesh2()
{}

void iexMesh2::Texture_change(iexMesh2_textures& in)
{
	if (*lpTexture == nullptr)
		delete[] lpTexture;
	lpTexture = in.t;
}

void iexMesh2::Texture_set_null()
{
	if (*this->lpTexture == nullptr)
		delete[] lpTexture;
	lpTexture = new Texture2D*[1]{};
}

void iexMesh2::Get_texture(iexMesh2_textures* out)
{
	out->t = this->lpTexture;
}
