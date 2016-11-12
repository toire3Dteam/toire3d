#include "TDNLIB.h"

void tdnBone::Slerp( const tdnBone& pose1, const tdnBone& pose2, float rate, D3DXMATRIX* out )
{
	float riv = 1.0f - rate;

	D3DXVECTOR3 scale, translate;
	D3DXQUATERNION rotate;

	// 線形補間
	scale = pose1.scale * riv + pose2.scale * rate;
	D3DXQuaternionSlerp( &rotate, &pose1.rotate, &pose2.rotate, rate );
	translate = pose1.translate * riv + pose2.translate * rate;

	// 合成
	CreateSRTMatrix( scale, rotate, translate, out );
}

void tdnBone::SetSRTMatrix( const D3DXMATRIX& in )
{
	translate = D3DXVECTOR3( in._41, in._42, in._43 );

	D3DXMATRIX workMat = in;
	workMat._41 = workMat._42 = workMat._43 = 0;
	D3DXQUATERNION workRot;
	D3DXQuaternionRotationMatrix( &workRot, &workMat );
	D3DXQuaternionNormalize( &workRot, &workRot );
	rotate = workRot;

	D3DXQuaternionInverse( &workRot, &workRot );
	D3DXMatrixRotationQuaternion( &workMat, &workRot );
	workMat = workMat * in;

	scale.x = D3DXVec3Length( &D3DXVECTOR3( workMat._11, workMat._12, workMat._13 ) );
	scale.y = D3DXVec3Length( &D3DXVECTOR3( workMat._21, workMat._22, workMat._23 ) );
	scale.z = D3DXVec3Length( &D3DXVECTOR3( workMat._31, workMat._32, workMat._33 ) );
}

void tdnBone::GetSRTMatrix( D3DXMATRIX* out )
{
	CreateSRTMatrix( scale, rotate, translate, out );
}

void tdnBone::CreateSRTMatrix( const D3DXVECTOR3 & scale, const D3DXQUATERNION & rotate, const D3DXVECTOR3 & translate, D3DXMATRIX * out )
{
	D3DXMATRIX work;
	D3DXMatrixScaling( out, scale.x, scale.y, scale.z );
	D3DXMatrixRotationQuaternion( &work, &rotate );

	*out = work * *out;

	out->_41 = translate.x;
	out->_42 = translate.y;
	out->_43 = translate.z;
}





tdnSkinAnimation::tdnSkinAnimation()
{}

void tdnSkinAnimation::Resize( unsigned int numFrame, unsigned int numBone )
{
	this->numFrame = numFrame;
	this->numBone = numBone;

	currentBone.boneArray.resize( numBone );
	currentBone.boneArray.reserve( numBone );
	poseArray.resize( numFrame );
	poseArray.reserve( numFrame );
	for ( size_t i = 0; i < poseArray.size(); i++ )
	{
		poseArray[i].boneArray.resize( numBone );
		poseArray[i].boneArray.reserve( numBone );
	}
}





tdnAnimationFrame::tdnAnimationFrame() : frame( 0 ), command( 0 )
{}

tdnAnimationFrame::~tdnAnimationFrame()
{
	for ( auto it : comArray ) delete it;
}

bool tdnAnimationFrame::Load( const char * filename )
{
	std::ifstream ifs( filename );
	if ( !ifs )
		return false;

	for ( auto it : comArray ) delete it;
	comArray.clear();

	int index, start, end, nextCom;
	while ( !ifs.eof() )
	{
		ifs >> index;
		ifs >> start;
		ifs >> end;
		ifs >> nextCom;
		AddCommand( new ComJump( start, end, nextCom ) );
	}

	comArray.reserve( comArray.size() );

	return true;
}

void tdnAnimationFrame::OutPut( const char * filename )
{
	std::ofstream ofs( filename );
	if ( !ofs )
		return;

	for ( int i = 0; i < ( int ) comArray.size(); i++ )
	{
		ofs << std::endl;
		ofs << i << ' ';
		ofs << comArray[i]->start << ' ';
		ofs << comArray[i]->end << ' ';
		ofs << comArray[i]->nextCom;
	}
}

void tdnAnimationFrame::AddCommand( ComJump * com )
{
	comArray.push_back( com );
}

