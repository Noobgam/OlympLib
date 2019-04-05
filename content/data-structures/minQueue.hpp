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
        constexpr static size_t DEFAULT_SIZE = 1;

        std::pair<T, size_t>* _data;
        
        FunctorQueue() : FunctorQueue(DEFAULT_SIZE) {
        }

        FunctorQueue(size_t t) {
            _data = (std::pair<T, size_t>*)malloc(sizeof(std::pair<T, size_t>) * DEFAULT_SIZE);
        }

        size_t l = 0, r = 0, _b = 0, _e = 0, _size = DEFAULT_SIZE;

        void push(const T& t) {
            while (_b != _e && _back() >= t) {
                _pop_back();
            }
            _push_min(t);
            r++;
        }

        void pop() {
            if (_data[_b].second == l++) {
                _pop_min();
            }
        }

        ~FunctorQueue() { free(_data); }

        void _pop_min() {
            _b++;
            if (_b == _size) _b = 0;
        }

        T& _back() {
            size_t x = _e;
            if (x == 0) {
                x = _size;
            }
            x--;
            return _data[x].first;
        }

        void _pop_back() {
            if (_e == 0) {
                _e = _size;
            }
            _e--;
        }

        void _push_min(const T& t) {
            _data[_e++] = {t, r};
            if (_e == _size) _e = 0;
            if (_b == _e) {
                std::pair<T, size_t>* newData = (std::pair<T, size_t>*)malloc(sizeof(std::pair<T, size_t>) * 2 * _size);
                for (int i = 0; i < _size; ++i) {
                    newData[i] = std::move(_data[_b]);
                    _pop_min();
                }
                _b = 0;
                _e = _size;

                _size <<= 1;
                swap(newData, _data);
                free(newData);
            }
        }

        T& get_min() {
            return _data[_b].first;
        }
    };

    template <class T>
    using MinQueue = FunctorQueue<T, NPrivate::Minner>;
};
