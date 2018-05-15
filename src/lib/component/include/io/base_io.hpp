#ifndef SIM_BASE_IO_H
#define SIM_BASE_IO_H

#include <string>

//---------------------------------------------------------------------------//
// Abstract base definition of the IO interface.
//---------------------------------------------------------------------------//
// - Inputs and outputs are only move constructible and assignable to avoid
//   inconsistencies in the ownership of the handled ressources (e.g. file
//   streams, vectors or queues)
// - Input and output are templates with template argument T for the cargo
//   data type.
// - Input implements get, peek and the comparison operator
// - Output implements put
// - Each Input / Output has an ID that can be used to uniquely identify or
//   configure the adaptor.
//---------------------------------------------------------------------------//

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        template <typename T> class Input{

            public:

                //-Ctor------------------------------------------------------//
                Input<T>() {};
                
                //-Ctor------------------------------------------------------//
                Input<T>(std::string const id) : id(id) {}

                //-Dtor------------------------------------------------------//
                virtual ~Input<T>() {};

                //-get-------------------------------------------------------//
                // Get the next value into target and progress the stream.
                // Should return true as long the stream has data.
                //-----------------------------------------------------------//
                virtual bool get(T &target) = 0;

                //-Copy-the-next-value-without-taking-it---------------------//
                virtual T peek() const = 0;

                //-Compare-input-order-by-current-value----------------------//
                bool operator< (Input<T> const &rhs) const {
                    return peek() < rhs.peek();
                }

                //-getID-----------------------------------------------------//
                std::string const get_id() const{
                    return id;
                }

                //-ID--------------------------------------------------------//
                std::string id;

        };

        //-------------------------------------------------------------------//
        // Input factory.
        //-------------------------------------------------------------------//
        template <template <typename> class InputT, typename T>
        std::unique_ptr<Input<T>> create_input(std::string id) {
            static_assert(
                std::is_base_of<Input<T>, InputT<T>>::value,
                "OutputT has to be derived from the baseclass Output."
            );
            return std::make_unique<InputT<T>>(id);
        }
        //-------------------------------------------------------------------//
        


        //-------------------------------------------------------------------//
        template <typename T> class Output{

            public:

                //-Ctor------------------------------------------------------//
                Output<T>() {}

                //-Ctor------------------------------------------------------//
                Output<T>(std::string const id) : id(id) {}

                //-Dtor------------------------------------------------------//
                virtual ~Output<T>() {} 

                //-Put-------------------------------------------------------//
                // Put item into the stream.
                //-----------------------------------------------------------//
                virtual void put(T &item) = 0;

                //-getID-----------------------------------------------------//
                std::string const get_id() const{
                    return id;
                }

                //-ID--------------------------------------------------------//
                std::string id;

        };

        //-------------------------------------------------------------------//
        // Output factory.
        //-------------------------------------------------------------------//
        template <template <typename> class OutputT, typename T>
        std::unique_ptr<Output<T>> create_output(std::string id) {
            static_assert(
                std::is_base_of<Output<T>, OutputT<T>>::value,
                "OutputT has to be derived from the baseclass Output."
            );
            return std::make_unique<OutputT<T>>(id);
        }
        //-------------------------------------------------------------------//
    
    }
}

#endif
