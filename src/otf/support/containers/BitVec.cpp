#include "../utility/ASSERT.h"
#include "BitVec.h"

#include <sstream>
#include <utility>
#include <cstring>

PI BitVec::nb_bits_in_last_word() const {
    return ( _size - 1u ) % nb_bits_in_a_block + 1u;
}

PI BitVec::nb_blocks() const {
    return ( _size + nb_bits_in_a_block - 1u ) / nb_bits_in_a_block;
}

PI BitVec::nb_bytes() const {
    return ( _size + nb_bits_in_a_byte - 1u ) / nb_bits_in_a_byte;
}

PI BitVec::nb_bits() const {
    return _size;
}

bool BitVec::empty() const {
    return _size == 0;
}

PI BitVec::size() const {
    return _size;
}

const BitVec::Block &BitVec::block( PI num_word ) const {
    return _ptr[ num_word ];
}

BitVec::Block &BitVec::block( PI num_word ) {
    return _ptr[ num_word ];
}

const BitVec::Byte &BitVec::byte( PI num_byte ) const {
    return reinterpret_cast<const Byte *>( _ptr )[ num_byte ];
}

BitVec::Byte &BitVec::byte( PI num_byte ) {
    return reinterpret_cast<Byte *>( _ptr )[ num_byte ];
}

bool BitVec::bit( PI index ) const {
    return block( index / nb_bits_in_a_block ) & ( Block( 1 ) << index % nb_bits_in_a_block );
}

BitVec BitVec::default_value() {
    return {};
}

const BitVec::Byte *BitVec::bytes() const {
    return reinterpret_cast<const Byte *>( _ptr );
}

BitVec::Byte *BitVec::bytes() {
    return reinterpret_cast<Byte *>( _ptr );
}

String BitVec::to_String() const {
    return { reinterpret_cast<const char *>( bytes() ), size() / 8 };
}

BitVec::BitVec( Size size ) {
    _ptr = size.value > local_size ?
        new Block[ ( size.value + nb_bits_in_a_block - 1 ) / nb_bits_in_a_block ] :
        _local_data;
    _size = size.value;
}

BitVec::BitVec( Size size, bool default_value ) : BitVec( size ) {
    if ( default_value )
        for( PI i = 0; i < nb_blocks(); ++i )
            block( i ) = Block( -1 );
    else
        for( PI i = 0; i < nb_blocks(); ++i )
            block( i ) = Block( 0 );
}

BitVec::BitVec( const BitVec &that ) : BitVec( Size{ that._size } ) {
    std::memcpy( bytes(), that.bytes(), that.nb_bytes() );
}

BitVec::BitVec( Vec<PI8> &&that ) {
    _size = 8 * that.size();

    if ( _size <= local_size ) {
        std::memcpy( _local_data, that.ptr(), that.size() );
        _ptr = _local_data;
        return;
    }

    _ptr = reinterpret_cast<Block *>( that.moved_data().ptr );
}

BitVec::BitVec( BitVec &&that ) {
    _size = std::exchange( that._size, 0 );

    if ( _size <= local_size ) {
        std::memcpy( _local_data, that._local_data, nb_bytes() );
        _ptr = _local_data;
    } else
        _ptr = std::exchange( that._ptr, that._local_data );
}

BitVec::BitVec( Size size, const void *content ) : BitVec( size ) {
    std::memcpy( _ptr, content, nb_bytes() );
}

BitVec::BitVec() {
    _ptr = _local_data;
    _size = 0;
}

BitVec::~BitVec() {
    if ( _size > local_size )
        delete [] _ptr;
}

bool BitVec::operator!=( const BitVec &that ) const {
    if ( size() != that.size() )
        return true;
    const PI n = _size / nb_bits_in_a_block;
    for( PI i = 0; i < n; ++i )
        if ( _ptr[ i ] != that._ptr[ i ] )
            return true;
    if ( n < nb_blocks() ) {
        Block msk = ~ ( Block( -1 ) << _size % nb_bits_in_a_block );
        if ( ( _ptr[ n ] & msk ) != ( that._ptr[ n ] & msk ) )
            return true;
    }
    return false;
}

