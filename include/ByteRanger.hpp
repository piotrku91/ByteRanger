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

template <typename... IntegerDataType>
class ByteRanger final {
private:
    const std::vector<uint8_t>& vectorDataIn_;
    const std::tuple<IntegerDataType&...> contentReferences_;
    std::vector<uint8_t>::const_iterator byteIterator_;
    bool isSuccess_;
    size_t argsSize_;
    size_t argsCounter_;

    // private functions
    bool isDataWithBytesAndArgs() { return (!vectorDataIn_.empty() && argsSize_); }
    bool isDataWithArgsWithoutBytes() { return (vectorDataIn_.empty() && argsSize_); }

    template <typename CurrentProcessedType>
    void splitBytesToVariables(CurrentProcessedType&& arg) {
        if (byteIterator_ >= vectorDataIn_.end() || argsCounter_ > argsSize_) {
            return;
        };

        if ((std::is_same<std::remove_reference_t<CurrentProcessedType>, uint8_t>::value) ||
            (std::is_same<std::remove_reference_t<CurrentProcessedType>, int8_t>::value)) {
            arg = static_cast<std::remove_reference_t<CurrentProcessedType>>(*byteIterator_);
            if (byteIterator_ + 1 > vectorDataIn_.end()) {
                arg = 0;
                argsCounter_++;
                return;
            };
            std::advance(byteIterator_, 1);
        };

        if ((std::is_same<std::remove_reference_t<CurrentProcessedType>, uint16_t>::value) ||
            (std::is_same<std::remove_reference_t<CurrentProcessedType>, int16_t>::value)) {
            arg = static_cast<std::remove_reference_t<CurrentProcessedType>>((*byteIterator_ << 8) + (*(byteIterator_ + 1)));
            if (byteIterator_ + 2 > vectorDataIn_.end()) {
                arg = 0;
                argsCounter_++;
                return;
            };
            std::advance(byteIterator_, 2);
        };

        if ((std::is_same<std::remove_reference_t<CurrentProcessedType>, uint32_t>::value) ||
            (std::is_same<std::remove_reference_t<CurrentProcessedType>, int32_t>::value)) {
            arg = static_cast<std::remove_reference_t<CurrentProcessedType>>((*byteIterator_ << 24) + (*(byteIterator_ + 1) << 16) + (*(byteIterator_ + 2) << 8) + *(byteIterator_ + 3));
            if (byteIterator_ + 4 > vectorDataIn_.end()) {
                arg = 0;
                argsCounter_++;
                return;
            };
            std::advance(byteIterator_, 4);
        };
        argsCounter_++;
    }

    template <typename CurrentProcessedType>
    void writeZeroToVariables(CurrentProcessedType&& arg) {
        arg = 0;
        argsCounter_++;
    }

public:
/** @brief Create object with immiedietly conversion from bytes to deducted types from variables.
 * 
 *  It is constructor version without any shift on the bytes vector.
 *  @param data_vector Data vector with bytes.
 *  @param args... Variables (references) to store data from conversion (template deduct target sizes by these arguments)
 */
    explicit ByteRanger(const std::vector<uint8_t>& data_vector, IntegerDataType&... args)
        : ByteRanger(0, data_vector, args...){};

/** @brief Create object with immiedietly conversion from bytes to deducted types from variables.
 *
 *  @param iterator_entry_shift Entry shift in bytes data vector
 *  @param data_vector Data vector with bytes.
 *  @param args... Variables (references) to store data from conversion (template deduct target sizes by these arguments)
 */
    explicit ByteRanger(size_t iterator_entry_shift, const std::vector<uint8_t>& data_vector, IntegerDataType&... args)
        : vectorDataIn_{data_vector}, contentReferences_{std::tuple<IntegerDataType&...>(args...)}, isSuccess_{false}, argsCounter_{0} {
        argsSize_ = sizeof...(IntegerDataType);

        if (isDataWithArgsWithoutBytes()) {
            std::apply([this](auto&&... args) { (writeZeroToVariables(args), ...); }, contentReferences_);
            return;
        }

        if (!isDataWithBytesAndArgs()) {
            return;
        };

        byteIterator_ = vectorDataIn_.begin();
        std::advance(byteIterator_, iterator_entry_shift);
        std::apply([this](auto&&... args) { (writeZeroToVariables(args), ...); }, contentReferences_);  // Clean step (init variables by 0)
        argsCounter_ = 0;
        std::apply([this](auto&&... args) { (splitBytesToVariables(args), ...); }, contentReferences_);
        isSuccess_ = true;
    };

    // deleted c-tors
    ByteRanger() = delete;
    ByteRanger(ByteRanger&&) = delete;

    // Getters

/** @returns State of success conversion.*/
    size_t isSuccess() const { return isSuccess_; };
/** @returns Amount of all arguments passed to template*/
    size_t getArgsCount() const { return argsSize_; };
/** @returns Amount of all bytes passed by vector to template*/
    size_t getBytesCount() const { return vectorDataIn_.size(); };
/** @returns Const references to input converted variables*/
    const std::tuple<const IntegerDataType&...> getConstReferences() { return contentReferences_; };
};
