#include "UVEffectManager.h"

/*****************************/
//		UVEffectマネージャー
/****************************/
	
/***********************************************/
// 初期化・解放
/***********************************************/
UVEffectManager::UVEffectManager()
{
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		switch ((UV_EFFECT_TYPE)i)
		{
		case UV_EFFECT_TYPE::WAVE:
			m_baseUVEffect[i] = new WaveEffect();
			break;
		case UV_EFFECT_TYPE::HIT_IMPACT:
			m_baseUVEffect[i] = new HitImpactEffect();
			break;
		case UV_EFFECT_TYPE::PERSONA:
			m_baseUVEffect[i] = new PersonaEffect();
			break;
		case UV_EFFECT_TYPE::IMPACT:
			m_baseUVEffect[i] = new ImpactEffect();
			break;
		case UV_EFFECT_TYPE::SHOCK_WAVE:
			m_baseUVEffect[i] = new ShockWaveEffect();
			break;
		case UV_EFFECT_TYPE::UPPER:
			m_baseUVEffect[i] = new UpperEffect();
			break;
		case UV_EFFECT_TYPE::GUARD:
			m_baseUVEffect[i] = new GuardEffect();
			break;
		case UV_EFFECT_TYPE::RUN:
			m_baseUVEffect[i] = new RunEffect();
			break;
		case UV_EFFECT_TYPE::CONV:
			m_baseUVEffect[i] = new ConvEffect();
			break;
		case UV_EFFECT_TYPE::CONV2:
			m_baseUVEffect[i] = new ConvEffect2();
			break;
		case UV_EFFECT_TYPE::CONV3:
			m_baseUVEffect[i] = new ConvEffect3();
			break;
		case UV_EFFECT_TYPE::CONV4:
			m_baseUVEffect[i] = new ConvEffect4();
			break;
		case UV_EFFECT_TYPE::BURST_BALL:
			m_baseUVEffect[i] = new BurstBallEffect();
			break;
		case UV_EFFECT_TYPE::SPLASH:
			m_baseUVEffect[i] = new SplashEffect();
			break;
		case UV_EFFECT_TYPE::HIT_RING:
			m_baseUVEffect[i] = new HitRingEffect();
			break;
		case UV_EFFECT_TYPE::AIROU_DRILL:
			m_baseUVEffect[i] = new AirouDrillEffect();
			break;
		case UV_EFFECT_TYPE::BREAK_IMPACT:
			m_baseUVEffect[i] = new BreakImpactEffect();
			break;
		case UV_EFFECT_TYPE::FIRE_RING:
			m_baseUVEffect[i] = new FireRingEffect();
			break;
		case UV_EFFECT_TYPE::AIROU_CIRCLE:
			m_baseUVEffect[i] = new AirouCircleGridEffect();
			break;
		case UV_EFFECT_TYPE::OVER_DRIVE_RING:
			m_baseUVEffect[i] = new OverDriveRingEffect();
			break;
		case UV_EFFECT_TYPE::JUMP_WAVE:
			m_baseUVEffect[i] = new JumpWaveEffect();
			break;
		case UV_EFFECT_TYPE::JUMP_SPEED_LINE:
			m_baseUVEffect[i] = new JumpSpeedLineEffect();
			break;
		case UV_EFFECT_TYPE::PRESSURE:
			m_baseUVEffect[i] = new PressureEffect();
			break;
		case UV_EFFECT_TYPE::ORANGE_BURST:
			m_baseUVEffect[i] = new OrangeBurstEffect();
			break;
		default:
			assert(0);	// "そんなエフェクトはない"
			break;
		}
	}

	// ディレイタイマー初期化
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		m_delayTimer[i] = 0;
	}


	/***********************************************************/
	// 多段
	for (int i = 0; i < (int)UV_EFFECT_MULTIPLE_TYPE::ARRAY_END; i++)
	{
		for (int j = 0; j < MULTIPLE_HIT_MAX; j++)
		{
			switch ((UV_EFFECT_MULTIPLE_TYPE)i)
			{
			case UV_EFFECT_MULTIPLE_TYPE::GUARD_WAVE:
				m_pMultipleEffect[i].pBaseUVEffectMultiple[j] = new GuardWaveEffect();
				break;
			case UV_EFFECT_MULTIPLE_TYPE::GUARD_GRID:
				m_pMultipleEffect[i].pBaseUVEffectMultiple[j] = new GuardGridEffect();
				break;
			case UV_EFFECT_MULTIPLE_TYPE::HIT_RIP:
				m_pMultipleEffect[i].pBaseUVEffectMultiple[j] = new HitRipGridEffect();
				break;
			case UV_EFFECT_MULTIPLE_TYPE::HIT_SLASH:
				m_pMultipleEffect[i].pBaseUVEffectMultiple[j] = new HitSlashGridEffect();
				break;
			default:
				break;
			}
			
			// ディレイ初期化
			m_pMultipleEffect[i].iDelayTimer[j] = 0;	
		}	

	}
	

}

UVEffectManager::~UVEffectManager()
{
	// 全部消して
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_baseUVEffect[i]);
	}

	// 多段先輩も消します
	for (int i = 0; i < (int)UV_EFFECT_MULTIPLE_TYPE::ARRAY_END; i++)
	{
		for (int j = 0; j < MULTIPLE_HIT_MAX; j++)
		{
			SAFE_DELETE(m_pMultipleEffect[i].pBaseUVEffectMultiple[j]);
		}
	}

}


