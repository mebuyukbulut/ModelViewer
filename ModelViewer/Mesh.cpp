#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>


#define _USE_MATH_DEFINES
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;


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

void Mesh::loadFromFile(const char* filename)
{
}

void Mesh::loadDefault()
{
    //// set up vertex data (and buffer(s)) and configure vertex attributes
    //// ------------------------------------------------------------------
    //_vertices = {
    //    Vertex{glm::vec3( 0.5f,  0.5f, 0.0f)},  // top right
    //    Vertex{glm::vec3( 0.5f, -0.5f, 0.0f)},  // bottom right
    //    Vertex{glm::vec3(-0.5f, -0.5f, 0.0f)},  // bottom left
    //    Vertex{glm::vec3(-0.5f,  0.5f, 0.0f)}   // top left 
    //};
    //_indices = {  // note that we start from 0!
    //    0, 1, 3,  // first Triangle
    //    1, 2, 3   // second Triangle
    //};




    MyMesh mesh;
    MyMesh::VertexHandle vhandle[8];
    std::vector<MyMesh::VertexHandle>  face_vhandles;

    switch (DefaultShapes::Cube)
    {
    case DefaultShapes::Cube:
        vhandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1, 1));
        vhandle[1] = mesh.add_vertex(MyMesh::Point(1, -1, 1));
        vhandle[2] = mesh.add_vertex(MyMesh::Point(1, 1, 1));
        vhandle[3] = mesh.add_vertex(MyMesh::Point(-1, 1, 1));
        vhandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
        vhandle[5] = mesh.add_vertex(MyMesh::Point(1, -1, -1));
        vhandle[6] = mesh.add_vertex(MyMesh::Point(1, 1, -1));
        vhandle[7] = mesh.add_vertex(MyMesh::Point(-1, 1, -1));

        // generate (quadrilateral) faces
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[3]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[4]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[4]);
        face_vhandles.push_back(vhandle[5]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[6]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[7]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[4]);
        mesh.add_face(face_vhandles);
        break;
    case DefaultShapes::Sphere:
        break;
    case DefaultShapes::Cylinder:
        break;
    default:
        break;
    }


    mesh.request_vertex_normals();
    mesh.request_face_normals();
    mesh.update_normals();

    mesh.triangulate();

    //std::vector<Vertex> vertices;
    //std::vector<unsigned int> indices;

    // 🔹 Vertex data 
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        auto p = mesh.point(*v_it);
        auto n = mesh.normal(*v_it);
        //auto t = mesh.texcoord2D(*v_it); // ??? 

        // position (x, y, z) - normal (x, y, z) - texture coodinates (s, t)
        _vertices.push_back(Vertex{ {p[0],p[1],p[2]}, {n[0],n[1],n[2]}, {0,0} });
    }

    for (auto f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        std::vector<unsigned int> face_indices;
        for (auto fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
            face_indices.push_back((*fv_it).idx());

        if (face_indices.size() == 3) {
            _indices.push_back(face_indices[0]);
            _indices.push_back(face_indices[1]);
            _indices.push_back(face_indices[2]);
        }
    }




    setupMesh();
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



Mesh MeshFactory::create(DefaultShapes shape)
{
    MyMesh mesh;
    MyMesh::VertexHandle vhandle[8];
    std::vector<MyMesh::VertexHandle>  face_vhandles;

    switch (shape)
    {
    case DefaultShapes::Cube:
        vhandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1, 1));
        vhandle[1] = mesh.add_vertex(MyMesh::Point(1, -1, 1));
        vhandle[2] = mesh.add_vertex(MyMesh::Point(1, 1, 1));
        vhandle[3] = mesh.add_vertex(MyMesh::Point(-1, 1, 1));
        vhandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
        vhandle[5] = mesh.add_vertex(MyMesh::Point(1, -1, -1));
        vhandle[6] = mesh.add_vertex(MyMesh::Point(1, 1, -1));
        vhandle[7] = mesh.add_vertex(MyMesh::Point(-1, 1, -1));

        // generate (quadrilateral) faces
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[3]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[4]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[4]);
        face_vhandles.push_back(vhandle[5]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[6]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[7]);
        mesh.add_face(face_vhandles);

        face_vhandles.clear();
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[4]);
        mesh.add_face(face_vhandles);
        break;
    case DefaultShapes::Sphere:
        break;
    case DefaultShapes::Cylinder:
        break;
    default:
        break;
    }


    mesh.request_vertex_normals();
    mesh.request_face_normals();
    mesh.update_normals();

    mesh.triangulate();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // 🔹 Vertex data 
    for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        auto p = mesh.point(*v_it);
        auto n = mesh.normal(*v_it);
        //auto t = mesh.texcoord2D(*v_it); // ??? 

        // position (x, y, z) - normal (x, y, z) - texture coodinates (s, t)
        vertices.push_back(Vertex{ {p[0],p[1],p[2]}, {n[0],n[1],n[2]}, {0,0} });
    }

    for (auto f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        std::vector<unsigned int> face_indices;
        for (auto fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
            face_indices.push_back((*fv_it).idx());

        if (face_indices.size() == 3) {
            indices.push_back(face_indices[0]);
            indices.push_back(face_indices[1]);
            indices.push_back(face_indices[2]);
        }
    }


    Mesh m;
    m.init(vertices, indices);
    return m; 
}
