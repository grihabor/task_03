#include "Texture.h"
#include <SOIL.h>
#include <VM/vec2.h>
#include <vector>
#include <VM/vec4.h>

using namespace std;

namespace GL {

    void bindTexture(GLuint program, const string& name, GLuint texture, GLint unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);                                            CHECK_GL_ERRORS
        glBindTexture(GL_TEXTURE_2D, texture);                                          CHECK_GL_ERRORS

        GLint location = glGetUniformLocation(program, name.c_str());                   CHECK_GL_ERRORS
        if(location >=0 ) {
            glUniform1i(location, unit);                                                CHECK_GL_ERRORS
        }
    }

    Texture LoadTexture(const char *filename, GLuint border_mode, GLuint interpolation_mode)
    {
        Texture texture;
        glGenTextures(1, &texture.id);                                              CHECK_GL_ERRORS


        // load texture
        unsigned char *textureData = SOIL_load_image(filename, &texture.width,
                                                    &texture.height, &texture.n, 4);
        if(!textureData){
            cout << "stbi_load failed: " << filename << endl;
            return texture;
        } else {
            cout << "stbi_load success! " << filename << endl;
        }


        // pass texture to OpenGL
        glBindTexture(GL_TEXTURE_2D, texture.id);                                   CHECK_GL_ERRORS
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);                    CHECK_GL_ERRORS
        glGenerateMipmap(GL_TEXTURE_2D);                                            CHECK_GL_ERRORS



        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, border_mode);                    CHECK_GL_ERRORS
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, border_mode);                    CHECK_GL_ERRORS


        //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,interpolation_mode);            CHECK_GL_ERRORS
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,interpolation_mode);            CHECK_GL_ERRORS

        // free image data
        SOIL_free_image_data(textureData);
        return texture;
    }

    int LoadCoords(const char *filename, Texture t, vector<VM::vec4>& coords,
                   vector<VM::vec2>& uvcoords, vector<unsigned char>& faces)
    {
        ifstream file(filename);
        string s;
        int maxIndex = 0;

        float ratio = t.width / float(t.height);

        while(file >> s){
            if(s == "v"){
                string x, y;
                file >> x >> y;

                VM::vec2 coord;

                if(x == "max"){
                    coord.x = 1.f;
                } else {
                    coord.x = strtof(x.c_str(), nullptr) / t.width;
                }

                if(y == "max"){
                    coord.y = 1.f;
                } else {
                    coord.y = strtof(y.c_str(), nullptr) / t.height;
                }

                uvcoords.push_back(VM::vec2(coord.x, coord.y));
                coords.push_back(VM::vec4(ratio*(1.f - coord.x), 1.f - coord.y, 0.f, 1.f));
            } else if(s == "f") {
                unsigned char index;
                for(int i = 0; i < 3; ++i){
                    file >> index;
                    if(index > maxIndex){
                        maxIndex = index;
                    }
                    faces.push_back(index - '1');
                }
            } else {
                throw string("Failed to parse: ") + filename;
            }

        }

        return maxIndex - '0';
    }
}
