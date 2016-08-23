//------------------------------------------------------
//		変換行列
//------------------------------------------------------
float4x4 WMatrix;		// ワールド変換行列
float4x4 VMatrix;		// ビュー変換行列
float4x4 PMatrix;		// プロジェクション変換行列

float4x4 WVPMatrix;		// 投影変換行列
float4x4 VPMatrix;		// ワールド座標から→プロジェクション座標へ持っていく行列

float4x4 InvPMatrix;	// プロジェクション座標 ->View座標　へ戻す逆行列
float4x4 InvVMatrix;	// View座標 ->ワールド座標　へ戻す逆行列
float4x4 InvVPMatrix;	// プロジェクション座標 -> ワールド座標 へ戻す逆行列

float4x4 ShadowProjection; // 影

//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float3	ViewPos;		// ワールド座標の目のポジション

float3 LightVec;		// 平行光の向き
float3 ViewLightVec;	// ビュー座標での光りの向き
float3 LightColor = { 1.0f, 1.0f, 1.0f };

float3 SkyColor = { .4f, .6f, .6f };
float3 GroundColor = { .6f, .4f, .4f };

float RimPowerRate = 2.0f;		 // リムライトの光の幅 
float EmissivePowerRate = 2.0f;// エミッシブの幅 エミッシブがいらない時はテクス茶を変えずこの数値を下げる
// 今は0に

float FogNear = 514.0f;
float FogFar = 1024.0f;

float3 FogColor = { 100.0f, 100.0f, 100.0f };

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
};


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
//		G_Buffer
//*****************************************

