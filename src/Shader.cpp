#include "Shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexShaderCode = readFile(vertexPath);
    std::string fragmentShaderCode = readFile(fragmentPath);

    compile(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

Shader::~Shader()
{
    glDeleteProgram(shaderID);
}

void Shader::useShaderProgram() const
{
    glUseProgram(shaderID);
}

void Shader::deleteShaderProgram()
{
    glDeleteProgram(shaderID);
}

std::string Shader::readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Unable to open file " << filePath << std::endl;

        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::compile(const char *vertexShaderCode, const char *fragmentShaderCode)
{
    GLuint vertexShader = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glLinkProgram(shaderID);
    checkCompileErrors(shaderID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::compileShader(const char *code, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(GLuint object, const std::string &type)
{
    GLint success;
    char infoLog[1024];

    if (type == "PROGRAM")
    {
        // Überprüfen, ob das Programm erfolgreich gelinkt wurde
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }
}
