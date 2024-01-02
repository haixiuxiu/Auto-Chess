#include"AiminiHero.h"
#include<ctime>
#include<vector>
#include<algorithm>
#include<iostream>
#include"const.h" 
void AiminiHero::initData(Ecamp Camp)
{
	_camp = Camp;
	_money = 1;
	_exp = 0;
	_hpLim = InitHp;
	_dragable = false;  //dragable���ܲ��ܶ��ı�ǩ
	_isDead = false;
	_isHurt = false;
	setVisible(false);
	_lv = 1;
	_maxOnBoard = _lv + 2;
	
}


AiminiHero* AiminiHero::create(const std::string& filename, Ecamp camp, twoStateType type,int AiID)
{
	AiminiHero* sprite = new (std::nothrow)AiminiHero(AiID);
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->initData(camp);
		sprite->setAnchorPoint(Vec2(0.5, 0.5));
		sprite->setScale(0.25);//Ӳ����
		//Ѫ��
		sprite->_hpBar = HP::create(ENERGY, MINIhp, 10);
		sprite->_hpBar->setScaleY(5);//Ӳ����
		sprite->_hpBar->setScaleX(5);//Ӳ����
		sprite->addChild(sprite->_hpBar);
		sprite->_hpBar->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height + 10));
		sprite->autorelease();

		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

AiminiHero::AiminiHero(int AiID)
{
	HeroCost[HOUYIHOUYI] = 2;
	HeroCost[YASEYASE] = 2;
	HeroCost[DAJIDAJI] = 2;//Ӣ�۶�Ӧ����

	setExp(0);
	setLevel(1);
	setMaxOnBoard(3);

	myHeroInBattle.resize(28);
	// ��ȡ��ǰʱ����Ϊ����
	unsigned int seed = static_cast<unsigned int>(time(nullptr));
	std::cout << (time(nullptr)) << std::endl;
	// ʹ�����ӳ�ʼ�������������
	srand(seed);

	int random = rand() % 100; //�����

	switch (AiID)
	{
	case 1:
		if (random <= Probability1)
		{
			HeroMessage Hero1 = { 1,HOUYIHOUYI };
			HeroMessage Hero2 = { 1,YASEYASE };
			AllmyHero.push_back(Hero1);
			AllmyHero.push_back(Hero2);
			SameTypeHero[Hero1] += 1;
			SameTypeHero[Hero2] += 1;
		}
		else
		{
			HeroMessage Hero1 = { 1,HOUYIHOUYI };
			HeroMessage Hero2 = { 1,HOUYIHOUYI };
			AllmyHero.push_back(Hero1);
			AllmyHero.push_back(Hero2);
			SameTypeHero[Hero1] += 1;
			SameTypeHero[Hero2] += 1;
		}
		break;
	case 2:
		if (random <= Probability1)
		{
			HeroMessage Hero1 = { 1,YASEYASE };
			HeroMessage Hero2 = { 1,YASEYASE };
			AllmyHero.push_back(Hero1);
			AllmyHero.push_back(Hero2);
			SameTypeHero[Hero1] += 1;
			SameTypeHero[Hero2] += 1;
		}
		else
		{
			HeroMessage Hero1 = { 1,DAJIDAJI };
			HeroMessage Hero2 = { 1,YASEYASE };
			AllmyHero.push_back(Hero1);
			AllmyHero.push_back(Hero2);
			SameTypeHero[Hero1] += 1;
			SameTypeHero[Hero2] += 1;
		}
		break;
	default:
		HeroMessage Hero1 = { 1,YASEYASE };
		HeroMessage Hero2 = { 1,HOUYIHOUYI };
		AllmyHero.push_back(Hero1);
		AllmyHero.push_back(Hero2);
		SameTypeHero[Hero1] += 1;
		SameTypeHero[Hero2] += 1;
		break;
	}//��ʼ��
	for (auto i = AllmyHero.begin(); i != AllmyHero.end(); i++)
	{
		_money -= HeroCost[(*i).herotype];
	}
}


