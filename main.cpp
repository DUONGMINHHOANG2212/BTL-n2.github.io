#include "CommonFun.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "Timer.h"
#include "Threats.h"
#include"Text.h"

#define BUG_SCRORE 20

BaseObject g_background;
TTF_Font* font_time = NULL;

using namespace std;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret<0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window = SDL_CreateWindow("Game cua Hoang", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1,SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }

        if(TTF_Init()==-1)
        {
            success = false;
        }
        font_time = TTF_OpenFont("font/dlxfont_.ttf", FONT_SIZE);
        if(font_time==NULL)
        {
            success = false;
        }
    }
      return success;
}

bool LoadBackgroung()
{
    bool ret = g_background.LoadBackGr("image/background/3.png", g_screen);
    if(ret==false)
        return false;

    return true;
}

void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen=NULL;
    SDL_DestroyWindow(g_window);
    g_window=NULL;

    IMG_Quit();
    SDL_Quit();
}

std::vector<ThreatsObj*> ThreatsList()
{
    std::vector<ThreatsObj*> list_threats;

    ThreatsObj* dynamic_threats = new ThreatsObj[20];//threat di chuyen
    for(int i = 0; i< 20; i++)
    {
        ThreatsObj* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("image/threats_dong/1/threats5.png", g_screen);
            p_threat->set_clip();
            p_threat->set_type_move(ThreatsObj::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i*500); // dieu chinh lai
            p_threat->set_y_pos(200);

            int pos1 = p_threat->get_x_pos()-60;
            int pos2 = p_threat->get_x_pos()+60;
            p_threat->set_animationPos(pos1,pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }

    ThreatsObj* threats_objs = new ThreatsObj[20];//threat co dinh
    for(int i = 0;i < 20; i++)
    {
        ThreatsObj* p_threat = (threats_objs+i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("image/threats_tinh/threat51.png", g_screen);
            p_threat->set_clip();
            p_threat->set_x_pos(700 + i*1200); // dieu chinh lai
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatsObj::STATIC_THREAT);

            BulletObj* p_bullet = new BulletObj();
            p_threat->InitBullet(p_bullet, g_screen);

            list_threats.push_back(p_threat);
        }

    }

    return list_threats;
}

