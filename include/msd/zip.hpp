#ifndef MSD_ZIP_ZIP_HPP
#define MSD_ZIP_ZIP_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace msd {

/**
 * @brief Bidirectional iterator over multiple iterators simultaneously.
 *
 * @tparam Iterators Types of the iterators to be zipped. Each iterator should support bidirectional iteration
 *                   (i.e., it should support `++`, `--`, and dereferencing).
 */
template <typename... Iterators>
class zip_iterator {
   public:
    /**
     * @brief Supports bidirectional traversal.
     */
    using iterator_category = std::bidirectional_iterator_tag;

    /**
     * @brief The difference between two iterators.
     */
    using difference_type = std::ptrdiff_t;

    /**
     * @brief A tuple of references from each of the zipped iterators.
     */
    using value_type = std::tuple<typename std::iterator_traits<Iterators>::reference...>;

    /**
     * @brief A tuple of pointers from each of the zipped iterators.
     */
    using pointer = std::tuple<typename std::iterator_traits<Iterators>::pointer...>;

    /**
     * @brief A tuple of references from each of the zipped iterators.
     */
    using reference = std::tuple<typename std::iterator_traits<Iterators>::reference...>;

    /**
     * @brief Constructs a `zip_iterator` from the provided iterators.
     *
     * @param iterators The iterators to be zipped together.
     */
    explicit zip_iterator(Iterators... iterators) noexcept : iterators_{iterators...} {}

    /**
     * @brief Dereferences the `zip_iterator` to obtain a tuple of references from each iterator.
     *
     * @return A tuple containing the values pointed to by each iterator.
     */
    value_type operator*() const noexcept { return dereference(std::index_sequence_for<Iterators...>{}); }

    /**
     * @brief Checks if two `zip_iterator` instances are equal.
     *
     * @param other The other `zip_iterator` to compare with.
     * @return `true` if the iterators are equal, `false` otherwise.
     */
    bool operator==(const zip_iterator& other) const noexcept
    {
        return equal(std::index_sequence_for<Iterators...>{}, other);
    }

    /**
     * @brief Checks if two `zip_iterator` instances are not equal.
     *
     * @param other The other `zip_iterator` to compare with.
     * @return `true` if the iterators are not equal, `false` otherwise.
     */
    bool operator!=(const zip_iterator& other) const noexcept
    {
        return !equal(std::index_sequence_for<Iterators...>{}, other);
    }

    /**
     * @brief Advances the `zip_iterator` by one position.
     *
     * @return A reference to the updated `zip_iterator`.
     */
    zip_iterator& operator++() noexcept
    {
        advance(std::index_sequence_for<Iterators...>{}, 1);
        return *this;
    }

    /**
     * @brief Returns a new `zip_iterator` advanced by a specified offset.
     *
     * @param offset The number of positions to advance.
     * @return A new `zip_iterator` advanced by the specified offset.
     */
    zip_iterator operator+(const std::size_t offset) const noexcept
    {
        auto iterator = *this;
        iterator.advance(std::index_sequence_for<Iterators...>{}, offset);
        return iterator;
    }

    /**
     * @brief Returns a new `zip_iterator` advanced by the distance between two iterators.
     *
     * @param other The `zip_iterator` to measure the distance to.
     * @return A new `zip_iterator` advanced by the distance to the specified iterator.
     */
    zip_iterator operator+(const zip_iterator& other) const noexcept
    {
        auto iterator = *this;
        const auto distance = std::distance(iterator, other);
        iterator.advance(std::index_sequence_for<Iterators...>{}, distance);
        return iterator;
    }

    /**
     * @brief Moves the `zip_iterator` back by one position.
     *
     * @return A reference to the updated `zip_iterator`.
     */
    zip_iterator& operator--() noexcept
    {
        advance(std::index_sequence_for<Iterators...>{}, -1);
        return *this;
    }

    /**
     * @brief Returns a new `zip_iterator` moved back by a specified offset.
     *
     * @param offset The number of positions to move back.
     * @return A new `zip_iterator` moved back by the specified offset.
     */
    zip_iterator operator-(const int offset) const noexcept
    {
        auto iterator = *this;
        iterator.advance(std::index_sequence_for<Iterators...>{}, -offset);
        return iterator;
    }

