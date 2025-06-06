#pragma once
#include "gameObject.h"
#include "component.h"

class Effect : public GameObject
{
protected:

	int frameCount = 0;

public:

	virtual void Init() override = 0;
	virtual void Uninit() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;

};

