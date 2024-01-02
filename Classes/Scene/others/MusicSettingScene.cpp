/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "LoginScene.h"
#include "ChoiceScene.h"
#include "SimpleAudioEngine.h"
#include "MusicSettingScene.h"
USING_NS_CC;

Scene* MusicSetting::createScene()
{
    return MusicSetting::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ChoiceScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MusicSetting::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    

    // create menu, it's an autorelease object
    //--------------------------张睿康写的
    auto menuItemImageBack = MenuItemImage::create("back.png", "back1.png", CC_CALLBACK_1(MusicSetting::back, this));
    menuItemImageBack->setPosition(Vec2(40, visibleSize.height - 20));

    auto menuItemImageMusic1 = MenuItemImage::create("music1_0.png", "music1_1.png", CC_CALLBACK_1(MusicSetting::setmusic1, this));
    menuItemImageMusic1->setPosition(Vec2(visibleSize.width/2-200, visibleSize.height/2+50));
    menuItemImageMusic1->setScale(0.25);

    auto menuItemImageMusic2 = MenuItemImage::create("music2_0.png", "music2_1.png", CC_CALLBACK_1(MusicSetting::setmusic2, this));
    menuItemImageMusic2->setPosition(Vec2(visibleSize.width / 2-200, visibleSize.height / 2 - 50));
    menuItemImageMusic2->setScale(0.25);
    


    
    auto menuItemImageVolumehigh = MenuItemImage::create("high.png", "high.png", CC_CALLBACK_1(MusicSetting::Volumehigh, this));
    menuItemImageVolumehigh->setPosition(Vec2(visibleSize.width /2 +200, visibleSize.height / 2+150));
    auto menuItemImageVolumemiddle = MenuItemImage::create("middle.png", "middle.png", CC_CALLBACK_1(MusicSetting::Volumemiddle, this));
    menuItemImageVolumemiddle->setPosition(Vec2(visibleSize.width / 2+200, visibleSize.height / 2 ));
    auto menuItemImageVolumelow = MenuItemImage::create("low.png", "low.png", CC_CALLBACK_1(MusicSetting::Volumelow, this));
    menuItemImageVolumelow->setPosition(Vec2(visibleSize.width / 2 + 200, visibleSize.height / 2-150));
    

    // --------------------张睿康写的结束
  auto menu = Menu::create( menuItemImageBack,menuItemImageMusic1, menuItemImageMusic2, menuItemImageVolumehigh , menuItemImageVolumemiddle, menuItemImageVolumelow, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
// add "Choice" splash screen"
    auto sprite = Sprite::create("youxijiemian.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'youxijiemian.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

    return true;
}


void MusicSetting::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}
void MusicSetting::back(Ref* obj) {
    auto scene = Choice::createScene();
    Director::getInstance()->replaceScene(scene);
}
void MusicSetting::setmusic1(Ref* obj) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();
    audio->playBackgroundMusic("longtimenosee.mp3", true);
}
void MusicSetting::setmusic2(Ref* obj) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();
    audio->playBackgroundMusic("lovestory.mp3", true);
}

void MusicSetting::Volumehigh(Ref* obj) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->setBackgroundMusicVolume(1.0f);
}
void MusicSetting::Volumemiddle(Ref* obj) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->setBackgroundMusicVolume(0.5f);
}
void MusicSetting::Volumelow (Ref* obj) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->setBackgroundMusicVolume(0.2f);
}
