#include "stdafx.h"
#include "SceneIngame.h"
#include "LayerIngameUI.h"
#include "SceneHome.h"
#include "GameManager.h"

void SceneIngame::createBlock(int x, int y, int type)
{
	auto cache = Director::getInstance()->getTextureCache();
	auto spr = Sprite::createWithTexture(cache->getTextureForKey("res/match3_tiles_px.png"),Rect(type*40,0,40,40));
	spr->setScale(2.0f);
	// 이상한 버그가 있다. 원래 position이 0,0일때 스프라이트가 출력이 되어야 하는데, 0,110이 되어야 출력이된다.
	//spr->setPosition(Vec2(0,110));
	
	addChild(spr);
	setBlockData(x, y, type);
	setBlockSprite(x, y, spr);
}

int SceneIngame::getBlockData(int x, int y)
{
	return blockData[y][x];
}

void SceneIngame::setBlockData(int x, int y, int type)
{
	blockData[y][x] = type;
}

Sprite* SceneIngame::getBlockSprite(int x, int y)
{
	return blockSprite[y][x];
}

void SceneIngame::setBlockSprite(int x, int y, Sprite* s)
{
	blockSprite[y][x] = s;
}

void SceneIngame::destroyBlock(int x, int y)
{
	if (blockData[y][x] != 0)
	{
		state = GameState::BLOCK_MOVING;
		blockSprite[y][x]->runAction
		(
			Sequence::create
			(
				FadeOut::create(0.125f),
				FadeIn::create(0.125f),
				FadeOut::create(0.125f),
				FadeIn::create(0.125f),
				Spawn::create
				(
					ScaleTo::create(0.125f, 0.0f),
					FadeOut::create(0.125f),
					nullptr
				),
				RemoveSelf::create(),
				nullptr
			)
		);
		blockSprite[y][x] = nullptr;
		blockData[y][x] = 0;

		this->runAction
		(
			Sequence::create
			(
				DelayTime::create(0.625f),
				CallFunc::create([=]() {dropBlocks(x); }),
				nullptr
			)
		);

	}
	
}

Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2& gameCoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2) + Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
	Vec2 delta = gameCoord - blockOrigin;
	Vec2 pos = Vec2(static_cast<int>(delta.x / BLOCK_WIDTH + 0.5), static_cast<int>(delta.y / BLOCK_HEIGHT + 0.5));
	return pos;
}

Vec2 SceneIngame::convertBlockCoordToGamecoord(Vec2& blockCoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2) + Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
	return blockOrigin+Vec2(BLOCK_WIDTH*blockCoord.x,BLOCK_HEIGHT*blockCoord.y);
}

int SceneIngame::findEmptyBlockYIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++)
	{
		if (getBlockData(x, i) == 0) return i;
	}
	return -1;
}

int SceneIngame::findFilledBlockYIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++)
	{
		if (getBlockData(x, i) != 0) return i;
	}
	return -1;
}

void SceneIngame::dropBlocks(int x)
{
	bool isDrop = false; // 최상단 블록이 추가되지 않을 경우에 유효함
	for (int i = 0; i < BLOCK_VERTICAL; i++)
	{
		int empty_y = findEmptyBlockYIndex(x, i);
		if (empty_y == -1) continue;
		int filled_y = findFilledBlockYIndex(x, empty_y + 1);
		if (filled_y == -1)
		{
			createBlock(x, empty_y, rand() % BLOCK_VAR + 1);
			blockSprite[empty_y][x]->setPosition(convertBlockCoordToGamecoord(Vec2(x, BLOCK_VERTICAL + 1)));
			blockSprite[empty_y][x]->runAction(MoveTo::create(0.125f, convertBlockCoordToGamecoord(Vec2(x, empty_y))));
			isDrop = true;
			continue;
		}

		{
			int a = getBlockData(x, empty_y);
			int b = getBlockData(x, filled_y);
			SWAP(int, a, b);
			setBlockData(x, empty_y, a);
			setBlockData(x, filled_y, b);
		}

		{
			Sprite* a = getBlockSprite(x, empty_y);
			Sprite* b = getBlockSprite(x, filled_y);
			SWAP(Sprite*, a, b);
			setBlockSprite(x, empty_y, a);
			setBlockSprite(x, filled_y, b);

			a->stopAllActions();
			a->runAction(MoveTo::create(0.125f, convertBlockCoordToGamecoord(Vec2(x, empty_y))));
		}
		isDrop = true;
		//allignBlockSprite();
	}

	if (isDrop)
	{
		for (int i = 0; i < BLOCK_VERTICAL; i++)
		{
			this->runAction(Sequence::create
			(
				DelayTime::create(0.1f),
				CallFunc::create([=]() {judgeMatch(x, i); }),
				nullptr
			)
			);
		}
			
	}
	else
	{
		state = GameState::PLAYING;
	}
	
}