/***********************************************/
// 更新
/***********************************************/
void UVEffectManager::Update()
{
	//
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_baseUVEffect[(int)i]->GetUV()->IsAction() == false)continue;// ハジく
		if (--m_delayTimer[(int)i] >= 0){ continue; };// ★先にデクリメントしてディレイ中ならはじく	
		m_baseUVEffect[(int)i]->Update();
	}


	// 多段先輩を更新
	for (int i = 0; i < (int)UV_EFFECT_MULTIPLE_TYPE::ARRAY_END; i++)
	{
		for (int j = 0; j < MULTIPLE_HIT_MAX; j++)
		{
			if (m_pMultipleEffect[i].pBaseUVEffectMultiple[j]->GetUV()->IsAction() == false)continue;// ハジく
			if (--m_pMultipleEffect[i].iDelayTimer[j] >= 0) { continue; };// ★先にデクリメントしてディレイ中ならはじく	

			m_pMultipleEffect[i].pBaseUVEffectMultiple[j]->Update();
		}
	}

}


/***********************************************/
// 描画
/***********************************************/
void UVEffectManager::Render()
{
	for (int i = 0; i < (int)UV_EFFECT_TYPE::ARRAY_END; i++)
	{
		if (m_baseUVEffect[(int)i]->GetUV()->IsAction() == false)continue;// ハジく
		if (m_delayTimer[(int)i] >= 0){  continue; };// ディレイ中ならはじく	
		m_baseUVEffect[(int)i]->Render();
	}

	// 多段先輩を更新
	for (int i = 0; i < (int)UV_EFFECT_MULTIPLE_TYPE::ARRAY_END; i++)
	{
		for (int j = 0; j < MULTIPLE_HIT_MAX; j++)
		{
			if (m_pMultipleEffect[i].pBaseUVEffectMultiple[j]->GetUV()->IsAction() == false)continue;// ハジく
			if (m_pMultipleEffect[i].iDelayTimer[j] >= 0) { continue; };// ディレイ中ならはじく	

			m_pMultipleEffect[i].pBaseUVEffectMultiple[j]->Render();
		}
	}

}

/************************************************/
//	エフェクト追加
/************************************************/
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->Action(pos);
	m_delayTimer[(int)type] = 0;
}

void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale)
{
	m_baseUVEffect[(int)type]->Action(pos, startScale, endScale);
	m_delayTimer[(int)type] = 0;
}

// アングル込みのエフェクト
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type,
    float startScale, float endScale, Vector3 startAngle, Vector3 endAngle)
{
    m_baseUVEffect[(int)type]->Action(pos, startScale, endScale, startAngle, endAngle);
	m_delayTimer[(int)type] = 0;
}

// アングル込みのエフェクト
void UVEffectManager::AddEffect(Vector3 pos, UV_EFFECT_TYPE type,
	float startScale, float endScale, Vector3 startAngle, Vector3 endAngle,int deayTimer)
{
    m_baseUVEffect[(int)type]->Action(pos, startScale, endScale, startAngle, endAngle);
	m_delayTimer[(int)type] = deayTimer;
}


/************************************************/
//	エフェクトを止める
/************************************************/
void UVEffectManager::StopEffect(UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->Stop();
	m_delayTimer[(int)type] = 0;
}



/************************************************/
//	ループエフェクト関連
/************************************************/
void UVEffectManager::AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->ActionRoop(pos);
	m_delayTimer[(int)type] = 0;
}
void UVEffectManager::AddEffectRoop(Vector3 pos, UV_EFFECT_TYPE type, float startScale, float endScale)
{
	m_baseUVEffect[(int)type]->ActionRoop(pos, startScale, endScale);
	m_delayTimer[(int)type] = 0;
}

void UVEffectManager::StopEffectRoop(UV_EFFECT_TYPE type)
{
	m_baseUVEffect[(int)type]->StopRoop();
	m_delayTimer[(int)type] = 0;
}


//+---------------------------------------
//	多段ヒット
//+---------------------------------------


// ポジションのみ
void UVEffectManager::AddMultipleEffect(Vector3 pos, UV_EFFECT_TYPE type)
{
	for (int i = 0; i < MULTIPLE_HIT_MAX; i++)
	{
		if (m_pMultipleEffect[(int)type].pBaseUVEffectMultiple[i]->GetUV()->IsAction() == true)continue;

		m_pMultipleEffect[(int)type].pBaseUVEffectMultiple[i]->Action(pos);
		m_pMultipleEffect[(int)type].iDelayTimer[i] = 0;

		break; // エフェクトを発動したらでる

	}
}


// アングル込みのエフェクト
void UVEffectManager::AddMultipleEffect(Vector3 pos, UV_EFFECT_MULTIPLE_TYPE type,
	float startScale, float endScale, Vector3 startAngle, Vector3 endAngle, int deayTimer)
{
	for (int i = 0; i < MULTIPLE_HIT_MAX; i++)
	{
		if (m_pMultipleEffect[(int)type].pBaseUVEffectMultiple[i]->GetUV()->IsAction() == true)continue;
		
		m_pMultipleEffect[(int)type].pBaseUVEffectMultiple[i]->Action(pos, startScale, endScale, startAngle, endAngle);	
		m_pMultipleEffect[(int)type].iDelayTimer[i] = deayTimer;
		
		break; // エフェクトを発動したらでる

	}

}
