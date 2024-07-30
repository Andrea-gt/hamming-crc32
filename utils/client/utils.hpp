#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <random>
#include <chrono>

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

    return encodedData;
}

/**
 * Converts a vector of integers (0s and 1s) to a C-style string (null-terminated char array).
 *
 * @param vec The input vector of integers.
 * @return A pointer to the newly allocated C-style string representing the binary data.
 */
char* vectorToCString(const std::vector<int>& vec) {
    size_t size = vec.size();
    // Allocate memory for the string, including the null terminator
    char* result = new char[size + 1];

    // Convert each integer in the vector to the corresponding char ('0' or '1') and assign to the array
    for (size_t i = 0; i < size; ++i) {
        result[i] = (vec[i] == 1) ? '1' : '0';
    }

    // Null-terminate the string
    result[size] = '\0';

    return result;
}

/**
 * Tests whether an event with a given probability occurs.
 *
 * @param probability The probability (0-100) that the event occurs.
 * @return true if the event occurs, false otherwise.
 */
bool testProbability(int probability) {
    // Seed the random number generator with the current time to ensure different results between calls
    static std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dist(0.0, 1.0); // Uniform distribution in the range [0.0, 1.0)

    // Generate a random number and check if it falls within the given percentage
    return dist(rng) < static_cast<float>(probability) / 100.0;
}

/**
 * Prints the contents of a vector of integers (0s and 1s) with an optional separator between elements.
 *
 * @param vec The input vector of integers.
 * @param sep The separator string to be printed between elements.
 */
void printVector(std::vector<int>& vec, std::string sep = "") {
    // Print each element in the vector followed by the separator
    for (int bit : vec) {
        std::cout << bit << sep;
    }
    std::cout << std::endl; // Print a newline at the end
}