#include <GL/Mesh.h>
#include <GL.h>
#include "scene.h"

using std::cout;
using std::endl;

std::shared_ptr<Scene> Scene::pScene(nullptr);

std::shared_ptr<Scene> Scene::GetScene(int argc, char **argv, bool (*customKeyboardEvents)(GL::Camera &camera, unsigned char key)) {
    if(!pScene){
        pScene.reset(new Scene(argc, argv, customKeyboardEvents));
    }
    return pScene;
}

Scene::Scene(int argc, char **argv, bool (*customKeyboardEvents)(GL::Camera &camera, unsigned char key))
        : userKeyboardEvents(customKeyboardEvents)
{
    InitializeGLUT(argc, argv);
    cout << "GLUT inited" << endl;

    glewInit();
    cout << "glew inited" << endl;

    CreateCamera();
    cout << "Camera created" << endl;
}



void Scene::AddMesh(GL::Mesh *mesh, bool front) {
    mesh->Create();
    auto p = std::shared_ptr<GL::Mesh>(mesh);
    if(front)
        meshes.push_front(p);
    else
        meshes.push_back(p);
}

void Scene::RenderLayouts() {
    // Включение буфера глубины
    glEnable(GL_DEPTH_TEST);
    // Очистка буфера глубины и цветового буфера
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Рисуем меши
    for(std::shared_ptr<GL::Mesh> mesh: pScene->meshes){
        mesh->Draw(pScene->camera);
    }
    glutSwapBuffers();
}

void Scene::FinishProgram() {
    glutDestroyWindow(glutGetWindow());
}

void Scene::KeyboardEvents(unsigned char key, int x, int y) {
    if(!pScene->userKeyboardEvents(pScene->camera, key)){
        pScene->FinishProgram();
    } else if (key == 'm') {
        pScene->captureMouse = !pScene->captureMouse;
        if (pScene->captureMouse) {
            glutWarpPointer(pScene->screenWidth / 2, pScene->screenHeight / 2);
            glutSetCursor(GLUT_CURSOR_NONE);
        } else {
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
        }
    }
}

void Scene::IdleFunc() {
    glutPostRedisplay();
}

void Scene::MouseMove(int x, int y) {
    if (pScene->captureMouse) {
        int centerX = pScene->screenWidth / 2,
                centerY = pScene->screenHeight / 2;
        if (x != centerX || y != centerY) {
            pScene->camera.rotateY((x - centerX) / 1000.0f);
            pScene->camera.rotateTop((y - centerY) / 1000.0f);
            glutWarpPointer(centerX, centerY);
        }
    }
}

void Scene::SpecialButtons(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        pScene->camera.rotateY(0.02);
    } else if (key == GLUT_KEY_LEFT) {
        pScene->camera.rotateY(-0.02);
    } else if (key == GLUT_KEY_UP) {
        pScene->camera.rotateTop(-0.02);
    } else if (key == GLUT_KEY_DOWN) {
        pScene->camera.rotateTop(0.02);
    }
}

void Scene::MouseClick(int button, int state, int x, int y)
{}

void Scene::windowReshapeFunc(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    pScene->screenWidth = newWidth;
    pScene->screenHeight = newHeight;

    pScene->camera.screenRatio = (float)pScene->screenWidth / pScene->screenHeight;
}


void Scene::InitializeGLUT(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 0);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Computer Graphics 3");
    glutWarpPointer(400, 300);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc(RenderLayouts);
    glutKeyboardFunc(KeyboardEvents);
    glutSpecialFunc(SpecialButtons);
    glutIdleFunc(IdleFunc);
    glutPassiveMotionFunc(MouseMove);
    glutMouseFunc(MouseClick);
    glutReshapeFunc(windowReshapeFunc);

    // enable face culling
    //glEnable( GL_CULL_FACE );
    //glFrontFace( GL_CW );
}


// Создаём камеру (Если шаблонная камера вам не нравится, то можете переделать, но я бы не стал)
void Scene::CreateCamera() {
    camera.angle = 45.0f / 180.0f * M_PI;
    camera.direction = VM::vec3(0, 0.3, -1);
    camera.position = VM::vec3(0.5, 0.2, 0);
    camera.screenRatio = (float)screenWidth / screenHeight;
    camera.up = VM::vec3(0, 1, 0);
    camera.zfar = 50.0f;
    camera.znear = 0.05f;
}
