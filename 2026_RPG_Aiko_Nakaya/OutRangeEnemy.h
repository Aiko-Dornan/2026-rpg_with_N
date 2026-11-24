#pragma once
#pragma once
#include"map.h"
#include"damageObj.h"
#include"enemyBullet.h"

class CSpawnPoint;

class COutRangeEnemy : public CDamageObj
{
public:
	COutRangeEnemy(Point, CMap*, CSpawnPoint*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//対象スポナー
	CSpawnPoint* spawn{ nullptr };

	float moveSpeed = 1.0f;

	CEnemyBullet* eb;

};