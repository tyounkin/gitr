#pragma once

#if defined(CUDA)
#include "cuda_runtime.h"
#endif

#include <iostream>
#include <new>
#include <memory>
#include <cstddef>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef __CUDACC__
#define DEVICE_CALLABLE __host__ __device__
#else
#define DEVICE_CALLABLE
#endif

class Managed {
 void *operator new(size_t len) {
 void *ptr;
 cudaMallocManaged(&ptr, len);
 return ptr;
 }
 void operator delete(void *ptr) {
 cudaFree(ptr);
 }
};
 
namespace gitr {
template<typename T>
  class Vector : public Managed {

    // @todo constexpr in c++14
    T *alloc_data() {
#if defined(CUDA)
      T* data;
      auto err = cudaMallocManaged(&data, sizeof(T)*capacity_);
      if(err != cudaSuccess){
        throw std::runtime_error("error allocating managed memory");
      }
      return data;
#else
      return new T[capacity_];
#endif
    }

    // @todo constexpr in c++14
    void free_data() {
#if defined(CUDA)
      cudaFree((void*)data_);
#else
      delete[] data_;
#endif
    }

  public:
    /*! Construct an vector of fixed capacity
     */
    Vector() : capacity_{1}, size_{1}, data_{alloc_data()} {}

    Vector(const std::size_t capacity) : capacity_{capacity}, size_{capacity}, data_{alloc_data()} {}

    /*! Construct an Vector of fixed capacity and initialize values
     */
    Vector(const std::size_t capacity, T initial_value) : capacity_{capacity}, size_{capacity}, data_{alloc_data()} {
        for (std::size_t i = 0; i < size_; i++) {
        data_[i] = initial_value;
      }
    }

    Vector(const std::vector<T> initial_vector) : capacity_{initial_vector.size()}, size_{initial_vector.size()}, data_{alloc_data()} {
        for (std::size_t i = 0; i < size_; i++) {
        data_[i] = initial_vector[i];
      }
    }

    //Vector(const std::size_t capacity, T initial_value, int on) : capacity_{capacity}, size_{capacity}, data_{alloc_data()} {

    //    std::cout << "capacity " << capacity << std::endl;
    //    std::cout << "size_ " << size_ << std::endl;
    //    std::cout << "inside vector creation int " << on << std::endl;
    //    std::size_t major_size = size_;
    //    for (std::size_t i = 0; i < major_size; i++) {
    //    std::cout << "inside vector loop int "<< i << std::endl;
    //    data_[i] = initial_value;
    //  }
    //}

    /*! Destruct vector memory
     */
    ~Vector() {
      free_data();
    }

    /*! Copy constructor
     */
    Vector(const Vector &source) {
      capacity_ = source.capacity_;
      size_ = source.size_;
      data_ = alloc_data();

      if (data_) {
        for (std::size_t i = 0; i < size_; i++) {
          data_[i] = source[i];
        }
      }
    }

    Vector &operator=(const Vector &source)// = delete;
    {
        for(int i=0;i<source.size();i++)
        {
            data_[i] = source[i];
        }
        return *this;
    }

    Vector &operator=(const std::vector<T> &source)// = delete;
    {
        for(int i=0;i<source.size();i++)
        {
            data_[i] = source[i];
        }
        return *this;
    }
    Vector(Vector &&) noexcept = delete;

    Vector &operator=(Vector &&)      = delete;

    /*! Vector size getter
     * @return the number of in use elements in the Vector
     */
    DEVICE_CALLABLE
    std::size_t size() const {
      return size_;
    }

    /*! Vector capacity getter
     * @return the maximum number of elements in the Vector
     */
    std::size_t capacity() const {
      return capacity_;
    }

    /*! Vector capacity getter
     * @return the maximum number of elements in the Vector
     */
    std::size_t available() const {
      return capacity() - size();
    }

