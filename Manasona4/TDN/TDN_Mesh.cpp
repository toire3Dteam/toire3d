
#include "TDNLIB.h"
#include <fstream>
#include <string>
#include <vector>

tdnMesh::tdnMesh() : decl( nullptr ), declSize( 0 ), vertexBuffer( nullptr ), numVertexes( 0 ), indexBuffer( nullptr ), numIndexes( 0 ), streamBuffer( nullptr ), streamSize( 0 ), numStream( 0 ), numFaces( 0 ), pos( 0, 0, 0 ), scale( 1, 1, 1 ), rot( 0, 0, 0, 1 ), texture( nullptr )
{
	D3DXMatrixIdentity( &worldMatrix );
}
tdnMesh::~tdnMesh()
{
	if( decl )
		decl->Release();
	if( vertexBuffer )
		vertexBuffer->Release();
	if( indexBuffer )
		indexBuffer->Release();
}

/********/
/* �쐬 */
/********/

bool tdnMesh::CreateVertex( unsigned int numVertexes, unsigned int vertexSize, void *vertexArray )
{
	HRESULT hr( S_OK );

	// ���_�z��̃T�C�Y�v�Z
	unsigned int vertexesSize = vertexSize * numVertexes;
	this->numVertexes = numVertexes;

	// �o�b�t�@new
	hr = tdnSystem::GetDevice()->CreateVertexBuffer(
		vertexesSize,    // ���_�z��̃o�C�g��
		0,               // �g�p���@ D3DUSAGE
		D3DFVF_MESHVERTEX2,      // ?
		D3DPOOL_MANAGED, // �ۑ������f�[�^�̎g�p���@�i�ǂݍ��ݐ�p, �œK������ ...�j
		&vertexBuffer,   // ���_�o�b�t�@ <out>
		0 );             // ?
	if( FAILED( hr ) )
		return false;

	// �o�b�t�@�ɃR�s�[
	void* workVertex( nullptr );
	hr = vertexBuffer->Lock( 0, 0, ( void** ) &workVertex, 0 );      // �o�b�t�@���b�N (�o�t�@�̃|�C���^�[�擾)
	if( FAILED( hr ) )
		return false;
	memcpy_s( workVertex, vertexesSize, vertexArray, vertexesSize ); // �o�b�t�@�ɒ��_�����R�s�[
	hr = vertexBuffer->Unlock();                                     // �o�b�t�@���b�N������
	if( FAILED( hr ) )
		return false;

	return true;
}
bool tdnMesh::CreateIndexes( unsigned int numIndexes, const DWORD *indexArray )
{
	HRESULT hr( S_OK );

	this->numIndexes = numIndexes;

	// �C���f�b�N�X�z��̃T�C�Y�v�Z
	unsigned int indexesSize = sizeof( DWORD ) * numIndexes;
	// �C���f�b�N�Xnew
	hr = tdnSystem::GetDevice()->CreateIndexBuffer(
		indexesSize,     // �C���f�b�N�X�z��̃o�C�g��
		0,               // �g�p���@ D3DUSAGE
		D3DFMT_INDEX32,  // �t�H�[�}�b�g
		D3DPOOL_MANAGED, // ?
		&indexBuffer,    // �C���f�b�N�X�o�b�t�@ <out>
		NULL );          // ?
	if( FAILED( hr ) )
		return false;

	// �o�b�t�@�ɃR�s�[
	DWORD* workIndex( nullptr );
	hr = indexBuffer->Lock( 0, 0, ( void** ) &workIndex, 0 );    // �o�b�t�@���b�N (�o�t�@�̃|�C���^�[�擾)
	if( FAILED( hr ) )
		return false;
	memcpy_s( workIndex, indexesSize, indexArray, indexesSize ); // �o�b�t�@�ɃC���f�b�N�X�����R�s�[
	hr = indexBuffer->Unlock();                                  // �o�b�t�@���b�N������
	if( FAILED( hr ) )
		return false;

	return true;
}
bool tdnMesh::CreateStream( unsigned int numData, unsigned int dataSize, void *dataArray )
{
	HRESULT hr( S_OK );

	// ���_�z��̃T�C�Y�v�Z
	unsigned int dataArraySize = dataSize * numData;
	streamSize = dataSize;
	numStream = numData;

	// �o�b�t�@new
	hr = tdnSystem::GetDevice()->CreateVertexBuffer(
		dataArraySize,   // �z��̃o�C�g��
		0,               // �g�p���@ D3DUSAGE
		0,               // ?
		D3DPOOL_MANAGED, // �ۑ������f�[�^�̎g�p���@�i�ǂݍ��ݐ�p, �œK������ ...�j
		&streamBuffer,   // �o�b�t�@ <out>
		0 );             // ?
	if( FAILED( hr ) )
		return false;

	// �o�b�t�@�ɃR�s�[
	void* workData( nullptr );
	hr = streamBuffer->Lock( 0, 0, ( void** ) &workData, 0 );      // �o�b�t�@���b�N (�o�t�@�̃|�C���^�[�擾)
	if( FAILED( hr ) )
		return false;
	memcpy_s( workData, dataArraySize, dataArray, dataArraySize ); // �o�b�t�@�ɒ��_�����R�s�[
	hr = streamBuffer->Unlock();                                     // �o�b�t�@���b�N������
	if( FAILED( hr ) )
		return false;

	return true;
}
bool tdnMesh::CreateDeclaration( unsigned int declSize, D3DVERTEXELEMENT9 *decl )
{
	HRESULT hr( S_OK );

	this->declSize = declSize;
	//FVF�쐬
	hr = tdnSystem::GetDevice()->CreateVertexDeclaration( decl, &this->decl );
	if( FAILED( hr ) )
		return false;

	return true;
}
bool tdnMesh::Create( const CreateData &data )
{
	if( CreateVertex( data.numVertexes, data.vertexSize, data.vertexArray ) == false )
		return false;
	if( data.numIndexes != 0 && data.indexArray != nullptr)
		if( CreateIndexes( data.numIndexes, data.indexArray ) == false )
			return false;
	if( data.streamSize != 0 && data.streamArray != nullptr && data.numStream != 0 )
		if( CreateStream( data.numStream, data.streamSize, data.streamArray ) == false )
			return false;
	if( CreateDeclaration( data.vertexSize, data.decl ) == false )
		return false;
	numFaces = data.numIndexes / 3;
	return true;
}

