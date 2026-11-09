#pragma once

#include "objBase.h"
#include"map.h"

#pragma once

class CEnemyBullet : public BaseVector
{
private:
    CMap* map = nullptr;

public:
    CEnemyBullet(Point p, int _dir, CMap* _map);

    int Action(const ObjList& base, ObjList& add_base) override;
    void Draw() override;
};