    /**
     * @brief Returns the distance between two `zip_iterator` instances.
     *
     * @param other The `zip_iterator` to measure the distance from.
     * @return A new `zip_iterator` moved back by the distance to the specified iterator.
     */
    zip_iterator operator-(const zip_iterator& other) const noexcept
    {
        auto iterator = *this;
        const auto distance = std::distance(other, iterator);
        iterator.advance(std::index_sequence_for<Iterators...>{}, -distance);
        return iterator;
    }

   private:
    /**
     * @brief Dereferences the `zip_iterator`.
     *
     * @tparam I Indices used to unpack the tuple of iterators.
     * @param std::index_sequence<I...> A compile-time sequence used to unpack the tuple of iterators.
     * @return A tuple containing the values pointed to by each iterator.
     */
    template <std::size_t... I>
    value_type dereference(std::index_sequence<I...>) const noexcept
    {
        return std::tie(*std::get<I>(iterators_)...);
    }

    /**
     * @brief Compares two `zip_iterator` instances for equality.
     *
     * @tparam I Indices used to unpack the tuple of iterators.
     * @param std::index_sequence<I...> A compile-time sequence used to unpack the tuple of iterators.
     * @param other The other `zip_iterator` to compare with.
     * @return `true` if all iterators are equal, `false` otherwise.
     */
    template <std::size_t... I>
    bool equal(std::index_sequence<I...>, const zip_iterator& other) const noexcept
    {
        return ((std::get<I>(iterators_) == std::get<I>(other.iterators_)) || ...);
    }

    /**
     * @brief Advances the `zip_iterator` by a specified offset.
     *
     * @tparam I Indices used to unpack the tuple of iterators.
     * @param std::index_sequence<I...> A compile-time sequence used to unpack the tuple of iterators.
     * @param offset The number of positions to advance.
     */
    template <std::size_t... I>
    void advance(std::index_sequence<I...>, const int offset) noexcept
    {
        ((std::advance(std::get<I>(iterators_), offset)), ...);
    }

    /**
     * @brief A list of the iterators being zipped.
     */
    std::tuple<Iterators...> iterators_;
};

/**
 * @brief A view over multiple containers simultaneously.
 *        It allows iterating through multiple containers at once, stopping at the shortest container.
 *
 * @tparam Containers The types of the containers to be zipped.
 *                    Each container must support standard iteration
 *                    (must have `begin()`, `end()`, `cbegin()`, and `cend()` methods).
 */
template <typename... Containers>
class zip {
   public:
    static_assert(sizeof...(Containers) > 1, "zip requires at least 2 containers");

    /**
     * @brief A composite iterator that iterates over all provided containers in parallel.
     *
     *
     * It is constructed based on whether the containers are const or non-const.
     */
    using iterator =
        zip_iterator<typename std::conditional_t<std::is_const_v<Containers>, typename Containers::const_iterator,
                                                 typename Containers::iterator>...>;
    /**
     * @brief A composite iterator that iterates over all provided containers in parallel.
     *
     * Guarantees that the containers are not modified.
     */
    using const_iterator = zip_iterator<typename Containers::const_iterator...>;

    /**
     * @brief The value_type is the type of the element returned by the iterator, which is a tuple of
     * elements from each container.
     */
    using value_type = typename iterator::value_type;

    /**
     * @brief Constructs a `zip` object from the provided containers.
     *
     * @param containers The containers to be zipped together.
     * @pre At least two containers must be provided.
     */
    explicit zip(Containers&... containers) noexcept : containers_{containers...} {}

    /**
     * @brief Returns an iterator pointing to the beginning of the zipped containers.
     *
     * @return An iterator to the first element in the zipped sequence.
     */
    iterator begin() const { return begin_impl<iterator>(std::index_sequence_for<Containers...>{}); }

    /**
     * @brief Returns an iterator pointing to the end of the zipped containers.
     *
     * @return An iterator to the end of the zipped sequence.
     */
    iterator end() const { return end_impl<iterator>(std::index_sequence_for<Containers...>{}); }

    /**
     * @brief Returns a const iterator pointing to the beginning of the zipped containers.
     *
     * @return A const iterator to the first element in the zipped sequence.
     */
    const_iterator cbegin() const { return begin_impl<const_iterator>(std::index_sequence_for<Containers...>{}); }