void AiminiHero::sendAIMessage() 
{

	bool isadd = true; //�Ƿ����

	int random = rand() % 100; //�����

	//�����߼�
	for (int j = 0; j < 10; j++)
	{
		random = rand() % 100;  // ����һ�� 0 �� 99 �������
		if (random <= Probability3) 
		{
			HeroMessage newHero;
			for (auto it = SameTypeHero.begin(); it != SameTypeHero.end(); it++)
			{
				random = rand() % 100;
				if ((*it).second == 2 && random > Probability2)
				{
					if (getMoney() >= HeroCost[(*it).first.herotype])//��������Ӣ�۵Ľ׼�
					{
						newHero = (*it).first;
						newHero.HeroLv = 1;
						_money -= HeroCost[(*it).first.herotype];
					}
					else isadd = false;
				}
				else if (random > Probability2)//�������������Ӣ��
				{
					random = rand() % 100;
					if (random < 33 && getMoney() >= HeroCost[HOUYIHOUYI])//�������һ��
					{
						newHero = {1,HOUYIHOUYI };
						_money -= HeroCost[HOUYIHOUYI];
					}
					else if (random > 80 && getMoney() >= HeroCost[DAJIDAJI])
					{
						newHero = {1,DAJIDAJI };
						_money -= HeroCost[DAJIDAJI];
					}
					else if (random >= 33 && random <= 66 && getMoney() >= HeroCost[YASEYASE])
					{
						newHero = {1,YASEYASE };
						_money -= HeroCost[DAJIDAJI];
					}
					else isadd = false;//ɶҲ����

				}
				else isadd = false;
				if (isadd)
				{
					AllmyHero.push_back(newHero);
					if (newHero.herotype == DAJIDAJI)
					{
						int a = 1;
					}
					SameTypeHero[newHero]++;
					if (SameTypeHero[newHero] == 3)
					{
						SameTypeHero[newHero] = 0;
						for (int i = 0; i < 3; i++)
						{
							auto it2 = std::find(AllmyHero.begin(), AllmyHero.end(), newHero);
							if (i == 2)
							{
								(*it2).HeroLv++;
								SameTypeHero[(*it2)]++;
								break;
							}
							it2 = AllmyHero.erase(it2);

						}//�ϳ��߼�
					}

				}
				else isadd = true;
			}//����Ӣ��
		}
		else if (random > Probability3 && random <= 90)//����
		{
			random = rand() % 100;
			if (random > Probability1 && getMoney() >= upgradeCost)
			{
				_exp += 5;
				if (_exp == _level * 10)
				{
					_level++;
					_maxOnBoard++;
					setExp(0);
				}
				_money -= upgradeCost;
			}
		}
		else //ɾ��
		{
			for (auto it = AllmyHero.begin(); it != AllmyHero.end(); it++)
			{
				random = rand() % 100;
				if ((*it).HeroLv == 1 && random > Probability1 && SameTypeHero[*it] == 1)
				{
					_money += HeroCost[(*it).herotype] - 1;//�ӽ��
					it = AllmyHero.erase(it); //ɾ��
					if (it == AllmyHero.end()) break; //�ֶ��˳�
				}
			}
		}
	}
	int counter = 0;//������
	//�ϳ��߼�
	for (auto it3 = AllmyHero.begin(); it3 != AllmyHero.end(); it3++)
	{
		if (counter < getMaxOnBoard())
		{
			random = rand() % 100;
			if ((*it3).HeroLv > 1) random += 10 * ((*it3).HeroLv - 1); //����ȼ��߸��ʸ���
			if (random <= Probability4)
			{
				if ((*it3).herotype == YASEYASE)//����Ϊսʿ
				{
					if (myHeroInBattle[colArow(3, 2)].HeroLv == 0)
					{
						myHeroInBattle[colArow(3, 2)] = (*it3);
					}
					else if (myHeroInBattle[colArow(3, 3)].HeroLv == 0)
					{
						myHeroInBattle[colArow(3, 3)] = (*it3);
					}
					else if (myHeroInBattle[colArow(3, 4)].HeroLv == 0)
					{
						myHeroInBattle[colArow(3, 4)] = (*it3);
					}
					else if (myHeroInBattle[colArow(2, 2)].HeroLv == 0)
					{
						myHeroInBattle[colArow(2, 2)] = (*it3);
					}
					else if (myHeroInBattle[colArow(2, 3)].HeroLv == 0)
					{
						myHeroInBattle[colArow(2, 3)] = (*it3);
					}
					else if (myHeroInBattle[colArow(2, 4)].HeroLv == 0)
					{
						myHeroInBattle[colArow(2, 4)] = (*it3);
					}
				}
				else
				{
					if (myHeroInBattle[colArow(0, 2)].HeroLv == 0)
					{
						myHeroInBattle[colArow(0, 2)] = (*it3);
					}
					else if (myHeroInBattle[colArow(0, 3)].HeroLv == 0)
					{
						myHeroInBattle[colArow(0, 3)] = (*it3);
					}
					else if (myHeroInBattle[colArow(0, 4)].HeroLv == 0)
					{
						myHeroInBattle[colArow(0, 4)] = (*it3);
					}
					else if (myHeroInBattle[colArow(1, 2)].HeroLv == 0)
					{
						myHeroInBattle[colArow(1, 2)] = (*it3);
					}
					else if (myHeroInBattle[colArow(1, 3)].HeroLv == 0)
					{
						myHeroInBattle[colArow(1, 3)] = (*it3);
					}
					else if (myHeroInBattle[colArow(1, 4)].HeroLv == 0)
					{
						myHeroInBattle[colArow(1, 4)] = (*it3);
					}
				}
			} //���ò���
			else //�����ò���
			{
				while (1)
				{
					int randomPos = rand() % 28;
					if (myHeroInBattle[randomPos].HeroLv == 0)
					{
						myHeroInBattle[randomPos] = (*it3);
						break;
					}
				}
			}
			counter++;
		}
		else
			myHeroOutBattle.push_back((*it3));
		if (it3 == AllmyHero.end()) break;
	}
	for (int i = 0; i < 28; i++)
	{
		_heroMessage[i * 2] = myHeroInBattle[i].herotype + '0';
		_heroMessage[i * 2 + 1] = myHeroInBattle[i].HeroLv + '0';
	}

}

