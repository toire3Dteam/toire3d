#include	"TDNLIB.h"

//**************************************************************************************************
//
//
//
//**************************************************************************************************

//------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------
iexMesh::iexMesh( char* filename )
{
	lpMesh = NULL;
	lpTexture = NULL;
	lpNormal = NULL;
	lpSpecular = NULL;
	lpHeight = NULL;

	//	�t�@�C���ǂݍ���
	char*	ext = &filename[ lstrlen(filename)-4 ];
	if( lstrcmpi( ext, ".imo" ) == 0 ) LoadIMO(filename);
	 else LoadX(filename);

	if( lpMesh == NULL )
	{
		//	�ǂݍ��ݎ��s
		tdnSystem::printf( "*�G���[[iexMesh] ---> ���[�h���s: \"%s\"\n", filename );
		bLoad = FALSE;
	} else {
		DWORD* pAdjacency = new DWORD [ lpMesh->GetNumFaces() * 3 ];
		// �œK��
		lpMesh->GenerateAdjacency( 1e-6f, pAdjacency );
		lpMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, pAdjacency, NULL, NULL, NULL );
		delete[] pAdjacency;

		bLoad = TRUE;
	}
	SetPos(0,0,0);
	SetScale(1.0f);
	SetAngle(0);
	Update();
}

//------------------------------------------------------
//	�f�X�g���N�^
//------------------------------------------------------
iexMesh::~iexMesh()
{
	Release();
}

void iexMesh::Release()
{
	if( bLoad )
	{
		if( lpMesh ) lpMesh->Release();

		//	�e�N�X�`�����
		for( DWORD i=0 ; i<MaterialCount ; i++ ){
			if( lpTexture[i] != NULL ) tdnTexture::Release(lpTexture[i]);
			if( lpNormal[i] != NULL ) tdnTexture::Release(lpNormal[i]);
			if( lpSpecular[i] != NULL ) tdnTexture::Release(lpSpecular[i]);
			if( lpHeight[i] != NULL ) tdnTexture::Release(lpHeight[i]);
		}
		delete[] lpMaterial;
		delete[] lpNormal;
		delete[] lpSpecular;
		delete[] lpHeight;
	}
	delete[] lpTexture;
}

//------------------------------------------------------
//	�N���[���̍쐬
//------------------------------------------------------
iexMesh*	iexMesh::Clone()
{
	iexMesh* obj = new iexMesh(*this);

	int num_tex = sizeof(this->lpTexture) / sizeof(*this->lpTexture);
	obj->lpTexture = new Texture2D*[num_tex];
	for (int i = 0; i < num_tex; i++) obj->lpTexture[i] = this->lpTexture[i];

	obj->bLoad = FALSE;
	return obj;
}

iexMesh*	iexMesh::Clone(int num_tex)
{
	iexMesh*	obj = new iexMesh(*this);

	obj->lpTexture = new Texture2D*[num_tex];
	for (int i = 0; i < num_tex; i++) obj->lpTexture[i] = this->lpTexture[i];

	obj->bLoad = FALSE;
	return obj;
}

//*****************************************************************************
//	���ݒ�
//*****************************************************************************
//------------------------------------------------------
//	�ʒu�ݒ�
//------------------------------------------------------
void iexMesh::SetPos( float x, float y, float z )
{
	Pos.x = x;
	Pos.y = y;
	Pos.z = z;
	bChanged = TRUE;
}

void iexMesh::SetPos( Vector3& pos )
{
	Pos = pos;
	bChanged = TRUE;
}

//------------------------------------------------------
//	��]�ݒ�
//------------------------------------------------------
void iexMesh::SetAngle( float x, float y, float z )
{
	Angle.x = x;
	Angle.y = y;
	Angle.z = z;
	bChanged = TRUE;
}

void iexMesh::SetAngle( float angle )
{
	Angle.x = .0f;
	Angle.y = angle;
	Angle.z = .0f;
	bChanged = TRUE;
}

void iexMesh::SetAngle( Vector3& angle )
{
	Angle = angle;
	bChanged = TRUE;
}

//------------------------------------------------------
//	�X�P�[���ݒ�
//------------------------------------------------------
void iexMesh::SetScale( float x, float y, float z )
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
	bChanged = TRUE;
}

void iexMesh::SetScale( float scale )
{
	Scale.x = scale;
	Scale.y = scale;
	Scale.z = scale;
	bChanged = TRUE;
}
void iexMesh::SetScale( Vector3& scale )
{
	Scale = scale;
	bChanged = TRUE;
}

