#ifndef CONFIG_H
#define CONFIG_H

#ifdef MEMORYMGT_EXPORTS
#define MEMORYMGT_API __declspec(dllexport)
#else
#define MEMORYMGT_API __declspec(dllimport)
#endif

#endif