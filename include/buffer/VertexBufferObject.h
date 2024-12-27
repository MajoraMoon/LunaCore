#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

#include <SDLGL.h>

class VertexBufferObject
{

public:
    GLuint vboID;

    VertexBufferObject(GLsizeiptr sizeInBytes, const GLvoid *data, GLenum usage);

    ~VertexBufferObject();

    void Bind() const;

    void Unbind() const;

    void Delete();
};

#endif