//**************************************************************************************************
//
//		�X�V����
//
//**************************************************************************************************
void	iexMesh::Update()
{
	Matrix MatScale;

	if( !lpMesh ) return;
	//	�X�P�[�����O
	D3DXMatrixScaling(&MatScale, Scale.x, Scale.y, Scale.z);

	//	���W�n�ݒ� 
	//SetTransformMatrixXYZ( &TransMatrix, Pos.x, Pos.y, Pos.z, Angle.x, Angle.y, Angle.z );
	{
		float	sx, sy, sz, cx, cy, cz;

		sx = sinf(Angle.x);	sy = sinf(Angle.y);	sz = sinf(Angle.z);
		cx = cosf(Angle.x);	cy = cosf(Angle.y);	cz = cosf(Angle.z);

		TransMatrix._11 = (float)(cy*cz + sx*sy*sz);
		TransMatrix._12 = (float)(cx*sz);
		TransMatrix._13 = (float)(-sy*cz + sx*cy*sz);
		TransMatrix._14 = (float)(0.0f);

		TransMatrix._21 = (float)(-cy*sz + sx*sy*cz);
		TransMatrix._22 = (float)(cx*cz);
		TransMatrix._23 = (float)(sy*sz + sx*cy*cz);
		TransMatrix._24 = (float)(0.0f);

		TransMatrix._31 = (float)(cx*sy);
		TransMatrix._32 = (float)(-sx);
		TransMatrix._33 = (float)(cx*cy);
		TransMatrix._34 = (float)(0.0f);

		TransMatrix._41 = Pos.x;
		TransMatrix._42 = Pos.y;
		TransMatrix._43 = Pos.z;
		TransMatrix._44 = (float)(1.0f);
	}

	//	�]���s��쐬 
	TransMatrix = MatScale * TransMatrix;
}

//**************************************************************************************************
//
//		�`��
//
//**************************************************************************************************

//------------------------------------------------------
//	�ʏ�`��
//------------------------------------------------------
void iexMesh::Render(){ Render( RS::COPY, -1.0f ); }

//------------------------------------------------------
//	�t���O�w��`��
//------------------------------------------------------
void iexMesh::Render( u32 dwFlags, float param )
{
	if( !lpMesh ) return;

    //	�]���s��ݒ�
	tdnSystem::GetDevice()->SetTransform( D3DTS_WORLD, &TransMatrix );
	//	���b�V���̃����_�����O
	for( u32 i=0 ; i<MaterialCount ; i++ ){
		//	�����x�ݒ�
		if( param != -1.0f ) lpMaterial[i].Diffuse.a = lpMaterial[i].Ambient.a = param;
		//	�ގ��ݒ�
		tdnRenderState::Set( dwFlags, &lpMaterial[i], lpTexture[i] );
		//	�ގ��O���[�v�`��
		lpMesh->DrawSubset( i );
	}
}

//------------------------------------------------------
//	�V�F�[�_�[�`��
//------------------------------------------------------
void iexMesh::Render( tdnShader* shader, char* name )
{
	//	�V�F�[�_�[�̓K�p
	u32 pass = shader->Begin(name);

	// ���[�J��-�ˉe�ϊ��s��
	Matrix m = TransMatrix * matView * matProjection;
	shader->SetValue("WMatrix", TransMatrix );
	shader->SetWVPMatrix( &m );

	for( u32 p=0 ; p<pass ; p++ )
	{
		//	�p�X�̃����_�����O
		shader->BeginPass(p);
		for( u32 i=0 ; i<MaterialCount ; i++ )
		{
			//	�e�N�X�`���w��
			shader->SetDecaleTexture( lpTexture[i] );
			shader->SetValue( "NormalMap", lpNormal[i] );
			shader->SetValue( "LightMap", lpSpecular[i] );
			shader->SetValue( "MultiMap", lpHeight[i] );
			shader->CommitChanges();
			//	�ގ��O���[�v�`��
			lpMesh->DrawSubset( i );
		}
		shader->EndPass();
	}

	shader->End();
}

