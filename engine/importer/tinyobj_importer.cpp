#include "tinyobj_importer.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <stdexcept>
#include <unordered_map>

#include "../mesh/mesh.h"

TinyObjectImporter::TinyObjectImporter()
{

}

std::shared_ptr<Mesh> TinyObjectImporter::LoadModel() {
    std::shared_ptr<Mesh>  mesh = std::make_shared<Mesh>();
    mesh->texturePath = TEXTURE_PATH;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str())) {
        throw std::runtime_error(err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(mesh->vertices.size());
                mesh->vertices.push_back(vertex);
            }

            mesh->indices.push_back(uniqueVertices[vertex]);
        }
    }

    return mesh;
}