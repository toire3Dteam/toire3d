//------------------------------------------------------
//		変換行列
//------------------------------------------------------
float4x4 g_mWMatrix;		// ワールド変換行列
float4x4 g_mVMatrix;		// ビュー変換行列
float4x4 g_mPMatrix;		// プロジェクション変換行列

float4x4 g_mWVPMatrix;		// 投影変換行列
float4x4 g_mVPMatrix;		// ワールド座標から→プロジェクション座標へ持っていく行列

float4x4 g_mInvPMatrix;	// プロジェクション座標 ->View座標　へ戻す逆行列
float4x4 g_mInvVMatrix;	// View座標 ->ワールド座標　へ戻す逆行列
float4x4 g_mInvVPMatrix;	// プロジェクション座標 -> ワールド座標 へ戻す逆行列

float4x4 g_mShadowProjection; // 影

//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float3	g_vViewPos;		// ワールド座標の目のポジション

float3 g_vWLightVec = { 1.0f, -1.0f, 1.0f };// ワールド空間での平行光の向き

//float3 LightVec;		// 平行光の向き
float3 g_vViewLightVec;	// ビュー座標での光りの向き
float3 g_vLightColor = { 1.0f, 1.0f, 1.0f };

float3 g_vSkyColor = { .4f, .6f, .6f };
float3 g_vGroundColor = { .6f, .4f, .4f };

float g_fRimPowerRate = 2.0f;		 // リムライトの光の幅 
float g_fEmissivePowerRate = 2.0f;// エミッシブの幅 エミッシブがいらない時はテクス茶を変えずこの数値を下げる
// 今は0に

float g_fFogNear = 128.0f;
float g_fFogFar = 512.0f;
float3 g_vFogColor = { 0.2f, 0.3f, 0.35f };

/***************************/
//	最初の頂点データ
/***************************/
struct VS_INPUT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------

// MinFilter	縮小フィルタに使用されるメソッド
// MagFilter	拡大フィルタに使用されるメソッド
// MipFilter	MIPMAPに使用されるメソッド
// AddressU	0 - 1の範囲外にあるuテクスチャー座標の解決に使用されるメソッド
// AddressV	0 - 1の範囲外にあるvテクスチャー座標の解決に使用されるメソッド
// LINEAR		バイリニア補間フィルタリング。目的のピクセルに外接する2×2テクセルの重み付き平均領域を使用
// NONE		MIPMAPを無効にし、拡大フィルタを使用
// CLAMP		範囲[0.0, 1.0]の外にあるテクスチャー座標には、それぞれ0.0または1.0のテクスチャーカラーを設定
// MIPMAP		メインとなるテクスチャの画像を補完するよう事前計算され最適化された画像群。詳細さのレベルによって画像を切り替える

// WRAP...反対側から同じ絵を繰り返します
// CLAMP...端の色を繰り返します
// BORDER...透明色にします

texture Texture;
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

texture RoughnessMap;	//	粗さテクスチャ
sampler RoughnessSamp = sampler_state
{
	Texture = <RoughnessMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture MultiMap;		//	マルチマップテクスチャ
sampler MultiSamp = sampler_state
{
	Texture = <MultiMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;

	BorderColor = 0x00000000;
};

texture ToonShadowMap;	//	トゥーンの影用テクスチャ
sampler ToonShadowSamp = sampler_state
{
	Texture = <ToonShadowMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

//********************************************************************
//
//		基本３Ｄシェーダー		
//
//********************************************************************

/***************************/
//	★TDNMeshで使用
/***************************/
struct VSINPUT
{
	float4 Pos : POSITION;
	float4 Normal : NORMAL;
	float4 Color : COLOR;
	float2 UV : TEXCOORD0;
	// インスタンシング
	float4 worldPos : TEXCOORD1;
};

VSINPUT VS(VSINPUT In)
{
	VSINPUT Out = (VSINPUT)0;

	Out.Pos = mul(In.Pos, g_mWMatrix);
	Out.Pos += In.worldPos;

	//float4x4 g_mVPMatrix;
	//g_mVPMatrix = mul(g_mVMatrix, g_mPMatrix);

	Out.Pos = mul(Out.Pos, g_mVPMatrix);

	Out.UV = In.UV;
	Out.Normal = In.Normal;
	Out.Color = In.Color;
	return Out;
}

float4 PS(VSINPUT In) : COLOR
{
	float4 Out = 1;
	In.Color = min(1, In.Color);
	Out.rgb *= tex2D(DecaleSamp, In.UV);
	Out.rgb *= In.Color;
	//Out.a = alpha;
	Out.a = 1;
	return Out;
}

technique linecopy
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		Zenable = true;
		ZWriteEnable = true;

		// シェーダ
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}

//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos			: TEXCOORD1;//　ピクセルに送る情報にワールドからのポジション追加
	float4 ProjectionPos: TEXCOORD2;//　ピクセルに送る情報にゲーム画面ラストのポジション追加
};
//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT VS_Basic(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//TransMatrixとPosを合成してwPosの情報生成
	Out.wPos = mul(In.Pos, g_mWMatrix);

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.ProjectionPos = Out.Pos;

	return Out;
}
//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);

	//OUT.r = 1.0f;
	OUT.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique copy
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Basic();
	}
}

//------------------------------------------------------
//		黒色ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Black(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	ピクセル色決定
	OUT.rgb = 0.0f;

	OUT.a = 1.0f;

	return OUT;
}
//------------------------------------------------------
//		黒塗りつぶし描画テクニック
//------------------------------------------------------
technique black
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_Black();
	}
}

//------------------------------------------------------
//		カメラピクセルシェーダー	
//------------------------------------------------------
float4 PS_CameraAlpha(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);
	OUT.a = 0.5f;

	return OUT;
}
//------------------------------------------------------
//		半透明描画テクニック
//------------------------------------------------------
technique cameraAlpha
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader = compile ps_3_0 PS_CameraAlpha();
	}
}


/***************************/
//	G_Buffer用の頂点データ
/***************************/

struct VS_DEFERRED
{
	float4 Pos				:POSITION;
	float4 Color			:COLOR0;
	float2 Tex				:TEXCOORD0;
	float3 Normal			:TEXCOORD1;			//法線
	float4 Depth			:TEXCOORD2;			//深度情報
	float3 Tangent			:TEXCOORD3;			//タンジェント(接線)
	float3 Binormal			:TEXCOORD4;			//Binormal(従法線)
	//float3 vE				:TEXCOORD5;			//視線ベクトル
};

// 四つの情報をこの順番通りに描く
struct PS_DEFERRED
{
	float4 Diffuse			:COLOR0;	// テクスチャー
	float4 Normal			:COLOR1;	// 法線情報
	float4 Specular			:COLOR2;	// スペキュラ
	float4 Depth			:COLOR3;	// 奥行き
};

// 2つの情報をこの順番通りに描く
struct PS_G_BUFFER
{
	float4 NormalDepth		:COLOR0;	// 法線情報&奥行き
	float4 Specular			:COLOR1;	// スペキュラ
};


//*****************************************
//		G_Buffer [1206] 最適化ずみ
//*****************************************

VS_DEFERRED VS_G_Buffer(VS_INPUT In)
{
	VS_DEFERRED Out = (VS_DEFERRED)0;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = In.Color;

	// スクリーン座標での深度値保存
	Out.Depth = Out.Pos;

	// 法線情報の修正
	float3x3 WVmat = (float3x3)mul(g_mWMatrix, g_mVMatrix); // モデル→ワールド→ビュー空間への変換行列

	// ビュー行列の法線ができあがる
	float3 N = mul(In.Normal, WVmat);// 
	N = normalize(N);
	Out.Normal = N;

	// ビュー行列の法線のタンジェントとバイノーマルを作る

	// 頂点スクリーン座標系算出
	float3	vx;
	float3	vy = { .0f, 1.0f, 0.5f };			// 仮のy方向ベクトル  0.001f は　外積を成功させる為にずらしている　同じ値で外積すると0ベクトルになる

	vx = cross(N, vy);
	vx = normalize(vx);
	Out.Tangent = vx;

	vy = cross(N, vx);
	vy = normalize(vy);
	Out.Binormal = vy;

	return Out;
}

PS_G_BUFFER PS_G_Buffer(VS_DEFERRED In)
{
	PS_G_BUFFER OUT = (PS_G_BUFFER)0;

	// (1212) アルファが少しでもなければ破棄 Decaleを吐き出さないようにしているので
	clip((tex2D(DecaleSamp, In.Tex).a - 0.999f));

	//float2 Tex = In.Tex;

		//float3 E = normalize(In.vE);

		//　各色に光のパラメーターを設定
		//  R->AO  G->Emissive  B->未設定
		// float3 multiRate = tex2D(MultiSamp, Tex).rgb;

		//ディヒューズ
		//OUT.Diffuse = In.Color * tex2D( DecaleSamp,Tex );
	//	OUT.Diffuse = tex2D(DecaleSamp, Tex);
	//OUT.Diffuse.a = 1.0f;

	//スペキュラ
	OUT.Specular = tex2D(RoughnessSamp, In.Tex);
	//OUT.Specular.a = 1.0f;
	//OUT.Specular.a = multiRate.g;	// エミッシブを入れる

	
	// OUT.Depth = In.Depth.z / In.Depth.w;	//★　wで割って-1~1に変換する　
	// Positonと違い自分でNDC空間にする必要がある


	{
		// [1206] 接空間は頂点で正規化してるのでPixel側では正規化しない
		
		// 	接空間からビュー空間へ持っていくビュー回転行列を作成
		// ↓★これは（接→Viewの[変換行列]）
		float3x3 ts;								//法線から求めた軸
		ts[0] = normalize(In.Tangent);				//接線
		ts[1] = normalize(In.Binormal);				//従法線
		ts[2] = normalize(In.Normal);				//法線


		float3 N = tex2D(NormalSamp, In.Tex).xyz * 2.0f - 1.0f;// テクスチャから色を取得してベクトルに変換
		float3 normal = normalize(mul(N, ts));// 接空間のNormalを↑の変換行列を掛け合わせView空間の法線へ　
		normal = normal * 0.5f + 0.5f;// 法線を色で出すため0~1の間に戻す
		OUT.NormalDepth.rg = normal.rg;

	}

	//深度
	//OUT.NormalDepth.ba = In.Depth.zw;
	OUT.NormalDepth.b = In.Depth.z / In.Depth.w; // 一応Wで割ってNDC空間にしてみる
	OUT.NormalDepth.a = In.Depth.w; //　


	return OUT;
}

