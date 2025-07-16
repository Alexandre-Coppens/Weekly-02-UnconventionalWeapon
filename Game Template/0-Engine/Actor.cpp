#include "Actor.h"

map<string, Actor*> Actor::ActorList;
vector<vector<Actor*>> Actor::ActorsByLayer;

Actor::Actor(Vector2 _pos, Vector2 _size, ActorType _type, Texture2D* _sprite, Color _color):
	position(_pos),
	size(_size),
	type(_type),
	texture(_sprite),
	color(_color),
	name(""),
	enabled(true)
{
}

Actor::~Actor(){
	ActorList.erase(name);
}

void Actor::Start() {
}

void Actor::Update(Vector2* scroll){
}

//Only change Draw Function if it's not affected by scroll or others.
void Actor::Draw(Vector2* scroll) {
	Rectangle source{ 0, 0, texture->width, texture->height };
	Rectangle dest{ position.x - scroll->x, position.y - scroll->y, size.x, size.y };
	DrawTexturePro(*texture, source, dest, Vector2{ size.x * 0.5f, size.y * 0.5f }, 0, color);
}

Actor* Actor::CreateActor(const string id, int layer,Actor* actor){
	actor->name = id;
	ActorList[id] = actor;
	if (ActorsByLayer.size() < layer+1) {
		ActorsByLayer.resize(layer+1);
	}
	ActorsByLayer[layer].push_back(actor);
	actor->Start();
	return actor;
}

Actor* Actor::GetActorWithName(string _name) {
	for (auto const& i : ActorList) {
		if (i.first == _name) {
			return const_cast<Actor*>(i.second);
		}
	}
	return nullptr;
}

vector<Actor*> Actor::GetAllActors(){
	vector<Actor*> ret;
	for (auto const& i : ActorList) {
		ret.push_back(const_cast<Actor*>(i.second));
	}
	return ret;
}

vector<vector<Actor*>>* Actor::GetAllActorsLayered(){
	return &ActorsByLayer;
}

vector<Actor*> Actor::GetAllActorsWith(ActorType type){
	vector<Actor*> ret;
	for(auto const& i : ActorList){
		if (i.second->type == type) {
			ret.push_back(const_cast<Actor*>(i.second));
		}
	}
    return ret;
}

//Get every actors in a Specific Point
vector<Actor*> Actor::GetAllActorsInCollisionVector2(Vector2 point)
{
	vector<Actor*> ret{};
	for (auto const& i : ActorList) {
		if (i.second->position.x + i.second->size.x >= point.x &&
			i.second->position.x <= point.x &&
			i.second->position.y + i.second->size.y >= point.y &&
			i.second->position.y <= point.y) {
			ret.push_back(const_cast<Actor*>(i.second));
		}
	}
	return ret;
}

//Get every actors in Rectangle Collision
vector<Actor*> Actor::GetAllActorsInCollisionRect(Rectangle collider)
{
	vector<Actor*> ret{};
	for (auto const& i : ActorList) {
		if (i.second->position.x + i.second->size.x >= collider.x && 
			i.second->position.x <= collider.x + collider.width && 
			i.second->position.y + i.second->size.y >= collider.y && 
			i.second->position.y <= collider.y + collider.height) {
			ret.push_back(const_cast<Actor*>(i.second));
		}
	}
	return ret;
}

//Get every actors in Circle Collision
vector<Actor*> Actor::GetAllActorsInCollisionCirc(Vector2 origin, float radius)
{
	vector<Actor*> ret{};
	for (auto const& i : ActorList) {
		Vector2 pos = i.second->position;
		Vector2 size = i.second->size;
		if (Vector2Distance(origin, pos) <= radius ||
			Vector2Distance(origin, Vector2 {pos.x + size.x, pos.y}) <= radius ||
			Vector2Distance(origin, Vector2{ pos.x, pos.y + size.y }) <= radius ||
			Vector2Distance(origin, Vector2{ pos.x + size.x, pos.y + size.y }) <= radius) {
			ret.push_back(const_cast<Actor*>(i.second));
		}
	}
	return ret;
}

void Actor::DestroyActorList(){
	ActorList.clear();
}

void Actor::Destroy() {
	onDestroyList = true;
}

void Actor::RemoveActorFromLists(Actor* actor) {
	auto iteratorList = ActorList.find(actor->name);
	if (iteratorList != ActorList.end()) {
		ActorList.erase(iteratorList);
	}

	for (int i = 0; i < ActorsByLayer.size(); i++) {
		auto iteratorLayer = find(ActorsByLayer[i].begin(), ActorsByLayer[i].end(), actor);
		if (iteratorLayer != ActorsByLayer[i].end()) {
			ActorsByLayer[i].erase(iteratorLayer);
		}
	}

	delete actor;
}

//Function made to be modified

void Actor::Collided() {}

void Actor::MouseHover() {}

void Actor::MouseHoverUI() {}

void Actor::Clicked() {}

