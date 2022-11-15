#include "model.hh"

#include <exception>

#include "kuro/math/vec.hh"
#include "kuro/util/file.hh"
#include "kuro/util/log.hh"

using namespace kuro;

Model::Model()
{
  ResetBoundingCoordinate();
}

Model::Model(char const *path) 
  : Model()
{
  if (!ParseFrom(path)) {
    // TODO Specific exception
    throw std::runtime_error{"Failed to parse *.obj file"};
  }
}

Model::~Model() noexcept = default;

bool Model::ParseFrom(char const *path)
{
  File file;
  if (!file.Open(path, File::READ)) return false;
  std::string line;

  for (;;) {
    auto ret = file.ReadLine(line, false);
    if (ret == File::E_ERROR) return false;
    if (ret == File::E_EOF) break;

    // Skip empty line
    if (line.empty()) continue;
    
    // Because of SSO, the substr() might don't be a hot spot
    switch (line[0]) {
      case 'v':
      {
        if (line[1] != ' ') {
          switch (line[1]) {
            case 't':
            {
              if (!ParseTexture(line)) return false;
            } break;

            case 'n':
            {
              if (!ParseNormal(line)) return false;
            } break;

            default:
              break;
          }

        break;
        }

        ParseVertex(line);

      } break;

      case 'f':
      {
        if (!ParseFace(line)) return false;
      } break;

    }
  }

  return true;
}

bool Model::ParseMesh(std::string const &mesh_slice, Face &face)
{
  std::array<int, 3> mesh{ -1, -1, -1 };
  std::string::size_type slash_pos = std::string::npos;
  std::string::size_type cur_pos = 0;
  std::string numeric;
  char *end = nullptr;

  for (int i = 0; i < 3; ++i) {
    slash_pos = mesh_slice.find('/', cur_pos);
    
    // std::string::npos is also OK
    if (cur_pos != slash_pos) {
      numeric = mesh_slice.substr(cur_pos, slash_pos-cur_pos);
      mesh[i] = std::strtol(numeric.c_str(), &end, 10);
      if (mesh[i] == 0 && end == numeric.c_str()) {
        return false;
      }
      
      // Avoid cur_pos round to 0(npos + 1 == 0)
      if (slash_pos == std::string::npos) break;
      cur_pos = slash_pos + 1;
    } else {
      // No vertex index
      if (i == 0)
        return false; 
      else
        cur_pos++;
    }
  }
   
  mesh[0] = mesh[0] > 0 ? (mesh[0] - 1) : (vertexes_.size() + mesh[0]);
  mesh[1] = mesh[1] > 0 ? (mesh[1] - 1) : (textures_.size() + mesh[1]);
  mesh[2] = mesh[2] > 0 ? (mesh[2] - 1) : (textures_.size() + mesh[2]);
  face.push_back(Mesh{ mesh[0], mesh[1], mesh[2] });

  return true;
}

bool Model::ParseTexture(std::string const &line)
{
  // u, [v, w]
  // v and w are optional(default: 0.)
  Vec3f texture(NAN, 0., 0.);
  
  // Space count can greater than 1
  std::string::size_type cur_pos = line.find_first_not_of(' ', 2);
  if (cur_pos == std::string::npos) return false;
  
  std::string::size_type slash_pos = std::string::npos;
  char *end = nullptr;
  std::string numeric;

  for (int i = 0; i < 3; ++i) {
    slash_pos = line.find(' ', cur_pos);
    
    // std::string::npos is also OK
    if (cur_pos != slash_pos) {
      numeric = line.substr(cur_pos, slash_pos-cur_pos);
      texture[i] = std::strtof(numeric.c_str(), &end);
      if (texture[i] == 0 && end == numeric.c_str()) {
        return false;
      }

      if (slash_pos == std::string::npos) break;
      cur_pos = line.find_first_not_of(' ', slash_pos+1);
    } else if (i == 0) {
      // u is missing
      return false; 
    }
  }

  textures_.push_back(std::move(texture));
  return true;
}

