#include "io/buffer.hpp"
#include "env/env.hpp"

namespace sim{
    namespace io{

        bool SIMFS_BUFFER_CONTROL = get_env("SIMFS_BUFFER_CONTROL", 0);
        unsigned long int SIMFS_DELAY_NS = get_env_nanos("SIMFS_DELAY", 100);
        unsigned long int SIMFS_CHUNK_SIZE_BYTES = get_env_bytes("SIMFS_CHUNK_SIZE", 1<<14);   // 16kB
        unsigned long int SIMFS_BUFFER_SIZE_BYTES = get_env_bytes("SIMFS_BUFFER_SIZE", 1<<20); //  1MB

    }
  
}
