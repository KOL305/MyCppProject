#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <brotli/encode.h>

// Function to read a file into a vector
bool readFile(const std::string& filename, std::vector<uint8_t>& buffer) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer.resize(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return false;
    }
    return true;
}

// Function to write a vector to a file
bool writeFile(const std::string& filename, const std::vector<uint8_t>& buffer) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    return true;
}

// Function to compress data using Brotli
std::vector<uint8_t> compressData(const std::vector<uint8_t>& input) {
    BrotliEncoderState* state = BrotliEncoderCreateInstance(nullptr, nullptr, nullptr);
    if (state == nullptr) {
        throw std::runtime_error("Failed to create Brotli encoder instance");
    }

    // Estimate compressed size
    size_t input_size = input.size();
    size_t compressed_size = BrotliEncoderMaxCompressedSize(input_size);
    std::vector<uint8_t> compressed_data(compressed_size);

    size_t available_in = input_size;
    size_t available_out = compressed_size;
    size_t total_out = 0;
    const uint8_t* input_buffer = input.data();
    uint8_t* output_buffer = compressed_data.data();

    // Set Brotli parameters
    BrotliEncoderSetParameter(state, BROTLI_PARAM_QUALITY, 11);
    BrotliEncoderSetParameter(state, BROTLI_PARAM_LGWIN, 22);

    // Compress data
    if (BrotliEncoderCompressStream(
            state,
            BROTLI_OPERATION_FINISH,
            &available_in,
            &input_buffer,
            &available_out,
            &output_buffer,
            &total_out) == BROTLI_FALSE) {
        BrotliEncoderDestroyInstance(state);
        throw std::runtime_error("Compression failed");
    }

    BrotliEncoderDestroyInstance(state);

    compressed_data.resize(total_out); // Resize to actual compressed size
    return compressed_data;
}

int main() {
    std::string input_filename = "output1.bmp";
    std::string output_filename = "outputdynamic.br";

    std::vector<uint8_t> input_data;
    if (!readFile(input_filename, input_data)) {
        return 1;
    }

    std::vector<uint8_t> compressed_data;
    try {
        compressed_data = compressData(input_data);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (!writeFile(output_filename, compressed_data)) {
        return 1;
    }

    std::cout << "File compressed and saved as " << output_filename << std::endl;
    return 0;
}