bool BitVec::operator==( const BitVec &that ) const {
    if ( size() != that.size() )
        return false;
    const PI n = _size / nb_bits_in_a_block;
    for( PI i = 0; i < n; ++i )
        if ( _ptr[ i ] != that._ptr[ i ] )
            return false;
    if ( n < nb_blocks() ) {
        Block msk = ~ ( Block( -1 ) << _size % nb_bits_in_a_block );
        if ( ( _ptr[ n ] & msk ) != ( that._ptr[ n ] & msk ) )
            return false;
    }
    return true;
}

BitVec BitVec::from_content( const void *beg, const void *end ) {
    const PI nb_bytes = reinterpret_cast<const PI8 *>( end ) - reinterpret_cast<const PI8 *>( beg );
    BitVec res = Size{ nb_bytes * nb_bits_in_a_byte };
    std::memcpy( res.bytes(), beg, nb_bytes );
    return res;
}

BitVec BitVec::from_content( const void *beg, PI size ) {
    return { Size{ size }, beg };
}

BitVec BitVec::from_content( StringView txt ) {
    return from_content( txt.data(), 8 * txt.size() );
}

BitVec BitVec::from_size( PI size, bool value ) {
    return { Size{ size }, value };
}

BitVec BitVec::from_size( PI size ) {
    return Size{ size };
}

BitVec BitVec::from_display( StringView sv ) {
    PI s = 0;
    for( PI i = 0; i < sv.size(); ++i )
        s += sv[ i ] != '_';

    BitVec res( Size{ s } );
    for( PI i = 0, o = 0; i < sv.size(); ++i )
        if ( sv[ i ] != '_' )
            res.set_bit( o++, sv[ i ] == '1' );
    return res;
}


BitVec &BitVec::operator=( const BitVec &that ) {
    resize_without_copy( that._size );
    std::memcpy( _ptr, that._ptr, that.nb_bytes() );
    return *this;
}

BitVec &BitVec::operator=( BitVec &&that ) {
    if ( ext_data() )
        delete [] _ptr;

    if ( that.ext_data() ) {
        _ptr = std::exchange( that._ptr, that._local_data );
        _size = std::exchange( that._size, 0 );
    } else {
        _ptr = _local_data;
        _size = std::exchange( that._size, 0 );
        std::memcpy( _ptr, that._local_data, nb_bytes() );
    }

    return *this;
}

BitVec BitVec::operator<<( PI shift ) const {
    const PI beg_wd = shift / nb_bits_in_a_block, end_wd = ( shift + nb_bits() + nb_bits_in_a_block - 1 ) / nb_bits_in_a_block;
    const PI sb = shift % nb_bits_in_a_block;
    BitVec res = Size{ shift + nb_bits() };

    // blocks of 0 at the beginning
    for( PI i = 0; i < beg_wd; ++i )
        res.block( i ) = 0;

    // fast path for aligned offset
    if ( sb == 0 ) {
        for( PI i = beg_wd; i < end_wd; ++i )
            res.block( i ) = block( i - beg_wd );
        return res;
    }

    // write the first word (00xyz => yz000)
    res.block( beg_wd ) = block( 0 ) << sb;

    // write middle + last
    if ( end_wd - beg_wd > nb_blocks() ) {
        for( PI i = beg_wd + 1; i < end_wd - 1; ++i )
            res.block( i ) = ( block( i - ( beg_wd + 1 ) ) >> ( nb_bits_in_a_block - sb ) ) | ( block( i - beg_wd ) << sb );
        res.block( end_wd - 1 ) = block( end_wd - 1 - ( beg_wd + 1 ) ) >> ( nb_bits_in_a_block - sb );
    } else {
        for( PI i = beg_wd + 1; i < end_wd; ++i )
            res.block( i ) = ( block( i - ( beg_wd + 1 ) ) >> ( nb_bits_in_a_block - sb ) ) | ( block( i - beg_wd ) << sb );
    }

    return res;
}

BitVec BitVec::operator>>( PI n ) const {
    if ( n >= nb_bits() )
        return {};
    return sub_vec( n, nb_bits() );
}

BitVec BitVec::operator~() const {
    BitVec res = BitVec::from_size( _size );
    for( PI i = 0; i < nb_blocks(); ++i )
        res.block( i ) = ~ block( i );
    return res;
}

