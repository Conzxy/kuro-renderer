#include "kuro/img/model.hh"

using namespace kuro;

int main(int argc, char **argv)
{
  Model model;
  
  if (argc < 2) {
    printf("Usage: %s *.obj", argv[0]);
    return 0;
  }

  model.ParseFrom(argv[1]);
  
  printf("Vertexes: \n");
  for (auto const& vertex : model.vertexes()) {
    printf("(%f, %f, %f)\n", vertex.x(), vertex.y(), vertex.z());
  }

  printf("Textures: \n");
  for (auto const &t : model.textures()) {
    printf("(%f, %f, %f)\n", t.x(), t.y(), t.z());
  }

  printf("Normals: \n");
  for (auto const &normal : model.normals()) {
    printf("(%f, %f, %f)\n", normal.x(), normal.y(), normal.z());
  }

  printf("Faces: \n");
  int face_num = 0;
  for (auto const &face : model.faces()) {
    printf("face[%d]: \n", face_num);
    face_num++;
    for (auto const &mesh : face) {
      printf("%d/%d/%d ", mesh.vertex_idx, mesh.uv_idx, mesh.normal_idx);
    }
    puts("");
  }
}
