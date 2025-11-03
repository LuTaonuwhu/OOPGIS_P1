#include "fileio.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>

bool dataparser::readBinary()
{
    std::ifstream infile(file_path_in, std::ios::binary | std::ios::in);
    if (!infile.is_open())
    {   std::cout << "Failed to open file for reading: " << file_path_in << std::endl;
        return false;
    }

    // clear data_list before decoding new data
    data_list.clear();

    // Read until a read fails. This is safer than using peek()/EOF for binary reads.
    {
        data d;
        while (infile.read(reinterpret_cast<char*>(&d.str), sizeof(d.str))
               && infile.read(reinterpret_cast<char*>(&d.idx), sizeof(d.idx)))
        {
            data_list.push_back(d);
        }
    }
    infile.close();

    if (!this->sort_datalist())
    {
        std::cout << "Failed to sort data list." << std::endl;
        return false;
    }

    return true;
}

bool dataparser::sort_datalist()
{
    if(data_list.empty())
    {
        std::cout << "Data list is empty, nothing to sort." << std::endl;
        return false;
    }
    std::sort(data_list.begin(), data_list.end(), [](const data &a, const data &b) {
        return a.idx < b.idx;
    });

    is_sorted = true;

    return true;
}

bool dataparser::write2Binary()
{
    if (data_list.empty())
    {
        std::cout << "Data list is empty, nothing to encode." << std::endl;
        return false;
    }

    std::ofstream outfile(file_path_out, std::ios::binary | std::ios::out);
    if (!outfile.is_open())
    {
        std::cout << "Failed to open file for writing: " << file_path_out << std::endl;
        return false;
    }

    if(!this->shuffle_datalist())
    {
        std::cout << "Failed to shuffle data list before encoding." << std::endl;
        return false;
    }

    for (const auto &d : data_list)
    {
        outfile.write(reinterpret_cast<const char*>(&d.str), sizeof(d.str));
        outfile.write(reinterpret_cast<const char*>(&d.idx), sizeof(d.idx));
    }
    outfile.close();
    return true;
}

bool dataparser::shuffle_datalist()
{
    if (data_list.empty())
    {
        std::cout << "Data list is empty, nothing to shuffle." << std::endl;
        return false;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data_list.begin(), data_list.end(), g);

    is_sorted = false;

    return true;
}

bool dataparser::readText()
{
    std::ifstream infile(file_path_in);
    if (!infile.is_open())
    {
        std::cout << "Failed to open file for reading: " << file_path_in << std::endl;
        return false;
    }

    // clear data_list before decoding new data
    data_list.clear();
  
    char ch;
    int seq = 0;
    while (infile.get(ch))
    {
        if (ch == '\r')
            continue; // skip carriage return on Windows
        data d;
        d.idx = seq++;
        d.str = ch;
        data_list.push_back(d);
    }
    infile.close();

    return true;
}

bool dataparser::write2Text()
{
    if (data_list.empty())
    {
        std::cout << "Data list is empty, nothing to encode." << std::endl;
        return false;
    }

    std::ofstream outfile(file_path_out);
    if (!outfile.is_open())
    {
        std::cout << "Failed to open file for writing: " << file_path_out << std::endl;
        return false;
    }

    if(!is_sorted)
    {
       this->sort_datalist();
    }

    for (const auto &d : data_list)
    {
        outfile << d.str;
    }
    outfile.close();
    return true;
}


void fileio::Binary2Text()
{
    if (!this->readBinary())
    {
        std::cout << "Failed to read binary file." << std::endl;
        return;
    }

    if (!this->write2Text())
    {
        std::cout << "Failed to write text file." << std::endl;
        return;
    }

    std::cout << "Successfully converted binary to text." << std::endl;
}

void fileio::Text2Binary()
{
    if (!this->readText())
    {
        std::cout << "Failed to read text file." << std::endl;
        return;
    }

    if (!this->write2Binary())
    {
        std::cout << "Failed to write binary file." << std::endl;
        return;
    }

    std::cout << "Successfully converted text to binary." << std::endl;
}