/* deferredでαに情報を書き入れる方法 */
/*
普通に描くと　半透明オブジェクトが重なり合い情報が正確にでない
解決方法は
SrcBlend = One;
DestBlend = Zero
ZEnable = true;
に設定する

↑どういうことが起こるか
ソースブレンドは　今書いているものをどれくらい乗っけるかの値
これを One(1,1,1,1)にすることで全力で上にかぶせる用に設定

デプスブレンドは　後に描いていた物体をどう薄めるか
これを　Zero(0,0,0,0)にすることで一切描かない

これだと　後ろが絶対見える処理になっていしまうけど
Ｚ値を考慮すればそもそも後ろを書かなくできる

*/
technique G_Buffer
{
	pass P0
	{
		// レンダリングステート
		AlphaBlendEnable = true;
		BlendOp = Add;
		//SrcBlend = SrcAlpha;		// 最初のRGBデータソースを指定します
		//DestBlend = InvSrcAlpha;	// 2番目のRGBデータソースを指定します
		SrcBlend = One;		//1,1,1,1
		DestBlend = Zero;	//0,0,0,0
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;
	
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// αがAlphaRef以上ならOK

		VertexShader = compile vs_3_0 VS_G_Buffer();
		PixelShader = compile ps_3_0 PS_G_Buffer();
	}
}

/*☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆*/
//	ここからがG_Bufferを利用してライティングなどをする			
/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
/****************************/
//	関数
/****************************/

float3 CalcNormal(float2 xy)
{
	float3 normal;
	normal.xy = xy;
	normal.z = -sqrt(1.0f - (normal.x*normal.x) - (normal.y*normal.y));

	normal=normalize(normal);

	return normal;
}

// UV値とZ値からビュー空間座標を算出
float3 CalcViewPosition(float2 UV, float2 zw)
{
	// UV空間
	//      0.0 ---> 1.0
	//	  y ↓				
	//		1.0f

	// NDC空間  UVではｙは↓を向いていたのでｙは反転する必要がある
	//			1.0
	//			↑
	//  -1.0<--- 0.0 ---> 1.0
	//	      y ↓				
	//		   -1.0f

	float4 proj;
	proj.xy = (UV*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* zw.y; //先生に聞こう
	//proj.xy = UV*2.0f - 1.0f;
	//proj.y *= -1.0f;
	proj.zw = zw;	// ZとWはそのまま入れる

	// プロジェクション逆行列でビュー座標系に変換　Proj->View
	float4 viewPos = mul(proj, g_mInvPMatrix);
	//viewPos.xyz /= viewPos.w;		// Wで割ってはいけない。なぜ？

	return viewPos.xyz;
}


// オブジェの場所と法線、光の向き　光の絞る率　でスペキュラレートを返す
float CalcSpecular(float3 pos, float3 normal, float3 lightVec, float power)
{
	//　スペキュラ
	float3 E = normalize(pos.xyz);// ビュー行列での目線
		 
	float3 R = normalize(-lightVec - E);
	float specRate = pow(max(0, dot(R, normal)), power) ;// いつも通り絞って

	return specRate;
}

//------------------------------------------------------
///		サンプラー
//------------------------------------------------------

texture NormalDepthTex;
sampler NormalDepthSamp = sampler_state
{
	Texture = <NormalDepthTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


texture RoughnessLightMapTex;
sampler RoughnessLightMapSamp = sampler_state
{
	Texture = <RoughnessLightMapTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = BORDER;
	AddressV = BORDER;
};

// 影
texture ShadowMap;
sampler ShadowSamp = sampler_state
{
	Texture = <ShadowMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

/********************************************************/
//			シャドウ
/********************************************************/

struct VS_SHADOW
{
	float4 Pos			:POSITION;
	float2 ZW			:TEXCOORD0;		//深度値
	float2 Tex			:TEXCOORD1;
};

// 近距離用シャドウマップ作成
VS_SHADOW VS_ShadowBuf(float4 Pos : POSITION, float2 Tex : TEXCOORD0)
{
	VS_SHADOW Out;

	// 座標変換　太陽からみた空間に
	float4x4	mat = mul(g_mWMatrix, g_mShadowProjection);
		Out.Pos = mul(Pos, mat);

	Out.ZW = Out.Pos.zw;
	Out.Tex = Tex;

	return Out;
}

// PixelShaderは共通で
float4 PS_ShadowBuf(VS_SHADOW In) : COLOR
{
	float4 OUT = float4(0, 0, 0, 0);
	OUT.r = In.ZW.r / In.ZW.g;			// 距離を色に
	OUT.a = tex2D(DecaleSamp, In.Tex).a;// 透明だったら切るようにする
	
	return OUT;
}

technique ShadowBuf
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;// αがAlphaRef以上ならOK

		ZWriteEnable = true;
		CullMode = none;		 // カリングをしないことで綺麗に影を描画　

		VertexShader = compile vs_2_0 VS_ShadowBuf();
		PixelShader = compile ps_2_0 PS_ShadowBuf();
	}
}

/***************************/
//	ライティング用構造体
/***************************/
struct PS_LIGHT
{
	float4	color : COLOR0;
	float4  spec  : COLOR1;
};

/***********************************/
//	平行光
PS_LIGHT PS_DirLight(float2 Tex:TEXCOORD0)
{
	PS_LIGHT OUT;

	// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);

	// ライト率(ハーフランバート)
	float rate = (dot(Normal, -g_vViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 Lambert = max(0, rate);				// Lambert

	// ピクセルの色
	OUT.color.rgb = (HalfLambert * g_vLightColor);
	OUT.color.a = 1.0f;

	//　スペキュラレート取得
	float sp = CalcSpecular(Pos, Normal, g_vViewLightVec, 80);
	// スペキュラの色をどれくらい乗せるか
	OUT.spec.rgb = sp*g_vLightColor;
	OUT.spec.a = 1.0f;

	return OUT;

}

technique DirLight
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // 重ねて描画していく
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_DirLight();
	}
}

/***********************************/
//	平行光(影)
PS_LIGHT PS_DirLightShadow(float2 Tex:TEXCOORD0)
{
	PS_LIGHT OUT;

	// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
	const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

	// ライト率(ハーフランバート)
	float rate = (dot(Normal, -g_vViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 Lambert = max(0, rate);				// Lambert

	// ピクセルの色
	OUT.color.rgb = (HalfLambert * g_vLightColor);
	OUT.color.a = 1.0f;

	//　スペキュラレート取得
	float specRate = CalcSpecular(Pos, Normal, g_vViewLightVec, RoughnessLightMap.g * 255);
	// スペキュラの色をどれくらい乗せるか
	OUT.spec.rgb = (specRate*g_vLightColor)*RoughnessLightMap.r;
	OUT.spec.a = 1.0f;

	float4 ShadowTex;
	float ShadowValue = 0.0f;
	float2 depth;		//	バリアンスシャドウマップ

	{
		// 太陽視点から見たプロジェクション座標のオブジェ
		ShadowTex = mul(mul(float4(Pos, 1), g_mInvVMatrix), g_mShadowProjection);
		ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
		ShadowTex.y = -ShadowTex.y;
		ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

		depth.r = tex2D(ShadowSamp, ShadowTex.xy).r;
		depth.g = depth.r*depth.r;// ヴァリアンスは二乗した値が必要
	}

	{
		//ブラー処理
		float2 otherDepth = float2(0, 0);
			float temp = 0;

		float2 GaussShadowSamples[] =
		{
			{ -(1.0f / 1280.0f), (1.0f / 720.0f) },
			{ 0, (1.0f / 720.0f) },
			{ (1.0f / 1280.0f), (1.0f / 720.0f) },

			{ -(1.0f / 1280.0f), 0 },
			{ (1.0f / 1280.0f), 0 },

			{ -(1.0f / 1280.0f), -(1.0f / 720.0f) },
			{ 0, -(1.0f / 720.0f) },
			{ (1.0f / 1280.0f), -(1.0f / 720.0f) },
		};
		// ↑のサンプル回数分　Forで回る
		for (uint i = 0; i < 8; ++i)
		{
			temp = tex2D(ShadowSamp, ShadowTex.xy + GaussShadowSamples[i]).r;
			otherDepth.x += temp;
			otherDepth.y += temp*temp;
		}
		// 普通の影と　もうひとつの影を足したものを入れる
		depth = depth*0.2f + otherDepth*0.1f;
	}

	{
		const float fragDepth = ShadowTex.z / ShadowTex.w; // 太陽からのオブジェの奥行き
		
		ShadowValue = 1; // 何も起こらなければ1で普通に描画

		//if (depth.r < fragDepth - 0.001f)
		{
			const float variance = depth.g - depth.r * depth.r;
			ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
		}

		//ShadowValue = tex2D(ShadowSamp, ShadowTex.xy).r;
		
		OUT.color.rgb *= ShadowValue;
		OUT.spec.rgb *= ShadowValue;
	}



	//if (tex2D(ShadowSamp, ShadowTex.xy).r < ShadowTex.z - 0.005f)
	//{
	//	OUT.color.rgb = 0;
	//	OUT.spec.rgb = 0;
	//}

	return OUT;

}

technique DirLightShadow
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // 重ねて描画していく
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_DirLightShadow();
	}
}

//*************************************
///		半球ライティング
float4 PS_HemiLight(float2 Tex: TEXCOORD0) :COLOR
{
	float4 OUT;

	// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);

	//float AO = n.a;// NormalのαからAOの値を取得

	//半球処理
	float rate = Normal.y * 0.5f + 0.5f;
	OUT.rgb = g_vSkyColor * rate;
	OUT.rgb += g_vGroundColor * (1.0f - rate);
	//OUT.rgb *= AO;
	OUT.a = 1;

	return OUT;

}
technique HemiLight
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // 重ねて描画していく
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_HemiLight();
	}
}



