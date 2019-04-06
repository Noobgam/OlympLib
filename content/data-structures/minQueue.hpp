#include <algorithm>
#include <utility>
#include <vector>

#ifndef OLYMPLIB_FUNCTOR_QUEUE
#define OLYMPLIB_FUNCTOR_QUEUE

namespace olymplib {
    namespace NPrivate {
        struct Minner{};
        struct Maxxer{};
    } // namespace NPrivate

    template <class T, class Functor>
    struct FunctorQueue {
        Functor f;
    };

    template <class T>
    struct FunctorQueue<T, NPrivate::Minner> {
        std::pair<T, T>* front = NULL, *back = NULL;
        size_t fsize = 0, bsize = 0;
        size_t fcap = 0, bcap = 0;
        std::pair<T, T>* fhead = NULL, *bhead = NULL;
        
        FunctorQueue() : FunctorQueue(1) {}

        FunctorQueue(size_t t) {
            reserve(t);
        }

        void reserve(size_t t) {
            if (fcap < t) {
                reserve(front, fcap, fhead, t, fsize);
            }
            if (bcap < t) {
                reserve(back, bcap, bhead, t, bsize);
            }
        }

        void reserve(std::pair<T,T>*& ref, size_t& cap, std::pair<T,T>*& head, size_t t, size_t sz) {
            ref = (std::pair<T,T>*) realloc(ref, t * sizeof(*ref));
            cap = t;
            head = ref + sz - 1;
        }
        
        ~FunctorQueue() { free(front); free(back); }

        bool empty() const {
            return fsize == 0 && bsize == 0;
        }

        size_t size() const {
            return fsize + bsize;
        }

        void push_back(const T& t) {
            if (bsize == 0) *(bhead + 1) = {t,t};
            else {
                if (bsize == bcap) {
                    reserve(back, bcap, bhead, bcap * 2, bsize);
                }
                *(bhead + 1) = {t, std::min(bhead->second, t)};
            }
            ++bsize;
            ++bhead;
        }

        void push_front(const T& t) {
            if (fsize == 0) *(fhead + 1) = {t,t};
            else {
                if (fsize == fcap) {
                    reserve(front, fcap, fhead, fcap * 2, fsize);
                }
                *(fhead + 1) = {t, std::min(fhead->second, t)};
            }
            ++fsize;
            ++fhead;
        }

        inline void push(const T& t) { return push_back(t); }

        inline void swap_ptrs() {
                swap(front, back);
                std::swap(fsize, bsize);
                std::swap(fcap, bcap);
                std::swap(fhead, bhead);
        }

        void pop_front() {
            if (fsize == 0) {
                swap_ptrs();
                reverse(front, front + fsize);
                front[0].second = front[0].first;
                for (size_t i = 1; i < fsize; ++i) {
                    front[i].second = std::min(front[i - 1].second, front[i].first);
                }
                bsize = 0;
            }
            --fsize;
            --fhead;
        }

        void pop_back() {
            if (bsize == 0) {
                swap_ptrs();
                reverse(back, back + bsize);
                back[0].second = back[0].first;
                for (size_t i = 1; i < bsize; ++i) {
                    back[i].second = std::min(back[i - 1].second, back[i].first);
                }
                fsize = 0;
            }
            --bsize;
            --bhead;
        }

        inline void pop() { return pop_front(); }

        T get_min() {
            if (fsize > 0 && bsize > 0) {
                return std::min(fhead->second, bhead->second);
            }
            return fsize == 0 ? bhead->second : fhead->second;
        }
    };

    template <class T>
    using MinQueue = FunctorQueue<T, NPrivate::Minner>;
};
#endif
