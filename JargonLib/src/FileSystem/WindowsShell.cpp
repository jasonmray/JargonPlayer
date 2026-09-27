#include "Jargon/FileSystem/WindowsShell.h"
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/ScopedPointer.h"
#include "Jargon/StringBuffer.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/System/ComUtilities.h"

#include "Jargon/System/WindowsDefines.h"
#include <windows.h>
#include <shellapi.h>
#include <ShObjIdl.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")


namespace Jargon{
namespace FileSystem{
namespace WindowShell {

	namespace {
		class FileProgressSink : public IFileOperationProgressSink {
			public:
			FileProgressSink(CompletedOperations& completedOperations) :
				referenceCount(1),
				completedOperations(completedOperations) {
			}

			////////////////////////////////////////////
			// IUnknown methods
			IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) {
				static const QITAB qit[] = {
					QITABENT(FileProgressSink, IFileOperationProgressSink),
					{ 0 },
				};
				return QISearch(this, qit, riid, ppv);
			}

			IFACEMETHODIMP_(ULONG) AddRef() {
				return InterlockedIncrement(&referenceCount);
			}

			IFACEMETHODIMP_(ULONG) Release() {
				LONG cRef = InterlockedDecrement(&referenceCount);
				if (cRef == 0) {
					delete this;
				}
				return cRef;
			}

			////////////////////////////////////////////
			// IFileOperationProgressSink methods
			IFACEMETHODIMP StartOperations() { return S_OK; }
			IFACEMETHODIMP PostQueryItem(DWORD, IShellItem*, ULONG, HRESULT hrResult) { return S_OK; }
			IFACEMETHODIMP PreCopyItem(DWORD, IShellItem* psiItem, IShellItem* psiDestinationFolder, LPCWSTR pszNewName) { return S_OK; }
			IFACEMETHODIMP PostCopyItem(DWORD, IShellItem* psiItem, IShellItem* psiDestinationFolder, LPCWSTR pszNewName, HRESULT hrCopyResult, IShellItem* psiNewlyCreatedItem) { return S_OK; }
			IFACEMETHODIMP PreMoveItem(DWORD, IShellItem*, IShellItem*, LPCWSTR) { return S_OK; }
			IFACEMETHODIMP PostMoveItem(DWORD, IShellItem*, IShellItem*, LPCWSTR, HRESULT, IShellItem*) { return S_OK; }
			IFACEMETHODIMP PreDeleteItem(DWORD, IShellItem*) { return S_OK; }
			IFACEMETHODIMP PostDeleteItem(DWORD, IShellItem* deletedItem, HRESULT deleteResult, IShellItem* resultingItem) {
				if (SUCCEEDED(deleteResult)) {
					SIGDN displayNameType = SIGDN_FILESYSPATH;
					LPWSTR name;
					if (SUCCEEDED(deletedItem->GetDisplayName(displayNameType, &name))) {
						completedOperations.deletedFiles.push_back(name);
						CoTaskMemFree(name);
					}
				}
				return S_OK;
			}
			IFACEMETHODIMP PreNewItem(DWORD dwFlags, IShellItem* psiDestinationFolder, LPCWSTR pszNewName) { return S_OK; }
			IFACEMETHODIMP PostNewItem(DWORD dwFlags, IShellItem* psiDestinationFolder, LPCWSTR pszNewName, LPCWSTR pszTemplateName, DWORD dwFileAttributes, HRESULT hrNew, IShellItem* psiNewItem) { return S_OK; }
			IFACEMETHODIMP PreRenameItem(DWORD, IShellItem*, LPCWSTR) { return S_OK; }
			IFACEMETHODIMP PostRenameItem(DWORD, IShellItem*, LPCWSTR, HRESULT, IShellItem*) { return S_OK; }
			IFACEMETHODIMP UpdateProgress(UINT uWorkDone, UINT uTotalWork) { return S_OK; }
			IFACEMETHODIMP ResetTimer() { return S_OK; }
			IFACEMETHODIMP PauseTimer() { return S_OK; }
			IFACEMETHODIMP ResumeTimer() { return S_OK; }
			IFACEMETHODIMP FinishOperations(HRESULT hrResult) { return S_OK; }

