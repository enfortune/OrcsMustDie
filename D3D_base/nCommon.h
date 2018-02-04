#pragma once

// === macro =============================================================================================================

#define HAS_COMMA_IMPL(a, b, ...) 0 ## b
#define HAS_COMMA(...) HAS_COMMA_IMPL(__VA_ARGS__, 1)

// should use this to check compiler type
#define IS_VA_ARGS_NON_STANDARD(...) HAS_COMMA(dummy, ## __VA_ARGS__)
//#if defined(_MSC_VER) || defined(__GNUC__) /* || defined(__TURBOC__) */ /* borland, clang, beluga, ... */

// Visual Studio 2013 and below is not support noexcept keyword
#if _MSC_VER < 1900
#define noexcept 
#endif

// --- for class signature -----------------------------------------------------------------------------------------------

#define GETTER_BOOL(var, funcName) \
inline bool is##funcName() const noexcept { return var; }

#define GETTER(type, var, funcName) \
inline type get##funcName() noexcept { return var; }

// recommend to use expression 'constexpr GETTER_CONST(type, var, funcName);' 
#define GETTER_CONST(type, var, funcName) \
inline type get##funcName() const noexcept { return var; }

#define SETTER(type, var, funcName) \
inline void set##funcName(type value) noexcept { var = value; }

#define SETTER_MOVE(type, var, funcName) \
inline void set##funcName(type value) noexcept { var = std::move(value); }

#define SETTER_FORWARD(type, var, funcName) \
inline void set##funcName(type value) noexcept { var = std::forward<type>(value); }

#define SETTER_REF_BOTH(type, var, funcName) \
inline void set##funcName(const type & value) noexcept { var = value; } \
inline void set##funcName(type && value) noexcept { var = std::move(value); }

// rule of three
#define COPYABLE_DEFAULT(className) \
className(const className &) noexcept = default; \
className & operator=(const className &) noexcept = default;	\
className(className &&) noexcept = default; \
className & operator=(className &&) noexcept = default;

#define UNCOPYABLE(className) \
className(const className &) = delete; \
className(className &&) = delete; \
className & operator=(const className &) = delete; \
className & operator=(className &&) = delete;			// implicit delete

// --- for comfortable code ----------------------------------------------------------------------------------------

#if defined(_MSC_VER) && _MSC_VER <= 1910
#define FOR_EACH(var, container) for each(var in container)		/* up to Visual Studio 2017 Community */
#else
#define FOR_EACH(var, container) for each(var : container)		/* c++11 standard */
#endif

// should use template function version instead
// but isn't same expression between macro and template function
#define RUN_IF_PTR_EXIST(ptr, expression) \
{ auto check = (ptr); if(check) { ptr->expression; } }

// carefully use if expression have ','
#define STATIC_ASSERT(expression) static_assert((expression), #expression)

// 'HRESULT hr = S_OK;' required
// if need assertion, should use assert(expression == S_OK)
// IF_HRESULT_FAIL_RETURN_HR -> IF_HRESULT_FAIL(...) return hr;
#define IF_HRESULT_FAIL(expression) \
hr = expression; if(hr != S_OK)

// --- STL ---------------------------------------------------------------------------------------------------------

#define FOR_STL_INT_ALL(var, container) \
for(int var = 0; var < static_cast<int> (container.size()); ++var)

#define FOR_STL_ITERATOR_ALL(iter, container) \
for(auto iter = container.begin(); iter != container.end(); ++iter)

#define FOR_STL_INT_ALL_REVERSE(var, container) \
for(int var = static_cast<int> (container.size()) - 1; var >= 0; --var)

#define FOR_STL_ITERATOR_ALL_REVERSE(iter, container) \
for(auto iter = container.rbegin(); iter != container.rend(); ++iter)

// do not use such as 'else IF_STL_FIND_SUCCESS()' expression
// should use 'else { IF_STL_FIND_SUCCESS() ... }' instead
#define IF_STL_FIND_SUCCESS(iter, container, key) \
auto iter = container.find(key); if(iter != container.end())

