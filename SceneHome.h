#pragma once

#include "stdafx.h"

class SceneHome :public Scene
{
public:
	Button* btnStart;
	Button* btnQuit;
	static SceneHome* create();
	virtual bool init() override;
};