/***********************************/
//	平行光(影)+環境光
PS_LIGHT PS_AllLight(float2 Tex:TEXCOORD0)
{
	PS_LIGHT OUT;

	// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
	const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

	// ライト率(ハーフランバート)
	float rate = (dot(Normal, -g_vViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
														//float3 Lambert = max(0, rate);				// Lambert

														// ピクセルの色
	OUT.color.rgb = (HalfLambert * g_vLightColor);
	OUT.color.a = 1.0f;

	//　スペキュラレート取得
	float specRate = CalcSpecular(Pos, Normal, g_vViewLightVec, RoughnessLightMap.g * 255);
	// スペキュラの色をどれくらい乗せるか
	OUT.spec.rgb = (specRate*g_vLightColor)*RoughnessLightMap.r;
	OUT.spec.a = 1.0f;

	float4 ShadowTex;
	float ShadowValue = 0.0f;
	float2 depth;		//	バリアンスシャドウマップ

	{
		// 太陽視点から見たプロジェクション座標のオブジェ
		ShadowTex = mul(mul(float4(Pos, 1), g_mInvVMatrix), g_mShadowProjection);
		ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
		ShadowTex.y = -ShadowTex.y;
		ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

		depth.r = tex2D(ShadowSamp, ShadowTex.xy).r;
		depth.g = depth.r*depth.r;// ヴァリアンスは二乗した値が必要
	}

	{
		//ブラー処理
		float2 otherDepth = float2(0, 0);
		float temp = 0;

		float2 GaussShadowSamples[] =
		{
			{ -(1.0f / 1280.0f), (1.0f / 720.0f) },
			{ 0, (1.0f / 720.0f) },
			{ (1.0f / 1280.0f), (1.0f / 720.0f) },

			{ -(1.0f / 1280.0f), 0 },
			{ (1.0f / 1280.0f), 0 },

			{ -(1.0f / 1280.0f), -(1.0f / 720.0f) },
			{ 0, -(1.0f / 720.0f) },
			{ (1.0f / 1280.0f), -(1.0f / 720.0f) },
		};
		// ↑のサンプル回数分　Forで回る
		for (uint i = 0; i < 8; ++i)
		{
			temp = tex2D(ShadowSamp, ShadowTex.xy + GaussShadowSamples[i]).r;
			otherDepth.x += temp;
			otherDepth.y += temp*temp;
		}
		// 普通の影と　もうひとつの影を足したものを入れる
		depth = depth*0.2f + otherDepth*0.1f;
	}

	{
		const float fragDepth = ShadowTex.z / ShadowTex.w; // 太陽からのオブジェの奥行き

		ShadowValue = 1; // 何も起こらなければ1で普通に描画

		// 砂利じゃり防止
		if (depth.r < fragDepth - 0.001f)
		{
			const float variance = depth.g - depth.r * depth.r;
			ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
		}

		//ShadowValue = tex2D(ShadowSamp, ShadowTex.xy).r;

		OUT.color.rgb *= ShadowValue;
		OUT.spec.rgb *= ShadowValue;
	}


	//半球処理
	float HemiRate = Normal.y * 0.5f + 0.5f;
	OUT.color.rgb += g_vSkyColor * HemiRate;
	OUT.color.rgb += g_vGroundColor * (1.0f - HemiRate);


	//// フォグの値を入れる
	//float fogRate = smoothstep(g_fFogFar, g_fFogNear, Pos.z);
	//OUT.color.rgb += g_vFogColor * (1.0f - fogRate);

	//if (tex2D(ShadowSamp, ShadowTex.xy).r < ShadowTex.z - 0.005f)
	//{
	//	OUT.color.rgb = 0;
	//	OUT.spec.rgb = 0;
	//}

	return OUT;

}

technique AllLight
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One; // 重ねて描画していく
		ZEnable = False;

		PixelShader = compile ps_3_0 PS_AllLight();
	}
}


//**********************************
///		点光源スフィア配置型
//**********************************

struct DEF_POINTLIGHT
{
	float4	color	:	COLOR0;
};

// ストラクチャ
struct VS_POINTLIGHT
{
	float4 Pos		:	POSITION;
	float4 wvpPos	:	TEXCOORD0;
	float4 WPos		:	TEXCOORD1;
};

// グローバル座標
float3 g_vPLSpos;		// ※View座標系に変換しておくこと
float  g_fPLSrange = 30.0f;
float3 g_vPLScolor = float3(300, 600, 300);// カラー
float  g_fPLSpower = 2.0f;// パワー

//------------------------------------------------------
//		PointLightSphereの頂点シェーダー
//------------------------------------------------------
VS_POINTLIGHT VS_PointLightSphere(VS_INPUT In)
{
	VS_POINTLIGHT Out = (VS_POINTLIGHT)0;

	// スケールを法線にしたがって拡大 レンジ分
	In.Pos.xyz += In.Normal*g_fPLSrange;
	// 絶対に送る必要がある　ボールのポジション
	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.WPos = mul(In.Pos, g_mWMatrix);

	//　フェッチ用
	Out.wvpPos = Out.Pos;

	return Out;
}


DEF_POINTLIGHT PS_PointLightSphere(VS_POINTLIGHT In)
{
	DEF_POINTLIGHT OUT = (DEF_POINTLIGHT)0;

	//スクリーン空間をテクスチャ座標に ○
	In.wvpPos.x += 0.5f;// ★ずらす
	In.wvpPos.y -= 0.5f;// ★ずらす

	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	//ScreenTex.x += 0.001f;
	// 必要な情報を取得　○
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);// ○
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	// PLのPosとこのピクセルの位置で←ポジション＆距離
	float3 g_vViewLightVec = g_vPLSpos.xyz - (Pos.xyz);
	float dist = pow(max(0.0f, 1.0f - (length(g_vViewLightVec) / g_fPLSrange)), 2);//←数値をいじり絞る

	// 法線マップから陰影遮断
	g_vViewLightVec = normalize(g_vViewLightVec);// 光<-場所ベクトルへ
	float rate = max(0, dot(Normal.xyz, g_vViewLightVec.xyz));

	OUT.color.rgb = (g_vPLScolor*(dist*rate))*g_fPLSpower;
	OUT.color.a = 1.0f;

	return OUT;

}

//
technique PointLightSphere
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;//　追加
		DestBlend = One;
		CullMode = Cw;	// カリングはなし 法線方向にのばしたいけど中見せたい→CW
		ZEnable = FALSE;	// 奥行考慮とかいう必殺　はなしになった。　プレイヤーが前にいると困る
		ZWriteEnable = FALSE;
		//PixelShader = compile ps_3_0 PS_PointLight();
		//VertexShader = compile vs_3_0 VS_PointLightBall();
		VertexShader = compile vs_3_0 VS_PointLightSphere();
		PixelShader = compile ps_3_0 PS_PointLightSphere();
	}
}


/*☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆*/
//	ここからが↑で作ってきたライトのバッファなどを使い最後の仕上げ
/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/

//------------------------------------------------------
//		演算結果のサンプラー	
//------------------------------------------------------

//texture LightMap;
//sampler LightSamp = sampler_state
//{
//	Texture = <DiffuseLightMap>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = Wrap;
//	AddressV = Wrap;
//};
//
//texture SpecuarLightMap;
//sampler SpecuarLightSamp = sampler_state
//{
//	Texture = <SpecuarLightMap>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = Wrap;
//	AddressV = Wrap;
//};

texture LightMap;
sampler LightSamp = sampler_state
{
	Texture = <LightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture SpecMap;
sampler SpecSamp = sampler_state
{
	Texture = <SpecMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture ForwardTex;
sampler ForwardSamp = sampler_state
{
	Texture = <ForwardTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture ShadowTex;
sampler ShadowTexSamp = sampler_state
{
	Texture = <ShadowTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture SSAOTex;	//	SSAOテクスチャ
sampler SSAOSamp = sampler_state
{
	Texture = <SSAOTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture PLSMap;	//	PLSテクスチャ
sampler PLSSamp = sampler_state
{
	Texture = <PLSMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

//------------------------------------------------------
///		環境用スクリーン	主にフォアード空間で使用
//------------------------------------------------------
texture EnvFullBuf;	//	EnvFullBuf
sampler EnvFullBufSamp = sampler_state
{
	Texture = <EnvFullBuf>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	// 横だけミラーにしてごまかす
	AddressU = MIRROR;//MIRROR
	AddressV = WRAP;//MIRROR
};

/***************************************/
/*ぼかし用サンプラー*/
/***************************************/

sampler GaussianSamp = sampler_state
{
	Texture = <Texture>;
	MinFilter = GAUSSIANQUAD;
	MagFilter = GAUSSIANQUAD;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


// 画面サイズをUVに合わせる
float g_fTU = 1.0f / 1280.0f, g_fTV = 1.0f / 720.0f;
float g_fBlurValue = 1.0f;

/***************************************/
/*		ガウシアンブラー			　 */
float4 PS_gaussX(float2 Tex : TEXCOORD0) : COLOR
{

	////テクセルを取得   
	//float2 Texel0 = Tex + float2(-g_fTU * 1 * g_fBlurValue, 0.0f);
	//float2 Texel1 = Tex + float2(-g_fTU * 2 * g_fBlurValue, 0.0f);
	//float2 Texel2 = Tex + float2(-g_fTU * 3 * g_fBlurValue, 0.0f);
	//float2 Texel3 = Tex + float2(-g_fTU * 4 * g_fBlurValue, 0.0f);
	//float2 Texel4 = Tex + float2(-g_fTU * 5 * g_fBlurValue, 0.0f);

	//float2 Texel5 = Tex + float2(g_fTU * 1 * g_fBlurValue, 0.0f);
	//float2 Texel6 = Tex + float2(g_fTU * 2 * g_fBlurValue, 0.0f);
	//float2 Texel7 = Tex + float2(g_fTU * 3 * g_fBlurValue, 0.0f);
	//float2 Texel8 = Tex + float2(g_fTU * 4 * g_fBlurValue, 0.0f);
	//float2 Texel9 = Tex + float2(g_fTU * 5 * g_fBlurValue, 0.0f);

	////取得したテクセル位置のカラー情報を取得する。
	////それぞれのカラー値に重みをかけている。この重み値はすべての合計が 1.0f になるように調整する。
	//float4 p0 = tex2D(GaussianSamp, Tex) * 0.20f;

	//float4 p1 = tex2D(GaussianSamp, Texel0) * 0.12f;
	//float4 p2 = tex2D(GaussianSamp, Texel1) * 0.10f;
	//float4 p3 = tex2D(GaussianSamp, Texel2) * 0.08f;
	//float4 p4 = tex2D(GaussianSamp, Texel3) * 0.06f;
	//float4 p5 = tex2D(GaussianSamp, Texel4) * 0.04f;

	//float4 p6 = tex2D(GaussianSamp, Texel5) * 0.12f;
	//float4 p7 = tex2D(GaussianSamp, Texel6) * 0.10f;
	//float4 p8 = tex2D(GaussianSamp, Texel7) * 0.08f;
	//float4 p9 = tex2D(GaussianSamp, Texel8) * 0.06f;
	//float4 p10 = tex2D(GaussianSamp, Texel9) * 0.04f;

	////カラーを合成する
	//return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;

	float4 Out = tex2D(GaussianSamp, Tex)*0.20f;

	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 1 * g_fBlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 2 * g_fBlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 3 * g_fBlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 4 * g_fBlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(-g_fTU * 5 * g_fBlurValue, 0.0f))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 1 * g_fBlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 2 * g_fBlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 3 * g_fBlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 4 * g_fBlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(g_fTU * 5 * g_fBlurValue, 0.0f))*0.04f;

	return Out;

}

float4 PS_gaussY(float2 Tex : TEXCOORD0) : COLOR
{

	////テクセルを取得   
	//float2 Texel0 = Tex + float2(0.0, -g_fTV * 1 * g_fBlurValue);
	//float2 Texel1 = Tex + float2(0.0, -g_fTV * 2 * g_fBlurValue);
	//float2 Texel2 = Tex + float2(0.0, -g_fTV * 3 * g_fBlurValue);
	//float2 Texel3 = Tex + float2(0.0, -g_fTV * 4 * g_fBlurValue);
	//float2 Texel4 = Tex + float2(0.0, -g_fTV * 5 * g_fBlurValue);

	//float2 Texel5 = Tex + float2(0.0, g_fTV * 1 * g_fBlurValue);
	//float2 Texel6 = Tex + float2(0.0, g_fTV * 2 * g_fBlurValue);
	//float2 Texel7 = Tex + float2(0.0, g_fTV * 3 * g_fBlurValue);
	//float2 Texel8 = Tex + float2(0.0, g_fTV * 4 * g_fBlurValue);
	//float2 Texel9 = Tex + float2(0.0, g_fTV * 5 * g_fBlurValue);

	/////取得したテクセル位置のカラー情報を取得する。
	/////それぞれのカラー値に重みをかけている。この重み値はすべての合計が 1.0f になるように調整する。
	//float4 p0 = tex2D(GaussianSamp, Tex) * 0.20f;

	//float4 p1 = tex2D(GaussianSamp, Texel0) * 0.12f;
	//float4 p2 = tex2D(GaussianSamp, Texel1) * 0.10f;
	//float4 p3 = tex2D(GaussianSamp, Texel2) * 0.08f;
	//float4 p4 = tex2D(GaussianSamp, Texel3) * 0.06f;
	//float4 p5 = tex2D(GaussianSamp, Texel4) * 0.04f;

	//float4 p6 = tex2D(GaussianSamp, Texel5) * 0.12f;
	//float4 p7 = tex2D(GaussianSamp, Texel6) * 0.10f;
	//float4 p8 = tex2D(GaussianSamp, Texel7) * 0.08f;
	//float4 p9 = tex2D(GaussianSamp, Texel8) * 0.06f;
	//float4 p10 = tex2D(GaussianSamp, Texel9) * 0.04f;

	////カラーを合成する
	//return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;

	float4 Out = tex2D(GaussianSamp, Tex)*0.20f;

	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 1 * g_fBlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 2 * g_fBlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 3 * g_fBlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 4 * g_fBlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-g_fTV * 5 * g_fBlurValue))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 1 * g_fBlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 2 * g_fBlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 3 * g_fBlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 4 * g_fBlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, g_fTV * 5 * g_fBlurValue))*0.04f;

	return Out;
}


technique gaussZ
{

	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// シェーダ
		PixelShader = compile ps_3_0 PS_gaussX();
	}

	pass P1
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// シェーダ
		PixelShader = compile ps_3_0 PS_gaussY();
	}
}