    /**
     * @brief Returns a const iterator pointing to the end of the zipped containers.
     *
     * @return A const iterator to the end of the zipped sequence.
     */
    const_iterator cend() const { return end_impl<const_iterator>(std::index_sequence_for<Containers...>{}); }

    /**
     * @brief Returns the size of the zipped sequence, which is the size of the smallest container.
     *
     * @return The number of elements in the zipped sequence.
     */
    [[nodiscard]] std::size_t size() const { return size_impl(std::index_sequence_for<Containers...>{}); }

    /**
     * @brief Checks if the zipped sequence is empty.
     *
     * @return `true` if the zipped sequence is empty, `false` otherwise.
     */
    [[nodiscard]] bool empty() const noexcept { return begin() == end(); }

    /**
     * @brief Allows the `zip` object to be used in a boolean context, indicating whether the zipped sequence is
     * non-empty.
     *
     * @return `true` if the zipped sequence is non-empty, `false` otherwise.
     */
    constexpr explicit operator bool() const { return !empty(); }

    /**
     * @brief Returns the first element in the zipped sequence.
     *
     * @pre The zipped sequence must not be empty.
     * @return The first element in the zipped sequence.
     */
    value_type front()
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *begin();
    }

    /**
     * @brief Returns the first element in the zipped sequence (const overload).
     *
     * @pre The zipped sequence must not be empty.
     * @return The first element in the zipped sequence.
     */
    value_type front() const
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *begin();
    }

    /**
     * @brief Returns the last element in the zipped sequence.
     *
     * @pre The zipped sequence must not be empty.
     * @return The last element in the zipped sequence.
     */
    value_type back()
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *std::prev(begin() + size());
    }

    /**
     * @brief Returns the last element in the zipped sequence (const overload).
     *
     * @pre The zipped sequence must not be empty.
     * @return The last element in the zipped sequence.
     */
    value_type back() const
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *std::prev(begin() + size());
    }

    /**
     * @brief Returns the element at the specified offset in the zipped sequence.
     *
     * @param offset The index of the element to retrieve.
     * @pre The offset must be less than the size of the zipped sequence.
     * @return Reference to the element at the specified offset.
     */
    constexpr value_type operator[](const std::size_t offset) const
    {
        assert(offset < size());
        return *std::next(begin(), offset);
    }

   private:
    /**
     * @brief Constructs the beginning iterator of the zipped sequence.
     *
     * @tparam Iterator Type of the iterator to be constructed.
     * @tparam I Indices used to unpack the tuple of containers.
     * @param std::index_sequence<I...> A compile-time sequence used to unpack the tuple of containers.
     * @return An iterator to the beginning of the zipped sequence.
     */
    template <typename Iterator, std::size_t... I>
    Iterator begin_impl(std::index_sequence<I...>) const noexcept
    {
        return Iterator{std::get<I>(containers_).begin()...};
    }

    /**
     * @brief Constructs the end iterator of the zipped sequence.
     *
     * @tparam Iterator Type of the iterator to be constructed.
     * @tparam I Indices used to unpack the tuple of containers.
     * @param std::index_sequence<I...> A compile-time sequence used to unpack the tuple of containers.
     * @return An iterator to the end of the zipped sequence.
     */
    template <typename Iterator, std::size_t... I>
    Iterator end_impl(std::index_sequence<I...>) const noexcept
    {
        return std::next(Iterator{std::get<I>(containers_).begin()...}, size());
    }

    /**
     * @brief Determine the size of the zipped sequence as the size of the smallest container.
     *
     * @tparam I Indices used to unpack the tuple of containers.
     * @param std::index_sequence<I...> A compile-time sequence used to unpack the tuple of containers.
     * @return The size of the zipped sequence.
     */
    template <std::size_t... I>
    std::size_t size_impl(std::index_sequence<I...>) const
    {
        return std::min({std::distance(std::get<I>(containers_).begin(), std::get<I>(containers_).end())...});
    }

    /**
     * @brief A list of references to the containers being zipped.
     */
    std::tuple<Containers&...> containers_;
};

}  // namespace msd

#endif  // MSD_ZIP_ZIP_HPP
