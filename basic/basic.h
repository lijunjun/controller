#ifndef BASIC_H
#define BASIC_H

#ifdef BASIC_EXPORTS
#define BASIC_API __declspec(dllexport)
#else
#define BASIC_API __declspec(dllimport)
#endif

#endif