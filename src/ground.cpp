#include "ground.h"
#include <vector>
#include "Texture.h"

using std::vector;
using std::cout;
using std::endl;

Ground::Ground()
    : Mesh("ground")
{}

// Функция, рисующая землю
void Ground::Draw(const GL::Camera &camera) {
    // Используем шейдер для земли
    glUseProgram(shader);                                                 CHECK_GL_ERRORS

    // Устанавливаем юниформ для шейдера. В данном случае передадим перспективную матрицу камеры
    // Находим локацию юниформа 'camera' в шейдере
    GLint cameraLocation = glGetUniformLocation(shader, "camera");        CHECK_GL_ERRORS
    // Устанавливаем юниформ (загружаем на GPU матрицу проекции?)
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
                       camera.getMatrix().data().data());                       CHECK_GL_ERRORS

    // Подключаем VAO, который содержит буферы, необходимые для отрисовки земли
    glBindVertexArray(vao);                                               CHECK_GL_ERRORS
    glBindTexture(GL_TEXTURE_2D, texture.id);                                   CHECK_GL_ERRORS

    // Рисуем землю: 2 треугольника (6 вершин)
    glDrawArrays(GL_TRIANGLES, 0, 6);                                           CHECK_GL_ERRORS

    // Отсоединяем VAO
    glBindVertexArray(0);                                                       CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);                                                            CHECK_GL_ERRORS
}

#define U(x) (1.f - x)
#define V(x) (1.f - x)

// Создаём замлю
void Ground::Create() {
    float k = 4.f;

    vector<VM::vec2> uvPoints = {
            VM::vec2(U(0.f * k), V(0.f * k)),
            VM::vec2(U(1.f * k), V(0.f * k)),
            VM::vec2(U(1.f * k), V(1.f * k)),

            VM::vec2(U(0.f * k), V(0.f * k)),
            VM::vec2(U(1.f * k), V(1.f * k)),
            VM::vec2(U(0.f * k), V(1.f * k)),
    };
    vector<VM::vec4> meshPoints = {
            VM::vec4(0, 0, 0, 1),
            VM::vec4(1, 0, 0, 1),
            VM::vec4(1, 0, 1, 1),

            VM::vec4(0, 0, 0, 1),
            VM::vec4(1, 0, 1, 1),
            VM::vec4(0, 0, 1, 1)
    };




    // VBO
    GLuint vboArray[2];
    int bufferCount = sizeof(vboArray)/sizeof(vboArray[0]);
    glGenBuffers(bufferCount, vboArray);                                        CHECK_GL_ERRORS

    GLuint pointsVBO = vboArray[0];
    GLuint uvVBO = vboArray[1];


    texture = GL::LoadTexture("Texture/ground.jpg", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
    //GL::bindTexture(groundShader, "textureSampler", texture);

    GLuint pointLocation = glGetAttribLocation(shader, "point");          CHECK_GL_ERRORS
    GLuint uvpointLocation = glGetAttribLocation(shader, "uvpoint");      CHECK_GL_ERRORS


    // mesh points
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);                                   CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshPoints[0]) * meshPoints.size(),
                 meshPoints.data(), GL_STATIC_DRAW);                            CHECK_GL_ERRORS

    glEnableVertexAttribArray(pointLocation);                                   CHECK_GL_ERRORS
    glVertexAttribPointer(pointLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS


    // uv points
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);                                       CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvPoints[0]) * uvPoints.size(),
                 uvPoints.data(), GL_STATIC_DRAW);                              CHECK_GL_ERRORS

    glEnableVertexAttribArray(uvpointLocation);                                 CHECK_GL_ERRORS
    glVertexAttribPointer(uvpointLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS


    // Отвязываем VAO
    glBindVertexArray(0);                                                       CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                           CHECK_GL_ERRORS

}