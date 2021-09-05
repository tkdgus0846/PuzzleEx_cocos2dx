#pragma once
#include "Environment.h"

class GameManager
{
	DECLARE_SINGLE(GameManager);
private:
	long long score = 0;
	int backgroundMusicId = -1;
public:
	void setScore(long long value);
	long long getScore();
	void addScore(long long value);

	void playPop();
	void playBackgroundMusic();
	void stopBackgroundMusic();
	void playBlockExplosion();

};
