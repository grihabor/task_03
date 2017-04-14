#ifndef TASK_03_OBJECT_H
#define TASK_03_OBJECT_H

#include "GL.h"
#include "Mesh.h"
#include "vec3.h"
#include "vec2.h"
#include "Texture.h"
#include <vector>

class Object : public GL::Mesh
{
private:
    std::string name;

    //std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< VM::vec3 > vertices;
    std::vector< VM::vec2 > uvs;
    std::vector< VM::vec3 > normals;

    GL::Texture texture;
    GLuint indexBuffer;
    VM::vec3 position;
    float scale;

public:
    Object(std::string name, std::string texture_path, VM::vec3 position, float scale);
    void Draw(const GL::Camera &camera) override;
    void Create() override;
};

#endif //TASK_03_OBJECT_H
