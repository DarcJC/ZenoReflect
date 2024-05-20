
#define ZENO_NONCOPYABLE(TypeName) \
	TypeName(TypeName&&) = delete; \
	TypeName(const TypeName&) = delete; \
	TypeName& operator=(const TypeName&) = delete; \
	TypeName& operator=(TypeName&&) = delete;

#define ZENO_DECLARE_FIELD_API(TClass, TSuperClass) \
public:\
	typedef TSuperClass Super;\
	typedef TClass ThisClass;\
	template<typename T> static constexpr bool is_same() { return std::is_same<T, TClass>::value; }
