﻿#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>




void Mesh::setupMesh()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}


void Mesh::init(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    _vertices = vertices;
    _indices = indices;
    _textures = textures;
    setupMesh();
}

void Mesh::init(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    _vertices = vertices;
    _indices = indices;
    setupMesh();
}

void Mesh::draw(Shader& shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = _textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
		shader.setInt(name + number, i);
        //std::cout << "texture name: " << (name+number) << std::endl;
        
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, _textures[i].id);
    }



    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}


void Mesh::terminate(){
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}




OMesh MeshFactory::createCube()
{
    OMesh mesh;
    OMesh::VertexHandle vhandle[8];
    std::vector<OMesh::VertexHandle> face_vhandles;

    vhandle[0] = mesh.add_vertex(OMesh::Point(-1, -1, 1));
    vhandle[1] = mesh.add_vertex(OMesh::Point(1, -1, 1));
    vhandle[2] = mesh.add_vertex(OMesh::Point(1, 1, 1));
    vhandle[3] = mesh.add_vertex(OMesh::Point(-1, 1, 1));
    vhandle[4] = mesh.add_vertex(OMesh::Point(-1, -1, -1));
    vhandle[5] = mesh.add_vertex(OMesh::Point(1, -1, -1));
    vhandle[6] = mesh.add_vertex(OMesh::Point(1, 1, -1));
    vhandle[7] = mesh.add_vertex(OMesh::Point(-1, 1, -1));

    // Quad yüzler (6 yüz)
    std::vector<std::array<int, 4>> quads = {
        {0,1,2,3}, // front
        {7,6,5,4}, // back
        {1,0,4,5}, // bottom
        {2,1,5,6}, // right
        {3,2,6,7}, // top
        {0,3,7,4}  // left
    };

    for (auto& q : quads) {
        face_vhandles.clear();
        for (int i = 0; i < 4; ++i)
            face_vhandles.push_back(vhandle[q[i]]);
        mesh.add_face(face_vhandles);
    }
    return mesh;
}
OMesh MeshFactory::createCone()
{
    OMesh mesh;
    std::vector <OMesh::VertexHandle> vhandle;

    // parametrelerimiz 
    int resolution = 32; // number of edges 
    float radius = 1; 
    float height = 2; 


    // vertexleri oluşturuyoruz 
    std::vector<Vertex> base_vertices;
    for (int i{}; i < resolution; i++) {
        float ratio = static_cast<float>(i) / (resolution);
        float angle = ratio * (M_PI * 2.0);
        float x = std::cos(angle) * radius;
        float z = std::sin(angle) * radius;
        vhandle.push_back(mesh.add_vertex(OMesh::Point(x, 0, z)));
    }
    int centerIndex = vhandle.size();
    vhandle.push_back(mesh.add_vertex(OMesh::Point(0, 0, 0))); // center of cone floor
    int tipIndex = vhandle.size();
    vhandle.push_back(mesh.add_vertex(OMesh::Point(0, height, 0))); // tip of cone 


    // Yüzeylerin vertex indexlerini belirliyoruz:
    std::vector<std::vector<int>> faces;    

    for (int i{}; i < resolution-1; i++) {
        faces.push_back({i, i + 1, centerIndex }); // flor
        faces.push_back({i + 1, i, tipIndex }); // sidewall
    }
    // Son trisleri ekliyoruz:
    faces.push_back({resolution - 1, 0, centerIndex });
    faces.push_back({0, resolution - 1, tipIndex });


    // Belirlediğimiz indexlerdeki vertexlerden face oluşturup mesh e ekliyoruz:
    std::vector<OMesh::VertexHandle> face_vhandles;
    for (std::vector<int>& face : faces) {
        face_vhandles.clear();
        for (int& vertexIndex : face)
            face_vhandles.push_back(vhandle[vertexIndex]);
        mesh.add_face(face_vhandles);
    }
    return mesh;
}

OMesh MeshFactory::createCylinder()
{
    return OMesh();
}

OMesh MeshFactory::createPlane()
{
    return OMesh();
}

Mesh MeshFactory::create(DefaultShapes shape)
{
    OMesh mesh;

    // 🔹 Geometrik şekil seçimi
    switch (shape)
    {
    case DefaultShapes::Cube:
        mesh = createCube();
        break;
    case DefaultShapes::Cone:
        mesh = createCone();
        break;

    case DefaultShapes::Cylinder:
        mesh = createCylinder();
        break;

    default:
        break;
    }





    // 🔹 Normalleri hesapla
    mesh.request_face_normals();
    mesh.update_face_normals();
    //mesh.triangulate(); // OpenGL için üçgenlere ayır




    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;


    // 🔹 Flat shading: her yüzey için ayrı vertex oluştur
    for (auto f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        OpenMesh::Vec3f faceNormal = mesh.normal(*f_it);
        std::vector<unsigned int> local_indices;

        for (auto fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
            auto p = mesh.point(*fv_it);

            unsigned int index = static_cast<unsigned int>(vertices.size());
            vertices.push_back(Vertex{
                glm::vec3(p[0], p[1], p[2]),
                glm::vec3(faceNormal[0], faceNormal[1], faceNormal[2]),
                glm::vec2(0.0f, 0.0f)
                });

            local_indices.push_back(index);
        }

        // yüz üçgen mi?
        if (local_indices.size() == 3) {
            indices.push_back(local_indices[0]);
            indices.push_back(local_indices[1]);
            indices.push_back(local_indices[2]);
        }
        // quad ise triangulate et
        else if (local_indices.size() == 4) {
            indices.push_back(local_indices[0]);
            indices.push_back(local_indices[1]);
            indices.push_back(local_indices[2]);
            indices.push_back(local_indices[0]);
            indices.push_back(local_indices[2]);
            indices.push_back(local_indices[3]);
        }
    }

    Mesh m; 
    m.init(vertices, indices);
    return m;
}