technique gaussX
{

	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// シェーダ
		PixelShader = compile ps_3_0 PS_gaussX();
	}
}

technique gaussY
{

	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// シェーダ
		PixelShader = compile ps_3_0 PS_gaussY();
	}
}

//********************************************************************
//		放射ブラ―
//********************************************************************

float g_fCenterX = 0.0f;
float g_fCenterY = 0.0f;

float g_fBlurPower = 1.0f;
const float g_cIMAGE_SIZE = 512.0f;

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_RadialBlur(float2 Tex:TEXCOORD0) : COLOR
{
	float4	OUT;

	/*******************************************/
	// ( NDC空間 ) -> ( UV空間 ) に持ってくる
	// 
	// 
	// 
	/*******************************************/
	
	//　放射中心
	float2 ss_center = float2((g_fCenterX + 1.0f) * 0.5f, (-g_fCenterY + 1.0f) * 0.5f);

	//　オフセット
	float2 uvOffset = (ss_center - Tex) * (g_fBlurPower / g_cIMAGE_SIZE);

	//　サンプリング数の逆数 　for文で回す回数文色を減らし　完成したときに元の色にする。
	float InvSampling = 1.0f / 8.0f;

	//　テクスチャ座標　動かすために今のテクスチャーの場所を渡す。
	float2 uv = Tex;

		//　サンプリングの回数だけ実行
	for (int i = 0; i<8; i++)
	{
		OUT += tex2D(DecaleSamp, uv) * InvSampling;
		uv += uvOffset;
	}

	return OUT;

}
//------------------------------------------------------
// テクニック
//------------------------------------------------------
technique RadialBlur
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_3_0 PS_RadialBlur();
	}
}


//********************************************************************
//		移動ブラ―
//********************************************************************

float g_fDirectionalX = 1.0f;
float g_fDirectionalY = 0.0f;
float g_fDirectionalBlurPower = 0.015f;
//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_DirectionalBlur(float2 Tex:TEXCOORD0) : COLOR
{
	float4	OUT;

	/*******************************************/
	// ( NDC空間 ) -> ( UV空間 ) に持ってくる
	// 
	/*******************************************/

	//　オフセット
	float2 uvOffset = float2(g_fDirectionalX, g_fDirectionalY) *(g_fDirectionalBlurPower);

	//　サンプリング数の逆数 　for文で回す回数文色を減らし　完成したときに元の色にする。
	float InvSampling = 1.0f / 16.0f;

	//　テクスチャ座標　動かすために今のテクスチャーの場所を渡す。
	float2 uv = Tex;

	//　サンプリングの回数だけ実行
	for (int i = 0; i<16; i++)
	{
		OUT += tex2D(GaussianSamp, uv) * InvSampling;
		uv += uvOffset;
	}

	return OUT;
}
//------------------------------------------------------
// テクニック
//------------------------------------------------------
technique DirectionalBlur
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_3_0 PS_DirectionalBlur();
	}
}

/*-------------------*/
//  HDRブルーム効果
/*-------------------*/

// ------------------------------
/// 作成したブルームをのっける
// ------------------------------
float4 PS_hdrBloom(float2 Tex:TEXCOORD0) : COLOR
{
	float4 OUT;
	OUT = tex2D(GaussianSamp, Tex);
	OUT.rgb += OUT.rgb* 2.5f;//←ブルームの色の強さを調整
	return OUT;
}

// レンダーステート集
// https://msdn.microsoft.com/ja-jp/library/cc324307.aspx

technique add_hdrBloom
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = InvDestColor; //　ソースアルファのままだと加算だったので　InvDestColorでスクリーンに！
		DestBlend = one;
		// シェーダ
		PixelShader = compile ps_3_0 PS_hdrBloom();
	}
}

/****************************/
/*			ToneMap			*/
/****************************/

/*露光度*/
float g_fExposure = 0.0f;

// 高輝度
float3 g_vBloomColor = { 0.825f, 0.85f, 0.85f };

/***********************************/
//	高輝度抽出用の構造体
/***********************************/
struct PS_TONEMAP
{
	float4	color : COLOR0;
	float4  high  : COLOR1;
};



/*********************************************/
//
//			キャラクター用シェーダ
//
/*********************************************/

//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT_FINAL
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos			: TEXCOORD1;//　ピクセルに送る情報にワールドからのポジション追加
	float4 wvpPos		: TEXCOORD2;//　ピクセルに送る情報にゲーム画面ラストのポジション追加
};

//********************************************************************
//
//		基本３Ｄシェーダー		
//
//********************************************************************

//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT_FINAL VS_DefaultLighting(VS_INPUT In)
{
	VS_OUTPUT_FINAL Out = (VS_OUTPUT_FINAL)0;
	//TransMatrixとPosを合成してwPosの情報生成
	Out.wPos = mul(In.Pos, g_mWMatrix);

	Out.wvpPos = Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	//Out.wvpPos = Out.Pos;

	return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_DefaultLighting(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
	lightCol += tex2D(PLSSamp, ScreenTex);
	OUT.color.rgb *= lightCol;
	OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	//OUT.color.g += 0.5;

	//トーンマッピング
	OUT.color.rgb *= exp2(g_fExposure);
	
	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique DefaultLighting
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		//SrcBlend = SrcAlpha;		// 現在描いてる方
		//DestBlend = InvSrcAlpha;	// 描かれている方
		SrcBlend = One;		//1,1,1,1
		DestBlend = Zero;	//0,0,0,0

		CullMode = CCW;				// カリングの仕様

		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// αがAlphaRef以上ならOK

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_DefaultLighting();
	}
}

/*************************************/
// ステージに必要な変数
/*************************************/

float    g_fOverDriveDim = 1.0f;

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_Stage(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
	lightCol += tex2D(PLSSamp, ScreenTex);
	OUT.color.rgb *= lightCol;
	OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	
	// 必殺暗転の値
	OUT.color.rgb *= g_fOverDriveDim;

	//トーンマッピング
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		ステージ用描画テクニック
//------------------------------------------------------
technique Stage
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		//SrcBlend = SrcAlpha;		// 現在描いてる方
		//DestBlend = InvSrcAlpha;	// 描かれている方
		SrcBlend = One;				//1,1,1,1
		DestBlend = Zero;			//0,0,0,0
		CullMode = CCW;				// カリングの仕様
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// αがAlphaRef以上ならOK

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Stage();
	}
}


//------------------------------------------------------
//		(七里ステージ用)ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_StageNanasato(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//スクリーン空間をテクスチャ座標に  NDC->UV y反転
const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

//	ピクセル色決定
OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

float4 lightCol = tex2D(LightSamp, ScreenTex);
lightCol += tex2D(PLSSamp, ScreenTex);
OUT.color.rgb *= lightCol;
OUT.color.rgb += tex2D(SpecSamp, ScreenTex);

// ピクセル色をグロウ用にサンプリング
const float4 l_fGlowCol = tex2D(MultiSamp, In.Tex);
OUT.color.rgb += l_fGlowCol.rgb;


// 必殺暗転の値
OUT.color.rgb *= g_fOverDriveDim;

//トーンマッピング
OUT.color.rgb *= exp2(g_fExposure);

OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
OUT.high.a = 1.0f;


return OUT;
}

//------------------------------------------------------
//		(七里ステージ用)ステージ用描画テクニック
//------------------------------------------------------
technique StageNanasato
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = One;				//1,1,1,1
		DestBlend = Zero;			//0,0,0,0
		CullMode = CCW;				// カリングの仕様
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// αがAlphaRef以上ならOK

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_StageNanasato();
	}
}



//------------------------------------------------------
//		プレイヤー用のグローバルエリア
//------------------------------------------------------
float g_InvincibleColRate = 0.0f;//Flash そのキャラクターダウン後の点滅のレート
float g_OrangeColRate = 0.0f;//　オレンジの光
float g_MagentaColRate = 0.0f;//　マゼンタの光
float g_OverDriveColRate = 0.0f;//
float g_WillPowerRate = 0.0f;//  根性値

// [1206] 最適化のためレートの送る回数を減らすため仕方なく固めることに
float3 g_vPlayerColDesc;		//	赤->点滅 青->オレンジ 緑->マゼンタ
float3 g_vPlayerColDesc2;	//  赤->オーバードライブ時　青->覚醒 　緑->エッジが赤か青か

