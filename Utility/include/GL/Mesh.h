#ifndef TASK_03_MESH_H_H
#define TASK_03_MESH_H_H

#include <vector>
#include <string>
#include "vec3.h"
#include "vec2.h"
#include "Camera.h"
#include "GL.h"

namespace GL
{
    class Mesh
    {
    protected:
        GLuint shader;
        GLuint vao;

        template<typename T>
        void BindDataAndAttribute(GLuint buffer, std::vector<T> &data, GLuint location, int nValues, bool eachInstance)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffer);                                 CHECK_GL_ERRORS
            glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * data.size(),
                         data.data(), GL_STATIC_DRAW);                            CHECK_GL_ERRORS


            glEnableVertexAttribArray(location);                                   CHECK_GL_ERRORS
            glVertexAttribPointer(location, nValues, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS
            if(eachInstance){
                // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
                glVertexAttribDivisor(location, 1);                                       CHECK_GL_ERRORS
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

    private:
        void CreateVAO();

    public:
        Mesh(std::string filename);
        virtual void Draw(const Camera &camera) = 0;
        virtual void Create() = 0;
        virtual ~Mesh();
    };
}

#endif //TASK_03_MESH_H_H
