# ByteRanger

This smart object after create converts data from bytes to variables checked by template deductions and fill it with proper destination size of variable.

Written by Piotr K. (dajmosster@gmail.com)

piotrq.eu

Required C++ version: 17

GCC version: gcc (GCC) 10.3.1 20210422

Compiled and tested on: Linux Fedora 32

#

## Example program
>   
     std::vector<uint8_t> bytesVector = {
                                        255, // will be save in uint8_t var_one_byte
                                        255, 255, // will be save in uint16_t var_two_bytes
                                        0, 0, 0, 1, // will be save in uint32_t var_four_bytes 
                                        255, 255, 255, 255 // will be save in uint32_t var_four_bytes2
                                        };

    uint8_t var_one_byte = 0;
    uint16_t var_two_bytes = 0;
    uint32_t var_four_bytes = 0;
    uint32_t var_four_bytes2 = 0;

    ByteRanger converted_data{bytesVector, var_one_byte, var_two_bytes, var_four_bytes, var_four_bytes2};

    std::cout << "\n";
    std::cout << "Bytes data:\n";
    for (const auto& byte : bytesVector) {
        std::cout << static_cast<int>(byte) << ", ";
    }
    std::cout << "\n\n";
    std::cout << "Args incjeted:" << converted_data.getArgsCount() << " | "
              << "Bytes given: " << converted_data.getBytesCount() << "\n";
    std::cout << "\n";
    std::cout << "Types deducted, variables rearanged and interpreted as: \n";
    std::cout << "uint8_t " << static_cast<int>(var_one_byte) << "\n"
              << "uint16_t " << var_two_bytes << "\n"
              << "uint32_t " << var_four_bytes << "\n"
              << "uint32_t " << var_four_bytes2 << 
              std::endl;

    auto [new_var, new_var2, new_var3, new_var4] = converted_data.getConstReferences();
    std::cout << static_cast<int>(new_var) << new_var2 << new_var3 << new_var4 << "\n";

    std::cout << "\n";



## Example output
>   
    Bytes data:
    255, 255, 255, 0, 0, 0, 1, 255, 255, 255, 255, 

    Args incjeted:4 | Bytes given: 11

    Types deducted, variables rearanged and interpreted as: 
    uint8_t 255
    uint16_t 65535
    uint32_t 1
    uint32_t 4294967295


## How to build test file with examples
>   

    mkdir build
    cd build
    cmake ..
    make -j4