bool Model::ParseNormal(std::string const &line)
{
  // (x, y, z)
  Vec3f normal(0, 0, 0);
      
  std::string::size_type cur_pos = line.find_first_not_of(' ', 2);
  if (cur_pos == std::string::npos) return false;
  
  std::string::size_type slash_pos = std::string::npos;
  char *end = nullptr;
  std::string numeric;

  for (int i = 0; i < 3; ++i) {
    slash_pos = line.find(' ', cur_pos);
    
    // std::string::npos is also OK
    if (cur_pos != slash_pos) {
      numeric = line.substr(cur_pos, slash_pos-cur_pos);
      normal[i] = std::strtof(numeric.c_str(), &end);
      if (normal[i] == 0 && end == numeric.c_str()) {
        return false;
      }
      cur_pos = line.find_first_not_of(' ', slash_pos+1);
    } else { 
      return false; 
    }
  }

  normals_.push_back(std::move(normal));
  return true;
}

bool Model::ParseVertex(std::string const &line)
{
  size_t cur_pos = line.find_first_not_of(' ', 1);
  if (cur_pos == std::string::npos) return false;

  std::array<float, 3> vertex;
  size_t slash_pos = std::string::npos;
  char *end = nullptr;
  std::string numeric;
  
  for (int i = 0; i < 3; ++i) {
    slash_pos = line.find(' ', cur_pos);
    if (slash_pos == std::string::npos && i != 2) {
      return false;
    }

    numeric = line.substr(cur_pos, slash_pos-cur_pos);
    
    vertex[i] = std::strtof(numeric.c_str(), &end);
  
    if (vertex[i] == 0.0 && end == numeric.c_str()) {
      return false;
    }

    cur_pos = line.find_first_not_of(' ', slash_pos+1);
  }
  
  for (int i = 0; i < 3; ++i) {
    max_bounding_coor_[i] = std::max(max_bounding_coor_[i], vertex[i]);
    min_bounding_coor_[i] = std::min(min_bounding_coor_[i], vertex[i]);
  }

  vertexes_.emplace_back(vertex);
  return true;
}

bool Model::ParseFace(std::string const &line)
{
  size_t cur_pos = line.find_first_not_of(' ', 1);
  if (cur_pos == std::string::npos) return false;

  std::string::size_type slash_pos = std::string::npos;
  std::string mesh_slice;
  std::string numeric;
  Face face;

  for (;;) {
    slash_pos = line.find(' ', cur_pos);
    mesh_slice = line.substr(cur_pos, slash_pos-cur_pos);
     
    if (!ParseMesh(mesh_slice, face)) {
      return false;
    }
    
    if (slash_pos == std::string::npos) break;
    cur_pos = line.find_first_not_of(' ', slash_pos+1);
  }
  faces_.push_back(std::move(face));
  return true;
}

void Model::Clear()
{
  vertexes_.clear();
  faces_.clear();
  textures_.clear();
  normals_.clear();
}

Matrix4x4f Model::GetModelMatrix() const noexcept
{
#if 1
  DebugPrintf("max bounding = (%f, %f, %f)\n", max_bounding_coor_[0], max_bounding_coor_[1], max_bounding_coor_[2]);
  DebugPrintf("min bounding = (%f, %f, %f)\n", min_bounding_coor_[0], min_bounding_coor_[1], min_bounding_coor_[2]);
#endif
  if (!has_model_matrix_cache_) {
    auto scale = 2 / (max_bounding_coor_ - min_bounding_coor_);
    auto tr = (max_bounding_coor_ + min_bounding_coor_) / -2;
    
    auto scale_uniform = std::max(scale.x(), std::max(scale.y(), scale.z()));

    model_matrix_.SetData({
      { scale_uniform, 0, 0, scale_uniform * tr.x() },
      { 0, scale_uniform, 0, scale_uniform * tr.y() },
      { 0, 0, scale_uniform, scale_uniform * tr.z() },
      { 0, 0, 0, 1 }
    });
    has_model_matrix_cache_ = true;
  }
  return model_matrix_;
}

void Model::ResetBoundingCoordinate() noexcept
{
  max_bounding_coor_.Fill(-std::numeric_limits<float>::max());
  min_bounding_coor_.Fill(std::numeric_limits<float>::max());
}
