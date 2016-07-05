#include "Number.h"

/***********************************************/
// ���l�\���p�N���X
/***********************************************/

/***********************************************/
// �C���X�^���X��
/***********************************************/
//Number* Number::m_pInstance = nullptr;
//
//Number & Number::GetInstance()
//{
//	if (!m_pInstance)
//	{
//		m_pInstance = new Number();
//	}
//
//	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
//	return *m_pInstance;
//}
//
//void Number::Release()
//{
//	SAFE_DELETE(m_pInstance);
//}


/***********************************************/
// �������E���
/***********************************************/
Number::Number()
{
	m_pic = new tdn2DAnim("Data/Number/Number.png");
	m_pic->OrderJump(4, 1.0f, 1.3f);
	//m_pic->OrderShake(7, 10, 20, 2);
	//m_pic->OrderRipple(12, 1.0, 0.1f);
	//m_pic->OrderShrink(6, 1.0f, 3.0f);

	m_picSize = 64;
	m_picScale = 1;

	m_col.r = 255;
	m_col.g = 255;
	m_col.b = 255;

}

Number::Number(char * name, int picSize)
{
	m_pic = new tdn2DAnim(name);
	m_pic->OrderJump(4, 1.0f, 1.3f);
	//m_pic->OrderShake(7, 10, 20, 2);
	//m_pic->OrderRipple(12, 1.0, 0.1f);
	//m_pic->OrderShrink(6, 1.0f, 3.0f);
	m_picSize = picSize;
	m_picScale = 1;

}

Number::Number(char * name, int picSize, NUM_KIND kind)
{
	m_pic = new tdn2DAnim(name);
	m_pic->OrderJump(4, 1.0f, 1.3f);
	m_picSize = picSize;
	m_picScale = 1;

	switch (kind)
	{
	case Number::NUM_KIND::NORMAL:
		m_pic->OrderJump(4, 1.0f, 1.3f);
		break;
	case Number::NUM_KIND::HEAL:
		m_pic->OrderJump(4, 1.0f, 1.3f);
		break;
	case Number::NUM_KIND::COMBO:
		m_pic->OrderShrink(6, 1.0f, 3.0f);
		break;
	case Number::NUM_KIND::TIMER:
		m_pic->OrderJump(4, 1.0f, 1.3f);
		break;
	default:
		break;
	}

}

Number::~Number()
{
	SAFE_DELETE(m_pic);
}


/***********************************************/
// �X�V
/***********************************************/
void Number::Update()
{
	m_pic->Update();

	// �F�ς�����
	//m_pic->SetRGB(m_col.r, m_col.g,m_col.b);

}


/***********************************************/
// �`��
/***********************************************/
void Number::Render()
{

}