void tdnAnimationFrame::Select( int comNum, bool waitAnimEnd )
{
	if ( 0 <= comNum && comNum < ( int ) comArray.size() )
	{
		command = comNum;
		if ( waitAnimEnd == false )
		{
			frame = ( float ) comArray[command]->start;
		}
	}
}

void tdnAnimationFrame::AddFrame( float add )
{
	SetFrame( frame + add );
}

float tdnAnimationFrame::GetFrame()
{
	return frame;
}

void tdnAnimationFrame::Update()
{
	if ( command < ( int ) comArray.size() )
	{
		int nextCom( 0 );
		while ( true )
		{
			nextCom = comArray[command]->Check( &frame );
			if ( nextCom < 0 )
				break;

			command = nextCom;
			comArray[command]->Start( &frame );
		}
	}
}

void tdnAnimationFrame::SetFrame( float frame )
{
	this->frame = frame;
}

int tdnAnimationFrame::GetCommandId()
{
	if ( command >= ( int ) comArray.size() )
		return -1;
	return command;
}

int tdnAnimationFrame::GetStartFrame()
{
	if ( command >= ( int ) comArray.size() )
		return -1;
	return comArray[command]->start;
}

int tdnAnimationFrame::GetEndFrame()
{
	if ( command >= ( int ) comArray.size() )
		return -1;
	return comArray[command]->end;
}

int tdnAnimationFrame::GetNextCom()
{
	if ( command >= ( int ) comArray.size() )
		return -1;
	return comArray[command]->nextCom;
}

int tdnAnimationFrame::GetNumCom()
{
	return ( int ) comArray.size();
}

void tdnAnimationFrame::SetStartFrame( int frame )
{
	if ( command >= ( int ) comArray.size() )
		return;
	if ( frame < 0 || frame >= comArray[command]->end )
		return;
	comArray[command]->start = frame;
}

void tdnAnimationFrame::SetEndFrame( int frame )
{
	if ( command >= ( int ) comArray.size() )
		return;
	if ( frame < 0 || frame <= comArray[command]->start )
		return;
	comArray[command]->end = frame;
}

void tdnAnimationFrame::SetNextCom( int com )
{
	if ( command >= ( int ) comArray.size() )
		return;
	if ( com < 0 || com >= ( int ) comArray.size() )
		return;
	comArray[command]->nextCom = com;
}

void tdnAnimationFrame::EraseCom()
{
	if ( command >= ( int ) comArray.size() )
		return;
	delete comArray[command];
	comArray.erase( comArray.begin() + command );
	command -= 1;
	// -1
	if ( command < 0 )
		command = 0;
}

tdnAnimationFrame::ComJump::ComJump( int start, int end, int nextCom ) : start( start ), end( end ), nextCom( nextCom ) {}

int tdnAnimationFrame::ComJump::Check( float* frame )
{
	if ( *frame >= end )
	{
		*frame -= end;
		return nextCom;
	}
	return -1;
}

void tdnAnimationFrame::ComJump::Start( float* frame )
{
	*frame += start;
}





tdnSkinMesh::tdnSkinMesh() : tdnMesh(), numBone( 0 )
{}

tdnSkinMesh::~tdnSkinMesh()
{
	delete boneChain;
	delete animation;
	delete[] boneArray;
}

bool tdnSkinMesh::CreateWeight( WeightCreateData& weights )
{
	HRESULT hr;

	numBone = weights.bonesWeight.size();

	CreateSkinInfo();

	delete[]boneArray;
	boneArray = new D3DXMATRIX[numBone];
	for ( int i = 0; i < numBone; i++ )
		D3DXMatrixIdentity( &boneArray[i] );

	// skinInfo にウェイト設定
	for ( int i = 0; i < numBone; i++ )
	{
		if ( weights.bonesWeight[i].vertices.size() <= 0 )
			continue;
		hr = skinInfo->SetBoneInfluence( i, weights.bonesWeight[i].vertices.size(), weights.bonesWeight[i].vertices.data(), weights.bonesWeight[i].weights.data() );
		if ( FAILED( hr ) )
		{
			SAFE_RELEASE( skinInfo );
			return false;
		}
	}

	return true;
}

