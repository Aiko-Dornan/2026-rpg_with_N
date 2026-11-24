#include"allow.h"
#include"function.h"

CAllow::CAllow(Point p, int _dir, CMap* _map)
{
	sprite.img = Resource::bow_img;
	sprite.cutX = 0;
	sprite.cutY = 8;
	sprite.width = 32;
	sprite.height = 16;

	pos = p;
	Dir = _dir;
	//マップ情報
	map = _map;

	//剣の向き
	//if (Dir == (int)DIR::UP)
	//{
	//	angle = start_angle = 180;//上
	//}
	//else if (Dir == (int)DIR::DOWN)
	//{
	//	angle = start_angle = 0;//下
	//}
	//else if (Dir >= (int)DIR::LEFT_DOWN && Dir <= (int)DIR::LEFT_UP)
	//{
	//	angle = start_angle = -270;//左
	//}
	//else
	//{
	//	angle = start_angle = 270;//右

	//}

	// 移動ベクトルを初期化
	vec.x = 0.0f;
	vec.y = 0.0f;

	// 上下左右の4方向
	switch (Dir)
	{
	case (int)DIR::UP:
		vec.y = -5.0f;
		angle = 270;
		break;

	case (int)DIR::DOWN:
		vec.y = 5.0f;
		angle = 90;
		break;

	case (int)DIR::LEFT:
		vec.x = -5.0f;
		sprite.LR_reverse_flag = true;
		break;

	case (int)DIR::RIGHT:
		vec.x = 5.0f;
		sprite.LR_reverse_flag = false;
		break;
	}

	pri = 2;
	ID = (int)ObjID::ALLOW;

	sprite.angle = RADIAN(angle);
	/*Point draw_pos = CameraToScreen(pos, map->camera);

	map->camera.x = draw_pos.x - WINDOW_WIDTH / 2;
	map->camera.y = draw_pos.y - WINDOW_HEIGHT / 2;*/

}

int CAllow::Action(const ObjList& base, ObjList& add_base)
{
	/*Point draw_pos = CameraToScreen(pos, map->camera);

	map->camera.x = draw_pos.x - WINDOW_WIDTH / 2;
	map->camera.y = draw_pos.y - WINDOW_HEIGHT / 2;*/

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

void CAllow::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x + vec.x, draw_pos.y + vec.y);
}