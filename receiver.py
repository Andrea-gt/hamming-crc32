"""
Server Implementation

This module implements a server that performs Fletcher's checksum and Hamming code error detection.
It sets up a socket to listen for incoming connections and allows users to select operations
for data validation and error detection on received binary strings.

Author: Andrea Ximena Ramirez Recinos
Date: Jul 2024
"""

import socket
from utils.server.fletcher_receptor import Fletcher16, validateInputString
from utils.server.hamming_receptor import hamming

def main():
    """
    Main function to display the menu and handle user input for Fletcher's checksum operations.

    This function sets up a TCP server that listens for incoming connections on a specified port.
    Upon receiving data from a client, it presents a menu to the user for selecting operations:
    1. Perform Fletcher's Checksum
    2. Perform Hamming Code Error Detection
    3. Exit the Program

    The selected operation is executed based on the user's choice, and the results are displayed.
    """
    
    HOST = ''                 # Bind to all available interfaces
    PORT = 50007              # Define an arbitrary non-privileged port
    data = ""

    # Set up the socket for server connection
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as socket_:
        socket_.bind((HOST, PORT))
        

        while True:

            socket_.listen(1)
            conn, addr = socket_.accept()  # Wait for a connection
            with conn:
                print('Connected by', addr)
                while True:
                    received_data = conn.recv(1024)  # Receive data from the client
                    if not received_data:  # Check for connection closure
                        break
                    data = received_data.decode('utf-8')  # Decode byte data to string
                    print("Received data:", data)

            # Display the menu and get user input
            operation = input("\nSelect an operation: \n[1] Perform Fletcher's Checksum.\n[2] Perform Hamming Code Error Detection.\n[3] Exit Program.\n")

            # Check user input and perform corresponding actions
            if operation == '1':
                print("Data to process:", data)  # Display data for Fletcher's checksum
                if not validateInputString(data):  # Validate input
                    print("Error: Invalid input. Only binary strings (containing '0' and '1') are allowed.\n")
                else:
                    result = Fletcher16(input_string=data)  # Perform Fletcher's checksum
                    print("Fletcher's checksum result:", result)
            elif operation == '2':
                if not validateInputString(data):  # Validate input
                    print("Error: Invalid input. Only binary strings (containing '0' and '1') are allowed.\n")
                else:
                    result = hamming(input_string=data)  # Perform Hamming code error detection
                    print("Hamming code detection result:", result)
            elif operation == '3':
                print("Exiting program. Goodbye!")  # Exit the program
                break
            else:
                print("Invalid choice. Please select a valid option from the menu.\n")  # Handle invalid input

# Entry point of the program
if __name__ == "__main__":
    main()