bool tdnMesh::CreateTriangle( float radius, DWORD color )
{
	CreateData data;
	float rad120( PI * 2 / 3 );
	float s( sinf( rad120 ) ), c( cosf( rad120 ) );
	VECTOR_LINE vertexArray[]
	{
		{ -s*radius, c*radius, 0, color },
		{ 0, radius, 0, color },
		{ s*radius, c*radius, 0, color }
	};
	data.vertexArray = vertexArray;
	data.vertexSize = sizeof( VECTOR_LINE );
	data.numVertexes = ARRAYSIZE( vertexArray );

	DWORD indexArray[]
	{
		0, 1, 2
	};
	data.indexArray = indexArray;
	data.numIndexes = ARRAYSIZE( indexArray );

	D3DVERTEXELEMENT9 declAry[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof( Vector3 ), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	data.decl = declAry;

	return Create( data );
}
bool tdnMesh::CreateRectangle( float width, float height, DWORD color )
{
	CreateData data;
	width *= 0.5f;
	height *= 0.5f;
	VECTOR_LINE vertexArray[]
	{
		{ -width, -height, 0, color},
		{ -width, height, 0, color },
		{ width, -height, 0, color },
		{ width, height, 0, color }
	};
	data.vertexArray = vertexArray;
	data.vertexSize = sizeof( VECTOR_LINE );
	data.numVertexes = ARRAYSIZE( vertexArray );

	DWORD indexArray[]
	{
		0, 1, 2,
			1, 3, 2
	};
	data.indexArray = indexArray;
	data.numIndexes = ARRAYSIZE( indexArray );

	D3DVERTEXELEMENT9 declAry[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof( Vector3 ), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	data.decl = declAry;

	return Create( data );
}
bool tdnMesh::CreateTriangular( float radius, DWORD color )
{
	CreateData data;
	float rad120( PI * 2 / 3 );
	float s( sinf( rad120 ) ), c( cosf( rad120 ) );
	VECTOR_LINE vertexArray[]
	{
		{0, radius, 0, color},
		{ 0, c*radius, radius, color },
		{ -s*radius, c*radius, c*radius, color },
		{ s*radius, c*radius, c*radius, color }
	};
	data.vertexArray = vertexArray;
	data.vertexSize = sizeof( VECTOR_LINE );
	data.numVertexes = ARRAYSIZE( vertexArray );

	DWORD indexArray[]
	{
		0, 3, 2,
			0, 1, 3,
			0, 2, 1,
			1, 2, 3,
	};
	data.indexArray = indexArray;
	data.numIndexes = ARRAYSIZE( indexArray );

	D3DVERTEXELEMENT9 declAry[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof( Vector3 ), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	data.decl = declAry;

	return Create( data );
}
bool tdnMesh::CreateCube( float width, float height, float depth, DWORD color, Vector3 *posList, unsigned int numPos )
{
	CreateData data;
	width *= 0.5f;
	height *= 0.5f;
	depth *= 0.5f;
	VECTOR_LINE vertexArray[]
	{
		{ -width, -height, -depth, color},
		{ -width, -height, depth, color },
		{ -width, height, -depth, color },
		{ -width, height, depth, color },
		{ width, -height, -depth, color },
		{ width, -height, depth, color },
		{ width, height, -depth, color },
		{ width, height, depth, color }
	};
	data.vertexArray = vertexArray;
	data.vertexSize = sizeof( VECTOR_LINE );
	data.numVertexes = ARRAYSIZE( vertexArray );

	DWORD indexArray[]
	{
		0, 2, 6,
			0, 6, 4,
			4, 6, 7,
			4, 7, 5,
			3, 5, 7,
			1, 5, 3,
			1, 3, 2,
			2, 0, 1,
			0, 4, 1,
			4, 5, 1,
			2, 3, 7,
			7, 6, 2
	};
	data.indexArray = indexArray;
	data.numIndexes = ARRAYSIZE( indexArray );

	if( numPos > 0 )
	{
		D3DVERTEXELEMENT9 declAry[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, sizeof( Vector3 ), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			// streamBuffer �C���X�^���V���O
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
			D3DDECL_END()
		};
		data.decl = declAry;
	}
	else
	{
		D3DVERTEXELEMENT9 declAry[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, sizeof( Vector3 ), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			D3DDECL_END()
		};
		data.decl = declAry;
	}

	data.streamArray = posList;
	data.streamSize = sizeof( Vector3 );
	data.numStream = numPos;

	return Create( data );
}

/************/
/* �ǂݍ��� */
/************/

template<class TYPE_FSTREAM> bool OpenFstream( TYPE_FSTREAM *out, std::string filename )
{
	out->open( filename );
	if( out->fail() ) // �J���Ȃ�����
	{
		char text[256] = {};
		sprintf_s( text, "%s ���J���܂���", filename );
		MessageBox( NULL, text, "�G���[", 0 );
		return false;
	}
	return true;
}

// MQO�t�@�C��

class MqoLoadObject
{
public:
	virtual void Load( std::ifstream &mqoFile ) = 0;

protected:
	// LoadLoop�Ŏg���֐��i�֐��|�C���^�[�j�̖߂�l
	enum class FPCODE : byte
	{
		FAILURE, // ���s
		SUCCESS, // ����
		END      // ���[�v�I��
	};

	// " " �̊Ԃ����o��
	inline void LoadName(
		std::ifstream &mqoFile, // <in> : �ǂݍ��ރt�@�C��
		std::string &name       // <out>: �ǂݍ��񂾕����񂩂� " " �̊Ԃ����o��������
		)
	{
		mqoFile >> name;
		name = name.substr( 1, name.size() - 2 ); // �O��� " " ������
	}

	template< class type, // <�ȗ���> out�̌`
		size_t size>      // <�ȗ���> �z��̗v�f��
		// type �̔z���ǂݍ���
		inline FPCODE LoadArray(
		std::ifstream &mqoFile,  // <in> : �ǂݍ��ރt�@�C��
		const std::string& load, // <in> : �O��ǂݍ��񂾕�����
		const std::string& com,  // <in> : ����p������
		type( &out )[size]       // <out>: �����ɓǂݍ���
		)
	{
		// �I��
		if( load == "}" )
			return FPCODE::END;
		// �Ⴄ
		if( load != com )
			return FPCODE::FAILURE;
		// �z�񕪓ǂݍ���
		for( size_t i = 0; i < size; i++ )
		{
			mqoFile >> out[i];
		}
		return FPCODE::SUCCESS;
	}

	void sto( const std::string in, int *out )
	{
		*out = std::stoi( in );
	}
	void sto( const std::string in, unsigned int *out )
	{
		*out = std::stoi( in );
	}
	void sto( const std::string in, float *out )
	{
		*out = std::stof( in );
	}

	template <class type> // <�ȗ���> ���o���^
	// com(type) �̌`�̕������ǂݍ���type�̎��o��
	inline FPCODE LoadParenthesi(
		const std::string& load, // <in> : �O��ǂݍ��񂾕�����
		const std::string com,   // <in> : ����p������
		type *out                // <out>: �����ɓǂݍ���
		)
	{
		// �I��
		if( load == "}" )
			return FPCODE::END;
		// �Ⴄ
		if( load.substr( 0, com.size() ) != com )
			return FPCODE::FAILURE;
		// ���o��
		sto( load.substr( com.size() + 1 ), out );
		return FPCODE::SUCCESS;
	}

	// com("string") �̌`�̕������ǂݍ���string�����o��
	inline FPCODE LoadStringParenthesi(
		const std::string& load, // <in> : �O��ǂݍ��񂾕�����
		const std::string com,   // <in> : ����p������
		std::string* out         // <out>: �����ɓǂݍ���
		)
	{
		// �I��
		if( load == "}" )
			return FPCODE::END;
		// �Ⴄ
		if( load.substr( 0, com.size() ) != com )
			return FPCODE::FAILURE;
		// ���o��
		*out = load.substr( com.size() + 2, load.size() - com.size() - 4 );
		return FPCODE::SUCCESS;
	}

	template<class type, // ���o���^
		size_t size>     // �z��̗v�f��
		// com(type type ...) �̌`�̕������ǂݍ���type�̔z������o��
		inline FPCODE LoadArrayParenthesi(
		std::ifstream& mqoFile, // <in> : �ǂݍ��ރt�@�C��
		std::string& load,      // <in> : �O��ǂݍ��񂾕�����
		const std::string com,  // <in> : ����p������
		type( &out )[size]      // <out>: �����ɓǂݍ���
		)
	{
		// �I��
		if( load == "}" )
			return FPCODE::END;
		// �Ⴄ
		if( load.substr( 0, com.size() ) != com )
			return FPCODE::FAILURE;
		// ���������o��
		out[0] = stof( load.substr( com.size() + 1 ) );
		for( size_t i = 1; i < size; i++ )
		{
			mqoFile >> out[i];
		}
		std::string workString;
		mqoFile >> workString; // ")" ��ǂ�
		return FPCODE::SUCCESS;
	}

	template<class LoadObjectClass> // MqoLoadObject ���p�������N���X 
	// �֐��|�C���^�[�̔z�����
	void LoadLoop(
		LoadObjectClass* me,    // <in> : this�|�C���^�[�����
		std::ifstream &mqoFile, // <in> : �ǂݍ��ރt�@�C��
		FPCODE( LoadObjectClass::*loadFP[] )( std::ifstream&, std::string& load ), // <in> : �֐��|�C���^�z��
		unsigned int numFP      // <in> : �z�񂨗v�f��
		)
	{
		for( unsigned int step( 0 ); step < numFP; )
		{
			std::string workString;
			mqoFile >> workString;
			if( mqoFile.eof() )
				break;
			// �֐��|�C���^�[����
			for( unsigned int i = step; i < numFP; i++ )
			{
				switch( ( me->*loadFP[i] )( mqoFile, workString ) )
				{
					case FPCODE::FAILURE: // �����
						break;

					case FPCODE::SUCCESS: // �ǂݍ���
						step = i;         // step��i�߂�
						i = numFP;        // ���[�v�������
						break;

					case FPCODE::END: // ���[�v�I��
						i = numFP;    // ���[�v�������
						step = numFP; // ���[�v������
						break;
				}
			}
		}
	}
};

class MqoMaterial : public MqoLoadObject
{
public:
	std::string name;    // �ގ���       31byte�ȓ�
	float col[4];        // ���_�F RGBA  0~1
	float dif;           // �g�U��       0~1
	float amb;           // ���͌�       0~1
	float emi;           // ���ȏƖ�     0~1
	float spc;           // ���ˌ�       0~1
	float power;         // ���ˌ��̋��� 0~100
	float reflect;       // ���ʔ���     0~1
	float refract;       // ���ܗ�       1~5
	std::string tex;     // �͗l�e�N�X�`��
	std::string aplane;  // �����e�N�X�`��
	std::string bump;    // ���ʃe�N�X�`��
	float proj_pos[3];   // �ʒu XYZ
	float proj_scale[3]; // �g�嗦 XYZ
	float proj_angle[3]; // �p�x YXZ     -180~180

	MqoMaterial() : dif( 0 ), amb( 0 ), emi( 0 ), spc( 0 ), power( 0 ), reflect( 0 ), refract( 0 )
	{
		col[0] = col[1] = col[2] = col[3] = 1;
		proj_pos[0] = proj_pos[1] = proj_pos[2] = 0;
		proj_scale[0] = proj_scale[1] = proj_scale[2] = 1;
		proj_angle[0] = proj_angle[1] = proj_angle[2] = 0;
	}

	void Load( std::ifstream &mqoFile )
	{
		std::string workString;
		mqoFile >> workString; // { ��ǂ�
		LoadName( mqoFile, name );

		// �ϐ���ǂݍ��ފ֐�
		FPCODE( MqoMaterial::*loadFP[] )( std::ifstream &mqoFile, std::string& load )
		{
			&MqoMaterial::LaodCol, &MqoMaterial::LoadDif, &MqoMaterial::LoadAmb, &MqoMaterial::LoadEmi, &MqoMaterial::LoadSpc, &MqoMaterial::LoadPower, &MqoMaterial::LoadReflect, &MqoMaterial::LoadRefract, &MqoMaterial::LoadTex, &MqoMaterial::LoadAplane, &MqoMaterial::LoadBump, &MqoMaterial::LaodProj_pos, &MqoMaterial::LaodProj_scale, &MqoMaterial::Laodproj_angle
		};

		LoadLoop<MqoMaterial>( this, mqoFile, loadFP, ARRAYSIZE( loadFP ) );
	}

private:
	FPCODE LaodCol( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArrayParenthesi( mqoFile, load, "col", col );
	}
	FPCODE LoadDif( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "dif", &dif );
	}
	FPCODE LoadAmb( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "amb", &amb );
	}
	FPCODE LoadEmi( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "emi", &emi );
	}
	FPCODE LoadSpc( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "spc", &spc );
	}
	FPCODE LoadPower( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "power", &power );
	}
	FPCODE LoadReflect( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "reflect", &reflect );
	}
	FPCODE LoadRefract( std::ifstream &mqoFile, std::string& load )
	{
		return LoadParenthesi( load, "refract", &refract );
	}
	FPCODE LoadTex( std::ifstream &mqoFile, std::string& load )
	{
		return LoadStringParenthesi( load, "tex", &tex );
	}
	FPCODE LoadAplane( std::ifstream &mqoFile, std::string& load )
	{
		return LoadStringParenthesi( load, "aplane", &aplane );
	}
	FPCODE LoadBump( std::ifstream &mqoFile, std::string& load )
	{
		return LoadStringParenthesi( load, "bump", &bump );
	}
	FPCODE LaodProj_pos( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArrayParenthesi( mqoFile, load, "proj_pos", proj_pos );
	}
	FPCODE LaodProj_scale( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArrayParenthesi( mqoFile, load, "proj_scale", proj_scale );
	}
	FPCODE Laodproj_angle( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArrayParenthesi( mqoFile, load, "proj_angle", proj_angle );
	}
};

