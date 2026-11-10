#include "Mesh.h"
#include "OpenGLDebug.h"

#include <limits>
#include <stdexcept>
#include <utility>


Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
{
    Upload(vertices, indices);
}

Mesh::Mesh(const float* points, std::size_t pointCount, const std::uint16_t* indices, std::size_t triangleCount)
{
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> _indices;

    for (std::size_t i = 0; i < pointCount * 3; i += 3)
    {
        vec3 v = vec3(points[i], points[i+1], points[i+2]);

        Vertex vertex = Vertex(v);

        vertices.push_back(vertex);
    }


    for (std::size_t i = 0; i < triangleCount * 3; i++)
    {
        int index = indices[i];
        _indices.push_back(index);
    }


    Upload(vertices, _indices);

}

void Mesh::Upload(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
{
    if (vertices.empty())
        throw std::invalid_argument("Mesh requires vertices");

    if (indices.empty())
        throw std::invalid_argument("Mesh requires indices");

    if (indices.size() >
        static_cast<std::size_t>(std::numeric_limits<GLsizei>::max()))
    {
        throw std::overflow_error("Mesh has too many indices");
    }

    indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(std::uint32_t),
        indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, Position))
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, Normal))
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, TexCoords))
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CheckOpenGLError("uploading mesh");
}

void Mesh::Release() noexcept
{
    if (ebo != 0)
    {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }

    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    indexCount = 0;
}

Mesh::~Mesh()
{
    Release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao(std::exchange(other.vao, 0)),
    vbo(std::exchange(other.vbo, 0)),
    ebo(std::exchange(other.ebo, 0)),
    indexCount(std::exchange(other.indexCount, 0))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    vao = std::exchange(other.vao, 0);
    vbo = std::exchange(other.vbo, 0);
    ebo = std::exchange(other.ebo, 0);
    indexCount = std::exchange(other.indexCount, 0);

    return *this;

}

void Mesh::Bind() const
{
    glBindVertexArray(vao);
}

void Mesh::Unbind()
{
    glBindVertexArray(0);
}

GLsizei Mesh::GetIndexCount() const noexcept
{
    return indexCount;
}

GLuint Mesh::GetVertexArray() const noexcept
{
    return vao;
}

