
import argparse
import struct
import sys

def calculate_crc(contents, seed):
    accumulating_xor_value = seed;

    for i in range(0, len(contents), 4):
        value = struct.unpack('<I', contents[ i : i + 4])[0]
        accumulating_xor_value = accumulating_xor_value ^ value
    return accumulating_xor_value

def xor_block(r0, r1, block_number, block_size, file_key):
    # This is the loop counter
    loop_counter = 0x0

    # This is the key length
    key_length = 0x18

    # This is an initial seed
    xor_seed = 0x4bad

    # This is the block counter
    block_number = xor_seed * block_number

    #load the xor key from the file
    r7 =  file_key

    for loop_counter in range(0, block_size):
        # meant to make sure different bits of the key are used.
        xor_seed = loop_counter/key_length

        # IP is a scratch register / R12
        ip = loop_counter - (key_length * xor_seed)

        # xor_seed = (loop_counter * loop_counter) + block_number
        xor_seed = (loop_counter * loop_counter) + block_number

        # shift the xor_seed left by the bits in IP.
        xor_seed = xor_seed >> ip

        # load a byte into IP
        ip = r0[loop_counter]

        # XOR the seed with r7
        xor_seed = xor_seed ^ r7

        # and then with IP
        xor_seed = xor_seed ^ ip

        #Now store the byte back
        r1[loop_counter] = xor_seed & 0xFF

        #increment the loop_counter
        loop_counter = loop_counter + 1


def decrypt_file(input_file_name, output_file_name):
    file = bytearray(open(input_file_name, 'rb').read())
    output_file = open(output_file_name, 'wb')

    file_key = struct.unpack('<I', file[ 4 : 8])[0]
    block_size = 0x800
    header_size = 0xC

    block_count = (len(file) - header_size) / block_size
    for block_number in range(0, block_count):
        block_offset = header_size + (block_number * block_size)
        block_end = block_offset + block_size
        block_array = bytearray(file[block_offset: block_end])
        xor_block(block_array, block_array, block_number, block_size, file_key)
        output_file.write(block_array)

def encrypt_file(input_file_name, output_file_name):
    input_file = bytearray(open(input_file_name, 'rb').read())
    output_file = open(output_file_name, 'wb')

    block_size = 0x800
    key_length = 0x18
    file_key = 0x947FB2DA
    file_key = 0xDAB27F94

    xor_crc = 0xef3d4323;

    # write the file header
    output_file.write(struct.pack(">I", 0x443D2D3F))
    # encrypt the contents using a known file header key

    # write the file_key
    output_file.write(struct.pack(">I", 0x947FB2DA))

    #TODO - how to enforce that the firmware aligns to block boundaries?
    block_count = len(input_file) / block_size

    for block_number in range(0, block_count):
        block_offset = (block_number * block_size)
        block_end = block_offset + block_size
        block_array = bytearray(input_file[block_offset: block_end])
        xor_block(block_array, block_array, block_number, block_size, file_key)
        for n in range (0, block_size):
            input_file[block_offset + n] = block_array[n]
        # update the expected CRC value.
        xor_crc = calculate_crc(block_array, xor_crc)

    # and then CRC
    # write the file_key
    output_file.write(struct.pack("<I", xor_crc))

    # finally, append the encrypted results.
    output_file.write(input_file)
    return

# Main function
parser = argparse.ArgumentParser("Chitu Encryption Script Manager")
parser.add_argument("inputFile", help="File to read from.")
parser.add_argument("outputFile", help="File to write to.")
parser.add_argument("operation", help="encrypt or decrypt.")

args = parser.parse_args()

if args.operation == "decrypt":
    decrypt_file(args.inputFile, args.outputFile)
else:
    encrypt_file(args.inputFile, args.outputFile)