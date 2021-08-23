#pragma once

namespace metrics::util
{

	template <typename T, typename Value, typename Next>
	class ForwardIterator
	{
		using R = std::remove_pointer_t<std::invoke_result_t<Value, T*>>;
	public:
		constexpr ForwardIterator(T*) noexcept;

		constexpr ForwardIterator operator ++ () noexcept;
		constexpr ForwardIterator operator ++ (int) noexcept;
		constexpr R& operator * () noexcept;
		constexpr R* operator -> () noexcept;
		constexpr R& operator * () const noexcept;
		constexpr R* operator -> () const noexcept;
		constexpr bool operator != (ForwardIterator const&) const noexcept;
		constexpr bool operator == (ForwardIterator const&) const noexcept;

	private:
		constexpr R* Get() const noexcept;

		T* m_Current;
	};

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N>::ForwardIterator(T* p) noexcept
		: m_Current{ p }
	{}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N> ForwardIterator<T, V, N>::operator++() noexcept
	{
		m_Current = N{}(m_Current);
		return *this;
	}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N> ForwardIterator<T, V, N>::operator++(int) noexcept
	{
		auto old{ *this };
		operator++();
		return old;
	}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N>::R& ForwardIterator<T, V, N>::operator*() noexcept
	{
		return *Get();
	}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N>::R* ForwardIterator<T, V, N>::operator->() noexcept
	{
		return Get();
	}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N>::R& ForwardIterator<T, V, N>::operator*() const noexcept
	{
		return *Get();
	}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N>::R* ForwardIterator<T, V, N>::operator->() const noexcept
	{
		return Get();
	}

	template <typename T, typename V, typename N>
	constexpr inline bool ForwardIterator<T, V, N>::operator!=(ForwardIterator const& other) const noexcept
	{
		return !operator==(other);
	}

	template <typename T, typename V, typename N>
	constexpr inline bool ForwardIterator<T, V, N>::operator==(ForwardIterator const& other) const noexcept
	{
		return other.m_Current == m_Current;
	}

	template <typename T, typename V, typename N>
	constexpr inline ForwardIterator<T, V, N>::R* ForwardIterator<T, V, N>::Get() const noexcept
	{
		return V{}(m_Current);
	}

}