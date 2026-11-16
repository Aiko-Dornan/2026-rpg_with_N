#include"octrock.h"
#include"function.h"
#include"spawnPoint.h"
#include"expitem.h"
#include"player.h"
#include"enemyBullet.h"
#include <cmath>
#include <Windows.h>
#include <cstdio>
#include <algorithm>

COctrock::COctrock(Point p, CMap* _map, CSpawnPoint* _sp)
{
    sprite.img = Resource::octrock_img;
    sprite.cutX = 0;
    sprite.cutY = 0;
    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    map = _map;
    pos = p;

    HP(2);
    MoveSpeed(10.0f);

    spawn = _sp;

    ID = (int)ObjID::ENEMY;
    pri = 2;
}

// プレイヤーに向かって移動する
 


void COctrock::MoveToPlayer(
    Point playerPos,
    float playerWidth,
    float playerHeight,
    bool allowInsideRange)
{
    // 敵とプレイヤーの中心座標
    float enemyCenterX = pos.x + ImgWidth / 2.0f;
    float enemyCenterY = pos.y + ImgHeight / 2.0f;

    float playerCenterX = playerPos.x + playerWidth / 2.0f;
    float playerCenterY = playerPos.y + playerHeight / 2.0f;

    // 中心座標の差
    float dx = playerCenterX - enemyCenterX;
    float dy = playerCenterY - enemyCenterY;

    float absX = std::fabs(dx);
    float absY = std::fabs(dy);

    // 射線判定に必要な重なり範囲
    float rangeX = (ImgWidth + playerWidth) / 2.0f;
    float rangeY = (ImgHeight + playerHeight) / 2.0f;

    // X・Y方向の残り距離
    float gapX = absX - rangeX;
    float gapY = absY - rangeY;

    if (gapX < 0.0f)
        gapX = 0.0f;

    if (gapY < 0.0f)
        gapY = 0.0f;

    // プレイヤーとの距離
    float distance = std::sqrt(dx * dx + dy * dy);

    // 射程内で移動を許可されていない場合は停止
    if (distance <= attackRange && !allowInsideRange)
    {
        vec.x = 0.0f;
        vec.y = 0.0f;
        return;
    }

    // 一定時間ごとに移動方向を決定
    if (positionCheckTimer <= 0)
    {
        // X方向が揃っている場合はY方向
        if (gapX <= 0.0f)
        {
            moveAxis = 2;
        }
        // Y方向が揃っている場合はX方向
        else if (gapY <= 0.0f)
        {
            moveAxis = 1;
        }
        // 両方ずれている場合は残り距離が短い方
        // 同じ距離ならX方向を優先
        else if (gapX <= gapY)
        {
            moveAxis = 1;
        }
        else
        {
            moveAxis = 2;
        }

        positionCheckTimer = positionCheckInterval;
    }
    else
    {
        positionCheckTimer--;
    }

    // 移動量を初期化
    vec.x = 0.0f;
    vec.y = 0.0f;

    // X方向に移動
    if (moveAxis == 1)
    {
        if (gapX <= 0.0f)
            return;

        float move = (gapX < moveSpeed)
            ? gapX : moveSpeed;

        vec.x = (dx > 0.0f) ? move : -move;

        Dir = (dx > 0.0f)
            ? (int)DIR::RIGHT
            : (int)DIR::LEFT;
    }
    // Y方向に移動
    else if (moveAxis == 2)
    {
        if (gapY <= 0.0f)
            return;

        float move = (gapY < moveSpeed)
            ? gapY : moveSpeed;

        vec.y = (dy > 0.0f) ? move : -move;

        Dir = (dy > 0.0f)
            ? (int)DIR::DOWN
            : (int)DIR::UP;
    }

    // 移動先を計算
    Point nextPos = pos;

    nextPos.x += vec.x;
    nextPos.y += vec.y;

    // 移動可能なら位置を更新
    if (map->CanMove(nextPos, ImgWidth, ImgHeight))
    {
        pos = nextPos;
    }

    // 移動量をリセット
    vec.x = 0.0f;
    vec.y = 0.0f;
}



