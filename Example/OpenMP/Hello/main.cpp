#include <iostream>
#include <kvs/OpenMP>

int main()
{
    int thread_id = 0;

    KVS_OMP_PARALLEL( private(thread_id) )
    {
        thread_id = kvs::OpenMP::GetThreadNumber();

        KVS_OMP_MASTER
        {
            int nthreads = kvs::OpenMP::GetNumberOfThreads();
            int nprocessors = kvs::OpenMP::GetNumberOfProcessors();
            int max_threads = kvs::OpenMP::GetMaxThreads();
            bool in_parallel = kvs::OpenMP::InParallel();
            bool dynamic = kvs::OpenMP::GetDynamic();
            bool nested = kvs::OpenMP::GetNested();

            std::cout << "Hello World from master thread." << std::endl;
            std::cout << "    Thread ID: " << thread_id << std::endl;
            std::cout << "    Number of processors: " << nprocessors << std::endl;
            std::cout << "    Number of threads: " << nthreads << std::endl;
            std::cout << "    Max threads: " << max_threads << std::endl;
            std::cout << "    In parallel: " << std::boolalpha << in_parallel << std::endl;
            std::cout << "    Dynamic threads enabled: " << std::boolalpha << dynamic << std::endl;
            std::cout << "    Nested parallelism enabled: " << std::boolalpha << nested << std::endl;
        }
    }

    return 0;
}
