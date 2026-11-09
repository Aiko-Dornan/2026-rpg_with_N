#include"enemyBullet.h"
#include"function.h"
#include <cmath>

CEnemyBullet::CEnemyBullet(Point p, int _dir, CMap* _map)
{
    sprite.img = Resource::player_img;
    sprite.cutX = 96;
    sprite.cutY = 32;
    sprite.width = 32;
    sprite.height = 32;

    pos = p;
    Dir = _dir;
    map = _map;

    // 移動ベクトルを初期化
    vec.x = 0.0f;
    vec.y = 0.0f;

    // 上下左右の4方向
    switch (Dir)
    {
    case (int)DIR::UP:
        vec.y = -5.0f;
        break;

    case (int)DIR::DOWN:
        vec.y = 5.0f;
        break;

    case (int)DIR::LEFT:
        vec.x = -5.0f;
        break;

    case (int)DIR::RIGHT:
        vec.x = 5.0f;
        break;
    }

    pri = 2;
    ID = (int)ObjID::ENEMYBULLET;
}

// 弾の行動
int CEnemyBullet::Action(const ObjList& base, ObjList& add_base)
{
    // 移動
    pos.x += vec.x;
    pos.y += vec.y;

    // プレイヤーの位置を取得
    Point playerPos{};

    for (int i = 0; i < base.size(); i++)
    {
        BaseVector* obj = base[i].get();

        if (obj->ID == (int)ObjID::PLAYER)
        {
            playerPos = obj->pos;
            break;
        }
    }

    // プレイヤーを基準に画面外判定
    if (pos.x > playerPos.x + WINDOW_WIDTH / 2 ||
        pos.x < playerPos.x - WINDOW_WIDTH / 2 ||
        pos.y > playerPos.y + WINDOW_HEIGHT / 2 ||
        pos.y < playerPos.y - WINDOW_HEIGHT / 2)
    {
        FLAG = false;
    }

    return 0;
}

// 描画
void CEnemyBullet::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);
}