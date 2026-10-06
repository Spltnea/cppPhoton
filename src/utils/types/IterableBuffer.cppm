module;
export module IterableBuffer;

import std;

namespace photon {

    /// @brief A buffer whose contents can easily be iterated over, provides methods for secure and easy content pushing and popping
    /// @tparam T The data type stored in this buffer
    export template<class T>
    class iterableBuffer {
    private:
        std::vector<T> data;
        std::size_t idx{0};

    public:
        // === Buffer's information gathering ===

        /// @brief Returns the size of the buffer
        [[nodiscard]] std::size_t size() const { return data.size(); }

        /// @brief Asserts that the buffer is empty
        [[nodiscard]] bool isEmpty() const { return data.empty(); }

        /// @brief Returns the position of the index cursor
        [[nodiscard]] std::size_t cursorPosition() const { return idx; }

        /// @brief Returns the capacity of the buffer memory wise
        [[nodiscard]] std::size_t capacity() const { return data.capacity(); }

        /// @brief Defines the minimum size of the buffer, allocates storage if necessary
        /// @param size The minimum size to define
        void reserve(std::size_t size) { data.reserve(size); }
            
        // === Safety methods ===

        /// @brief Asserts that the index cursor has reached the end of the buffer
        [[nodiscard]] bool isAtEnd() const { return idx >= size(); }

        /// @brief Asserts that the given offset relative to the current index cursor is valid (e.g : Does not go out of bounds)
        /// @param offset The offset to test (idx + offset)
        [[nodiscard]] bool isOffsetValid(std::size_t offset) const {
            return (isAtEnd()
                ? false
                : offset < (data.size() - idx));
        }

        /// @brief Asserts that the given offset is suitable for a cursor shift
        /// @param offset The offset to test (idx + offset)
        [[nodiscard]] bool canAdvanceAtOffset(std::size_t offset) const {
            return offset <= (data.size() - idx); 
        }

        // === Iterator methods ===

        /// @brief Advances the index cursor by an offset given it's valid (does not consume anything)
        /// @param offset The offset (default : 1)
        void advance(std::size_t offset = 1) {
            if (!canAdvanceAtOffset(offset)) { throw std::out_of_range("The given offset [" + std::to_string(offset) + "] goes out of bounds"); }
            idx += offset;
        }

        // === Data manipulation methods ===

        /// @brief Adds an element to the buffer
        /// @param element The element to add
        void addElement(const T element) { data.push_back(element); }

        /// @brief Appends elements from an iterator range to the end of the buffer
        /// @tparam TIn The iterator type
        /// @param first The beginning of the range of elements to append
        /// @param last The end of the range of elements to append
        template<typename TIn>
        void append(TIn first, TIn last) { data.insert(data.end(), first, last); }
        
        /// @brief Appends elements from a range or container to the end of the buffer, reserving capacity if known
        /// @tparam R A range type satisfying std::ranges::input_range
        /// @param range The range or container whose elements are to be appended
        template<std::ranges::input_range R>
        void append(R&& range) {
            if constexpr (std::ranges::sized_range<R>) { data.reserve(data.size() + std::ranges::size(range)); }
            data.insert(data.end(), std::ranges::begin(range), std::ranges::end(range));
        }

        /// @brief Adds an element to the buffer (pass by reference)
        /// @param element The element to add
        void addElementRef(const T& element) { data.push_back(element); }

        /// @brief Removes the last element in the buffer
        void removeLastElement() { data.pop_back(); }

        /// @brief Clears the buffer
        void clear() { data.clear(); idx = 0; }

        // === Lookup methods ===

        /// @brief Returns the current element pointed by the index cursor
        [[nodiscard]] const T& currentElement() const { return data[idx]; }
        
        /// @brief Returns a pointer to the current element in the buffer
        [[nodiscard]] const T* currentElementPointer() const { return data.data() + idx; }

        /// @brief Returns a pointer to the end of the buffer 
        [[nodiscard]] const T* endOfBufferPointer() const { return data.data() + data.size(); }

        /// @brief Returns a pointer to the last element in the buffer 
        [[nodiscard]] const T* lastElementPointer() const { return data.data() + data.size() - 1; }