    /*! Return reference to first element of Vector
     * @return reference to first element
     */
    T &front() {
      return data_[0];
    }
    /*
    *  This function will %resize the %vector to the specified
               *         *  number of elements.  If the number is smaller than the
               *                *  %vector's current size the %vector is truncated, otherwise
               *                       *  default constructed elements are appended.
               *                              */
    void resize(const T __new_size)
    {
      if (__new_size > size())
      {
          free_data();
          capacity_ = __new_size;
          size_ = __new_size;
          data_ = alloc_data();

      }
      else if (__new_size < size())
      {

      }
    }
    /*! Add element to end of the Vector
     * Copies argument to the back of Vector and increased the size by one
     */
/*
    void push_back(const T &value) {
      if (size_ + 1 > capacity_)
        throw std::runtime_error("Not enough capacity to push_back");
      else
        data_[size_++] = value;
    }
*/
    /*! Add multiple elements of a single value to end of the Vector
     * Copies argument value push_count times and increased the size by push_count
     */
/*
    void push_back(const T &value, const size_t push_count) {
      if (size_ + 1 > capacity_)
        throw std::runtime_error("Not enough capacity to push_back");
      else {
        for (std::size_t i = 0; i < push_count; i++) {
          this->push_back(value);
        }
      }
    }
*/
    /*! Add multiple elements to end of the Vector
     * Copies elements starting at argument values_ptr to the back of Vector and increased the size by push_count
     */
/*
    void push_back(const T *values_ptr, const size_t push_count) {
      if (size_ + 1 > capacity_)
        throw std::runtime_error("Not enough capacity to push_back");
      else {
        for (std::size_t i = 0; i < push_count; i++) {
          this->push_back(values_ptr[i]);
        }
      }
    }
*/
    /*! Remove element from end of the Vector
     * Remove element from end of Vector by reducing size by 1, element is not destructed
     */
    void pop_back() {
      if (size_ == 0)
        throw std::runtime_error("Vector popped_back with 0 size");
      else
        size_--;
    }

    /*! Remove multiple elements from end of the Vector
     * Remove element from end of Vector by reducing size by 1, element is not destructed
     */
    void pop_back(std::size_t pop_count) {
      if (size_ == 0 && pop_count != 0)
        throw std::runtime_error("Vector popped_back with 0 size");
      else
        size_ -= pop_count;
    }

    /*! Getter for pointer to underlying data
     */
    DEVICE_CALLABLE
    T *data() {
      return this->data_;
    }

    /*! const getter for pointer to underlying data
     */
    DEVICE_CALLABLE
    T *data() const {
      return this->data_;
    }

    /*! Subscript operator, []
     * Retrieve reference to element using subscript notation
     */
    DEVICE_CALLABLE
    T &operator[](const std::size_t index) {
      return data_[index];
    }

    /*! const subscript operator, []
     *  Retrieve const reference to element using subscript notation
     */
    DEVICE_CALLABLE
    const T &operator[](const std::size_t index) const {
      return data_[index];
    }
/*
    DEVICE_CALLABLE
    const T* begin() {
      return this->data();
    }
    DEVICE_CALLABLE
    const T* end() {
      return this->data() + this->size();
    }
*/
    DEVICE_CALLABLE
    T *begin() const {
      return this->data();
    }

    DEVICE_CALLABLE
    T *end() const {
      return this->data() + this->size();
    }


//private:
// @todo DEVICE_CALLABLE cant use private member variables
  public:
    std::size_t capacity_;
    std::size_t size_;
    T *data_;

void print(std::string const label)
{
  std::cout << label << '\n';
  //if constexpr (std::is_floating_point<P>::value)
  //{
    for (auto i = 0; i < size_; ++i)
      std::cout << std::setw(12) << std::setprecision(4) << std::scientific
                << std::right << data_[i];
  //}
  //else
  //{
  //  for (auto i = 0; i < size(); ++i)
  //    std::cout << std::right << (*this)(i) << " ";
  //}
  std::cout << '\n';
}
  };

  /*! begin iterator for range based for loops
   */
  template<typename T>
  DEVICE_CALLABLE
  const T *begin(const Vector<T> &vector) {
    return vector.data();
  }

  /*! end iterator for range based for loops
   */
  template<typename T>
  DEVICE_CALLABLE
  const T *end(const Vector<T> &vector) {
    return vector.data() + vector.size();
  }




}
