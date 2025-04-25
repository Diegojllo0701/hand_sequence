#pragma once

#if defined _WIN32 || defined __CYGWIN__
#  define HandSequenceController_DLLIMPORT __declspec(dllimport)
#  define HandSequenceController_DLLEXPORT __declspec(dllexport)
#  define HandSequenceController_DLLLOCAL
#else
// On Linux, for GCC >= 4, tag symbols using GCC extension.
#  if __GNUC__ >= 4
#    define HandSequenceController_DLLIMPORT __attribute__((visibility("default")))
#    define HandSequenceController_DLLEXPORT __attribute__((visibility("default")))
#    define HandSequenceController_DLLLOCAL __attribute__((visibility("hidden")))
#  else
// Otherwise (GCC < 4 or another compiler is used), export everything.
#    define HandSequenceController_DLLIMPORT
#    define HandSequenceController_DLLEXPORT
#    define HandSequenceController_DLLLOCAL
#  endif // __GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

#ifdef HandSequenceController_STATIC
// If one is using the library statically, get rid of
// extra information.
#  define HandSequenceController_DLLAPI
#  define HandSequenceController_LOCAL
#else
// Depending on whether one is building or using the
// library define DLLAPI to import or export.
#  ifdef HandSequenceController_EXPORTS
#    define HandSequenceController_DLLAPI HandSequenceController_DLLEXPORT
#  else
#    define HandSequenceController_DLLAPI HandSequenceController_DLLIMPORT
#  endif // HandSequenceController_EXPORTS
#  define HandSequenceController_LOCAL HandSequenceController_DLLLOCAL
#endif // HandSequenceController_STATIC