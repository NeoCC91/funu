#pragma once

#ifdef FUNU_EXPORTS
#define FUNU_API __declspec(dllexport)
#else
#define FUNU_API __declspec(dllimport)
#endif