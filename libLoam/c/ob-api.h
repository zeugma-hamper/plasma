
/* (c)  oblong industries */

#ifndef _OB_API_H
#define _OB_API_H

#if defined(_MSC_VER) && !defined(OB_WINDOWS_STATIC)

//definitions for windows import/export library stuff

//if you are compiling a library, you want to export the
//symbols for your library, but (probably) import symbols from
//one or more of the other yovo libraries.
//
//for example to compile libPlasma, you want to add these to your
//pre-processor #defines -
//  OB_LOAM_IMPORTS
//  OB_PLASMA_EXPORTS

//libLoam
#ifdef OB_LOAM_EXPORTS
#define OB_LOAM_API __declspec(dllexport)
#else
#define OB_LOAM_API __declspec(dllimport)
#endif
//libLoam

//libLoam++
#ifdef OB_LOAMXX_EXPORTS
#define OB_LOAMXX_API __declspec(dllexport)
#else
#define OB_LOAMXX_API __declspec(dllimport)
#endif
//libLoam++

//libPlasma
#ifdef OB_PLASMA_EXPORTS
#define OB_PLASMA_API __declspec(dllexport)
#else
#define OB_PLASMA_API __declspec(dllimport)
#endif
//libPlasma

//libPlasma++
#ifdef OB_PLASMAXX_EXPORTS
#define OB_PLASMAXX_API __declspec(dllexport)
#else
#define OB_PLASMAXX_API __declspec(dllimport)
#endif
//libPlasma++

//rubyPlasma
#ifdef OB_RUBYPLASMA_EXPORTS
#define OB_RUBYPLASMA_API __declspec(dllexport)
#else
#define OB_RUBYPLASMA_API __declspec(dllimport)
#endif
//rubyPlasma

//libBasement
#ifdef OB_BASEMENT_EXPORTS
#define OB_BASEMENT_API __declspec(dllexport)
#else
#define OB_BASEMENT_API __declspec(dllimport)
#endif
//libBasement

//libEventSlurper
#ifdef OB_EVENTSLURPER_EXPORTS
#define OB_EVENTSLURPER_API __declspec(dllexport)
#else
#define OB_EVENTSLURPER_API __declspec(dllimport)
#endif
//libEventSlurper

//libProtist
#ifdef OB_PROTIST_EXPORTS
#define OB_PROTIST_API __declspec(dllexport)
#else
#define OB_PROTIST_API __declspec(dllimport)
#endif
//libProtist

//libNoodoo
#ifdef OB_NOODOO_EXPORTS
#define OB_NOODOO_API __declspec(dllexport)
#else
#define OB_NOODOO_API __declspec(dllimport)
#endif
//libNoodoo

//libMedia
#ifdef OB_MEDIA_EXPORTS
#define OB_MEDIA_API __declspec(dllexport)
#else
#define OB_MEDIA_API __declspec(dllimport)
#endif
//libMedia

//libImpetus
#ifdef OB_IMPETUS_EXPORTS
#define OB_IMPETUS_API __declspec(dllexport)
#else
#define OB_IMPETUS_API __declspec(dllimport)
#endif
//libImpetus

//libAfferent
#ifdef OB_AFFERENT_EXPORTS
#define OB_AFFERENT_API __declspec(dllexport)
#else
#define OB_AFFERENT_API __declspec(dllimport)
#endif
//libAfferent

//libTwillig
#ifdef OB_TWILLIG_EXPORTS
#define OB_TWILLIG_API __declspec(dllexport)
#else
#define OB_TWILLIG_API __declspec(dllimport)
#endif
//libTwillig

//libGanglia
#ifdef OB_GANGLIA_EXPORTS
#define OB_GANGLIA_API __declspec(dllexport)
#else
#define OB_GANGLIA_API __declspec(dllimport)
#endif
//libGanglia

//libOuija
#ifdef OB_OUIJA_EXPORTS
#define OB_OUIJA_API __declspec(dllexport)
#else
#define OB_OUIJA_API __declspec(dllimport)
#endif
//libOuija

//libViddle
#ifdef OB_VIDDLE_EXPORTS
#define OB_VIDDLE_API __declspec(dllexport)
#else
#define OB_VIDDLE_API __declspec(dllimport)
#endif
//libViddle

//libQuartermaster
#ifdef OB_QUARTERMASTER_EXPORTS
#define OB_QUARTERMASTER_API __declspec(dllexport)
#else
#define OB_QUARTERMASTER_API __declspec(dllimport)
#endif
//libQuartermaster

//libVentriloquy
#ifdef OB_VENTRILOQUY_EXPORTS
#define OB_VENTRILOQUY_API __declspec(dllexport)
#else
#define OB_VENTRILOQUY_API __declspec(dllimport)
#endif
//libVentriloquy

//libBoom
#ifdef OB_BOOM_EXPORTS
#define OB_BOOM_API __declspec(dllexport)
#else
#define OB_BOOM_API __declspec(dllimport)
#endif
//libBoom

//libPix
#ifdef OB_PIX_EXPORTS
#define OB_PIX_API __declspec(dllexport)
#else
#define OB_PIX_API __declspec(dllimport)
#endif
//libPix

//libResource
#ifdef OB_RESOURCE_EXPORTS
#define OB_RESOURCE_API __declspec(dllexport)
#else
#define OB_RESOURCE_API __declspec(dllimport)
#endif
//libResource

