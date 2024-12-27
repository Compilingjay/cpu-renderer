#include "string_utils.h"

std::vector<std::string> split(std::string string, const std::string& delimitter, int times_to_split) {
    std::vector<std::string> split_string;
    std::string::size_type i;

    while (times_to_split != 0) {
        if ((i = string.find(delimitter)) == std::string::npos) {
            break;
        }

        split_string.push_back(string.substr(0, i));
        string.erase(0, i+delimitter.size());
        --times_to_split;
    }
    
    split_string.push_back(string);
    return split_string;
}

std::vector<std::string> split(std::string string, const std::initializer_list<std::string>& delimitters, int times_to_split) {
    std::vector<std::string> split_string;
    std::string::size_type closest = string.length();
    std::string::size_type curr_idx;
    std::string_view curr_delim;
    bool end = false;

    while (!end && times_to_split != 0) {
        end = true;
        for (const std::string& d : delimitters) {
            curr_idx = string.find(d);
            if (curr_idx != std::string::npos && curr_idx < closest) {
                closest = curr_idx;
                curr_delim = d;
                end = false;
            }
        }
        if (end) {
            break;
        }

        split_string.push_back(string.substr(0, closest));
        string.erase(0, closest+curr_delim.size());
        closest = string.size();
        --times_to_split;
    }
    
    split_string.push_back(string);
    return split_string;
}

std::vector<std::string> split(std::string string, const std::vector<std::string>& delimitters, int times_to_split) {
    std::vector<std::string> split_string;
    std::string::size_type closest = string.length();
    std::string::size_type curr_idx;
    std::string_view curr_delim;
    bool end = false;

    while (!end && times_to_split != 0) {
        end = true;
        for (const std::string& d : delimitters) {
            curr_idx = string.find(d);
            if (curr_idx != std::string::npos && curr_idx < closest) {
                closest = curr_idx;
                curr_delim = d;
                end = false;
            }
        }
        if (end) {
            break;
        }

        split_string.push_back(string.substr(0, closest));
        string.erase(0, closest+curr_delim.size());
        closest = string.size();
        --times_to_split;
    }
    
    split_string.push_back(string);
    return split_string;
}