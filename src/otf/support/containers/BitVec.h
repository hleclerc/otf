#pragma once

#include "../utility/ByteEndianness.h"
#include "../display/Displayer.h"
#include "Vec.h"

/**
  Vector of bits.

  Display and index are little-endian, for the bytes AND for the bits (even if the hardware is big endian).

  [
    bit 0 of byte 0 (1 << 0),
    bit 1 of byte 0 (1 << 1),
    ...
    bit 0 of byte 1 (1 << 0),
    ...
  ]

  operator<< and operator>> follow the big-endian convention (the usual one for these operators).

*/
class BitVec {
public:
    using               Block                           = std::conditional<ByteEndianness::host==ByteEndianness::big,PI8,PI>::type;
    using               Byte                            = PI8;

    static BitVec       default_value                   ();

    static BitVec       from_content                    ( const void *beg, const void *end );
    static BitVec       from_content                    ( const void *beg, PI size );
    static BitVec       from_content                    ( StringView txt );
    static BitVec       from_display                    ( StringView sv );
    static BitVec       from_size                       ( PI size, bool value );
    static BitVec       from_size                       ( PI size );

    /**/                BitVec                          ( const BitVec &that );
    /**/                BitVec                          ( Vec<PI8> &&that );
    /**/                BitVec                          ( BitVec &&that );

    /**/                BitVec                          ( Size size, const void *content );
    /**/                BitVec                          ( Size size, bool default_value );
    /**/                BitVec                          ( Size size );

    /**/                BitVec                          ();

    /**/               ~BitVec                          ();

    bool                operator!=                      ( const BitVec &that ) const;
    bool                operator==                      ( const BitVec &that ) const;

    BitVec&             operator&=                      ( const BitVec &that );
    BitVec&             operator|=                      ( const BitVec &that );

    BitVec&             operator=                       ( const BitVec &that );
    BitVec&             operator=                       ( BitVec &&that );

    BitVec              operator<<                      ( PI n ) const;
    BitVec              operator>>                      ( PI n ) const;
    BitVec              operator&                       ( const BitVec &that ) const;
    BitVec              operator|                       ( const BitVec &that ) const;
    BitVec              operator~                       () const;

    void                display                         ( Displayer &sr, const String &prefix = {} ) const;

    PI                  nb_bits_in_last_word            () const;
    PI                  nb_blocks                       () const;
    PI                  nb_bytes                        () const;
    PI                  nb_bits                         () const;
    bool                empty                           () const;
    PI                  size                            () const;

    bool                all_false                       () const;
    bool                all_true                        () const;

    void                resize_without_copy             ( PI new_size );

    void                set_false                       ( PI beg, PI end );
    void                set_false                       ( PI ind );
    void                set_true                        ( PI beg, PI end );
    void                set_true                        ( PI ind );

    void                set_range                       ( PI beg, PI end, bool value );
    void                set_if                          ( PI beg, const BitVec &cond, const BitVec &value );

    void                copy_to                         ( void *data ) const;

    void                set_sub_vec_conditional         ( PI beg, const BitVec &value, const BitVec &condition );
    void                set_sub_vec                     ( PI beg, const BitVec &value );
    BitVec              sub_vec                         ( PI beg, PI end ) const;

    const Block&        block                           ( PI num_word ) const;
    Block&              block                           ( PI num_word );

    const Byte&         byte                            ( PI num_word ) const;
    Byte&               byte                            ( PI num_word );

    void                set_bit                         ( PI index, bool value );
    bool                bit                             ( PI index ) const;

    const Byte*         bytes                           () const;
    Byte*               bytes                           ();

    String              to_String                       () const;

private:
    static constexpr PI nb_bits_in_a_block              = sizeof( Block ) * 8;
    static constexpr PI nb_bits_in_a_byte               = sizeof( Byte ) * 8;
    static constexpr PI nb_bits_in_a_bit                = 1;
    static constexpr PI nb_local_blocks                 = std::max( 1ul, 64ul / nb_bits_in_a_block );
    static constexpr PI local_size                      = nb_local_blocks * nb_bits_in_a_block;

    bool                ext_data                        () const { return _size > local_size; }

    Block               _local_data[ nb_local_blocks ]; ///< used only if _size <= local_size
    PI                  _size;                          ///< nb bits
    Block*              _ptr;                           ///< reserved size % sizeof( Block ) is guaranted to be = 0 (so that we can use "words")
};

