// Public method(s)
template <typename Key, typename Priority>
void PriorityQueue<Key, Priority>::push(Key key, Priority priority)
{
	mValues.emplace_back(key, std::numeric_limits<Priority>().max());
	decreasePriority(key, priority);
}

template <typename Key, typename Priority>
void PriorityQueue<Key, Priority>::decreasePriority(Key key, Priority priority)
{
	auto valueItr = std::find_if(mValues.begin(), mValues.end(), [key](auto& value) {
		return value.first == key;
	});
	if (valueItr == mValues.end() || priority > (*valueItr).second) {
		return;
	}

	(*valueItr).second = priority;
	size_t index = std::distance(mValues.begin(), valueItr);
	while (index > 0 && mValues[(index - 1) / 2].second > mValues[index].second) {
		std::swap(mValues[(index - 1) / 2], mValues[index]);
		index = (index - 1) / 2;
	}
}

template <typename Key, typename Priority>
std::pair<Key, Priority> PriorityQueue<Key, Priority>::pop()
{
	if (empty()) {
		return createInvalidValue();
	}

	const std::pair<Key, Priority> MIN = std::exchange(mValues.front(), mValues.back());
	mValues.pop_back();
	minHeap();

	return MIN;
}

template <typename Key, typename Priority>
std::pair<Key, Priority> PriorityQueue<Key, Priority>::find(Key key) const
{
	auto found = std::find_if(mValues.begin(), mValues.end(), [key](const auto& value) {
		return value.first == key;
	});
	return (found != mValues.end()) ? *found : createInvalidValue();
}

template <typename Key, typename Priority>
std::pair<Key, Priority> PriorityQueue<Key, Priority>::top() const
{
	return !empty() ? mValues.front() : createInvalidValue();
}

// Private method(s)
template <typename Key, typename Priority>
void PriorityQueue<Key, Priority>::minHeap(size_t root)
{
	size_t sonIndex = root * 2 + 1;
	bool isMin = false;

	while (!isMin && sonIndex < size()) {
		if (sonIndex + 1 < size() && mValues[sonIndex + 1].second < mValues[sonIndex].second) {
			++sonIndex;
		}

		if (mValues[sonIndex].second < mValues[root].second) {
			std::swap(mValues[sonIndex], mValues[root]);
			root = std::exchange(sonIndex, sonIndex * 2 + 1);
		}
		else {
			isMin = true;
		}
	}
}

template <typename Key, typename Priority>
std::pair<Key, Priority> PriorityQueue<Key, Priority>::createInvalidValue() const
{
	return std::make_pair(std::numeric_limits<Key>().max(), static_cast<Priority>(0));
}