//**************************************************************************************************
//																									
//		基本シェーダー		
//
//**************************************************************************************************



//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float4x4 TransMatrix;	//	変換行列
float4x4 matView;		//	変換行列
float4x4 Projection;	//	変換行列

float3	ViewPos;

//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------
texture Texture;	//	デカールテクスチャ
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture NormalMap;	//	法線マップテクスチャ
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture SpecularMap;	//	スペキュラマップテクスチャ
sampler SpecularSamp = sampler_state
{
    Texture = <SpecularMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture HeightMap;		//	高さマップテクスチャ
sampler HeightSamp = sampler_state
{
    Texture = <HeightMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

//**************************************************************************************************
//		頂点フォーマット
//**************************************************************************************************
struct VS_BASIC
{
    float4 Pos    : POSITION;
    float4 Color  : COLOR0;
    float2 Tex	  : TEXCOORD0;

	float4 WorldPos:TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
    float3 Normal	: TEXCOORD1;

    float4 Ambient	: COLOR1;
	float4 light	: COLOR2;
	float3 vLight	: TEXCOORD2;
	float3 vE		: TEXCOORD3;
	float  specular	: TEXCOORD4;
};

struct VS_INPUTL
{
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float4 Color  : COLOR0;
    float2 Tex	  : TEXCOORD0;
};


//**************************************************************************************************
//
//		ライティング
//
//**************************************************************************************************

//**************************************************************************************************
//	半球ライティング
//**************************************************************************************************
float3 SkyColor = { 0.48f, 0.5f, 0.5f };
float3 GroundColor = { 0.4f, 0.4f, 0.4f };

inline float4 HemiLight( float3 normal )
{
	float4 color;
	float rate = (normal.y*0.5f) + 0.5f;
	color.rgb = SkyColor * rate;
	color.rgb += GroundColor * (1-rate);
	color.a = 1.0f;

	return color;
}

//**************************************************************************************************
//	平行光
//**************************************************************************************************
float3 LightDir = { 1.0f, -1.0f, 0.0f };
float3 DirLightColor = { 0.6f, 0.6f, 0.6f };

inline float3 DirLight( float3 dir, float3 normal )
{
	float3 light;
	float rate = max( 0.0f, dot( -dir, normal ) );
	light = DirLightColor * rate;
	
	return light;
}

//**************************************************************************************************
//	スペキュラ
//**************************************************************************************************
inline float Specular(float3 pos, float3 normal)
{
    float   sp;

    float3	H = normalize(ViewPos - pos);
    H = normalize(H - LightDir);
    
	sp = dot(normal, H);
	sp = max( 0, sp );
	sp = pow(sp, 10);

    return sp;
}

//------------------------------------------------------
//		フォグ関連
//------------------------------------------------------
float	FogNear	= 60.0f;
float	FogFar  = 80.0f;
float3	FogColor = { 0.7f, 0.7f, 0.7f };


//**************************************************************************************************
//	環境マップ用
//**************************************************************************************************
texture EnvMap;		//	環境テクスチャ
sampler EnvSamp = sampler_state
{
    Texture = <EnvMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

inline float4 Environment( float3 normal )
{
	float4	color;
	
	float2	uv;
	uv.x = normal.x*0.5f + 0.5f;
	uv.y = normal.y*0.5f + 0.5f;

	color = tex2D( EnvSamp, uv );
	return color;
}


//**************************************************************************************************
//
//		基本シェーダー
//
//**************************************************************************************************

//------------------------------------------------------
//	頂点シェーダー
//------------------------------------------------------
VS_BASIC VS_Basic( VS_INPUTL In )
{
	VS_BASIC Out = (VS_BASIC)0;

	float3 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = DirLight( LightDir, N ) + HemiLight( N );
	Out.Color.a = 1.0f;
	Out.Tex   = In.Tex;

	return Out;
}

//------------------------------------------------------
//	頂点カラー付シェーダー
//------------------------------------------------------
VS_BASIC VS_Basic2( VS_INPUTL In )
{
    VS_BASIC Out = (VS_BASIC)0;
	
	float3 P = mul( In.Pos, TransMatrix );

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = (DirLight( LightDir, N ) + HemiLight( N ));
	Out.Color.a = In.Color.a;
	Out.Tex   = In.Tex;

    return Out;
}

//------------------------------------------------------
//	ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic( VS_BASIC In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );

	return OUT;
}

//------------------------------------------------------
//	テクニック
//------------------------------------------------------
technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader  = compile ps_3_0 PS_Basic();
    }
}

technique lcopy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_2_0 VS_Basic2();
		PixelShader  = compile ps_2_0 PS_Basic();
    }
}

