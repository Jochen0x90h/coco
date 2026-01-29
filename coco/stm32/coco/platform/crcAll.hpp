#pragma once

// do not include directly, use #include <coco/platform/crc.hpp>

#include "platform.hpp"


/*
    Defines:
    HAVE_CRC              CRC is supported
    HAVE_CRC_SIZE         16, 8 and 7 bit are supporded in addition to 32 bit
    HAVE_CRC_REVERSE      Reversed bit order is supported
    HAVE_CRC_SET_POLY     setPoly() method is supported
    HAVE_CRC_CALC         calc() methods are supported
*/

#define HAVE_CRC

#ifdef CRC_CR_POLYSIZE
#define HAVE_CRC_SIZE
#endif

#ifdef CRC_CR_REV_IN
#define HAVE_CRC_REVERSE
#endif

#ifdef CRC_POL_POL
#define HAVE_CRC_SET_POLY
#endif

#ifdef CRC_INIT_INIT
#define HAVE_CRC_CALC
#endif


namespace coco {

/// @brief CRC helpers.
/// G0X0: Refernece manual: https://www.st.com/resource/en/reference_manual/rm0454-stm32g0x0-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 13
/// G0X1: Refernece manual: https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 14
/// G4 Refernece manual: https://www.st.com/resource/en/reference_manual/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
///   Section 16
/// H503 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 18
/// H5 https://www.st.com/en/microcontrollers-microprocessors/stm32h5-series/documentation.html Section 19
/// U3 https://www.st.com/en/microcontrollers-microprocessors/stm32u3-series/documentation.html Section 18
namespace crc {

enum class Config : uint32_t {
    DEFAULT = 0,

    // size
    SIZE_32 = 0, // default
#ifdef HAVE_CRC_SIZE
    SIZE_16 = CRC_CR_POLYSIZE_0,
    SIZE_8 = CRC_CR_POLYSIZE_1,
    SIZE_7 = CRC_CR_POLYSIZE_1 | CRC_CR_POLYSIZE_0,
#endif

    // reversed bit order
#ifdef HAVE_CRC_REVERSE
    REVERSE_SIZE_32 = CRC_CR_REV_IN | CRC_CR_REV_OUT,
#ifdef HAVE_CRC_SIZE
    REVERSE_SIZE_16 = CRC_CR_REV_IN_1 | CRC_CR_REV_OUT | CRC_CR_POLYSIZE_0,
    REVERSE_SIZE_8 = CRC_CR_REV_IN_0 | CRC_CR_REV_OUT | CRC_CR_POLYSIZE_1,
    REVERSE_SIZE_7 = CRC_CR_REV_IN_0 | CRC_CR_REV_OUT | CRC_CR_POLYSIZE_1 | CRC_CR_POLYSIZE_0,
#endif
#endif
};


struct Instance {
    CRC_TypeDef *crc;


    CRC_TypeDef *operator ->() const {return crc;}
    operator CRC_TypeDef *() const {return crc;}

    /// @brief Configure the CRC unit
    /// @param config Configuration
    /// @return *this
    auto &configure(Config config) {
        crc->CR = uint32_t(config);

        // with this hack it is possible to write crc::init().configure()
        return *this;
    }

#ifdef HAVE_CRC_SET_POLY
    /// @brief Set polynomial for CRC calculation
    /// Default is 0x04C11DB7 for CRC-32, see https://crccalc.com/?crc=4321&method=CRC-32/MPEG-2&datatype=ascii&outtype=hex
    /// @param poly Polynomial
    /// @return *this
    auto &setPoly(uint32_t poly) {
        crc->POL = poly;
        return *this;
    }
#endif

    /// @brief Start a new CRC calculation
    /// @return *this
    auto &start() {
        crc->CR = crc->CR | CRC_CR_RESET;
        return *this;
    }

    /// @brief Add a buffer to the CRC calculation.
    /// @param buf Buffer of integers (e.g. uint32_t) to calculate the CRC for
    /// @param size Number of integers in the buffer
    /// @return *this
    auto &add(const auto *buf, int size) {
        while (size > 0) {
            crc->DR = *buf;
            ++buf;
            --size;
        }
        return crc->DR;
    }

    /// @brief Add an iterator range to the CRC calculation.
    /// @tparam T Type of iterators
    /// @param begin Begin iterator of a container of integers (e.g. uint32_t) to calculate the CRC for
    /// @param end End iterator
    /// @return *this
    template <typename T>
    auto &add(T begin, T end) {
        for (auto it = begin; it != end; ++it) {
            crc->DR = *it;
        }
        return crc->DR;
    }

    /// @brief Add a container to the CRC calculation.
    /// @tparam T Type of container, can be a c-array, std::vector or similar
    /// @param container Container of integers (e.g. uint32_t) to calculate the CRC for
    /// @return *this
    template <typename T>
    auto &add(T &container) {
        return calc(std::begin(container), std::end(container));
    }

    /// @brief Get the result of the CRC calculation
    /// @return Calculated CRC
    uint32_t result() {
        return crc->DR;
    }

#ifdef HAVE_CRC_CALC
    /// @brief Calculate the CRC of a buffer given a start value.
    /// Note that start() does not need to be called
    /// @param crc Initial or previous CRC value
    /// @param buf Buffer of integers (e.g. uint32_t) to calculate the CRC for
    /// @param size Number of integers in the buffer
    /// @return Calculated CRC value
    uint32_t calc(uint32_t init, const auto *buf, int size) {
        crc->INIT = init;
        while (size > 0) {
            crc->DR = *buf;
            ++buf;
            --size;
        }
        return crc->DR;
    }

    /// @brief Calculate the CRC of an iterator range given a start value.
    /// Note that start() does not need to be called
    /// @tparam T Type of iterators
    /// @param crc Initial or previous CRC value
    /// @param begin Begin iterator of a container of integers (e.g. uint32_t) to calculate the CRC for
    /// @param end End iterator
    /// @return Calculated CRC value
    template <typename T>
    uint32_t calc(uint32_t init, T begin, T end) {
        crc->INIT = init;
        for (auto it = begin; it != end; ++it) {
            crc->DR = *it;
        }
        return crc->DR;
    }

    /// @brief Calculate the CRC of a container given a start value.
    /// Note that start() does not need to be called
    /// @tparam T Type of container, can be a c-array, std::vector or similar
    /// @param crc Initial or previous CRC value
    /// @param container Container of integers (e.g. uint32_t) to calculate the CRC for
    /// @return Calculated CRC value
    template <typename T>
    uint32_t calc(uint32_t init, T &container) {
        return calc(init, std::begin(container), std::end(container));
    }
#endif
};

} // namespace crc
} // namespace coco