class MqoVertexList : public MqoLoadObject
{
public:
	class vertex
	{
	public:
		float pos[3];

		vertex()
		{
			pos[0] = pos[1] = pos[2] = 0;
		}

		void Load( std::ifstream& mqoFile )
		{
			mqoFile >> pos[0];
			mqoFile >> pos[1];
			mqoFile >> pos[2];
		}
	};

	std::vector<vertex> list;

	void Load( std::ifstream &mqoFile )
	{
		unsigned int numVertex;
		mqoFile >> numVertex;
		list.resize( numVertex );

		std::string workString;
		mqoFile >> workString; // { ��ǂݍ���

		for( size_t i = 0; i < numVertex; i++ )
		{
			list[i].Load( mqoFile );
		}

		mqoFile >> workString; // } ��ǂݍ���
	}
};

class MqoFaceList : public MqoLoadObject
{
public:

	class Face
	{
	public:
		class Vertex
		{
		public:
			unsigned int V;        // ���_�C���f�b�N�X
			float UV[2];
			DWORD COL;             // ���_�F ABGR

			Vertex() : V( 0 ), COL( 0xFFFFFFFF )
			{
				UV[0] = UV[1] = 0;
			}
		};
		std::vector<Vertex> vertex; // ���_�C���f�b�N�X�ƒ��_���
		unsigned int M;             // �ގ��C���f�b�N�X