VS_DEFERRED VS_G_Buffer(VS_INPUT In)
{
	VS_DEFERRED Out = (VS_DEFERRED)0;

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = In.Color;
	Out.Color.a = 1.0f;

	// スクリーン座標での深度値保存
	Out.Depth = Out.Pos;

	// 法線情報の修正
	float3x3 WVmat = (float3x3)mul(WMatrix, VMatrix); // モデル→ワールド→ビュー空間への変換行列

	// ビュー行列の法線ができあがる
	float3 N = mul(In.Normal, WVmat);// 
	N = normalize(N);
	Out.Normal = N;

	// ビュー行列の法線のタンジェントとバイノーマルを作る

	// 頂点スクリーン座標系算出
	float3	vx;
	float3	vy = { .0f, 1.0f, 0.0001f };			// 仮のy方向ベクトル  0.001f は　外積を成功させる為にずらしている　同じ値で外積すると0ベクトルになる

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
	float4 viewPos = mul(proj, InvPMatrix);
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
	float4x4	mat = mul(WMatrix, ShadowProjection);
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
	float rate = (dot(Normal, -ViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	float3 Lambert = max(0, rate);				// Lambert

	// ピクセルの色
	OUT.color.rgb = (HalfLambert * LightColor);
	OUT.color.a = 1.0f;

	//　スペキュラレート取得
	float sp = CalcSpecular(Pos, Normal, ViewLightVec, 80);
	// スペキュラの色をどれくらい乗せるか
	OUT.spec.rgb = sp*LightColor;
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
	float rate = (dot(Normal, -ViewLightVec));
	float3 HalfLambert = pow((rate + 1.0f)*0.5f, 2);	// HalfLambert
	//float3 Lambert = max(0, rate);				// Lambert

	// ピクセルの色
	OUT.color.rgb = (HalfLambert * LightColor);
	OUT.color.a = 1.0f;

	//　スペキュラレート取得
	float specRate = CalcSpecular(Pos, Normal, ViewLightVec, RoughnessLightMap.g * 255);
	// スペキュラの色をどれくらい乗せるか
	OUT.spec.rgb = (specRate*LightColor)*RoughnessLightMap.r;
	OUT.spec.a = 1.0f;

	float4 ShadowTex;
	float ShadowValue = 0.0f;
	float2 depth;		//	バリアンスシャドウマップ

	{
		// 太陽視点から見たプロジェクション座標のオブジェ
		ShadowTex = mul(mul(float4(Pos, 1), InvVMatrix), ShadowProjection);
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

		if (depth.r < fragDepth - 0.001f)
		{
			const float variance = depth.g - depth.r * depth.r;
			ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
		}

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
	OUT.rgb = SkyColor * rate;
	OUT.rgb += GroundColor * (1.0f - rate);
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
float3 PLSpos;		// ※View座標系に変換しておくこと
float  PLSrange = 30.0f;
float3 PLScolor = float3(300, 600, 300);// カラー
float  PLSpower = 2.0f;// パワー

//------------------------------------------------------
//		PointLightSphereの頂点シェーダー
//------------------------------------------------------
VS_POINTLIGHT VS_PointLightSphere(VS_INPUT In)
{
	VS_POINTLIGHT Out = (VS_POINTLIGHT)0;

	// スケールを法線にしたがって拡大 レンジ分
	In.Pos.xyz += In.Normal*PLSrange;
	// 絶対に送る必要がある　ボールのポジション
	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.WPos = mul(In.Pos, WMatrix);

	//　フェッチ用
	Out.wvpPos = Out.Pos;

	return Out;
}


DEF_POINTLIGHT PS_PointLightSphere(VS_POINTLIGHT In)
{
	DEF_POINTLIGHT OUT = (DEF_POINTLIGHT)0;

	//スクリーン空間をテクスチャ座標に ○
	const float2 ScreenTex = In.wvpPos.xy / In.wvpPos.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	// 必要な情報を取得　○
	const float4 NormalDepth = tex2D(NormalDepthSamp, ScreenTex);
	const float3 Normal = CalcNormal(NormalDepth.xy*2.0f - 1.0f);// ○
	const float3 Pos = CalcViewPosition(ScreenTex, NormalDepth.zw);

	// PLのPosとこのピクセルの位置で←ポジション＆距離
	float3 ViewLightVec = PLSpos.xyz - (Pos.xyz);
	float dist = pow(max(0.0f, 1.0f - (length(ViewLightVec) / PLSrange)), 2);//←数値をいじり絞る

	// 法線マップから陰影遮断
	ViewLightVec = normalize(ViewLightVec);// 光<-場所ベクトルへ
	float rate = max(0, dot(Normal.xyz, ViewLightVec.xyz));

	OUT.color.rgb = (PLScolor*(dist*rate))*PLSpower;
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
float TU = 1.0f / 1280.0f, TV = 1.0f / 720.0f;
float BlurValue = 1.0f;

/***************************************/
/*		ガウシアンブラー			　 */
float4 PS_gaussX(float2 Tex : TEXCOORD0) : COLOR
{

	////テクセルを取得   
	//float2 Texel0 = Tex + float2(-TU * 1 * BlurValue, 0.0f);
	//float2 Texel1 = Tex + float2(-TU * 2 * BlurValue, 0.0f);
	//float2 Texel2 = Tex + float2(-TU * 3 * BlurValue, 0.0f);
	//float2 Texel3 = Tex + float2(-TU * 4 * BlurValue, 0.0f);
	//float2 Texel4 = Tex + float2(-TU * 5 * BlurValue, 0.0f);

	//float2 Texel5 = Tex + float2(TU * 1 * BlurValue, 0.0f);
	//float2 Texel6 = Tex + float2(TU * 2 * BlurValue, 0.0f);
	//float2 Texel7 = Tex + float2(TU * 3 * BlurValue, 0.0f);
	//float2 Texel8 = Tex + float2(TU * 4 * BlurValue, 0.0f);
	//float2 Texel9 = Tex + float2(TU * 5 * BlurValue, 0.0f);

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

	Out += tex2D(GaussianSamp, Tex + float2(-TU * 1 * BlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 2 * BlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 3 * BlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 4 * BlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(-TU * 5 * BlurValue, 0.0f))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(TU * 1 * BlurValue, 0.0f))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 2 * BlurValue, 0.0f))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 3 * BlurValue, 0.0f))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 4 * BlurValue, 0.0f))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(TU * 5 * BlurValue, 0.0f))*0.04f;

	return Out;

}

