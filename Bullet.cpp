
#include "Bullet.h"


BulletObj::BulletObj()
{
    x_val = 0;
    y_val = 0;
    is_move = false;

}

BulletObj::~BulletObj()
{

}

void BulletObj::HandleMove(const int& x_border, const int& y_border)
{
    if(bullet_direct == DIR_RIGHT)
    {
    rect_.x +=x_val;
    if(rect_.x>x_border)
    {
        is_move = false;
    }
    }
    else if(bullet_direct == DIR_LEFT)
    {
        rect_.x -= x_val;
        if(rect_.x<0)
        {
            is_move = false;
        }
    }

}

