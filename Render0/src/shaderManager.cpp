/*
* Define os programas de shader, os l? e carrega
*/

#include "render0/render0.hpp"

void initShader(int shader, const char* fileVertex, const char* fileFragment) {
    shaderPrograms[shader].shadersID = shader;
    shaderPrograms[shader].shaderInfo[VERT] =
    { GL_VERTEX_SHADER, fileVertex };
    shaderPrograms[shader].shaderInfo[FRAG] =
    { GL_FRAGMENT_SHADER, fileFragment };
    shaderPrograms[shader].shaderInfo[NOT_USING] =
    { GL_NONE, NULL };
}

void initShaderPrograms() {
    
    initShader(SIMPLE, "shaders/triangles.vert", "shaders/triangles.frag");
    initShader(SIMPLE_CAMERA, "shaders/trianglesWithCamera.vert", "shaders/trianglesWithCamera.frag");
    initShader(SIMPLE_MVP, "shaders/simpleModelWithMVP.vert", "shaders/simpleModelWithMVP.frag"); 
}

static const GLchar* ReadShader(const char* filename)
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

GLuint LoadShaders(shaderInfo_t* shaders)
{
    if (shaders == NULL) { return 0; }

    GLuint program = glCreateProgram();

    shaderInfo_t* entry = shaders;
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