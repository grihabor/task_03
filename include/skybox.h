#ifndef TASK_03_SKYBOX_H
#define TASK_03_SKYBOX_H

#include <vector>
#include "Utility.h"
#include "Texture.h"
#include "Mesh.h"

class Skybox : public GL::Mesh
{
private:
    GL::Texture texture;
    GLuint indexBuffer;
    vector<unsigned char> indices;

public:
    Skybox();
    void Draw(const GL::Camera &camera) override;
    void Create() override;

private:
    void LoadTexture();
    std::vector<VM::vec3> GetMesh();
};

#endif //TASK_03_SKYBOX_H
