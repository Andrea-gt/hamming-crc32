import socket
from utils.fletcher_receptor import Fletcher16, validateInputString
from utils.hamming_receptor import hamming

def main():
    """
    Main function to display the menu and handle user input for Fletcher's checksum operations.
    """

    HOST = ''                 # Symbolic name meaning all available interfaces
    PORT = 50007              # Arbitrary non-privileged port

    # Setting up the socket for server connection
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as socket_:
        socket_.bind((HOST, PORT))
        socket_.listen(1)
        conn, addr = socket_.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print("Received data:", data.decode('utf-8'))

    while True:
        # Display the menu and get user input
        operation = input("\nSelect an operation: \n[1] Perform Fletcher's Checksum.\n[2] Perform Hamming Code Error Detection.\n[3] Exit Program.\n")

        # Check user input and perform corresponding actions
        if operation == '1':
            # Get binary input from the user
            binary_entry = input("Enter the binary data to be transferred:\n")
            # Validate the input string
            if not validateInputString(binary_entry):
                print("Error: Invalid input. Only binary strings (containing '0' and '1') are allowed.\n")
            else:
                # Perform Fletcher's checksum procedure
                Fletcher16(input_string=binary_entry)

        elif operation == '2':
            # Get binary input from the user
            binary_entry = input("Enter the binary data to be transferred:\n")
            # Validate the input string
            if not validateInputString(binary_entry):
                print("Error: Invalid input. Only binary strings (containing '0' and '1') are allowed.\n")
            else:
                # Perform Fletcher's checksum procedure
                hamming(input_string=binary_entry)

        elif operation == '3':
            # Exit the loop and end the program
            print("Exiting program. Goodbye!")
            break

        else:
            # Handle invalid input
            print("Invalid choice. Please select a valid option from the menu.\n")

# Entry point of the program
if __name__ == "__main__":
    main()