//libSplotch
#ifdef OB_SPLOTCH_EXPORTS
#define OB_SPLOTCH_API __declspec(dllexport)
#else
#define OB_SPLOTCH_API __declspec(dllimport)
#endif
//libSplotch

//libNoodoo2
#ifdef OB_NOODOO2_EXPORTS
#define OB_NOODOO2_API __declspec(dllexport)
#else
#define OB_NOODOO2_API __declspec(dllimport)
#endif
//libNoodoo2

//libTwillig2
#ifdef OB_TWILLIG2_EXPORTS
#define OB_TWILLIG2_API __declspec(dllexport)
#else
#define OB_TWILLIG2_API __declspec(dllimport)
#endif
//libTwillig2

// by default, symbols in MSC aren't visable so just define
#define OB_SPLOTCH_LOCAL
#define OB_NOODOO2_LOCAL

// right so we need to make certain template classes and functions
// visible on Linux and macOS for boom's type id to function, but this
// causes problems with the windows linker.
#define OB_LOAMXX_TEMPLATE
#define OB_MEDIA_TEMPLATE
#define OB_BASEMENT_TEMPLATE

#define OB_BOOM_TEMPLATE
#define OB_RESOURCE_TEMPLATE
#define OB_PIX_TEMPLATE
#define OB_SPLOTCH_TEMPLATE
#define OB_NOODOO2_TEMPLATE
#define OB_TWILLIG2_TEMPLATE


#else  // if defined (_MSC_VER) && ! defined (OB_WINDOWS_STATIC)

// Definitions for Windows static linking, and for object formats
// like ELF and Mach-O which aren't as cantankerous as Windows.

#if __GNUC__ >= 4

#define OB_LOAM_API __attribute__ ((visibility ("default")))
#define OB_LOAMXX_API __attribute__ ((visibility ("default")))
#define OB_PLASMA_API __attribute__ ((visibility ("default")))
#define OB_PLASMAXX_API __attribute__ ((visibility ("default")))
#define OB_RUBYPLASMA_API __attribute__ ((visibility ("default")))
#define OB_BASEMENT_API __attribute__ ((visibility ("default")))
#define OB_EVENTSLURPER_API __attribute__ ((visibility ("default")))
#define OB_PROTIST_API __attribute__ ((visibility ("default")))
#define OB_NOODOO_API __attribute__ ((visibility ("default")))
#define OB_MEDIA_API __attribute__ ((visibility ("default")))
#define OB_IMPETUS_API __attribute__ ((visibility ("default")))
#define OB_AFFERENT_API __attribute__ ((visibility ("default")))
#define OB_TWILLIG_API __attribute__ ((visibility ("default")))
#define OB_GANGLIA_API __attribute__ ((visibility ("default")))
#define OB_OUIJA_API __attribute__ ((visibility ("default")))
#define OB_VIDDLE_API __attribute__ ((visibility ("default")))
#define OB_QUARTERMASTER_API __attribute__ ((visibility ("default")))
#define OB_VENTRILOQUY_API __attribute__ ((visibility ("default")))
#define OB_BOOM_API __attribute__ ((visibility ("default")))
#define OB_RESOURCE_API __attribute__ ((visibility ("default")))
#define OB_PIX_API __attribute__ ((visibility ("default")))
#define OB_SPLOTCH_API __attribute__ ((visibility ("default")))
#define OB_SPLOTCH_LOCAL __attribute__ ((visibility ("hidden")))
#define OB_NOODOO2_API __attribute__ ((visibility ("default")))
#define OB_NOODOO2_LOCAL __attribute__ ((visibility ("hidden")))
#define OB_TWILLIG2_API __attribute__ ((visibility ("default")))

#define OB_LOAMXX_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_MEDIA_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_BASEMENT_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_BOOM_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_RESOURCE_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_PIX_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_SPLOTCH_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_NOODOO2_TEMPLATE __attribute__ ((visibility ("default")))
#define OB_TWILLIG2_TEMPLATE __attribute__ ((visibility ("default")))

#else

#define OB_LOAM_API
#define OB_LOAMXX_API
#define OB_PLASMA_API
#define OB_PLASMAXX_API
#define OB_RUBYPLASMA_API
#define OB_BASEMENT_API
#define OB_EVENTSLURPER_API
#define OB_PROTIST_API
#define OB_NOODOO_API
#define OB_MEDIA_API
#define OB_IMPETUS_API
#define OB_AFFERENT_API
#define OB_TWILLIG_API
#define OB_GANGLIA_API
#define OB_OUIJA_API
#define OB_VIDDLE_API
#define OB_QUARTERMASTER_API
#define OB_VENTRILOQUY_API
#define OB_BOOM_API
#define OB_RESOURCE_API
#define OB_PIX_API
#define OB_SPLOTCH_API
#define OB_SPLOTCH_LOCAL
#define OB_NOODOO2_API
#define OB_NOODOO2_LOCAL
#define OB_TWILLIG2_API

#define OB_LOAMXX_TEMPLATE
#define OB_MEDIA_TEMPLATE
#define OB_BASEMENT_TEMPLATE
#define OB_BOOM_TEMPLATE
#define OB_RESOURCE_TEMPLATE
#define OB_PIX_TEMPLATE
#define OB_SPLOTCH_TEMPLATE
#define OB_NOODOO2_TEMPLATE
#define OB_TWILLIG2_TEMPLATE

#endif

#endif  //#ifdef _MSC_VER

#endif  //#ifdef _OB_API_H
