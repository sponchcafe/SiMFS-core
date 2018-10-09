#include "io/buffer.hpp"
#include "env/env.hpp"

namespace sim{
    namespace io{
        size_t CHUNK_SIZE_BYTES     = get_env("SIMFS_CHUNK_SIZE", 1<<14); // 16kB chunks
        unsigned int LOW_WATERMARK  = get_env("SIMFS_LOW_MARK", 1<<6);
        unsigned int HIGH_WATERMARK = get_env("SIMFS_HIGH_MARK", 1<<10);
        unsigned int BASE_DELAY_NS  = get_env("SIMFS_BASE_DELAY_NS", 1);
        unsigned int TOP_DELAY_NS   = get_env("SIMFS_TOP_DELAY_NS", 1<<20);
    }
  
}
