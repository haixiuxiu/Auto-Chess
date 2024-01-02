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
	_dragable = false;  //dragable即能不能动的标签
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
		sprite->setScale(0.25);//硬编码
		//血条
		sprite->_hpBar = HP::create(ENERGY, MINIhp, 10);
		sprite->_hpBar->setScaleY(5);//硬编码
		sprite->_hpBar->setScaleX(5);//硬编码
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
	HeroCost[DAJIDAJI] = 2;//英雄对应花费

	setExp(0);
	setLevel(1);
	setMaxOnBoard(3);

	myHeroInBattle.resize(28);
	// 获取当前时间作为种子
	unsigned int seed = static_cast<unsigned int>(time(nullptr));
	std::cout << (time(nullptr)) << std::endl;
	// 使用种子初始化随机数生成器
	srand(seed);

	int random = rand() % 100; //随机数

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
	}//初始化
	for (auto i = AllmyHero.begin(); i != AllmyHero.end(); i++)
	{
		_money -= HeroCost[(*i).herotype];
	}
}


void AiminiHero::sendAIMessage() 
{

	bool isadd = true; //是否添加

	int random = rand() % 100; //随机数

	//购买逻辑
	for (int j = 0; j < 10; j++)
	{
		random = rand() % 100;  // 生成一个 0 到 99 的随机数
		if (random <= Probability3) 
		{
			HeroMessage newHero;
			for (auto it = SameTypeHero.begin(); it != SameTypeHero.end(); it++)
			{
				random = rand() % 100;
				if ((*it).second == 2 && random > Probability2)
				{
					if (getMoney() >= HeroCost[(*it).first.herotype])//可能升级英雄的阶级
					{
						newHero = (*it).first;
						newHero.HeroLv = 1;
						_money -= HeroCost[(*it).first.herotype];
					}
					else isadd = false;
				}
				else if (random > Probability2)//可能随便买其他英雄
				{
					random = rand() % 100;
					if (random < 33 && getMoney() >= HeroCost[HOUYIHOUYI])//随机购买一个
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
					else isadd = false;//啥也不做

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

						}//合成逻辑
					}

				}
				else isadd = true;
			}//购买英雄
		}
		else if (random > Probability3 && random <= 90)//升级
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
		else //删除
		{
			for (auto it = AllmyHero.begin(); it != AllmyHero.end(); it++)
			{
				random = rand() % 100;
				if ((*it).HeroLv == 1 && random > Probability1 && SameTypeHero[*it] == 1)
				{
					_money += HeroCost[(*it).herotype] - 1;//加金币
					it = AllmyHero.erase(it); //删除
					if (it == AllmyHero.end()) break; //手动退出
				}
			}
		}
	}
	int counter = 0;//计数器
	//上场逻辑
	for (auto it3 = AllmyHero.begin(); it3 != AllmyHero.end(); it3++)
	{
		if (counter < getMaxOnBoard())
		{
			random = rand() % 100;
			if ((*it3).HeroLv > 1) random += 10 * ((*it3).HeroLv - 1); //如果等级高概率更高
			if (random <= Probability4)
			{
				if ((*it3).herotype == YASEYASE)//棋子为战士
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
			} //采用策略
			else //不采用策略
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

