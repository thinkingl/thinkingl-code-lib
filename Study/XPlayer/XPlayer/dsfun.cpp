#include "stdafx.h"
#include "dsfun.h"

HRESULT AddToRot(IUnknown* pUnknownGraph, DWORD* pdwRegister)
{
	IMoniker *pMoniker = NULL;
	IRunningObjectTable *pROT = NULL;

	if (FAILED(GetRunningObjectTable(0, &pROT)))
	{
		return E_FAIL;
	}

	const size_t STRING_LENGTH = 256;
	WCHAR wsz[STRING_LENGTH] = { 0 };

	StringCchPrintfW(
		wsz, STRING_LENGTH,
		L"FilterGraph %08x pid %08x",
		(DWORD_PTR)pUnknownGraph,
		GetCurrentProcessId()
		);

	HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
	if (SUCCEEDED(hr))
	{
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnknownGraph, pMoniker, pdwRegister);
		pMoniker->Release();
	}
	pROT->Release();
	return hr;
}

void RemoveFromRot(DWORD pdwRegister)
{
	IRunningObjectTable *pROT;
	if (SUCCEEDED(GetRunningObjectTable(0, &pROT)))
	{
		pROT->Revoke(pdwRegister);
		pROT->Release();
	}
}