#ifndef KURO_UTIL_FILE_H__ 
#define KURO_UTIL_FILE_H__ 

#include <stdio.h>
#include <string>
#include <stddef.h>
#include <stdexcept>

#include "kuro/util/noncopyable.hh"

namespace kuro {

struct FileException : std::runtime_error {
  explicit FileException(char const* msg)
    : std::runtime_error(msg)
  {
  }

  explicit FileException(std::string const& msg) 
    : std::runtime_error(msg)
  {
  }
};

class File : kanon::noncopyable {
 public:
  enum Errno {
    E_EOF = 1,
    E_ERROR,
  };

  enum OpenMode : uint8_t {
    READ = 0x1,
    WRITE = 0x2,
    TRUNC = 0x4,
    APP = 0x8,
  };

  File()
    : fp_(NULL)
  { }

  File(char const* filename, int mode);
  File(std::string const& filename, int mode);

  bool Open(std::string const& filename, int mode)
  {
    return Open(filename.data(), mode);
  }

  bool Open(char const* filename, int mode);

  ~File() noexcept;

  size_t Read(void* buf, size_t len);

  int ReadLine(std::string& line, const bool need_newline = true);

  void Write(void const* buf, size_t len) noexcept { ::fwrite(buf, 1, len, fp_); }

  void Flush() noexcept { ::fflush(fp_); }
  bool IsValid() const noexcept { return fp_ != NULL; }

  void Rewind() noexcept { ::rewind(fp_); }

  void SeekCurrent(long offset) noexcept { Seek(offset, SEEK_CUR); }
  void SeekBegin(long offset) noexcept { Seek(offset, SEEK_SET); }
  void SeekEnd(long offset) noexcept { Seek(offset, SEEK_END); }
  long GetCurrentPosition() noexcept { return ::ftell(fp_); }

  static const size_t kInvalidReturn = static_cast<size_t>(-1);
 private:
  void Seek(long offset, int whence) noexcept { ::fseek(fp_, offset, whence); }

  FILE* fp_;
};

} // kuro 

#endif 
