#include "IfsRemainingCounter.h"
#include "Debug.h"

IfsRemainingCounter::IfsRemainingCounter()
{
	Reset();
}

IfsRemainingCounter::~IfsRemainingCounter()
{
	if(IsEmpty() == false)
		ERREUR("Destroying a non empty counter");
	Reset();
}

void IfsRemainingCounter::Reset(void)
{
	m_table.clear();
}

void IfsRemainingCounter::Push(quint32 InitialValue)
{
	//DEBUG("RemainingCounter::Adding a counter=%d", InitialValue);
	m_table.push_back(InitialValue);
}

bool IfsRemainingCounter::Pop(void)
{
	//DEBUG("RemainingCounter::Removing last counter");
	// check
	if(Last() != 0)
	{
		ERREUR("Poping a non 0 value (%d) on counter", Last(), m_table.size() - 1);
		return false;
	}

	// remove
	m_table.pop_back();
	return true;
}

bool IfsRemainingCounter::Decremente(quint32 value)
{
	for(quint32 boucle = 0; boucle != value; boucle++)
	{
		if(Decremente() == false)
			return false;
	}
	return true;
}

bool IfsRemainingCounter::Decremente(void)
{
	for(int boucle = 0; boucle != m_table.size(); boucle++)
	{
		if(m_table[boucle] == 0)
		{
			ERREUR("Decrementing a 0 value for counter %d", boucle);
			return false;
		}
		m_table[boucle]--;
	}
	return true;
}

quint32 IfsRemainingCounter::Last(void) const
{
	return m_table.back();
}
