#include"item.h"
#include"player.h"

void CItem::GetItem(BaseVector* b)
{
	//取得した時の処理
	CPlayer* p = dynamic_cast<CPlayer*>(b);

	switch (itemNo)
	{
	case ItemNo::EXP:
		p->exp += value;
		break;
	default:
		break;
	}

	FLAG = false;
}