//**************************************************************************************************
//
//		���C�s�b�N
//
//**************************************************************************************************
//------------------------------------------------------
//		���C�s�b�N
//------------------------------------------------------
int	iexMesh::RayPick( Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	int		ret = -1;

	if( vec->x == .0f && vec->z == .0f ) return RayPickUD( out, pos, vec, Dist );

	Vector3 p = *pos;
	Vector3 vv = *vec;

	float neart = *Dist;
	float dist = *Dist;
	dist = dist*dist;
	*out = p;
	//	���擾	
	u32 fvf = lpMesh->GetFVF();
	//	���_�T�C�Y�v�Z
	int VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);

	//	�o�b�t�@���b�N
	float	*pVertices;
	u16		*pIndices;
	u32 NumIndices = lpMesh->GetNumFaces();
	lpMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
	lpMesh->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pIndices );

	Vector3 v1, v2, v3;
	Vector3	n;
	Vector3	l1, l2, l3;
	Vector3	temp;
	Vector3	cp;

	Vector3 p1, p2, p3;

	for( u32 j=0 ; j<NumIndices ; j++ )
	{
		//	�ʒ��_�擾
		int a = pIndices[j*3+0] * VertexSize;
		v1.x = pVertices[a];	v1.y = pVertices[a+1];	v1.z = pVertices[a+2];

		int b = pIndices[j*3+1] * VertexSize;
		v2.x = pVertices[b];	v2.y = pVertices[b+1];	v2.z = pVertices[b+2];

		int c = pIndices[j*3+2] * VertexSize;
		v3.x = pVertices[c];	v3.y = pVertices[c+1];	v3.z = pVertices[c+2];
		
		//	��������
		//Vector3	ss = (v1 + v2 + v3) / 3.0f - p;
		//if( ss.LengthSq() > dist ) continue;
		l1.x = v2.x - v1.x;
		l1.y = v2.y - v1.y;
		l1.z = v2.z - v1.z;
		l2.x = v3.x - v2.x;
		l2.y = v3.y - v2.y;
		l2.z = v3.z - v2.z;
		
		//	�O�ςɂ��@���Z�o		
		Vector3Cross( n, l1, l2 );
		//	���ς̌��ʂ��v���X�Ȃ�Η�����
		float dot = Vector3Dot( vv, n );
		if( dot >= 0 ) continue;
		//	��_�Z�o
		p1.x = v1.x - p.x;
		p1.y = v1.y - p.y;
		p1.z = v1.z - p.z;
		float t = Vector3Dot( n, p1 ) / dot;
		if( t < .0f || t > neart ) continue;

		cp.x = vv.x*t + p.x;
		cp.y = vv.y*t + p.y;
		cp.z = vv.z*t + p.z;
		//	���_����
		p1.x = v1.x - cp.x;
		p1.y = v1.y - cp.y;
		p1.z = v1.z - cp.z;
		
		Vector3Cross( temp, p1, l1 );
		if( Vector3Dot(temp, n) < .0f ) continue;

		p2.x = v2.x - cp.x;
		p2.y = v2.y - cp.y;
		p2.z = v2.z - cp.z;
		Vector3Cross( temp, p2, l2 );
		if( Vector3Dot(temp, n) < .0f ) continue;

		l3.x = v1.x - v3.x;
		l3.y = v1.y - v3.y;
		l3.z = v1.z - v3.z;
		p3.x = v3.x - cp.x;
		p3.y = v3.y - cp.y;
		p3.z = v3.z - cp.z;
		Vector3Cross( temp, p3, l3 );
		if( Vector3Dot(temp, n) < .0f ) continue;

		*out = cp;
		*vec = n;
		ret = j;
		neart = t;
	}
	lpMesh->UnlockVertexBuffer();
	lpMesh->UnlockIndexBuffer();
	*Dist = neart;

	return	ret;
}

