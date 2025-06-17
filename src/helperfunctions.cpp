//
//  helperfunctions.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <optional>

#include <limits.h>
#include <iostream>


using namespace std;

bool vectorContains(const vector<int> &vector, const int &value) {
    if (find(vector.begin(), vector.end(), value) != vector.end()) {
        return true;
    }
    return false;
}

bool fileExists(const string &path) {
    ifstream f(path);
    return f.good();
}

uint64_t getFileSize(const string &path) {
    ifstream f(path, ios::binary | ios::ate);
    return f.tellg();
}

string getTextFromFile(const string &path) {
    ifstream ifs(path);
    return string((istreambuf_iterator<char>(ifs)),
                  (istreambuf_iterator<char>()));
}

time_t zuluToTimestamp(const string &zuluDate) {
    tm t{};
    istringstream stringStream(zuluDate);
    
    stringStream >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
    if (stringStream.fail()) {
        throw runtime_error{"failed to parse time string"};
    }
    
    return mktime(&t);
}

string joinIntVector(const vector<int> &nums, const string &delim) {
    stringstream result;
    copy(nums.begin(), nums.end(), std::ostream_iterator<int>(result, delim.c_str()));
    
    string temp = result.str();
    
    if (!temp.empty()) {
        temp.pop_back();
    }
    
    return temp;
}

time_t timestampShift(const time_t &timestamp, int shift) {
    return timestamp + (3600 * shift);
}

bool stringHasPrefix(const string &originalString, const string &prefix) {
    return originalString.rfind(prefix, 0) == 0;
}

void saveFile(const string &path, const string &contents) {
    // Перезапись файла [ВКЛ]
    cout << "[Сохранение идентификаторов отправленных объявлений]" << endl;
    ofstream ofs(path, ofstream::trunc);
    ofs << contents;
    ofs.close();
}

#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <filesystem>
    #include <libgen.h>
    #include <unistd.h>
    optional<string> getWorkingDirectory() {
        char buffer[PATH_MAX];
        uint32_t buffsize = PATH_MAX;
        
        if (_NSGetExecutablePath(buffer, &buffsize) == 0) {
            return dirname(buffer);
        }
        
        return nullopt;
    }
#elif __linux__
    #include <linux/limits.h>
    #include <libgen.h>
    #include <unistd.h>

    optional<string> getWorkingDirectory() {
        char result[PATH_MAX];
        size_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            return dirname(result);
        }
        return nullopt;
    }
#elif _WIN32
    #define NOMINMAX  // Prevent Windows.h from defining min/max macros
    #define WIN32_LEAN_AND_MEAN  // Reduce header bloat
    #include <windows.h>
    #include <libloaderapi.h>
    #include <string>
    #include <optional>

    optional<string> getWorkingDirectory() {
        char buffer[MAX_PATH];
        DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
        if (length == 0) {
            return nullopt;
        }

        char* lastSlash = strrchr(buffer, '\\');
        if (!lastSlash) {
            lastSlash = strrchr(buffer, '/');
        }

        if (lastSlash) {
            *lastSlash = '\0';
            return string(buffer);
        }
        
        return nullopt;
    }
#else
    optional<string> getWorkingDirectory() { return nullopt; }
#endif