bool tdnSkinMesh::CreateSkinInfo()
{
	// element 抽出
	UINT numElement( 0 );
	HRESULT hr = decl->GetDeclaration( NULL, &numElement );
	D3DVERTEXELEMENT9* element = new D3DVERTEXELEMENT9[numElement];

	hr = decl->GetDeclaration( element, &numElement );
	if ( FAILED( hr ) )
	{
		delete[] element;
		return false;
	}

	// skinInfo を作成
	SAFE_RELEASE( skinInfo );
	hr = D3DXCreateSkinInfo( numVertexes, element, numBone, &skinInfo );
	if ( FAILED( hr ) )
	{
		delete[] element;
		SAFE_RELEASE( skinInfo );
		return false;
	}
	delete[] element;

	return true;
}

void tdnSkinMesh::SetBoneChain( tdnBoneChain* chain )
{
	if ( boneChain )
		delete boneChain;
	boneChain = chain;
	// 一番上にいるボーンインデックス保存
	for ( int i = 0; i < numBone; i++ )
	{
		if ( boneChain->bones[i].parent == -1 )
		{
			rootBones.push_back( i );
		}
	}
}

bool tdnSkinMesh::UpdateSkin()
{
	if ( vertexBuffer == nullptr )
		return false;

	MESHVERTEX2* work;
	vertexBuffer->Lock( 0, sizeof( MESHVERTEX2 )*numVertexes, ( void** ) &work, D3DLOCK_DISCARD );
	skinInfo->UpdateSkinnedMesh( boneArray, NULL, vertexArray, work );
	vertexBuffer->Unlock();
	return true;
}

void tdnSkinMesh::UpdateBoneMatrix()
{
	for ( auto index : rootBones )
	{
		UpdateBoneChain( index );
	}
}

void tdnSkinMesh::SetCurPose()
{
	MESHVERTEX2* work;
	vertexBuffer->Lock( 0, sizeof( MESHVERTEX2 )*numVertexes, ( void** ) &work, D3DLOCK_DISCARD );
	memcpy_s( work, sizeof( MESHVERTEX2 )*numVertexes, vertexArray, sizeof( MESHVERTEX2 )*numVertexes );
	vertexBuffer->Unlock();
}

std::string tdnSkinMesh::GetBoneName( int index )
{
	if ( index < 0 || ( int ) animation->currentBone.boneArray.size() <= index )
		return "";

	for ( auto itr = boneChain->boneName.begin(); itr != boneChain->boneName.end(); itr++ )
	{
		if ( itr->second == index )
		{
			return itr->first;
		}
	}

	return "";
}

void tdnSkinMesh::GetBone( std::string boneName, Matrix *outMat )
{
	assert( this->boneChain != nullptr );
	auto index = this->boneChain->boneName.find( boneName );
	if ( index != this->boneChain->boneName.end() )
	{
		GetBone( index->second, outMat );
	}
}

bool tdnSkinMesh::GetBone( int index, Matrix *outMat )
{
	if ( index < 0 || ( int ) animation->currentBone.boneArray.size() <= index )
		return false;

	animation->currentBone.boneArray[index].GetSRTMatrix( outMat );
	//*outMat *= boneArray[index];
	*outMat = *outMat * boneArray[index] * worldMatrix;
	return true;
}

bool tdnSkinMesh::GetCureBone( int index, Matrix* outMat )
{
	if ( index < 0 || ( int ) animation->currentBone.boneArray.size() <= index )
		return false;

	animation->currentBone.boneArray[index].GetSRTMatrix( outMat );
	return true;
}

void tdnSkinMesh::SetCureBone( int index, Matrix &inMat )
{
	if ( index < 0 || ( int ) animation->currentBone.boneArray.size() <= index )
		return;

	animation->currentBone.boneArray[index].SetSRTMatrix( inMat );
}

bool tdnSkinMesh::AddFrame( float in )
{
	return SetFrame( frame + in );
}

bool tdnSkinMesh::SetFrame( float in )
{
	if ( !animation )
		return false;

	frame = in;
	if ( frame > animation->numFrame - 2 )
	{
		// 最大フレームを超えた場合
		frame = ( float ) ( animation->numFrame - 2 );
		return false;
	}

	return true;
}

