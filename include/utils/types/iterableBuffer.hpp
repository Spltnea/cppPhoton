#ifndef PHOTON_ITERABLE_BUFFER_HPP
#   define PHOTON_ITERABLE_BUFFER_HPP

#include <vector>
#include <cstddef>
#include <string>

namespace photon {
    /** A buffer which has an attached iterator
     * 
     *  Provides methods for iterating through the buffer and getting elements out of it
     * @tparam T The type of element stored in that buffer
     */
    template<typename T>
    class iterableBuffer {
    private:
        /// @brief The data stored inside the buffer
        std::vector<T> _data;

        /// @brief The index iterator to access the data at any place
        size_t _idx{0};

        /** 
         * Asserts that the set offset is valid relative to the current index cursor position of the buffer
         * @param offset The desired offset
        */
        [[nodiscard]] bool isValidOffset(int offset) const {
            std::ptrdiff_t target = static_cast<std::ptrdiff_t>(_idx) + offset;
            return target >= 0 && static_cast<size_t>(target) < _data.size();
        }

    public:
        /// Makes an empty iterable buffer
        iterableBuffer() = default;

        explicit iterableBuffer(std::vector<T> data) : _data(std::move(data)) {}

        /**
         * Sets the buffer contents and the index cursor to zero
         * @param data A vector containing data
         */
        void setBuffer(std::vector<T> data) {
            _data = std::move(data);
            _idx = 0;
        }

        /**
         * Empties the buffer and reinits the cursor to zero
         */
        void resetBuffer() {
            _data.clear();
            _idx = 0;
        }

        /**
         * Add an element to the buffer
         * @param element The element to add
         */
        void addElement(const T& element) {
            _data.push_back(element);
        }

        /**
         * Add an element to the buffer
         * @param element The element to add
         */
        void addElement(T&& element) {
            _data.push_back(std::move(element));
        }

        /**
         * Removes the last element in the buffer
         */
        void removeLastElement() {
            if (_data.empty()) return;
            
            _data.pop_back();

            if (_idx > _data.size()) {
                _idx = _data.size();
            }
        }

        /**
         * Asserts if the index cursor is at the end of the stream
         */
        [[nodiscard]] bool isAtEnd() const {
            return _idx >= _data.size();
        }

        /**
         * Returns the size of the buffer
         */
        [[nodiscard]] size_t size() const { return _data.size(); }

        /**
         * Returns the index cursor position
         */
        [[nodiscard]] size_t position() const { return _idx; }

        /**
         * Asserts that the buffer is empty
         */
        [[nodiscard]] bool isEmpty() const {
            return _data.empty();
        }

        /**
         * Returns the current element without advancing the index cursor
         */
        [[nodiscard]] const T& currentElement() const {
            if (isAtEnd()) {
                static const T default_value{};
                return default_value;
            }
            return _data[_idx];
        }

        /**
         * Returns the current element and advances the index cursor
         */
        T advance() {
            if (isAtEnd()) return T{};
            return _data[_idx++];
        }

        /**
         * Fetches the element at an offset relative to the current index cursor
         * @param offset The offset to look at
         */
        [[nodiscard]] bool isValidOffset(int offset) const {
            if (offset < 0) {
                auto abs_offset = static_cast<size_t>(-static_cast<long long>(offset));
                return _idx >= abs_offset;
            }
            auto pos_offset = static_cast<size_t>(offset);
            return (_data.size() - _idx) > pos_offset;
        }
        
        auto begin()        { return _data.begin(); }
        auto end()          { return _data.end(); }
        auto begin()  const { return _data.begin(); }
        auto end()    const { return _data.end(); }
        auto cbegin() const { return _data.cbegin(); }
        auto cend()   const { return _data.cend(); }

    }; // class iteratableBuffer
} // namespace photon

#endif