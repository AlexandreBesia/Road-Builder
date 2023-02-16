#pragma once

#include <vector>

template <typename Key, typename Priority>
class PriorityQueue
{
public:
	// Public constructor(s)
	PriorityQueue() = default;

	// Public method(s)
	void push(Key key, Priority priority);
	void decreasePriority(Key key, Priority priority);
	std::pair<Key, Priority> pop();
	[[nodiscard]] std::pair<Key, Priority> find(Key key) const;
	[[nodiscard]] std::pair<Key, Priority> top() const;
	[[nodiscard]] inline size_t size() const noexcept { return mValues.size(); }
	[[nodiscard]] inline bool empty() const noexcept { return mValues.empty(); }
private:
	// Private method(s)
	void minHeap(size_t root = 0);
	std::pair<Key, Priority> createInvalidValue() const;

private:
	// Private member(s)
	std::vector<std::pair<Key, Priority>> mValues;
};
#include "PriorityQueue.inl"