#define IF_STL_INSERT_SUCCESS(iter, container, expression) \
auto iter = container.insert(expression); if(iter.second)

#define IF_MAP_INSERT_SUCCESS(iter, container, key, value) \
auto iter = container.insert(std::make_pair(key, value)); if(iter.second)

// emplace is faster than insert normally (one less call of destructor at least)
// support both standard and non-standard __VA_ARGS__ extension
#if IS_VA_ARGS_NON_STANDARD()

#define IF_STL_EMPLACE_SUCCESS(iter, container, ...) \
auto iter = container.emplace(__VA_ARGS__); if(iter.second)

#else

#define IF_STL_EMPLACE_SUCCESS(iter, container) \
auto iter = container.emplace(); if(iter.second)

#define IF_STL_EMPLACE_SUCCESS(iter, container, ...) \
auto iter = container.emplace(__VA_ARGS__); if(iter.second)

#endif

#define IF_STL_FIND_FAIL(container, key) \
if(container.find(key) == container.end())

#define IF_STL_INSERT_FAIL(container, expression) \
if(container.insert(expression).second == false)

#define IF_MAP_INSERT_FAIL(container, key, value) \
if(container.insert(std::make_pair(key, value)).second == false)

#define IF_STL_EMPLACE_FAIL(container, ...) \
if(container.emplace(__VA_ARGS__).second == false)

#define FOR_STL_ITERATOR_ERASE_ALL(iter, container, expression) \
{ for(auto iter = container.begin(); iter != container.end();) \
  { if(expression) iter = container.erase(iter); \
    else ++iter; \
  } \
}

// --- unusual case ------------------------------------------------------------------------------------------------

#ifndef DEBUG_TYPE
template <typename T> class ______DebugType;				// declare only
#define DEBUG_TYPE(var) ______DebugType<decltype(var)> 
#endif

// write 'DEBUG_TYPE(var) result'
// and check result in IDE to get template type data

// === function ====================================================================================================

template <typename T, int N> constexpr const int arraySize(T(&)[N]) { return N; }

// --- safe delete, release, Release -------------------------------------------------------------------------------

template <typename T> void deleteSafe(T * var)
{
	// checked delete
	using checkCompletePtrType = char [sizeof(T) ? 1 : -1];
	(void) sizeof(checkCompletePtrType);

	delete var; var = nullptr;
}

template <typename T> void deleteArraySafe(T * var)
{
	// checked delete
	using checkCompletePtrType = char [sizeof(T) ? 1 : -1];
	(void) sizeof(checkCompletePtrType);

	delete[] var; var = nullptr;
}

template <typename T> void releaseSafe(T * var) { if (var) { var->release(); var = nullptr; } }
template <typename T> void ReleaseSafe(T * var) { if (var) { var->Release(); var = nullptr; } }
template <typename T> void ReleaseCOM(T * var) { if (var) { assert(var->Release() == 0); var = nullptr; } }

// --- template impl struct ----------------------------------------------------------------------------------------

#define WHAT_STL_CONTAINER_SAFE_IMPL(prefix) \
template <typename T> \
struct prefix ## STLContainerSafeImpl \
{ \
/* static void impl(std::array<T, size> &) = delete; */ \
	/*template <typename T2, int N> static void impl(std::array<T2, N> &) = delete;*/ \
	static void impl(std::string &) = delete; \
	static void impl(std::wstring &) = delete; \
\
	/*static void impl(std::vector<T> & container); /* wrong signature, need to fix */ \
	static void impl(T & container); \
}

namespace		// nameless
{
	// do not specialize non-member function template

	WHAT_STL_CONTAINER_SAFE_IMPL(delete);
	WHAT_STL_CONTAINER_SAFE_IMPL(release);
	WHAT_STL_CONTAINER_SAFE_IMPL(Release);

