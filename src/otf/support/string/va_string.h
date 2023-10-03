#pragma once

#include "to_string.h"
#include <iostream>

/// Ex: "... {0} {KEY} ...", "84", "KEY:", "smurf" => "... 84 smurf ..."
String va_string_repl_vec( const Vec<String> &values );

/// Ex: "... {0} {KEY} ...", val_0, "KEY:", val_KEY => "... 84 smurf ..."
TCA String va_string( const A &...args ) { return va_string_repl_vec( Vec<String>::from_values( to_string( args )... ) ); }
