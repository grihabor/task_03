#include <GL/glew.h>
#include "Mesh.h"
#include <exception>
#include <sstream>
#include <Utility.h>

GL::Mesh::Mesh(std::string filename)
{
    if(filename != "")
        shader = GL::CompileShaderProgram(filename);
        std::cout << "Mesh: shader \'" + filename + "\' loaded!" << std::endl;
    CreateVAO();
}

GL::Mesh::~Mesh()
{}


void GL::Mesh::CreateVAO()
{
    glGenVertexArrays(1, &vao);                                             CHECK_GL_ERRORS
    glBindVertexArray(vao);                                                 CHECK_GL_ERRORS
}
