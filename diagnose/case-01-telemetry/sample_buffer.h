#ifndef CASE01_SAMPLE_BUFFER_H
#define CASE01_SAMPLE_BUFFER_H

#include <cstddef>

// Буфер останніх N зразків телеметрії. Коли буфер заповнений, найстаріший
// зразок витісняється, а нові зразки додаються в кінець.
class SampleBuffer {
public:
    explicit SampleBuffer(std::size_t capacity);
    ~SampleBuffer();

    SampleBuffer(const SampleBuffer&) = delete;
    SampleBuffer& operator=(const SampleBuffer&) = delete;

    void push(double value);

    double average() const;
    double newest() const;
    std::size_t size() const { return size_; }

private:
    double* data_;
    std::size_t capacity_;
    std::size_t size_;
};

#endif  // CASE01_SAMPLE_BUFFER_H
