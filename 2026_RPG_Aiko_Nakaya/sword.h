#pragma once
#include "objBase.h"
#include"map.h"

class CSword : public BaseVector
{
public:
	CSword(Point,int, CMap*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//CMap* map = nullptr;

	//角度
	float angle{ 0 };
	float start_angle{ 0 };

	//剣の長さ
	int length{ 48 };
};

