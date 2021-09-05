#include "stdafx.h"
#include "SceneHome.h"
#include "SceneIngame.h"
#include "Environment.h"
#include "GameManager.h"

SceneHome* SceneHome::create()
{
	SceneHome* ret = new SceneHome();
	if (ret && ret->init())
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneHome::init()
{
	if (!Scene::init()) return false;

	Sprite* img = Sprite::create("res/title_girl.png");
	addChild(img);
	img->setOpacity(0);
	img->runAction(FadeIn::create(1.0f));
	img->setPosition(Vec2(720 / 2, 800));

	Sprite* title = Sprite::create("res/title.png");
	addChild(title);
	title->setPosition(Vec2(720 / 2, -100));
	title->runAction(EaseSineOut::create(MoveTo::create(0.25f, Vec2(720 / 2, 600))));


	addChild(btnStart = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));
	addChild(btnQuit = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));

	btnStart->setPosition(Vec2(720 / 2, 400));
	btnQuit->setPosition(Vec2(720 / 2, 250));

	btnStart->setTitleFontName(FONT_NAME);
	btnQuit->setTitleFontName(FONT_NAME);

	btnStart->setTitleFontSize(40.0f);
	btnQuit->setTitleFontSize(40.0f);

	btnStart->setTitleColor(Color3B(0, 50, 50));
	btnQuit->setTitleColor(Color3B(0, 50, 50));

	btnStart->setTitleText("Start Game");
	btnQuit->setTitleText("Quit Game");

	GameManager::getInstance()->stopBackgroundMusic();

	btnStart->addClickEventListener
	(
		[=](Ref* r)
		{
			auto scene = SceneIngame::create();
			auto transit = TransitionSlideInR::create(0.125f, scene);
			Director::getInstance()->replaceScene(transit);
			GameManager::getInstance()->playPop();
		}
	);
	
	btnQuit->addClickEventListener
	(
		[=](Ref* r)
		{
			Director::getInstance()->end();
			GameManager::getInstance()->playPop();
		}
	);
	return true;
}
