#include "stdafx.h"
#include "GameManager.h"


void GameManager::setScore(long long value)
{
	this->score = value;
}

long long GameManager::getScore()
{
	return score;
}

void GameManager::addScore(long long value)
{
	this->score += value;
}

void GameManager::playPop()
{
	AudioEngine::play2d("res/pop.mp3");
	
}

void GameManager::playBackgroundMusic()
{
	this->backgroundMusicId= AudioEngine::play2d("res/Attic Secrets.mp3",true,0.5f);
}

void GameManager::stopBackgroundMusic()
{
	if (backgroundMusicId == -1) return;
	AudioEngine::stop(this->backgroundMusicId);
}

void GameManager::playBlockExplosion()
{
	AudioEngine::play2d("res/block_explosion.mp3", false, 0.3f);
}