void Number::Render(int x, int y, int num, NUM_KIND kind)
{
	int number = num;

	//int DIGIT_MAX = 0;
	//int calcNum = num;
	//for (int i = 0;; i++)
	//{	
	//	calcNum = (int)(calcNum / 10);// ���l�̈�ԏ�������������
	//	if (calcNum <= 0)break;
	//	DIGIT_MAX++;
	//}

	// �����̎�ނɂ�蕪��
	switch (kind)
	{
	case Number::NUM_KIND::NORMAL:
		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pic->Render(x - ((count * (m_picSize /2))*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}
		break;
	case Number::NUM_KIND::HEAL:
		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pic->Render(x - ((count *32)*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}
		break;
	case Number::NUM_KIND::TIMER:
	{
		// ���@�ł���
		float PiccScale = 1.5f;
		m_pic->SetScale(PiccScale);

		int count = 0;
		for (;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pic->Render(x - ((count * (int)(32* PiccScale))*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}

		m_pic->Render(x - ((++count * (34 * (int)PiccScale))*m_picScale)+8, y, m_picSize*m_picScale, m_picSize*m_picScale, 10 * m_picSize, 0, m_picSize, m_picSize);// �����`��
		m_pic->Render(x - ((++count * (36 * (int)PiccScale))*m_picScale)+8, y, m_picSize*m_picScale, m_picSize*m_picScale, 11 * m_picSize, 0, m_picSize, m_picSize);// �����`��


		break;
	}
	case Number::NUM_KIND::BIG:
	{
		// ���@�ł���
		float PiccScale = 1.5f;
		m_pic->SetScale(PiccScale);

		int count = 0;
		for (;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pic->Render(x - ((count * (32 * PiccScale))*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}

		//m_pic->Render(x - ((++count * (34 * PiccScale))*m_picScale) + 8, y, m_picSize*m_picScale, m_picSize*m_picScale, 10 * m_picSize, 0, m_picSize, m_picSize);// �����`��
		//m_pic->Render(x - ((++count * (36 * PiccScale))*m_picScale) + 8, y, m_picSize*m_picScale, m_picSize*m_picScale, 11 * m_picSize, 0, m_picSize, m_picSize);// �����`��


		break;
	}
	default:
		//for (int count = 0;; ++count)
		//{
		//	int digitNum = number % 10;	// ��ԏ������������
		//	number = (int)(number / 10);// ���l�̈�ԏ�������������

		//	m_pic->Render(x + (((DIGIT_MAX - count) * 32)*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

		//	if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		//}
		break;
	}

}

// �A�N�V����
void Number::Action(int derayTime)
{
	m_pic->Action(derayTime);
}





/*****************************************************************/
//
//				�i���o�[�G�t�F�N�g
//
/*****************************************************************/

/***********************************************/
// �C���X�^���X��
/***********************************************/
Number_Effect* Number_Effect::m_pInstance = nullptr;

Number_Effect& Number_Effect::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Number_Effect();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *m_pInstance;
}

void Number_Effect::Release()
{
	SAFE_DELETE(m_pInstance);
}


/***********************************************/
// �������E���
/***********************************************/
Number_Effect::Number_Effect()
{

}

Number_Effect::~Number_Effect()
{
	// �S��������
	for (auto it : m_NumberData)
	{
		SAFE_DELETE(it->number);
		SAFE_DELETE(it);
	}

	//�@�f�[�^�����
	m_NumberData.clear();
}


/***********************************************/
// �X�V
/***********************************************/
void Number_Effect::Update()
{

	// List
	for (auto it = m_NumberData.begin(); it != m_NumberData.end();)
	{
		// �^�C�}�[�Ȃ�x���W��
		if ((*it)->kind == Number::NUM_KIND::TIMER)
		{
			//it->Update();

			//it->alpha += 21;
			//if (it->alpha > 255)
			//{
			//	it->alpha = 255;
			//}

			// �X�e�[�g�œ����𕪊�
			switch ((*it)->state)
			{
			case NumberData::STATE::START:

				// y���W�����ɂ��炷
				(*it)->y -= 2;

				// �A���t�@���グ��
				(*it)->alpha += 50;
				if ((*it)->alpha >= 255)(*it)->alpha = 255;

				// �t���[���Œ���
				(*it)->flame++;

				//if ((*it)->alpha >= 255)
				if ((*it)->flame >= 32)
				{
					//�@�A���t�@���}�b�N�X�ɂȂ�����I��
					(*it)->state = NumberData::STATE::ARRIVAL;

					// �t���[����������
					(*it)->flame = 0;

					//(*it)->number->GetAnim()->OrderJump(14, 1.0f, 1.3f);
					//(*it)->number->GetAnim()->Action();
				}

				break;
			case NumberData::STATE::ARRIVAL:

				// y���W�����ɂ��炷
				(*it)->y -= 1;

				// �t���[���Ŏ��ԉ҂�
				(*it)->flame++;

				if ((*it)->flame > 32)
				{
					(*it)->state = NumberData::STATE::END;
				}

				break;
			case NumberData::STATE::END:
				(*it)->y += 1;
				// �A���t�@��������
				(*it)->alpha -= 40;

				// �Ȃ񂩏I���t���O�𗧂Ă�
				if ((*it)->alpha <= 0)
				{
					(*it)->alpha = 0;
					// �I���t���OOn 
					(*it)->isEnd = true;
				}

				break;
			default:
				break;
			}

			// �A���t�@�X�V
			//(*it)->number->GetAnim()->SetARGB((*it)->alpha, 255, 255, 255);
			(*it)->number->GetAnim()->SetAlpha((*it)->alpha);

			// �i���o�[�X�V
			(*it)->number->Update();

			if ((*it)->isEnd == true)
			{
				// ��ɏ���
				SAFE_DELETE((*it)->number);
				SAFE_DELETE((*it));
				// ����ɍX�V�����
				it = m_NumberData.erase(it);
			}
			else
			{
				// �����ōX�V
				it++;
			}
		}// timer
		 // �^�C�}�[�Ȃ�x���W��
		else if ((*it)->kind == Number::NUM_KIND::BIG)
		{
			// �X�e�[�g�œ����𕪊�
			switch ((*it)->state)
			{
			case NumberData::STATE::START:

				// y���W�����ɂ��炷
				(*it)->y -= 2;

				// �A���t�@���グ��
				(*it)->alpha += 50;
				if ((*it)->alpha >= 255)(*it)->alpha = 255;

				// �t���[���Œ���
				(*it)->flame++;

				//if ((*it)->alpha >= 255)
				if ((*it)->flame >= 32)
				{
					//�@�A���t�@���}�b�N�X�ɂȂ�����I��
					(*it)->state = NumberData::STATE::ARRIVAL;

					// �t���[����������
					(*it)->flame = 0;

					//(*it)->number->GetAnim()->OrderJump(14, 1.0f, 1.3f);
					//(*it)->number->GetAnim()->Action();
				}

				break;
			case NumberData::STATE::ARRIVAL:

				// y���W�����ɂ��炷
				(*it)->y -= 1;

				// �t���[���Ŏ��ԉ҂�
				(*it)->flame++;

				if ((*it)->flame > 32)
				{
					(*it)->state = NumberData::STATE::END;
				}

				break;
			case NumberData::STATE::END:
				(*it)->y += 1;
				// �A���t�@��������
				(*it)->alpha -= 40;

				// �Ȃ񂩏I���t���O�𗧂Ă�
				if ((*it)->alpha <= 0)
				{
					(*it)->alpha = 0;
					// �I���t���OOn 
					(*it)->isEnd = true;
				}

				break;
			default:
				break;
			}

			// �A���t�@�X�V
			//(*it)->number->GetAnim()->SetARGB((*it)->alpha, 255, 255, 255);
			(*it)->number->GetAnim()->SetAlpha((*it)->alpha);

			// �i���o�[�X�V
			(*it)->number->Update();

			if ((*it)->isEnd == true)
			{
				// ��ɏ���
				SAFE_DELETE((*it)->number);
				SAFE_DELETE((*it));
				// ����ɍX�V�����
				it = m_NumberData.erase(it);
			}
			else
			{
				// �����ōX�V
				it++;
			}

		}
		else // �ӂ�
		{
			//it->Update();

			//it->alpha += 21;
			//if (it->alpha > 255)
			//{
			//	it->alpha = 255;
			//}

			// �X�e�[�g�œ����𕪊�
			switch ((*it)->state)
			{
			case NumberData::STATE::START:

				// y���W�����ɂ��炷
				(*it)->y -= 4;

				// �A���t�@���グ��
				(*it)->alpha += 50;
				if ((*it)->alpha >= 255)(*it)->alpha = 255;

				// �t���[���Œ���
				(*it)->flame++;

				//if ((*it)->alpha >= 255)
				if ((*it)->flame >= 12)
				{
					//�@�A���t�@���}�b�N�X�ɂȂ�����I��
					(*it)->state = NumberData::STATE::ARRIVAL;

					// �t���[����������
					(*it)->flame = 0;


				}

				break;
			case NumberData::STATE::ARRIVAL:

				// y���W�����ɂ��炷
				(*it)->y -= 1;

				// �t���[���Ŏ��ԉ҂�
				(*it)->flame++;

				if ((*it)->flame > 12)
				{
					(*it)->state = NumberData::STATE::END;
				}

				break;
			case NumberData::STATE::END:
				(*it)->y += 1;
				// �A���t�@��������
				(*it)->alpha -= 40;

				// �Ȃ񂩏I���t���O�𗧂Ă�
				if ((*it)->alpha <= 0)
				{
					(*it)->alpha = 0;
					// �I���t���OOn 
					(*it)->isEnd = true;
				}

				break;
			default:
				break;
			}

			// �A���t�@�X�V
			//(*it)->number->GetAnim()->SetARGB((*it)->alpha, 255, 255, 255);
			(*it)->number->GetAnim()->SetAlpha((*it)->alpha);

			// �i���o�[�X�V
			(*it)->number->Update();

			if ((*it)->isEnd == true)
			{
				// ��ɏ���
				SAFE_DELETE((*it)->number);
				SAFE_DELETE((*it));
				// ����ɍX�V�����
				it = m_NumberData.erase(it);
			}
			else
			{
				// �����ōX�V
				it++;
			}
		}

	
	}
}

void Time_Update()
{




}


/***********************************************/
// �`��
/***********************************************/
void Number_Effect::Render()
{
	// List
	for (auto it : m_NumberData)
	{
		it->number->Render(it->x, it->y, it->score, it->kind);
	}
}

/************************************************/
//	�����ǉ�
/************************************************/
void Number_Effect::AddNumber(int x, int y, int score, COLOR_TYPE type, Number::NUM_KIND kind)
{
	NumberData* data;
	
	data = new NumberData();
	data->number = new Number();	
	//data->number->Action();
	data->x = x;
	data->y = y;
	data->score = score;
	data->kind = kind;

	data->flame = 0;
	data->endFlame = 100;
	data->alpha = 0;
	data->state = NumberData::STATE::START;
	
	data->isEnd = false;

	// �F
	switch (type)
	{
	case Number_Effect::COLOR_TYPE::WHITE:
		data->number->GetAnim()->SetARGB(0xffffffff);// �F

		break;
	case Number_Effect::COLOR_TYPE::BLUE:
		data->number->GetAnim()->SetARGB(0xff00ff00);// �F

		break;
	case Number_Effect::COLOR_TYPE::RED:
		data->number->GetAnim()->SetARGB(0xffff0000);// �F
		
		break;
	case Number_Effect::COLOR_TYPE::GREEN:
		data->number->GetAnim()->SetARGB(0xff00ff00);// �F
		
		break;
	case Number_Effect::COLOR_TYPE::LIGHT_BLUE:
		data->number->GetAnim()->SetARGB(0xff00ffff);// �F
		
		break;
	case Number_Effect::COLOR_TYPE::YELLOW_GREEN:
		data->number->GetAnim()->SetARGB(0xffaaff77);// �F
		
		break;	
	case Number_Effect::COLOR_TYPE::ORANGE:

		data->number->GetAnim()->SetARGB(0xffff8000);// �F
		
		break;
	default:
		break;
	}
	

	// �v�f�ǉ�
	m_NumberData.push_back(data);


}

