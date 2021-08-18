#include "miscStdHeaders.h"

#include "render0/render0.hpp"
#include "RZ_API/RZ_api.hpp"
#include "logAPI.hpp"

namespace rz {

    int testTriangle(void) {
        return renderSimple(getTestTriangleInfo());
    }
}

renderInfo_t getTestTriangleInfo()
{
    GLuint  VAOs[NumVAOs];
    GLuint  Buffers[NumBuffers];

    RZ_TRACE("Desenhar triangulinho colorido de teste...");

    const GLuint  NumVertices = 3;

    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    //criei typedef render.hpp
    struct VertexData {
        GLubyte color[4];
        GLfloat position[4];
    };

    VertexData vertices[NumVertices] = {
        {{ 255,   0,   0, 255 }, { -0.90f, -0.90f }},
        {{   0, 255,   0, 255 }, {  0.85f, -0.90f }},
        {{   0,   0, 255, 255 }, { -0.90f,  0.85f }},
    };


    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    RZ_TRACE("Buffers preparados...");

    shaderInfo_t  shaders[] =
    {
        { GL_VERTEX_SHADER, "shaders/triangles.vert" },
        { GL_FRAGMENT_SHADER, "shaders/triangles.frag" },
        { GL_NONE, NULL }
    };

    RZ_TRACE("Carregar shaders...");

    GLuint program = LoadShaders(shaders);
    RZ_TRACE("Ligar programa...");
    glUseProgram(program);

    RZ_TRACE("Shaders ligados...");

    glVertexAttribPointer(vPosition, 2, GL_FLOAT
        , GL_FALSE, sizeof(VertexData)
        , BUFFER_OFFSET(sizeof(vertices[0].color)));
    glVertexAttribPointer(vColor, 4, GL_UNSIGNED_BYTE
        , GL_TRUE, sizeof(VertexData)
        , BUFFER_OFFSET(0));

    //glEnableVertexAttribArray(vColor);
    //glEnableVertexAttribArray(vPosition);

    RZ_TRACE("Vai renderizar...");

    const rz::colorf_t black = { 0.0f, 0.0f, 0.0f, 0.0f };

    renderInfo_t renderInfo = {
        black[0],
        black[1],
        black[2],
        black[3],
        VAOs[Triangles],
        NumVertices,
        renderWindow.window
    };

    return renderInfo;
}