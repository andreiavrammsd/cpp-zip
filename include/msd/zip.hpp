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

template <typename... Iterators>
class zip_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::tuple<typename std::iterator_traits<Iterators>::reference...>;
    using pointer = std::tuple<typename std::iterator_traits<Iterators>::pointer...>;
    using reference = std::tuple<typename std::iterator_traits<Iterators>::reference...>;

    explicit zip_iterator(Iterators... iterators) noexcept : iterators_{iterators...} {}

    value_type operator*() const noexcept { return dereference(std::index_sequence_for<Iterators...>{}); }

    bool operator==(const zip_iterator& other) const noexcept
    {
        return equal(std::index_sequence_for<Iterators...>{}, other);
    }

    bool operator!=(const zip_iterator& other) const noexcept
    {
        return !equal(std::index_sequence_for<Iterators...>{}, other);
    }

    zip_iterator& operator++() noexcept
    {
        advance(std::index_sequence_for<Iterators...>{}, 1);
        return *this;
    }

    zip_iterator operator+(const std::size_t offset) const noexcept
    {
        auto iterator = *this;
        iterator.advance(std::index_sequence_for<Iterators...>{}, offset);
        return iterator;
    }

    zip_iterator operator+(const zip_iterator& other) const noexcept
    {
        auto iterator = *this;
        const auto distance = std::distance(iterator, other);
        iterator.advance(std::index_sequence_for<Iterators...>{}, distance);
        return iterator;
    }

    zip_iterator& operator--() noexcept
    {
        advance(std::index_sequence_for<Iterators...>{}, -1);
        return *this;
    }

    zip_iterator operator-(const int offset) const noexcept
    {
        auto iterator = *this;
        iterator.advance(std::index_sequence_for<Iterators...>{}, -offset);
        return iterator;
    }

    zip_iterator operator-(const zip_iterator& other) const noexcept
    {
        auto iterator = *this;
        const auto distance = std::distance(other, iterator);
        iterator.advance(std::index_sequence_for<Iterators...>{}, -distance);
        return iterator;
    }

   private:
    template <std::size_t... I>
    value_type dereference(std::index_sequence<I...>) const noexcept
    {
        return std::tie(*std::get<I>(iterators_)...);
    }

    template <std::size_t... I>
    bool equal(std::index_sequence<I...>, const zip_iterator& other) const noexcept
    {
        return ((std::get<I>(iterators_) == std::get<I>(other.iterators_)) || ...);
    }

    template <std::size_t... I>
    void advance(std::index_sequence<I...>, const int offset) noexcept
    {
        ((std::advance(std::get<I>(iterators_), offset)), ...);
    }

    std::tuple<Iterators...> iterators_;
};

template <typename... Containers>
class zip {
   public:
    static_assert(sizeof...(Containers) > 1, "zip requires at least 2 containers");

    using iterator =
        zip_iterator<typename std::conditional_t<std::is_const_v<Containers>, typename Containers::const_iterator,
                                                 typename Containers::iterator>...>;

    using value_type = typename iterator::value_type;

    explicit zip(Containers&... containers) noexcept : containers_{containers...} {}

    iterator begin() const { return begin_impl(std::index_sequence_for<Containers...>{}); }
    iterator end() const { return end_impl(std::index_sequence_for<Containers...>{}); }

    iterator cbegin() const { return begin_impl(std::index_sequence_for<Containers...>{}); }
    iterator cend() const { return end_impl(std::index_sequence_for<Containers...>{}); }

    [[nodiscard]] std::size_t size() const { return size_impl(std::index_sequence_for<Containers...>{}); }

    [[nodiscard]] bool empty() const noexcept { return begin() == end(); }

    constexpr explicit operator bool() const { return !empty(); }

    value_type front()
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *begin();
    }

    value_type front() const
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *begin();
    }

    value_type back()
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *std::prev(begin() + size());
    }

    value_type back() const
    {
        assert(!empty());  // LCOV_EXCL_LINE
        return *std::prev(begin() + size());
    }

    constexpr value_type operator[](const std::size_t offset) const
    {
        assert(offset < size());
        return *std::next(begin(), offset);
    }

   private:
    template <std::size_t... I>
    iterator begin_impl(std::index_sequence<I...>) const noexcept
    {
        return iterator{std::get<I>(containers_).begin()...};
    }

    template <std::size_t... I>
    iterator end_impl(std::index_sequence<I...>) const noexcept
    {
        return std::next(iterator{std::get<I>(containers_).begin()...}, size());
    }

    template <std::size_t... I>
    std::size_t size_impl(std::index_sequence<I...>) const
    {
        return std::min({std::distance(std::get<I>(containers_).begin(), std::get<I>(containers_).end())...});
    }

    std::tuple<Containers&...> containers_;
};

}  // namespace msd

#endif  // MSD_ZIP_ZIP_HPP