		Face() : M( 0 )
		{}

		void LoadV( std::ifstream &mqoFile, const std::string& load )
		{
			vertex[0].V = std::stoi( load.substr( 2 ) );
			for( size_t i = 1; i < vertex.size(); i++ )
			{
				mqoFile >> vertex[i].V;
			}
		};
		void LoadM( const std::string& load )
		{
			M = std::stoi( load.substr( 2 ) );
		}
		void LoadUV( std::ifstream &mqoFile, const std::string& load )
		{
			vertex[0].UV[0] = std::stof( load.substr( 3 ) );
			mqoFile >> vertex[0].UV[1];
			for( size_t i = 1; i < vertex.size(); i++ )
			{
				mqoFile >> vertex[i].UV[0];
				mqoFile >> vertex[i].UV[1];
			}
		}
		void LoadCOL( std::ifstream &mqoFile, const std::string& load )
		{
			vertex[0].COL = std::stoi( load.substr( 4 ) );
			for( size_t i = 1; i < vertex.size(); i++ )
			{
				mqoFile >> vertex[i].COL;
			}
		}
	};
	std::vector<Face> list;

	void Load( std::ifstream &mqoFile )
	{
		unsigned int numFace( 0 );
		mqoFile >> numFace;
		list.resize( numFace );

		std::string workString;
		mqoFile >> workString; // { �ǂݍ���

		for( size_t i = 0; i <= list.size(); )
		{
			mqoFile >> workString;
			if( workString == "}" )
				break;

			if( "0" <= workString && workString <= "9" )
			{
				i++;
				list[i - 1].vertex.resize( std::stoi( workString ) );
				continue;
			}

			if( i <= 0 )
				continue;

			if( workString.find( "COL" ) != std::string::npos )
			{
				list[i - 1].LoadCOL( mqoFile, workString );
			}
			else if( workString.find( "UV" ) != std::string::npos )
			{
				list[i - 1].LoadUV( mqoFile, workString );
			}
			else if( workString.find( "V" ) != std::string::npos )
			{
				list[i - 1].LoadV( mqoFile, workString );
			}
			else if( workString.find( "M" ) != std::string::npos )
			{
				list[i - 1].LoadM( workString );
			}
		}

		mqoFile >> workString; // } �ǂݍ���
	}

private:
};

