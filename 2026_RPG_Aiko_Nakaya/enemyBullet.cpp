#include"enemyBullet.h"
#include"function.h"

CEnemyBullet::CEnemyBullet(Point p, int _dir, CMap* _map)
{
	sprite.img = Resource::player_img;
	sprite.cutX = 96;
	sprite.cutY = 32;
	sprite.width = 32;
	sprite.height = 32;

	pos = p;
	Dir = _dir;
	//マップ情報
	map = _map;

	//剣の向き
	if (Dir == (int)DIR::UP)
	{
		vec.y=-5.0f;//上
	}
	else if (Dir == (int)DIR::DOWN)
	{
		vec.y=5.0f;//下
	}
	else if (Dir >= (int)DIR::LEFT_DOWN && Dir <= (int)DIR::LEFT_UP)
	{
		vec.x=-5.0f;//左
	}
	else
	{
		vec.x=5.0f;//右

	}

	
	sprite.angle = RADIAN(angle);

	pri = 2;
	ID = (int)ObjID::ENEMYBULLET;

	/*Point draw_pos = CameraToScreen(pos, map->camera);

	map->camera.x = draw_pos.x - WINDOW_WIDTH / 2;
	map->camera.y = draw_pos.y - WINDOW_HEIGHT / 2;*/

}

int CEnemyBullet::Action(const ObjList& base, ObjList& add_base)
{
	/*Point draw_pos = CameraToScreen(pos, map->camera);

	map->camera.x = draw_pos.x - WINDOW_WIDTH / 2;
	map->camera.y = draw_pos.y - WINDOW_HEIGHT / 2;*/

	pos.x += vec.x;
	
	pos.y += vec.y;
	
	for (int i = 0; i < base.size(); i++) 
		{
			BaseVector* a = base[i].get();
		
			if (a->ID == (int)ObjID::PLAYER)
			{
				pl_pos = a->pos;
			}

		}

	if (pos.x> pl_pos .x+WINDOW_WIDTH|| pos.x < pl_pos.x - WINDOW_WIDTH|| pos.y > pl_pos.y + WINDOW_HEIGHT|| pos.y < pl_pos.y - WINDOW_HEIGHT)
	{
		FLAG = false;
	}
	

	return 0;
}

void CEnemyBullet::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x + vec.x, draw_pos.y + vec.y);
}

CEnemyBullet::~CEnemyBullet()
{
	FLAG = false;
}