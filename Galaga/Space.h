#pragma once

#include <Tei.h>
namespace units = tei::units;

class Body;
class Ship;
class Level;

class Grid final : std::vector<bool>
{
public:

	constexpr Grid() noexcept = default;

	constexpr Grid(size_t width, size_t height)
		: vector(height * width)
		, m_Stride{ width }
	{}

	constexpr void Resize(size_t width, size_t height) noexcept
	{
		vector::clear();
		vector::resize(width * height);
		m_Stride = width;
	}

	constexpr size_t Width() const noexcept
	{ return m_Stride; }
	constexpr size_t Height() const noexcept
	{ return vector::size() / m_Stride; }
	
	constexpr auto operator[] (size_t row) const
	{
		return std::ranges::subrange(
			next(vector::cbegin(),  row      * m_Stride), 
			next(vector::cbegin(), (row + 1) * m_Stride)
		);
	}
	constexpr auto operator[] (size_t row)
	{
		return std::ranges::subrange(
			next(vector::begin(),  row      * m_Stride),
			next(vector::begin(), (row + 1) * m_Stride)
		);
	}

	std::vector<bool> const& AsVector() const noexcept
	{ return *static_cast<std::vector<bool> const*>(this); }
	std::vector<bool> & AsVector() noexcept
	{ return *static_cast<std::vector<bool> *>(this); }

private:

	size_t m_Stride{};

};

class Space final : public tei::Component
{
	using Grid_t = Grid;
public:

	Space() = default;

	auto const& Grid() const noexcept
	{ return m_Grid; }
	auto & GridRef() noexcept
	{ return m_Grid; }
	
	auto const& Bodies() const noexcept
	{ return m_Bodies; }
	auto & BodiesRef() noexcept
	{ return m_Bodies; }

	void SetShip(Ship&) noexcept;
	units::Position const& ShipPosition() const noexcept;
	bool IsShipAlive() const noexcept;

	void AddBody(Body&);
	void RemoveBody(Body&);

private:

	virtual void OnEnable() override;
	virtual void OnUpdate() override;
	virtual void OnRender() const override;

	Grid_t m_Grid{};
	Body* m_pShipBody{};
	std::vector<Body*> m_Bodies{};
	int m_AlienCount{};
	bool m_IsAlive{ true };
	tei::Clock::duration m_EmptyTime{ 250_ms };

};