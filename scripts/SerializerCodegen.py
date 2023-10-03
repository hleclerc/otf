import os

class Cg:
    def __init__( self, h_file, c_file, pos_includes, writer, includes ) -> None:
        self.pos_includes = pos_includes
        self.includes = includes
        self.writer = writer
        self.h_file = h_file
        self.c_file = c_file

        self.c_sp = ""
        self.h_sp = ""

    def c( self, str: str ):
        if str.startswith( "}" ) or str.endswith( ':' ):
            self.c_sp = self.c_sp[ 4: ]
        self.c_file.write( self.c_sp + str + "\n" )
        if str.endswith( "{" ) or str.endswith( ':' ):
            self.c_sp += "    "

    def h( self, str: str ):
        if str.startswith( "}" ) or str.endswith( ':' ):
            self.h_sp = self.h_sp[ 4: ]
        self.h_file.write( self.h_sp + str + "\n" )
        if str.endswith( "{" ) or str.endswith( ':' ):
            self.h_sp += "    "

class Attr:
    def __init__( self, name, type ) -> None:
        self.name = name 
        self.type = type
        self.last = False


class Class:
    def __init__( self, name, attrs, ext_attrs, parent, writer ) -> None:
        self.name = name
        self.parent = parent
        self.writer = writer
        self.children = []
        self.write_sized = True

        self.attrs = []
        for name, type in attrs:
            self.attrs.append( Attr( name, type ) )

        self.ext_attrs = []
        for name, type in ext_attrs:
            self.ext_attrs.append( Attr( name, type ) )

    def needs( self, type ):
        for attr in self.attrs:
            if attr.type == type:
                return True
        return False

    def write( self, cg : Cg ):
        # includes
        self.write_includes( cg )

        # decl
        if len( self.children ):
            self.write_parent_class_content( cg )
        else:
            self.write_leaf_class_content( cg )

    def write_includes( self, cg : Cg ):
        decls = {}
        incls = {}
        extis = {}
        cppis = { self.name: 1 }
        for ch in self.children:
            decls[ ch.name ] = 1
            cppis[ ch.name ] = 1
            for attr in ch.attrs + ch.ext_attrs:
                type = attr.type
                if c := self.container_type( type ):
                    extis[ f'{ cg.pos_includes }/support/binary/{ c[ 0 ] }.h' ] = 1
                    type = c[ 1 ]

                if not self.base_type( type ):
                    decls[ type ] = 1

        for incl in cg.includes:
            extis[ incl ] = 1

        for attr in self.attrs + self.ext_attrs:
            if self.base_type( attr.type ):
                continue
            if c := self.container_type( attr.type ):
                extis[ f'{ cg.pos_includes }/support/binary/{ c[ 0 ] }.h' ] = 1
                if not self.base_type( c[ 1 ] ):
                    decls[ c[ 1 ] ] = 1
                    cppis[ c[ 1 ] ] = 1
                continue

            incls[ attr.type ] = 1

        for incl in incls:
            if incl in decls:
                decls.pop( incl )

        # .cpp
        for incl in cppis:
            cg.c( f'#include "{ incl }.h"' )
        cg.c( "" )

        # .h        
        cg.h( f'#pragma once' )
        cg.h( f'' )
        cg.h( f'#include "{ cg.pos_includes }/support/display/Displayer.h"' )
        cg.h( f'#include "{ cg.pos_includes }/support/utility/ERROR.h"' )
        cg.h( f'' )
        cg.h( f'#include "{ cg.pos_includes }/support/binary/BioWriterFor.h"' )
        cg.h( f'#include "{ cg.pos_includes }/support/binary/BioReader.h"' )
        for exti in extis:
            cg.h( f'#include "{ exti }"' )
        cg.h( f'' )

        for incl in incls:
            cg.h( f'#include "{ incl }.h"' )
        for decl in decls:
            cg.h( f'class { decl };' )
        cg.h( f'' )
        
    def container_type( self, type : str ):
        if type.startswith( "BioVec<" ):
            return "BioVec", type[ 7: -1 ]
        if type.startswith( "BioOpt<" ):
            return "BioOpt", type[ 7: -1 ]
        return None

    def base_type( self, type ):
        return type in [ "StringView", "PI", "Bool" ]

    def write_parent_class_content( self, cg ):
        cg.h( f"class { self.name } {{" )
        cg.h( f"public:" )
        cg.h( "enum { " + ", ".join( [ f"num_{ ch.name }" for ch in self.children ] ) + " };" )
        cg.h( "" )

        # Visitor
        cg.h( f"struct Visitor {{" )
        for ch in self.children:
            cg.h( f"virtual void operator()( const { ch.name } & ) = 0;" )
        cg.h( f"}};" )
        cg.h( f"" )

        # read Ctor
        cg.h( f"{ self.name }( BioReader &reader, bool last_in_sized ) {{" )
        cg.h( f"data = reader.read_bytes( last_in_sized );" )
        cg.h( f"}}" )
        cg.h( f"" )

        # void Ctor
        cg.h( f"{ self.name }() {{" )
        cg.h( f"}}" )
        cg.h( f"" )

        # visit
        cg.h( f"void visit( Visitor &visitor ) const;" )

        cg.c( f"void { self.name }::visit( Visitor &visitor ) const {{" )
        cg.c( f"BioReader br( data );" )
        cg.c( f"PI8 type = br.read_PI8();" )
        cg.c( f"switch ( type ) {{" )
        for ch in self.children:
            cg.c( f"case num_{ ch.name }: return visitor( { ch.name }( br, true ) );" )
        cg.c( f"}}" )
        cg.c( f'ERROR( "binary data corruption ?" );' )
        cg.c( f"}}" )
        cg.c( f"" )


        # display
        cg.h( f"void display( Displayer &sr ) const;" )

        cg.c( f"void { self.name }::display( Displayer &sr ) const {{" )
        cg.c( f"struct DisplayVisitor : Visitor {{" )
        for ch in self.children:
            cg.c( f"void operator()( const { ch.name } &v ) {{ v.display( *sr ); }}" )
        cg.c( f"Displayer *sr;" )
        cg.c( f"}};" )
        cg.c( f"DisplayVisitor v;" )
        cg.c( f"v.sr = &sr;" )
        cg.c( f"visit( v );" )
        cg.c( f"}}" )
        cg.c( f"" )

        # write helper
        cg.h( f"void write( BioWriter &bw, bool last_in_sized ) const {{ bw.write_bytes( data, last_in_sized ); }}" )

        # attributes
        cg.h( f"std::span<PI8> data;" )

        # end of class decl
        cg.h( f"}};" )
        cg.h( f"" )

        # BioWriterFor
        cg.h( f"template<> struct BioWriterFor<{ self.name },{ self.writer or cg.writer }> {{" )
        for ch in self.children:
            arg_vals, args = ch.get_write_args( cg, self.ext_attrs )
            cg.h( f"void write_{ ch.name }( { args } );" )
        cg.h( f"{ self.writer or cg.writer } &bw;" )
        cg.h( f"bool last_in_sized;" )
        cg.h( f"}};" )
        cg.h( f"" )

        for ch in self.children:
            arg_vals, args = ch.get_write_args( cg, self.ext_attrs )
            beg_ext_attr = ""
            end_ext_attr = ""
            for attr in self.ext_attrs:
                # /*debug_info*/ [&]( auto &&dw ) { write_debug_info( wr.bw, dw ); },
                beg_ext_attr += f"bw.write_{ attr.name }( { attr.name }, [&]() {{ "
                end_ext_attr += f" }} );"

            cg.c( f"void BioWriterFor<{ self.name },{ self.writer or cg.writer }>::write_{ ch.name }( { args } ) {{ bw.write_sized( last_in_sized, [&]() {{ { beg_ext_attr }bw.write_PI8( { self.name }::num_{ ch.name } ); { ch.name }::write( bw, true, { arg_vals } ); }} );{ end_ext_attr } }}" )
        cg.c( f"" )

    def get_write_args( self, cg, ext_attrs = [] ):
        arg_vals = []
        args = []
        for attr in ext_attrs:
            # arg_vals.append( attr.name )
            if self.base_type( attr.type ):
                args.append(  f"{ attr.type } { attr.name }" )
            else:
                args.append(  f"const std::function<void( BioWriterFor<{ attr.type },BioWriter> &&writer )> &{ attr.name }" )
        for attr in self.attrs:
            arg_vals.append( attr.name )

            if self.base_type( attr.type ):
                args.append(  f"{ attr.type } { attr.name }" )
            # elif c := self.container_type( attr.type ):
            #     args.append(  f"const std::function<void( { c[ 1 ] }::Writer &&writer )> &{ attr.name }" )
            else:
                args.append(  f"const std::function<void( BioWriterFor<{ attr.type },{ self.writer or cg.writer }> &&writer )> &{ attr.name }" )
        arg_vals = ", ".join( arg_vals )
        args = ", ".join( args )

        return arg_vals, args

    def write_leaf_class_content( self, cg: Cg ):
        cg.h( f"class { self.name } {{" )
        cg.h( f"public:" )
        self.write_read_ctor( cg )
        self.write_void_ctor( cg )
        self.write_write_helper( cg )
        self.write_display( cg )
        self.write_attrs( cg )
        cg.h( f"}};" )
        cg.h( f"" )

        self.write_Writer( cg )

    def write_write_helper( self, cg: Cg ):
        arg_vals, args = self.get_write_args( cg )
        if len( self.attrs ):
            cg.h( f"static void write( { self.writer or cg.writer } &bw, bool last_in_sized, { args } );" )
            cg.h( f"" )

            cg.c( f"void { self.name }::write( { self.writer or cg.writer } &bw, bool last_in_sized, { args } ) {{" )
            cg.c( f"BioWriterFor<{ self.name },{ self.writer or cg.writer }> wr{{ bw, last_in_sized }};" )
            cg.c( f"wr.write( { arg_vals } );" )
            cg.c( f"}}" )
            cg.c( f"" )

        cg.h( f"void write( BioWriter &bw, bool last_in_sized ) const;" )
        cg.h( f"" )

        cg.c( f"void { self.name }::write( BioWriter &bw, bool last_in_sized ) const {{" )
        for attr in self.sorted_attrs:
            spfl = ""
            if attr.last:
                if self.write_sized:
                    spfl = "true"
                else:
                    spfl = "last_in_sized"
            else:
                spfl = "false"

            if self.base_type( attr.type ):
                cg.c( f"bw.write( { attr.name }, { spfl } );" )
            else:
                cg.c( f"{ attr.name }.write( bw, { spfl } );" )
        cg.c( f"}}" )
        cg.c( f"" )

    def write_Writer( self, cg: Cg ):
        # decl
        arg_vals, args = self.get_write_args( cg )
        cg.h( f"template<> struct BioWriterFor<{ self.name },{ self.writer or cg.writer }> {{" )
        cg.h( f"void write( { args } );" )
        cg.h( f"{ self.writer or cg.writer } &bw;" )
        cg.h( f"bool last_in_sized;" )
        cg.h( f"}};" )
        cg.h( f"" )

        # def
        cg.c( f"void BioWriterFor<{ self.name },{ self.writer or cg.writer }>::write( { args } ) {{" )
        if self.write_sized:
            cg.c( f"bw.write_sized( last_in_sized, [&]() {{" )

        for attr in self.sorted_attrs:
            spfl = ""
            if attr.last:
                if self.write_sized:
                    spfl = "true"
                else:
                    spfl = "last_in_sized"
            else:
                spfl = "false"

            if self.base_type( attr.type ):
                cg.c( f"bw.write( { attr.name }, { spfl } );" )
            # elif c := self.container_type( attr.type ):
            #     cg.c( f"bw.write_sized( { spfl }, [&]() {{ { attr.name } }} );" )
            else:
                cg.c( f"{ attr.name }( {{ bw, { spfl } }} );" )

        if self.write_sized:
            cg.c( f"}} );" )
        cg.c( f"}}" )

    def write_display( self, cg ):
        cg.h( f"void display( Displayer &sr ) const;" )

        cg.c( f"void { self.name }::display( Displayer &sr ) const {{" )

        args = [ self.name ]
        for attr in self.attrs:
            args.append(  attr.name )
        args = ", ".join( args )

        cg.c( f"SR_OBJECT( { args } );" )
        cg.c( f"}}\n" )
    
    def write_attrs( self, cg ):
        cg.h( f"" )
        for attr in self.attrs:
            cg.h( f"{ attr.type } { attr.name };" )
    
    def write_read_ctor( self, cg ):
        cg.h( f"{ self.name }( BioReader &reader, bool last_in_sized ) {{" )

        if self.write_sized:
            cg.h( f"reader.read_sized( last_in_sized, [&]( BioReader &reader ) {{" )

        for attr in self.sorted_attrs:
            spfl = ""
            if attr.last:
                if self.write_sized:
                    spfl = "true"
                else:
                    spfl = "last_in_sized"
            else:
                spfl = "false"
            cg.h( f"reader.read( { attr.name }, { spfl } );" )

        if self.write_sized:
            cg.h( f"}} );" )

        cg.h( f"}}\n" )
    
    def write_void_ctor( self, cg ):
        cg.h( f"{ self.name }() {{" )
        cg.h( f"}}\n" )

