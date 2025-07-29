/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_FILEMANAGER_HPP
#define ILRD_RD1645_FILEMANAGER_HPP

#include <fstream>          //std::fstream

#include "IFileManager.hpp"

namespace ilrd
{
namespace details   
{


class FileManager final : public IFileManager
{
public:
    FileManager(const std::string& file_path_ = "./minion.txt");
    STATUS Read(size_t offset_, size_t length_, std::shared_ptr<char[]> read_to_);
    STATUS Write(size_t offset_, size_t length_, std::shared_ptr<char[]> write_from_);

private:
    std::fstream m_file;
    std::mutex m_mtx;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_FILEMANAGER_HPP



