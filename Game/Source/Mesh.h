#pragma once

#include "glew.h"
#include "Vertex.h"

#include <cstdint>
#include <vector>

class Mesh final
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);

    Mesh(const float* points, std::size_t pointCount, const std::uint16_t* indices, std::size_t triangleCount);

    ~Mesh();

    // Un recurso OpenGL no debe copiar sus handles.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Sí puede transferir su propiedad.
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Bind() const;
    static void Unbind();

    GLsizei GetIndexCount() const noexcept;
    GLuint GetVertexArray() const noexcept;

private:
    void Upload(
        const std::vector<Vertex>& vertices,
        const std::vector<std::uint32_t>& indices
    );

    void Release() noexcept;

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    GLsizei indexCount = 0;
};