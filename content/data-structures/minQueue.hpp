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
        std::vector<std::pair<T, T>> front, back;
        size_t fsize = 0, bsize = 0;
        
        FunctorQueue() {}

        FunctorQueue(size_t t) {
            front.reserve(t);
            back.reserve(t);
        }

        
        // must be nonempty. UB otherwise
        static void push_to_vector(std::vector<std::pair<T, T>>& vec, const T& t) {
            vec.push_back({t, std::min(vec.back().second, t)});
        }

        bool empty() const {
            return fsize == 0 && bsize == 0;
        }

        size_t size() const {
            return fsize + bsize;
        }

        // use at your own risk
        void push_back(const T& t) {
            if (bsize == 0) back = {{t,t}};
            else push_to_vector(back, t);
            ++bsize;
        }

        void push_front(const T& t) {
            if (fsize == 0) front = {{t,t}};
            else push_to_vector(front, t);
            ++fsize;
        }

        inline void push(const T& t) { return push_back(t); }

        void pop_front() {
            if (fsize == 0) {
                front.swap(back);
                std::swap(fsize, bsize);
                reverse(front.begin(), front.end());
                front[0].second = front[0].first;
                for (size_t i = 1; i < front.size(); ++i) {
                    front[i].second = std::min(front[i - 1].second, front[i].first);
                }                
            }
            front.pop_back();
            --fsize;
        }

        void pop_back() {
            if (bsize == 0) {
                back.swap(front);
                std::swap(fsize, bsize);
                reverse(back.begin(), back.end());
                back[0].second = back[0].first;
                for (size_t i = 1; i < back.size(); ++i) {
                    back[i].second = std::min(back[i - 1].second, back[i].first);
                }
            }
            back.pop_back();
            --bsize;
        }

        inline void pop() { return pop_front(); }

        T get_min() {
            if (fsize > 0 && bsize > 0) {
                return std::min(front[fsize - 1].second, back[bsize - 1].second);
            }
            return fsize == 0 ? back[bsize - 1].second : front[fsize - 1].second;
        }
    };

    template <class T>
    using MinQueue = FunctorQueue<T, NPrivate::Minner>;
};
#endif
