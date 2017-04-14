#include "object.h"
#include <iterator>
#include <sstream>

using std::string;
using std::istream_iterator;

struct FileException : std::exception
{
    std::string exception_string;
    const char* what() const noexcept override
    {
        return exception_string.c_str();
    }

    FileException(std::string filename)
            :exception_string(std::string("Failed to open file: ") + filename + "\n")
    {}
};

struct ObjException : std::exception
{
    std::string exception_string;
    const char* what() const noexcept override
    {
        return exception_string.c_str();
    }

    ObjException(std::string exception_string)
            :exception_string(exception_string)
    {}
};


void Object::Draw(const GL::Camera &camera)
{
    glUseProgram(shader);                                                 CHECK_GL_ERRORS

    GLint cameraLocation = glGetUniformLocation(shader, "camera");        CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
    camera.getMatrix().data().data());                       CHECK_GL_ERRORS

    GLint positionLocation = glGetUniformLocation(shader, "position");        CHECK_GL_ERRORS
    glUniform3f(positionLocation,
                position.x, position.y, position.z);                       CHECK_GL_ERRORS

    GLint scaleLocation = glGetUniformLocation(shader, "scale");        CHECK_GL_ERRORS
    glUniform1f(scaleLocation, scale);                       CHECK_GL_ERRORS

    glBindVertexArray(vao);                                               CHECK_GL_ERRORS
    glBindTexture(GL_TEXTURE_2D, texture.id);                                   CHECK_GL_ERRORS
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);                         CHECK_GL_ERRORS

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    /*
    glDrawElements(
            GL_TRIANGLES,
            vertexIndices.size(),
            GL_UNSIGNED_INT,
            0
            );                                           CHECK_GL_ERRORS
*/
    // Отсоединяем VAO
    glBindVertexArray(0);                                                       CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);                                                            CHECK_GL_ERRORS
}

void Object::Create()
{

    // VBO
    GLuint buffers[2];
    int bufCount = sizeof(buffers)/sizeof(buffers[0]);
    glGenBuffers(bufCount, buffers);                                              CHECK_GL_ERRORS

    GLuint meshBuffer = buffers[0];
    GLuint uvBuffer = buffers[1];

    GLuint pointsLocation = glGetAttribLocation(shader, "point");           CHECK_GL_ERRORS
    GLuint uvpointLocation = glGetAttribLocation(shader, "uvpoint");        CHECK_GL_ERRORS


    // indices
    /*
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);                        CHECK_GL_ERRORS
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(vertexIndices[0]),
                 vertexIndices.data(), GL_STATIC_DRAW);     CHECK_GL_ERRORS
    */

    // mesh points
    BindDataAndAttribute(meshBuffer, vertices, pointsLocation, 3, false);

    // uv points
    BindDataAndAttribute(uvBuffer, uvs, uvpointLocation, 2, false);

    // Отвязываем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

Object::Object(std::string filename, std::string texture_path, VM::vec3 pos, float scale)
    : Mesh(filename), position(pos), scale(scale)
{
    texture = GL::LoadTexture(texture_path.c_str(), GL_REPEAT, GL_LINEAR);

    filename = std::string("mesh/") + filename + ".obj";
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException(filename);
    }

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

    std::string line_string;
    while (std::getline(file, line_string)) {

        // skip comments
        if (line_string[0] == '#') {
            continue;
        }

        std::stringstream line(line_string);
        std::string lineHeader;
        line >> lineHeader;

        if (lineHeader == "v") {
            VM::vec3 vertex;
            line >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);

        } else if (lineHeader == "vt") {
            VM::vec2 uv;
            line >> uv.x >> uv.y;
            //uv.x = 1. - uv.x;
            uv.y = 1. - uv.y;
            uvs.push_back(uv);

        } else if (lineHeader == "vn") {
            VM::vec3 normal;
            line >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (lineHeader == "f") {

            vector<string> vertices{istream_iterator<string>(line),
                                  istream_iterator<string>()};

            if(vertices.size() > 4){
                std::cout << "Obj import: file has faces with more than 4 vertices! (unsupported)" << std::endl;
            }

            for(int i = 0; i < 3; ++i){
                vector<string> vertex = split(vertices[i], '/');

                vertexIndices.push_back(atoi(vertex[0].c_str())-1);
                uvIndices.push_back(atoi(vertex[1].c_str())-1);
                normalIndices.push_back(atoi(vertex[2].c_str())-1);
            }

            if(vertices.size() == 3){
                // if triangle, then we are done
                continue;
            }

            // if square
            for(int i = 2; i < 5; ++i){
                vector<string> vertex = split(vertices[i%4], '/');

                vertexIndices.push_back(atoi(vertex[0].c_str())-1);
                uvIndices.push_back(atoi(vertex[1].c_str())-1);
                normalIndices.push_back(atoi(vertex[2].c_str())-1);
            }


        } else if (lineHeader == "o") {
            // object header
            // parse only one object in the file
            line >> name;
        } else if (lineHeader == "s") {
            // smooth shading
            // skip
            continue;
        } else if (lineHeader == "mtllib") {
            // mtl library to use
            // skip
            std::string mtl_file;
            line >> mtl_file;
        } else if (lineHeader == "usemtl") {
            // material ot use
            // skip
            std::string material;
            line >> material;
        } else {
            throw ObjException("Unknown tag: " + lineHeader + "\n" +
                               "    in line: " + line_string + "\n");
        }
    }


    // i don't know if it is possible to use different
    // indices for mesh and texture, so here is bad solution
    // Sorry :(
    vector<VM::vec2> tempuv;
    for(auto index: uvIndices){
        tempuv.push_back(uvs[index]);
    }
    uvs = tempuv;

    vector<VM::vec3> tempvert;
    for(auto index: vertexIndices){
        tempvert.push_back(vertices[index]);
    }
    vertices = tempvert;


}


