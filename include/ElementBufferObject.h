#ifndef ELEMENT_BUFFER_OBJECT_H
#define ELEMENT_BUFFER_OBJECT_H

#include <glad/glad.h>

class ElementBufferObject
{

private:
public:
    GLuint eboID;

    ElementBufferObject();
    ~ElementBufferObject();

    void bindEBO() const;
    void unbindEBO() const;
    void deleteEBO();

    void bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
};

#endif