void COctrock::MoveToPlayer(
    Point playerPos,
    float playerWidth,
    float playerHeight)
{
    // 敵とプレイヤーの中心座標
    float enemyCenterX = pos.x + ImgWidth / 2.0f;
    float enemyCenterY = pos.y + ImgHeight / 2.0f;

    float playerCenterX = playerPos.x + playerWidth / 2.0f;
    float playerCenterY = playerPos.y + playerHeight / 2.0f;

    // 中心座標の差
    float dx = playerCenterX - enemyCenterX;
    float dy = playerCenterY - enemyCenterY;

    float absX = std::fabs(dx);
    float absY = std::fabs(dy);

    // 射線が通るために必要な重なり判定の範囲
    float rangeX = (ImgWidth + playerWidth) / 2.0f;
    float rangeY = (ImgHeight + playerHeight) / 2.0f;

    // X・Y方向の残り距離
    float gapX = absX - rangeX;
    float gapY = absY - rangeY;

    if (gapX < 0.0f)
        gapX = 0.0f;

    if (gapY < 0.0f)
        gapY = 0.0f;

    // プレイヤーとの距離
    float distance = std::sqrt(dx * dx + dy * dy);

    // 攻撃射程内なら移動しない
    if (distance <= attackRange)
    {
        vec.x = 0.0f;
        vec.y = 0.0f;
        return;
    }

    // 一定時間ごとに移動方向を決定
    if (positionCheckTimer <= 0)
    {
        // X方向が揃っているならY方向
        if (gapX <= 0.0f)
        {
            moveAxis = 2;
        }
        // Y方向が揃っているならX方向
        else if (gapY <= 0.0f)
        {
            moveAxis = 1;
        }
        // 両方ずれている場合、距離が大きい方
        // 同じ距離ならX方向を優先
        else if (gapX >= gapY)
        {
            moveAxis = 1;
        }
        else
        {
            moveAxis = 2;
        }

        positionCheckTimer = positionCheckInterval;
    }
    else
    {
        positionCheckTimer--;
    }

    // 移動量を初期化
    vec.x = 0.0f;
    vec.y = 0.0f;

    // X方向に移動
    if (moveAxis == 1)
    {
        if (gapX <= 0.0f)
            return;

        float move = (gapX < moveSpeed)
            ? gapX : moveSpeed;

        vec.x = (dx > 0.0f) ? move : -move;

        Dir = (dx > 0.0f)
            ? (int)DIR::RIGHT
            : (int)DIR::LEFT;
    }
    // Y方向に移動
    else if (moveAxis == 2)
    {
        if (gapY <= 0.0f)
            return;

        float move = (gapY < moveSpeed)
            ? gapY : moveSpeed;

        vec.y = (dy > 0.0f) ? move : -move;

        Dir = (dy > 0.0f)
            ? (int)DIR::DOWN
            : (int)DIR::UP;
    }

    // 移動先を計算
    Point nextPos = pos;

    nextPos.x += vec.x;
    nextPos.y += vec.y;

    // 移動可能なら位置を更新
    if (map->CanMove(nextPos, ImgWidth, ImgHeight))
    {
        pos = nextPos;
    }

    // 移動量をリセット
    vec.x = 0.0f;
    vec.y = 0.0f;
}



