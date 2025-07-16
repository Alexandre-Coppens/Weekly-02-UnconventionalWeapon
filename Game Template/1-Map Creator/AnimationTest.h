#pragma once

#include <string>
#include <vector>
#include <map>
#include "Raylib.h"
#include "Actor.h"

using std::string;
using std::vector;
using std::map;

class ATest : public Actor {
private:
public:

private:
public:
    ATest();
    ~ATest();

    void Update(Vector2* scroll)override;
    void Draw(Vector2* scroll)override;
};