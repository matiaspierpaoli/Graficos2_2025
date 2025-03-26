#ifndef EXPORTS_H
#define EXPORTS_H

#ifdef GraficosEngine_API
#define GraficosEngine_API __declspec(dllexport) //Especifica clase para exportacion
#else
#define GraficosEngine_API __declspec(dllimport) //Especifica clase para importacion
#endif

#endif