//void COctrock::MoveToPlayer(Point playerPos, float playerWidth, float playerHeight)
//{
//    // プレイヤーとの座標差
//    float dx = playerPos.x - pos.x;
//    float dy = playerPos.y - pos.y;
//
//    // 敵とプレイヤーの矩形がX方向で重なっているか
//    bool overlapX =
//        std::fabs(dx) < (ImgWidth + playerWidth) / 2.0f;
//
//    // 敵とプレイヤーの矩形がY方向で重なっているか
//    bool overlapY =
//        std::fabs(dy) < (ImgHeight + playerHeight) / 2.0f;
//
//    // 移動量を初期化
//    vec.x = 0.0f;
//    vec.y = 0.0f;
//
//    // X方向が重なっていない場合、まずX方向に移動
//    if (!overlapX)
//    {
//        float distance = std::fabs(dx) -
//            (ImgWidth + playerWidth) / 2.0f;
//
//        float move = (distance < moveSpeed)
//            ? distance : moveSpeed;
//
//        if (dx > 0)
//        {
//            Dir = (int)DIR::RIGHT;
//            vec.x = move;
//        }
//        else
//        {
//            Dir = (int)DIR::LEFT;
//            vec.x = -move;
//        }
//    }
//    // X方向が重なっていて、Y方向が重なっていない場合
//    else if (!overlapY)
//    {
//        float distance = std::fabs(dy) -
//            (ImgHeight + playerHeight) / 2.0f;
//
//        float move = (distance < moveSpeed)
//            ? distance : moveSpeed;
//
//        if (dy > 0)
//        {
//            Dir = (int)DIR::DOWN;
//            vec.y = move;
//        }
//        else
//        {
//            Dir = (int)DIR::UP;
//            vec.y = -move;
//        }
//    }
//
//    // 移動先の判定
//    Point nextPos = pos;
//    nextPos.x += vec.x;
//    nextPos.y += vec.y;
//
//    if (map->CanMove(nextPos, ImgWidth, ImgHeight))
//    {
//        pos = nextPos;
//    }
//
//    // 移動量をリセット
//    vec.x = 0.0f;
//    vec.y = 0.0f;
//}





    void COctrock::MoveToPlayer(Point playerPos)
    {
        // 敵とプレイヤーの中心座標
        float enemyCenterX = pos.x + ImgWidth / 2.0f;
        float enemyCenterY = pos.y + ImgHeight / 2.0f;

        float playerCenterX = playerPos.x + 16.0f;
        float playerCenterY = playerPos.y + 16.0f;

        // 中心座標の差
        float dx = playerCenterX - enemyCenterX;
        float dy = playerCenterY - enemyCenterY;

        vec.x = 0.0f;
        vec.y = 0.0f;

        // X方向を優先して合わせる
        if (std::fabs(dx) > 0.01f)
        {
            Dir = (dx > 0)
                ? (int)DIR::RIGHT
                : (int)DIR::LEFT;

            vec.x = (std::fabs(dx) < moveSpeed)
                ? dx
                : (dx > 0 ? moveSpeed : -moveSpeed);
        }
        // X方向が一致したらY方向を合わせる
        else if (std::fabs(dy) > 0.01f)
        {
            Dir = (dy > 0)
                ? (int)DIR::DOWN
                : (int)DIR::UP;

            vec.y = (std::fabs(dy) < moveSpeed)
                ? dy
                : (dy > 0 ? moveSpeed : -moveSpeed);
        }

        // 移動先の判定
        Point nextPos = pos;
        nextPos.x += vec.x;
        nextPos.y += vec.y;

        if (map->CanMove(nextPos, ImgWidth, ImgHeight))
        {
            pos = nextPos;
        }

        vec.x = 0.0f;
        vec.y = 0.0f;
    }


// 弾を発射する
void COctrock::Attack(ObjList& add_base)
{
    add_base.push_back(
        make_unique<CEnemyBullet>(pos, Dir, map)
    );

    attackTime = 100.0f;
}

// 敵の行動

// 敵の行動


