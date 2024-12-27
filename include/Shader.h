#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SDLGL.h>

class Shader
{
public:
    GLuint shaderID;

    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void useShaderProgram() const;
    void deleteShaderProgram();

private:
    std::string readFile(const std::string &filePath);

    void compile(const char *vertexShaderCode, const char *fragmentShaderCode);

    GLuint compileShader(const char *code, GLenum shaderType);

    void checkCompileErrors(GLuint object, const std::string &type);
};

#endif