BitVec &BitVec::operator&=( const BitVec &that ) {
    const PI nb_full_blocks_in_that = that.nb_bits() / nb_bits_in_a_block;
    ASSERT( that.nb_bits() <= nb_bits() );

    for( PI i = 0; i < nb_full_blocks_in_that; ++i )
        block( i ) &= that.block( i );
    for( PI i = nb_full_blocks_in_that; i < that.nb_blocks(); ++i )
        block( i ) &= that.block( i ) & ( Block( -1 ) >> ( nb_bits_in_a_block - that.nb_bits() % nb_bits_in_a_block ) );
    return *this;
}

BitVec &BitVec::operator|=( const BitVec &that ) {
    const PI nb_full_blocks_in_that = that.nb_bits() / nb_bits_in_a_block;
    ASSERT( that.nb_bits() <= nb_bits() );

    for( PI i = 0; i < nb_full_blocks_in_that; ++i )
        block( i ) |= that.block( i );
    for( PI i = nb_full_blocks_in_that; i < that.nb_blocks(); ++i )
        block( i ) |= that.block( i ) & ( Block( -1 ) >> ( nb_bits_in_a_block - that.nb_bits() % nb_bits_in_a_block ) );
    return *this;
}

BitVec BitVec::operator&( const BitVec &that ) const {
    BitVec res = *this;
    res &= that;
    return res;
}

BitVec BitVec::operator|( const BitVec &that ) const {
    BitVec res = *this;
    res |= that;
    return res;
}

void BitVec::copy_to( void *data ) const {
    std::memcpy( data, _ptr, nb_bytes() );
}

void BitVec::set_sub_vec_conditional( PI beg, const BitVec &value, const BitVec &condition ) {
    ASSERT( value.size() == condition.size() );
    if ( beg )
        return set_sub_vec_conditional( 0, value << beg, condition << beg );

    //
    const PI nb_full_blocks_in_condition = condition.nb_bits() / nb_bits_in_a_block;
    for( PI i = 0; i < nb_full_blocks_in_condition; ++i )
        block( i ) = ( block( i ) & ~condition.block( i ) ) | ( value.block( i ) & condition.block( i ) );

    for( PI i = nb_full_blocks_in_condition; i < condition.nb_blocks(); ++i ) {
        const Block mask = condition.block( i ) & ( Block( -1 ) >> ( nb_bits_in_a_block - condition.nb_bits() % nb_bits_in_a_block ) );
        block( i ) = ( block( i ) & ~mask ) | ( value.block( i ) & mask );
    }
}

void BitVec::display( Displayer &sr, const String &prefix ) const {
    std::ostringstream res;
    res << prefix;

    for( PI b = 0; b < nb_bits(); ++b ) {
        if ( b && b % 8 == 0 )
            res << '_';
        res << ( bit( b ) ? '1' : '0' );
    }

    sr << res.str();
}

bool BitVec::all_false() const {
    const PI mw = _size / nb_bits_in_a_block;
    const PI mb = _size / nb_bits_in_a_byte;

    // TODO: optimize
    for( PI i = 0; i < mw; ++i )
        if ( block( i ) )
            return false;

    for( PI i = mw * nb_bits_in_a_block / nb_bits_in_a_byte; i < mb; ++i )
        if ( byte( i ) )
            return false;

    for( PI i = mb * nb_bits_in_a_byte / nb_bits_in_a_bit; i < nb_bits(); ++i )
        if ( bit( i ) )
            return false;

    return true;
}

bool BitVec::all_true() const {
    const PI mw = _size / nb_bits_in_a_block;
    const PI mb = _size / nb_bits_in_a_byte;

    // TODO: optimize
    for( PI i = 0; i < mw; ++i )
        if ( Block( ~ block( i ) ) )
            return false;

    for( PI i = mw * nb_bits_in_a_block / nb_bits_in_a_byte; i < mb; ++i )
        if ( Byte( ~ byte( i ) ) )
            return false;

    for( PI i = mb * nb_bits_in_a_byte / nb_bits_in_a_bit; i < nb_bits(); ++i )
        if ( ! bit( i ) )
            return false;

    return true;
}

void BitVec::resize_without_copy( PI new_size ) {
    *this = Size{ new_size };
}

