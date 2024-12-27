#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject()
{
    glGenBuffers(1, &eboID);
}

ElementBufferObject::~ElementBufferObject()
{
    glDeleteBuffers(1, &eboID);
}

void ElementBufferObject::bindEBO() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
}

void ElementBufferObject::unbindEBO() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::deleteEBO()
{
    glDeleteBuffers(1, &eboID);
}

void ElementBufferObject::bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    glBufferData(target, size, data, usage);
}