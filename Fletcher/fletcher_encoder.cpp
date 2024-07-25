/**
 * Fletcher Checksum Calculation
 *
 * This program calculates the Fletcher checksum of a binary string. It performs the following functions:
 *
 * 1. **Checksum Calculation**: Computes the Fletcher checksum of the input binary data using a specified block size.
 * 2. **Binary Conversion**: Converts the checksum value to both binary and hexadecimal representations.
 * 3. **Input Validation**: Ensures that the input string contains only binary characters ('0' and '1'). If invalid characters are present, the program will indicate an error.
 *
 * Usage:
 * - Compile the source code to generate an executable.
 * - Run the generated executable with the binary string and block size as arguments. For example: `./<executable> <data> <blockSize>`.
 * - The program will output the Fletcher checksum in both hexadecimal and binary formats, along with the original data and checksum combined.
 *
 * Author: Adrian Ricardo Flores Trujillo
 * Date: July 2024
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>

/**
 * Convert a binary vector to an integer.
 *
 * @param data A vector of integers representing the binary data.
 * @return The integer value of the binary data.
 */
int binaryVectorToInt(const std::vector<int>& data) {
    int result = 0;
    for (int bit : data) {
        result = (result << 1) | bit;
    }
    return result;
}

/**
 * Calculate the Fletcher checksum for the given data.
 *
 * @param data A vector of integers representing the data.
 * @param mode The checksum mode (block size).
 * @return The calculated checksum.
 */
uint64_t fletcherChecksum(const std::vector<int>& data, int mode) {
    int length = data.size();
    int blockSize = mode / 2;

    // Determine modulus and bit shifts based on block size
    uint64_t modulus = pow(2, blockSize) - 1;
    uint64_t sum1 = 0;
    uint64_t sum2 = 0;

    // Process each block
    for (int i = 0; i < length; i++) {
        sum1 = (sum1 + data[i]) % modulus;
        sum2 = (sum2 + sum1) % modulus;
    }

    return (sum2 << blockSize) | sum1;
}

/**
 * Convert a uint64_t value to a binary string of a specific size.
 *
 * @param value The value to convert.
 * @param bits The size of the binary string.
 * @return A binary string representation of the value.
 */
std::string toBinaryString(uint64_t value, int bits) {
    std::bitset<64> binary(value);
    return binary.to_string().substr(64 - bits, bits);
}

/**
 * Main function to calculate Fletcher checksum and output the result.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <Data> <BlockSize>" << std::endl;
        return 1;
    }

    // Get the data string and block size from command line arguments
    std::string dataString = argv[1];
    int blockSize = std::stoi(argv[2]);

    // Convert the data string to a vector of integers
    std::vector<int> data;
    for (char c : dataString) {
        if (c == '0' || c == '1') {
            data.push_back(c - '0');
        } else {
            std::cout << "Invalid data character: " << c << std::endl;
            return 1;
        }
    }

    // Validate block size
    if (blockSize != 8 && blockSize != 16 && blockSize != 32) {
        std::cout << "Invalid block size. Use 8, 16, or 32." << std::endl;
        return 1;
    }

    // Compute the Fletcher checksum
    uint64_t checksum = fletcherChecksum(data, blockSize);
    std::string checksumBinary = toBinaryString(checksum, blockSize);

    // Join the checksum binary to the original data
    std::string dataWithChecksum = dataString + checksumBinary;

    // Output results
    std::cout << "Original data: " << dataString << std::endl;
    std::cout << "Fletcher checksum (hex): " << std::hex << checksum << std::endl;
    std::cout << "Fletcher checksum (binary): " << checksumBinary << std::endl;
    std::cout << "Data with checksum: " << dataWithChecksum << std::endl;

    return 0;
}