int COctrock::Action(const ObjList& base, ObjList& add_base)
{
    // 攻撃タイマー更新
    if (attackTime > 0.0f)
    {
        attackTime--;
    }

    // 移動タイマー更新
    if (MoveMentTime > 0.0f)
    {
        MoveMentTime--;
    }

    // HPが0以下なら死亡
    if (hp <= 0)
    {
        FLAG = false;

        if (spawn != nullptr)
        {
            spawn->enemyAlive = false;
        }

        return 0;
    }

    // プレイヤーを取得
    CPlayer* player = nullptr;

    for (const auto& obj : base)
    {
        if (obj->ID == (int)ObjID::PLAYER)
        {
            player = dynamic_cast<CPlayer*>(obj.get());

            if (player != nullptr)
            {
                break;
            }
        }
    }

    // プレイヤーが見つからなければ終了
    if (player == nullptr)
    {
        return 0;
    }

    // 敵とプレイヤーの中心座標
    float enemyCenterX = pos.x + ImgWidth / 2.0f;
    float enemyCenterY = pos.y + ImgHeight / 2.0f;

    float playerCenterX = player->pos.x + player->ImgWidth / 2.0f;
    float playerCenterY = player->pos.y + player->ImgHeight / 2.0f;

    // 座標の差
    float dx = playerCenterX - enemyCenterX;
    float dy = playerCenterY - enemyCenterY;

    // プレイヤーとの距離
    float distance = std::sqrt(dx * dx + dy * dy);

    // X方向・Y方向の重なり判定
    float rangeX = (ImgWidth + player->ImgWidth) / 2.0f;
    float rangeY = (ImgHeight + player->ImgHeight) / 2.0f;

    bool overlapX = std::fabs(dx) <= rangeX;
    bool overlapY = std::fabs(dy) <= rangeY;

    // X方向またはY方向が重なっていれば射線が通っていると判定
    bool lineOfSight = overlapX || overlapY;

    // 後退を終了する距離
    float retreatEndDistance = attackRange + 30.0f;

    // 射程内で射線が通っていなければ後退開始
    if (distance <= attackRange && !lineOfSight)
    {
        isRetreating = true;
    }

    // 後退中の処理
    if (isRetreating)
    {
        // 射線が通った場合は後退を終了
        if (lineOfSight)
        {
            isRetreating = false;
        }
        // 十分な距離が空いたら後退終了
        else if (distance >= retreatEndDistance)
        {
            isRetreating = false;
        }
        else
        {
            // プレイヤーから直線的に後退
            MoveAwayFromPlayer(
                player->pos,
                player->ImgWidth,
                player->ImgHeight
            );

            return 0;
        }
    }

    // 射程内かつ射線が通っている場合は攻撃
    if (distance <= attackRange && lineOfSight)
    {
        // プレイヤーに向きを合わせる
        if (std::fabs(dx) >= std::fabs(dy))
        {
            Dir = (dx >= 0.0f)
                ? (int)DIR::RIGHT
                : (int)DIR::LEFT;
        }
        else
        {
            Dir = (dy >= 0.0f)
                ? (int)DIR::DOWN
                : (int)DIR::UP;
        }

        // 移動停止
        vec.x = 0.0f;
        vec.y = 0.0f;

        // 攻撃タイマーが終了していれば攻撃
        if (attackTime <= 0.0f)
        {
            Attack(add_base);
        }
    }
    // 射程外なら位置調整
    else
    {
        MoveToPlayer(
            player->pos,
            player->ImgWidth,
            player->ImgHeight,
            true
        );
    }

    return 0;
}


