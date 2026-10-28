#include "Mesh.h"
#include "Log.h"

//Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
//{
//    this->vertices = vertices;
//    this->indices = indices;
//    this->textures = textures;
//    this->renderer = nullptr;
//    SetupMesh();
//}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    //this->renderer = nullptr;
    SetupMesh();
}

Mesh::Mesh(float* points, int nPoints, uint16_t* indices, int nIndices)
{

    for (size_t i = 0; i < nPoints * 3; i += 3)
    {
        DEBUG_LOG("point (%f,%f,%f)", points[i], points[i+1], points[i+2]);
    }



    for (int i = 0; i < nPoints * 3; i += 3)
    {
        vec3 v = vec3(points[i], points[i+1], points[i+2]);

        Vertex vertex = Vertex(v);

        this->vertices.push_back(vertex);
    }


    for (int i = 0; i < nIndices * 3; i++)
    {
        int index = indices[i];
        this->indices.push_back(index);
    }

    for (Vertex i : this->vertices)
    {
        DEBUG_LOG("vertex: (%f,%f,%f)", i.Position.x, i.Position.y, i.Position.z);
    }

    for (int i : this->indices)
    {
        DEBUG_LOG("index: %i", i);
    }


    SetupMesh();
}

void Mesh::SetupMesh()
{
    //set mesh
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

    //set bbox
    //bbox.SetNegativeInfinity();
    //float3 *bboxVertices = new float3[sizeof(float3) * vertices.size()];

    //for (size_t i = 0; i < vertices.size(); i++)
    //{
    //    float3 vec;
    //    vec.x = vertices[i].Position.x;
    //    vec.y = vertices[i].Position.y;
    //    vec.z = vertices[i].Position.z;


    //    bboxVertices[i] = vec;
    //}


    //bbox.Enclose(bboxVertices, vertices.size());
    //
    ////bbox.GetCornerPoints(bboxOriginalPoints);
    //bbox.GetCornerPoints(bboxPoints);


    //delete[] bboxVertices;
}

void Mesh::Draw(Shader& shader, bool wireframeActive)
{
    

    //bboxTransformed = bbox;

    //vec3 translation = renderer->GetGameObject()->transform->translationWorld.translation();
    ////vec3 scaling = renderer->GetGameObject()->transform->scalingWorld.scaling();

    ////translate bbox points
    //bboxTransformed.Translate(vec(translation.x, translation.y, translation.z));


    //vec bboxTransformedPoints[8];
    //bboxTransformed.GetCornerPoints(bboxTransformedPoints);

    ////scale box points
    //for (size_t i = 0; i < 8; i++)
    //{
    //    bboxTransformedPoints[i].x *= scaling.x;
    //    bboxTransformedPoints[i].y *= scaling.y;
    //    bboxTransformedPoints[i].z *= scaling.z;
    //
    //    bboxTransformed.Enclose(bboxTransformedPoints, 8);
    //    //bboxPoints[i] = bboxTransformedPoints[i];
    //}




    //Draw textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    if (!textures.empty())
    {
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);


            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

            //shader.SetInt(("material." + name + number).c_str(), i);
            glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }

   /* for (size_t i = 0; i < 8; i++)
    {
        bboxPoints[i].x = bboxOriginalPoints[i].x + renderer->GetGameObject()->transform->translationLocal.translation().x;
        bboxPoints[i].y = bboxOriginalPoints[i].y + renderer->GetGameObject()->transform->translationLocal.translation().y;
        bboxPoints[i].x = bboxOriginalPoints[i].x + renderer->GetGameObject()->transform->translationLocal.translation().z;
    }*/
    

    glPolygonMode(GL_FRONT, GL_LINE);

    //if (drawBbox)
    //{
    //    DrawCube(bboxPoints, Color(1, 1, 1, 1));
    //}

    if (!wireframeActive)
    {
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    
    //deactivate Textures
    if (!textures.empty())
    {
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
        }
    }

}

//void Mesh::DrawCube(static float3* corners, Color color)
//{
//    glLineWidth(2.0f);
//    glColor4f(color.r, color.g, color.b, color.a);
//
//    glBegin(GL_QUADS);
//
//    glVertex3fv((GLfloat*)&corners[1]);
//    glVertex3fv((GLfloat*)&corners[5]);
//    glVertex3fv((GLfloat*)&corners[7]);
//    glVertex3fv((GLfloat*)&corners[3]);
//
//    glVertex3fv((GLfloat*)&corners[4]);
//    glVertex3fv((GLfloat*)&corners[0]);
//    glVertex3fv((GLfloat*)&corners[2]);
//    glVertex3fv((GLfloat*)&corners[6]);
//
//    glVertex3fv((GLfloat*)&corners[5]);
//    glVertex3fv((GLfloat*)&corners[4]);
//    glVertex3fv((GLfloat*)&corners[6]);
//    glVertex3fv((GLfloat*)&corners[7]);
//
//    glVertex3fv((GLfloat*)&corners[0]);
//    glVertex3fv((GLfloat*)&corners[1]);
//    glVertex3fv((GLfloat*)&corners[3]);
//    glVertex3fv((GLfloat*)&corners[2]);
//
//    glVertex3fv((GLfloat*)&corners[3]);
//    glVertex3fv((GLfloat*)&corners[7]);
//    glVertex3fv((GLfloat*)&corners[6]);
//    glVertex3fv((GLfloat*)&corners[2]);
//
//    glVertex3fv((GLfloat*)&corners[0]);
//    glVertex3fv((GLfloat*)&corners[4]);
//    glVertex3fv((GLfloat*)&corners[5]);
//    glVertex3fv((GLfloat*)&corners[1]);
//
//    glEnd();
//
//}