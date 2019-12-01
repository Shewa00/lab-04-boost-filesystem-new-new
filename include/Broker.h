#pragma once

#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>

class Broker {
    struct Account {
        std::string Card_number;
        unsigned int Last_date;
        size_t Lots_files;
    };
    std::string Name;
    std::unordered_map<std::string, Account> Accounts;
public:
    explicit Broker(const std::string& name) : Name(name)
    {}

    void add_account(const std::string& card_number, unsigned int date)
    {
        auto it = Accounts.find(card_number);
        if (it == Accounts.end()) {
            Accounts[card_number].Card_number = card_number;
            Accounts[card_number].Last_date = date;
        } else {
            it->second.Lots_files += 1;
            if (date > it->second.Last_date)
                it->second.Last_date = date;
        }
    }

    std::string get_statistic() const noexcept {
        std::stringstream Output_;

        Output_.str(std::string());
        for (const auto &a : Accounts) {
            Output_ << "Broker:" << Name << ' '
                    << "Account:" << a.first << ' '
                    << "Files:" << a.second.Lots_files << ' '
                    << "Last date:" << a.second.Last_date << std::endl;
        }
        return Output_.str();
    }
};
