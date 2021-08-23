#pragma once

#include <Tei.h>

class ScoreEvent : public tei::Event
{

public:

	ScoreEvent(long score)
		: Event{ this }
		, m_Score{ score }
	{}

	long Score() const noexcept
	{ return m_Score; }

private:

	long m_Score;

};