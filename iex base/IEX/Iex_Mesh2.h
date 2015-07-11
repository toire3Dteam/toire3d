
#ifndef IEX_MESH2_H
#define IEX_MESH2_H

#include "iextreme.h"
#include <stdarg.h>

// 注意
/*
デストラクターで、lpTextureは消されます
消したくない場合はlpTextureにnullptrを設定してください
<Texture_set_null()>
*/

class iexMesh2_textures
{
public:
	Texture2D** t;

	~iexMesh2_textures();
	// 空のテクスチャをnumber個作成
	void Create(int number);
	// index の テクスチャをロード
	void Load(int index, char* filename);
	// number個のテクスチャをロード < 2, filename, filename >
	void Create_load(int number, ...);
};

class iexMesh2 : public iexMesh
{
private:


public:
	iexMesh2();
	iexMesh2(char* filename);
	~iexMesh2();

	void Texture_change(iexMesh2_textures& in);

	void Texture_set_null();

	void Get_texture(iexMesh2_textures* out);
};

#endif // !IEX_MESH2_H
