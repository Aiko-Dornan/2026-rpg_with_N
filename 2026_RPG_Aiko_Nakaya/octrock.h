#pragma once
#include"damageObj.h"
#include"map.h"
#include"enemyBullet.h"

class CSpawnPoint;

class COctrock : public CDamageObj
{
private:
    CMap* map = nullptr;
    CSpawnPoint* spawn = nullptr;

    float MoveMentTime = 10.0f;
    float attackTime = 50.0f;

    float moveSpeed = 2.0f;
    float attackRange = 200.0f;

    void MoveToPlayer(Point playerPos);
    void Attack(ObjList& add_base);

public:
    COctrock(Point p, CMap* _map, CSpawnPoint* _sp);

    int Action(const ObjList& base, ObjList& add_base) override;
    void Draw() override;
};