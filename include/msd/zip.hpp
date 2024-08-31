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
    explicit zip_iterator(Iterators... iterators) : iterators_{iterators...} {}

    auto operator*() const { return dereference(std::index_sequence_for<Iterators...>{}); }

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
    auto dereference(std::index_sequence<I...> /*unused*/) const
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

    explicit zip(Containers&... containers) : containers_{containers...} {}

    using iterator =
        zip_iterator<typename std::conditional_t<std::is_const_v<Containers>, typename Containers::const_iterator,
                                                 typename Containers::iterator>...>;

    iterator begin() const { return begin_impl(std::index_sequence_for<Containers...>{}); }
    iterator end() const { return end_impl(std::index_sequence_for<Containers...>{}); }

    iterator cbegin() const { return begin_impl(std::index_sequence_for<Containers...>{}); }
    iterator cend() const { return end_impl(std::index_sequence_for<Containers...>{}); }

    [[nodiscard]] std::size_t size() const { return size_impl(std::index_sequence_for<Containers...>{}); }

    [[nodiscard]] bool empty() const noexcept { return begin() == end(); }

    constexpr explicit operator bool() { return !empty(); }
    constexpr explicit operator bool() const { return !empty(); }

    auto front()
    {
        assert(!empty());
        return *begin();
    }

    auto front() const
    {
        assert(!empty());
        return *begin();
    }

    auto back()
    {
        assert(!empty());
        return *std::prev(begin() + size());
    }

    auto back() const
    {
        assert(!empty());
        return *std::prev(begin() + size());
    }

    constexpr auto operator[](std::size_t offset)
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
