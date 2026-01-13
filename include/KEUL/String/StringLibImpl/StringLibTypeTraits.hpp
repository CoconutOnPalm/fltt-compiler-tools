#pragma once

#include <ranges>
#include <type_traits>

namespace ke::_impl
{
    /**
     * @brief dynamic_container<std::string>. Examples: std::vector<std::string>, std::list<std::string>
     */
    template <class Container>
    concept _StringRangeTypeConcept =
        std::ranges::range<Container const> &&
        std::is_same_v<typename Container::value_type, std::string> &&
        requires(Container &c) {
            { c.emplace_back("") };
            { Container({std::string()}) };
        };

    /**
     * @brief dynamic_container<std::string>. Examples: std::set<std::string>, std::queue<std::string>
     */
    template <class Container>
    concept _StringContainerTypeConcept =
        std::is_same_v<typename Container::value_type, std::string> &&
        requires(Container &c) {
            { c.emplace("") };
            { Container({std::string()}) };
        };

    template <class Container>
    concept _StringPairTypeConcept = requires(Container &c) {
        { c.first } -> std::convertible_to<std::string>;
        { c.second } -> std::convertible_to<std::string>;
    };
}