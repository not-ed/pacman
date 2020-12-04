#pragma once
#ifndef BONUSITEM_H
#define BONUSITEM_H

#include "S2D/S2D.h";
#include "object.h";
#include <sstream>;

class BonusItem : public Object
{
public:
	BonusItem(float x, float y);

private:
	// How long / how much longer the item will remain in the level before disappearing.
	int remainingTime = 15 * 1000;

	void Update(int deltaTime);
	void Draw();
	void OnCollision(Object* other);

};

#endif