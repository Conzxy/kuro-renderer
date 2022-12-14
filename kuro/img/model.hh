#ifndef KURO_IMG_MODEL_H__
#define KURO_IMG_MODEL_H__

#include <vector>
#include <string>
#include <limits>

#include "kuro/math/vec.hh"
#include "kuro/math/matrix.hh"

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

  using Vertex = Vec3f;
  using Face = std::vector<Mesh>;
  using Texture = Vec3f;
  using Normal = Vec3f;
  using Vectexes = std::vector<Vec3f>;
  using Faces = std::vector<std::vector<Mesh>>;
  using Textures = std::vector<Vec3f>;
  using Normals = std::vector<Vec3f>;

  Model();
  explicit Model(char const *path);
  ~Model() noexcept;
  
  bool ParseFrom(char const *path);
  void Clear();

  Vectexes &vertexes() noexcept { return vertexes_; }
  Faces &faces() noexcept { return faces_; }
  Textures &textures() noexcept { return textures_; }
  Normals &normals() noexcept { return normals_; }
  
  size_t GetVertexesNum() const noexcept { return vertexes_.size(); }
  size_t GetFacesNum() const noexcept { return faces_.size(); }
  size_t GetTexturesNum() const noexcept { return textures_.size(); }
  size_t GetNormalsNum() const noexcept { return normals_.size(); }

  Vertex &GetVertex(size_t i) noexcept { return vertexes_[i]; }
  Vertex const &GetVertex(size_t i) const noexcept { return vertexes_[i]; }
  Face &GetFace(size_t i) noexcept { return faces_[i]; }
  Face const &GetFace(size_t i) const noexcept { return faces_[i]; }
  Texture &GetTexture(size_t i) noexcept { return textures_[i]; }
  Texture const &GetTexture(size_t i) const noexcept { return textures_[i]; }
  Normal &GetNormal(size_t i) noexcept { return normals_[i]; }
  Normal const &GetNormal(size_t i) const noexcept { return normals_[i]; }
  
  Matrix4x4f GetModelMatrix() const noexcept;

  void ResetBoundingCoordinate() noexcept;
 private:
  bool ParseMesh(std::string const &mesh_slice, Face &face);
  bool ParseTexture(std::string const &line);
  bool ParseNormal(std::string const &line);
  bool ParseVertex(std::string const &line);
  bool ParseFace(std::string const &line);

  Vectexes vertexes_;
  Faces faces_;
  Textures textures_;
  Normals normals_;
  
  Vec3f max_bounding_coor_;
  Vec3f min_bounding_coor_;

  mutable Matrix4x4f model_matrix_;
  mutable bool has_model_matrix_cache_ = false;
};

} // namespace kuro

#endif
