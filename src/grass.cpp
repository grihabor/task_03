#include "grass.h"



Grass::Grass(GL::Camera& camera_ref)
        : grassVarianceData(GRASS_INSTANCES),
          camera(camera_ref)
{}

// Обновление смещения травинок
void Grass::UpdateGrassVariance() {
    // Генерация случайных смещений
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i].x = (float)rand() / RAND_MAX / 100;
        grassVarianceData[i].z = (float)rand() / RAND_MAX / 100;
    }

    // Привязываем буфер, содержащий смещения
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    // Загружаем данные в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Рисование травы
void Grass::Draw() {
    // Тут то же самое, что и в рисовании земли
    glUseProgram(grassShader);                                                   CHECK_GL_ERRORS
    GLint cameraLocation = glGetUniformLocation(grassShader, "camera");          CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS
    // Обновляем смещения для травы
    UpdateGrassVariance();
    // Отрисовка травинок в количестве GRASS_INSTANCES
    glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount, GRASS_INSTANCES);   CHECK_GL_ERRORS
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

double Crop(double a)
{
    if(a < 0.)
        return 0.;
    if(a > 1.)
        return 1.;
    return a;
}

vector<VM::vec2> Grass::GenerateGrassPositions() {
    vector<VM::vec2> grassPositions(GRASS_INSTANCES);
    double width = sqrt(GRASS_INSTANCES);
    double k = 1;

    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        double x = i % int(width);
        double y = i / int(width);
        x += k * (float(rand()) / RAND_MAX);
        y += k * (float(rand()) / RAND_MAX);
        x = Crop(x/width);
        y = Crop(y/width);

        grassPositions[i] = VM::vec2(x, y);
    }
    return grassPositions;
}

// Здесь вам нужно будет генерировать меш
vector<VM::vec4> GenMesh(uint n) {
    return {
            VM::vec4(0, 0, 0, 1),
            VM::vec4(1, 0, 0, 1),
            VM::vec4(1, .5, 0, 1),

            VM::vec4(0, 0, 0, 1),
            VM::vec4(1, .5, 0, 1),
            VM::vec4(0, .5, 0, 1),

            VM::vec4(0, .5, 0, 1),
            VM::vec4(1, .5, 0, 1),
            VM::vec4(.5, 1, 0, 1),
    };
}

vector<VM::vec2> GenUV(const vector<VM::vec4> &mesh)
{
    vector<VM::vec2> uv;
    for(const VM::vec4 & v : mesh){
        uv.push_back(VM::vec2(v.x, v.y));
    }
    return uv;
}

// Создание травы
void Grass::Create() {
    uint LOD = 1;
    // Создаём меш
    vector<VM::vec4> grassPoints = GenMesh(LOD);
    vector<VM::vec2> uvPoints = GenUV(grassPoints);

    // Сохраняем количество вершин в меше травы
    grassPointsCount = grassPoints.size();
    // Создаём позиции для травинок
    vector<VM::vec2> grassPositions = GenerateGrassPositions();
    // Инициализация смещений для травинок
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = VM::vec4(0, 0, 0, 0);
    }

    grassShader = GL::CompileShaderProgram("grass");

    // VAO
    glGenVertexArrays(1, &grassVAO);                                             CHECK_GL_ERRORS
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS

    // VBO
    GLuint buffers[3];
    int bufCount = sizeof(buffers)/sizeof(buffers[0]);
    glGenBuffers(bufCount, buffers);                                              CHECK_GL_ERRORS

    GLuint pointsBuffer = buffers[0];
    GLuint positionBuffer = buffers[1];
    grassVariance = buffers[2];

    GLuint pointsLocation = glGetAttribLocation(grassShader, "point");           CHECK_GL_ERRORS
    GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
    GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS


    // mesh points
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints.size(),
                 grassPoints.data(), GL_STATIC_DRAW);                            CHECK_GL_ERRORS


    glEnableVertexAttribArray(pointsLocation);                                   CHECK_GL_ERRORS
    glVertexAttribPointer(pointsLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS


    // grass position
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositions.size(),
                 grassPositions.data(), GL_STATIC_DRAW);                         CHECK_GL_ERRORS

    glEnableVertexAttribArray(positionLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
    glVertexAttribDivisor(positionLocation, 1);                                  CHECK_GL_ERRORS


    // grass variance
    glGenBuffers(1, &grassVariance);                                             CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES,
                 grassVarianceData.data(), GL_STATIC_DRAW);                      CHECK_GL_ERRORS

    glEnableVertexAttribArray(varianceLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(varianceLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    glVertexAttribDivisor(varianceLocation, 1);                                  CHECK_GL_ERRORS

    // Отвязываем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}