#include "gtest/gtest.h"
#include <iostream>
#include <fstream>

//---------------------------------------------------------------------------//
class ReadTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        ReadTest(){} 


        //-------------------------------------------------------------------//
        virtual void SetUp() {
            auto out_1 = new std::ofstream(bytefile, std::ofstream::binary);
            for (int i=0; i<1000; i++) *out_1 << char(0x07);
            out_1->close();
        }
        

        //-------------------------------------------------------------------//
        virtual void TearDown() {
            std::remove(bytefile);
        }
        
        char const * bytefile = ".TESTFILE_1000_0x7.dat";
        char c = 0;
        char dummy_char = 0x07;
        int count = 0;

};
