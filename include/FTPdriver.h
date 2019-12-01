#pragma once

#include <boost/filesystem.hpp>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include "Broker.h"

using  boost::filesystem::all_all;


class FTPdriver {
    std::stringstream Output;
    const char Delimiter = '_';

    bool FTPformat(const std::string &filename, const std::string &path){
        size_t begin = filename.find(Delimiter);
        size_t found = filename.find_last_of(Delimiter);
        return ((begin == 7) &&
                (filename.find("balance") == 0) &&
                (path.find(".old.txt") == std::string::npos) &&
                (filename.substr(begin + 1, 8).size() == 8) &&
                (filename.substr(found + 1).size() == 8) &&
                (filename.size() == 25));
    }

    void FTPfind(const boost::filesystem::path& directory,
                 std::unordered_map<std::string, Broker> *brokers){
        for (const auto x : boost::filesystem::directory_iterator(directory)) {
            if (is_directory(x.path())) {
                FTPfind(x.path(), brokers);
            } else {
                auto file_name = x.path().stem();
                auto broker_name = x.path().parent_path().stem().string();

                if (FTPformat(file_name.string(), x.path().string())) {
                    Output << broker_name << ' '
                           << x.path().filename().string() << std::endl;

                    auto card_number = x.path().stem().string().substr(8, 8);
                    auto date =
                            std::atoi(x.path().stem().string().substr(17, 8).c_str());

                    auto it = brokers->find(broker_name);
                    if (it == brokers->end()) {
                        Broker b(broker_name);
                        b.add_account(card_number, static_cast<unsigned int>(date));
                        brokers->insert(std::make_pair(broker_name, b));
                    } else {
                        it->second.add_account(card_number, date);
                    }
                }
            }
        }
    }

    std::unordered_map<std::string, Broker> Brokers;

public:
    explicit FTPdriver(const boost::filesystem::path& path){
        FTPfind(path, &Brokers);
    }
    std::string FTPresult(){
        std::string out = Output.str();
        for (const auto& brocker : Brokers) {
            out += brocker.second.get_statistic();
        }
        return out;
    }
};
