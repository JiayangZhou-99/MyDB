#ifndef CUSTOMLINKEDLIST_HPP
#define CUSTOMLINKEDLIST_HPP

namespace MyDB {
	template<typename T>
	class Node {
	public:
		Node(const T& anElement) : prev(nullptr), next(nullptr), data(anElement){}

		inline void setData(T aData)
		{
			this->data = aData;
		}

		inline T getData()
		{
			return data;
		}

		inline void setNext(Node* aNext)
		{
			this->next = aNext;
		}

		inline void setPrev(Node* aPrev)
		{
			this->prev = aPrev;
		}

		inline Node* getNext()
		{
			return next;
		}

		inline Node* getPrev()
		{
			return prev;
		}

		Node* prev;
		Node* next;

	protected:
		T data;
		
	};

	template<typename T>
	class CustomLinkedList {
	public:
		CustomLinkedList() : begin(new Node<T>(nullptr)), end(new Node<T>(nullptr)) {
			begin->setNext(end);
			end->setPrev(begin);
		}

		//Puts element at the end of the list
		Node<T>* push(T anElement)
		{
			Node<T>* theNewNode = new Node<T>(anElement);
			end->getPrev()->setNext(theNewNode);
			theNewNode->setPrev(end->getPrev());
			end->setPrev(theNewNode);
			theNewNode->setNext(end);
			return end->getPrev();
		}

		//Removes the first element in the list and returns the data
		T pop()
		{
			Node<T>* theNodeToReturn = begin->getNext();
			begin->setNext(theNodeToReturn->getNext());
			theNodeToReturn->getNext()->setPrev(begin);
			return theNodeToReturn->getData();
		}

		//Fix up pointers and move node to end
		void pushToEnd(Node<T>* aNode)
		{
			aNode->getPrev()->setNext(aNode->getNext());
			aNode->getNext()->setPrev(aNode->getPrev());

			aNode->setPrev(end->getPrev());
			end->getPrev()->setNext(aNode);
			aNode->setNext(end);
			end->setPrev(aNode);
		}

		void clear()
		{
			Node<T>* theCurr = begin->getNext();
			Node<T>* thePrev = nullptr;
			while (theCurr != end)
			{
				thePrev = theCurr;
				theCurr = theCurr->getNext();
				delete thePrev;
			}

			begin->setNext(end);
			end->setPrev(begin);
		}

		inline Node<T>* getBegin()
		{
			return begin;
		}

		inline Node<T>* getEnd()
		{
			return end;
		}

	protected:
		Node<T>* begin;
		Node<T>* end;
	};

}


#endif // !CUSTOMLINKEDLIST_HPP