void BitVec::set_false( PI beg, PI end ) {
    if ( beg == end )
        return;

    // all on the same word
    if ( beg / nb_bits_in_a_block == end / nb_bits_in_a_block ) {
        PI n = ( Block( -1 ) >> ( nb_bits_in_a_block - ( end % nb_bits_in_a_block - beg % nb_bits_in_a_block ) ) ) << beg % nb_bits_in_a_block;
        block( beg / nb_bits_in_a_block ) &= ~n;
        return;
    }

    // first word
    block( beg / nb_bits_in_a_block ) &= ~( Block( -1 ) << beg % nb_bits_in_a_block );

    // middle
    for( PI i = beg / nb_bits_in_a_block + 1; i < end / nb_bits_in_a_block; ++i )
        block( i ) = 0;

    // last word
    block( end / nb_bits_in_a_block ) &= Block( -1 ) << end % nb_bits_in_a_block;
}

void BitVec::set_false( PI ind ) {
    byte( ind / nb_bits_in_a_byte ) &= ~( 1 << ind % nb_bits_in_a_byte );
}

void BitVec::set_true( PI beg, PI end ) {
    if ( beg == end )
        return;

    if ( ByteEndianness::host == ByteEndianness::little ) {
        // all on the same word
        if ( beg / nb_bits_in_a_block == end / nb_bits_in_a_block ) {
            PI n = ( Block( -1 ) >> ( nb_bits_in_a_block - ( end % nb_bits_in_a_block - beg % nb_bits_in_a_block ) ) ) << beg % nb_bits_in_a_block;
            block( beg / nb_bits_in_a_block ) |= n;
            return;
        }

        // first word
        block( beg / nb_bits_in_a_block ) |= Block( -1 ) << beg % nb_bits_in_a_block;

        // middle
        for( PI i = beg / nb_bits_in_a_block + 1; i < end / nb_bits_in_a_block; ++i )
            block( i ) = Block( -1 );

        // last word
        block( end / nb_bits_in_a_block ) |= ~ ( Block( -1 ) << end % nb_bits_in_a_block );
    } else {
        TODO;
    }
}

void BitVec::set_true( PI ind ) {
    byte( ind / nb_bits_in_a_byte ) |= 1 << ind % nb_bits_in_a_byte;
}

void BitVec::set_if( PI beg, const BitVec &cond, const BitVec &value ) {
    TODO;
    //    // TODO: optimize

    //    const PI s = beg / bs;
    //    if ( beg % bs )
    //        return set_if( s * bs, cond << beg % bs, value << beg % bs );

    //    ASSERT( cond._size == value._size );
    //    if ( value._size )
    //        for( PI i = 0; i <= value._nb_ext_words; ++i )
    //            w( i + s ) = ( value.w( i ) & cond.wz( i ) ) | ( w( i + s ) & ~ cond.wz( i ) );
}

void BitVec::set_range( PI beg, PI end, bool value ) {
    if ( value )
        return set_true( beg, end );
    return set_false( beg, end );
}

void BitVec::set_bit( PI ind, bool value ) {
    if ( value )
        return set_true( ind );
    return set_false( ind );
}

void BitVec::set_sub_vec( PI beg, const BitVec &value ) {
    const PI end = beg + value.nb_bits();
    ASSERT( end <= nb_bits() );
    if ( value.nb_bits() == 0 )
        return;

    set_range( beg, end, false );
    operator|=( value << beg );
}


BitVec BitVec::sub_vec( PI beg, PI end ) const {
    ASSERT( end <= nb_bits() );
    ASSERT( beg <= end );

    const PI first_word_to_take = beg / nb_bits_in_a_block;
    const PI shift = beg % nb_bits_in_a_block;
    BitVec res = Size{ end - beg };

    // fast path if aligned offset
    if ( shift == 0 ) {
        for( PI i = 0; i < res.nb_blocks(); ++i )
            res.block( i ) = block( first_word_to_take + i );
        return res;
    }

    // test if last block is a sum
    if ( first_word_to_take + res.nb_blocks() >= nb_blocks() ) {
        for( PI i = 0; i < res.nb_blocks() - 1; ++i )
            res.block( i ) = ( block( first_word_to_take + i ) >> shift ) | ( block( first_word_to_take + i + 1 ) << ( nb_bits_in_a_block - shift ) );
        res.block( res.nb_blocks() - 1 ) = block( res.nb_blocks() - 1 + first_word_to_take ) >> shift;
    } else {
        for( PI i = 0; i < res.nb_blocks(); ++i )
            res.block( i ) = ( block( i + first_word_to_take ) >> shift ) | ( block( i + first_word_to_take + 1 ) << ( nb_bits_in_a_block - shift ) );
    }

    return res;
}