//int COctrock::Action(const ObjList& base, ObjList& add_base)
//{
//    // 攻撃・移動タイマー更新
//    if (attackTime > 0)
//        attackTime--;
//
//    if (MoveMentTime > 0)
//        MoveMentTime--;
//
//    // HPが0以下なら死亡
//    if (hp <= 0)
//    {
//        FLAG = false;
//
//        if (spawn != nullptr)
//            spawn->enemyAlive = false;
//
//        return 0;
//    }
//
//    // プレイヤーを探す
//    CPlayer* player = nullptr;
//
//    for (int i = 0; i < base.size(); i++)
//    {
//        BaseVector* obj = base[i].get();
//
//        if (obj->ID == (int)ObjID::PLAYER)
//        {
//            player = dynamic_cast<CPlayer*>(obj);
//            break;
//        }
//    }
//
//    if (player == nullptr)
//        return 0;
//
//    // 敵とプレイヤーの中心座標の差
//    float dx =
//        (player->pos.x + player->ImgWidth / 2.0f) -
//        (pos.x + ImgWidth / 2.0f);
//
//    float dy =
//        (player->pos.y + player->ImgHeight / 2.0f) -
//        (pos.y + ImgHeight / 2.0f);
//
//    // 中心座標同士の距離
//    float distance = std::sqrt(dx * dx + dy * dy);
//
//    // X方向に重なっているか
//    bool overlapX =
//        pos.x < player->pos.x + player->ImgWidth &&
//        pos.x + ImgWidth > player->pos.x;
//
//    // Y方向に重なっているか
//    bool overlapY =
//        pos.y < player->pos.y + player->ImgHeight &&
//        pos.y + ImgHeight > player->pos.y;
//
//    // 射線が通っているか
//    bool lineOfSight = overlapX || overlapY;
//
//    // 後退を開始する距離
//    float retreatRange = 80.0f;
//
//    // プレイヤーとの距離が近すぎる場合
//    if (distance <= retreatRange)
//    {
//        // プレイヤーから後退
//        MoveAwayFromPlayer(
//            player->pos,
//            player->ImgWidth,
//            player->ImgHeight
//        );
//    }
//    // 射程内かつ射線が通っている場合
//    else if (distance <= attackRange && lineOfSight)
//    {
//
//        // プレイヤーの方向を向く
//        if (overlapX && overlapY)
//        {
//            if (std::fabs(dx) > std::fabs(dy))
//            {
//                Dir = (dx > 0)
//                    ? (int)DIR::RIGHT
//                    : (int)DIR::LEFT;
//            }
//            else
//            {
//                Dir = (dy > 0)
//                    ? (int)DIR::DOWN
//                    : (int)DIR::UP;
//            }
//        }
//        else if (overlapX)
//        {
//            Dir = (dy > 0)
//                ? (int)DIR::DOWN
//                : (int)DIR::UP;
//        }
//        else
//        {
//            Dir = (dx > 0)
//                ? (int)DIR::RIGHT
//                : (int)DIR::LEFT;
//        }
//
//        // 移動停止
//        vec.x = 0.0f;
//        vec.y = 0.0f;
//
//        // 攻撃タイマーが0なら攻撃
//        if (attackTime <= 0)
//        {
//            Attack(add_base);
//        }
//        // ここは現在の攻撃処理をそのまま使用
//        // プレイヤーの方向を向く処理
//        // 攻撃タイマーの処理
//    }
//    // それ以外の場合
//    else
//    {
//        // プレイヤーに接近
//        MoveToPlayer(
//            player->pos,
//            player->ImgWidth,
//            player->ImgHeight
//        );
//    }
//
//    return 0;
//}

//int COctrock::Action(const ObjList& base, ObjList& add_base)
//{
//    // 攻撃・移動タイマー更新
//    if (attackTime > 0)
//        attackTime--;
//
//    if (MoveMentTime > 0)
//        MoveMentTime--;
//
//    // HPが0以下なら死亡
//    if (hp <= 0)
//    {
//        FLAG = false;
//
//        if (spawn != nullptr)
//            spawn->enemyAlive = false;
//
//        // 経験値アイテム生成処理がある場合はここに記述
//        return 0;
//    }
//
//    // プレイヤーを探す
//    CPlayer* player = nullptr;
//
//    for (int i = 0; i < base.size(); i++)
//    {
//        BaseVector* obj = base[i].get();
//
//        if (obj->ID == (int)ObjID::PLAYER)
//        {
//            player = dynamic_cast<CPlayer*>(obj);
//            break;
//        }
//    }
//
//    if (player == nullptr)
//        return 0;
//
//    // プレイヤーとの距離
//    // プレイヤーと敵の中心座標の差
//    float dx =
//        (player->pos.x + player->ImgWidth / 2.0f) -
//        (pos.x + ImgWidth / 2.0f);
//
//    float dy =
//        (player->pos.y + player->ImgHeight / 2.0f) -
//        (pos.y + ImgHeight / 2.0f);
//
//    // 中心座標同士の距離
//    float distance = std::sqrt(dx * dx + dy * dy);
//
//    // X方向に重なっているか
//    bool overlapX =
//        pos.x < player->pos.x + player->ImgWidth &&
//        pos.x + ImgWidth > player->pos.x;
//
//    // Y方向に重なっているか
//    bool overlapY =
//        pos.y < player->pos.y + player->ImgHeight &&
//        pos.y + ImgHeight > player->pos.y;
//
//    // 攻撃範囲内かつ、XかYのどちらかが重なっている場合
//    if (distance <= attackRange && (overlapX || overlapY))
//    {
//        // プレイヤーの方向を向く
//        if (std::fabs(dx) > std::fabs(dy))
//        {
//            Dir = (dx > 0)
//                ? (int)DIR::RIGHT
//                : (int)DIR::LEFT;
//        }
//        else
//        {
//            Dir = (dy > 0)
//                ? (int)DIR::DOWN
//                : (int)DIR::UP;
//        }
//
//        vec.x = 0.0f;
//        vec.y = 0.0f;
//
//        // 攻撃タイマーが0なら発射
//        if (attackTime <= 0)
//            Attack(add_base);
//    }
//    else
//    {
//        // 攻撃できない場合はプレイヤーに向かって移動
//        /*MoveToPlayer(
//            player->pos,
//            player->ImgWidth,
//            player->ImgHeight
//        );*/
//        MoveToPlayer(player->pos);
//    }
//
//    return 0;
//}