float tdnSkinMesh::GetFrame()
{
	return this->frame;
}

void tdnSkinMesh::OutPutTDNSM( const char* fileName )
{
	FILE* file( nullptr );
	//char filePath[MAX_PATH] = {};
	// 拡張子付与
	//strcat_s( filePath, fileName );
	//strcat_s( filePath, ".TDNSM" );
	fopen_s( &file, fileName, "wb" );
	if ( !file )
	{
		std::string err;
		err = fileName;
		err += " にファイルを作成できません";
		MessageBox( NULL, err.c_str(), "エラー", 0 );
		return;
	}

	OutPutTDNM( file );
	OutPutTDNSM( file );

	fclose( file );
}

void tdnSkinMesh::OutPutTDNSM( FILE* file )
{
	HRESULT hr( S_OK );

	DWORD* vertices;
	FLOAT* weights;
	DWORD numBoneInfluences( 0 );

	fwrite( &numBone, sizeof( numBone ), 1, file );

	// ボーンチェーン
	std::map<int, std::string > boneName;
	for ( auto itr = boneChain->boneName.begin(); itr != boneChain->boneName.end(); itr++ )
		boneName[itr->second] = itr->first;

	for ( int i = 0; i < numBone; i++ )
	{
		size_t size = boneName[i].length();
		fwrite( &size, sizeof( size_t ), 1, file );
		fwrite( boneName[i].c_str(), sizeof( char ), size, file );

		fwrite( &boneChain->bones[i].parent, sizeof( int ), 1, file );
		size = boneChain->bones[i].children.size();
		fwrite( &size, sizeof( size_t ), 1, file );
		fwrite( boneChain->bones[i].children.data(), sizeof( int ), size, file );
	}

	// ウェイト
	for ( int i = 0; i < numBone; i++ )
	{
		numBoneInfluences = skinInfo->GetNumBoneInfluences( i );
		vertices = new DWORD[numBoneInfluences];
		weights = new FLOAT[numBoneInfluences];
		hr = skinInfo->GetBoneInfluence( i, vertices, weights );

		fwrite( &numBoneInfluences, sizeof( DWORD ), 1, file );
		fwrite( vertices, sizeof( DWORD ) * numBoneInfluences, 1, file );
		fwrite( weights, sizeof( FLOAT ) * numBoneInfluences, 1, file );

		delete[] vertices;
		delete[] weights;
	}

	// アニメーション
	size_t size = animation->currentBone.boneArray.size();
	fwrite( &size, sizeof( size_t ), 1, file );
	fwrite( animation->currentBone.boneArray.data(), sizeof( tdnBone ), size, file );

	fwrite( &animation->numFrame, sizeof( animation->numFrame ), 1, file );
	for ( unsigned int i = 0; i < animation->numFrame; i++ )
	{
		fwrite( animation->poseArray[i].boneArray.data(), sizeof( tdnBone ) * numBone, 1, file );
	}
}

bool tdnSkinMesh::LoadTDNSM( const char* fileName )
{
	FILE* file( nullptr );
	fopen_s( &file, fileName, "rb" );
	if ( file == nullptr )
		return false;

	LoadTDNM(file, fileName);
	LoadTDNSM( file );

	fclose( file );

	return true;
}

