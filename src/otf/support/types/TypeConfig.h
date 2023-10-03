#pragma once

#include <type_traits>
#include <string_view>
#include <cstdint>
#include <string>

using StringView = std::string_view;
using String     = std::string;

using PI64       = std::conditional< sizeof( unsigned long ) == 8, unsigned long, unsigned long long >::type;
using PI32       = std::uint32_t;
using PI32       = std::uint32_t;
using PI16       = std::uint16_t;
using PI8        = std::uint8_t;

using SI64       = std::conditional< sizeof( signed long ) == 8, signed long, signed long long >::type;
using SI32       = std::int32_t;
using SI16       = std::int16_t;
using SI8        = std::int8_t;

using Bool       = bool;

using Byte       = std::byte;

using PI         = std::conditional< sizeof( void * ) == 8, PI64, PI32 >::type;
using SI         = std::conditional< sizeof( void * ) == 8, SI64, SI32 >::type;

using FP64       = double;
using FP32       = float;

#define TCAB template<class A,class B>
#define TCA template<class... A>
#define TCT template<class T>
#define TCU template<class U>
#define TCI template<int i>
#define TCJ template<int i,int j>

struct Size { PI value; };
struct Fill {};

#include "../utility/ASSERT.h" // IWYU pragma: export
#include "../utility/TODO.h" // IWYU pragma: export
