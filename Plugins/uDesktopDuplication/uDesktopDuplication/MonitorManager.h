#include <d3d11.h>
#include <dxgi1_2.h>
#include <vector>
#include <string>
#include <memory>

struct IUnityInterfaces;
class Monitor;
class Cursor;

class MonitorManager
{
public:
    explicit MonitorManager();
    ~MonitorManager();

    void SetCursorMonitorId(int id) { cursorMonitorId_ = id; }
    int GetCursorMonitorId() const { return cursorMonitorId_; }
    std::shared_ptr<Monitor> GetMonitor(int id) const;

private:
    void Initialize();
    void Finalize();

// Setters from Unity
public:
    void OnRender(int id);
    void UpdateCursorTexture(int id, ID3D11Texture2D* ptr);
    void SetTimeout(int timeout);
    void SetTexturePtr(int id, void* texture);

// Getters from Unity
public:
    int GetMonitorCount() const;
    void GetName(int id, char* buf, int len) const;
    bool IsPrimary(int id) const;
    int GetWidth(int id) const;
    int GetHeight(int id) const;
    int IsCursorVisible(int id) const;
    int GetCursorX(int id) const;
    int GetCursorY(int id) const;
    int GetCursorShapeWidth(int id) const;
    int GetCursorShapeHeight(int id) const;
    int GetCursorShapePitch(int id) const;
    int GetCursorShapeType(int id) const;

private:
    int timeout_ = 10;
    std::vector<std::shared_ptr<Monitor>> monitors_;
    std::shared_ptr<Cursor> cursor_;
    int cursorMonitorId_ = -1;
};