class MqoObject : public MqoLoadObject
{
public:
	std::string name;     // �I�u�W�F�N�g�� 63byte�ȓ�
	float scale[3];       // �g�� XYZ
	float rotation[3];    // ��] YXZ
	float translation[3]; // �ړ� XYZ
	int visible;         // 0 : ��\��, 15 : �\��
	float color[3];       // �F   RGB 0~1

	MqoVertexList vertexList;
	MqoFaceList faceList;

	MqoObject()
	{}

	void Load( std::ifstream &mqoFile )
	{
		LoadName( mqoFile, name );

		FPCODE( MqoObject::*loadFP[] )( std::ifstream &mqoFile, std::string& load )
		{
			&MqoObject::LoadScale, &MqoObject::LoadRotation, &MqoObject::LoadtTranslation, &MqoObject::LoadtVisible, &MqoObject::LoadtColor, &MqoObject::LoadVertex, &MqoObject::LoadFace,
		};

		LoadLoop<MqoObject>( this, mqoFile, loadFP, ARRAYSIZE( loadFP ) );
	}

private:
	FPCODE LoadScale( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArray( mqoFile, load, "scale", scale );
	}
	FPCODE LoadRotation( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArray( mqoFile, load, "rotation", rotation );
	}
	FPCODE LoadtTranslation( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArray( mqoFile, load, "translation", translation );
	}
	FPCODE LoadtVisible( std::ifstream &mqoFile, std::string& load )
	{
		if( load == "}" )
			return FPCODE::END;

		if( load != "visible" )
			return FPCODE::FAILURE;

		mqoFile >> visible;
		return FPCODE::SUCCESS;
	}
	FPCODE LoadtColor( std::ifstream &mqoFile, std::string& load )
	{
		return LoadArray( mqoFile, load, "color", color );
	}
	FPCODE LoadVertex( std::ifstream &mqoFile, std::string& load )
	{
		if( load == "}" )
			return FPCODE::END;

		if( load != "vertex" )
			return FPCODE::FAILURE;

		vertexList.Load( mqoFile );
		return FPCODE::SUCCESS;
	}
	FPCODE LoadFace( std::ifstream &mqoFile, std::string& load )
	{
		if( load == "}" )
			return FPCODE::END;

		if( load != "face" )
			return FPCODE::FAILURE;

		faceList.Load( mqoFile );
		return FPCODE::SUCCESS;
	}
};

