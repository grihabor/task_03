#include "skybox.h"
#include <vector>
#include "SOIL.h"
#include "GL/glext.h"
#include "Texture.h"

using std::vector;
using std::cout;
using std::endl;

Skybox::Skybox()
        : Mesh("skybox") {}

// Функция, рисующая землю
void Skybox::Draw(const GL::Camera &camera) {

    glCullFace(GL_FRONT);     CHECK_GL_ERRORS
    glDepthMask(GL_FALSE);    CHECK_GL_ERRORS


    glUseProgram(shader);    CHECK_GL_ERRORS

    GLint cameraLocation = glGetUniformLocation(shader, "camera");    CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE,
                       camera.getMatrix().data().data());    CHECK_GL_ERRORS

    GLint camera_posLocation = glGetUniformLocation(shader, "camera_pos");    CHECK_GL_ERRORS
    glUniform3f(camera_posLocation, camera.position.x,
        camera.position.y, camera.position.z);    CHECK_GL_ERRORS

    glBindVertexArray(vao);    CHECK_GL_ERRORS
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);    CHECK_GL_ERRORS

    glDrawArrays(GL_TRIANGLES, 0, 36);    CHECK_GL_ERRORS

    // Отсоединяем VAO
    glBindVertexArray(0);    CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);    CHECK_GL_ERRORS


    glDepthMask(GL_TRUE);    CHECK_GL_ERRORS
    glCullFace(GL_FRONT_AND_BACK);     CHECK_GL_ERRORS
}


void Skybox::LoadTexture() {
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    std::vector<std::string> texture_filenames = {
            "skybox_right.png",
            "skybox_left.png",
            "skybox_top.png",
            "skybox_bottom.png",
            "skybox_front.png",
            "skybox_back.png"
    };
    std::vector<GLuint> texture_targets = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for (GLuint i = 0; i < texture_filenames.size(); i++) {
        unsigned char *image = SOIL_load_image(
                ("Texture/" + texture_filenames[i]).c_str(), &texture.width,
                &texture.height, 0, SOIL_LOAD_RGB
        );
        if (image) {
            std::cout << "Success: Texture/" << texture_filenames[i] << " loaded" << std::endl;
        }
        glTexImage2D(
                texture_targets[i], 0, GL_RGB, texture.width, texture.height,
                0, GL_RGB, GL_UNSIGNED_BYTE, image
        );

        SOIL_free_image_data(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

vector<VM::vec3> Skybox::GetMesh()
{

    vector<VM::vec3> faces[6];
    faces[0] = vector<VM::vec3>(
            {
                    // Positions
                    VM::vec3(-1.0f, 1.0f, -1.0f),
                    VM::vec3(-1.0f, -1.0f, -1.0f),
                    VM::vec3(1.0f, -1.0f, -1.0f),
                    VM::vec3(1.0f, -1.0f, -1.0f),
                    VM::vec3(1.0f, 1.0f, -1.0f),
                    VM::vec3(-1.0f, 1.0f, -1.0f)
            }
    );
    faces[1] = vector<VM::vec3>(
            {
                    VM::vec3(-1.0f, -1.0f, 1.0f),
                    VM::vec3(-1.0f, -1.0f, -1.0f),
                    VM::vec3(-1.0f, 1.0f, -1.0f),
                    VM::vec3(-1.0f, 1.0f, -1.0f),
                    VM::vec3(-1.0f, 1.0f, 1.0f),
                    VM::vec3(-1.0f, -1.0f, 1.0f)
            }
    );

    faces[2] = vector<VM::vec3>(
            {
                    VM::vec3(1.0f, -1.0f, -1.0f),
                    VM::vec3(1.0f, -1.0f, 1.0f),
                    VM::vec3(1.0f, 1.0f, 1.0f),
                    VM::vec3(1.0f, 1.0f, 1.0f),
                    VM::vec3(1.0f, 1.0f, -1.0f),
                    VM::vec3(1.0f, -1.0f, -1.0f)
            }
    );

    faces[3] = vector<VM::vec3>(
            {
                    VM::vec3(-1.0f, -1.0f, 1.0f),
                    VM::vec3(-1.0f, 1.0f, 1.0f),
                    VM::vec3(1.0f, 1.0f, 1.0f),
                    VM::vec3(1.0f, 1.0f, 1.0f),
                    VM::vec3(1.0f, -1.0f, 1.0f),
                    VM::vec3(-1.0f, -1.0f, 1.0f)
            }
    );

    faces[4] = vector<VM::vec3>(
            {
                    VM::vec3(-1.0f, 1.0f, -1.0f),
                    VM::vec3(1.0f, 1.0f, -1.0f),
                    VM::vec3(1.0f, 1.0f, 1.0f),
                    VM::vec3(1.0f, 1.0f, 1.0f),
                    VM::vec3(-1.0f, 1.0f, 1.0f),
                    VM::vec3(-1.0f, 1.0f, -1.0f)
            }
    );

    faces[5] = vector<VM::vec3>(
            {
                    VM::vec3(-1.0f, -1.0f, -1.0f),
                    VM::vec3(-1.0f, -1.0f, 1.0f),
                    VM::vec3(1.0f, -1.0f, -1.0f),
                    VM::vec3(1.0f, -1.0f, -1.0f),
                    VM::vec3(-1.0f, -1.0f, 1.0f),
                    VM::vec3(1.0f, -1.0f, 1.0f)
            }
    );

    vector<VM::vec3> meshPoints;
    for(int i = 0; i < 6; ++i)
        meshPoints.insert(meshPoints.end(), faces[i].begin(), faces[i].end());

    return meshPoints;
}

// Создаём замлю
void Skybox::Create() {


    vector<VM::vec3> meshPoints = GetMesh();

    // VBO
    GLuint vboArray[1];
    int bufferCount = sizeof(vboArray) / sizeof(vboArray[0]);
    glGenBuffers(bufferCount, vboArray);    CHECK_GL_ERRORS

    GLuint pointsVBO = vboArray[0];

    this->LoadTexture();

    GLuint pointLocation = glGetAttribLocation(shader, "point");    CHECK_GL_ERRORS


    // mesh points
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshPoints[0]) * meshPoints.size(),
                 meshPoints.data(), GL_STATIC_DRAW);    CHECK_GL_ERRORS

    glEnableVertexAttribArray(pointLocation);
    CHECK_GL_ERRORS
    glVertexAttribPointer(pointLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL_ERRORS


// uv points
//glBindBuffer(GL_ARRAY_BUFFER, uvVBO);                                       CHECK_GL_ERRORS
//glBufferData(GL_ARRAY_BUFFER, sizeof(uvPoints[0]) * uvPoints.size(),
//             uvPoints.data(), GL_STATIC_DRAW);                              CHECK_GL_ERRORS


// Отвязываем VAO
    glBindVertexArray(0);    CHECK_GL_ERRORS
// Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);    CHECK_GL_ERRORS

}