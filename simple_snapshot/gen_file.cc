#include <iostream>
#include <unistd.h>  
#include <fcntl.h>   
#include <cstring>   
#include <cerrno>    

int main() {
    // Define output file path
    const char* output_file = "/redis.conf";

    // Open the output file (create if it doesn't exist, truncate if it exists)
    int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        std::cerr << "Error: Could not open output file: " << output_file << " - " << strerror(errno) << std::endl;
        return 1;
    }

    // Track the total bytes written
    size_t total_bytes_written = 0;
    const size_t sync_threshold = 192 * 1024; // 192KB

    // Generate and write 20000 lines
    for (int line_num = 1; line_num <= 20000; ++line_num) {
        // Generate the line content
        char line[128]; // Buffer to hold the line
        int line_length = snprintf(line, sizeof(line),
                                   "# line %d: This is a simple demo of Eviction Attack, it will show how to capture a snapshot and restore it\n",
                                   line_num);

        // Write the line to the output file
        ssize_t bytes_written = write(out_fd, line, line_length);
        if (bytes_written == -1) {
            std::cerr << "Error: Failed to write to output file: " << strerror(errno) << std::endl;
            break;
        }

        // Update the total bytes written
        total_bytes_written += bytes_written;

        // Print to the console
     //   std::cout.write(line, line_length);
     	if (line_num == 5000) {
	    std::cout<<"reach line 10000, sleep 60 seconds" << std::endl;
		sleep(60);
	}

        // Check if the total bytes written reaches the threshold
    }

    // Note: File descriptor is not closed here, keeping the file open
    std::cout << "Fileclose. File descriptor: out_fd=" << out_fd << std::endl;
    fsync(out_fd);
    close(out_fd);
    // Sleep for 300 seconds

    return 0;
}
