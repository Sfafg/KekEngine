#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "KekEngine/Graphics/GraphicsAPI.h"
#include "KekEngine/Graphics/Shaders.h"
using namespace Kek;
using namespace Graphics;
using namespace SHADER;

// Crash when deleting window using X button. With multiple windows.
// Window has a renderer which is initialized things are getting drown to a specific window.

Application app(Window{"KekEngine", vec2i(800, 600), Transparent});

struct Vertex
{
    vec2f pos;
    vec3f col;
};
VertexArray<Vertex, unsigned char> mesh({{FLOAT, 2}, {FLOAT, 3}});
Shader shader({{vertexShaderSource, SOURCE::TEXT, VERTEX}, {fragmentShaderSource, SOURCE::TEXT, FRAGMENT}});
VertexArray<Vertex, unsigned char> mesh2;
Shader shader2;
void Kek::Setup()
{
    mesh.AddVertices({{vec2f(0.03f, 0.05f), vec3f(1, 1, 0)},
                      {vec2f(0.03f, -0.05f), vec3f(1, 0, 0)},
                      {vec2f(-0.03f, 0.05f), vec3f(0, 1, 0)},
                      {vec2f(-0.03f, -0.05f), vec3f(0, 0, 0)}});
    mesh.AddIndices({0, 1, 2, 2, 1, 3});

    app.AddWindow(Window{"KekEngine2", vec2i(800, 600), Transparent});
    shader2 = Shader({{vertexShaderSource, SOURCE::TEXT, VERTEX}, {fragmentShaderSource, SOURCE::TEXT, FRAGMENT}});
    mesh2 = VertexArray<Vertex, unsigned char>({{FLOAT, 2}, {FLOAT, 3}});
    mesh2.AddVertices({{vec2f(0.3f, 0.5f), vec3f(1, 1, 0)},
                       {vec2f(0.3f, -0.5f), vec3f(1, 0, 0)},
                       {vec2f(-0.3f, 0.5f), vec3f(0, 1, 0)},
                       {vec2f(-0.3f, -0.5f), vec3f(0, 0, 0)}});
    mesh2.AddIndices({0, 1, 2, 2, 1, 3});
}
void Kek::Update()
{
    for (int i = 0; i < app.WindowCount(); i++)
    {
        if (app[i].GetKey(KEY_ESCAPE))
        {
            app[i].Close();
            return;
        }
        app[i].ContextCurrent();
        app[i].Clear(vec4f(0.0f));
        if (i == 0)
        {
            shader.Bind();
            mesh.Draw();
        }
        else if (i == 1)
        {
            shader2.Bind();
            mesh2.Draw();
        }
        app[i].SwapBuffers();
    }



    
}