        /// @brief Returns the element pointed after the current cursor (does not increment the cursor), throws an exception if the next element leads to out of bounds positions
        [[nodiscard]] const T& elementAhead() const {
            return (!(idx + 1 >= data.size())
                ? data[idx + 1]
                : throw std::out_of_range("Index goes out of bounds"));
        }

        /// @brief Returns the element pointed after an offset relative to the current cursor (does not increment the cursor), throws an exception if the offset leads to out of bounds positions
        /// @param offset The offset to look at
        [[nodiscard]] const T& elementAfter(std::size_t offset = 1) const { 
            return (isOffsetValid(offset) 
                ?  data[idx + offset]
                :  throw std::out_of_range("The given offset [" + std::to_string(offset) + "] goes out of bounds"));
        }

        /// @brief Increments the cursor and returns the data afterwards, throws an exception if the increment leads to out of bounds positions
        [[nodiscard]] const T& advanceAndFetch() {
            return (!(idx + 1 >= data.size()) 
                ? data[++idx]
                : throw std::out_of_range("Index goes out of bounds"));
        }

        /// @brief Returns the current element pointed by the index cursor (does increment the cursor afterwards), throws an exception if the increment leads to out of bounds positions
        [[nodiscard]] const T& fetchAndAdvance() { 
            return (!isAtEnd() 
                ? data[idx++]
                : throw std::out_of_range("Index goes out of bounds"));
        }

        

        // === Constructors ===

        /// @brief Creates a new empty buffer
        iterableBuffer() {}

        /// @brief Creates a new buffer by copying an old buffer (Copy Constructor)
        /// @param buffer The buffer to copy its data from
        iterableBuffer(const iterableBuffer &oldBuffer) : data(oldBuffer.data), idx(oldBuffer.idx) {}

        /// @brief Creates a new buffer by moving the date out of an old buffer (Move Constructor)
        /// @param buffer The buffer to move its data from
        iterableBuffer(iterableBuffer &&other) : data(std::move(other.data)), idx(std::exchange(other.idx, 0)) {}

        /// @brief Creates a new buffer with already existing elements (Initializer Constructor)
        /// @param values The values to place inside the buffer
        iterableBuffer(std::initializer_list<T> values) : data(values) {}

        // === Operators overloading ===

        /// @brief Move assignment operator, transfers ownership of another buffer's data and resets its cursor
        /// @param other The buffer to move from
        /// @return A reference to this buffer
        iterableBuffer& operator=(iterableBuffer &&other) {
            if (this != &other) {
                data = std::move(other.data);
                idx = std::exchange(other.idx, 0);
            }
            return *this;
        }

        /// @brief Checks whether there are still unconsumed elements available in the buffer
        /// @return True if the cursor has not reached the end, false otherwise
        [[nodiscard]] explicit operator bool() const { return !isAtEnd(); }

        // === Range based "for" support methods ===

        /// @brief Returns a lightweight view over the remaining (unread) elements
        [[nodiscard]] std::span<const T> remaining() const noexcept {
            const std::size_t safe_idx = std::min(idx, data.size());
            return std::span<const T>(data).subspan(safe_idx);
        }

        /// @brief Returns a mutable view over the remaining (unread) elements
        [[nodiscard]] std::span<T> remaining() noexcept {
            const std::size_t safe_idx = std::min(idx, data.size());
            return std::span<T>(data).subspan(safe_idx);
        }

        /// @brief Returns a lightweight view over all elements from index 0 to the end
        [[nodiscard]] std::span<const T> all() const noexcept {
            return data;
        }

        /// @brief Returns a mutable view over all elements from index 0 to the end
        [[nodiscard]] std::span<T> all() noexcept {
            return data;
        }

        // === Default Iterator Methods (Defaults to 'remaining') ===

        [[nodiscard]] auto begin() noexcept { return data.begin() + std::min(idx, data.size()); }
        [[nodiscard]] auto begin() const noexcept { return data.begin() + std::min(idx, data.size()); }
        [[nodiscard]] auto cbegin() const noexcept { return data.cbegin() + std::min(idx, data.size()); }

        [[nodiscard]] auto end() noexcept { return data.end(); }
        [[nodiscard]] auto end() const noexcept { return data.end(); }
        [[nodiscard]] auto cend() const noexcept { return data.cend(); }
    };
}