//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "miscStdHeaders.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "render0/render0.hpp"
#include "RZ_API/RZ_api.hpp"
#include "logAPI.hpp"

#define BUFFER_OFFSET(a) ((void*)(a))

typedef struct {
    GLenum       type;
    const char* filename;
    GLuint       shader;
} ShaderInfo;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];

const GLuint  NumVertices = 3;

static const GLchar*
ReadShader(const char* filename)
{
    FILE* infile = fopen(filename, "rb");

    if (!infile) {
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
        return NULL;
    }

    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    GLchar* source = new GLchar[len + 1];

    fread(source, 1, len, infile);
    fclose(infile);

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}


GLuint LoadShaders(ShaderInfo* shaders)
{
    if (shaders == NULL) { return 0; }

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while (entry->type != GL_NONE) {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        const GLchar* source = ReadShader(entry->filename);
        if (source == NULL) {
            for (entry = shaders; entry->type != GL_NONE; ++entry) {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            return 0;
        }

        glShaderSource(shader, 1, &source, NULL);
        delete[] source;

        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLsizei len;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

            GLchar* log = new GLchar[len + 1];
            glGetShaderInfoLog(shader, len, &len, log);
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete[] log;

            return 0;
        }

        glAttachShader(program, shader);

        ++entry;
    }

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        GLchar* log = new GLchar[len + 1];
        glGetProgramInfoLog(program, len, &len, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete[] log;

        for (entry = shaders; entry->type != GL_NONE; ++entry) {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        return 0;
    }

    return program;
}


namespace renderz {
    int triangles()
    {
        RZ_TRACE("Desenhar triangulinhos...");

        glGenVertexArrays(NumVAOs, VAOs);
        glBindVertexArray(VAOs[Triangles]);

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

        ShaderInfo  shaders[] =
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

        glEnableVertexAttribArray(vColor);
        glEnableVertexAttribArray(vPosition);

        RZ_TRACE("Vai renderizar...");

        const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

        while (!glfwWindowShouldClose(renderWindow.window))
        {
            glClearBufferfv(GL_COLOR, 0, black);

            glBindVertexArray(VAOs[Triangles]);

            glDrawArrays(GL_TRIANGLES, 0, NumVertices);

            glfwSwapBuffers(renderWindow.window);

            glfwPollEvents();
        }

        RZ_TRACE("Chega de triangulo...");

        return 1;
    }
}
