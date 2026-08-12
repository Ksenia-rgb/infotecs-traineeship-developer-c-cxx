#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <queue>

namespace demo
{
  template< class T >
  class ThreadSafeQueue
  {
  public:
    ThreadSafeQueue();

    T* front();
    bool empty();
    size_t size();
    void push(const T& value);
    void push(T&& value);
    void pop();

    void forceStop();
    bool isForceStopped();
  private:
    std::queue< T > queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool force_wait_stop_;

    template< class... Args >
    void unsafeEmplaceBack(Args&&... args);
    bool unsafeEmpty();
    bool unsafeCanFront();
  };
}

template< class T >
demo::ThreadSafeQueue< T >::ThreadSafeQueue():
  queue_(),
  mutex_(),
  cond_(),
  force_wait_stop_(false)
{}

template< class T >
T* demo::ThreadSafeQueue< T >::front()
{
  std::unique_lock< std::mutex > lock(mutex_);
  cond_.wait(lock, std::bind(&ThreadSafeQueue< T >::unsafeCanFront, this));
  if (unsafeEmpty())
  {
    return nullptr;
  }
  return &queue_.front();
}

template< class T >
bool demo::ThreadSafeQueue< T >::empty()
{
  std::lock_guard< std::mutex > lock(mutex_);
  return queue_.empty();
}

template< class T >
size_t demo::ThreadSafeQueue< T >::size()
{
  std::lock_guard< std::mutex > lock(mutex_);
  return queue_.size();
}

template< class T >
void demo::ThreadSafeQueue< T >::push(const T& value)
{
  std::lock_guard< std::mutex > lock(mutex_);
  unsafeEmplaceBack(value);
  cond_.notify_one();
}

template< class T >
void demo::ThreadSafeQueue< T >::push(T&& value)
{
  std::lock_guard< std::mutex > lock(mutex_);
  unsafeEmplaceBack(std::forward< T >(value));
  cond_.notify_one();
}

template< class T >
void demo::ThreadSafeQueue< T >::pop()
{
  std::lock_guard< std::mutex > lock(mutex_);
  queue_.pop();
}

template< class T >
void demo::ThreadSafeQueue< T >::forceStop()
{
  std::lock_guard< std::mutex > lock(mutex_);
  force_wait_stop_ = true;
  cond_.notify_one();
}

template< class T >
bool demo::ThreadSafeQueue< T >::isForceStopped()
{
  std::lock_guard< std::mutex > lock(mutex_);
  return force_wait_stop_;
}

template< class T >
template< class... Args >
void demo::ThreadSafeQueue< T >::unsafeEmplaceBack(Args&&... args)
{
  queue_.emplace(std::forward< Args... >(args...));
}

template< class T >
bool demo::ThreadSafeQueue< T >::unsafeEmpty()
{
  return queue_.empty();
}

template< class T >
bool demo::ThreadSafeQueue< T >::unsafeCanFront()
{
  return !unsafeEmpty() || force_wait_stop_;
}

#endif
