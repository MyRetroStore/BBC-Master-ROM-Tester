#!/usr/bin/python3

# Python script to generate CRC-32 hashes for BBC Master 128 ROM images (16k) used by the Arduino based BBC ROM Tester
# https://github.com/MyRetroStore/BBC-Master-ROM-Tester


# Search crc.h file if hash already exists
def search_and_insert(search_word,insert_word, marker):
	try:
		with open("crc.h", 'r') as file:
			lines = file.readlines()
	except FileNotFoundError:
		print(f"Error: File crc.h not found.")
		quit()
	except Exception as e:
		print(f"Error: Unable to open file crc.h. Reason: {e}")
		quit

	found = False
	for i, line in enumerate(lines):
		if search_word in line:
			print ("Found existing CRC value in crc.h file. No need to update")
			found = True
			break

	if not found:
		for i, line in enumerate(lines):
			if marker in line:
				lines.insert(i, insert_word + '\n')
				break

		with open("crc.h", 'w') as file:
			print ("Hash added to crc.h file")
			file.writelines(lines)

POLY = 0xEDB88320

def calculate_crc32(crc, value):
    crc ^= value
    for _ in range(32):
        crc = (crc >> 1) ^ (POLY if crc & 1 else 0)
    return crc

def crc32_for_buffer(buffer):
    crc = 0xFFFFFFFF#

    for byte in buffer:
        crc = calculate_crc32(crc, byte)

    crc ^= 0xFFFFFFFF
    return crc

if __name__ == "__main__":
	import sys
	import argparse
	import os
#	import hashlib

	crc = 0xFFFFFFFF
	valid_file_sizes = [16384]	#File size of the ROM must be one of these

	parser = argparse.ArgumentParser(description="A Python script to generate ROM file md5's used with Reto ROM Tester")
	parser.add_argument('-i', '--input', help="Input file")
	args = parser.parse_args()

	input_file = args.input
	if not len(sys.argv) > 1:
		print ("No arguments passed.")
		print ("Try running generate_crc.py -h")
		quit()

	print ("Calculating CRC hash for " + input_file)

	if not os.path.isfile(input_file):
		print (f"Error: File '{input_file}' does not exist!")
		quit()

	size = os.path.getsize(input_file)

	if size not in valid_file_sizes:
		print ("Error: File size needs to be 16384 bytes")
		quit()

	chunk_size = 4096  # 4kB chunks
	crc_values = []

	with open(input_file, 'rb') as file:
		while True:
			chunk = file.read(chunk_size)
			if not chunk:
				break
			crc = crc32_for_buffer(chunk)
			crc_values.append(crc)

	crc = 0xFFFFFFFF

    # Combine all CRC values
	for value in crc_values:
		crc = calculate_crc32(crc, value)

    # Finalize the CRC calculation
	crc ^= 0xFFFFFFFF

	print(f"CRC-32: 0x{crc:08X}")
	hex_string = f'{crc:08X}'
	if input_file.endswith(".bin"):
		input_file = os.path.basename(input_file)
		input_file = input_file[:-4]
		input_file = input_file.replace('-', '_').replace('.', '_')
	search_and_insert(str(hex_string), '  {"' + input_file + '", "' + str(hex_string) + '"},','End of 16k marker')
