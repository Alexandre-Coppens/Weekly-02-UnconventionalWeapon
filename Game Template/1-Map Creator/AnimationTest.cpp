#include "AnimationTest.h"
#include "AnimationTest.h"
#include <iostream>

ATest::ATest():
	Actor(Vector2{ 200,200 }, Vector2{100,100}, ActorType::Actor, AssetList::GetTexture("TestAnimation"), WHITE)
{
	animation.SetAnimation(AssetList::GetAnimation(texture));
	animation.SetSpeed(5);
	animation.SetSequence(0);
}

ATest::~ATest(){
}

void ATest::Update(Vector2* scroll){
	animation.PlayAnimation();
}

void ATest::Draw(Vector2* scroll){
	Rectangle dest{ position.x - scroll->x, position.y - scroll->y, size.x, size.y };
	DrawTexturePro(*texture, animation.GetAnimationRect(), dest, Vector2{size.x * 0.5f, size.y * 0.5f}, 0, color);
}
