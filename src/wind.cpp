#include <cmath>
#include <VM/vec2.h>
#include "wind.h"

Wind::Wind()
        : windDirection(0.3),
          windPhase(0),
          maxWindMagnitude(0.03),
          windFrequency(3)
{}

VM::vec2 Wind::GetForce(VM::vec2 position)
{

    float t = cos(windDirection) * position.x -
              sin(windDirection) * position.y;

    float windMagnitude = std::sin(windFrequency * 1.57 * t + windPhase);
    windMagnitude *= windMagnitude;

    windMagnitude *= maxWindMagnitude;

    return VM::vec2(sin(windDirection), cos(windDirection)) * windMagnitude;
}

void Wind::Update(int deltaTime)
{
    windPhase += 1e-3 * deltaTime;
    windDirection += 7e-5 * deltaTime;
}