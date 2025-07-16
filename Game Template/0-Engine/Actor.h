#pragma once

#include <string>
#include <vector>
#include <map>
#include "Raylib.h"
#include "raymath.h"
#include "AssetList.h"

using std::string;
using std::vector;
using std::map;

enum class ActorType {
    Actor,
    UI,
    None
};

class Actor {
private:
protected:
    static map<string, Actor*> ActorList;
    static vector<vector<Actor*>> ActorsByLayer;

    bool enabled{ true };
    bool hasCollision{ true };
    bool onDestroyList{ false };

    string name{ "" };
    Vector2 position{ 0,0 };
    Vector2 size{ 0,0 };
    short layer{ 0 };
    Texture2D* texture{ nullptr };
    AnimationPlayer animation{ NULL };
    Color color{ WHITE };
    ActorType type{ ActorType::None };

public:
    bool GetEnabled() { return this->enabled; }
    void SetEnabled(bool enabled) { this->enabled = enabled; }

    bool GetHasCollision() { return hasCollision; }
    void SetHasCollision(bool hasCollision) { this->hasCollision = hasCollision; }

    bool GetOnDestroyList() { return onDestroyList; }
    void SetOnDestroyList(bool onDestroyList) { this->onDestroyList = onDestroyList; }


    string GetName() { return name; }
    void SetName(string name) { this->name = name; }

    Vector2 GetPosition() { return position; }
    void SetPosition(Vector2 position) { this->position = position; }

    Vector2 GetSize() { return size; }
    void SetSize(Vector2 size) { this->size = size; }

    short GetLayer() { return layer; }
    void SetLayer(bool layer) { this->layer = layer; }

    Texture2D* GetTexture() { return texture; }
    void SetTexture(Texture2D* texture) { this->texture = texture; }

    Color GetColor() { return color; }
    void SetColor(Color color) { this->color = color; }

    ActorType GetActorType() { return type; }
    void SetActorType(ActorType type) { this->type = type; }

private:
protected:

public:
    Actor(Vector2 _pos = Vector2Zero(),
        Vector2 _size = Vector2Zero(),
        ActorType _type = ActorType::None,
        Texture2D* _sprite = AssetList::GetTexture("Unknown"),
        Color _color = WHITE);
    virtual ~Actor();
    
    virtual void Start();
    virtual void Update(Vector2* scroll);
    virtual void Draw(Vector2* scroll);
    
    //Put the actor on the Destroy list for next Update
    void Destroy();
    static void RemoveActorFromLists(Actor* go);

    //Mouse Gestion
    virtual void Clicked();
    virtual void MouseHover();
    virtual void MouseHoverUI();

    //Collision Gestion
    virtual void Collided();

    static Actor* CreateActor(const string id, int layer, Actor* actor);

    static Actor* GetActorWithName(string _name);
    static vector<Actor*> GetAllActors();
    static vector<vector<Actor*>>* GetAllActorsLayered();
    static vector<Actor*> GetAllActorsWith(ActorType type);
    static vector<Actor*> GetAllActorsInCollisionVector2(Vector2 point);
    static vector<Actor*> GetAllActorsInCollisionRect(Rectangle collider);
    static vector<Actor*> GetAllActorsInCollisionCirc(Vector2 origin, float radius);

    static void DestroyActorList();
};