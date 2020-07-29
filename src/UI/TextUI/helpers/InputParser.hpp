#pragma once

#include <InputMatcher.hpp>
#include <any>
#include <functional>
#include <vector>
#include <unordered_map>

template <typename ID, typename ReturnT, typename FnArgT = const std::any&>
class InputParser
{
// clang-format off
    using return_t      = ReturnT;
    using callback_fn_t = std::function<return_t(FnArgT)>;
    using matcher_t     = InputMatcher;
    using matchers_t    = std::vector<matcher_t>;

    struct MatchedCallback /* if any of the following matchers matches then callback will be called */
    {
        matchers_t matchers;
        callback_fn_t callback;
    };

    using callbacks_to_match_t     = std::vector<MatchedCallback>;
    using invalid_input_callback_t = std::function<return_t()>;

    struct ParseCallbackInfo
    {
        callbacks_to_match_t     callbacksToMatch;     /* at least one matcher has to match           */
        invalid_input_callback_t invalidInputCallback; /* if no matcher matches then input is invaild */
    };

    /* every ID has it's own set of matchers and callbacks to create tree like scenarios */
    using id_parse_infos_t = std::unordered_map<ID, ParseCallbackInfo>;
// clang-format on

public:
    InputParser() = default;

    return_t parse(ID id, std::any input)
    {
        /* get set of matchers and callbacks for current ID */
        auto& parseCallbackInfo = parseInfo.at(id);

        /* iterate over callbacks to match, at least one has to match */
        for(auto& matchedCallback : parseCallbackInfo.callbacksToMatch)
        {
            /* each MatchedCallback can have many matchers if at least one matches then callback is called*/
            for(auto& matcher : matchedCallback.matchers)
            {
                if(matcher.isMatch(input))
                {
                    return matchedCallback.callback(matcher.getInputInMatchedForm());
                }
            }
        }

        /* if no match was found for no callback at this ID then input is invalid */
        return parseCallbackInfo.invalidInputCallback();        
    }

    void enable(ID id, invalid_input_callback_t invalidInputCallback)
    {
        auto res = parseInfo.insert({id, {{}, std::move(invalidInputCallback)}});
        assert(res.second);
    }
   
    void addCallback(ID id, std::initializer_list<matcher_t>&& matchers, callback_fn_t&& callback)
    {
        auto& parseCallbackInfo = parseInfo.at(id);
        parseCallbackInfo.callbacksToMatch.push_back({{matchers}, std::move(callback)});
    }

    void addCallback(std::initializer_list<ID> ids, std::initializer_list<matcher_t>&& matchers, callback_fn_t callback)
    {
        for(const auto& id : ids)
        {
            auto& parseCallbackInfo = parseInfo.at(id);
            parseCallbackInfo.callbacksToMatch.push_back({{matchers}, callback});
        }
    }

private:
    id_parse_infos_t parseInfo{};
};