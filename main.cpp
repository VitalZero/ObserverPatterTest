//#pragma once
//
//#include <vector>
//#include <memory>
//
//class CancelationToken
//{
//public:
//    inline void Release() { Active = false; }
//    inline bool Valid()const  { return Active; }
//
//    using Ptr = std::shared_ptr<CancelationToken>;
//protected:
//    bool Active = true;
//};
//
//class CancelationTokenHost
//{
//public:
//    CancelationTokenHost()
//    {
//        TokenPtr = std::make_shared<CancelationToken>();
//    }
//
//    virtual ~CancelationTokenHost()
//    {
//        TokenPtr->Release();
//    }
//
//    CancelationToken::Ptr Get() const { return TokenPtr; }
//
//private:
//    CancelationToken::Ptr TokenPtr;
//};
//
//template<typename F>
//class EventHandler
//{
//private:
//    std::vector<std::pair<F,CancelationToken::Ptr>> Callbacks;
//
//public:
//    inline void Add(F callback, CancelationToken::Ptr token = nullptr)
//    {
//        Callbacks.push_back(std::pair<F, CancelationToken::Ptr>{callback, token});
//    }
//
//    inline void Add(F callback, const CancelationTokenHost& token)
//    {
//        Callbacks.push_back(std::pair<F, CancelationToken::Ptr>{callback, token.Get()});
//    }
//
//    inline void Remove(F callback)
//    {
////         std::vector<std::pair<F, CancelationToken::Ptr>>::iterator itr = Callbacks.begin();
////         while (itr != Callbacks.end())
////         {
////             if (itr->first == callback)
////                 itr = Callbacks.erase(itr);
////             else
////                 itr++;
////         }
//    }
//
//    template<typename ...Args>
//    inline void Call(Args&&... args)
//    {
//        auto itr = Callbacks.begin();
//        while (itr != Callbacks.end())
//        {
//            bool valid = !itr->second || itr->second->Valid();
//
//            if (!valid)
//            {
//                itr = Callbacks.erase(itr);
//            }
//            else
//            {
//                itr->first(std::forward<Args>(args)...);
//                itr++;
//            }
//        }
//    }
//};
//
//class Player
//{
//  public:
//    EventHandler<std::function<void(int)>> ScoreChanged;
//};
//
//player.ScoreChange.Add([](int score){handleScoreChange(score);});

#include <raylib.h>
#include "Observer_Subject.h"
#include <string>
#include <raymath.h>

static const int WIDTH = 800;
static const int HEIGHT = 600;

class Player : public Subject
{
public:
    Player(const Vector2& pos, float radius, float speed)
        : pos(pos), dir({}), radius(radius), speed(speed)
    {}

    void Input()
    {
        if(IsKeyDown(KEY_LEFT)) dir.x = -1.0f;
        if(IsKeyDown(KEY_RIGHT)) dir.x =  1.0f;
        if(IsKeyDown(KEY_UP)) dir.y = -1.0f;
        if(IsKeyDown(KEY_DOWN)) dir.y =  1.0f;
    }

    void Update(float dt)
    {
        Vector2 vel = Vector2Scale(dir, speed * dt);

        pos = Vector2Add(pos, vel);
        dir = {};
    }

    void Draw()
    {
        DrawCircleV(pos, radius, LIGHTGRAY);
    }

    Vector2 GetPos() const { return pos; }
    void SetPos(const Vector2& pos) { this->pos = pos; }
    float GetRadius() const { return radius; }

private:
    Vector2 pos;
    Vector2 dir;
    float speed;
    float radius;
};

class Enemy
{
public:
    Enemy(const Vector2& pos, float radius, float speed)
        : pos(pos), dir({1, 1}), radius(radius), speed(speed)
    {}

    void Update(float dt)
    {
        Vector2 vel = Vector2Scale(dir, speed * dt);

        pos = Vector2Add(pos, vel);

        if(pos.x - radius < 0)
        {
            pos.x = radius;
            dir.x = 1;
        }
        if(pos.x + radius > WIDTH)
        {
            pos.x = WIDTH - radius;
            dir.x = -1;
        }
        if(pos.y - radius < 0)
        {
            pos.y = radius;
            dir.y = 1;
        }
        if(pos.y + radius > HEIGHT)
        {
            pos.y = HEIGHT - radius;
            dir.y = -1;
        }
    }

    void Draw()
    {
        DrawCircleV(pos, radius, LIME);
    }

    Vector2 GetPos() const { return pos; }
    void SetPos(const Vector2& pos) { this->pos = pos; }
    float GetRadius() const { return radius; }

private:
    Vector2 pos;
    Vector2 dir;
    float speed;
    float radius;
};

class Score : public Observer
{
public:
    virtual void OnNotify(int event) override
    {
        totalPoints += 100;
    }

    void Update(float dt)
    {

    }

    void Draw()
    {
        DrawText(std::to_string(totalPoints).c_str(), 10, 10, 20, RAYWHITE);
    }

private:
    int totalPoints = 0;
};

int main()
{
    InitWindow(WIDTH, HEIGHT, "Observer pattern test");

    Score score;
    Player player({(float)WIDTH / 2, (float)HEIGHT / 2}, 20, 200);

    player.AddObserver(&score);

    Enemy enemy({(float)WIDTH / 5, (float)HEIGHT / 5}, 30, 100);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();
        // input
        player.Input();

        // update
        enemy.Update(dt);
        player.Update(dt);

        if(CheckCollisionCircles(player.GetPos(), player.GetRadius(), enemy.GetPos(), enemy.GetRadius()))
        {
           player.Notify(1);
           player.SetPos({(float)WIDTH / 2, (float)HEIGHT / 2});
           enemy.SetPos({(float)WIDTH / 5, (float)HEIGHT / 5});
        }

        score.Update(dt);

        // draw
        ClearBackground(BLACK);
        BeginDrawing();
        {
            // Draw Here
            enemy.Draw();
            player.Draw();
            score.Draw();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