int main(int argc, char** argv)
{

    int SCORE = BUG_SCRORE + 1;
    ImpTimer fps_timer;
    if(InitData()==false)
        return -1;
    if(LoadBackgroung()==false)
        return -1;

    GameMap game_map;
    game_map.LoadMap("map/map.dat");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("image/player/p1/player_right.png", g_screen);
    p_player.set_clips();

    std::vector<ThreatsObj*> threats_list = ThreatsList();
    //time text
    TextObj time_game;
    TextObj score_game;
    TextObj lifes_game;
    time_game.SetColor(TextObj::WHITE_TEXT);
    score_game.SetColor(TextObj::WHITE_TEXT);
    lifes_game.SetColor(TextObj::WHITE_TEXT);

    bool is_quit = false;
    //duy trì ctrinh
    while(!is_quit)
    {
        fps_timer.start();
        while(SDL_PollEvent(&g_event)!=0)
        {
            if(g_event.type==SDL_QUIT)
            {
                is_quit=true;
            }

            p_player.HandelInputAction(g_event, g_screen);
        }
        // set lại màu cho màn hình
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        // xóa màn hình đi reset lại để làm mới
        SDL_RenderClear(g_screen);
        // fill lại vào màn hình
        g_background.Render(g_screen, NULL);

        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPalyer(map_data);
        p_player.Show(g_screen);
        //cout<<p_player.GetScore()<<endl;

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        if(p_player.get_NUM_DIE()>3)
        {
            close();
            SDL_Quit();
            return 0;
        }

        for(int i = 0; i< threats_list.size(); i++)
        {
            ThreatsObj* p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_,map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Show(g_screen);

                // xoa dan cua threat khi cham nvat
                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCoL1= false;

                std::vector<BulletObj*> threatBullet_list = p_threat->get_bullet_list();
                for(int jj =0;jj<threatBullet_list.size();++jj)
                {

                    BulletObj* pthreat_bullet= threatBullet_list.at(jj);
                    if(pthreat_bullet != NULL)
                    {
                        bCoL1 = SDLCommonFun::CheckCollision(pthreat_bullet->GetRect(), rect_player);
                        if(bCoL1)
                        {
                           //p_threat->RemoveBullet(jj);
                           if(SCORE>0)
                           {
                               SCORE--;
                           }

                        }

                    }
                }
                // nhan vat chet
                SDL_Rect threatRect;
                        threatRect.x = p_threat->GetRect().x;
                        threatRect.y = p_threat->GetRect().y;
                        threatRect.w = p_threat->get_width_frame();
                        threatRect.h = p_threat->get_height_frame();
                bool bCol2 = SDLCommonFun::CheckCollision(rect_player, threatRect);
                if( bCol2|| SCORE == 0 )
                {
                    if(p_player.get_NUM_DIE()<=3)
                    {
                       // cout<<p_player.get_NUM_DIE()<<endl;
                        SCORE++;
                        p_player.SetRect(0,0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(500);
                        continue;
                    }
                    else
                    //if(MessageBox(NULL, L"GAME OVER",L"Info", MB_OK | MB_ICONSTOP) == IDOK)
                    {
                        p_threat->Free();
                        close();
                        SDL_Quit();
                        return 0;
                    }
                }

            }
        }

        std::vector<BulletObj*>bullet_arr = p_player.get_bullet_list();
        for(int i = 0; i< bullet_arr.size();i++)
        {
            BulletObj* p_bullet = bullet_arr.at(i);
            if(p_bullet != NULL)
            {
                for(int thr=0;thr<threats_list.size();thr++)
                {
                    ThreatsObj* obj_threat = threats_list.at(thr);
                    if(obj_threat != NULL)
                    {
                        SDL_Rect threatRect;
                        threatRect.x = obj_threat->GetRect().x;
                        threatRect.y = obj_threat->GetRect().y;
                        threatRect.w = obj_threat->get_width_frame();
                        threatRect.h = obj_threat->get_height_frame();

                        SDL_Rect bulletRect = p_bullet->GetRect();

                        bool bulletCol = SDLCommonFun::CheckCollision(bulletRect, threatRect);

                        if(bulletCol)
                        {
                            p_player.RemoveBullet(i);
                            obj_threat->Free();
                            threats_list.erase(threats_list.begin()+thr);
                        }
                    }
                }
            }
        }

        //Show game time
        std::string str_time = "Time: ";
        std::string str_score= "Score: ";
        std::string str_lifes = "Lifes: ";

        std::string score_val = std::to_string(p_player.GetScore());
        std::string lifes_val = std::to_string(3-p_player.get_NUM_DIE());

        str_score+=score_val;
        str_lifes+=lifes_val;

        score_game.SetText(str_score);
        score_game.LoadFromRenderText(font_time, g_screen);
        score_game.RenderText(g_screen, 100,15);

        lifes_game.SetText(str_lifes);
        lifes_game.LoadFromRenderText(font_time, g_screen);
        lifes_game.RenderText(g_screen, SCREEN_WIDTH/2,15);

        Uint32 time_val = SDL_GetTicks()/1000;
        Uint32 val_time = 300-time_val;
        if(val_time<0)
        {
            //if(MessageBox(NULL, L"GAME OVER",L"Info", MB_OK | MB_ICONSTOP) == IDOK)
                {
                    is_quit = true;
                    break;
                }
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time+=str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH-200, 15);
        }

        SDL_RenderPresent(g_screen);//ham xu ly

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND;//ms
        if(real_imp_time < time_one_frame)
        {
            // time thuc te be hon ly thuyet thi tao do tre de can bang
            int delay_time = time_one_frame -real_imp_time;
            if(delay_time >= 0)
                SDL_Delay(delay_time); //fps cang nho thi delay cang lon => tang fps

        }
    }

    for(int i = 0; i< threats_list.size();i++)
    {
        ThreatsObj* p_threat = threats_list.at(i);
        if(p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();

    close();
    return 0;
}


