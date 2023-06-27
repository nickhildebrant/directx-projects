#pragma once
#include <Windows.h>
#include <vector>
#include <string>

#include "WindowsErrorMacros.h"
#include "RendererErrorMacros.h"

class DxgiInfoManager {
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long next = 0u;
	struct IDXGIInfoQueue* m_DxgiInfoQueue = nullptr;
};