//------------------------------------------------------
//		�㉺�œK��
//------------------------------------------------------
int	iexMesh::RayPickUD( Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	float	t, neart;
	float	vy;
	int		ret = -1;

	int		VertexSize;

	Vector3	p = *pos;
	vy = vec->y;

	neart = *Dist;

	//	���擾	
	int fvf = lpMesh->GetFVF();
	//	���_�T�C�Y�v�Z
	VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);

	//	�o�b�t�@���b�N
	float	*pVertices;
	u16		*pIndices;
	int NumIndices = lpMesh->GetNumFaces();
	lpMesh->LockVertexBuffer( D3DLOCK_READONLY , (void**)&pVertices );
	lpMesh->LockIndexBuffer( D3DLOCK_READONLY , (void**)&pIndices );

	Vector	l1, l2, l3;
	Vector	p1, p2, p3;
	Vector v[3];
	Vector n;

	for( int j=0 ; j<NumIndices ; j++ )
	{
		//	�ʒ��_�擾
		int a = pIndices[j*3+0] * VertexSize;
		int b = pIndices[j*3+1] * VertexSize;
		int c = pIndices[j*3+2] * VertexSize;

		v[0].x = pVertices[a];	v[1].x = pVertices[b];	v[2].x = pVertices[c];
		if( v[0].x > p.x && v[1].x > p.x && v[2].x > p.x ) continue;

		v[0].z = pVertices[a+2];	v[1].z = pVertices[b+2];	v[2].z = pVertices[c+2];
		if( v[0].z > p.z && v[1].z > p.z && v[2].z > p.z ) continue;

		v[0].y = pVertices[a+1];	v[1].y = pVertices[b+1];	v[2].y = pVertices[c+1];

		//	���_����i�S�O�ς��}�C�i�X�j		
		l1.x = v[1].x - v[0].x;
		l1.z = v[1].z - v[0].z;
		p1.x = v[0].x - p.x;
		p1.z = v[0].z - p.z;
		if( (p1.x*l1.z - p1.z*l1.x)*vy < 0 ) continue;

		l2.x = v[2].x - v[1].x;
		l2.z = v[2].z - v[1].z;
		p2.x = v[1].x - p.x;
		p2.z = v[1].z - p.z;
		if( (p2.x*l2.z - p2.z*l2.x)*vy < 0 ) continue;

		l3.x = v[0].x - v[2].x;
		l3.z = v[0].z - v[2].z;
		p3.x = v[2].x - p.x;
		p3.z = v[2].z - p.z;
		if( (p3.x*l3.z - p3.z*l3.x)*vy < 0 ) continue;

		//	�O�ςɂ��@���Z�o		
		l1.y = v[1].y - v[0].y;
		l2.y = v[2].y - v[1].y;
		Vector3Cross( n, l1, l2 );
		//	�\������
		if( vy*n.y >= 0 ) continue;

		//	��_�Z�o
		p1.y = v[0].y - p.y;
		t = Vector3Dot( n, p1 ) / (n.y*vy);
		if( t < .0f || t > neart ) continue;

		*vec = n;
		ret = j;
		neart = t;
	}
	lpMesh->UnlockVertexBuffer();
	lpMesh->UnlockIndexBuffer();

	out->y = neart*vy + p.y;
	out->x = pos->x;
	out->z = pos->z;
	*Dist = neart;

	return	ret;
}

//**************************************************************************************************
//
//	�t�@�C���ǂݍ���
//
//**************************************************************************************************

//	IMO�I�u�W�F�N�g
typedef struct tagIMOOBJ {
	DWORD		id;	
	int			type;
	DWORD		NumVertex;
	LPLVERTEX	lpVertex;

	DWORD		NumFace;
	LPWORD		lpFace;
	LPDWORD		lpAtr;

	DWORD			NumMaterial;	//	�}�e���A����
	D3DMATERIAL9	Material[32];	//	�}�e���A��
	char			Texture[32][32];//	�e�N�X�`���t�@�C��

} IMOOBJ, *LPIMOOBJ;

