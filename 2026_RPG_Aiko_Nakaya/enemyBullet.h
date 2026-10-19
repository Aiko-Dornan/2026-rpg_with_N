#pragma once

#include "objBase.h"
#include"map.h"

class CEnemyBullet : public BaseVector
{
public:
	CEnemyBullet(Point,int, CMap*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//CMap* map = nullptr;

	//Šp“x
	float angle{ 0 };
	float start_angle{ 0 };

	//Œ•‚Ì’·‚³
	int length{ 48 };
};

