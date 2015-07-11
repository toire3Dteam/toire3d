
#ifndef IEX_MESH2_H
#define IEX_MESH2_H

#include "iextreme.h"
#include <stdarg.h>

// ����
/*
�f�X�g���N�^�[�ŁAlpTexture�͏�����܂�
���������Ȃ��ꍇ��lpTexture��nullptr��ݒ肵�Ă�������
<Texture_set_null()>
*/

class iexMesh2_textures
{
public:
	Texture2D** t;

	~iexMesh2_textures();
	// ��̃e�N�X�`����number�쐬
	void Create(int number);
	// index �� �e�N�X�`�������[�h
	void Load(int index, char* filename);
	// number�̃e�N�X�`�������[�h < 2, filename, filename >
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
