#pragma once

#include "stdafx.h"
#include "Environment.h"

class LayerIngameUI : public Node
{
private:
	long long score;
	char scoreString[128];
public:
	Label* lbScore;
	Button* btnPause;
	Scale9Sprite* pausePanel;
	Button* btnResume;
	Button* btnRestart;
	Button* btnHome;
	DrawNode* dnCurtain;

	static LayerIngameUI* create();
	virtual bool init() override;

	void setScore(long long score);
	long long getScore();
	void showPausePanel();
	void hidePausePanel();

};

