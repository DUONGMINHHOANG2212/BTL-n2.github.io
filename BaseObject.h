
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFun.h"

#define VAT_PHAM 12 // ma so vat pham
#define REVIVE 60

// Quản lý vấn đề hình ảnh

class BaseObject{
public:
    BaseObject();
    ~BaseObject();
    // hàm set kích thước cho rect
    void SetRect(const int& x, const int& y){rect_.x=x; rect_.y=y;}
    // hàm lấy ra kích thước rect
    SDL_Rect GetRect() const {return rect_;}
    // hàm lấy ra biến lưu trữ hình ảnh
    SDL_Texture* GetObject() const {return p_object_;}

    virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    bool LoadBackGr(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();
protected:
    // biến để lưu trữ các hình ảnh
    SDL_Texture* p_object_;
    // biến lưu kích thước
    SDL_Rect rect_;

};
#endif // BASE_OBJECT_H_

