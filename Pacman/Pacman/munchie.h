#pragma once
#ifndef MUNCHIE_H
#define MUNCHIE_H

#include "S2D/S2D.h"
#include "object.h"

using namespace S2D;

class Munchie : public Object
{
public:
	Munchie(float x, float y);

private:
	void Update(int deltaTime);
	void Draw();
	void OnCollision(Object* other);

};

#endif