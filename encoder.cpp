/**
 * Communication Client with Error Detection
 *
 * This program performs the following functions:
 *
 * 1. **Message Encoding**: Converts a given message into its binary representation.
 * 2. **Error Detection**: Computes either the Fletcher checksum or encodes the message using Hamming code based on user input.
 * 3. **Noise Simulation**: Simulates noise by randomly flipping bits in the encoded message based on a specified probability.
 * 4. **Network Communication**: Sends the final encoded (and possibly noisy) message to a specified server.
 *
 * Usage:
 * - Compile the source code to generate an executable.
 * - Run the generated executable with the message, algorithm choice ('fletcher' or 'hamming'), and bit flip probability as arguments.
 *   For example: `./<executable> "message" fletcher 10`.
 * - The program will output the encoded message before and after noise simulation, along with the indices of flipped bits.
 * - It will then send the final message to the server specified by SERVER_IP and port 50007.
 *
 * Author: Adrian Ricardo Flores Trujillo
 * Date: July 2024
 */

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <unistd.h>
#include <vector>
#include <bitset>
#include "utils/client/utils.hpp"

// Define the server IP address
const char* SERVER_IP = "192.168.1.7";

/**
 * @brief Converts a string to a binary vector.
 * 
 * This function takes a string as input and converts each character 
 * to its corresponding binary representation, which is then stored 
 * in a vector of integers.
 * 
 * @param str The input string.
 * @param binaryVector The output binary vector.
 */
void convertToBinaryVector(const char* str, std::vector<int>& binaryVector) {
    for (const char* p = str; *p != '\0'; ++p) {
        // Get ASCII value of the character
        unsigned char asciiValue = static_cast<unsigned char>(*p);
        
        // Convert to binary and extract each bit
        std::bitset<8> bits(asciiValue);
        for (int i = 0; i < 8; ++i) {
            binaryVector.push_back(bits[i]);
        }
    }
}

int main(int argc, char* argv[]) { 
    // Check for the correct number of arguments
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <Message> <Algorithm> <Bit flip %>" << std::endl;
        return 1;
    }

    // Extract arguments
    const char* message = argv[1];
    const char* algorithm = argv[2];
    int choice;

    // Determine the algorithm choice based on input
    if (strcmp(algorithm, "fletcher") == 0) {
        choice = 0;
    } else if (strcmp(algorithm, "hamming") == 0) {
        choice = 1;
    } else {
        std::cout << "Error: " << algorithm << " is not implemented. Options: fletcher, hamming." << std::endl;
        return 1;
    }

    // Extract bit flip chance
    const int flipChance = std::stoi(argv[3]);

    // Convert the message to a binary vector
    std::vector<int> binaryVector;
    convertToBinaryVector(message, binaryVector);

    // Compute and append checksum or encode with Hamming code
    if (choice == 0) {
        // Compute the Fletcher checksum
        uint64_t checksum = fletcherChecksum(binaryVector, 16);
        std::vector<int> checksumBinary = intToBinaryVector(checksum, 16);

        std::cout << "Fletcher checksum (hex): " << std::hex << checksum << std::dec << std::endl;

        binaryVector.insert(binaryVector.end(), checksumBinary.begin(), checksumBinary.end());
    } else {
        // Encode with Hamming code
        std::reverse(binaryVector.begin(), binaryVector.end());
        binaryVector = hammingEncode(binaryVector);
    }

    std::cout << "\nEncoded message before noise simulation: \n";
    printVector(binaryVector);

    // Simulate noise by flipping bits based on the provided probability
    std::vector<int> flipIndices;
    for (int i = 0; i < binaryVector.size(); i++) {
        if (testProbability(flipChance)) {
            flipIndices.push_back(i);
            binaryVector[i] ^= 1;
        }
    }

    // Print the modified message and the indices of flipped bits
    if (flipIndices.size() == 0) {
        std::cout << "Noise simulation yielded no changes. The message is unchanged." << std::endl;
    } else {
        std::cout << "New message:\n";
        printVector(binaryVector);

        std::cout << "\nBits were flipped at indices ";
        printVector(flipIndices, " ");
    }
 
    // Create a socket for the client
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set up the server address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(50007);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address/Address not supported." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Send the data to the server
    const char* payload = vectorToCString(binaryVector); 
    send(clientSocket, payload, strlen(payload), 0);

    std::cout << "\nMessage sent." << std::endl;
  
    // Close the socket
    close(clientSocket);
  
    return 0; 
}
