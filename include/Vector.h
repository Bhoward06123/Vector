#pragma once

#include <iostream>

template<typename T>
class Vector
{
public:

    Vector() noexcept = default;

    explicit Vector(std::size_t size) : m_Data(new T[size]), m_Size(size), m_Capacity(size) {}

    Vector(const Vector& other) : m_Data(new T[other.m_Size]), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
    {
        std::memcpy(m_Data, other.m_Data, m_Size * sizeof(T));
    }

    Vector& operator=(const Vector& other)
    {
        if(this != &other)
        {
            T* newData = new T[other.m_Size];
            std::memcpy(newData, other.m_Data, other.m_Size * sizeof(T));
            delete[] m_Data;

            m_Data = newData;
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;
        }

        return *this;
    }

    Vector(Vector&& other) noexcept : m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
    {
        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
    }

    Vector& operator=(Vector && other) noexcept
    {
        if(this != &other)
        {
            delete[] m_Data;
            m_Data = other.m_Data;
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;

            other.m_Data = nullptr;
            other.m_Size = 0;
            other.m_Capacity = 0;
        }

        return *this;
    }

    ~Vector()
    {
        delete[] m_Data;
    }

public:

    T* data() noexcept
    {
        return m_Data;
    }

    const T* data() const noexcept
    {
        return m_Data;
    }

    std::size_t size() const noexcept
    {
        return m_Size;
    }

    std::size_t capacity() const noexcept
    {
        return m_Capacity;
    }

    void reserve(std::size_t newCapacity)
    {
        if(newCapacity > m_Capacity)
        {
            reallocate(newCapacity);
        }
    }

    void resize(std::size_t newSize)
    {
        if(newSize > m_Size)
        {
            if(newSize > m_Capacity)
            {
                reallocate(newSize);
            }

            for(std::size_t i = m_Size; i < newSize; i++)
            {
                m_Data[i] = T();
            }
        }

        m_Size = newSize;
    }

    void resize(std::size_t newSize, const T& value)
    {
        if(newSize > m_Size)
        {
            if(newSize > m_Capacity)
            {
                reallocate(newSize);
            }

            for(std::size_t i = m_Size; i < newSize; i++)
            {
                m_Data[i] = value;
            }
        }

        m_Size = newSize;
    }

    void shrink_to_fit()
    {
        if(m_Size < m_Capacity)
        {
            reallocate(m_Size);
        }
    }

    void push_back(const T& value)
    {
        if(m_Size >= m_Capacity)
        {
            reallocate(m_Capacity + m_Capacity / 2 + 1);
        }

        m_Data[m_Size] = value;
        m_Size++;
    }

    void push_back(T&& value)
    {
        if(m_Size >= m_Capacity)
        {
            reallocate(m_Capacity + m_Capacity / 2 + 1);
        }

        m_Data[m_Size] = std::move(value);
        m_Size++;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if(m_Size >= m_Capacity)
        {
            reallocate(m_Capacity + m_Capacity / 2 + 1);
        }

        new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
        m_Size++;
    }

    void pop_back() noexcept
    {
        if(m_Size > 0)
        {
            m_Size--;
            m_Data[m_Size].~T();
        }
    }

    void insert(std::size_t index, const T& value)
    {
        if(index > m_Size)
        {
            throw std::out_of_range("Index out of range");
        }

        if(m_Size >= m_Capacity)
        {
            reallocate(m_Capacity + m_Capacity / 2 + 1);
        }

        for(std::size_t i = m_Size; i > index; i--)
        {
            m_Data[i] = std::move(m_Data[i - 1]);
        }

        m_Data[index] = value;
        m_Size++;
    }

    void insert(std::size_t index, T&& value)
    {
        if(index > m_Size)
        {
            throw std::out_of_range("Index out of range");
        }

        if(m_Size >= m_Capacity)
        {
            reallocate(m_Capacity + m_Capacity / 2 + 1);
        }

        for(std::size_t i = m_Size; i > index; i--)
        {
            m_Data[i] = std::move(m_Data[i - 1]);
        }

        m_Data[index] = std::move(value);
        m_Size++;
    }

    template<typename... Args>
    void emplace(std::size_t index, Args&&... args)
    {
        if(index > m_Size)
        {
            throw std::out_of_range("Index out of range");
        }

        if(m_Size >= m_Capacity)
        {
            reallocate(m_Capacity + m_Capacity / 2 + 1);
        }

        for(std::size_t i = m_Size; i > index; i--)
        {
            m_Data[i] = std::move(m_Data[i - 1]);
        }

        new(&m_Data[index]) T(std::forward<Args>(args)...);
        m_Size++;
    }

    void erase(std::size_t index)
    {
        if(index >= m_Size)
        {
            throw std::out_of_range("Index out of range");
        }

        m_Data[index].~T();

        for(std::size_t i = index; i < m_Size - 1; i++)
        {
            m_Data[i] = std::move(m_Data[i + 1]);
        }

        m_Size--;
    }

    void clear() noexcept
    {
        for(std::size_t i = 0; i < m_Size; i++)
        {
            m_Data[i].~T();
        }

        m_Size = 0;
    }

    T& operator[](std::size_t index)
    {
        if(index >= m_Size)
        {
            throw std::out_of_range("Index out of range");
        }

        return m_Data[index];
    }

    const T& operator[](std::size_t index) const
    {
        if(index >= m_Size)
        {
            throw std::out_of_range("Index out of range");
        }

        return m_Data[index];
    }

private:

    void reallocate(std::size_t newCapacity)
    {
        T* newData = new T[newCapacity];
        std::memcpy(newData, m_Data, m_Size * sizeof(T));
        delete[] m_Data;

        m_Data = newData;
        m_Capacity = newCapacity;
    }

private:

    T* m_Data = nullptr;
    std::size_t m_Size = 0;
    std::size_t m_Capacity = 0;

};
