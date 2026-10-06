// AI Generated

module;

export module ASTContext;
import std;

export namespace photon {

    /// @brief Provides an arena to store all the created nodes
    class ASTContext {
    private:
        std::pmr::monotonic_buffer_resource arena;

    public:
        ASTContext(std::size_t initialSize = 64 * 1024) : arena(initialSize, std::pmr::get_default_resource()) {}

        /// @brief Creates a node of any type
        /// @tparam T The type of the node
        /// @param ...args The node arguments
        template<typename T, typename... Args>
        T* createNode(Args&&... args) {
            void* memory = arena.allocate(sizeof(T), alignof(T));
            return new (memory) T(std::forward<Args>(args)...);
        }

        /// @brief Allocates a contiguous array of elements in the arena
        /// @tparam T The type of the elements in the array
        /// @param size The number of elements to allocate
        template<typename T>
        std::span<T> allocateArray(std::size_t size) {
            if (size == 0) return std::span<T>();
            
            void* memory = arena.allocate(sizeof(T) * size, alignof(T));
            T* array = new (memory) T[size](); 
            return std::span<T>(array, size);
        }

        /// @brief Provides raw memory resource access 
        std::pmr::memory_resource* getResource() { return &arena; }

        /// @brief Frees the memory space of the context without destroying it
        void reset() { arena.release(); }

        ASTContext(const ASTContext&) = delete;
        ASTContext& operator =(const ASTContext&) = delete;
    };
}