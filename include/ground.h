#ifndef TASK_03_GROUND_H
#define TASK_03_GROUND_H

#include "Utility.h"
#include "Texture.h"
#include "Mesh.h"

class Ground : public GL::Mesh
{
private:
    GLuint groundShader; // Шейдер для земли
    GLuint groundVAO; // VAO для земли
    GL::Texture texture;

public:
    Ground();
    void Draw(const GL::Camera &camera) override;
    void Create() override;
};

#endif //TASK_03_GROUND_H
