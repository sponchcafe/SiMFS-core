#ifndef SIMFS_VECTOR_IO_H
#define SIMFS_VECTOR_IO_H

#include "io/base_io.hpp"
#include "io/vector_fs.hpp"


namespace sim{
    namespace vector_io{

        //-----------------------------------------------------------------------//
        template <typename T> class VectorInput : public Input<T>{

            public:

                //---------------------------------------------------------------//
                VectorInput<T>() {}

                //---------------------------------------------------------------//
                VectorInput<T>(std::string id) : 
                    Input<T>(id),
                    data(open<T>(id)) {
                        current = data.begin();
                    }

                //-Copy-ctor:-DELETED--------------------------------------------//
                VectorInput<T>(VectorInput<T> &source) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                VectorInput<T> &operator=(VectorInput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                VectorInput<T>(VectorInput<T> &&other) = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                VectorInput<T> &operator=(VectorInput<T> &&other) = default;

                //---------------------------------------------------------------//
                bool get(T &target) override {
                    if(current == data.end()) return false;
                    target = *current;
                    ++current;
                    return true;
                }

                //---------------------------------------------------------------//
                T peek() const override {
                    return *current;
                }

            private:

                //---------------------------------------------------------------//
                std::vector<T> &data;
                typename std::vector<T>::iterator current;

        };

        //-----------------------------------------------------------------------//
        template <typename T> class VectorOutput : public Output<T>{

            public:

                //---------------------------------------------------------------//
                VectorOutput<T>() {}

                //---------------------------------------------------------------//
                VectorOutput<T>(std::string id) :
                    Output<T>(id),
                    data(open<T>(id)) {
                    }

                //-Copy-ctor:-DELETED--------------------------------------------//
                VectorOutput<T>(VectorOutput<T> &source) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                VectorOutput<T> &operator=(VectorOutput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                VectorOutput<T>(VectorOutput<T> &&other) = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                VectorOutput<T> &operator=(VectorOutput<T> &&other) = default;

                //---------------------------------------------------------------//
                void put(T &item) override {
                    data.push_back(item);
                }

            private:

                //---------------------------------------------------------------//
                std::vector<T> &data;

        };

    }
}

#endif
