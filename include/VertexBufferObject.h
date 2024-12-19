#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

#include <glad/glad.h>

class VertexBufferObject
{

public:
    GLuint vboID;

    VertexBufferObject(GLsizeiptr sizeInBytes, const GLvoid *data, GLenum usage);

    void Bind() const;

    void Unbind() const;

    void Delete();
};

#endif