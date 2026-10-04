#pragma once

#ifdef PILEDLL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

extern "C" {
    API bool validerExpressionTableau(const char* expr);
    API bool validerExpressionPointeur(const char* expr);
}
