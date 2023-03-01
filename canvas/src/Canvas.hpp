#include <memory>
#include <string>
#include "CanvasRenderingContextBase.hpp"
#include "FontManager.hpp"
#include "Macros.hpp"

class Canvas {
public:
    Canvas(int width, int height);

    ~Canvas();

    std::shared_ptr<CanvasRenderingContextBase> getContext(std::string typeName);

    void loadFont(const void* data, size_t length, const FontInfo& info);

    void flush();

private:
    int mWidth;
    int mHeight;
    std::shared_ptr<CanvasRenderingContextBase> mContextSkia;
    std::shared_ptr<FontManager> mFontManager;
};