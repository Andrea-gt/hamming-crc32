"""
Hamming's Code Implementation

This module implements Hamming code error detection and correction on binary strings.
It provides functions to detect and correct single-bit errors in encoded messages.
The main function, `hamming`, takes a binary string as input and returns the corrected
message or an error indication if multiple errors are detected.

Author: Andrea Ximena Ramirez Recinos
Date: Jul 2024
"""

import pandas as pd
from functools import reduce

def hamming(input_string):
    """
    Perform Hamming code error detection and correction on a binary string.

    This function checks for errors in the encoded binary message using Hamming code.
    It detects and corrects single-bit errors, and reports if multiple errors are suspected.
    The function returns the original message if no errors are detected,
    or the corrected message if a single error is found.

    Parameters:
    - input_string (str): A binary string representing the encoded message.

    Returns:
    - str: The original message if no errors are detected,
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
        - str: The result of the XOR operation as a string.
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
        print(f'Error: The message has errors at position {xor_int}. Message discarded.\nCorrect message: {input_string[::-1]}')
        return None

    else:
        print(f'The message is error-free: {input_string[::-1]}')
        # Generate a new binary string excluding the parity bits
        data_bits = []
        for i in range(1, len(input_string) + 1):
            if not is_power_of_two(i):
                data_bits.append(input_string[i - 1])

        # Join the data bits into a long binary string
        binary_data_string = ''.join(data_bits)
        
        # Convert the binary string into ASCII text
        divided_message = [binary_data_string[i:i + 8] for i in range(0, len(binary_data_string), 8)]
        ascii_text = binary_to_string(divided_message)
        return ascii_text

def getParityBits(input_length):
    """
    Calculate the number of parity bits required for a given input length.

    The number of parity bits is determined based on the length of the input string
    to ensure that enough bits are available for error detection and correction.

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

def is_power_of_two(n):
    """
    Check if a number is a power of two.

    This function determines if the given integer is a power of two,
    which is useful for identifying the positions of parity bits.

    Parameters:
    - n (int): The number to check.

    Returns:
    - bool: True if n is a power of two, False otherwise.
    """
    return (n & (n - 1)) == 0 and n != 0

# Function to convert binary to ASCII value
def binary_to_string(bits):
    """
    Convert a list of binary strings to their corresponding ASCII characters.

    This function takes a list of binary strings (each representing an ASCII character)
    and converts them into a single string of characters.

    Parameters:
    - bits (list): List of binary strings to be converted to ASCII.

    Returns:
    - str: The resulting string after converting the binary values to characters.
    """
    return ''.join([chr(int(i, 2)) for i in bits])