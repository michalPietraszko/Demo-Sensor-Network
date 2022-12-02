#pragma once
#include <algorithm>
#include <any>
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <utility>

using matcher_fn_t = std::function<bool(std::any&)>;

namespace matchers {
struct Matchers {
    static matcher_fn_t alwaysMatcher() {
        return [](auto&) { return true; };
    }
};

struct NumberMatchers {
    static matcher_fn_t equals(const int& number) {
        return [number](auto& input) { return std::any_cast<int>(input) == number; };
    }

    static matcher_fn_t inRange(const int& inclusiveLeftRange, const int& inclusiveRightRange) {
        return [l = inclusiveLeftRange, r = inclusiveRightRange](auto& input) {
            const auto intInput = std::any_cast<int>(input);
            return l <= intInput and intInput <= r;
        };
    }
};

struct StringMatchers {
    static matcher_fn_t isNumber() {
        return [](auto& input) {
            const auto strInput = std::any_cast<std::string>(input);
            if (strInput.empty()) return false;
            const auto isAllDigits = std::all_of(strInput.begin(), strInput.end(), ::isdigit);
            if (not isAllDigits) return false;

            input = static_cast<int>(std::stoi(strInput));
            return true;
        };
    }

    static matcher_fn_t equals(const std::string str) {
        return [str](auto& input) {
            const auto strInput = std::any_cast<std::string>(input);
            return str == strInput;
        };
    }
};
} // namespace matchers

class InputMatcher {
public:
    InputMatcher() = default;
    InputMatcher(matcher_fn_t fn, InputMatcher&& nextMatcher) : m_Matcher(std::move(fn)) {
        m_NextMatcher = std::make_unique<InputMatcher>();
        *m_NextMatcher = std::move(nextMatcher);
    }

    void setMatcher(matcher_fn_t fn) { m_Matcher = std::move(fn); }

    void setNext(InputMatcher&& matcher) {
        if (not m_NextMatcher) m_NextMatcher = std::make_unique<InputMatcher>();
        *m_NextMatcher = std::move(matcher);
    }

    bool isMatch(std::any& input) {
        modifiableInput = input;
        if (not m_Matcher(modifiableInput)) return false;
        if (not m_NextMatcher) return true;

        return m_NextMatcher->isMatch(modifiableInput);
    }

    std::any getInputInMatchedForm() { return modifiableInput; }

    // clang-format off
    matcher_fn_t m_Matcher = [](auto&){ assert(false); return false; };
    std::shared_ptr<InputMatcher> m_NextMatcher{nullptr};
    std::any modifiableInput;
};
// clang-format on

class InputMatcherCreator {
public:
    template <typename Fn, typename... Fns>
    static InputMatcher create(Fn first, Fns&&... rest) {
        return InputMatcher(first, create(rest...));
    }

    template <typename Fn>
    static InputMatcher create(Fn&& fn) {
        InputMatcher m;
        m.setMatcher(std::move(fn));
        return m;
    }

    static InputMatcher numberEqual(const int n) {
        return create(matchers::StringMatchers::isNumber(), matchers::NumberMatchers::equals(n));
    }

    static InputMatcher numberInRange(const int lhs, const int rhs) {
        return create(matchers::StringMatchers::isNumber(), matchers::NumberMatchers::inRange(lhs, rhs));
    }

    static InputMatcher stringEqual(const std::string& str) { return create(matchers::StringMatchers::equals(str)); }
};