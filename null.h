#ifndef __NULL_H__
#define __NULL_H__

#if defined(__GNUG__) && defined(__GNUC__) && __GNUC__ >= 4
#	define null __null
#else

class __null {
public:
	template<class T>
	operator T*() const { return 0; }
	template<class C, class T>
	operator T C::*() const { return 0; }
private:
	void operator&() const;
};

const __Null::__null null = __Null::__null();
#endif

#endif
