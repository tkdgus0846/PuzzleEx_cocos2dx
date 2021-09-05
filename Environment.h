#pragma once

#include "stdafx.h"

enum class GameState
{
	PLAYING, PAUSED, WIN, LOSE, BLOCK_MOVING
};

#define BLOCK_HORIZONTAL 7
#define BLOCK_VERTICAL 9
#define BLOCK_OFFSET Vec2(720/2, 1280/2)
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 80

#define BLOCK_VAR 4

#define FONT_NAME "fonts/SDSamliphopangcheTTFBasic.ttf"

#define SWAP(TYPE,A,B) {TYPE t= A; A=B; B=t;}

#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* getInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\



