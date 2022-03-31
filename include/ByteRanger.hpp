#pragma once

#include <tuple>
#include <type_traits>
#include <vector>

/*
ByteRanger - Converts data from bytes to variables checked by template deductions and fill it with proper destination size of variable.

Written by Piotr K. (dajmosster@gmail.com)
piotrq.eu
2022
*/

template <typename... T2>
class ByteRanger final {
private:
    const std::vector<uint8_t>& vecIn_;
    std::tuple<T2&...> content_;
    size_t argsSize_ = 0;
    size_t argCounter_ = 0;
    std::vector<uint8_t>::const_iterator posCounter_;

public:
    template <typename currentType>
    void splitBytesToVariables(currentType&& arg) {
        if (posCounter_ >= vecIn_.end() || vecIn_.empty() || !argsSize_) {
            return;
        };

        argCounter_++;

        if ((std::is_same<std::remove_reference_t<currentType>, uint8_t>::value) ||
            (std::is_same<std::remove_reference_t<currentType>, int8_t>::value)) {
            arg = *posCounter_;
            posCounter_++;
        };

        if ((std::is_same<std::remove_reference_t<currentType>, uint16_t>::value) ||
            (std::is_same<std::remove_reference_t<currentType>, int16_t>::value)) {
            arg = static_cast<std::remove_reference_t<currentType>>((*posCounter_ << 8) + (*(posCounter_ + 1)));
            posCounter_ += 2;
        };

        if ((std::is_same<std::remove_reference_t<currentType>, uint32_t>::value) ||
            (std::is_same<std::remove_reference_t<currentType>, int32_t>::value)) {
            arg = (*posCounter_ << 24) + (*(posCounter_ + 1) << 16) + (*(posCounter_ + 2) << 8) + *(posCounter_ + 3);
            posCounter_ += 4;
        };
    }

    // c - tors
    explicit ByteRanger(const std::vector<uint8_t>& vec, T2&... args)
        : vecIn_{vec}, content_{std::tuple<T2&...>(args...)} {
        if (!vecIn_.empty()) {
        posCounter_ = vecIn_.begin();
        };
        argsSize_ = sizeof...(T2);

        std::apply([this](auto&&... args) {
            (splitBytesToVariables(args), ...);
        },
                   content_);
    };

    explicit ByteRanger(size_t entry_shift, const std::vector<uint8_t>& vec, T2&... args)
        : vecIn_{vec}, content_{std::tuple<T2&...>(args...)} {

        if (!vecIn_.empty()) {
        posCounter_ = vecIn_.begin() + entry_shift;
        };

        argsSize_ = sizeof...(T2);
        std::apply([this](auto&&... args) {
            (splitBytesToVariables(args), ...);
        },
                   content_);
    };

    // deleted to avoid moving and copying object (7 rule)
    ByteRanger() = delete;
    ByteRanger(ByteRanger&&) = delete;

    std::tuple<T2&...> getContent() const { return content_; };
    size_t getArgsCount() const { return argsSize_; };
    size_t getBytesCount() const { return vecIn_.size(); };
};
