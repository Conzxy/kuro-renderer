#ifndef KURO_IMG_MODEL_H__
#define KURO_IMG_MODEL_H__

#include <vector>
#include <string>

#include "kuro/math/vec.hh"

namespace kuro {

/**
 * \see https://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
class Model {
 public:
  struct Mesh {
    int vertex_idx = -1;
    int uv_idx = -1;
    int normal_idx = -1;
  };

  using Vectexes = std::vector<Vec3f>;
  using Faces = std::vector<std::vector<Mesh>>;
  using Textures = std::vector<Vec3f>;
  using Normals = std::vector<Vec3f>;

  Model();
  explicit Model(char const *path);
  ~Model() noexcept;
  
  bool ParseFrom(char const *path);
  
  Vectexes &vertexes() noexcept { return vertexes_; }
  Faces &faces() noexcept { return faces_; }
  Textures &textures() noexcept { return textures_; }
  Normals &normals() noexcept { return normals_; }

 private:
  bool ParseMesh(std::string const &mesh_slice, std::array<int, 3> &mesh);

  Vectexes vertexes_;
  Faces faces_;
  Textures textures_;
  Normals normals_;
};

} // namespace kuro

#endif
