#ifndef FILEIO_H
#define FILEIO_H

#include <vector>
#include <string>

struct data
{
    int idx;
    char str;
};

class dataparser
{
    public:
        dataparser(std::string file_path_in, std::string file_path_out)
         : file_path_in(std::move(file_path_in)), file_path_out(std::move(file_path_out)) 
         {};
    
    protected:
        bool readBinary();
        bool readText();
        bool write2Binary();
        bool write2Text();

        std::vector<data> data_list;
    
    private:
        std::string file_path_in;
        std::string file_path_out;
        bool sort_datalist();
        bool shuffle_datalist();

        bool is_sorted = false;

};

class fileio : public dataparser
{
    public:
    fileio(std::string infile, std::string outfile)
     : dataparser(std::move(infile), std::move(outfile)) {};
        
        void Binary2Text();
        void Text2Binary();

};

#endif // FILEIO_H


