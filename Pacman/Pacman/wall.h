#pragma once
#ifndef WALL_H
#define WALL_H

#include "S2D/S2D.h"
#include "object.h"

class Wall : public Object
{
public:
	Wall(float x, float y);

private:
	void Update(int deltaTime);
	void Draw();
	void OnCollision(Object* other);
};

#endif