//------------------------------------------------------
//	�h�l�n�ǂݍ���
//------------------------------------------------------
BOOL iexMesh::LoadIMO( LPSTR filename )
{
	IMOOBJ		imo;
	DWORD		i;
	//	�t�@�C���ǂݍ���
	HANDLE	hfile;
	DWORD	dum;

	int		version = 1;

	hfile = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL );
	if( hfile == INVALID_HANDLE_VALUE ) return FALSE;

	ReadFile( hfile, &imo, sizeof(IMOOBJ), &dum, NULL );
	if( imo.id == '2OMI' ) version = 2;

	//	���b�V���쐬
	BYTE	*pVertex, *pFace;
	DWORD	*pData;

	LVERTEX*	workV = new LVERTEX[imo.NumVertex];
	MESHVERTEX2*	workV2 = new MESHVERTEX2[imo.NumVertex];
	WORD*		workF = new WORD[imo.NumFace*3];

	if( version == 1 )
	{
		//	���_�ǂݍ���
		ReadFile( hfile, workV, sizeof(LVERTEX)*imo.NumVertex, &dum, NULL );
		//	�C���f�b�N�X�ǂݍ���
		ReadFile( hfile, workF, sizeof(WORD)*imo.NumFace*3, &dum, NULL );
		
		for (DWORD i = 0; i < imo.NumVertex; i++)
		{
			workV2[i].x = workV[i].x;
			workV2[i].y = workV[i].y;
			workV2[i].z = workV[i].z;
			workV2[i].tu = workV[i].tu;
			workV2[i].tv = workV[i].tv;
			workV2[i].color = workV[i].color;
			workV2[i].nx = .0f;
			workV2[i].ny = .0f;
			workV2[i].nz = .0f;
		}

		for (DWORD i = 0; i < imo.NumFace; i++)
		{
			//	�@���v�Z
			Vector3 v1, v2, n;
			v1.x = workV[workF[i * 3 + 0]].x - workV[workF[i * 3 + 1]].x;
			v1.y = workV[workF[i * 3 + 0]].y - workV[workF[i * 3 + 1]].y;
			v1.z = workV[workF[i * 3 + 0]].z - workV[workF[i * 3 + 1]].z;
			
			v2.x = workV[workF[i * 3 + 1]].x - workV[workF[i * 3 + 2]].x;
			v2.y = workV[workF[i * 3 + 1]].y - workV[workF[i * 3 + 2]].y;
			v2.z = workV[workF[i * 3 + 1]].z - workV[workF[i * 3 + 2]].z;

			n.x = (v1.y*v2.z - v1.z*v2.y);
			n.y = (v1.z*v2.x - v1.x*v2.z);
			n.z = (v1.x*v2.y - v1.y*v2.x);
			float d = sqrtf( n.x*n.x + n.y*n.y + n.z*n.z );
			n.x /= d;
			n.y /= d;
			n.z /= d;

			workV2[workF[i * 3 + 0]].nx += n.x;
			workV2[workF[i * 3 + 0]].ny += n.y;
			workV2[workF[i * 3 + 0]].nz += n.z;
			workV2[workF[i * 3 + 1]].nx += n.x;
			workV2[workF[i * 3 + 1]].ny += n.y;
			workV2[workF[i * 3 + 1]].nz += n.z;
			workV2[workF[i * 3 + 2]].nx += n.x;
			workV2[workF[i * 3 + 2]].ny += n.y;
			workV2[workF[i * 3 + 2]].nz += n.z;
		}
		for (DWORD i = 0; i < imo.NumVertex; i++)
		{
			float d = sqrtf( workV2[i].nx*workV2[i].nx + workV2[i].ny*workV2[i].ny + workV2[i].nz*workV2[i].nz );
			workV2[i].nx /= d;
			workV2[i].ny /= d;
			workV2[i].nz /= d;
		}
	} else {
		//	���_�ǂݍ���
		ReadFile( hfile, workV2, sizeof(MESHVERTEX2)*imo.NumVertex, &dum, NULL );
		//	�C���f�b�N�X�ǂݍ���
		ReadFile( hfile, workF, sizeof(WORD)*imo.NumFace*3, &dum, NULL );
	}

	DWORD	Declaration = D3DFVF_MESHVERTEX2;
	D3DXCreateMeshFVF( imo.NumFace, imo.NumVertex, D3DXMESH_MANAGED, Declaration, tdnSystem::GetDevice(), &lpMesh );
	//	���_�ݒ�
	lpMesh->LockVertexBuffer( 0, (void**)&pVertex );
	memcpy( pVertex, workV2, sizeof(MESHVERTEX2)*imo.NumVertex );
	lpMesh->UnlockVertexBuffer();

	//	�ʐݒ�
	lpMesh->LockIndexBuffer( 0, (void**)&pFace );
	memcpy( pFace, workF, sizeof(WORD)*imo.NumFace*3 );
	lpMesh->UnlockIndexBuffer();

	//	�����ݒ�
	lpMesh->LockAttributeBuffer( 0, &pData );
	ReadFile( hfile, pData, sizeof(DWORD)*imo.NumFace, &dum, NULL );
	lpMesh->UnlockAttributeBuffer();

	//	�p�X����
	char	workpath[MAX_PATH];
	CopyMemory( workpath, filename, lstrlen(filename)+1 );
	for( i=lstrlen(filename) ; i>0 ; i-- ){
		if( IsDBCSLeadByte(workpath[i-2]) ){
			i--;
			continue;
		}
		if( workpath[i-1] == '\\' || workpath[i-1] == '/' ){
			workpath[i] = '\0';
			break;
		}
	}

	//	�}�e���A���ݒ�
	MaterialCount = imo.NumMaterial;
	lpMaterial = new D3DMATERIAL9[ MaterialCount ];
	CopyMemory( lpMaterial, imo.Material, sizeof(D3DMATERIAL9)*imo.NumMaterial );
	//	�e�N�X�`���ݒ�
	lpTexture  = new Texture2D* [ imo.NumMaterial ];
	lpNormal   = new Texture2D* [ imo.NumMaterial ];
	lpSpecular = new Texture2D* [ imo.NumMaterial ];
	lpHeight   = new Texture2D* [ imo.NumMaterial ];

	char temp[256];
	for( i=0 ; i<MaterialCount ; i++ ){
		lpTexture[i]  = NULL;
		lpNormal[i]   = NULL;
		lpSpecular[i] = NULL;
		lpHeight[i]   = NULL;

		if( imo.Texture[i][0] == '\0' ) continue;
		//	�e�N�X�`���ǂݍ���
		sprintf( temp, "%s%s", workpath, imo.Texture[i] );
		lpTexture[i] = tdnTexture::Load( temp );

		sprintf( temp, "%sN%s", workpath, imo.Texture[i] );
		lpNormal[i] = tdnTexture::Load( temp );

		sprintf( temp, "%sS%s", workpath, imo.Texture[i] );
		lpSpecular[i] = tdnTexture::Load( temp );

		sprintf( temp, "%sH%s", workpath, imo.Texture[i] );
		lpHeight[i] = tdnTexture::Load( temp, 1 );
	}
	CloseHandle(hfile);

	delete[] workV;
	delete[] workV2;
	delete[] workF;

	//	�����ݒ�
	SetPos(0,0,0);
	SetAngle(0.0f);
	SetScale(1.0f);
	dwFlags = 0;

	bLoad = TRUE;

	return TRUE;
}