class MQOLoader
{
public:
	MQOLoader() : vertexList( nullptr ), numVtx( 0 ), indexList( nullptr ), vertexDataList( nullptr ), numIdx( 0 )
	{}
	~MQOLoader()
	{
		SAFE_DELETE( vertexList );
		SAFE_DELETE( indexList );
		SAFE_DELETE( vertexDataList );
	}

	bool CreateMesh( tdnMesh *mesh, std::string filename )
	{
		std::ifstream mqoFile;
		OpenFstream( &mqoFile, filename );
		Load( mqoFile );
		mqoFile.close();

		CreateVertexArray();
		CreateIndexAndDataArray();

		tdnMesh::CreateData data;
		data.numVertexes = numIdx;
		data.vertexSize = sizeof( MESHVERTEX2 );
		data.vertexArray = vertexDataList;
		data.numIndexes = numIdx;
		data.indexArray = indexList;
		data.streamSize = 0;
		data.streamArray = nullptr;

		D3DVERTEXELEMENT9 declAry[] = {
			// vertexBufer
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, sizeof( float ) * 3, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, sizeof( float ) * 6, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 0, sizeof( float ) * 6 + sizeof(COLOR), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};
		data.decl = declAry;

		bool ret( true );
		ret = mesh->Create( data );

		std::string filepath;
		UINT seekl = filename.find_last_of( "/" );
		filepath = filename.substr( 0, seekl + 1 );
		filepath += materialList[0].tex;
		mesh->texture = tdnTexture::Load( "DATA/mqo/box/box.png" );

		return ret;
	}

private:
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
	};
	Vertex *vertexList;
	unsigned int numVtx;

	DWORD *indexList;
	MESHVERTEX2 *vertexDataList;
	unsigned int numIdx;

	std::vector<MqoMaterial> materialList;
	std::vector<MqoObject> objectList;

	// �ǂݍ���
	void Load( std::ifstream &mqoFile )
	{
		while( true )
		{
			std::string command;
			mqoFile >> command;
			if( mqoFile.eof() )
				break;

			// �}�e���A���ǂݍ���
			if( command.find( "Material" ) != std::string::npos )
			{
				unsigned int numMaterial;
				mqoFile >> numMaterial;
				materialList.resize( numMaterial );

				for( unsigned int i = 0; i < numMaterial; i++ )
				{
					materialList[i].Load( mqoFile );
				}
			}
			// �I�u�W�F�N�g�ǂݍ���
			else if( command.find( "Object" ) != std::string::npos )
			{
				objectList.push_back( MqoObject() );
				objectList.rbegin()->Load( mqoFile );
			}
		}
	}

	// �ʒu�����ݒ�
	void CreateVertexArray()
	{
		for( unsigned int i1 = 0; i1 < objectList.size(); i1++ )
		{
			numVtx += objectList[i1].vertexList.list.size();
		}
		SAFE_DELETE( vertexList );
		vertexList = new Vertex[numVtx]
		{};

		for( unsigned int vi = 0; vi < numVtx; )
		{
			for( unsigned int i1 = 0; i1 < objectList.size(); i1++ )
			{
				for( unsigned int i2 = 0; i2 < objectList[i1].vertexList.list.size(); i2++ )
				{
					vertexList[vi].position.x = objectList[i1].vertexList.list[i2].pos[0];
					vertexList[vi].position.y = objectList[i1].vertexList.list[i2].pos[1];
					vertexList[vi].position.z = objectList[i1].vertexList.list[i2].pos[2];
					vi++;
				}
			}
		}
	}

	void CreateIndexAndDataArray()
	{
		// �C���f�b�N�X��
		for( unsigned int i1 = 0; i1 < objectList.size(); i1++ )
		{
			for( unsigned int i2 = 0; i2 < objectList[i1].faceList.list.size(); i2++ ) // �O�p�|���S���ɕύX
				numIdx += 3 * ( objectList[i1].faceList.list[i2].vertex.size() - 2 );
		}
		// �z��new
		SAFE_DELETE( indexList );
		indexList = new DWORD[numIdx]
		{};
		for( unsigned int i = 0; i < numIdx; i++ )
		{
			indexList[i] = i;
		}
		SAFE_DELETE( vertexDataList );
		vertexDataList = new MESHVERTEX2[numIdx]
		{};

		// �@���v�Z
		for( unsigned int i1 = 0; i1 < objectList.size(); i1++ )
			for( unsigned int i2 = 0; i2 < objectList[i1].faceList.list.size(); i2++ )
				// �O�p�|���S���Ŗ@���v�Z
				for( unsigned int i3 = 0; i3 + 2 < objectList[i1].faceList.list[i2].vertex.size(); i3++ )
				{
					// �C���f�b�N�X
					unsigned int index[3];
					index[0] = objectList[i1].faceList.list[i2].vertex[i3].V;
					index[1] = objectList[i1].faceList.list[i2].vertex[i3 + 1].V;
					index[2] = objectList[i1].faceList.list[i2].vertex[i3 + 2].V;
					// ���_
					Vector3 vertex[3];
					vertex[0] = vertexList[index[0]].position;
					vertex[1] = vertexList[index[1]].position;
					vertex[2] = vertexList[index[2]].position;
					// �@��
					Vector3 Normal;
					Normal = Vector3Cross( vertex[2] - vertex[0], vertex[1] - vertex[0] );
					Normal.Normalize();

					vertexList[index[0]].normal += Normal;
					vertexList[index[1]].normal += Normal;
					vertexList[index[2]].normal += Normal;
				}
		// ���K��
		for( unsigned int i1 = 0; i1 < numVtx; i1++ )
			vertexList[i1].normal.Normalize();

		// ���_�z��ݒ�
		for( unsigned int iIndex = 0, i1 = 0; i1 < objectList.size(); i1++ )
			for( unsigned int i2 = 0; i2 < objectList[i1].faceList.list.size(); i2++ )
				for( unsigned int i3 = 0; i3 + 2 < objectList[i1].faceList.list[i2].vertex.size(); i3++ )
				{
					// �C���f�b�N�X
					unsigned int index[3];
					index[0] = objectList[i1].faceList.list[i2].vertex[0].V;
					index[1] = objectList[i1].faceList.list[i2].vertex[i3 + 1].V;
					index[2] = objectList[i1].faceList.list[i2].vertex[i3 + 2].V;

					auto setData = [&]( unsigned int index1, unsigned int index2, unsigned int index3 )
					{
						vertexDataList[index1].x = vertexList[index2].position.x;
						vertexDataList[index1].y = vertexList[index2].position.y;
						vertexDataList[index1].z = vertexList[index2].position.z;
						vertexDataList[index1].nx = vertexList[index2].normal.x;
						vertexDataList[index1].ny = vertexList[index2].normal.y;
						vertexDataList[index1].nz = vertexList[index2].normal.z;

						vertexDataList[index1].tu = objectList[i1].faceList.list[i2].vertex[index3].UV[0];
						vertexDataList[index1].tv = objectList[i1].faceList.list[i2].vertex[index3].UV[1];
						vertexDataList[index1].color = objectList[i1].faceList.list[i2].vertex[index3].COL;
					};
					// ���_
					setData( iIndex, index[0], 0 );
					setData( iIndex + 2, index[1], i3 + 1 );
					setData( iIndex + 1, index[2], i3 + 2 );
					iIndex += 3;
				}

	}

};

