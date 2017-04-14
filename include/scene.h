#ifndef TASK_03_SCENE_H
#define TASK_03_SCENE_H

#include "Camera.h"
#include "Mesh.h"
#include "GL.h"
#include <deque>
#include <memory>

class Scene
{
private:
    static std::shared_ptr<Scene> pScene;

    GL::Camera camera;              // Мы предоставляем Вам реализацию камеры. В OpenGL камера - это просто 2 матрицы. Модельно-видовая матрица и матрица проекции. // ###
                                    // Задача этого класса только в том чтобы обработать ввод с клавиатуры и правильно сформировать эти матрицы.
                                    // Вы можете просто пользоваться этим классом для расчёта указанных матриц.

    // Размеры экрана
    uint screenWidth = 2*800;
    uint screenHeight = 2*600;

    // Это для захвата мышки. Вам это не потребуется (это не значит, что нужно удалять эту строку)
    bool captureMouse = true;

    // scene objects to render
    std::deque<std::shared_ptr<GL::Mesh>> meshes;

    bool (*userKeyboardEvents)(GL::Camera &camera, unsigned char key);

public:

    static std::shared_ptr<Scene> GetScene(int argc, char **argv,
                                           bool (*customKeyboardEvents)(GL::Camera &camera, unsigned char key));

    void AddMesh(GL::Mesh *mesh, bool front = false);

    // Инициализация окна
    void InitializeGLUT(int argc, char **argv);

    // Создаём камеру (Если шаблонная камера вам не нравится, то можете переделать, но я бы не стал)
    void CreateCamera();

private:
    Scene(int argc, char **argv, bool (*customKeyboardEvents)(GL::Camera &camera, unsigned char key));

    // Эта функция вызывается для обновления экрана
    static void RenderLayouts();

    // Завершение программы
    void FinishProgram();

    // Обработка события нажатия клавиши (специальные клавиши обрабатываются в функции SpecialButtons)
    static void KeyboardEvents(unsigned char key, int x, int y);

    // Обработка события нажатия специальных клавиш
    static void SpecialButtons(int key, int x, int y);

    static void IdleFunc();

    // Обработка события движения мыши
    static void MouseMove(int x, int y);

    // Обработка нажатия кнопки мыши
    static void MouseClick(int button, int state, int x, int y);

    // Событие изменение размера окна
    static void windowReshapeFunc(GLint newWidth, GLint newHeight);

};

#endif //TASK_03_SCENE_H
