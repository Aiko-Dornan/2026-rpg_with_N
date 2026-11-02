#pragma once
#include"map.h"
#include"damageObj.h"
#include"enemyBullet.h"

class CSpawnPoint;

class CSlime : public CDamageObj
{
public:
	CSlime(Point, CMap*, CSpawnPoint*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//対象スポナー
	CSpawnPoint* spawn{ nullptr };

	//CEnemyBullet* eb;

};