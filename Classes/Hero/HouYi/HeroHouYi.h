#ifndef __HEROHouYi_H__
#define __HEROHouYi_H__

#include"Hero.h"
#include"HelloWorldScene.h"

class Equipment;
class ExpComponent;
class StateComponent;
class Record;

USING_NS_CC;

class HeroHouYi :public Hero
{
public:
	static HeroHouYi* create(Ecamp camp, Ref* scene);//¥¥‘Ï”¢–€;
	bool init(Ecamp camp, Ref* scene);
	void initWalkingAnimation(Vec2 destination);
	void decideToAttack();
	void performAttack();
	CallFunc* fireArrow();
	void releaseSkill();
	void upDateMoving();
	void upGrade();
	void displayHeroLevel(int level);
};


#endif
