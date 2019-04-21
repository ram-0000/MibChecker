#ifndef STRINGMATCH_H
#define STRINGMATCH_H

class StringMatch
{
public:
	inline static const char * SnmpTypeTranslate(int idx, bool raise_exception = true)
		{ return translate(idx, m_snmp_type_array, raise_exception); }
	inline static int SnmpTypeTranslate(const char *str, bool raise_exception = true)
		{ return translate(str, m_snmp_type_array, raise_exception); }

	inline static const char * SnmpStatusTranslate(int idx, bool raise_exception = true)
		{ return translate(idx, m_snmp_status_array, raise_exception); }
	inline static int SnmpStatusTranslate(const char * str, bool raise_exception = true)
		{ return translate(str, m_snmp_status_array, raise_exception); }

	inline static const char * SnmpAccessTranslate(int idx, bool raise_exception = true)
		{ return translate(idx, m_snmp_access_array, raise_exception); }
	inline static int SnmpAccessTranslate(const char * str, bool raise_exception = true)
		{ return translate(str, m_snmp_access_array, raise_exception); }

private:
		typedef struct
		{	int idx;
			const char * str;
		} Array_t;

	static Array_t m_snmp_type_array[];
	static Array_t m_snmp_status_array[];
	static Array_t m_snmp_access_array[];

	static const char * translate(int idx, const Array_t * array, bool raise_exception);
	static int translate(const char * str, const Array_t * array, bool raise_exception);

	StringMatch(void);  // only a static class, non constructable class
};

#endif // STRINGMATCH_H
