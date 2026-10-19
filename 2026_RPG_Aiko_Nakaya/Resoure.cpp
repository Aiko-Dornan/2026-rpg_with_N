//リロース
#include "Resource.h"

//int Resource::img;
int Resource::map_img;
int Resource::player_img;
int Resource::octrock_img;
int Resource::item_img;

//画像読み込み
void Resource::Load()
{
	//img = LoadGraph("");
	map_img = LoadGraph("image\\map.png");
	player_img = LoadGraph("image\\player32.png");
	octrock_img = LoadGraph("image\\octrock.png");

	item_img = LoadGraph("image\\item.png");
}

//画像削除
void Resource::Release() {
	//DeleteGraph(img);
	DeleteGraph(map_img);
	DeleteGraph(player_img);
	DeleteGraph(octrock_img);

	DeleteGraph(item_img);
}

