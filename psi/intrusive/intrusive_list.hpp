#pragma once

#include <cassert>

#include <memory>
#include <iterator>

namespace psi::intrusive
{
    template <typename T>
    struct intrusive_node
    {
        /**
         * @brief links node before next in list
         *
         * @param next pointer to next element
         */
        void insert_before(intrusive_node *next) noexcept
        {
            if (is_linked())
                return;

            // inserting
            next_ = next;
            prev_ = next->prev_;
            prev_->next_ = this;
            next->prev_ = this;
        }

        /**
         * @brief unlink node from current list
         *
         */
        void unlink() noexcept
        {
            if (next_)
            {
                next_->prev_ = prev_;
            }
            if (prev_)
            {
                prev_->next_ = next_;
            }

            next_ = prev_ = nullptr;
        }

        /**
         * @brief checks if current node is linked in circular buffer
         *
         * @return true node is linked in buffer
         * @return false node not linked in buffer
         */
        bool is_linked() const noexcept
        {
            return next_ != nullptr;
        }

        /**
         * @brief Converts intrusive node to item type pointer
         *
         * @return T* item pointer
         */
        T *as_item() noexcept
        {
            return static_cast<T *>(this);
        }

        intrusive_node *next_ = nullptr;
        intrusive_node *prev_ = nullptr;
    };

    /**
     * @brief intrusive cyclic list
     *
     * @tparam T value_type
     * @tparam Allocator allocator type
     */
    template <typename T>
    class intrusive_list
    {
    public:
        intrusive_list()
        {
            init();
        }

        ~intrusive_list()
        {
            // assert
        }

        // NO COPY
        intrusive_list(const intrusive_list &that) = delete;
        intrusive_list &operator=(const intrusive_list &that) = delete;

        // MOVE
        intrusive_list(intrusive_list &&that)
        {
            init();
            append(that);
        }
        intrusive_list &operator=(intrusive_list &&that) = delete;

        template <typename NodeT, typename ValueT>
        struct iterator_impl
        {
            using value_type = ValueT;
            using difference_type = std::ptrdiff_t;
            using pointer = ValueT *;
            using reference = ValueT &;
            using iterator_category = std::bidirectional_iterator_tag;

            iterator_impl(NodeT *node) : ptr_(node)
            {
            }

            iterator_impl &operator++()
            {
                ptr_ = ptr_->next_;
                return *this;
            }

            iterator_impl &operator--()
            {
                ptr_ = ptr_->prev_;
                return *this;
            }

            // operator std::enable_if_t<std::is_same_v<value_type, const value_type>, iterator_impl<node, value_type>>

            bool operator==(const iterator_impl &that) const
            {
                return ptr_ == that.ptr_;
            }

            bool operator!=(const iterator_impl &that) const
            {
                return !(*this == that);
            }

            reference operator*() const
            {
                return *get_item();
            }

            pointer operator->() const
            {
                return get_item();
            }

            pointer get_item() const
            {
                return static_cast<ValueT *>(ptr_);
            }

        private:
            NodeT *ptr_;
        };

        /////////////////////////

        using node = intrusive_node<T>;
        // using allocator = std::allocator_traits<Allocator>::template rebind_alloc<node>;
        // using alloc_traits = std::allocator_traits<allocator>;
        //////////////////////////

        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using iterator = iterator_impl<node, T>;
        using const_iterator = iterator_impl<const node, const T>;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        ////////////////////////
        ////// ITERATOR SUPPORT/
        ////////////////////////
        iterator begin()
        {
            return iterator{head_.next};
        }

        iterator end()
        {
            return iterator{&head_};
        }

        const_iterator cbegin() const
        {
            return const_iterator{head_.next};
        }

        const_iterator cend() const
        {
            return const_iterator{&head_};
        }

        /////////////////////////

        // template <typename U>
        // void push_back(U &&value)
        // {
        // }

        // template <typename U>
        // void push_front(U &&value)
        // {
        // }

        void push_back(node *node) noexcept
        {
            node->insert_before(&head_);
        }

        void push_front(node *node) noexcept
        {
            node->insert_before(head_.next_);
        }

        T *pop_front()
        {
            if (empty())
                return nullptr;
            auto front = head_.next_;
            front->unlink();
            return front->as_item();
        }

        T *pop_back()
        {
            if (empty())
                return nullptr;
            auto back = head_.prev_;
            back->unlink();
            return back->as_item();
        }

        void append(intrusive_list &that) noexcept
        {
            if (that.empty())
                return;

            auto *that_front = that.head_.next_;
            auto *that_back = that.head_.prev_;

            that_back->next_ = &head_;
            that_front->prev_ = head_.prev_;

            auto *back = head_.prev_;

            head_.prev_ = that_back;
            back->next_ = that_front;

            that.head_.next_ = that.head_.prev_ = &that.head_;
        }

        void unlink_all()
        {
            auto *current = head_.next_;
            while (current != &head_)
            {
                auto *next = current->next_;
                current->unlink();
                current = next;
            }
        }

        //////////////////////////
        // Properties
        /////////////////////////

        bool empty()
        {
            return head_.next_ == &head_;
        }

        size_type size()
        {
            std::distance(begin(), end());
        }

    private:
        void init() noexcept
        {
            head_.next_ = head_.prev_ = &head_;
        }

    private:
        // allocator a_;
        node head_;
    };

}
