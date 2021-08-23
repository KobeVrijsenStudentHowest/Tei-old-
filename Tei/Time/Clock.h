#pragma once

#include <chrono>

#include "../Units/Units.h"
#include "../Utility/Utility.h"

namespace tei::internal::time
{

    class Clock
    {

        using Source = std::chrono::steady_clock;

        struct Duration;
        struct TimePoint;

    public:

        using rep = units::Unit;
        using period = std::ratio<1, 1>;
        using duration = Duration;
        using time_point = TimePoint;
        static constexpr bool is_steady = true;

        [[nodiscard]] static inline time_point now() noexcept
        {
            return time_point{ duration{ Source::now() - m_Epoch } };
        }

    private:

        struct Duration : std::chrono::duration<rep, period>
        {
            using Base = std::chrono::duration<rep, period>;
            using Base::duration;

            [[nodiscard]] constexpr inline operator Source::duration() const noexcept
            {
                return std::chrono::duration_cast<Source::duration>(*this);
            }

            template <typename Rep, typename Period>
            [[nodiscard]] inline Clock::duration& operator %= (std::chrono::duration<Rep, Period> const& r) noexcept
            {
                return *this = *this % r;
            }

            template <typename Rep1, typename Period1, typename Rep2, typename Period2>
            [[nodiscard]] friend inline Clock::duration operator % (std::chrono::duration<Rep1, Period1> const& l, std::chrono::duration<Rep2, Period2> const& r) noexcept
            requires std::is_floating_point_v< std::common_type_t<Rep1, Rep2>>
            {
                using Common = std::common_type_t<std::chrono::duration<Rep1, Period1>, std::chrono::duration<Rep2, Period2>>;
                return Common(std::fmod(Common(l).count(), Common(r).count()));
            }

        };

        struct TimePoint : std::chrono::time_point<Clock, Duration::Base>
        {
            using std::chrono::time_point<Clock, Duration::Base>::time_point;

            [[nodiscard]] constexpr inline operator Source::time_point() const noexcept
            {
                return Clock::m_Epoch + std::chrono::duration_cast<Source::duration>(time_since_epoch());
            }

            [[nodiscard]] constexpr inline operator Duration() const noexcept
            {
                return time_since_epoch();
            }
        };

        static Source::time_point const m_Epoch;

    };

    namespace literals
    {

        constexpr Clock::duration operator "" _d (unsigned long long d) noexcept
        {
            return Clock::duration{ d * (60.f * 60.f * 24.f) };
        }

        constexpr Clock::duration operator "" _h (unsigned long long d) noexcept
        {
            return Clock::duration{ d * (60.f * 60.f) };
        }

        constexpr Clock::duration operator "" _min (unsigned long long d) noexcept
        {
            return Clock::duration{ d / 60.f };
        }

        constexpr Clock::duration operator "" _s (unsigned long long d) noexcept
        {
            return Clock::duration{ float(d) };
        }

        constexpr Clock::duration operator "" _ms (unsigned long long d) noexcept
        {
            return Clock::duration{ d / 1e3f };
        }

        constexpr Clock::duration operator "" _us (unsigned long long d) noexcept
        {
            return Clock::duration{ d / 1e6f };
        }

        constexpr Clock::duration operator "" _ns (unsigned long long d) noexcept
        {
            return Clock::duration{ d / 1e9f };
        }

    }

}

using namespace tei::internal::time::literals;