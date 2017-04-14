#include <GL/Texture.h>
#include "grass.h"



Grass::Grass(std::vector<VM::vec4> spots)
        : Mesh("grass"),
          grassVarianceData(GRASS_INSTANCES),
          grassVelocity(GRASS_INSTANCES),
          prevTimestamp(0),
          spots(spots),
          wind()
{}

// Обновление смещения травинок
void Grass::UpdateGrassVariance() {

    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime;
    if(prevTimestamp)
        deltaTime = timeSinceStart - prevTimestamp;
    else
        deltaTime = 0;
    prevTimestamp = timeSinceStart;

    if(windFlag) {
        wind.Update(deltaTime);
    }

    // grass characteristics
    float k = .1;
    float mass = 1e4;
    float friction = 15;
    float balancePoint = 0.1;

    for (uint i = 0; i < GRASS_INSTANCES; ++i) {

        VM::vec2 newVariance = grassVarianceData[i] + grassVelocity[i] * deltaTime;

        VM::vec2 windForce(0.f);
        if(windFlag)
            windForce = wind.GetForce(grassPositions[i]);


        VM::vec2 hookForce = (grassVarianceData[i] - VM::vec2(balancePoint)) * (-k)
                             * abs(grassVarianceData[i] - VM::vec2(balancePoint));
        VM::vec2 frictionForce = grassVelocity[i] * (- friction);


        grassVelocity[i] += (hookForce + windForce + frictionForce) * deltaTime / mass;

        grassVarianceData[i] = newVariance;
    }

    // Привязываем буфер, содержащий смещения
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    // Загружаем данные в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES,
                 grassVarianceData.data(), GL_STATIC_DRAW);                      CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Рисование травы
void Grass::Draw(const GL::Camera &camera) {
    // Тут то же самое, что и в рисовании земли
    glUseProgram(shader);                                                  CHECK_GL_ERRORS
    GLint cameraLocation = glGetUniformLocation(shader, "camera");         CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
                       camera.getMatrix().data().data());                       CHECK_GL_ERRORS

    glBindVertexArray(vao);                                                CHECK_GL_ERRORS
    glBindTexture(GL_TEXTURE_2D, texture.id);                                   CHECK_GL_ERRORS
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);                         CHECK_GL_ERRORS

    // Обновляем смещения для травы
    UpdateGrassVariance();
    // Отрисовка травинок в количестве GRASS_INSTANCES
    //glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount, GRASS_INSTANCES);   CHECK_GL_ERRORS

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glDrawElementsInstanced(
        GL_TRIANGLES,
        nTriangles*3,
        GL_UNSIGNED_BYTE,
        (void*)0,
        GRASS_INSTANCES
    );                                                                          CHECK_GL_ERRORS

    glBindVertexArray(0);                                                       CHECK_GL_ERRORS
    glUseProgram(0);                                                            CHECK_GL_ERRORS
}

double Crop(double a)
{
    if(a < 0.)
        return 0.;
    if(a > 1.)
        return 1.;
    return a;
}

#include <algorithm>

vector<VM::vec2> Grass::GenerateGrassPositions() {
    vector<VM::vec2> grassPositions(GRASS_INSTANCES);
    float width = sqrt(GRASS_INSTANCES) + .5;
    float k = 1;

    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        float x = i % int(width);
        float y = i / int(width);


        x += k * (float(rand()) / RAND_MAX + 0.4);
        y += k * (float(rand()) / RAND_MAX + 0.4);
        x = x/width;
        y = y/width;


        bool done = false;
        while(!done) {
            done = true;
            for (auto spot: spots) {
                if ((x - spot.x) * (x - spot.x) + (y - spot.z) * (y - spot.z) < spot.w * spot.w) {
                    // point in spot
                    done = false;
                    x = float(rand()) / RAND_MAX;
                    y = float(rand()) / RAND_MAX;
                    break;
                }
            }
        }




        grassPositions[i] = VM::vec2(x, y);
    }

    std::sort(grassPositions.begin(), grassPositions.end(),
              [](const VM::vec2 &v1, const VM::vec2 &v2){return v1.y > v2.y;});

    return grassPositions;
}

vector<float> Grass::GenerateGrassRotations()
{
    vector<float> grassRotations(GRASS_INSTANCES);

    for(int i = 0; i < GRASS_INSTANCES; ++i){
        grassRotations[i] = float(rand()) / RAND_MAX * 1.57f;
    }

    return grassRotations;
}

void Grass::InitMeshAndUV()
{
    grassPositions = GenerateGrassPositions();
    grassRotations = GenerateGrassRotations();

    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = VM::vec2(0, 0);
    }

    texture = GL::LoadTexture(FILENAME_TEXTURE_GRASS, GL_CLAMP_TO_BORDER, GL_LINEAR);
    nTriangles = GL::LoadCoords("mesh/grass.uv", texture, grassPoints, uvPoints, faces);
    std::cout << "grass: nTriangles = " << nTriangles << std::endl;
    for(int i = 1; i <= nTriangles; ++i){
        indices.push_back(i);
    }
}




// Создание травы
void Grass::Create() {
    InitMeshAndUV();

    // VBO
    GLuint buffers[6];
    int bufCount = sizeof(buffers)/sizeof(buffers[0]);
    glGenBuffers(bufCount, buffers);                                              CHECK_GL_ERRORS

    GLuint pointsBuffer = buffers[0];
    GLuint positionBuffer = buffers[1];
    GLuint rotationBuffer = buffers[5];
    grassVariance = buffers[2];
    GLuint uvBuffer = buffers[3];
    indexBuffer = buffers[4];


    GLuint pointsLocation = glGetAttribLocation(shader, "point");           CHECK_GL_ERRORS
    GLuint positionLocation = glGetAttribLocation(shader, "position");      CHECK_GL_ERRORS
    GLuint rotationLocation = glGetAttribLocation(shader, "rotation");      CHECK_GL_ERRORS
    GLuint varianceLocation = glGetAttribLocation(shader, "variance");      CHECK_GL_ERRORS
    GLuint uvpointLocation = glGetAttribLocation(shader, "uvpoint");        CHECK_GL_ERRORS


    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);                        CHECK_GL_ERRORS
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(faces[0]),
                 faces.data(), GL_STATIC_DRAW);     CHECK_GL_ERRORS

    // mesh points
    BindDataAndAttribute(pointsBuffer, grassPoints, pointsLocation, 4, false);

    // grass position
    BindDataAndAttribute(positionBuffer, grassPositions, positionLocation, 2, true);
    // grass rotation
    BindDataAndAttribute(rotationBuffer, grassRotations, rotationLocation, 1, true);

    // grass variance
    BindDataAndAttribute(grassVariance, grassVarianceData, varianceLocation, 2, true);

    // uv points
    BindDataAndAttribute(uvBuffer, uvPoints, uvpointLocation, 2, false);


    // Отвязываем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}