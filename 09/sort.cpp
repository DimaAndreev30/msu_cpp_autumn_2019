#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>

#include <cstdint>
#include <cstddef>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>


class sorter
{
    std::mutex mutex_;
    int working_threads_counter_;
    std::condition_variable signal_;

    static constexpr size_t type_size_ = sizeof(uint64_t);
    size_t mem_size_;
    char* memory_;

    std::ifstream& input_;
    std::ofstream& output_;

    int temp_files_counter_;
    std::string basis_;
    std::queue<std::string> merge_queue_;

public:
    sorter(std::ifstream& input, std::ofstream& output, size_t max_mem)
        : mem_size_(max_mem)
        , memory_(new char [mem_size_])
        , input_(input), output_(output)
        , working_threads_counter_(0)
        , temp_files_counter_(0), basis_("temp_")
    {
        size_t block_size = mem_size_/2;
        block_size -= block_size%type_size_;
        auto th = std::thread([=] { thread_merge_sort(memory_, block_size); });
        thread_merge_sort(memory_ + block_size, block_size);
        th.join();
    }

    ~sorter()
    {
        std::ifstream in(merge_queue_.front(), std::ios::in | std::ios::binary);
        while (in.read(memory_, mem_size_).gcount() > 0)
        {
            output_.write(memory_, in.gcount());
        }

        delete [] memory_;
    }


private:
    void thread_merge_sort(char* mem, size_t size);

    bool sort_partition(char* mem, size_t size);
    bool merge(char* mem, size_t size);

    void create_unique_name(std::string& name);
};


void sort(std::ifstream& input, std::ofstream& output, size_t max_mem)
{
    sorter(input, output, max_mem);
}


void sorter::thread_merge_sort(char* mem, size_t size)
{
    // Первый этап - отсортировать по кускам наибольших доступных размеров
    // и рассовать их по отдельным файлам:
    while(sort_partition(mem, size))
    {}

    // Второй этап - последовательным слиянием файлов получить результат:
    while(merge(mem, size))
    {}

    signal_.notify_one();
}


bool sorter::sort_partition(char* mem, size_t size)
{
    std::unique_lock<std::mutex> lock(mutex_);

    input_.read(mem, size);
    size = input_.gcount();
    if (size == 0) return false;

    std::string name;
    create_unique_name(name);

    lock.unlock();


    auto array_begin = reinterpret_cast<uint64_t*>(mem);
    auto array_end = array_begin + size/type_size_;
    std::sort(array_begin, array_end);

    std::ofstream out(name, std::ios::out | std::ios::binary);
    out.write(mem, size);
    out.close();

    lock.lock();
    merge_queue_.push(name);

    return true;
}


bool sorter::merge(char* mem, size_t size)
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (merge_queue_.size() < 2)
    {
        if (working_threads_counter_ > 0) signal_.wait(lock);
        else return false;
    }
    working_threads_counter_++;

    std::ifstream in_a (merge_queue_.front(), std::ios::in | std::ios::binary);
    merge_queue_.pop();
    std::ifstream in_b (merge_queue_.front(), std::ios::in | std::ios::binary);
    merge_queue_.pop();

    std::string name;
    create_unique_name(name);

    lock.unlock();

    std::ofstream out (name, std::ios::out | std::ios::binary);

    size_t buff_size = (size/type_size_)/8;
    size_t left_size = (size/type_size_ - buff_size)/2, right_size = left_size;

    auto left_arr = reinterpret_cast<uint64_t*>(mem);
    auto right_arr = left_arr + left_size;
    auto buff = right_arr + right_size;

    in_a.read(reinterpret_cast<char*>(left_arr), type_size_*left_size);
    in_b.read(reinterpret_cast<char*>(right_arr), type_size_*right_size);
    left_size = in_a.gcount()/type_size_;
    right_size = in_b.gcount()/type_size_;

    size_t left = 0, right = 0, curr = 0;
    while(true)
    {
        if (left_arr[left] < right_arr[right])
        {
            buff[curr] = left_arr[left];
            curr++;
            if (++left >= left_size)
            {
                left = 0;

                in_a.read(reinterpret_cast<char*>(left_arr), type_size_*left_size);
                left_size = in_a.gcount()/type_size_;
                if (left_size == 0)
                {
                    out.write(reinterpret_cast<char*>(buff), type_size_*curr);
                    out.write(reinterpret_cast<char*>(right_arr + right), type_size_*(right_size - right));
                    while(in_b.read(mem, size).gcount() > 0)
                    { out.write(mem, in_b.gcount()); }
                    break;
                }
            }
        }
        else
        {
            buff[curr] = right_arr[right];
            curr++;
            if (++right >= right_size)
            {
                right = 0;

                in_b.read(reinterpret_cast<char*>(right_arr), type_size_*right_size);
                right_size = in_b.gcount()/type_size_;
                if (right_size == 0)
                {
                    out.write(reinterpret_cast<char*>(buff), type_size_*curr);
                    out.write(reinterpret_cast<char*>(left_arr + left), type_size_*(left_size - left));
                    while(in_a.read(mem, size).gcount() > 0)
                    { out.write(mem, in_a.gcount()); }
                    break;
                }
            }
        }

        if (curr >= buff_size)
        {
            curr = 0;
            out.write(reinterpret_cast<char*>(buff), type_size_*buff_size);
        }
    }


    out.close();
    in_b.close();
    in_a.close();

    lock.lock();
    merge_queue_.push(name);
    working_threads_counter_--;
    signal_.notify_one();
}


void sorter::create_unique_name(std::string& name)
{
    name = basis_ + std::to_string(++temp_files_counter_);
}
