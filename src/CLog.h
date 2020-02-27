#ifndef CLOG_H
#define CLOG_H

#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

class CLog {
private:
    std::fstream m_file;

    CLog() {
        m_file.open("log.txt", std::ios::out | std::ios::trunc);
    }

    ~CLog() {
        if (m_file.is_open())
            m_file.close();
    }

    CLog(const CLog &);

public:
    static CLog & instance () {
        static CLog singleton;
        return singleton;
    }

    template < typename T > CLog & operator << (const T & os) {
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string str = [] (std::string str) {
            return str.substr(str.find(":")-2,8);
            } ((std::string)ctime(&timenow));
            
        m_file << str << " " << os << std::endl;
        std::cout << str << " " << os << std::endl;
        return * this;
    }
};

#endif
