#pragma once

#include "stdafx.h"
#include "Environment.h"


class SceneIngame: public Scene
{
private:
	GameState state;
	// blockData�� 0���ϰ�� ����ִ� ���, 0�� �ƴ� ������� ��� ���
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	// blockSprite�� nullptr�� ��쿡 �������.
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	Vec2 judgeStack[128];
	// ���ÿ� �ִ� �ڷ��� ��
	int judgeStackCount = 0;
	// 0�̶�� ���ÿ� �ڷᰡ ����, �װ� �ƴ϶�� �ڷᰡ ����
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	void createBlock(int x, int y, int type);
	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2& gameCoord);
	Vec2 convertBlockCoordToGamecoord(Vec2& blockCoord);

	// �Ʒ��������� ã�� �ö󰡸鼭 ��������� �ش� y�ε����� ��ȯ�ϰ�, ������ ������� �ʴٸ� -1�� ��ȯ�Ѵ�.
	int findEmptyBlockYIndex(int x, int y);

	// y ��ġ���� ã�� �ö󰡸鼭 ä�����ִ� ����� ã�� -1�� ������ �Ǹ� ä�����ִ� ����� ���ٴ� ���̴�.
	int findFilledBlockYIndex(int x, int y);

	// ����� ����Ʈ���� �Լ�
	void dropBlocks(int x);

	void stackPush(const Vec2& value);
	const Vec2& stackPop();
	void stackEmpty();
	bool stackFind(const Vec2& value);
	void judgeMatch(int x, int y);

	class LayerIngameUI* ui;
public:
	static SceneIngame* create();
	virtual bool init() override;
	virtual void onEnter() override;

	void initUI();
	void initGame();
	void destroyUI();
	void destroyGame();

	void allignBlockSprite();

	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();

};

