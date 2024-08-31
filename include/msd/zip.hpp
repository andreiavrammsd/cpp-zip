#ifndef MSD_ZIP_ZIP_HPP
#define MSD_ZIP_ZIP_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>

namespace msd {

template <typename... Iterators>
class zip_iterator {
   public:
    using value_type = std::tuple<typename std::iterator_traits<Iterators>::reference...>;

    explicit zip_iterator(Iterators... iterators) : iterators_{iterators...} {}

    value_type operator*() const { return dereference(std::index_sequence_for<Iterators...>{}); }

    bool operator==(const zip_iterator& other) const { return equal(std::index_sequence_for<Iterators...>{}, other); }

    bool operator!=(const zip_iterator& other) const { return !equal(std::index_sequence_for<Iterators...>{}, other); }

    zip_iterator& operator++() noexcept
    {
        advance(std::index_sequence_for<Iterators...>{}, 1);
        return *this;
    }

    zip_iterator& operator--() noexcept
    {
        advance(std::index_sequence_for<Iterators...>{}, -1);
        return *this;
    }

    zip_iterator operator+(const std::size_t offset) noexcept
    {
        zip_iterator it = *this;
        it.advance(std::index_sequence_for<Iterators...>{}, offset);
        return it;
    }

   private:
    template <std::size_t... I>
    value_type dereference(std::index_sequence<I...> /*unused*/) const
    {
        return std::tie(*std::get<I>(iterators_)...);
    }

    template <std::size_t... I>
    bool equal(std::index_sequence<I...> /*unused*/, const zip_iterator& other) const
    {
        return ((std::get<I>(iterators_) == std::get<I>(other.iterators_)) || ...);
    }

    template <std::size_t... I>
    void advance(std::index_sequence<I...> /*unused*/, int offset)
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

    explicit zip(Containers&... containers) : containers_{containers...} {}

    iterator begin() const { return begin_impl(std::index_sequence_for<Containers...>{}); }
    iterator end() const { return end_impl(std::index_sequence_for<Containers...>{}); }

    iterator cbegin() const { return begin_impl(std::index_sequence_for<Containers...>{}); }
    iterator cend() const { return end_impl(std::index_sequence_for<Containers...>{}); }

    [[nodiscard]] std::size_t size() const { return size_impl(std::index_sequence_for<Containers...>{}); }

    [[nodiscard]] bool empty() const noexcept { return begin() == end(); }

    constexpr explicit operator bool() { return !empty(); }
    constexpr explicit operator bool() const { return !empty(); }

    value_type front()
    {
        assert(!empty());
        return *begin();
    }

    value_type front() const
    {
        assert(!empty());
        return *begin();
    }

    value_type back()
    {
        assert(!empty());
        return *std::prev(begin() + size());
    }

    value_type back() const
    {
        assert(!empty());
        return *std::prev(begin() + size());
    }

    constexpr value_type operator[](std::size_t offset)
    {
        assert(offset < size());
        return *std::next(begin(), offset);
    }

   private:
    template <std::size_t... I>
    iterator begin_impl(std::index_sequence<I...> /*unused*/) const
    {
        return iterator{std::get<I>(containers_).begin()...};
    }

    template <std::size_t... I>
    iterator end_impl(std::index_sequence<I...> /*unused*/) const
    {
        return iterator{std::get<I>(containers_).end()...};
    }

    template <std::size_t... I>
    std::size_t size_impl(std::index_sequence<I...> /*unused*/) const
    {
        return std::min({std::get<I>(containers_).size()...});
    }

    std::tuple<Containers&...> containers_;
};

}  // namespace msd

template <typename... Iterators>
struct std::iterator_traits<msd::zip_iterator<Iterators...>> {
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
};

#endif  // MSD_ZIP_ZIP_HPP
