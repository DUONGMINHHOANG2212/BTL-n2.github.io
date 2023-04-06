// Quản lý nhân vật

#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>
#include "CommonFun.h"
#include "BaseObject.h"
#include "Bullet.h"


#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 20


class MainObject : public BaseObject
{
    public:
        MainObject();
        ~MainObject();

        enum WalkType
        {
            WALK_RIGHT = 0,
            WALK_LEFT = 1,
        };
        bool LoadImg(std::string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des); // show tung frame
        // Ham xu ly su kien len xuong trai phai
        void HandelInputAction(SDL_Event events, SDL_Renderer* screen);
        void set_clips();//hieu ung animation

        void DoPalyer(Map& map_data); // di chuyen
        void CheckToMap(Map& map_data);// va cham map
        void SetMapXY(const int map_x, const int map_y){map_x_ = map_x; map_y_ = map_y;}// luu vi tri ban do
        void CenterEntityOnMap(Map& map_data); //thong so map khi di chuyen nvat

        SDL_Rect GetRectFrame();

        void set_bullet_list(std::vector<BulletObj*> bullet_list)
        {
            p_bullet_list = bullet_list;
        }

        std::vector<BulletObj*> get_bullet_list() const {return p_bullet_list;}
        void HandleBullet(SDL_Renderer* des);
        void RemoveBullet(const int& index);
        void set_comeback_time(const int& cb_time){comeback_time=cb_time;}
        int get_NUM_DIE() const {return NUM_DIE;}
        int SetScore(const int& score_threats){money_count+=score_threats;}
        int GetScore() const {return money_count;}
private:

    int money_count;
    //bang dan
    std::vector<BulletObj*> p_bullet_list;
    // bien luu tru do tang khi di chuyen
    float x_val_;
    float y_val_;
    // vi tri nhan vat
    float x_pos_;
    float y_pos_;
    // kich thuoc 1 frame
    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8]; // so frame
    Input input_type_;
    int frame_;
    int status_;// luu khi nao left khi nao right
    bool on_ground;

    int map_x_;
    int map_y_;

    // xu ly roi
    int comeback_time;
    int NUM_DIE;
};

#endif // MAIN_OBJECT_H_

