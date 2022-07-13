#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include "CustomLinkedList.hpp"
#include <unordered_map>
#include <functional>
#include "Block.hpp"

namespace MyDB {

	using CacheVisitor = std::function<void(BlockPtr aBlock)>;

	template<typename KeyT, typename ValueT>
	class LRUCache {
	public:

		//OCF 
		LRUCache(size_t aSize) : maxSize(aSize) {};

		ValueT put(const KeyT& aKey, const ValueT& aValue)
		{
			ValueT theNodeToReturn = aValue;
			if (size() >= maxSize)
			{
				theNodeToReturn = priority.pop();
			}
			lookup[aKey] = priority.push(aValue);
			return theNodeToReturn;
		}

		inline ValueT get(const KeyT& aKey)
		{
			Node<ValueT>*& theNode = lookup.at(aKey);
			priority.pushToEnd(theNode);
			return theNode->getData();
		}

		inline bool contains(const KeyT& aKey) const
		{
			return lookup.count(aKey) > 0;
		}

		inline size_t size() const 
		{
			return lookup.size();
		}

		void flush(const CacheVisitor& aVisitor)
		{
			Node<ValueT>* theIter = priority.getBegin()->getNext();
			while (theIter != priority.getEnd())
			{
				aVisitor(theIter->getData());
				theIter = theIter->getNext();
			}

			priority.clear();
		}

	protected:
		size_t maxSize; //prevent cache from growing past this size...

		std::unordered_map<KeyT, Node<ValueT>*> lookup;

		CustomLinkedList<ValueT> priority;
	};
}

#endif
