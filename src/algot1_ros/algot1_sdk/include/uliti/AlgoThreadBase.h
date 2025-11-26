/**
 * @file: AlgoThreadBase.h
 * @brief: This header file contains the definition of AlgoThreadBase.
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @note This file is created by Algo1010 Technology Co.,Ltd. All rights reserved.
 *
 */

#ifndef _ALGO_THREAD_BASE_H_
#define _ALGO_THREAD_BASE_H_

#include <iostream>
#include <thread>
#include <atomic>

namespace Algo1010
{

    /**
     * @class AlgoThreadBase
     * @brief A base class for creating threads in C++.
     */
    class AlgoThreadBase
    {
    public:
        /**
         * @brief Default constructor for AlgoThreadBase.
         */
        AlgoThreadBase() : isRunning_(false) {}

        /**
         * @brief Constructor for AlgoThreadBase.
         * @param obj A pointer to an object that will be associated with this thread.
         */
        AlgoThreadBase(void *obj) : isRunning_(false), object_(obj) {}

        /**
         * @brief Virtual destructor.
         */
        virtual ~AlgoThreadBase() {}

        /**
         * @brief Start running the thread.
         */
        void start()
        {
            if (!isRunning_.exchange(true))
            {
                thread_ = std::thread(&AlgoThreadBase::run, this);
            }
        }

        /**
         * @brief Stop the thread.
         */
        void stop()
        {
            isRunning_ = false;
        }

        /**
         * @brief Detach the thread.
         */
        void detach()
        {
            thread_.detach();
        }

        /**
         * @brief Wait for the thread to finish.
         */
        void join()
        {
            if (thread_.joinable())
            {
                thread_.join();
            }
        }

        /**
         * @brief Sleep for a specified number of milliseconds.
         * @param ms The number of milliseconds to sleep for.
         */
        static void sleep(int ms)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

    protected:
        /**
         * @brief Pure virtual function that needs to be implemented in derived classes.
         */
        virtual void run() = 0;

    protected:
        std::thread thread_; /**< The underlying thread object. */
        std::atomic<bool> isRunning_; /**< Flag indicating whether the thread is running or not. */
        void *object_;                /**< A pointer to an associated object (optional). */
    };

} //! Algo1010

#endif //!_THREAD_BASE_H_