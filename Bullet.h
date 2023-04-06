
#ifndef BULLET_H_
#define BULLET_H_

#include "BaseObject.h"
#include "CommonFun.h"

#define BULLET_SPEED 20

class BulletObj : public BaseObject
{
public:
    BulletObj();
    ~BulletObj();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    void set_x_val(const int& xVal){x_val = xVal;}
    void set_y_val(const int& yVal){y_val = yVal;}
    int get_y_val() const {return y_val;}
    int get_x_val() const {return x_val;}

    void set_is_move(const bool& isMove){is_move = isMove;}
    bool get_is_move() const {return is_move;}
    void set_bullet_direct(const unsigned int& bulletdir){bullet_direct=bulletdir;}
    int get_bullet_direct() const {return bullet_direct;}
    void HandleMove(const int& x_border, const int& y_border);//giới hạn của đạn


private:
    int x_val;
    int y_val;
    bool is_move;
    unsigned int bullet_direct;//quan ly huong dan
};

#endif // BULLET_H_