/*************************************/
// アウトラインに必要な変数
/*************************************/
float    g_fOutlineSize = 0.005f;
float3   g_vEdgeColor = float3(0.0f, 0.0f, 0.0f);
//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT_OUTLINE
{
	float4 Pos		: POSITION;
	float2 Tex		: TEXCOORD0;
	float4 Color	: COLOR;
};

/*************************************/
// アウトライン用の頂点シェーダー
/*************************************/

VS_OUTPUT_OUTLINE VS_OutLine(VS_INPUT In) //:POSITION テクスコードの情報がなくなるので書かない
{

	VS_OUTPUT_OUTLINE OUT = (VS_OUTPUT_OUTLINE)0;

	OUT.Pos = mul(In.Pos, g_mWVPMatrix);
	OUT.Pos.xy += normalize(mul(mul(float4(In.Normal, 0), g_mWMatrix), g_mVPMatrix).xy) * OUT.Pos.z * g_fOutlineSize;
	OUT.Pos.z += 0.001f;

	OUT.Tex = In.Tex;

	return OUT;
}

/*************************************/
// アウトライン用のピクセルシェーダー
/*************************************/

PS_TONEMAP PS_OutLine(VS_OUTPUT_OUTLINE In)
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	float4 col = tex2D(DecaleSamp, In.Tex); //そのキャラクターのエッジ付近の色に

	//OUT.color.rgb = col.rgb - float3(0.25,0.5,0.5);// ふちは少し暗く	
	// [11/27] 操作しているキャラクターを見失うとの意見が少しあったので淵の色を変える工夫をしてみます。
	//OUT.color.rgb = g_vEdgeColor;
	OUT.color.rgb = float3(0.95f, 0.1f, 0.0f)*g_vPlayerColDesc2.b;
	OUT.color.rgb += float3(0.0f, 0.65f, 1.0f)*(1.0f - g_vPlayerColDesc2.b);
	OUT.color.a = 1.0f;

	OUT.high = col - 1;
	//OUT.high.rgb = float3(0, 1, 1.5);
	OUT.high.a = 1;

	return OUT;
}


technique OutLine
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CW;				// カリングの仕様


		VertexShader = compile vs_3_0 VS_OutLine();
		PixelShader = compile ps_3_0 PS_OutLine();
	}
}




//------------------------------------------------------
//		プレイヤー用のピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_Player(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
		lightCol += tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);


	//OUT.color.g += 0.5;



	//トーンマッピング
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	//高輝度抽出後にしないとHDRで光ってしまうので最後に
	OUT.color.rgb += g_InvincibleColRate;

	return OUT;
}

//------------------------------------------------------
//		プレイヤー用のテクニック
//------------------------------------------------------
technique Player
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		//SrcBlend = SrcAlpha;		// 現在描いてる方
		//DestBlend = InvSrcAlpha;	// 描かれている方
		SrcBlend = One;				//1,1,1,1
		DestBlend = Zero;			//0,0,0,0
		CullMode = CCW;				// カリングの仕様
		AlphaRef = 0x00000080;
		AlphaFunc = GREATEREQUAL;	// αがAlphaRef以上ならOK
		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Player();
	}
}


//------------------------------------------------------
//		プレイヤー用のピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_ToonPlayer(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
	// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	//float4 lightCol = tex2D(LightSamp, ScreenTex);
	//float4 PLlightCol = tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb += PLlightCol.rgb;
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	// ライト率(ハーフランバート)
	float rate = (dot(Normal, -g_vViewLightVec));
	float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
		OUT.color.rgb *= toonShadowCol;

	//OUT.color.g += 0.5;
	float3 E = Pos.xyz;// ビューの目線
		E = normalize(E);
	float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)), 4.0f);
	float RimLightPower = max(.0f, dot(-E, g_vViewLightVec));

	//トーンマッピング
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
	OUT.high.a = 1.0f;

	// キャサリンっぽい白いリム
	OUT.color.rgb += (RimPower)* float3(1, 1, 1);

	// オーバードライブ用
	//float RimPower2 = pow(1.0f - max(0.0f, dot(-E, Normal)), 1.0f);
	OUT.high.rgb += float3(0.8, 0.5, 0.0)*g_vPlayerColDesc.g/*g_OrangeColRate*/;
	OUT.high.rgb += float3(0.7, 0.0, 0.4)*g_vPlayerColDesc.b/*g_MagentaColRate*/;
	OUT.high.rgb += float3(0.0, 0.1, 0.4)*g_vPlayerColDesc2.r/*g_OverDriveColRate*/;
	OUT.high.rgb += float3(0.25f, 0.0, 0.0)*g_vPlayerColDesc2.g/*g_WillPowerRate*/;

	//高輝度抽出後にしないとHDRで光ってしまうので最後に
	OUT.color.rgb += g_vPlayerColDesc.r/*g_InvincibleColRate*/;

	return OUT;
}

//------------------------------------------------------
//		プレイヤー用のテクニック
//------------------------------------------------------
technique PlayerToon
{
	pass OutLine
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = false;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_OutLine();
		PixelShader = compile ps_3_0 PS_OutLine();
	}

	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_ToonPlayer();
	}
}


//------------------------------------------------------
//		プレイヤー用のピクセルシェーダー2	
//------------------------------------------------------
PS_TONEMAP PS_ToonPlayerNoRim(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//スクリーン空間をテクスチャ座標に  NDC->UV y反転
const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
// 必要な情報を取得
const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

//	ピクセル色決定
OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

//float4 lightCol = tex2D(LightSamp, ScreenTex);
//float4 PLlightCol = tex2D(PLSSamp, ScreenTex);
//OUT.color.rgb += PLlightCol.rgb;
//OUT.color.rgb *= lightCol;
//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
//OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

// ライト率(ハーフランバート)
float rate = (dot(Normal, -g_vViewLightVec));
float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
OUT.color.rgb *= toonShadowCol;

//OUT.color.g += 0.5;
float3 E = Pos.xyz;// ビューの目線
E = normalize(E);
float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)), 4.0f);
float RimLightPower = max(.0f, dot(-E, g_vViewLightVec));

//トーンマッピング
OUT.color.rgb *= exp2(g_fExposure);

OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));
OUT.high.a = 1.0f;

// オーバードライブ用
//float RimPower2 = pow(1.0f - max(0.0f, dot(-E, Normal)), 1.0f);
OUT.high.rgb += float3(0.8, 0.5, 0.0)*g_vPlayerColDesc.g/*g_OrangeColRate*/;
OUT.high.rgb += float3(0.7, 0.0, 0.4)*g_vPlayerColDesc.b/*g_MagentaColRate*/;
OUT.high.rgb += float3(0.0, 0.1, 0.4)*g_vPlayerColDesc2.r/*g_OverDriveColRate*/;
OUT.high.rgb += float3(0.25f, 0.0, 0.0)*g_vPlayerColDesc2.g/*g_WillPowerRate*/;

//高輝度抽出後にしないとHDRで光ってしまうので最後に
OUT.color.rgb += g_vPlayerColDesc.r/*g_InvincibleColRate*/;

return OUT;
}

//------------------------------------------------------
//		プレイヤー用のテクニック2
//------------------------------------------------------
technique PlayerToonNoRim
{
	pass OutLine
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = false;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_OutLine();
		PixelShader = compile ps_3_0 PS_OutLine();
	}

	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_ToonPlayerNoRim();
	}
}



//------------------------------------------------------
//		ペルソナ用のピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_Persona(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
	// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	float4 lightCol = tex2D(LightSamp, ScreenTex);
		lightCol += tex2D(PLSSamp, ScreenTex);
	OUT.color.rgb *= lightCol;
	OUT.color.rgb += tex2D(SpecSamp, ScreenTex);

	// ライト率(ハーフランバート)
	//float rate = (dot(Normal, -g_vViewLightVec));
	//float HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 toonShadowCol = tex2D(ToonShadowSamp, float2(HalfLambert, 0.0f));
	//	OUT.color.rgb *= toonShadowCol;

		// ピクセルの色
		//OUT.color.rgb = (HalfLambert * g_vLightColor);
	

	// 青いリムライティング
	float3 E = Pos.xyz;// ビューの目線
	E = normalize(E);

	float RimPower = pow(1.0f - max(0.0f, dot(-E, Normal)),2.0f);

	// ペルソナの青い光
	OUT.color.rgb += RimPower * float3(0, 0.4, 2.5);

	//トーンマッピング
	OUT.color.rgb *= exp2(g_fExposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_vBloomColor));

	// ペルソナの青い光
	//OUT.high.rgb += RimPower * float3(0,1.3, 1.5);

	OUT.high.a = 1.0f;

	//高輝度抽出後にしないとHDRで光ってしまうので最後に
	//OUT.color.rgb += g_InvincibleColRate;



	return OUT;
}

//------------------------------------------------------
//		ペルソナテクニック
//------------------------------------------------------
technique Persona
{
	//pass OutLine
	//{
	//	ZEnable = true;				// 奥行考慮
	//	ZWriteEnable = false;		// 奥行を書き込むか

	//	AlphaBlendEnable = true;	// アルファブレンド考慮
	//	BlendOp = Add;				// ブレンド仕様
	//	SrcBlend = SrcAlpha;		// 現在描いてる方
	//	DestBlend = InvSrcAlpha;	// 描かれている方
	//	CullMode = CW;				// カリングの仕様

	//	VertexShader = compile vs_3_0 VS_OutLine();
	//	PixelShader = compile ps_3_0 PS_OutLine();

	//}

	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Persona();
	}



}

//********************************************************************
//
//		基本３Ｄシェーダー		
//
//********************************************************************

//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT_FINAL VS_Sky(VS_INPUT In)
{
	VS_OUTPUT_FINAL Out = (VS_OUTPUT_FINAL)0;
	//TransMatrixとPosを合成してwPosの情報生成
	Out.wPos = mul(In.Pos, g_mWMatrix);

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.wvpPos = Out.Pos;

	return Out;
}
//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_Sky(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

	// 必殺暗転の値
	OUT.color.rgb *= g_fOverDriveDim;

	//トーンマッピング
	OUT.color.rgb *= exp2(g_fExposure);
	//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
	OUT.high.rgb = float3(0.05f, 0.1f, 0.3f)*g_fOverDriveDim;// 必殺暗転の値
	OUT.high.a = 1.0f;

	return OUT;
}

//------------------------------------------------------
//		空テクニック
//------------------------------------------------------
technique sky
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_Sky();
		PixelShader = compile ps_3_0 PS_Sky();
	}
}

//------------------------------------------------------
//		夜空ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_NightSky(VS_OUTPUT_FINAL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//スクリーン空間をテクスチャ座標に  NDC->UV y反転
//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

//	ピクセル色決定
OUT.color = In.Color * tex2D(DecaleSamp, In.Tex);

// 必殺暗転の値
OUT.color.rgb *= g_fOverDriveDim;

