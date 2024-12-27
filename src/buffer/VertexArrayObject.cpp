#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &vaoID);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &vaoID);
}

void VertexArrayObject::bindVAO() const
{
    glBindVertexArray(vaoID);
}

void VertexArrayObject::unbindVAO() const
{
    glBindVertexArray(0);
}

void VertexArrayObject::deleteVAO()
{
    glDeleteVertexArrays(1, &vaoID);
}

void VertexArrayObject::enableAttribute(GLuint index) const
{
    glEnableVertexAttribArray(index);
}

void VertexArrayObject::disableAttribute(GLuint index) const
{
    glDisableVertexAttribArray(index);
}

void VertexArrayObject::setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) const
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}