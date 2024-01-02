#ifndef __AIMINIHERO_H__
#define __AIMINIHERO_H__
#include "Hero.h"
#include "cocos2d.h"
#include<vector>

USING_NS_CC;
class AiminiHero : public Hero
{
	CC_SYNTHESIZE(int, _money, Money);
	CC_SYNTHESIZE(int, _exp, Exp);
	CC_SYNTHESIZE(int, _maxOnBoard, MaxOnBoard);
	//CC_SYNTHESIZE(int, _experience, Experience);
	CC_SYNTHESIZE(int, _level, Level);
protected:
	AiminiHero(int AiID);
	void initData(Ecamp Camp);
	virtual bool onTouchBegan(Touch* t, Event* e) { return true; }
	virtual void onTouchMoved(Touch* t, Event* e) {}
	virtual void onTouchEnded(Touch* t, Event* e) {}
	const int upgradeCost = 2;
	/*int upgradeCost*/
	//英雄数组(分上场和不上场)
	std::vector<HeroMessage> myHeroInBattle;
	std::vector<HeroMessage> myHeroOutBattle;
	std::vector<HeroMessage> AllmyHero;
	//映射 作统计用
	std::map<HeroMessage, int> SameTypeHero;
	std::map< HeroType, int>HeroCost;
public:
	static AiminiHero* create(const std::string& filename, Ecamp camp, twoStateType type, int AiID);
	void sendAIMessage();
	char _heroMessage[56] ;
};
#endif