//トーンマッピング
OUT.color.rgb *= exp2(g_fExposure);
//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
OUT.high.rgb = float3(0.035f, 0.045f, 0.075f)*g_fOverDriveDim;// 必殺暗転の値
OUT.high.a = 1.0f;

return OUT;
}
//------------------------------------------------------
//		夜空テクニック
//------------------------------------------------------
technique nightsky
{
	pass P0
	{
		ZEnable = true;				// 奥行考慮
		ZWriteEnable = true;		// 奥行を書き込むか

		AlphaBlendEnable = true;	// アルファブレンド考慮
		BlendOp = Add;				// ブレンド仕様
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_Sky();
		PixelShader = compile ps_3_0 PS_NightSky();
	}
}



//**************************************
//
///		ＵＶアニメーション
//
//**************************************

// UV用
sampler DecaleSampUV = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	//BorderColor = 0x00FFFFFF;
	AddressU = WRAP;
	AddressV = WRAP;
};


struct VS_INPUT_UV
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float2 Tex	  : TEXCOORD0;
	float4 Color	: COLOR0;
};
struct VS_OUTPUT_UV
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float4 wvpPos	: TEXCOORD1;
	float3 Normal	: NORMAL;
};

// UV
float m_fTuAnime = 0.0f;
float m_fTvAnime = 0.0f;
float m_fAlphaUV = 1.0f;

VS_OUTPUT_UV VS_UvAnime(VS_INPUT_UV In)
{
	VS_OUTPUT_UV Out = (VS_OUTPUT_UV)0;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.wvpPos = Out.Pos;
	Out.Normal = In.Normal;
	Out.Color = In.Color;// 頂点カラー取得
	Out.Tex = In.Tex + float2(m_fTuAnime, m_fTvAnime);//座標

	Out.Color.rgb = 1.0f;
	Out.Color.a *= m_fAlphaUV; //　透明度

	return Out;
}


//------------------------------------------------------
///		ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime(VS_OUTPUT_UV In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);
	
	//トーンマッピング
	//OUT.color.rgb *= exp2(g_fExposure); 今は考慮はなしで
	
	// 高輝度抽出
	OUT.high.rgb = max(OUT.color.rgb - 0.45f,0.0f);
	//OUT.high.rgb = float3(0, 0.5, 0);
	OUT.high.a = OUT.color.a;

	return OUT;
}

technique uvAnime
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}


technique uvAnime_add
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = one;
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}

technique uvAnime_add_noz
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = one;
		CullMode = CCW;
		ZEnable = false;			// このオブジェクトはZバッファを考慮しない
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}

//------------------------------------------------------
///		ガード用ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime_Guard(VS_OUTPUT_UV In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//スクリーン空間をテクスチャ座標に  NDC->UV y反転
	//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
	// 必要な情報を取得
	//const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	//const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	//const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);

	// 目線
	//float3 E = Pos.xyz;
	//	E.normalize();

	In.wvpPos.xyz /= In.wvpPos.w;// NDC
	float RimPower = pow(1.0f - max(0.0f, dot(-In.wvpPos.xyz, In.Normal)), 2.0f);
	RimPower *= 3.0f;
	OUT.color.a *= RimPower;
	OUT.color.rgb += float3(0.3, 0.3, 1.0);

	//トーンマッピング
	//OUT.color.rgb *= exp2(g_fExposure); 今は考慮はなしで

	// 高輝度抽出
	OUT.high.rgb = max(OUT.color.rgb - 0.5f, 0.0f);
	//OUT.high.rgb = float3(0, 0.5, 0);
	OUT.high.a = OUT.color.a;

	return OUT;
}

//------------------------------------------------------
///		ガード用テクニック
//------------------------------------------------------
technique uvAnime_guard
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = ONE;// InvSrcAlpha
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime_Guard();
	}
}


//------------------------------------------------------
///		魂オーラ用ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime_Soul(VS_OUTPUT_UV In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

//スクリーン空間をテクスチャ座標に  NDC->UV y反転
//const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);
// 必要な情報を取得
//const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
//const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
//const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

//	ピクセル色決定
OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);

// 目線
//float3 E = Pos.xyz;
//	E.normalize();

In.wvpPos.xyz /= In.wvpPos.w;// NDC
float RimPower = pow(1.0f - max(0.0f, dot(-In.wvpPos.xyz, In.Normal)), 1.0f);
RimPower *= 2.2f;
OUT.color.a *= RimPower;
//OUT.color.rgb += float3(0.3, 0.3, 1.0);

float RimPower2 = pow(max(0.0f, dot(-In.wvpPos.xyz, In.Normal)), 2.5f);
RimPower2 *= 2.5f;
OUT.color.a *= RimPower2;

//OUT.color.rgb += float3(0.1, 0.0, 0.0);

//トーンマッピング
//OUT.color.rgb *= exp2(g_fExposure); 今は考慮はなしで

// 高輝度抽出
OUT.high.rgb = max(OUT.color.rgb - 0.25f, 0.0f);
//OUT.high.rgb = float3(0, 0.5, 0);
OUT.high.a = OUT.color.a;

return OUT;
}

//------------------------------------------------------
///		魂用テクニック
//------------------------------------------------------
technique uvAnime_soul
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = ONE;// InvSrcAlpha
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime_Soul();
	}
}


//------------------------------------------------------
//	エリアのマスク用の頂点構造体
//------------------------------------------------------
struct VS_OUTPUT_UV_WALL
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float4 wvpPos	: TEXCOORD1;
	float2 MaskTex	: TEXCOORD3;
	float3 Normal	: NORMAL;
};

//------------------------------------------------------
//	エリアのマスク
//------------------------------------------------------
texture AreaWallTex;
sampler AreaWallSamp = sampler_state
{
	Texture = <AreaWallTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;// 繰り返す
	AddressV = Wrap;
};


//------------------------------------------------------
///		エリアの壁用頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT_UV_WALL VS_UvAnime_AreaWall(VS_INPUT_UV In)
{
	VS_OUTPUT_UV_WALL Out = (VS_OUTPUT_UV_WALL)0;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);
	Out.wvpPos = Out.Pos;
	Out.Normal = In.Normal;
	Out.Color = In.Color;// 頂点カラー取得
	Out.Tex = In.Tex + float2(m_fTuAnime, m_fTvAnime);//座標
	Out.MaskTex = In.Tex;
	Out.Color.rgb = 1.0f;
	Out.Color.a *= m_fAlphaUV; //　透明度

	return Out;
}

//------------------------------------------------------
///		エリアの壁用ピクセルシェーダー	
//------------------------------------------------------
PS_TONEMAP PS_UvAnime_AreaWall(VS_OUTPUT_UV_WALL In) : COLOR
{
	PS_TONEMAP	OUT = (PS_TONEMAP)0;

	//	ピクセル色決定
	OUT.color = In.Color * tex2D(DecaleSampUV, In.Tex);

	// マスクマップ取得
	float rate = tex2D(AreaWallSamp, In.MaskTex).r;

	// デカールと掛け合わす
	OUT.color.rgb *= rate;

	// ヘキサが映っていないときでも少し色を乗せる
	//OUT.color.a += 0.2f;
	OUT.color.g += 0.1f;
	OUT.color.b += 0.2f;

	//トーンマッピング
	//OUT.color.rgb *= exp2(g_fExposure); 今は考慮はなしで

	// 高輝度抽出
	OUT.high.rgb = max(OUT.color.rgb - 0.45f, 0.0f);
	OUT.high.a = OUT.color.a;

	return OUT;
}

//------------------------------------------------------
///		エリアの壁用テクニック
//------------------------------------------------------
technique uvAnime_areaWall
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = One;
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime_AreaWall();
		PixelShader = compile ps_3_0 PS_UvAnime_AreaWall();
	}
}


//------------------------------------------------------
//		水に移りこむ世界
//------------------------------------------------------
struct VS_INPUT_WATER_REFLECT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

struct VS_OUTPUT_WATER_REFLECT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos		: TEXCOORD1;//　ピクセルに送る情報にワールドからのポジション追加
	float4 wvpPos	: TEXCOORD2;//　ピクセルに送る情報にゲーム画面ラストのポジション追加

	float3 vLight	: TEXCOORD4;
	float3 vE		: TEXCOORD5;

	float3 Normal	: COLOR1;

};

//------------------------------------------------------
///		水に移りこむ世界に必要なパラメーター
//------------------------------------------------------
float g_fWaterHeight = 0.0f;		//	水の高さ


//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT_WATER_REFLECT VS_WATER_REFLECT(VS_INPUT_WATER_REFLECT In)
{
	VS_OUTPUT_WATER_REFLECT Out = (VS_OUTPUT_WATER_REFLECT)0;

	//ローカルの状態でモデルのY軸反転
	In.Pos.y *= -1.0f;

	//TransMatrixとPosを合成してwPosの情報生成
	Out.wPos = mul(In.Pos, g_mWMatrix);

	// ここで水の高さを考慮した座標の反転を行う
	//Out.wPos.y = g_fWaterHeight - (Out.wPos.y - g_fWaterHeight);

	Out.wPos.y += 1;

	Out.Pos = mul(Out.wPos, g_mVPMatrix);

	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.wvpPos = Out.Pos;

	return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	 後でPS_TONE_MAP でやるかも
//------------------------------------------------------
float4 PS_WATER_REFLECT(VS_OUTPUT_WATER_REFLECT In) : COLOR
{
	float4	OUT = (float4)0;

	// 水より上に画像があれば破棄
	clip(g_fWaterHeight - In.wPos.y);

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);

	// 必殺暗転の値
	//OUT.color.rgb *= g_fOverDriveDim;

	//トーンマッピング
	//OUT.color.rgb *= exp2(g_fExposure);
	//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
	//OUT.high.rgb = float3(0.05f, 0.1f, 0.3f)*;// 必殺暗転の値
	//OUT.high.a = 1.0f;

	return OUT;
}

/* 水に移る反射マップ制作*/
technique CrystalWaterReflect
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CW;				// カリングを逆向きにして
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_WATER_REFLECT();
		PixelShader = compile ps_3_0 PS_WATER_REFLECT();
	}

}


// UV値とZ値からワールド空間座標を算出
float3 CalcWorldPosition(float2 UV, float2 zw)
{
	// UV空間
	//      0.0 ---> 1.0
	//	  y ↓				
	//		1.0f

	// NDC空間  UVではｙは↓を向いていたのでｙは反転する必要がある
	//			1.0
	//			↑
	//  -1.0<--- 0.0 ---> 1.0
	//	      y ↓				
	//		   -1.0f

	float4 proj;
	proj.xy = (UV*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* zw.y; //先生に聞こう
																	//proj.xy = UV*2.0f - 1.0f;
																	//proj.y *= -1.0f;
	proj.zw = zw;	// ZとWはそのまま入れる

					// プロジェクション逆行列でビュー座標系に変換　Proj->View
	float4 worldPos = mul(proj, g_mInvVPMatrix);
	//worldPos.xyz /= worldPos.w;		// ワールド空間へはWで割る必要がある

	return worldPos.xyz;
}
//------------------------------------------------------
//		水頂点フォーマット
//------------------------------------------------------

struct VS_INPUT_WATER
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};


