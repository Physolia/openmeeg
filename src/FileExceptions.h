#ifndef FILEEXCEPTIONS_H
#define FILEEXCEPTIONS_H

#include <iostream>
#include <stdexcept>  //  Needs class 'std::runtime_error'

//  Exception classes defined in this file (indentation shows derivation hierarchy) :

//  in namespace std::io_except :
//  io_error
//    read_error
//    write_error
//    in namespace std::io_except::file_except :
//    file_error
//      file_open_error
//        input_file_open_error
//        output_file_open_error

//  Define macro LANGUAGE with value 0 for english messages, or 1 for french messages.
//  Default is english. LANGUAGE is undefined at end of this file if it wasn't defined
//  before.

#ifndef LANGUAGE
#define LANGUAGE 0
#else
#define LANGUAGE_WAS_DEF
#endif

#if LANGUAGE < 0 || LANGUAGE > 1
#error : Invalid value for macro LANGUAGE
#endif

#if LANGUAGE == 0
#define UNEXP_EOF        string("Unexpected end of file")
#define FORMAT_ERR       string("Format error")
#define WRITE_ERR        string("Write error")
#define OPEN_ERR         string("Error opening file")
#define I_OPEN_ERR       string("Error opening input file")
#define O_OPEN_ERR       string("Error opening output file")
#define I_OPEN_ERR_CAUSE string("File read protected or doesn't exist")
#define O_OPEN_ERR_CAUSE string("File write protected")
#endif

#if LANGUAGE == 1
#define UNEXP_EOF        string("Fin de fichier inattendue")
#define FORMAT_ERR       string("Erreur de format")
#define WRITE_ERR        string("Erreur d'�criture")
#define OPEN_ERR         string("Erreur d'ouverture du fichier")
#define I_OPEN_ERR       string("Erreur d'ouverture en lecture du fichier")
#define O_OPEN_ERR       string("Erreur d'ouverture en �criture du fichier")
#define I_OPEN_ERR_CAUSE string("Fichier prot�g� en lecture ou inexistant")
#define O_OPEN_ERR_CAUSE string("Fichier prot�g� en �criture")
#endif

namespace std {
    namespace io_except {

        //  General input/output error. Requires a string describing the error.

        class io_error: public std::runtime_error {
        public:
            io_error(const std::string& what_arg):std::runtime_error(what_arg) { }
            ~io_error() throw() { }
        };

        //  Read error. Requires an istream object.

        class read_error: public io_error {
        public:
            read_error(const std::istream& is):io_error(is.eof() ? UNEXP_EOF : FORMAT_ERR) { }
        };

        //  Write error.

        class write_error: public io_error {
        public:
            write_error():io_error(WRITE_ERR) { }
        };

        namespace file_except {

            //  General input/output error. Requires a string describing the error.

            class file_error: public io_error {
            public:
                file_error(const std::string& what_arg):io_error(what_arg) { }
            };

            //  Error in opening a file.
            //  Optionnal arguments: the file name and the file open mode (in/out).

            class file_open_error: public file_error {
            public:

                file_open_error():file_error(std::string(OPEN_ERR)) { }
                file_open_error(const std::string& file_name):file_error(std::string(OPEN_ERR + " " + file_name)) { }

                file_open_error(const std::ios::openmode mode):
                    file_error(std::string((mode==ios::in) ? I_OPEN_ERR+"\n"+I_OPEN_ERR_CAUSE : O_OPEN_ERR+"\n"+O_OPEN_ERR_CAUSE)) { }

                file_open_error(const std::string& file_name,const std::ios::openmode mode):
                    file_error(std::string((mode==ios::in) ? I_OPEN_ERR+" "+file_name+"\n"+I_OPEN_ERR_CAUSE :
                                                        O_OPEN_ERR+" "+file_name+"\n"+O_OPEN_ERR_CAUSE)) { }
            };

            //  Error in opening an input file. Optionnal argument : the file name.

            class input_file_open_error: public file_open_error {
            public:

                input_file_open_error():file_open_error(ios::in) { }
                input_file_open_error(const std::string& file_name):file_open_error(file_name,ios::in) { }
            };

            //  Error in opening an output file. Optionnal argument: the file name.

            class output_file_open_error: public file_open_error {
            public:

                output_file_open_error():file_open_error(ios::out) { }
                output_file_open_error(const std::string& file_name):file_open_error(file_name,ios::out) { }
            };
        }
    }
}

//  Undefine local macros.
#undef UNEXP_EOF
#undef FORMAT_ERR
#undef WRITE_ERR
#undef OPEN_ERR
#undef I_OPEN_ERR
#undef O_OPEN_ERR
#undef I_OPEN_ERR_CAUSE
#undef O_OPEN_ERR_CAUSE

#ifndef LANGUAGE_WAS_DEF
#undef LANGUAGE
#else
#undef LANGUAGE_WAS_DEF
#endif

#endif  //  !FILEEXCEPTIONS_H
