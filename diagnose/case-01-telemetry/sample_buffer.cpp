#include "sample_buffer.h"

SampleBuffer::SampleBuffer(std::size_t capacity)
    : data_(new double[capacity]), capacity_(capacity), size_(0) {}

SampleBuffer::~SampleBuffer() {
    delete[] data_;
}

void SampleBuffer::push(double value) {
    if (size_ == capacity_) {
        // Буфер заповнений: зсуваємо всі зразки на одну позицію вліво,
        // найстаріший витісняється.
        for (std::size_t i = 0; i + 1 < size_; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
    }
    data_[size_++] = value;
}

double SampleBuffer::average() const {
    if (size_ == 0) return 0.0;
    double sum = 0.0;
    for (std::size_t i = 0; i < size_; ++i) {
        sum += data_[i];
    }
    return sum / static_cast<double>(size_);
}

double SampleBuffer::newest() const {
    return size_ == 0 ? 0.0 : data_[size_ - 1];
}
