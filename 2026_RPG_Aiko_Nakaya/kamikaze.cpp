#include"kamikaze.h"
#include"function.h"
#include"spawnPoint.h"
#include "CollisionUpdate.h"
#include"expitem.h"
#include"player.h"
#include <cmath>


CKamikaze::CKamikaze(Point p, CMap* _map, CSpawnPoint* _sp)
{
    sprite.img = Resource::kamikaze_img;
    sprite.cutX = 0;
    sprite.cutY = 0;
    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    map = _map;
    pos = p;

    HP(1);

    spawn = _sp;

    ID = (int)ObjID::KAMIKAZE;

    pri = 2;
}



// 敵の行動
int CKamikaze::Action(const ObjList& base, ObjList& add_base)
{
    vec.x = 0.0f;
    vec.y = 0.0f;

    // HPが0以下なら死亡
    if (hp <= 0)
    {
        FLAG = false;

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

    // 攻撃範囲内かつ、XかYのどちらかが重なっている場合
    if (distance > 0.0f)
    {
        vec.x = (dx / distance) * moveSpeed;
        vec.y = (dy / distance) * moveSpeed;
    }
   
    pos.x += vec.x;
    pos.y += vec.y;

    return 0;
}

// 描画
void CKamikaze::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);
}

//移動向き
void CKamikaze::UpdateDir(Point target)
{
    float dx = target.x - pos.x;
    float dy = target.y - pos.y;

    if (fabs(dx) > fabs(dy))
        Dir = (dx > 0) ? RIGHT : LEFT;
    else
        Dir = (dy > 0) ? DOWN : UP;
}