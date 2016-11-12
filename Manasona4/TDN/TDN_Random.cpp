
#include	"TDNLIB.h"

//*****************************************************
//		ランダム生成クラス
//*****************************************************

std::mt19937 tdnRandom::engine;

void tdnRandom::Initialize()
{
	std::random_device device;
	engine.seed((unsigned long)device());

	// ついでにsrandも
	srand((unsigned int)time(0));
}

std::uniform_int_distribution<int> tdnRandom::Make_distribution(int min_num, int max_num)
{
	return std::uniform_int_distribution<int>(min_num, max_num);
}

std::uniform_real_distribution<float> tdnRandom::Make_distribution(float min_num, float max_num)
{
	return std::uniform_real_distribution<float>(min_num, max_num);
}

int tdnRandom::Get(int min_num, int max_num)
{
	return std::uniform_int_distribution<int>(min_num, max_num)(engine);
}

float tdnRandom::Get(float min_num, float max_num)
{
	return std::uniform_real_distribution<float>(min_num, max_num)(engine);
}

void tdnRandom::GetRandomVector3(Vector3 &out)
{
	const float ax(Get(0, 618) * .01f), ay(Get(0, 618) * .01f);
	// 角度の値によるベクトルを作成
	out.x = sinf(ay) * cosf(ax);
	out.y = sinf(ax);
	out.z = cosf(ay) * cosf(ax);
}