#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <glad/glad.h>

class VertexArrayObject
{

public:
    GLuint vaoID;

    // Takes in the Number of VAO's and an array, where the number of VAO's are stored inside.
    VertexArrayObject();

    void bindVAO() const;

    void unbindVAO() const;

    void enableAttribute(GLuint index) const;

    void disableAttribute(GLuint index) const;

    void setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) const;

    void deleteVAO();
};

#endif