struct VS_OUTPUT_WATER
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex1		: TEXCOORD0;
	float2 Tex2		: TEXCOORD3;

	float4 wPos		: TEXCOORD1;//　ピクセルに送る情報にワールドからのポジション追加
	float4 wvpPos	: TEXCOORD2;//　ピクセルに送る情報にゲーム画面ラストのポジション追加

	float3 vLight	: TEXCOORD4;
	float3 vE		: TEXCOORD5;

	float3 Normal	: COLOR1;
	float4 vPos		: TEXCOORD6;//　ピクセルに送る情報にビューからのポジション追加

};

//------------------------------------------------------
///		水のリフレクションマップ
//------------------------------------------------------
texture WaterReflectBuf;
sampler WaterReflectSamp = sampler_state
{
	Texture = <WaterReflectBuf>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	// 横だけミラーにしてごまかす
	AddressU = MIRROR;//MIRROR
	AddressV = WRAP;//MIRROR
};


// グローバルパラメーター
float g_fUvWater = 0.0f;
//float3 wLightVec = { 1.0f, -1.0f, 1.0f };// 

//------------------------------------------------------
//		水頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT_WATER VS_WATER(VS_INPUT_WATER In)
{
	VS_OUTPUT_WATER Out = (VS_OUTPUT_WATER)0;
	//WMatrixとPosを合成してwPosの情報生成
	Out.wPos = mul(In.Pos, g_mWMatrix);
	Out.vPos = mul(In.Pos, g_mWMatrix);
	
	//  プロジェクション空間の座標を渡します
	Out.Pos = Out.wvpPos = mul(In.Pos, g_mWVPMatrix);
	//Out.wvpPos = Out.Pos;

	// 水の波用に2つテクスチャー座標に
	Out.Tex1 = In.Tex + float2(-g_fUvWater*1.5f, -g_fUvWater * 1.5f);
	Out.Tex2 = In.Tex + float2(g_fUvWater, g_fUvWater*1.51f);

	// 一応色も使える用にしとく　今は特に使わないので1
	Out.Color = 1.0f;

	//	法線変換	
	float3x3 mat = g_mWMatrix;			// ↓の演算をするため3x3に変更
	float3 N = mul(In.Normal, mat);	//	ローカルの法線とワールド行列掛け合わせてワールド空間の法線作ります
	Out.Normal = normalize(N);		// 正規化しときます。
	

	//	頂点ローカル座標系算出
	float3	T;
	float3	B = { .0f,1.0f,0.0001f};// ダミーでY軸のベクトルを作る
	T = cross(B, N);				// まずN(z軸)と仮のY軸で外積しXを入手
	T = normalize(T);				// Xの値をベクトルに変換
	B = cross(T, N);				// N(z軸)と入手したX軸で外積してY軸を入手
	B = normalize(B);				// Y軸の値をベクトルに

	//　ワールドから接空間へもどす必要がある
	//　ライトベクトルを接空間へ
	//　回転軸だけの逆行列は転置するだけでいいので　内積に置く
	Out.vLight.x = dot(T, g_vWLightVec);	// ワールド空間のベクトルをTと内積して　	接空間のＸ軸を作る
	Out.vLight.y = dot(B, g_vWLightVec);	// ワールド空間のベクトルをBと内積して　	接空間のＹ軸を作る
	Out.vLight.z = dot(N, g_vWLightVec);	// ワールド空間のベクトルをNと内積して　	接空間のＺ軸を作る
	Out.vLight = normalize(Out.vLight); // 最後に正規化してベクトルへ


										// 視線ベクトルを接空間へ
	float3 E = Out.wPos - g_vViewPos;		// 視線ベクトル
	Out.vE.x = dot(T, E);				// 同じく接空間のＸ軸を作る
	Out.vE.y = dot(B, E);				// 同じく接空間のＹ軸を作る
	Out.vE.z = dot(N, E);				// 同じく接空間のＺ軸を作る
	Out.vE = normalize(Out.vE);			// 最後に正規化してベクトルへ

	return Out;
}
//------------------------------------------------------
//		水ピクセルシェーダー
//------------------------------------------------------
float4 PS_WATER(VS_OUTPUT_WATER In) : COLOR
{
	float4 OUT = (float4)0;

	//UV２つ作成
	float2 UV1 = In.Tex1;
	float2 UV2 = In.Tex2;

	//**********************************************************
	// 視差マッピング
	//**********************************************************  
	// マルチマップのピクセルを今回は視差マップとして参照し、高さを取得する
	float h = tex2D(MultiSamp, UV1).r - 0.5f;
	h += tex2D(MultiSamp, UV2).r - 0.5f;
	h *= 0.5;//　二つサンプリングしてるから平均を取ってくる

	// テクセルを頂点座標系での視線ベクトル方向に重みをつけてずらす。
	float3 E = normalize(In.vE);//目線のベクトル
	UV1 -= 0.02f * h * E.xy /*+ m_fUvSea*/;//あとで数値上げる
	UV2 -= 0.02f * h * E.xy /*+ m_fUvSea*/;

	/*************************/
	//	視差適用後に 法線取得
	/*************************/

	//法線マップを参照し、法線を視差分ずらし取得する
	float3 NMap = (tex2D(NormalSamp, UV1).rgb - 0.5f)*2.0f;
	NMap += (tex2D(NormalSamp, UV2).rgb - 0.5f)*2.0f;
	NMap *= 0.5f;// ↑で2回足してるので半減させる

	//	視線反射ベクトル
	float3 R = reflect(-E, NMap);

	//	ライト計算
	//In.vLight = normalize(In.vLight);
	float3 light;
	float rate = max(0.0f, dot(-In.vLight, NMap));//　光の計算　内積で光の反射を求める
	light = rate;//

				 //	スペキュラ
	float SpecPower = 1.0f;
	float S = pow(max(0, dot(R, In.vLight)), 80) * (SpecPower);

	// スペキュラ加算
	OUT.rgb += S;

	//******************************************************
	/// G_Bufferを合わせ取得する
	//******************************************************
	float2 G_Fetch = (In.wvpPos.xy / In.wvpPos.w)*0.5f + 0.5f;
	G_Fetch.y = -G_Fetch.y;



	//******************************************************
	/// Z値取得　水の
	//******************************************************
	////float fZ = tex2D(DepthBufSampWater, G_Fetch).r;	// 深度バッファから奥行取得
	////float Z = 1 / fZ;
	//
	//// 必要な情報を取得
	const float4 NormalDepth = tex2D(NormalDepthSamp, G_Fetch);
	//const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);
	//const float3 envPos = CalcViewPosition(G_Fetch, NormalDepth.zw);
	const float3 envPos = CalcViewPosition(G_Fetch, NormalDepth.zw);
	////float4 proj;
	////proj.xy = (G_Fetch*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* NormalDepth.zw.y;
	////proj.zw = NormalDepth.zw;	// ZとWはそのまま入れる
	//// プロジェクション逆行列でビュー座標系に変換　Proj->View
	////float4 worldPos = mul(proj, g_mInvVPMatrix);
	////worldPos.xyz /= worldPos.w;		// ワールド空間へはWで割る必要がある


	////ZPos.xyz /= ZPos.w;
	//// PLのPosとこのピクセルの位置で←ポジション＆距離
	////float3 g_vViewLightVec = g_vPLSpos.xyz - (Pos.xyz);
	////float dist = pow(max(0.0f, 1.0f - (length(g_vViewLightVec) / g_fPLSrange)), 2);//←数値をいじり絞る

	//// 高さのみ
	//float waterLen = length(g_vViewPos.y - g_fWaterHeight);

	//float vLen = length(envPos-In.vPos.xyz);

	////
	//float Zdist= length (waterLen - worldPos.y);
	////  ベクトルの長さを三平方の定理から求め、求めたベクトルの長さを返す
	////float Zdist = sqrt(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
	//

	/**************************************/
	//	取得した奥行でFOG
	/**************************************/
	// 深度が深ければ深いほど透明感をなくす

	//float ZNear = 100.0f;
	//float ZFar = 300.0f;
	////float ZNear = 0.99f;
	////float ZFar = 1.1f;
	//float3 ZCol = { 0.0f, 1.0f, 0.0f };
	//float ZRate = smoothstep(ZNear, ZFar, Zdist);

	//// フォグの色をフォグの値を入れる 
	//ZCol = ZCol * (ZRate);
	//OUT.rgb += ZCol;

	//******************************************************
	/// 屈折の効果
	//******************************************************

	// 歪み度
	float distortion = 0.01f;

	// 環境度
	float3 Env = tex2D(EnvFullBufSamp, G_Fetch + (float2(NMap.x, NMap.y)*distortion));
	
	//深度値を利用してマスクを作る
	//float G_Depth = tex2D(DepthBufSampWater, G_Fetch + (float2(NMap.x, NMap.y)*distortion)).r;
	float4 proj;
	proj.xy = (G_Fetch*float2(2.0f, -2.0f) + float2(-1.0f, 1.0f))* NormalDepth.zw.y;
	proj.zw = NormalDepth.zw;	// ZとWはそのまま入れる
	// プロジェクション逆行列でビュー座標系に変換　Proj->View
	float4 worldPos = mul(proj, g_mInvVPMatrix);
	worldPos.xyz /= worldPos.w;		// ワールド空間へはWで割る必要がある
	
	// (TODO) 後で
	//float G_Depth = proj.z / proj.w;
	//float G_Depth = envPos.z;
	////float myDepth = In.wvpPos.z / In.wvpPos.w;			//wで割って-1~1に変換する
	//float myDepth = In.vPos.z;

	//// (if文)手前のPixel情報なら手前のオブジェクトの情報をとってこない処理
	//if (myDepth > G_Depth)
	//{
	//	Env.rgb = tex2D(EnvFullBufSamp, G_Fetch);
	//}

	
	Env *= 0.7f;// 色を暗くする度あい
	OUT.rgb += Env;// lerp(Env, OUT.rgb, 0.3f);


	//******************************************************
	/// フレネル反射
	//******************************************************

	// 水の法線ベクトルと始点ベクトルで内積　
	float fresnel = dot(normalize(g_vViewPos - In.wPos), In.Normal);

	// 線形補間
	//float3 RefCol = { -0.1f, -0.08f, -0.0f };
	float3 RefCol = { 0.0f, 0.02f, 0.1f };
	float3 SeaCol = { 0.0f, 0.0f, 0.0f };
	float AbujustF = +0.05f;					// フレネル反射の調整パラメーター　上げると弱まるが上まで見下ろすと-の値貫通してが描画される[1207]maxminで解決

	RefCol += tex2D(WaterReflectSamp, G_Fetch + (float2(NMap.x, NMap.y)*distortion));

	// 描画
	float3 fresnelCol;
	fresnelCol = lerp(RefCol, SeaCol, max(0.0f, min(1.0f, fresnel + AbujustF)));
	OUT.rgb += fresnelCol;

	/**************************************/
	//
	/**************************************/

	//OUT.rgb += ZCol;

	OUT.a = 1.0f;
	return OUT;
}

