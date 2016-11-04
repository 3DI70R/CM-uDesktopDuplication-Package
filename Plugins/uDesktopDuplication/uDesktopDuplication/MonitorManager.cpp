#include <d3d11.h>
#include <dxgi1_2.h>
#include <vector>
#include <string>

#include "IUnityInterface.h"
#include "IUnityGraphicsD3D11.h"

#include "Monitor.h"
#include "Cursor.h"
#include "MonitorManager.h"


MonitorManager::MonitorManager()
{
    Initialize();
}


MonitorManager::~MonitorManager()
{
    Finalize();
}


void MonitorManager::Initialize()
{
    Finalize();

    // Get factory
    IDXGIFactory1* factory;
    CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory));

    // Check all display adapters
    int id = 0;
    IDXGIAdapter1* adapter;
    for (int i = 0; (factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND); ++i) 
    {
        // Search the main monitor from all outputs
        IDXGIOutput* output;
        for (int j = 0; (adapter->EnumOutputs(j, &output) != DXGI_ERROR_NOT_FOUND); ++j) 
        {
            auto monitor = std::make_shared<Monitor>(id++, output);
            monitors_.push_back(monitor);
            output->Release();
        }
        adapter->Release();
    }
    factory->Release();
}


void MonitorManager::Finalize()
{
    monitors_.clear();
}


std::shared_ptr<Monitor> MonitorManager::GetMonitor(int id) const
{
    if (id >= 0 && id < monitors_.size())
    {
        return monitors_[id];
    }
    return nullptr;
}


void MonitorManager::OnRender(int id)
{
    if (auto monitor = GetMonitor(id))
    {
        // If any monitor setting has changed (e.g. monitor size has changed),
        // it is necessary to re-initialize monitors.
        const auto hr = monitor->Render(timeout_);
        if (hr == DXGI_ERROR_ACCESS_LOST)
        {
            Initialize();
        }
    }
}


void MonitorManager::UpdateCursorTexture(int id, ID3D11Texture2D* texture)
{
    if (auto monitor = GetMonitor(id))
    {
        monitor->UpdateCursorTexture(texture);
    }
}


void MonitorManager::SetTimeout(int timeout)
{
    timeout_ = timeout;
}


void MonitorManager::SetTexturePtr(int id, void* texture)
{
    if (auto monitor = GetMonitor(id))
    {
        auto d3d11Texture = reinterpret_cast<ID3D11Texture2D*>(texture);
        monitor->SetUnityTexture(d3d11Texture);
    }
}


int MonitorManager::GetMonitorCount() const
{
    return static_cast<int>(monitors_.size());
}


void MonitorManager::GetName(int id, char* buf, int len) const
{
    if (auto monitor = GetMonitor(id))
    {
        monitor->GetName(buf, len);
    }
}


bool MonitorManager::IsPrimary(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->IsPrimary();
    }
    return false;
}


int MonitorManager::GetWidth(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetWidth();
    }
    return -1;
}


int MonitorManager::GetHeight(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetHeight();
    }
    return -1;
}


int MonitorManager::IsCursorVisible(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->IsVisible();
    }
    return false;
}


int MonitorManager::GetCursorX(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->GetX();
    }
    return -1;
}


int MonitorManager::GetCursorY(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->GetY();
    }
    return -1;
}


int MonitorManager::GetCursorShapeWidth(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->GetWidth();
    }
    return -1;
}


int MonitorManager::GetCursorShapeHeight(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->GetHeight();
    }
    return -1;
}


int MonitorManager::GetCursorShapePitch(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->GetPitch();
    }
    return -1;
}


int MonitorManager::GetCursorShapeType(int id) const
{
    if (auto monitor = GetMonitor(id))
    {
        return monitor->GetCursor()->GetType();
    }
    return -1;
}