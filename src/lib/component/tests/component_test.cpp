#include "component_test_fixtures.hpp"
#include "io/buffer.hpp"

TEST_F(ComponentIO, PipelineTest){
   pure_in_memory_test();
   validate_result();
}

TEST_F(ComponentIO, NamedPipeTest){
   with_named_pipe_test();
   validate_result();
}

TEST_F(ComponentIO, FileTest){
   with_file_test();
   validate_result();
}
