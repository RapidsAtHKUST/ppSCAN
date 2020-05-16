#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <cstring>
#include <cstdlib>
#include <cstdint>

#include <string>
#include <iomanip>
#include <locale>
#include <sstream>
#include <memory>
#include <array>

#include "util/log/log.h"

using namespace std;

template<class T>
std::string FormatWithCommas(T value) {
//    std::stringstream ss;
//    ss.imbue(std::locale(""));
//    ss << std::fixed << value;
//    return ss.str();
    string numWithCommas = to_string(value);
    int insertPosition = numWithCommas.length() - 3;
    while (insertPosition > 0) {
        numWithCommas.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numWithCommas;
}

//inline void SetThreadSelfAffinity(int core_id) {
//#ifdef NOT_ENOUGH
//    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
//    assert(core_id >= 0 && core_id < num_cores);
//    if (core_id == 0) {
//        printf("affinity relevant logical cores: %ld\n", num_cores);
//    }
//    core_id = core_id % num_cores;
//#endif
//    cpu_set_t cpuset;
//    CPU_ZERO(&cpuset);
//    CPU_SET(core_id, &cpuset);
//
//    pthread_t current_thread = pthread_self();
//    int ret = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
//    if (ret != 0) {
//        log_error("Set affinity error: %d, %s", ret, strerror(errno));
//    }
//}

inline void reset(std::stringstream &stream) {
    const static std::stringstream initial;

    stream.str(std::string());
    stream.clear();
    stream.copyfmt(initial);
}


using namespace std;

inline size_t file_size(const char *file_name) {
    struct stat st;
    stat(file_name, &st);
    size_t size = st.st_size;
    return size;
}

inline std::string exec(const char *cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}