technique add
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = One;
		ZWriteEnable     = false;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader  = compile ps_3_0 PS_Basic();
    }
}

//**************************************************************************************************
//
//		フルエフェクト
//
//**************************************************************************************************
//------------------------------------------------------
//	頂点シェーダ
//------------------------------------------------------
VS_OUTPUT VS_FullFX( VS_INPUTL In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
	
	float4 P = mul(In.Pos,    TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);
	Out.Normal = N;
	Out.Ambient.rgb = HemiLight(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color = 1.0f;
	Out.Tex   = In.Tex;

	//	ライトベクトル補正
	float3	vx;
	float3	vy = { 0.0f, -1, 0.01f };

	vx = cross( N, vy );
	vx = normalize( vx );
	
	vy = cross( vx, N );
	vy = normalize( vy );

	//	ライトベクトル補正
	Out.vLight.x = dot(vx, LightDir);
	Out.vLight.y = dot(vy, LightDir);
	Out.vLight.z = dot(N, LightDir);
	Out.vLight = normalize( Out.vLight );

	// 視線ベクトル補正
	float3 E = (P - ViewPos);   // 視線ベクトル
	Out.vE.x = -dot(vx, E);
	Out.vE.y = dot(vy, E);
	Out.vE.z = dot(N, E);
	Out.vE = normalize( Out.vE );

	Out.light = 0;//PointLight( P, N );

	//	フォグ計算
	Out.Ambient.a = (FogFar-Out.Pos.z) / (FogFar-FogNear);
	Out.Ambient.a = saturate(Out.Ambient.a);

	return Out;
}

//------------------------------------------------------
//	頂点カラー付シェーダ
//------------------------------------------------------
VS_OUTPUT VS_FullFX2( VS_INPUTL In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);
	Out.Normal = N;

	Out.Ambient.rgb = HemiLight(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color = In.Color;
	Out.Tex   = In.Tex;

	//	ライトベクトル補正
	float3	vx;
	float3	vy = { 0, 1, 0.01f };

	vx = cross( N, vy );
	vx = normalize( vx );

	vy = cross( N, vx );
	vy = normalize( vy );

	//	ライトベクトル補正
	Out.vLight.x = dot(vx, LightDir);
	Out.vLight.y = dot(vy, LightDir);
	Out.vLight.z = dot(N, LightDir);

	// 視線ベクトル補正
	float3 E = (P - ViewPos);   // 視線ベクトル
	Out.vE.x = dot(vx, E);
	Out.vE.y = dot(vy, E);
	Out.vE.z = dot(N, E);

	Out.light = 0;//PointLight( P, N );

	//	フォグ計算
	Out.Ambient.a = (FogFar-Out.Pos.z) / (FogFar-FogNear);
	Out.Ambient.a = saturate(Out.Ambient.a);

	return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_FullFX( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	パララックスディスプレースメント
	float h = tex2D( HeightSamp, Tex ).r-0.5f;// 高さマップのサンプリング
	float3 E = normalize(In.vE);
	In.vLight = normalize(In.vLight);
	
	Tex -= 0.05f * h * E.xy;
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	ライト計算
	float3 light = DirLight(In.vLight, N);
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb*(light+In.Ambient.rgb));


	//	スペキュラ
	float3  R = normalize( reflect( E, N ) );
	OUT.rgb += pow( max( 0, dot(R, -In.vLight) ), 10 ) * tex2D( SpecularSamp, Tex );
	//	フォグ採用
	OUT.rgb = (OUT.rgb * In.Ambient.a) + (FogColor * (1-In.Ambient.a));

	return OUT;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_FullFX2( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	パララックスディスプレースメント
	float h = tex2D( HeightSamp, Tex ).r-0.5f;// 高さマップのサンプリング
	float3 E = normalize(In.vE);
	In.vLight = normalize(In.vLight);
	
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	ライト計算
	float3 light = DirLight(In.vLight, N);
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb*(light+In.Ambient.rgb));


	//	スペキュラ
	float3  R = normalize( reflect( E, N ) );
	OUT.rgb += pow( max( 0, dot(R, -In.vLight) ), 10 ) * tex2D( SpecularSamp, Tex );
	//	フォグ採用
	OUT.rgb = (OUT.rgb * In.Ambient.a) + (FogColor * (1-In.Ambient.a));

	return OUT;
}

//------------------------------------------------------
//		合成なし	
//------------------------------------------------------
technique copy_fx
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_FullFX();
		PixelShader  = compile ps_3_0 PS_FullFX();
    }
}

technique lcopy_fx
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;
		VertexShader = compile vs_3_0 VS_FullFX2();
		PixelShader  = compile ps_3_0 PS_FullFX();
    }
}

