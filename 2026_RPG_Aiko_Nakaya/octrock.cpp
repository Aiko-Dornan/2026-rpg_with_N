#include"octrock.h"
#include"function.h"
#include"spawnPoint.h"
#include"expitem.h"
#include"player.h"
#include"enemyBullet.h"
#include <cmath>


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
void COctrock::MoveToPlayer(Point playerPos)
{
    float dx = playerPos.x - pos.x;
    float dy = playerPos.y - pos.y;

    vec.x = 0.0f;
    vec.y = 0.0f;

    // X座標を合わせる
    if (dx != 0.0f)
    {
        if (dx > 0)
        {
            Dir = (int)DIR::RIGHT;
            vec.x = (dx < moveSpeed) ? dx : moveSpeed;
        }
        else
        {
            Dir = (int)DIR::LEFT;
            vec.x = (dx > -moveSpeed) ? dx : -moveSpeed;
        }
    }
    // X座標が一致したらY座標を合わせる
    else if (dy != 0.0f)
    {
        if (dy > 0)
        {
            Dir = (int)DIR::DOWN;
            vec.y = (dy < moveSpeed) ? dy : moveSpeed;
        }
        else
        {
            Dir = (int)DIR::UP;
            vec.y = (dy > -moveSpeed) ? dy : -moveSpeed;
        }
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
int COctrock::Action(const ObjList& base, ObjList& add_base)
{
    // 攻撃・移動タイマー更新
    if (attackTime > 0)
        attackTime--;

    if (MoveMentTime > 0)
        MoveMentTime--;

    // HPが0以下なら死亡
    if (hp <= 0)
    {
        FLAG = false;

        if (spawn != nullptr)
            spawn->enemyAlive = false;

        // 経験値アイテム生成処理がある場合はここに記述
        return 0;
    }

    // プレイヤーを探す
    CPlayer* player = nullptr;

    for (int i = 0; i < base.size(); i++)
    {
        BaseVector* obj = base[i].get();

        if (obj->ID == (int)ObjID::PLAYER)
        {
            player = dynamic_cast<CPlayer*>(obj);
            break;
        }
    }

    if (player == nullptr)
        return 0;

    // プレイヤーとの距離
    float dx = player->pos.x - pos.x;
    float dy = player->pos.y - pos.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    // X方向に重なっているか
    bool overlapX =
        pos.x < player->pos.x + player->ImgWidth &&
        pos.x + ImgWidth > player->pos.x;

    // Y方向に重なっているか
    bool overlapY =
        pos.y < player->pos.y + player->ImgHeight &&
        pos.y + ImgHeight > player->pos.y;

    // 攻撃範囲内かつ、XかYのどちらかが重なっている場合
    if (distance <= attackRange && (overlapX || overlapY))
    {
        // プレイヤーの方向を向く
        if (std::fabs(dx) > std::fabs(dy))
        {
            Dir = (dx > 0)
                ? (int)DIR::RIGHT
                : (int)DIR::LEFT;
        }
        else
        {
            Dir = (dy > 0)
                ? (int)DIR::DOWN
                : (int)DIR::UP;
        }

        vec.x = 0.0f;
        vec.y = 0.0f;

        // 攻撃タイマーが0なら発射
        if (attackTime <= 0)
            Attack(add_base);
    }
    else
    {
        // 攻撃できない場合はプレイヤーに向かって移動
        MoveToPlayer(player->pos);
    }

    return 0;
}

// 描画
void COctrock::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);
}