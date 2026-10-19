#include"player.h"
#include"function.h"
#include"sword.h"

CPlayer::CPlayer(CMap* _map)
{
	sprite.img = Resource::player_img;
	sprite.cutX = 0;
	sprite.cutY = 0;
	sprite.width = ImgWidth = 32;
	sprite.height = ImgHeight = 32;

	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2 - 32;

	HP(player_hp);//HP初期化

	//マップ情報
	map = _map;

	ID = (int)ObjID::PLAYER;
	pri = 1;
}

int CPlayer::Action(const ObjList& base, ObjList& add_base)
{
	//無敵時間
	if (damageCoolTime > 0)
	{
		damageCoolTime--;
	}

	//攻撃中は操作を受け付けない
	if (att_flame>0)
	{
		att_flame--;
		return 0;
	}

	vec.x = vec.y = 0;

	//HPがなくなった場合、オブジェクト削除
	if (hp <= 0)
	{
		FLAG = false;
		
		
	}

	//ノックバック処理
	if (UpdateKnockBack())return 0;
	//if (knockFrame>0)
	//{
	//	knockFrame--;
	//	//サブステップ判定
	//	//処理回数を計算
	//	int step = (int)max(abs(knockVec.x), abs(knockVec.y));
	//	step = max(step, 1);
	//	//1ステップの移動量を計算
	//	float moveX = knockVec.x / step;
	//	float moveY = knockVec.y / step;
	//	//ステップ判定
	//	for (int i = 0; i < step; i++)
	//	{
	//		//マップの判定
	//		Point nextPos = pos;
	//		nextPos.x += moveX;
	//		if (map->CanMove(nextPos, sprite.width, sprite.height))
	//			pos.x = nextPos.x;
	//		nextPos = pos;
	//		nextPos.y += moveY;
	//		if (map->CanMove(nextPos, sprite.width, sprite.height))
	//			pos.y = nextPos.y;
	//	}
	//	//ノックバックの勢いを徐々に減速
	//	knockVec.x *= 0.8f;
	//	knockVec.y *= 0.8f;

	//	return 0;
	//}


	//移動処理
	if (CheckHitKey(KEY_INPUT_A)) vec.x = -1.0f;
	if (CheckHitKey(KEY_INPUT_D)) vec.x = 1.0f;
	if (CheckHitKey(KEY_INPUT_W)) vec.y = -1.0f;
	if (CheckHitKey(KEY_INPUT_S)) vec.y = 1.0f;

	//移動しているかどうかのフラグ
	bool isMove = (vec.x != 0 || vec.y != 0);

	//移動方向からキャラクターの向きを取得
	Dir = GetDir(Dir);

	//向きに応じて切り取り位置を変更
	if (Dir==(int)DIR::DOWN)
	{
		//下向き
		sprite.cutY = 32;
	}else if (Dir == (int)DIR::UP)
	{
		//上向き
		sprite.cutY = 0;
	}
	else
	{
		//左右方向
		sprite.cutY = 64;
		//左の場合は画像を反転させない。
		if (Dir>=(int)DIR::LEFT_DOWN&&Dir<=(int)DIR::LEFT_UP)
		{
			sprite.LR_reverse_flag = false;//左右方向の画像
			
		}
		else
		{
			sprite.LR_reverse_flag = true;
			//sprite.cutY = 64;
		}
	}

	if (isMove)
	{
		//移動ベクトル
		vec = Vector_SetLength(vec, 4.0f);
		//移動後の座標
		Point nextPos = pos;

		//x方向
		nextPos.x += vec.x;
		if (map->CanMove(nextPos, ImgWidth, ImgHeight))
			pos.x = nextPos.x;

		//y方向
		//現在の位置をもう一度コピー
		//x方向の移動結果を反映した位置からy方向を判定する
		nextPos = pos;
		nextPos.y += vec.y;
		if (map->CanMove(nextPos, ImgWidth, ImgHeight))
			pos.y = nextPos.y;

	}

	//攻撃処理
	if (Check_TriggerKey(KEY_INPUT_SPACE))
	{
		add_base.push_back(make_unique<CSword>(pos, Dir, map));

		//攻撃後の硬直フレームを設定
		att_flame = 13;
	}

	return 0;

}

////移動可能かチェック(移動後の座標,判定横幅,判定縦幅)
//bool CMap::CanMove(Point p, int width, int height)
//{
//	//座標を左上基準に修正
//	p.x = p.x - width / 2;
//	p.y = p.y - height / 2;
//
//	//判定する4隅の座標
//	Point ver[4]{
//		{p.x / CHIP_SIZE_X,p.y / CHIP_SIZE_Y},
//		{(p.x+width-1)/CHIP_SIZE_X,p.y/CHIP_SIZE_Y},
//		{(p.x + width - 1) / CHIP_SIZE_X,(p.y+height-1) / CHIP_SIZE_Y},
//		{p.x / CHIP_SIZE_X,(p.y+height-1) / CHIP_SIZE_Y},
//	};
//	
//	//4隅チェック
//	for (int i = 0;  i < 4;  i++)
//	{
//		int x = (int)ver[i].x;
//		int y = (int)ver[i].y;
//		//配列範囲チェック
//		if (x<0||x>=MAP_CHIP_X|| y< 0 || y >= MAP_CHIP_Y)
//		{
//			return false;
//		}
//		if (IsWall(map[y][x]))return false;
//		
//
//		
//
//	}
//
//	return true;
//}

void CPlayer::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x, draw_pos.y);



	DrawFormatString(draw_pos.x, draw_pos.y, 0xfffff, "HP:%d", hp);

}