//------------------------------------------------------
//	�w�t�@�C���ǂݍ���
//------------------------------------------------------
BOOL	iexMesh::LoadX( LPSTR filename )
{
	HRESULT hr;
	LPD3DXBUFFER	lpD3DXMtrlBuffer;

	//	�p�X����
	char	workpath[MAX_PATH];
	CopyMemory( workpath, filename, lstrlen(filename)+1 );
	for( int i=lstrlen(filename) ; i>0 ; i-- ){
		if( IsDBCSLeadByte(workpath[i-2]) ){
			i--;
			continue;
		}
		if( workpath[i-1] == '\\' || workpath[i-1] == '/' ){
			workpath[i] = '\0';
			break;
		}
	}

	// X �t�@�C���̃��[�h
	hr = D3DXLoadMeshFromX( filename, D3DXMESH_MANAGED, tdnSystem::GetDevice(),NULL, &lpD3DXMtrlBuffer, NULL, &MaterialCount, &lpMesh );
 	if( FAILED( hr ) ) return FALSE;

	// lpD3DXMtrlBuffer����A������e�N�X�`���[�̏����擾����
	D3DXMATERIAL* d3dxMaterial = (D3DXMATERIAL *)lpD3DXMtrlBuffer->GetBufferPointer();

	// �e�N�X�`�����}�e���A���p�o�b�t�@����
	lpMaterial = new D3DMATERIAL9 [ MaterialCount ];
	lpTexture  = new LPDIRECT3DTEXTURE9 [ MaterialCount ];
	lpNormal   = new Texture2D* [ MaterialCount ];
	lpSpecular = new Texture2D* [ MaterialCount ];
	lpHeight   = new Texture2D* [ MaterialCount ];

	// �f�[�^�ǂݍ���
	for( u32 i=0 ; i<MaterialCount ; i++ ){
		// �}�e���A��
		lpMaterial[i] = d3dxMaterial[i].MatD3D;
		lpMaterial[i].Ambient.r = lpMaterial[i].Diffuse.r;
		lpMaterial[i].Ambient.g = lpMaterial[i].Diffuse.g;
		lpMaterial[i].Ambient.b = lpMaterial[i].Diffuse.b;
		// �e�N�X�`��
		char temp[256];
		lpTexture[i]  = NULL;
		lpNormal[i]   = NULL;
		lpSpecular[i] = NULL;
		lpHeight[i]   = NULL;

		if( d3dxMaterial[i].pTextureFilename != NULL ){
			//	�e�N�X�`���ǂݍ���
			sprintf( temp, "%s%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpTexture[i] = tdnTexture::Load( temp );

			sprintf( temp, "%sN%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpNormal[i] = tdnTexture::Load( temp );

			sprintf( temp, "%sS%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpSpecular[i] = tdnTexture::Load( temp );

			sprintf( temp, "%sH%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpHeight[i] = tdnTexture::Load( temp, 1 );
		}
	}
	lpD3DXMtrlBuffer->Release();

	//	�����ݒ�
	SetPos(0,0,0);
	SetAngle(0.0f);
	SetScale(1.0f);
	dwFlags = 0;

	return TRUE;
}








//*****************************************************************************
//
//	�ߋ��̂h�d�w�Ƃ̌݊��p
//
//*****************************************************************************
//------------------------------------------------------
//		���ݒ�
//------------------------------------------------------
void	IEX_SetMeshPos( iexMesh* lpMesh, float x, float y, float z )
{
	if( !lpMesh ) return;
	lpMesh->SetPos( x, y, z );
	lpMesh->Update();
}

void	IEX_SetMeshAngle( iexMesh* lpMesh, float x, float y, float z )
{
	if( !lpMesh ) return;
	lpMesh->SetAngle( x, y, z );
	lpMesh->Update();
}

void	IEX_SetMeshScale( iexMesh* lpMesh, float x, float y, float z )
{
	if( !lpMesh ) return;
	lpMesh->SetScale( x, y, z );
	lpMesh->Update();
}

void	IEX_SetMeshScale( iexMesh* lpMesh, float scale )
{
	if( !lpMesh ) return;
	lpMesh->SetScale( scale );
	lpMesh->Update();
}

//*****************************************************************************
//
//		���b�V���̃����_�����O
//
//*****************************************************************************

void	IEX_RenderMesh( iexMesh* lpMesh, u32 dwFlags, float param )
{
	if( !lpMesh ) return;
	lpMesh->Render(dwFlags, param);
}


void	IEX_RenderMesh( iexMesh* lpMesh, tdnShader* shader, char* name )
{
	if( !lpMesh ) return;
	lpMesh->Render( shader, name);
}

void	IEX_RenderMesh( iexMesh* lpMesh )
{
	IEX_RenderMesh( lpMesh, RS::COPY, -1.0f );
}

//*****************************************************************************
//
//
//
//*****************************************************************************

//------------------------------------------------------
//		�w�t�@�C�����b�V���쐬
//------------------------------------------------------

iexMesh*	IEX_LoadMeshFromX( LPSTR filename )
{
	iexMesh*	mesh = new iexMesh(filename);
	return mesh;
}

//------------------------------------------------------
//		iMo�t�@�C��
//------------------------------------------------------
iexMesh*	IEX_LoadIMO( LPSTR filename )
{
	iexMesh*	mesh = new iexMesh(filename);
	return mesh;
}

//*****************************************************************************
//
//
//
//*****************************************************************************

//
//		���b�V���̉��
//

void	IEX_ReleaseMesh( iexMesh* lpMesh )
{
	if( !lpMesh ) return;
	delete	lpMesh;
}

//*****************************************************************************
//
//		RayPick
//
//*****************************************************************************
 
//
//		���C�s�b�N�i�^��E�^���j
//

int	IEX_RayPickMeshUD( iexMesh* lpMesh, Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	int ret = lpMesh->RayPickUD( out, pos, vec, Dist );
	return	ret;
}

//
//		���C�s�b�N�i�ʏ�j
//

int	IEX_RayPickMesh( iexMesh* lpMesh, Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	const Matrix *TransMatrix = &lpMesh->TransMatrix;
	Matrix InverseMatrix;
	D3DXMatrixInverse( &InverseMatrix, 0, TransMatrix );

	Vector3 raypos;
	raypos.x = pos->x * InverseMatrix._11 + pos->y * InverseMatrix._21 + pos->z * InverseMatrix._31 + InverseMatrix._41;
	raypos.x = pos->x * InverseMatrix._11 + pos->y * InverseMatrix._21 + pos->z * InverseMatrix._31 + InverseMatrix._41;
	raypos.x = pos->x * InverseMatrix._11 + pos->y * InverseMatrix._21 + pos->z * InverseMatrix._31 + InverseMatrix._41;

	Vector3 rayvec;
	rayvec.x = vec->x * InverseMatrix._11 + vec->y * InverseMatrix._21 + vec->z * InverseMatrix._31;
	rayvec.x = vec->x * InverseMatrix._11 + vec->y * InverseMatrix._21 + vec->z * InverseMatrix._31;
	rayvec.x = vec->x * InverseMatrix._11 + vec->y * InverseMatrix._21 + vec->z * InverseMatrix._31;
	rayvec.Normalize();

	int ret = lpMesh->RayPick( out, &raypos, &rayvec, Dist );
	if( ret < 0 )
		return ret;

	raypos.x = pos->x * TransMatrix->_11 + pos->y * TransMatrix->_21 + pos->z * TransMatrix->_31 + TransMatrix->_41;
	raypos.x = pos->x * TransMatrix->_11 + pos->y * TransMatrix->_21 + pos->z * TransMatrix->_31 + TransMatrix->_41;
	raypos.x = pos->x * TransMatrix->_11 + pos->y * TransMatrix->_21 + pos->z * TransMatrix->_31 + TransMatrix->_41;

	rayvec.x = vec->x * TransMatrix->_11 + vec->y * TransMatrix->_21 + vec->z * TransMatrix->_31;
	rayvec.x = vec->x * TransMatrix->_11 + vec->y * TransMatrix->_21 + vec->z * TransMatrix->_31;
	rayvec.x = vec->x * TransMatrix->_11 + vec->y * TransMatrix->_21 + vec->z * TransMatrix->_31;
	rayvec.Normalize();

	return	ret;
}



// �ǉ������֐�
void iexMesh::NearestPoint( NearestPointOut *out, const Vector3 &inPos )
{
	//	���擾	
	u32 fvf = lpMesh->GetFVF();
	//	���_�T�C�Y�v�Z
	int vertexSize = D3DXGetFVFVertexSize( fvf ) / sizeof( float );
	//	�o�b�t�@���b�N
	float	*pVertices;
	u16		*pIndices;
	u32 numIndices = lpMesh->GetNumFaces();
	lpMesh->LockVertexBuffer( D3DLOCK_READONLY, ( void** ) &pVertices );
	lpMesh->LockIndexBuffer( D3DLOCK_READONLY, ( void** ) &pIndices );

	struct
	{
		Vector3 vertex[3];
		Vector3	normal;
		Vector3 line[3];

		// ���_��񂩂�c��̏����v�Z
		inline void ComputeFromVertex()
		{
			line[1] = vertex[2] - vertex[1];
			line[2] = vertex[3] - vertex[2];
			line[3] = vertex[1] - vertex[3];

			Vector3Cross( normal, line[1], line[2] );
			normal.Normalize();
		}
	}triangle; // �O�p�|���S��

	out->length = FLT_MAX;

	for( u32 j = 0; j < numIndices; j++ )
	{
		//	�ʒ��_�擾
		{
			int index = pIndices[j * 3 + 0] * vertexSize;
			triangle.vertex[1].x = pVertices[index];	triangle.vertex[1].y = pVertices[index + 1];	triangle.vertex[1].z = pVertices[index + 2];

			index = pIndices[j * 3 + 1] * vertexSize;
			triangle.vertex[2].x = pVertices[index];	triangle.vertex[2].y = pVertices[index + 1];	triangle.vertex[2].z = pVertices[index + 2];

			index = pIndices[j * 3 + 2] * vertexSize;
			triangle.vertex[3].x = pVertices[index];	triangle.vertex[3].y = pVertices[index + 1];	triangle.vertex[3].z = pVertices[index + 2];
		}

		// �@���ƕӌv�Z
		triangle.ComputeFromVertex();

		// �@�������ɋ����v�Z
		FLOAT lengthNormal( 0 );
		lengthNormal = Vector3Dot( ( inPos - triangle.vertex[1] ), triangle.normal );

		// �|���S����������
		if( lengthNormal <= 0 )
			continue;

		// ����
		if( lengthNormal > out->length )
			continue;

		Vector3 nearestPos( 0, 0, 0 );
		// �Ƃ肠�����������ʏ��
		nearestPos = inPos - ( triangle.normal * lengthNormal );

		Vector3 decisionVector( 0, 0, 0 );
		// ���_����
		for( unsigned int i = 0; i < 3; i++ )
		{
			Vector3Cross( decisionVector, ( triangle.vertex[i] - nearestPos ), triangle.line[i] );
			// �O�ɂ������ꍇ
			if( Vector3Dot( decisionVector, triangle.normal ) < 0 )
			{
				// nearestPos��ӏ�̍Őڋߓ_�ֈړ�
				float t = triangle.line[i].LengthSq();
				if( t != 0 ) // �ӏ��
					t = Vector3Dot( ( nearestPos - triangle.vertex[i] ), triangle.line[i] ) / t;
				// ���_��
				if( t < 0 ) t = 0;
				else if( t > 1 )t = 1;
				// �ړ�
				nearestPos = triangle.vertex[i] + triangle.line[i] * t;
			}
		}

		// ��������
		float length = ( nearestPos - inPos ).Length();
		if( length > out->length )
			continue;


		out->length = length;
		out->Normal = triangle.normal;
		out->Pos = nearestPos;
	}

	//	�o�b�t�@�A�����b�N
	lpMesh->UnlockVertexBuffer();
	lpMesh->UnlockIndexBuffer();
}

float iexMesh::Length_of_furthest_point()
{
	//	���擾	
	u32 fvf = lpMesh->GetFVF();
	//	���_�T�C�Y�v�Z
	int vertexSize = D3DXGetFVFVertexSize( fvf ) / sizeof( float );
	//	�o�b�t�@���b�N
	float	*pVertices;
	u32 numVertices = lpMesh->GetNumVertices();
	lpMesh->LockVertexBuffer( D3DLOCK_READONLY, ( void** ) &pVertices );

	float length( 0 );
	Vector3 vertex( 0, 0, 0 );
	for( u32 index = 0; index < numVertices; index += 3 )
	{
		vertex.x = pVertices[index] * vertexSize; vertex.y = pVertices[index + 1] * vertexSize; vertex.z = pVertices[index + 2] * vertexSize;
		length = max( length, vertex.LengthSq() );
	}

	lpMesh->UnlockVertexBuffer();

	return sqrtf( length );
}