void SceneIngame::stackPush(const Vec2& value)
{
	if (judgeData[static_cast<int>(value.y)][static_cast<int>(value.x)] != 0)
		return;
	judgeStack[judgeStackCount++] = value;
	judgeData[static_cast<int>(value.y)][static_cast<int>(value.x)] = 1;
}

const Vec2& SceneIngame::stackPop()
{
	auto ret = judgeStack[--judgeStackCount];
	judgeData[static_cast<int>(ret.y)][static_cast<int>(ret.x)] = 0;
	return ret;
}

void SceneIngame::stackEmpty()
{
	judgeStackCount = 0;
	for (int i = 0; i < BLOCK_VERTICAL; i++)
	{
		for (int j = 0; j < BLOCK_HORIZONTAL; j++)
		{
			judgeData[i][j] = 0;
		}
	}
}

bool SceneIngame::stackFind(const Vec2& value)
{
	return judgeData[static_cast<int>(value.y)][static_cast<int>(value.x)] == 1;
}

void SceneIngame::judgeMatch(int x, int y)
{
	int blockData = getBlockData(x, y);

	if (blockData == 0) return;

	stackPush(Vec2(x, y));
	int push_cnt = 0;

	for (int i = 0; i < 4; i++)
	{
		int next_x = x;
		int next_y = y;
		int inc_x;
		int inc_y;

		switch (i)
		{
		case 0: inc_x = 1; inc_y = 0; push_cnt = 0; break;
		case 1: inc_x = -1; inc_y = 0; break;
		case 2: inc_x = 0; inc_y = 1; push_cnt = 0; break;
		case 3: inc_x = 0; inc_y = -1; break;
		}

		while (true)
		{
			next_x += inc_x;
			next_y += inc_y;
			if (next_x < 0 || next_x >= BLOCK_HORIZONTAL) break;
			if (next_y < 0 || next_y >= BLOCK_VERTICAL) break;
			if (getBlockData(next_x, next_y) == blockData)
			{
				stackPush(Vec2(next_x, next_y));
				push_cnt++;
			}
			else break;
		}

		if (i % 2 == 0) continue;
		if (push_cnt < 2)
		{
			for (int k = 0; k < push_cnt; k++)
				stackPop();
		}
	}
	if (judgeStackCount > 1)
	{
		GameManager::getInstance()->addScore(judgeStackCount * 10);
		ui->setScore(GameManager::getInstance()->getScore());
		GameManager::getInstance()->playBlockExplosion();
		while (judgeStackCount > 0)
		{
			Vec2 p = stackPop();
			destroyBlock(p.x, p.y);
			
		}
		
	}
	else
	{
		state = GameState::PLAYING;
	}
	stackEmpty();
}

