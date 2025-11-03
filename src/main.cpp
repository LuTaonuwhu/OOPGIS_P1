#include "fileio.h"
#include <iostream>

int main(int argc, char* argv[])
{

    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <mode> <input_file> <output_file>\n";
        std::cout << "Modes:\n";
        std::cout << "  b2t - Convert binary to text\n";
        std::cout << "  t2b - Convert text to binary\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    fileio file_processor(input_file, output_file);

    if (mode == "b2t")
    {
        file_processor.Binary2Text();
    }
    else if (mode == "t2b")
    {
        file_processor.Text2Binary();
    }
    else
    {
        std::cout << "Invalid mode specified. Use 'b2t' or 't2b'.\n";
        return 1;
    }

    return 0;
}