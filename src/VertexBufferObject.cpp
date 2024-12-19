#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(GLsizeiptr sizeInBytes, const GLvoid *data, GLenum usage)
{
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, usage);
}

void VertexBufferObject::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void VertexBufferObject::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Delete()
{
    glDeleteBuffers(1, &vboID);
}
