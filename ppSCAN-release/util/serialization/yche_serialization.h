#pragma once

#include <cstdio>
#include <tuple>
#include <vector>

using namespace std;

class YcheSerializer {

public:
    // 1st: for sling 4-tuple serialization and deserialization
    template<class A, class B, class C, class D>
    void write_tuple_vec(FILE *fp, const std::vector<std::tuple<A, B, C, D>> &value) {
        size_t size = value.size();
        fwrite(&size, sizeof(size_t), 1, fp);
        if (size == 0) { return; }
        fwrite((const void *) &value.front(), sizeof(std::tuple<A, B, C, D>), size, fp);
    };

    template<class A, class B, class C, class D>
    void read_tuple_vec(FILE *fp, std::vector<std::tuple<A, B, C, D>> &value) {
        size_t size = 0;
        fread(&size, sizeof(size_t), 1, fp);
        if (size == 0) { return; }
        value.resize(size);
        fread((void *) &value.front(), sizeof(std::tuple<A, B, C, D>), size, fp);
    };

    template<class A>
    void write_vec(FILE *fp, const std::vector<A> &value) {
        size_t size = value.size();
        fwrite(&size, sizeof(size_t), 1, fp);
        if (size == 0) { return; }
        fwrite((const void *) &value.front(), sizeof(A), size, fp);
    };

    template<class A>
    void read_vec(FILE *fp, std::vector<A> &value) {
        size_t size = 0;
        fread(&size, sizeof(size_t), 1, fp);
        if (size == 0) { return; }
        value.resize(size);
        fread((void *) &value.front(), sizeof(A), size, fp);
    };

    // 2nd: write array
    template<class T>
    void write_array(FILE *fp, const T *my_arr, size_t arr_size) {
        fwrite(&arr_size, sizeof(size_t), 1, fp);
        if (arr_size == 0) { return; }
        fwrite((const void *) my_arr, sizeof(T), arr_size, fp);
    }

    // use carefully, due to a lack of RAII (resource allocation is initialization)
    template<class T>
    void read_array_into_ref(FILE *fp, T *&my_arr, size_t &actual_size) {
        size_t arr_size;
        fread(&arr_size, sizeof(size_t), 1, fp);
        actual_size = arr_size;
        if (arr_size == 0) { return; }
        my_arr = new T[arr_size];
//        cout << sizeof(T) << endl;
        fread((void *) my_arr, sizeof(T), arr_size, fp);
    }

    template<class T>
    void read_array(FILE *fp, vector<T> &my_arr) {
//        static_assert(!is_same<T, bool>::value);
        size_t arr_size;
        fread(&arr_size, sizeof(size_t), 1, fp);
        if (arr_size == 0) { return; }
        my_arr.resize(arr_size);
        fread((void *) &my_arr.front(), sizeof(T), arr_size, fp);
    }
};
