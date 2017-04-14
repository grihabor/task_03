#ifndef TASK_03_GRASS_H
#define TASK_03_GRASS_H

#include "Utility.h"
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include "wind.h"

using std::vector;


class Grass : public GL::Mesh
{
private:
    const uint GRASS_INSTANCES = 10000; // Количество травинок
    const char *FILENAME_TEXTURE_GRASS = "Texture/grass_1.png";

    GLuint indexBuffer;
    int nTriangles;
    GL::Texture texture;

    bool windFlag = true;
    Wind wind;
    int prevTimestamp;

    vector<VM::vec4> grassPoints;
    vector<VM::vec2> uvPoints;
    vector<unsigned char> faces;
    vector<unsigned char> indices;
    vector<VM::vec2> grassPositions;
    vector<float> grassRotations;

    std::vector<VM::vec4> spots;

    GLuint grassVariance;    // Буфер для смещения координат травинок
    vector<VM::vec2> grassVarianceData; // Вектор со смещениями для координат травинок
    vector<VM::vec2> grassVelocity; // Вектор со смещениями для координат травинок

private:
    void InitMeshAndUV();
    vector<float> GenerateGrassRotations();
    vector<VM::vec2> GenerateGrassPositions();

public:
    Grass(std::vector<VM::vec4> spots);
    void UpdateGrassVariance();
    void Draw(const GL::Camera &camera) override;
    void Create() override;
    void WindSwitch(){windFlag = !windFlag;}
};

#endif //TASK_03_GRASS_H
