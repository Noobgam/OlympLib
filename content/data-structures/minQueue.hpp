#include <algorithm>
#include <utility>

namespace olymplib {
    namespace NPrivate {
        struct Minner{};
        struct Maxxer{};
    } // namespace NPrivate

    template <class T, class Functor>
    struct FunctorQueue {};

    template <class T>
    struct FunctorQueue<T, NPrivate::Minner> {
        constexpr static size_t DEFAULT_SIZE = 4;

        std::pair<T, size_t>* _data = (std::pair<int, size_t>*)malloc(sizeof(std::pair<T, size_t>) * DEFAULT_SIZE);

        size_t l = 0, r = 0, _b = 0, _e = 0, _size = DEFAULT_SIZE;

        void push(const T& t)

        ~FunctorQueue() { free(_data); }
    };

    template <class T>
    using MinQueue = FunctorQueue<T, NPrivate::Minner>;
};