/* 水 */
technique CrystalWater
{
	pass p0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 VS_WATER();
		PixelShader = compile ps_3_0 PS_WATER();

	}

}




//**************************************************************************************************
//
//		海
//
//**************************************************************************************************

//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT_SEA
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex1		: TEXCOORD0;
	float2 Tex2		: TEXCOORD1;

	//行列
	float4 wPos		: TEXCOORD2;//	ピクセルに送る情報にワールドからのポジション追加
	float4 RocalPos	: TEXCOORD3;//	ピクセルに送る情報にゲーム画面ラストのポジション追加

	//ライティング
	//float4 Light	: COLOR1;
	float4 Ambient	: COLOR2;
	float3 Normal : COLOR3;
	float3 vLight	: TEXCOORD4;
	float3 vE		: TEXCOORD5;

	//追加　座標系
	float4 ProjectionPos : TEXCOORD6;
};


struct VS_INPUT_SEA
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float2 Tex	  : TEXCOORD0;
};

////------------------------------------------------------
////		フォグ関連
////------------------------------------------------------
//float	AlphaNear = 10.0f;
//float	AlphaFar = 200.0f;
//float	SeaAlpha = 0.0f;//

float m_fUvSea = 0.0f;
//float3 m_fSeaColor = { 0.2f, 0.8f, 1.7f };
float3 m_fSeaColor = { 0.3f, 0.65f, 1.1f };


inline float4 CalcHemiLight(float3 normal)//　上下の環境光を分ける
{
	float4 color;
	float rate = (normal.y*0.5f) + 0.5f;
	color.rgb = g_vSkyColor * rate;
	color.rgb += g_vGroundColor * (1 - rate);
	color.a = 1.0f;

	return color;
}

inline float3 CalcDirLight(float3 dir, float3 normal)//　ライトの向きで光の色の強さを返す
{
	float3 light;
	float rate = max(0.0f, dot(-dir, normal));//　光の計算　内積で光の反射を求める
	light = g_vLightColor * rate;//

	return light;
}

//------------------------------------------------------
//	環境マップ用　周りの風景が映りこむマップ
//------------------------------------------------------
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
//　EnvSamp　はここを通して使用↓　　
inline float4 CalcEnvironment(float3 normal)
{
	float4	color;

	float2	uv;
	uv.x = normal.x*0.5f + 0.5f;
	uv.y = normal.y*0.5f + 0.5f;

	color = tex2D(EnvSamp, uv);
	return color;
}


VS_OUTPUT_SEA VS_Sea(VS_INPUT_SEA In)
{
	VS_OUTPUT_SEA Out = (VS_OUTPUT_SEA)0;

	//ローカル座標系上での頂点の座標
	Out.RocalPos = In.Pos;

	Out.Pos = mul(In.Pos, g_mWVPMatrix);//プロジェクション行列と合成
	Out.ProjectionPos = Out.Pos;

	Out.Color = 1.0f;//In.Color;
	Out.Tex1 = In.Tex + float2(-m_fUvSea*1.5, -m_fUvSea * 1.5);
	Out.Tex2 = In.Tex + float2(m_fUvSea, m_fUvSea*1.5);


	float4 P = mul(In.Pos, g_mWMatrix);//ワールド座標に持っていく
	Out.wPos = mul(In.Pos, g_mWMatrix);//
	float3x3	mat = g_mWMatrix;//float4x4のTransMatrixの行列を　float3x3のmatに
								  //法線ベクトル
	Out.Normal = mul(In.Normal, mat);//
	Out.Normal = normalize(Out.Normal);//

									   // 半球ライティング
	//Out.Ambient.rgb = CalcHemiLight(Out.Normal);

	// 頂点ローカル座標系算出
	float3	vx;
	float3	vy = { 0, 1, 0.001f };
	vx = cross(vy, Out.Normal);
	vx = normalize(vx);
	vy = cross(vx, Out.Normal);
	vy = normalize(vy);

	//	ライトベクトル補正 ライトが当たった時の角度
	Out.vLight.x = dot(vx, g_vWLightVec);			// ライトベクトルを頂点座標系に変換する
	Out.vLight.y = dot(vy, g_vWLightVec);			// ライトベクトルを頂点座標系に変換する
	Out.vLight.z = dot(Out.Normal, g_vWLightVec);	// ライトベクトルを頂点座標系に変換する
	Out.vLight = normalize(Out.vLight);

	// 視線ベクトル補正
	float3 E = P - g_vViewPos;			 // 視線ベクトル
	Out.vE.x = dot(vx, E);			 // 視線ベクトルを頂点座標系に変換する
	Out.vE.y = dot(vy, E);			 // 視線ベクトルを頂点座標系に変換する
	Out.vE.z = dot(Out.Normal, E);   // 視線ベクトルを頂点座標系に変換する
	Out.vE = normalize(Out.vE);//目線

							   ////	フォグ計算
							   //Out.Ambient.a = (AlphaFar - Out.Pos.z) / (AlphaFar - AlphaNear);
							   //Out.Ambient.a = saturate(Out.Ambient.a);//指定された値を 0 ～ 1 の範囲にクランプします

	return Out;
}

float4 PS_Sea(VS_OUTPUT_SEA In) : COLOR
{
	float4	OUT;
//UV２つ作成
float2 UV1 = In.Tex1;
float2 UV2 = In.Tex2;

//**********************************************************
// 視差マッピング
//**********************************************************  
// 高さマップを参照し、高さを取得する
float h = tex2D(MultiSamp, UV1).r - 0.5f;
h += tex2D(MultiSamp, UV2).r - 0.5f;
h *= 0.5;//　二つサンプリングしてるから平均を取ってくる

		 // テクセルを頂点座標系での視線ベクトル方向に重みをつけてずらす。
float3 E = normalize(In.vE);// 目線
UV2 -= 0.02f * h * E.xy + m_fUvSea;
UV1 -= 0.02f * h * E.xy + m_fUvSea;// あとで数値上げる

/*************************/
//	視差適用後に 法線取得
/*************************/
//法線マップを参照し、法線を取得する
float3 NMap = (tex2D(NormalSamp, UV1).rgb - 0.5f)*2.0f;
NMap += (tex2D(NormalSamp, UV2).rgb - 0.5f)*2.0f;
NMap *= 0.5f;

////	ライト計算はしない！　今はじかうち
In.vLight = normalize(In.vLight);
//float3 light = CalcDirLight(In.vLight, NMap);
//light *= 0.5f;

//In.Color.rgb = light;//Ambient=環境光＋
////	ピクセル色決定
//OUT *= In.Color;

OUT = tex2D(DecaleSamp, UV1) + tex2D(DecaleSamp, UV2);
OUT *= 0.5;//　二つサンプリングしてるから平均を取ってくる
OUT.rgb *= m_fSeaColor;// +light;// 海の色指定
OUT.a = 1.0f;// 透明度

/*************************/
//	スペキュラマップ取得
/*************************/
float4 sp_tex = tex2D(LightSamp, UV1);
sp_tex += tex2D(LightSamp, UV2);
sp_tex *= 0.5;//　二つサンプリングしてるから平均を取ってきてる

			  //	視線反射ベクトル
float3 R = reflect(-E, NMap);
//	スペキュラ
float3 S;
S = pow(max(0, dot(R, In.vLight)), 60) * (sp_tex);

//************************************************************
//　屈折
//************************************************************
//ゆがみ量の計算
//水の厚みが薄くなるほど屈折しないようにする
//float4 Offset = float4((Nmap - E).xy * 20, 0.0f, 0.0f);
//　射影テクスチャ用
//float3 wvpNormal = normalize(mul(Nmap, (float3x3)Projection));
//float2 TexCoord;
//TexCoord = (In.Pos.xy + wvpNormal.xy * 1) / In.Pos.z;
//TexCoord = TexCoord * float2(0.5f, -0.5f) + 0.5f;

//海底の色
//float3 SeabedCol;
//SeabedCol = tex2Dproj(TEXSamp, TexCoord );
//OUT.rgb *= SeabedCol;

//	環境マップ　Rを入れ反射マップに
float3 env = CalcEnvironment(R) * (0.75f - sp_tex.a);
OUT.rgb = OUT.rgb * env + OUT.rgb;
//*********************************************************************************************************
//フレネル効果
//*********************************************************************************************************
//頂点 → 視点ベクトル と 法線ベクトルの内積を計算
//float3 pos = In.Pos;
//float fresnel = dot(normalize(-E - In.RocalPos), normalize(In.Normal));

//////内積の結果が 0.0f に近いほど反射マップの色が強くなり、 1.0f に近いほど海底マップの色が強くなるように線形合成する
//float4 Col;
//float3 ReflectCol = float3(0.8, 0.5, 0.8);
//float3 SeabedCol = float3(0.0, 0.9, 0.2);
//
//Col.rgb = lerp(ReflectCol, SeabedCol, fresnel);
////Col.rgb = Col.rgb * Diffuse * WaterCol + S;
//OUT.rgb = Col.rgb;

//*********************************************************************************************************
//透明度
//*********************************************************************************************************  
//水の厚みを計算する
//float Z = SeabedZ - In.PosWVP.z / m_ZF;
//Ｚ値が小さくなるほど透明度を高くする（注意１）
//Z = min(Z * 20.0f, 1.0f);
//Col.rgb = lerp(SeabedCol, Col.rgb, Z);

OUT.rgb += S;

//　射影テクスチャ用
//float3 wvpNormal = normalize(mul(In.Normal, (float3x3)Projection));
//	ref = (In.wPos.xy + wvpNormal.xy * 1) / In.wPos.z;
//output.TexCoord = output.TexCoord * float2(0.5f, -0.5f) + 0.5f;

////リフレクトマップ
//float2 ref = (In.ProjectionPos.xy / In.ProjectionPos.w)*0.5f + 0.5f;
//ref.y = -ref.y;
//float3 Env = tex2D(RefSamp, ref - 0.2f * h * E.xy/*←+目線*/) * (OUT.rgb);
//Env *= 0.35f;
//OUT.rgb += Env;// lerp(Env, OUT.rgb, 0.3f);

			   //	フォグ採用
			   //OUT.a = ((1 - In.Ambient.a));


// 必殺暗転の値
OUT.rgb *= g_fOverDriveDim;

return OUT;
}

technique Sea
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Sea();
		PixelShader = compile ps_3_0 PS_Sea();
	}
}

