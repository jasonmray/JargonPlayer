
#include "WebcamEnumerator.h"

#include "Jargon/StringUtilities.h"

#include <windows.h>
#include <dshow.h>

#pragma comment (lib, "ole32.lib")
#pragma comment (lib, "oleaut32.lib")
#pragma comment(lib, "strmiids")

#pragma warning(suppress : 6387)


static HRESULT EnumerateDevices(REFGUID category, IEnumMoniker** ppEnum)
{
	// Create the System Device Enumerator.
	ICreateDevEnum* pDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the category.
		hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
		}
		pDevEnum->Release();
	}
	return hr;
}

static void EnumerateWebcamNames(IEnumMoniker* pEnum, std::vector<std::string>& webcamNamesOut) {
	IMoniker* pMoniker = NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		IPropertyBag* pPropBag;
		HRESULT hr = pMoniker->BindToStorage(NULL, NULL, IID_PPV_ARGS(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;
		}

		VARIANT var;
		VariantInit(&var);

		hr = pPropBag->Read(L"FriendlyName", &var, 0);
		if (SUCCEEDED(hr))
		{
			printf("%S\n", var.bstrVal);

			std::string name = Jargon::StringUtilities::wideToUtf8(var.bstrVal);
			webcamNamesOut.push_back(name);

			VariantClear(&var);
		}


		//hr = pPropBag->Read(L"DevicePath", &var, 0);
		//if (SUCCEEDED(hr))
		//{
		//    // The device path is not intended for display.
		//    printf("Device path: %S\n", var.bstrVal);
		//    VariantClear(&var);
		//}

		pPropBag->Release();
		pMoniker->Release();
	}
}

WebcamEnumerator::WebcamEnumerator() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		initialized = true;
	}
}

WebcamEnumerator::~WebcamEnumerator() {
	if (initialized) {
		CoUninitialize();
	}
}

bool WebcamEnumerator::enumerateWebcamNames(std::vector<std::string>& webcamNamesOut) {
	IEnumMoniker* pEnum;

	HRESULT hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
	if (SUCCEEDED(hr))
	{
		EnumerateWebcamNames(pEnum, webcamNamesOut);
		pEnum->Release();

		return true;
	}

	return false;
}

std::string WebcamEnumerator::buildWebcamFileUrl(const char* webcamName) {
	return Jargon::StringUtilities::format("av://dshow:video=%s", webcamName);
}

bool WebcamEnumerator::enumerateWebcamUrls(std::vector<std::string>& webcamUrlsOut) {
	if (enumerateWebcamNames(webcamUrlsOut)) {
		for (size_t i = 0; i < webcamUrlsOut.size(); i++) {
			webcamUrlsOut[i] = buildWebcamFileUrl(webcamUrlsOut[i].c_str());
		}
		return true;
	}

	return false;
}