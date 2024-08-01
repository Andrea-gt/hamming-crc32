"""
Fletcher's Checksum Implementation

This Python script performs Fletcher's checksum algorithm for error detection in binary data. 

Key Functions:
1. Fletcher16: Computes Fletcher's checksum for a given binary string. It compares the computed checksum with the provided checksum to validate the integrity of the data.
2. validateInputString: Validates that the input string contains only binary characters ('0' and '1').
3. main: Provides a user interface for selecting operations, accepting binary data, validating the input, and performing checksum operations.

Usage:
- Run the script to display a menu with options to perform Fletcher's checksum or exit the program.
- Input binary data to compute and verify the checksum.
- The program handles validation of binary input and prints the result along with any errors.

Author: Andrea Ximena Ramirez Recinos
Date: Jul 2024
"""

def Fletcher16(input_string, size=16):
    """
    Compute Fletcher's checksum for a binary string.

    Parameters:
    - input_string (str): The binary string for which to compute the checksum.
    - size (int): The size of the checksum in bits. Default is 16.

    Returns:
    - bool: True if the checksum matches the original checksum, False otherwise.
    """
    sum1 = 0
    sum2 = 0

    # Extract the original checksum from the end of the input string
    original_checksum = input_string[-size:]
    input_string = input_string[:-size]
    module = 255

    # Process each character of the input string
    for char in input_string:
        # Convert the character to an integer
        value = int(char, 2)
        # Update the checksum sums
        sum1 = (sum1 + value) % module
        sum2 = (sum2 + sum1) % module

    # Calculate the mask to ensure the result fits within the bit size
    mask = (1 << size) - 1
    # Compute the checksum
    checksum = f"{(sum2 << (size // 2) | sum1) & mask:0{size}b}"

    # Print the computed checksum and the original checksum for comparison
    print(f"Computed checksum: {checksum}, Original checksum: {original_checksum}")
    
    if not original_checksum == checksum:
        print(f"Error: This message has errors. Message discarded.")

    else:
        print(f"Message is valid. Original message: {input_string}")
                    

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