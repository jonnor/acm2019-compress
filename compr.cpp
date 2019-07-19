// Necessary for bfd.h to include properly
#define PACKAGE "acmss-custom-compression"
#define PACKAGE_VERSION "1.0"

#include <inttypes.h>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include <fstream>
#include <cassert>

namespace bfd {
#include <bfd.h>
}

#include "types.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

// TODO: Implement a compression method to evaluate
// Return the compressed size of the cacheline
size_t compress_line(cacheline* line) {

  static int line_no = 0;

  if (line_no == 0) {
    printf("line%d = [", line_no);
    for (int i=0; i<64; i++) {
      printf("0x%02x,", line->byte[i]);
    }
    printf("]\n");
  }
  line_no += 1;

  //cerr << line->float32[4].b.exp << endl;
  return 64;
}

// Return the compressed size of the dataset, rounded up to the nearest byte
// The input size is guaranteed to be an even multiple of 64 bytes
size_t compress(uint8_t* bytes, size_t size) {

  // Make it easier to iterate over the data cacheline by cacheline
  // Take a look at types.h for more info.
  cacheline* cachelines = (cacheline*) bytes;
  size_t n_cachelines = size / 64;

  size_t compressed_size = 0;

  for (size_t n=0; n<n_cachelines; ++n) {
    cacheline* line = &cachelines[n];

    compressed_size += compress_line(line);
  }

  return compressed_size;
}


// Allocate and return a contiguous blob of all the bytes
// in the application's allocated memory space.
// Returns false on failure, true on success.
bool extract_data(const string& filename, uint8_t** data, size_t& data_size) {

	bfd::bfd *abfd = bfd::bfd_openr(filename.c_str(), NULL);
  if (abfd == NULL) {
    cerr << "Failed to open memory dump" << endl;
    return false;
  }

	if (!bfd::bfd_check_format (abfd, bfd::bfd_core)) {
		cerr << "File is not a recognized core dump" << endl;
		return false;
  }

  uint8_t* buf = NULL;
  size_t buf_size = 0;

  bfd::asection *p;
  for (p = abfd->sections; p != NULL; p = p->next) {
    // Skip some sections that are of no interest
    if (strstr(p->name, ".reg") != NULL) continue;
    //if (strstr(p->name, ".auxv") != NULL) continue;
    if (strstr(p->name, "note") != NULL) continue;
    if (!p->flags & SEC_HAS_CONTENTS) continue;


    cerr << "Section " << p->name << "  " << p->size << " bytes" << endl;
    size_t new_size = buf_size + p->size;

    void* new_buf = realloc(buf, new_size);
    if (new_buf == NULL) {
      cerr << "Memory allocation error. Core dump too large?" << endl;
      free(buf);
      return false;
    }

    buf = (uint8_t*) new_buf;

    bfd::bfd_get_section_contents(abfd, p, buf + buf_size, 0, p->size);


    buf_size = new_size;


  }

  cout << "Total size: " << buf_size << " bytes" << endl;

  *data = buf;
  data_size = buf_size;
  return true;
}

std::string base_name(std::string const & path)
{
  return path.substr(path.find_last_of("/\\") + 1);
}

int main(int argc, char** argv) {

  if (argc < 2) {
    cerr << argv[0] << " <MEMORY-DUMP>" << endl;
    return 1;
  }

  bfd::bfd_init();

  string dumpfile(argv[1]);

  uint8_t* bytes;
  size_t size;

  if (!extract_data(dumpfile, &bytes, size)) {
		cerr << "Unable to load dump." << endl;
    return 2;
  }


  // Truncate to 64 B (cache line size)
  size_t truncated_size = size & ~(64ULL - 1);


  // Dump raw memory to a file
  std::ofstream file;
  file.open((base_name(dumpfile)+".dat").c_str(), std::ios_base::binary);
  assert(file.is_open());
  file.write((const char *)bytes, truncated_size);

  // See what kind of compression we can expect!
  size_t compressed_size = compress(bytes, truncated_size);

  float compression_ratio = (float) truncated_size / (float) compressed_size;

  cout << "Compression ratio: " << std::setprecision(3) << compression_ratio << endl;

}
