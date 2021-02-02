#ifndef TACO2_COORDINATE_ITERATE_HPP
#define TACO2_COORDINATE_ITERATE_HPP
#include <iterator>
#include <taco2/util/template_utils.hpp>
#include <taco2/util/iterators.hpp>
#include <tuple>
#include <type_traits>

namespace taco2::level_capabilities {
    template<typename T>
    class coordinate_value_iterate
    {
        static_assert(std::is_integral_v<typename T::Value>, "`T::Value` must be an integral type.");
        static_assert(std::is_integral_v<typename T::Position>, "`T::Position` must be an integral type.");
        using IK = typename T::Value;
        using PK = typename T::Position;
    public:
        template<typename I, typename PKM1, typename IK>
        class iteration_helper
        {
        private:
            using iter_helper_type = iteration_helper<I, PKM1, IK>;

            static_assert(util::is_tuple_with_integral_template_arguments_v<I>,
                          "`I` must be a specialization of `std::tuple` with integral template arguments.");
            static_assert(std::is_integral_v<PKM1>,
                          "`PKM1` must be integral.");
            static_assert(std::is_integral_v<IK>,
                          "`IK` must be integral.");
            static_assert(std::is_nothrow_invocable_r_v<PK, decltype(T::coord_access), PKM1, I, IK>);

            T const& m_level;
            I const m_i;
            PKM1 const m_pkm1;
            IK const m_ik_begin, m_ik_end;

        public:
            class iterator;
            using value_type = PK;
            using difference_type = typename std::make_signed<IK>::type;
            using key_type = IK;
            using pointer = PK*;
            using reference = std::pair<IK, PK>;
            using iterator_type = iterator;

            class iterator : public taco2::util::iterators::random_access_iterator_base2<iter_helper_type>
            {
            private:
                iter_helper_type const& m_iterHelper;
                IK m_ik;

            public:
                explicit inline iterator(iter_helper_type const& iterationHelper, IK ik) noexcept
                    : m_iterHelper(iterationHelper), m_ik(ik)
                {}

                inline std::tuple<IK, PK> operator*() const noexcept
                {
                    auto pk = m_iterHelper.m_level.coord_access(m_iterHelper.m_pkm1, m_iterHelper.m_i, m_ik);
                    return { m_ik, pk };
                }

                inline iterator& operator++() noexcept
                {
                    ++m_ik;
                    return *this;
                }

                inline iterator& operator--() noexcept
                {
                    --m_ik;
                    return *this;
                }

                inline iterator& operator+=(difference_type n) noexcept
                {
                    m_ik += n;
                    return *this;
                }

                inline iterator& operator-=(difference_type n) noexcept
                {
                    m_ik -= n;
                    return *this;
                }

                inline difference_type operator-(iterator const& other) {
                    return static_cast<difference_type>(m_ik) - static_cast<difference_type>(other.m_ik);
                }

                inline bool operator==(iterator const& other) const noexcept { return m_ik == other.m_ik; }

                inline bool operator<(iterator const& other) const noexcept { return m_ik < other.m_ik; }
            };

            explicit inline iteration_helper(T const& level, I const i, PKM1 const pkm1) noexcept
                : m_level(level), m_i(i), m_pkm1(pkm1)
            {
                auto p = level.coord_bounds(i);
                m_ik_begin = p.first;
                m_ik_end = p.second;
            }

            inline iterator_type begin() const noexcept { return { *this, m_ik_begin }; }

            inline iterator_type end() const noexcept { return { *this, m_ik_end }; }
        };
    };

    template<typename T>
    class coordinate_position_iterate
    {
        static_assert(std::is_integral_v<typename T::Value>, "`T::Value` must be an integral type.");
        static_assert(std::is_integral_v<typename T::Position>, "`T::Position` must be an integral type.");
        using IK = typename T::Value;
        using PK = typename T::Position;
    public:
        template<typename I, typename PKM1, typename IK>
        class iteration_helper
        {
        private:
            using iter_helper_type = iteration_helper<I, PKM1, IK>;

            static_assert(util::is_tuple_with_integral_template_arguments_v<I>,
                          "`I` must be a specialization of `std::tuple` with integral template arguments.");
            static_assert(std::is_integral_v<PKM1>,
                          "`PKM1` must be integral.");
            static_assert(std::is_integral_v<IK>,
                          "`IK` must be integral.");
            static_assert(std::is_nothrow_invocable_r_v<IK, decltype(T::pos_access), PK, I>);

            T const& m_level;
            I const m_i;
            IK const m_pk_begin, m_pk_end;

        public:
            class iterator;
            using value_type = PK;
            using difference_type = typename std::make_signed<PK>::type;
            using key_type = IK;
            using pointer = PK*;
            using reference = std::pair<IK, PK>;
            using iterator_type = iterator;

            class iterator : public taco2::util::iterators::random_access_iterator_base2<iter_helper_type>
            {
            private:
                iter_helper_type const& m_iterHelper;
                PK m_pk;

            public:
                explicit inline iterator(iter_helper_type const& iterationHelper, PK pk) noexcept
                    : m_iterHelper(iterationHelper), m_pk(pk)
                {}

                inline std::tuple<IK, PK> operator*() const noexcept
                {
                    auto ik = m_iterHelper.m_level.pos_access(m_pk, m_iterHelper.m_i);
                    return { ik, m_pk };
                }

                inline iterator& operator++() noexcept
                {
                    ++m_pk;
                    return *this;
                }

                inline iterator& operator--() noexcept
                {
                    --m_pk;
                    return *this;
                }

                inline iterator& operator+=(difference_type n) noexcept
                {
                    m_pk += n;
                    return *this;
                }

                inline iterator& operator-=(difference_type n) noexcept
                {
                    m_pk -= n;
                    return *this;
                }

                inline difference_type operator-(iterator const& other) {
                    return static_cast<difference_type>(m_pk) - static_cast<difference_type>(other.m_pk);
                }

                inline bool operator==(iterator const& other) const noexcept { return m_pk == other.m_pk; }

                inline bool operator<(iterator const& other) const noexcept { return m_pk < other.m_pk; }
            };

            explicit inline iteration_helper(T const& level, I const i, PKM1 const pkm1) noexcept
                : m_level(level), m_i(i)
            {
                auto p = level.pos_bounds(pkm1);
                m_pk_begin = p.first;
                m_pk_end = p.second;
            }

            inline iterator_type begin() const noexcept { return { *this, m_pk_begin }; }

            inline iterator_type end() const noexcept { return { *this, m_pk_end }; }
        };
    };
}

#endif  // TACO2_COORDINATE_ITERATE_HPP