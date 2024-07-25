/**
 * Hamming Code Encoding
 *
 * This program implements the Hamming code to encode a binary string of data. It performs the following functions:
 *
 * 1. **Parity Bits Calculation**: Computes the number of redundant (parity) bits required for encoding the input binary data.
 * 2. **Data Encoding**: Inserts the data bits and calculates the parity bits based on the Hamming code algorithm. It then places the parity bits in the appropriate positions to form the encoded data.
 * 3. **Input Validation**: Ensures that the input string contains only binary characters ('0' and '1'). If invalid characters are present, the program will indicate an error.
 *
 * Usage:
 * - Compile the source code to generate an executable.
 * - Run the generated executable with the binary string as an argument. For example: `./<executable> <data>`.
 * - The program will output the encoded data, including the redundant parity bits.
 *
 * Author: Adrian Ricardo Flores Trujillo
 * Date: July 2024
 */


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

/**
 * Calculate the number of redundant bits needed for error detection.
 *
 * @param data A vector of integers representing the original data.
 * @return The number of redundant bits required.
 */
int calcRedundantBits(const std::vector<int>& data) {
    int m = data.size(); // Length of the original data
    int r = 0; // Number of redundant bits

    // Calculate the minimum number of redundant bits needed
    while (std::pow(2, r) < (m + r + 1)) {
        r++;
    }

    return r;
}

/**
 * Convert an integer to a binary vector of size r.
 *
 * @param n The integer to be converted.
 * @param r The size of the resulting binary vector.
 * @return A vector of integers representing the binary representation of n.
 */
std::vector<int> intToBinaryVector(int n, int r) {
    std::vector<int> binary;

    // If the number is zero, push a single zero
    if (n == 0) {
        binary.push_back(0);
    } else {
        // Convert the integer to binary
        while (n > 0) {
            binary.push_back(n % 2);
            n /= 2;
        }
    }

    // Pad the binary vector with zeros to ensure it has size r
    while (binary.size() < r) {
        binary.push_back(0);
    }

    // If the binary vector is longer than r, resize it to r
    if (binary.size() > r) {
        binary.resize(r);
    }

    // Reverse the binary vector to get the correct bit order
    std::reverse(binary.begin(), binary.end());
    return binary;
}

/**
 * Encode data using Hamming code.
 *
 * @param data A vector of integers representing the original data.
 * @return A vector of integers representing the Hamming encoded data.
 */
std::vector<int> hammingEncode(const std::vector<int>& data) {
    int msgLength = data.size(); // Length of the original data
    int r = calcRedundantBits(data); // Number of redundant bits
    int n = r + msgLength; // Total length of the encoded data

    // Initialize the encoded data vector with zeros
    std::vector<int> encodedData(n, 0);

    // Place the original data bits into the encoded data vector
    int dataPos = 0;
    for (int i = 0; i < n; i++) {
        // Skip positions that are powers of 2 (for redundant bits)
        if ((i & (i + 1)) != 0) {
            encodedData[i] = data[dataPos];
            dataPos++;
        }
    }

    // Calculate and place the redundant (parity) bits
    for (int i = 0; i < r; i++) {
        int parityPos = (1 << i) - 1; // Position of the parity bit
        int parity = 0;

        // Calculate parity for the current position
        for (int j = parityPos; j < n; j += (1 << (i + 1))) {
            for (int k = 0; k < (1 << i) && j + k < n; k++) {
                parity ^= encodedData[j + k];
            }
        }

        encodedData[parityPos] = parity; // Set the parity bit
    }

    // Reverse the encoded data vector to match the desired output format
    std::reverse(encodedData.begin(), encodedData.end());

    // Output the encoded data
    std::cout << "Encoded Data: ";
    for (int bit : encodedData) {
        std::cout << bit;
    }
    std::cout << std::endl;

    return encodedData;
}

/**
 * Main function to encode data using Hamming code and output the result.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 2) {
        std::cout << "Incorrect call: <executable> <Data>";
        return 1;
    }

    // Get the data string from the command line argument
    std::string dataString = argv[1];
    std::vector<int> data;

    // Convert the data string to a vector of integers
    for (char c : dataString) {
        if (c == '0' || c == '1') {
            data.push_back(c - '0');
        } else {
            std::cout << "Invalid data character: " << c << std::endl;
            return 1;
        }
    }

    // Reverse the data vector to match the desired input format
    std::reverse(data.begin(), data.end());

    // Encode the data using Hamming code
    std::vector<int> hammingCode = hammingEncode(data);

    return 0;
}
