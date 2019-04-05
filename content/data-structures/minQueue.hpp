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
        
        FunctorQueue() {}

        FunctorQueue(size_t t) {
            front.reserve(t);
            back.reserve(t);
        }

      static void push_to_vector(std::vector<std::pair<T, T>>& vec, const T& t) {
            if (vec.empty()) {
                vec.push_back({t, t});
            } else {
                vec.push_back({t, std::min(vec.back().second, t)});
            }
        }

        bool empty() const {
            return front.empty() && back.empty();
        }

        size_t size() const {
            return front.size() + back.size();
        }

        // use at your own risk
        void push_back(const T& t) {
            push_to_vector(back, t);
        }

        void push_front(const T& t) {
            push_to_vector(front, t);
        }

        inline void push(const T& t) { return push_back(t); }

        void pop_front() {
            if (front.empty()) {
                front.swap(back);
                reverse(front.begin(), front.end());
                front[0].second = front[0].first;
                for (size_t i = 1; i < front.size(); ++i) {
                    front[i].second = std::min(front[i - 1].second, front[i].first);
                }                
            }
            front.pop_back();
        }

        void pop_back() {
            if (back.empty()) {
                back.swap(front);
                reverse(back.begin(), back.end());
                back[0].second = back[0].first;
                for (size_t i = 1; i < back.size(); ++i) {
                    back[i].second = std::min(back[i - 1].second, back[i].first);
                }
            }
            back.pop_back();
        }

        inline void pop() { return pop_front(); }

        T get_min() {
            if (!front.empty() && !back.empty()) {
                return std::min(front.back().second, back.back().second);
            }
            return front.empty() ? back.back().second : front.back().second;
        }
    };

    template <class T>
    using MinQueue = FunctorQueue<T, NPrivate::Minner>;
};
#endif
