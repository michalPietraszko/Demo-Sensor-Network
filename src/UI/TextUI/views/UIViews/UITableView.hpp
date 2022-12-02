#pragma once

#include <algorithm>
#include <vector>

template <typename Key, typename Cell>
class UITableView {
public:
    using MappedCell = std::pair<Key, Cell>;
    using MappedCells = std::vector<MappedCell>;

    UITableView(MappedCells&& mappedCell) : m_MappedCells{std::move(mappedCell)} {}

    Cell& at(Key key) {
        auto result = findKey(key);
        const auto wasElemFound = result != m_MappedCells.end();

        assert(wasElemFound);
        return result->second;
    }

    void add(Key key, Cell value) {
        auto result = findKey(key);
        const auto wasElemFound = result != m_MappedCells.end();
        assert(not wasElemFound);
        m_MappedCells.emplace_back(key, value);
    }

    void remove(Key key) {
        auto result = findKey(key);
        const auto wasElemFound = result != m_MappedCells.end();
        assert(wasElemFound);
        m_MappedCells.erase(result);
    }

    typename MappedCells::iterator begin() { return m_MappedCells.begin(); }

    typename MappedCells::iterator end() { return m_MappedCells.end(); }

private:
    decltype(auto) findKey(const Key& key) {
        return std::find_if(
            m_MappedCells.begin(), m_MappedCells.end(), [key](auto& cell) { return key == cell.first; });
    }

protected:
    MappedCells m_MappedCells;
};