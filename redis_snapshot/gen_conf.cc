#include <iostream>
#include <unistd.h>  
#include <fcntl.h>   
#include <cstring>   
#include <cerrno>    

int main() {
    // Define input and output file paths
    const char* input_file = "/redis.conf.source";
    const char* output_file = "/redis.conf";

    // Open the input file
    int in_fd = open(input_file, O_RDONLY);
    if (in_fd == -1) {
        std::cerr << "Error: Could not open input file: " << input_file << " - " << strerror(errno) << std::endl;
        return 1;
    }

    // Open the output file (create if it doesn't exist, truncate if it exists)
    int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        std::cerr << "Error: Could not open output file: " << output_file << " - " << strerror(errno) << std::endl;
        close(in_fd); // Close the input file descriptor
        return 1;
    }

    // Buffer size
    const size_t buffer_size = 1024;
    char buffer[buffer_size];

    // Track the total bytes written
    size_t total_bytes_written = 0;
    const size_t sync_threshold = 200 * 1024; // 200KB

    // Read the input file line by line and write to the output file
    ssize_t bytes_read;
    while ((bytes_read = read(in_fd, buffer, buffer_size)) > 0) {
        // Write to the output file
        ssize_t bytes_written = write(out_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            std::cerr << "Error: Failed to write to output file: " << strerror(errno) << std::endl;
            break;
        }

        // Update the total bytes written
        total_bytes_written += bytes_written;

        // Print to the console
        std::cout.write(buffer, bytes_written);

        // Check if the total bytes written reaches the threshold
        if (total_bytes_written >= sync_threshold) {
            std::cout << "\nReached 192KB, sleep 1 min to capture the snapshot." << std::endl;
            total_bytes_written = 0; // Reset the counter
            sleep(60);
        }
    }

    if (bytes_read == -1) {
        std::cerr << "Error: Failed to read from input file: " << strerror(errno) << std::endl;
    }

    // Final fsync to ensure all data is flushed
    std::cout << "Final fsync..." << std::endl;
    if (fsync(out_fd) == -1) {
        std::cerr << "Error: Failed to fsync output file: " << strerror(errno) << std::endl;
    }

    // Note: File descriptors are not closed here, keeping the files open
    std::cout << "Files remain open. File descriptors: in_fd=" << in_fd << ", out_fd=" << out_fd << std::endl;

    // Sleep for 30 seconds
    fsync(out_fd);
    close(out_fd);
    close(in_fd);

    return 0;
}