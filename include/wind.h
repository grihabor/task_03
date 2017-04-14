#ifndef TASK_03_WIND_H_H
#define TASK_03_WIND_H_H

struct Wind
{
    float windDirection;
    float windPhase;
    float maxWindMagnitude;
    float windFrequency;

    Wind();
    void Update(int deltaTime);
    VM::vec2 GetForce(VM::vec2 position);
};

#endif //TASK_03_WIND_H_H
