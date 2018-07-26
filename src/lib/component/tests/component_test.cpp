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

/*
//---------------------------------------------------------------------------//
TEST_F(ComponentIO, VectorTest){
    using namespace sim::vector_io;
    serial_PIC_test<VectorInput, VectorOutput>();
    validate_result();
}

//---------------------------------------------------------------------------//
TEST_F(ComponentIO, FileTest){
    using namespace sim::file_io;
    serial_PIC_test<FileInput, FileOutput>();
    validate_result();
}

//---------------------------------------------------------------------------//
TEST_F(ComponentIO, QueueTest){
    using namespace sim::queue_io;
    parallel_PIC_test<QueueInput, QueueOutput>();
    validate_result();
}
    
//---------------------------------------------------------------------------//
TEST_F(ComponentIO, PipeTest){
    using namespace sim::file_io;
    parallel_PIC_test<FileInput, FileOutput>();
    validate_result();
}

*/
