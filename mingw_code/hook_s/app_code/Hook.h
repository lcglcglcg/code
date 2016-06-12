#ifdef  DLL_API
#else
#define DLL_API _declspec(dllimport)
#endif

DLL_API  LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
DLL_API  LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
DLL_API  void SetHook(HWND hwnd, FILE *pfile);
DLL_API  void UnSetHook();