technique copy_fx2
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_FullFX();
		PixelShader  = compile ps_3_0 PS_FullFX2();
    }
}

technique lcopy_fx2
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;
		VertexShader = compile vs_3_0 VS_FullFX2();
		PixelShader  = compile ps_3_0 PS_FullFX2();
    }
}



//**************************************************************************************************
//																									
//		基本シェーダー	の下に追加	
//
//**************************************************************************************************



//**************************************************************************************************
//		POINTLIGHT
//**************************************************************************************************
//必要な変数
static float LIGHTMAX = 1;
float3 P_pos=(float3)0;//まだmainで触ってない
float3 P_Color=(float3)0;//Color
float P_Power = 45.0f;//Power


//------------------------------------------------------
//	POINTLIGHT　頂点シェーダー
//------------------------------------------------------
VS_BASIC VS_POINTLIGHT(VS_INPUTL In)
{
	VS_BASIC Out = (VS_BASIC)0; //Outがピクセルシェーダーに情報を渡す変数。

	float3 P = mul(In.Pos, TransMatrix);//ある頂点のポジションとTransmatrixを掛けて　頂点のポジション→Ｐを作る

		float3x3	mat = TransMatrix;//TRANSMATRIXと化したmat　★追記：：TransMatrixはfloat4のmatはfloat3で　法線はfoat3なので　matをfloat3にしてんだよ。

		
		float3 N = mul(In.Normal, mat);//ある頂点の法線とmatを掛けて　頂点の法線→Ｎを作る
		N = normalize(N);//Ｎを正規化して　-１～１の間にする。

	Out.Pos = mul(In.Pos, Projection);//ある頂点のポジションとProjyectionを掛けて　Out.Posに入れていく　
	Out.Color.rgb = 0.5 + DirLight(LightDir, N);// +HemiLight(N);//DirLighitは法線で光の色を強くHemLightで阪急ライティングの関数を使い　★　阪急ライティングコメントアウト
	//ピクセルシェーダーに渡す
	Out.Color.a = 1.0f;//その渡すカラーのアルファ値
	Out.Tex = In.Tex;//そこ頂点のテクスチャーを　Out.Texに入れる？

	Out.WorldPos = float4(P, 1);


	return Out;//ピクセルシェーダーに入れる情報をおくる
}

//ポイントライトの計算をする関数
float3 PointLightRate(float3 Pos){
	float3 col = (float3)0;

	//for (int i = 0; i < 10; i++){
	float rate = length(P_pos - Pos) / P_Power;//字の間違い注意

	rate = min(1.0f, rate);//1.0からオーバーさせないため
	rate = max(0, rate);//.0からオーバーさせないため
	rate = (1.0f - rate);//Rateが弱い所を強くするから　数値を逆に！
	col += P_Color* rate;
	//}

	col = min(.75f, col);//1.0からオーバーさせないため(ここの数値をいじると程よいライトになる)
	col = max(0, col);//.0からオーバーさせないため

	return col;
}

//------------------------------------------------------
//	POINTLIGHT　ピクセルシェーダー	
//------------------------------------------------------
float4 PS_POINTLIGHT(VS_BASIC In) : COLOR
{

	float4	OUT;
	//ここから

	float4 LIGHT;//ライトの色情報
	LIGHT.rgb = PointLightRate(In.WorldPos);
	LIGHT.a = .0f;

	//ピクセル色決定
	OUT = /*In.Color **/ (tex2D(DecaleSamp, In.Tex) + LIGHT);

	return OUT;
}
//------------------------------------------------------
//	POINTLIGHT　テクニック
//------------------------------------------------------
technique pointlight
{
	pass P0
	{
		AlphaBlendEnable = true;//透明度
		BlendOp = Add;//合成の設定
		SrcBlend = SrcAlpha;//
		DestBlend = InvSrcAlpha;//
		ZWriteEnable = true;//Zバッファを使うか？

		VertexShader = compile vs_3_0 VS_POINTLIGHT();
		PixelShader = compile ps_3_0 PS_POINTLIGHT();
	}
}



/***********************************************************/
/*****************ここまでがポイントライト******************/
/***********************************************************/




//**************************************************************************************************
//
//		ＵＶアニメーション用
//
//**************************************************************************************************

float uvMove;

VS_OUTPUT VS_UvAni(VS_INPUTL In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(In.Pos, Projection);
	Out.Color = 1.0f;	//α値
	//	Out.Color.b = 0.0f;
	Out.Tex = In.Tex + float2(uvMove, .0f);

	return Out;
}

technique uvAnim
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_UvAni();
		PixelShader = compile ps_3_0 PS_Basic();
	}
}