bool tdnMesh::LoadMqo( char *filename )
{
	std::ifstream mqoFile;
	if( OpenFstream( &mqoFile, filename ) == false )
		return false;

	MQOLoader loader;
	loader.CreateMesh( this, filename );

	return true;
}

/********/
/* �X�V */
/********/

void tdnMesh::UpdateWorldMatrix()
{
	D3DXMatrixRotationQuaternion( &worldMatrix, &rot );
	worldMatrix._11 *= scale.x;
	worldMatrix._12 *= scale.x;
	worldMatrix._13 *= scale.x;
	worldMatrix._14 *= scale.x;
	worldMatrix._21 *= scale.y;
	worldMatrix._22 *= scale.y;
	worldMatrix._23 *= scale.y;
	worldMatrix._24 *= scale.y;
	worldMatrix._31 *= scale.z;
	worldMatrix._32 *= scale.z;
	worldMatrix._33 *= scale.z;
	worldMatrix._34 *= scale.z;
	worldMatrix._41 = pos.x;
	worldMatrix._42 = pos.y;
	worldMatrix._43 = pos.z;
	worldMatrix._44 = 1;
}

/********/
/* �`�� */
/********/

void tdnMesh::Render( tdnShader *shader, char *technique )
{
	unsigned int numPass = shader->Begin( technique );

	shader->SetValue( "worldMatrix", worldMatrix );
	if( texture )
		shader->SetValue( "Texture", texture );

	//�g�p�X�g���[���w��ƒ��_�o�b�t�@�ݒ�
	tdnSystem::GetDevice()->SetStreamSource( 0, vertexBuffer, 0, declSize );
	tdnSystem::GetDevice()->SetStreamSourceFreq( 0, D3DSTREAMSOURCE_INDEXEDDATA | numStream ); // �S��`�悷��
	if( numStream > 0 )
	{
		tdnSystem::GetDevice()->SetStreamSource( 1, streamBuffer, 0, streamSize );
		tdnSystem::GetDevice()->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1 ); // �P��`�悷�閈�Ɏ��̃f�[�^�ɕύX
	}
	//���_�C���f�b�N�X�w��
	bool isIndex = indexBuffer != null;
	tdnSystem::GetDevice()->SetIndices( indexBuffer );
	//���_�\���̎w��
	tdnSystem::GetDevice()->SetVertexDeclaration( decl );
	for( unsigned int i = 0; i < numPass; i++ )
	{
		shader->BeginPass( i );

		//�`��(�ʓ�ƂO����n�܂钸�_�Ǝw��)
		if( isIndex )
		tdnSystem::GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, //�J�n�n�_����̑��Έʒu
			0, //�ŏ��C���f�b�N�X�ԍ�
			numIndexes, //�ő�C���f�b�N�X�ԍ�
			0, //�C���f�b�N�X�z��̓ǂݎ��ʒu	
			numFaces //�ʂ̐�
			);
		else
			tdnSystem::GetDevice()->DrawPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			numFaces //�ʂ̐�
			);
		shader->EndPass();
	}

	shader->End();
}

/*********************/
/* �Z�b�^�[ �Q�b�^�[ */
/*********************/

const Vector3& tdnMesh::Pos()
{
	return pos;
}
void tdnMesh::Pos( const Vector3& in )
{
	pos = in;
}
const Vector3& tdnMesh::Scale()
{
	return scale;
}
void tdnMesh::Scale( const Vector3& in )
{
	scale = in;
}
const Quaternion& tdnMesh::Rot()
{
	return rot;
}
void tdnMesh::Rot( const Quaternion& in )
{
	rot = in;
}
const Matrix& tdnMesh::WorldMatrix()
{
	return worldMatrix;
}
void tdnMesh::WorldMatrix( const Matrix& in )
{
	worldMatrix = in;
}