class SerializerCodegen:
    def __init__( self, pos_includes ) -> None:
        self.pos_includes = pos_includes
        self.includes = []
        self.classes = []
        self.writer = "BioWriter"

    def add_class( self, name, attrs, ext_attrs = [], parent = "", writer = "" ) -> None:
        self.classes.append( Class( name, attrs, ext_attrs, parent, writer ) )

    def needs_size( self, type ):
        if type == "PI": return False
        return True

    def make_code( self, directory ):
        self.make_sorted_attrs()
        self.make_children()

        os.makedirs( directory, exist_ok = True )
        for c in self.classes:
            with open( directory + "/" + c.name + ".h", "w" ) as h_file:
                with open( directory + "/" + c.name + ".cpp", "w" ) as c_file:
                    c.write( Cg( h_file, c_file, self.pos_includes, self.writer, self.includes ) )

    def make_sorted_attrs( self ):
        for c in self.classes:
            c.sorted_attrs = [ a for a in c.attrs ]
            c.sorted_attrs.sort( key = lambda a: ( self.needs_size( a.type ) ) )
            if len( c.sorted_attrs ):
                c.sorted_attrs[ -1 ].last = True

    def sort_classes( self ):
        for i in range( len( self.classes ) ):
            for j in range( i ):
                if self.classes[ j ].needs( self.classes[ i ].name ):
                    self.classes[ j ], self.classes[ i ] = self.classes[ i ], self.classes[ j ]

    def find_class( self, name ):
        for c in self.classes:
            if c.name == name:
                return c 
        raise f"class { name} not found"

    def make_children( self ):
        for c in self.classes:
            c.children = []

        for c in self.classes:
            if c.parent:
                p = self.find_class( c.parent )
                p.children.append( c )