	/*template<typename T> void deleteSTLContainerSafeImpl<T>::impl(std::vector<T> & container)
	{ 
		FOR_STL_INT_ALL(container)
			delete container[i];
	
		container.clear();
	}*/

	template<typename T> void deleteSTLContainerSafeImpl<T>::impl(T & container)
	{
		FOR_STL_ITERATOR_ALL(iter, container)
			delete *iter;

		container.clear();
	}

	/*template<typename T> void releaseSTLContainerSafeImpl<T>::impl(std::vector<T> & container)
	{
		FOR_STL_INT_ALL(container)
			container[i].release();

		container.clear();
	}*/

	template<typename T> void releaseSTLContainerSafeImpl<T>::impl(T & container)
	{
		FOR_STL_ITERATOR_ALL(iter, container)
			iter->release();

		container.clear();
	}

	/*template<typename T> void ReleaseSTLContainerSafeImpl<T>::impl(std::vector<T> & container)
	{
		FOR_STL_INT_ALL(container)
			container[i].Release();

		container.clear();
	}*/

	template<typename T> void ReleaseSTLContainerSafeImpl<T>::impl(T & container)
	{
		FOR_STL_ITERATOR_ALL(iter, container)
			iter->Release();

		container.clear();
	}
}

#undef WHAT_STL_CONTAINER_SAFE_IMPL

// --- template function signature ---------------------------------------------------------------------------------

template <typename T> void deleteSTLContainerSafe(T & container) { deleteSTLContainerSafeImpl<T>::impl(container); }
template <typename T> void releaseSTLContainerSafe(T & container) { releaseSTLContainerSafeImpl<T>::impl(container); }
template <typename T> void ReleaseSTLContainerSafe(T & container) { ReleaseSTLContainerSafeImpl<T>::impl(container); }

// should use carefully for virtual function
// example of correct function value : &CameraD3D9::WndProc
// do not return because of '0 == SUCCESS' function
template <typename T, typename pfT, typename ... Types>
void runIfPtrExist(T * ptr, pfT function, Types ... args)
{ if (ptr) (ptr->*function)(args...); }

// === revision ====================================================================================================

// before : create
// 2018.01.10 : delete, release
// 2018.01.13 : RUN_IF_PTR_EXIST(), runIfPtrExist()
// 2018.01.14 : preclude ASSERT_HRESULT() - should use assert(expression == S_OK) instead
//            : FOR_STL_INT_ALL(), FOR_STL_ITERATOR_ALL(), FOR_EACH()
//            : WHAT_STL_CONTAINER_SAFE_IMPL()
// 2018.01.16 : IF_MAP_FIND_SUCCESS(), IF_STL_INSERT_SUCCESS(), ...
//            : SETTER_MOVE(), SETTER_FORWARD()
//            : IF_STL_EMPLACE_SUCCESS() __VA_ARGS__ treatment
// 2018.01.17 : IS_VA_ARGS_NON_STANDARD(), standard wrapper of __VA_ARGS__, find bug on macros
// 2018.01.18 : SETTER_REF_BOTH()
// 2018.01.23 : GETTER_BOOL()
// 2018.01.25 : checked delete technique, IF_STL_FIND_FAIL()
// 2018.01.27 : FOR_STL_ITERATOR_ERASE_ALL(), IF_STL_INSERT_FAIL(), EMPLACE_FAIL(), ...
// 2018.01.29 : exclude noexcept for Visual Studio 2013 or below

// === to do =======================================================================================================

// improve WHAT_STL_CONTAINER_SAFE_IMPL(), std::array support or delete, fix vector specialization
// WHAT_STL_CONTAINER_SAFE debug message isn't view correct error code line...

// __VA_OPT__ ?
// is impossible #define in #define ...
// fix mismatched expression of macro function that have 0 count parameter (gets 1 count always)
// bug on macros

// C++14 extend through using macro