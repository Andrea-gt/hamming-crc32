"""
Hamming Code Error Detection and Correction

This program implements the Hamming code for error detection and correction on binary strings. It performs the following functions:

1. **Hamming Code Generation**: Takes a binary string as input, calculates the number of parity bits required, and uses them to create a DataFrame for XOR operations to detect any errors in the data.
2. **Error Detection**: Identifies and reports the position of any errors in the binary string based on the Hamming code. If no errors are detected, it confirms the message as error-free.
3. **Input Validation**: Ensures that the input string contains only binary characters ('0' and '1').

Usage:
- Run the program and select an operation from the menu.
- For Hamming code error detection, enter the binary data to be transferred.
- The program will output whether the message is error-free or if errors are detected, along with their position and the correction.

Author: Andrea Ximena Ramirez Recinos
Date: Jul 2024
"""

import pandas as pd
from functools import reduce

def hamming(input_string):
    """
    Perform Hamming code error detection and correction on a binary string.

    Parameters:
    - input_string (str): A binary string representing the encoded message.

    Returns:
    - str: The original input string if no errors are detected,
           or an error message indicating the position of the error.
    """
    # Reverse the input string
    input_string = input_string[::-1]
    # Get the length of the input string
    input_length = len(input_string)
    # Calculate the number of parity bits needed
    parity_bits = getParityBits(input_length)
    # Generate a list of columns based on pow(2, i)
    columns = [pow(2, i) for i in range(parity_bits)]
    rows = []

    for i in range(input_length):
        if input_string[i] == '1':
            rows.append(list("{0:0{1}b}".format(i + 1, parity_bits)))
    
    # Convert rows and columns into a DataFrame
    df = pd.DataFrame(rows, columns=columns)

    # Compute XOR for each column
    def xorColumns(col_values):
        """
        Compute the XOR for a list of column values.

        Parameters:
        - col_values (list): List of column values (strings) to be XORed.

        Returns:
        - str: The result of XOR operation as a string.
        """
        return str(reduce(lambda x, y: int(x) ^ int(y), col_values))
    
    xor_results = [xorColumns(df[col]) for col in df.columns]
    
    # Concatenate the results into a single string
    # and get the position of the error.
    xor_int = int(''.join(xor_results), 2)

    if xor_int != 0:
        # Convert the string to a list for mutability
        correction = list(input_string)
        if xor_int >= input_length:
            return f'Error: The message may have multiple errors.'
        # Fix the bit at the position determined by xor_int
        correction[xor_int - 1] = '1' if correction[xor_int - 1] == '0' else '0'
        input_string = ''.join(correction)
        return f'Error: The message has errors at position {xor_int}. Message discarded.\nCorrect message: {input_string[::-1]}'

    return f'The message is error-free: {input_string[::-1]}'

def getParityBits(input_length):
    """
    Calculate the number of parity bits required for a given input length.

    Parameters:
    - input_length (int): The length of the input string.

    Returns:
    - int: The number of parity bits required.
    """
    parity_bits = 0
    # Determine the number of parity bits needed
    while (2 ** parity_bits < input_length + parity_bits + 1):
        parity_bits += 1
    return parity_bits

def validateInputString(input_string):
    """
    Validate that the input string contains only binary characters ('0' or '1').

    Parameters:
    - input_string (str): The string to validate.

    Returns:
    - bool: True if the input string contains only '0' and '1', False otherwise.
    """
    # Define a set containing binary characters '0' and '1'
    binary_set = {'0', '1'}
    
    # Convert the input string to a set of characters
    input_set = set(input_string)

    # Check if the input set matches the binary set or contains only '0' or '1'
    return binary_set == input_set or input_set == {'0'} or input_set == {'1'}

def main():
    """
    Main function to display the menu and handle user input for Hamming code operations.
    """
    while True:
        # Display the menu and get user input
        operation = input("\nSelect an operation: \n[1] Perform Hamming Code Error Detection.\n[2] Exit Program.\n")

        # Check user input and perform corresponding actions
        if operation == '1':
            # Get binary input from the user
            binary_entry = input("Enter the binary data to be transferred:\n")
            
            # Validate the input string
            if not validateInputString(binary_entry):
                print("Error: Invalid input. Only binary strings (containing '0' and '1') are allowed.\n")
            else:
                # Perform the Hamming procedure
                result = hamming(input_string=binary_entry)
                print(result)
        
        elif operation == '2':
            # Exit the loop and end the program
            print("Exiting program. Goodbye!")
            break

        else:
            # Handle invalid input
            print("Invalid choice. Please select a valid option from the menu.\n")

# Entry point of the program
if __name__ == "__main__":
    main()