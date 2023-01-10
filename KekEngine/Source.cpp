#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "KekEngine/Graphics/VertexArray.h"
#include "KekEngine/Graphics/Shader.h"
#include "KekEngine/Graphics/Shaders.h"

using namespace Kek;
using namespace Graphics;

// Improve window constructors.
// TOKENS // LOG LINE AND FILE OF ERRORS AND WARNINGS // AUTO SPACE BETWEEN ARGUMENTS
// Window has a renderer which is initialized things are getting drown to a specific window.

Application app({ "KekEngine",vec2i(800,600) });
Shader shader(vertexShaderSource, fragmentShaderSource);

struct Vertex
{
    vec2f pos;
    vec3f col;
};
VertexArray1<Vertex, unsigned char> mesh({{FLOAT, 2}, {FLOAT, 3}});
void Kek::Setup()
{
    mesh.AddVertices({{vec2f(0.5f, 0.5f), vec3f(1, 1, 0)},
                      {vec2f(0.5f, -0.5f), vec3f(1, 0, 0)},
                      {vec2f(-0.5f, 0.5f), vec3f(0, 1, 0)},
                      {vec2f(-0.5f, -0.5f), vec3f(0, 0, 0)}});
    mesh.AddIndices({0, 1, 2, 2, 1, 3});
}
void Kek::Update()
{

    if (app.WindowCount() == 0 || app[0].GetKey(KEY_ESCAPE))
    {
        app.Quit();
        return;
    }
    shader.Bind();
    mesh.Draw();

    app[0].SwapBuffers();
}