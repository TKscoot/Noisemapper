#include "mesh.h"
#include "glviewportwidget.h"

#include <QApplication>
#include <QColor>
#include <QResizeEvent>
#include <QResource>

#include <cmath>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/cimport.h>

Mesh::Mesh()
{
    // Defaults

    // Vertices
    m_vertices = {
        Vertex( QVector3D(0.8f,   0.8f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)),  // top right
        Vertex( QVector3D(0.8f,  -0.8f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)),  // bottom right
        Vertex( QVector3D(-0.8f, -0.8f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f)),  // bottom left
        Vertex( QVector3D(-0.8f,  0.8f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector2D(0.0f, 0.0f))  // top left
    };

    // Indices
    m_indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    LoadShaders();

    SetupVertexData();

    CreateVertexIndexBuffers();

    SetupVertexLayout();
    GLViewportWidget* v;
    for (auto &widget : QApplication::allWidgets())
    {
        if (widget->objectName() == "viewportWidget") {
           v = static_cast<GLViewportWidget*>(widget);
           break;
        }
    }
}

Mesh::Mesh(QString filepath)
{
    LoadMesh(filepath);

    LoadShaders();

    SetupVertexData();

    CreateVertexIndexBuffers();

    SetupVertexLayout();
    GLViewportWidget* v;
    for (auto &widget : QApplication::allWidgets())
    {
        if (widget->objectName() == "viewportWidget") {
           v = static_cast<GLViewportWidget*>(widget);
           break;
        }
    }
}

void Mesh::LoadMesh(QString filepath)
{
    unsigned int flags =
            aiProcess_Triangulate |
            aiProcess_GenNormals;

    Assimp::Importer importer;

    //QFile file(filepath);
    //file.open(QIODevice::ReadOnly);
    //auto data = file.readAll();
    //file.close();

    //const aiScene* scene = importer.ReadFileFromMemory(data.constData(), data.length(), flags);

    const aiScene* scene = importer.ReadFile(filepath.toStdString(), flags);

    if (!scene)
    {
        qCritical() << "Mesh did not load correctly!" << importer.GetErrorString();
        return;
    }

    if (!scene->HasMeshes())
    {
        qCritical() << "Scene has no meshes!" << importer.GetErrorString();
        return;
    }

    int totalVerts = 0;
    int totalIndices = 0;

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        totalVerts += scene->mMeshes[i]->mNumVertices;

        totalIndices += scene->mMeshes[i]->mNumFaces * 3;
    }

    m_vertices.resize(totalVerts);
    m_indices.resize(totalIndices);

    int offsetVerts = 0;
    int offsetIndices = 0;

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];

        for (int v = 0; v < mesh->mNumVertices; v++)
        {

            // Position
            m_vertices[v + offsetVerts].position = QVector3D(
                                    mesh->mVertices[v].x,
                                    mesh->mVertices[v].y,
                                    mesh->mVertices[v].z);
            // Normal
            m_vertices[v + offsetVerts].normal   = QVector3D(
                                    mesh->mNormals[v].x,
                                    mesh->mNormals[v].y,
                                    mesh->mNormals[v].z);
        }

        for (int f = 0; f < mesh->mNumFaces; f++)
        {
            aiFace* face = mesh->mFaces + f;
            for (int i = 0; i < face->mNumIndices; i++)
            {
                m_indices[((3 * f) + i + offsetIndices)] = *(face->mIndices + i);
            }
        }

        offsetVerts += mesh->mNumVertices;
        offsetIndices += mesh->mNumFaces * 3;
    }
}

Mesh::~Mesh()
{
    // resource cleanup
    m_vao.destroy();
    m_vertexBufferObject.destroy();
    m_indexBufferObject.destroy();
    delete m_program;
}

void Mesh::render(double deltaTime)
{
    if(!this->isInitialized(this->d_ptr))
    {
        // this function is called once, when the window is first shown, i.e. when
        // the the window content is first rendered
        this->initializeOpenGLFunctions();
    }


    static float timer = 0.0f;
    timer += deltaTime;

    // use our shader program
    m_program->bind();

    QMatrix4x4 world;
    world.setToIdentity();
    world.translate(std::sin(timer * 100.0f), 0.0f, 0.0f);
    world.rotate(std::sin(timer * 100.0f) * 360.0f, QVector3D(0.0f, 1.0f, 0.0f));

    m_program->setUniformValue("world", world);

    // bind the vertex array object, which in turn binds the vertex buffer object and
    // sets the attribute buffer in the OpenGL context
    m_vao.bind();

    // For old Intel drivers you may need to explicitely re-bind the index buffer, because
    // these drivers do not remember the binding-state of the index/element-buffer in the VAO
    //	m_indexBufferObject.bind();

    // now draw the two triangles via index drawing
    // - GL_TRIANGLES - draw individual triangles via elements
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    // finally release VAO again (not really necessary, just for completeness)
    m_vao.release();
}

void Mesh::LoadShaders()
{
    // build and compile our shader program
    // ------------------------------------

    m_program = new QOpenGLShaderProgram();

    // read the shader programs from the resource
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/pass_through.vert")){
        auto l = m_program->log();
        qCritical() << "Vertex shader errors:\n" << l;
    }

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag")){
        auto l = m_program->log();
        qCritical() << "Fragment shader errors:\n" << l;
    }

    if (!m_program->link()){
        auto l = m_program->log();
        qCritical() << "Shader linker errors:\n" << l;
    }

}

void Mesh::SetupVertexData()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


}

void Mesh::CreateVertexIndexBuffers()
{
    // create a new buffer for the vertices and colors, interleaved storage
    m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBufferObject.create();
    m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBufferObject.bind();
    // now copy buffer data over: first argument pointer to data, second argument: size in bytes
    m_vertexBufferObject.allocate(m_vertices.data(), m_vertices.size()*sizeof(Vertex) );

    // create and bind Vertex Array Object - must be bound *before* the element buffer is bound,
    // because the VAO remembers and manages element buffers as well
    m_vao.create();
    m_vao.bind();

    // create a new buffer for the indexes
    m_indexBufferObject = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer); // Mind: use 'IndexBuffer' here
    m_indexBufferObject.create();
    m_indexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBufferObject.bind();
    m_indexBufferObject.allocate(m_indices.data(), m_indices.size() * sizeof(uint32_t));
}

void Mesh::SetupVertexLayout()
{
    // stride = number of bytes for one vertex (with all its attributes) = 3+3 floats = 6*4 = 24 Bytes
    int stride = sizeof(Vertex);

    m_program->bind();
    // layout location 0 - vec3 with coordinates
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);

    // layout location 1 - vec3 with colors
    m_program->enableAttributeArray(1);
    int colorOffset = 3*sizeof(float);
    m_program->setAttributeBuffer(1, GL_FLOAT, colorOffset, 3, stride);

    m_program->enableAttributeArray(2);
    int uvOffset = 6*sizeof(float);
    m_program->setAttributeBuffer(2, GL_FLOAT, uvOffset, 2, stride);
}