float4 PS_gaussY(float2 Tex : TEXCOORD0) : COLOR
{

	////テクセルを取得   
	//float2 Texel0 = Tex + float2(0.0, -TV * 1 * BlurValue);
	//float2 Texel1 = Tex + float2(0.0, -TV * 2 * BlurValue);
	//float2 Texel2 = Tex + float2(0.0, -TV * 3 * BlurValue);
	//float2 Texel3 = Tex + float2(0.0, -TV * 4 * BlurValue);
	//float2 Texel4 = Tex + float2(0.0, -TV * 5 * BlurValue);

	//float2 Texel5 = Tex + float2(0.0, TV * 1 * BlurValue);
	//float2 Texel6 = Tex + float2(0.0, TV * 2 * BlurValue);
	//float2 Texel7 = Tex + float2(0.0, TV * 3 * BlurValue);
	//float2 Texel8 = Tex + float2(0.0, TV * 4 * BlurValue);
	//float2 Texel9 = Tex + float2(0.0, TV * 5 * BlurValue);

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

	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 1 * BlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 2 * BlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 3 * BlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 4 * BlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f,-TV * 5 * BlurValue))*0.04f;

	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 1 * BlurValue))*0.12f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 2 * BlurValue))*0.10f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 3 * BlurValue))*0.08f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 4 * BlurValue))*0.06f;
	Out += tex2D(GaussianSamp, Tex + float2(0.0f, TV * 5 * BlurValue))*0.04f;

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

float CenterX = 0.0f;
float CenterY = 0.0f;

float BlurPower = 1.0f;
const float IMAGE_SIZE = 512.0f;

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_RadialBlur(float2 Tex:TEXCOORD0) : COLOR
{
	float4	OUT;

	//　放射中心
	float2 ss_center = float2((CenterX + 1.0f) * 0.5f, (-CenterY + 1.0f) * 0.5f);

		//　オフセット
		float2 uvOffset = (ss_center - Tex) * (BlurPower / IMAGE_SIZE);

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
float exposure = 0.0f;

// 高輝度
float3 g_bloomColor = { 0.9f, 0.9f, 0.9f };

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
	Out.wPos = mul(In.Pos, WMatrix);

	Out.Pos = mul(In.Pos, WVPMatrix);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.wvpPos = Out.Pos;

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
	OUT.color.rgb *= exp2(exposure);
	
	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));
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
		SrcBlend = SrcAlpha;		// 現在描いてる方
		DestBlend = InvSrcAlpha;	// 描かれている方
		CullMode = CCW;				// カリングの仕様

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_DefaultLighting();
	}
}

//------------------------------------------------------
//		プレイヤー用のグローバルエリア
//------------------------------------------------------
float g_InvincibleColRate = 1.0f;//Flash そのキャラクターダウン後の点滅のレート　ちか



//------------------------------------------------------
//		ピクセルシェーダー	
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
	OUT.color.rgb *= exp2(exposure);

	OUT.high.rgb = max(float3(0.0f, 0.0f, 0.0f), (OUT.color.rgb - g_bloomColor));
	OUT.high.a = 1.0f;

	//高輝度抽出後にしないとHDRで光ってしまうので最後に
	OUT.color.rgb += g_InvincibleColRate;

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique Player
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

		VertexShader = compile vs_3_0 VS_DefaultLighting();
		PixelShader = compile ps_3_0 PS_Player();
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
	Out.wPos = mul(In.Pos, WMatrix);

	Out.Pos = mul(In.Pos, WVPMatrix);
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

	//float4 lightCol = tex2D(LightSamp, ScreenTex);
	//	lightCol += tex2D(PLSSamp, ScreenTex);
	//OUT.color.rgb *= lightCol;
	//OUT.color.rgb += tex2D(SpecSamp, ScreenTex);
	////OUT.color.rgb += tex2D(PLSSamp, ScreenTex);

	//OUT.color.g += 0.5;

	//トーンマッピング
	OUT.color.rgb *= exp2(exposure);
	//OUT.color.rgb -= float3(0.05f, 0.1f, 0.3f);
	OUT.high.rgb = float3(0.05f, 0.1f, 0.3f);
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