// 描画
void COctrock::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);
}




// プレイヤーから後退する


void COctrock::MoveAwayFromPlayer(
    Point playerPos,
    float playerWidth,
    float playerHeight)
{
    // 敵とプレイヤーの中心座標
    float enemyCenterX = pos.x + ImgWidth / 2.0f;
    float enemyCenterY = pos.y + ImgHeight / 2.0f;

    float playerCenterX = playerPos.x + playerWidth / 2.0f;
    float playerCenterY = playerPos.y + playerHeight / 2.0f;

    float dx = enemyCenterX - playerCenterX;
    float dy = enemyCenterY - playerCenterY;

    // 移動量を初期化
    vec.x = 0.0f;
    vec.y = 0.0f;

    // 距離が大きい方向へ後退する
    // 同じ距離ならX方向を優先
    if (std::fabs(dx) >= std::fabs(dy))
    {
        vec.x = (dx >= 0.0f) ? moveSpeed : -moveSpeed;

        Dir = (dx >= 0.0f)
            ? (int)DIR::RIGHT
            : (int)DIR::LEFT;
    }
    else
    {
        vec.y = (dy >= 0.0f) ? moveSpeed : -moveSpeed;

        Dir = (dy >= 0.0f)
            ? (int)DIR::DOWN
            : (int)DIR::UP;
    }

    // 移動先を計算
    Point nextPos = pos;

    nextPos.x += vec.x;
    nextPos.y += vec.y;

    // 移動可能なら位置を更新
    if (map->CanMove(nextPos, ImgWidth, ImgHeight))
    {
        pos = nextPos;
    }

    vec.x = 0.0f;
    vec.y = 0.0f;
}


//void COctrock::MoveAwayFromPlayer(Point playerPos,
//    float playerWidth, float playerHeight)
//{
//    // 中心座標
//    float enemyCenterX = pos.x + ImgWidth / 2.0f;
//    float enemyCenterY = pos.y + ImgHeight / 2.0f;
//
//    float playerCenterX = playerPos.x + playerWidth / 2.0f;
//    float playerCenterY = playerPos.y + playerHeight / 2.0f;
//
//    float dx = playerCenterX - enemyCenterX;
//    float dy = playerCenterY - enemyCenterY;
//
//    // 後退速度（通常移動より遅くする）
//    float retreatSpeed = 2.0f;
//
//    vec.x = 0.0f;
//    vec.y = 0.0f;
//
//    // X方向とY方向のうち、距離が大きい方向へ後退
//    if (std::fabs(dx) >= std::fabs(dy))
//    {
//        if (dx > 0)
//        {
//            vec.x = -retreatSpeed;
//            Dir = (int)DIR::LEFT;
//        }
//        else
//        {
//            vec.x = retreatSpeed;
//            Dir = (int)DIR::RIGHT;
//        }
//    }
//    else
//    {
//        if (dy > 0)
//        {
//            vec.y = -retreatSpeed;
//            Dir = (int)DIR::UP;
//        }
//        else
//        {
//            vec.y = retreatSpeed;
//            Dir = (int)DIR::DOWN;
//        }
//    }
//
//    // 移動先の判定
//    Point nextPos = pos;
//    nextPos.x += vec.x;
//    nextPos.y += vec.y;
//
//    if (map->CanMove(nextPos, ImgWidth, ImgHeight))
//    {
//        pos = nextPos;
//    }
//
//    vec.x = 0.0f;
//    vec.y = 0.0f;
//}