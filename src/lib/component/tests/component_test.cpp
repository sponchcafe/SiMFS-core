#include "component_test_fixtures.hpp"

#include "io/file_io.hpp"
#include "io/vector_io.hpp"
#include "io/queue_io.hpp"

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

