
#include <iostream>
#include "ByteRanger.hpp"

int main() {
    std::vector<uint8_t> data_in_bytes = {255, 255, 255, 0, 0, 1, 1};

    uint8_t variable_one_byte = 0;
    uint16_t variable_two_bytes = 0;
    uint32_t variable_four_bytes = 0;

    ByteRanger rearanged_data{data_in_bytes, variable_one_byte, variable_two_bytes, variable_four_bytes};

    std::cout << "\n";
    std::cout << "Bytes data:\n";
    for (const auto& byte : data_in_bytes) {
        std::cout << static_cast<int>(byte) << ", ";
    }
    std::cout << "\n\n";
    std::cout << "Args incjeted:" << rearanged_data.getArgsCount() << " | "
              << "Bytes given:" << rearanged_data.getBytesCount() << "\n";
    std::cout << "\n";
    std::cout << "Types deducted, variables rearanged and interpreted as: \n";
    std::cout << "uint8_t " << static_cast<int>(variable_one_byte) << "\n"
              << "uint16_t " << variable_two_bytes << "\n"
              << "uint32_t " << variable_four_bytes << std::endl;
    std::cout << "\n";
}
