#include "model.hh"

#include <exception>

#include "kuro/util/file.hh"

using namespace kuro;

Model::Model() = default;

Model::Model(char const *path) {
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
    if (!file.ReadLine(line, false)) return false;

    printf("line: %s\n", line.c_str());
    if (line.empty()) break;
    
    // Because of SSO, the substr() might don't be a hot spot
    switch (line[0]) {
      case 'v':
      {
        if (line[1] != ' ') {
          switch (line[1]) {
            case 't':
            {
              // u, [v, w]
              // v and w are optional(default: 0.)
              Vec3f texture(NAN, 0., 0.);

              std::string::size_type cur_pos = 2;
              if (line[cur_pos] != ' ') return false;
              cur_pos += 1;
              if (cur_pos == ' ') return false;
              
              std::string::size_type slash_pos = std::string::npos;
              char *end = nullptr;
              std::string numeric;

              for (int i = 0; i < 3; ++i) {
                slash_pos = line.find(' ', cur_pos);
                
                // std::string::npos is also OK
                if (cur_pos != slash_pos) {
                  numeric = line.substr(cur_pos, slash_pos-cur_pos);
                  printf("numeric = %s\n", numeric.c_str());
                  texture[i] = std::strtof(numeric.c_str(), &end);
                  if (texture[i] == 0 && end == numeric.c_str()) {
                    return false;
                  }

                  if (slash_pos == std::string::npos) break;
                  cur_pos = slash_pos + 1;
                } else if (i == 0) {
                  // u is missing
                  return false; 
                }
              }

              textures_.push_back(std::move(texture));
            } break;

            case 'n':
            {
              // (x, y, z)
              Vec3f normal(0, 0, 0);
                  
              std::string::size_type cur_pos = 2;
              if (line[cur_pos] != ' ') return false;
              cur_pos += 1;
              if (cur_pos == ' ') return false;
              
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
                } else { 
                  return false; 
                }
                cur_pos = slash_pos + 1;
              }

              normals_.push_back(std::move(normal));

            } break;

            default:
              break;
          }

        break;
        }

        size_t cur_pos = 1;
        if (line[cur_pos] != ' ') return false;
        cur_pos++;

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
          
          printf("numeric = %s\n", numeric.c_str());
          vertex[i] = std::strtof(numeric.c_str(), &end);
        
          printf("conv result=%f\n", vertex[i]);
          if (vertex[i] == 0.0 && end == numeric.c_str()) {
            return false;
          }
          cur_pos = slash_pos+1;
        }

        vertexes_.emplace_back(vertex);
      } break;

      case 'f':
      {
        std::vector<Mesh> face;
        size_t cur_pos = 1;
        if (line[cur_pos] != ' ') return false;
        cur_pos += 1;
        if (cur_pos == ' ') return false;

        std::string::size_type slash_pos = std::string::npos;
        std::string mesh_slice;
        std::string numeric;
        
        for (;;) {
          slash_pos = line.find(' ', cur_pos);

          std::array<int, 3> mesh{ -1, -1, -1 };
          mesh_slice = line.substr(cur_pos, slash_pos-cur_pos);
           
          if (!ParseMesh(mesh_slice, mesh)) {
            return false;
          }

          face.push_back(Mesh{ mesh[0], mesh[1], mesh[2] });
          
          if (slash_pos == std::string::npos) break;
          cur_pos = slash_pos + 1;
        }
        faces_.push_back(std::move(face));
      } break;

    }
  }

  return true;
}

bool Model::ParseMesh(std::string const &mesh_slice, std::array<int, 3> &mesh)
{
  std::string::size_type slash_pos = std::string::npos;
  std::string::size_type cur_pos = 0;
  std::string numeric;
  char *end = nullptr;

  for (int i = 0; i < 3; ++i) {
    slash_pos = mesh_slice.find('/', cur_pos);
    
    // std::string::npos is also OK
    if (cur_pos != slash_pos) {
      numeric = mesh_slice.substr(cur_pos, slash_pos-cur_pos);
      printf("numeric = %s\n", numeric.c_str());
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
  
  if (mesh[0] < 0) mesh[0] = vertexes_.size() - 1;
  if (mesh[1] < 0) mesh[1] = textures_.size() - 1;
  if (mesh[2] < 0) mesh[2] = normals_.size() - 1;

  return true;
}
