#pragma once

#include <memory>

#include "Iterator.h"

namespace metrics::util
{

	template <typename T>
	class AtomicStack
	{

		struct Node;

		using ItValue = decltype([](auto* p) { return &p->value; });
		using ItNext = decltype([](auto* p) { return p->next;   });

	public:

		using iterator = ForwardIterator<Node, ItValue, ItNext>;
		using const_iterator = ForwardIterator<Node const, ItValue, ItNext>;

		explicit AtomicStack();
		~AtomicStack();

		T& push(T t);
		void pop();

		T& front();
		T const& front() const;
		T& back();
		T const& back() const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const;
		const_iterator cend() const;

	private:

		AtomicStack(AtomicStack const&) = delete;
		AtomicStack(AtomicStack&&) = delete;
		AtomicStack& operator = (AtomicStack const&) = delete;
		AtomicStack& operator = (AtomicStack&&) = delete;

		struct Node
		{
			~Node() {};

			union
			{
				T value;
				struct {} null;
			};

			Node* next;
		};

		std::atomic<Node*> m_Head;
		std::atomic<Node*> m_Tail;

	};

	template <typename T>
	AtomicStack<T>::AtomicStack()
		: m_Head{ new Node{.null = {}, .next = {} } }
		, m_Tail{ m_Head.load() }
	{}

	template<typename T>
	inline AtomicStack<T>::~AtomicStack()
	{
		Node* node = m_Head;
		delete std::exchange(node, node->next);
		while (node)
		{
			node->value.~T();
			delete std::exchange(node, node->next);
		}
	}

	template<typename T>
	inline T& AtomicStack<T>::push(T t)
	{
		Node* node = new Node{ .value = { std::move(t) }, .next = {} };
		Node* tail = m_Tail.load();
		while (!m_Tail.compare_exchange_strong(tail, node));
		tail->next = node;
		return node->value;
	}

	template<typename T>
	inline void AtomicStack<T>::pop()
	{
		Node* head = m_Head.load();
		if (Node* next = head->next)
		{
			while (!m_Head.compare_exchange_strong(head, next));
			next->value.~T();
			delete head;
		}
	}

	template<typename T>
	inline T& AtomicStack<T>::front()
	{
		Node* head = m_Head.load();
		if (auto& node = head->next)
		{
			return node->value;
		}
		else head->value;
	}

	template<typename T>
	inline T& AtomicStack<T>::back()
	{
		m_Tail.load()->value;
	}

	template<typename T>
	inline T const& AtomicStack<T>::front() const
	{
		Node const* head = m_Head.load();
		if (auto& node = head->next)
		{
			return node->value;
		}
		else head->value;
	}

	template<typename T>
	inline T const& AtomicStack<T>::back() const
	{
		m_Tail.load()->value;
	}

	template<typename T>
	inline AtomicStack<T>::iterator AtomicStack<T>::begin()
	{
		return { m_Head.load()->next };
	}
	template<typename T>
	inline AtomicStack<T>::const_iterator AtomicStack<T>::begin() const
	{
		return cbegin();
	}
	template<typename T>
	inline AtomicStack<T>::const_iterator AtomicStack<T>::cbegin() const
	{
		return { m_Head.load()->next };
	}
	template<typename T>
	inline AtomicStack<T>::iterator AtomicStack<T>::end()
	{
		return { nullptr };
	}
	template<typename T>
	inline AtomicStack<T>::const_iterator AtomicStack<T>::end() const
	{
		return cbegin();
	}
	template<typename T>
	inline AtomicStack<T>::const_iterator AtomicStack<T>::cend() const
	{
		return { nullptr };
	}

}