SceneIngame* SceneIngame::create()
{
	auto ret = new SceneIngame();
	if (ret && ret->init())
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneIngame::init()
{
	if (!Scene::init()) return false;

	srand(time(0));

	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = std::bind(&SceneIngame::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchCancelled = touch->onTouchEnded;

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);

	return true;
}

void SceneIngame::onEnter()
{
	Scene::onEnter();

	this->initUI();
	this->initGame();
	this->startGame();
}

void SceneIngame::initUI()
{
	addChild(ui = LayerIngameUI::create());
	ui->setLocalZOrder(1);

	ui->btnPause->addClickEventListener
	(
		[=](Ref* r)->void
		{
			if (state == GameState::PLAYING)
			{
				ui->showPausePanel();
				state = GameState::PAUSED;
				GameManager::getInstance()->playPop();
			}
		}
	);

	ui->btnResume->addClickEventListener
	(
		[=](Ref* r)->void
		{
			if (state == GameState::PAUSED)
			{
				ui->hidePausePanel();
				state = GameState::PLAYING;
				GameManager::getInstance()->playPop();
			}
		}
	);

	ui->btnRestart->addClickEventListener
	(
		[=](Ref* r)->void
		{
			if (state == GameState::PAUSED)
			{
				//TODO: 게임 재시작
				ui->hidePausePanel();
				ui->setScore(0);
				this->destroyGame();
				this->initGame();
				this->startGame();
				state = GameState::PLAYING;
				GameManager::getInstance()->playPop();
				GameManager::getInstance()->stopBackgroundMusic();
			}
		}
	);

	ui->btnHome->addClickEventListener
	(
		[=](Ref* r)->void
		{
			if (state == GameState::PAUSED)
			{
				auto scene = SceneHome::create();
				auto transit = TransitionSlideInL::create(0.125f, scene);
				Director::getInstance()->replaceScene(transit);
				GameManager::getInstance()->playPop();
				//GameManager::getInstance()->stopBackgroundMusic();
			}
		}
	);
}

void SceneIngame::initGame()
{
	GameManager::getInstance()->setScore(0);
	for (int i = 0; i < BLOCK_VERTICAL; i++)
	{
		for (int j = 0; j < BLOCK_HORIZONTAL; j++)
		{
			createBlock(j, i, (rand()%BLOCK_VAR)+1);
		}
	}
	this->allignBlockSprite();
}

void SceneIngame::destroyUI()
{
	
}

void SceneIngame::destroyGame()
{
	GameManager::getInstance()->setScore(0);
	for (int i = 0; i < BLOCK_VERTICAL; i++)
	{
		for (int j = 0; j < BLOCK_HORIZONTAL; j++)
		{
			setBlockData(j, i, 0);
			getBlockSprite(j, i)->removeFromParent();
			setBlockSprite(j, i, nullptr);
		}
	}
}

void SceneIngame::allignBlockSprite()
{
	for (int i = 0; i < BLOCK_VERTICAL; i++)
	{
		for (int j = 0; j < BLOCK_HORIZONTAL; j++)
		{
			auto s = getBlockSprite(j, i);
			if (s != nullptr) 
				s->setPosition(convertBlockCoordToGamecoord(Vec2(j, i)));
		}
	}
}

bool SceneIngame::onTouchBegan(Touch* t, Event* e)
{
	Vec2 pos = convertGameCoordToBlockCoord(t->getLocation());

	if (state == GameState::PLAYING)
	{
		if (pos.x >= BLOCK_HORIZONTAL || pos.x < 0) return true;
		if (pos.y >= BLOCK_VERTICAL || pos.y < 0) return true;

		CCLOG("x: %f, y: %f", pos.x, pos.y);
		destroyBlock(pos.x, pos.y);
		GameManager::getInstance()->playBlockExplosion();

	}

	

	/*dropBlocks(pos.x);

	for (int i = 0; i < BLOCK_VERTICAL; i++)
		judgeMatch(pos.x, i);*/
	return true;
}

void SceneIngame::onTouchMoved(Touch* t, Event* e)
{
}

void SceneIngame::onTouchEnded(Touch* t, Event* e)
{
}

void SceneIngame::startGame()
{
	GameManager::getInstance()->playBackgroundMusic();
}

void SceneIngame::pauseGame()
{
}

void SceneIngame::winGame()
{
}

void SceneIngame::loseGame()
{
}
