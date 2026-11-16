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
    float attackRange = 250.0f;

    // プレイヤー位置の確認タイマー
    int positionCheckTimer = 0;

    // プレイヤー位置を確認する間隔
    int positionCheckInterval = 60;

    // 後退中かどうか
    bool isRetreating = false;

    // 移動方向
    // 0 = 移動なし
    // 1 = X方向
    // 2 = Y方向
    int moveAxis = 0;

    void MoveToPlayer(Point playerPos);
    void MoveToPlayer(Point playerPos, float playerWidth, float playerHeight);
    void MoveToPlayer(
        Point playerPos,
        float playerWidth,
        float playerHeight,
        bool allowInsideRange = false
    );
    void MoveAwayFromPlayer(Point playerPos,float playerWidth, float playerHeight);
    void Attack(ObjList& add_base);

public:
    COctrock(Point p, CMap* _map, CSpawnPoint* _sp);

    int Action(const ObjList& base, ObjList& add_base) override;
    void Draw() override;
};