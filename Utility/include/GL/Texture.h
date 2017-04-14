#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <map>
#include <set>
#include <vector>

#include "VM/vec2.h"
#include "VM/vec4.h"
#include "GL.h"

#include "ShaderProgram.h"

using std::vector;

namespace GL {


    struct Texture
    {
        int width, height, n;
        GLuint id;
    };

    void bindTexture(GLuint program, const std::string& name, GLuint texture, GLint unit=0);

    Texture LoadTexture(const char *filename, GLuint mode, GLuint interpolation_mode);
    int LoadCoords(const char *filename, Texture t, vector<VM::vec4>& coords,
                   vector<VM::vec2>& uvcoords, vector<unsigned char>& faces);
}

#endif // GL_TEXTURE_H
