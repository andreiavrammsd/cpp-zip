#ifndef MSD_ZIP_TESTS_DATA_HPP
#define MSD_ZIP_TESTS_DATA_HPP

class data {
   public:
    inline static int constructs_ = 0;
    inline static int copies_ = 0;
    inline static int moves_ = 0;
    int num = 0;

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
