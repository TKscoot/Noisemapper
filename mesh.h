#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

struct Vertex
{
    Vertex(){}

    Vertex(QVector3D position, QVector3D normal, QVector2D texCoords)
    {
        this->position = position;
        this->normal = normal;
        this->texCoords = texCoords;
    }

    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
};

class Mesh : protected QOpenGLFunctions
{
public:
    Mesh();
    ~Mesh();

    Mesh(QString filepath);

    void LoadMesh(QString filepath);

    void render(double deltaTime);

    // Holds the compiled shader programs.
    QOpenGLShaderProgram		*m_program = nullptr;

private:
    void LoadShaders();
    void SetupVertexData();
    void CreateVertexIndexBuffers();
    void SetupVertexLayout();

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Wraps an OpenGL VertexArrayObject (VAO)
    QOpenGLVertexArrayObject	m_vao;
    // Vertex buffer (positions and colors, interleaved storage mode).
    QOpenGLBuffer				m_vertexBufferObject;
    // Index buffer to draw two rectangles
    QOpenGLBuffer				m_indexBufferObject;


};

#endif // MESH_H