			private:
			LONG referenceCount;
			CompletedOperations& completedOperations;
		};


		bool moveFilesToRecycleBin(HWND parentWindow, const std::vector<const char*>& filenames, CompletedOperations* completedOperations, bool allowUserInteraction) {
			if (filenames.empty()) {
				return true;
			}

			Jargon::System::ComInitializer comInitializer(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

			if (!comInitializer) {
				// com initialization failed
				return false;
			}


			HRESULT hr;

			Jargon::System::ComPtr<IFileOperation> pFileOp;

			hr = CoCreateInstance(
				CLSID_FileOperation,
				nullptr,
				CLSCTX_ALL,
				IID_PPV_ARGS(pFileOp.addressOf())
			);

			if (FAILED(hr) || pFileOp.isNull()) {
				return false;
			}

			{
				// Set operation flags
				DWORD flags = FOF_ALLOWUNDO | FOF_WANTNUKEWARNING | FOF_FILESONLY;

				if (!allowUserInteraction) {
					flags |= FOF_NOCONFIRMATION | FOF_SILENT;
				}

				if (FAILED(pFileOp->SetOperationFlags(flags))) {
					return false;
				}
			}

			if (parentWindow != nullptr) {
				if (FAILED(pFileOp->SetOwnerWindow(parentWindow))) {
					return false;
				}
			}

			DWORD sinkCookie = 0;
			Jargon::ScopedPointer<FileProgressSink> progressSink;

			if (completedOperations != nullptr) {
				progressSink = new FileProgressSink(*completedOperations);
				pFileOp->Advise(progressSink.getRaw(), &sinkCookie);
			}

			for (size_t i = 0; i < filenames.size(); i++) {
				std::wstring filenameFull;
				if (!getFullPathName(filenames[i], filenameFull)) {
					return false;
				}

				filenameFull = Jargon::FileSystem::skipLongPathPrefix(filenameFull);

				Jargon::System::ComPtr<IShellItem> pShellItemToDelete;
				hr = SHCreateItemFromParsingName(filenameFull.c_str(), nullptr, IID_PPV_ARGS(pShellItemToDelete.addressOf()));

				if (FAILED(hr) || pShellItemToDelete.isNull()) {
					return false;
				}


				if (FAILED(pFileOp->DeleteItem(pShellItemToDelete.getRaw(), nullptr))) {
					return false;
				}
			}

			if (FAILED(pFileOp->PerformOperations())) {
				return false;
			}

			if (completedOperations != nullptr) {
				pFileOp->Unadvise(sinkCookie);
			}

			return true;
		}

		bool moveFilesToFolder(HWND parentWindow, const std::vector<const char*>& filenames, const std::wstring& destinationFolder, CompletedOperations* completedOperations, bool allowUserInteraction) {
			if (filenames.empty()) {
				return true;
			}

			Jargon::System::ComInitializer comInitializer(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

			if (!comInitializer) {
				// com initialization failed
				return false;
			}


			HRESULT hr;

			Jargon::System::ComPtr<IFileOperation> pFileOp;

			hr = CoCreateInstance(
				CLSID_FileOperation,
				nullptr,
				CLSCTX_ALL,
				IID_PPV_ARGS(pFileOp.addressOf())
			);

			if (FAILED(hr) || pFileOp.isNull()) {
				return false;
			}

			{
				// Set operation flags
				DWORD flags = FOF_ALLOWUNDO | FOF_WANTNUKEWARNING | FOF_FILESONLY;

				if (!allowUserInteraction) {
					flags |= FOF_NOCONFIRMATION | FOF_SILENT;
				}

				if (FAILED(pFileOp->SetOperationFlags(flags))) {
					return false;
				}
			}

			if (parentWindow != nullptr) {
				if (FAILED(pFileOp->SetOwnerWindow(parentWindow))) {
					return false;
				}
			}

			DWORD sinkCookie = 0;
			Jargon::ScopedPointer<FileProgressSink> progressSink;

			if (completedOperations != nullptr) {
				progressSink = new FileProgressSink(*completedOperations);
				pFileOp->Advise(progressSink.getRaw(), &sinkCookie);
			}

			Jargon::System::ComPtr<IShellItem> pShellItemDestination;
			{
				std::wstring desinationFolderFull;
				if (!getFullPathName(destinationFolder.c_str(), desinationFolderFull)) {
					return false;
				}

				desinationFolderFull = Jargon::FileSystem::skipLongPathPrefix(desinationFolderFull);

				hr = SHCreateItemFromParsingName(destinationFolder.c_str(), nullptr, IID_PPV_ARGS(pShellItemDestination.addressOf()));


				if (FAILED(hr) || pShellItemDestination.isNull()) {
					return false;
				}
			}

			for (size_t i = 0; i < filenames.size(); i++) {
				std::wstring filenameFull;
				if (!getFullPathName(filenames[i], filenameFull)) {
					return false;
				}

				filenameFull = Jargon::FileSystem::skipLongPathPrefix(filenameFull);

				Jargon::System::ComPtr<IShellItem> pShellItemToMove;
				hr = SHCreateItemFromParsingName(filenameFull.c_str(), nullptr, IID_PPV_ARGS(pShellItemToMove.addressOf()));

				if (FAILED(hr) || pShellItemToMove.isNull()) {
					return false;
				}


				if (FAILED(pFileOp->MoveItem(pShellItemToMove.getRaw(), pShellItemDestination.getRaw(), nullptr, nullptr))) {
					return false;
				}
			}

			if (FAILED(pFileOp->PerformOperations())) {
				return false;
			}

			if (completedOperations != nullptr) {
				pFileOp->Unadvise(sinkCookie);
			}

			return true;
		}
	}

	bool moveFilesToRecycleBin(const std::vector<const char*>& filenames, bool allowUserInteraction) {
		return moveFilesToRecycleBin(nullptr, filenames, nullptr, allowUserInteraction);
	}

	bool moveFilesToRecycleBin(const std::vector<const char*>& filenames, CompletedOperations& completedOperations, bool allowUserInteraction) {
		return moveFilesToRecycleBin(nullptr, filenames, &completedOperations, allowUserInteraction);
	}

	bool moveFilesToRecycleBin(HWND parentWindow, const std::vector<const char*>& filenames, bool allowUserInteraction) {
		return moveFilesToRecycleBin(parentWindow, filenames, nullptr, allowUserInteraction);
	}

	bool moveFilesToRecycleBin(HWND parentWindow, const std::vector<const char*>& filenames, CompletedOperations& completedOperations, bool allowUserInteraction) {
		return moveFilesToRecycleBin(parentWindow, filenames, &completedOperations, allowUserInteraction);
	}

	bool moveFilesToFolder(const std::vector<const char*>& filenames, const std::wstring& destinationFolder, bool allowUserInteraction) {
		return moveFilesToFolder(nullptr, filenames, destinationFolder, nullptr, allowUserInteraction);
	}

	bool moveFilesToFolder(const std::vector<const char*>& filenames, const std::wstring& destinationFolder, CompletedOperations& completedOperations, bool allowUserInteraction) {
		return moveFilesToFolder(nullptr, filenames, destinationFolder, &completedOperations, allowUserInteraction);
	}

	bool moveFilesToFolder(HWND parentWindow, const std::vector<const char*>& filenames, const std::wstring& destinationFolder, bool allowUserInteraction) {
		return moveFilesToFolder(parentWindow, filenames, destinationFolder, nullptr, allowUserInteraction);
	}

	bool moveFilesToFolder(HWND parentWindow, const std::vector<const char*>& filenames, const std::wstring& destinationFolder, CompletedOperations& completedOperations, bool allowUserInteraction) {
		return moveFilesToFolder(parentWindow, filenames, destinationFolder, &completedOperations, allowUserInteraction);
	}


}
}
}
