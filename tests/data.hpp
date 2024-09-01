#ifndef MSD_ZIP_TESTS_DATA_HPP
#define MSD_ZIP_TESTS_DATA_HPP

class data {
   public:
    inline static int constructs_ = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    inline static int copies_ = 0;      // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    inline static int moves_ = 0;       // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    int num = 0;                        // NOLINT(misc-non-private-member-variables-in-classes)

    explicit data(int n) : num{n} { ++constructs_; }

    data(const data& other) : num{other.num} { ++copies_; }

    data(data&& other) noexcept : num{other.num} { ++moves_; }

    data& operator=(const data& other)
    {
        if (this == &other) {
            return *this;
        }

        ++copies_;
        num = other.num;
        return *this;
    }

    data& operator=(data&& other) noexcept
    {
        ++moves_;
        num = other.num;
        return *this;
    }

    ~data() = default;
};

#endif  // MSD_ZIP_TESTS_DATA_HPP
