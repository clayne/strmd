#pragma once

#include "type_traits.h"

#include <limits>

#pragma warning(disable:4127)

namespace strmd
{
	class varint
	{
	public:
		enum {
			bits_per_byte = 7,
			terminator = 1 << bits_per_byte,
			mask = terminator - 1
		};

	public:
		template <typename StreamT, typename T>
		static void pack(StreamT &stream, T value)
		{
			if (std::numeric_limits<T>::is_signed)
				pack_unsigned<typename remove_sign<T>::type>(stream, (value << 1) ^ (value >> (sizeof(T) * 8 - 1)));
			else
				pack_unsigned(stream, value);
		}

		template <typename StreamT, typename T>
		static void unpack(StreamT &/*stream*/, T &value)
		{
			value = 0;
		}

	private:
		template <typename T, typename StreamT>
		static void pack_unsigned(StreamT &stream, T value)
		{
			do
			{
				unsigned char b = value & mask;

				value >>= bits_per_byte;
				if (!value)
					b |= terminator;
				stream.write(&b, 1);
			} while (value);
		}
	};
}
