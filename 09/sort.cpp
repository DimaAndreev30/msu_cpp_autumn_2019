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
    std::mutex input_mutex_, output_mutex_;
    int working_threads_counter_;
    std::condition_variable signal_;

    using type = uint64_t;
    static constexpr size_t type_size = sizeof(type);
    size_t mem_size_;
    type* memory_;

    std::ifstream& input_;
    std::ofstream& output_;

    int temp_files_counter_;
    std::string basis_;
    std::queue<std::string> merge_queue_;

public:
    sorter(std::ifstream& input, std::ofstream& output, size_t max_mem)
        : mem_size_(max_mem/type_size)
        , memory_(new type [mem_size_])
        , input_(input), output_(output)
        , working_threads_counter_(0)
        , temp_files_counter_(0), basis_("temp_")
    {
        // Доступную память делим поровну между потоками:
        // Вообще, есть мнение, что разумнее было дать одному потоку сортировать блоки из входного файла, а второму заниматься слиянием этих блоков.
        // Но я очень умный, поэтому оба потока занимаются один и тем же. Сначала оба сортируют блоки, потом оба их сливают.
        // В свою защиту - слияния выгоднее проводить, когда сливаемые файлы схожих размеров, чего не достичь, разделив обязанности.
        size_t block_size = mem_size_/2;
        auto th = std::thread([=] { thread_merge_sort(memory_, block_size); });
        thread_merge_sort(memory_ + block_size, block_size);
        th.join();
    }

    ~sorter()
    {
        std::ifstream in(merge_queue_.front(), std::ios::in | std::ios::binary);
        while (!in.read(reinterpret_cast<char*>(memory_), type_size*mem_size_).eof())
        {
            output_.write(reinterpret_cast<char*>(memory_), in.gcount());
        }

        delete [] memory_;
    }


private:
    void thread_merge_sort(type* mem, size_t size);

    bool sort_partition(type* mem, size_t size);
    bool merge(type* mem, size_t size);

    void create_unique_name(std::string& name);
};


void sort(std::ifstream& input, std::ofstream& output, size_t max_mem)
{
    sorter(input, output, max_mem);
}


void sorter::thread_merge_sort(type* mem, size_t size)
{
    // Первый этап - отсортировать по кускам наибольших доступных размеров
    // и рассовать их по отдельным файлам:
    while(sort_partition(mem, size))
    {}

    // Второй этап - последовательным слиянием файлов получить результат:
    // Вообще говоря, этой функции такие большие объемы памяти не нужны,
    // ведь ей нужна только память под буферы для чтения из файлов. Но пусть так.
    while(merge(mem, size))
    {}

    signal_.notify_one();
}


bool sorter::sort_partition(type* mem, size_t size)
{
    {
        std::unique_lock<std::mutex> lock(input_mutex_);

        input_.read(reinterpret_cast<char*>(mem), type_size*size);
        size = input_.gcount();
    }
    if (size == 0) return false;
    size /= type_size;

    std::string name;
    create_unique_name(name);

    std::sort(mem, mem + size);

    std::ofstream out(name, std::ios::out | std::ios::binary);
    out.write(reinterpret_cast<char*>(mem), type_size*size);
    out.close();

    {
        std::unique_lock<std::mutex> lock(output_mutex_);
        merge_queue_.push(name);
    }

    return true;
}


bool sorter::merge(type* mem, size_t size)
{
    std::unique_lock<std::mutex> lock(output_mutex_);
    // Ожидаем прихода в очередь двух файлов на слияние:
    while (merge_queue_.size() < 2)
    {
        // Надеяться на появление новых файлов в очереди имеет смысл
        // только до тех пор, пока какие-то потоки не завершат свою работу:
        if (working_threads_counter_ > 0) signal_.wait(lock);
        else return false;
    }
    working_threads_counter_++;

    std::ifstream in_a (merge_queue_.front(), std::ios::in | std::ios::binary);
    merge_queue_.pop();
    std::ifstream in_b (merge_queue_.front(), std::ios::in | std::ios::binary);
    merge_queue_.pop();

    lock.unlock();


    std::string name;
    create_unique_name(name);
    std::ofstream out (name, std::ios::out | std::ios::binary);

    size_t buff_size = size/2;
    size_t left_size = (size - buff_size)/2, right_size = left_size;

    type* left_arr = mem;
    type* right_arr = left_arr + left_size;
    type* buff = right_arr + right_size;

    in_a.read(reinterpret_cast<char*>(left_arr), type_size*left_size);
    in_b.read(reinterpret_cast<char*>(right_arr), type_size*right_size);
    left_size = in_a.gcount()/type_size;
    right_size = in_b.gcount()/type_size;

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

                in_a.read(reinterpret_cast<char*>(left_arr), type_size*left_size);
                left_size = in_a.gcount()/type_size;
                if (left_size == 0)
                {
                    out.write(reinterpret_cast<char*>(buff), type_size*curr);
                    out.write(reinterpret_cast<char*>(right_arr + right), type_size*(right_size - right));
                    while(!in_b.read(reinterpret_cast<char*>(buff), type_size*buff_size).eof())
                    { out.write(reinterpret_cast<char*>(buff), in_b.gcount()); }
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

                in_b.read(reinterpret_cast<char*>(right_arr), type_size*right_size);
                right_size = in_b.gcount()/type_size;
                if (right_size == 0)
                {
                    out.write(reinterpret_cast<char*>(buff), type_size*curr);
                    out.write(reinterpret_cast<char*>(left_arr + left), type_size*(left_size - left));
                    while(!in_a.read(reinterpret_cast<char*>(buff), type_size*buff_size).eof())
                    { out.write(reinterpret_cast<char*>(buff), in_a.gcount()); }
                    break;
                }
            }
        }

        if (curr >= buff_size)
        {
            curr = 0;
            out.write(reinterpret_cast<char*>(buff), type_size*buff_size);
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
    std::unique_lock<std::mutex> lock(output_mutex_);
    name = basis_ + std::to_string(++temp_files_counter_);
}