bool tdnSkinMesh::LoadTDNSM( FILE* file )
{
	DWORD numBoneInfluences( 0 );
	WeightCreateData data;

	fread_s( &numBone, sizeof( numBone ), sizeof( numBone ), 1, file );

	// ボーンチェーン
	delete boneChain;
	boneChain = new tdnBoneChain();
	boneChain->bones.resize( numBone );
	boneChain->bones.reserve( numBone );

	for ( int i = 0; i < numBone; i++ )
	{
		size_t size( 0 );
		fread_s( &size, sizeof( size_t ), sizeof( size_t ), 1, file );
		char *boneName = new char[size + 1] {};
		fread_s( boneName, sizeof( char ) * size + 1, sizeof( char ), size, file );
		boneChain->boneName[boneName] = i;
		delete[] boneName;

		fread_s( &boneChain->bones[i].parent, sizeof( int ), sizeof( int ), 1, file );
		if ( boneChain->bones[i].parent < 0 )
			rootBones.push_back( i );
		fread_s( &size, sizeof( size_t ), sizeof( size_t ), 1, file );
		boneChain->bones[i].children.resize( size );
		boneChain->bones[i].children.reserve( size );
		fread_s( boneChain->bones[i].children.data(), sizeof( int ) * size, sizeof( int ) * size, 1, file );
	}

	// ウェイト
	data.bonesWeight.resize( numBone );

	for ( int i = 0; i < numBone; i++ )
	{
		fread_s( &numBoneInfluences, sizeof( DWORD ), sizeof( DWORD ), 1, file );

		data.bonesWeight[i].vertices.resize( numBoneInfluences );
		data.bonesWeight[i].weights.resize( numBoneInfluences );

		fread_s( data.bonesWeight[i].vertices.data(), sizeof( DWORD ) * numBoneInfluences, sizeof( DWORD ) * numBoneInfluences, 1, file );
		fread_s( data.bonesWeight[i].weights.data(), sizeof( FLOAT ) * numBoneInfluences, sizeof( FLOAT ) * numBoneInfluences, 1, file );
	}

	CreateWeight( data );

	// アニメーション
	delete animation;
	animation = new tdnSkinAnimation();

	size_t size;
	fread_s( &size, sizeof( size_t ), sizeof( size_t ), 1, file );
	animation->currentBone.boneArray.resize( size );
	animation->currentBone.boneArray.reserve( size );
	fread_s( animation->currentBone.boneArray.data(), sizeof( tdnBone ) * size, sizeof( tdnBone ), size, file );

	fread_s( &animation->numFrame, sizeof( animation->numFrame ), sizeof( animation->numFrame ), 1, file );
	animation->poseArray.resize( animation->numFrame );

	animation->numBone = numBone;

	for ( unsigned int i = 0; i < animation->numFrame; i++ )
	{
		animation->poseArray[i].boneArray.resize( numBone );
		animation->poseArray[i].boneArray.reserve( numBone );
		fread_s( animation->poseArray[i].boneArray.data(), sizeof( tdnBone ) * numBone, sizeof( tdnBone ) * numBone, 1, file );
	}

	return true;
}

void tdnSkinMesh::UpdateBoneChain( int index )
{
	tdnChainBone* bone = &( boneChain->bones[index] );

	int animFrame = ( int ) floor( frame );

	tdnBone::Slerp( animation->poseArray[animFrame].boneArray[index], animation->poseArray[animFrame + 1].boneArray[index], frame - animFrame, &bone->srt );

	// モデル座標
	if ( bone->parent >= 0 )
	{
		bone->srt = boneArray[bone->parent] * bone->srt;
	}
	boneArray[index] = bone->srt;

	for each ( int child in boneChain->bones[index].children )
	{
		UpdateBoneChain( child );
	}
}



tdn3DObj::tdn3DObj( char* meshFile, char* motionFile )
{
	skinMesh = new tdnSkinMesh();
	skinMesh->LoadTDNSM( meshFile );
	motion = new tdnAnimationFrame();
	motion->Load( motionFile );
}

tdn3DObj::~tdn3DObj()
{
	delete skinMesh;
	delete motion;
}

void tdn3DObj::Update()
{
	skinMesh->UpdateBoneMatrix();
	skinMesh->UpdateSkin();
	skinMesh->UpdateWorldMatrix();
}

void tdn3DObj::SetMotion( int id, bool waitAnimEnd )
{
	motion->Select( id, waitAnimEnd );
}

void tdn3DObj::Animation( float speed )
{
	motion->AddFrame( speed );
	motion->Update();
	skinMesh->SetFrame( motion->GetFrame() );
}

void tdn3DObj::Render( tdnShader* shader, char* name )
{
	skinMesh->Render( shader, name );
}

void tdn3DObj::UpdateSkinMeshFrame( float frame )
{
	skinMesh->SetFrame( frame );
	skinMesh->UpdateBoneMatrix();
	skinMesh->UpdateSkin();
}

void tdn3DObj::UpdateBoneMatrix()
{
	skinMesh->UpdateBoneMatrix();
}
