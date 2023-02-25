#include <memory>
#include <string>
#include "CanvasRenderingContextBase.hpp"
#include "Macros.hpp"

class Canvas {
public:
    Canvas(int width, int height);

    ~Canvas();

    std::shared_ptr<CanvasRenderingContextBase> getContext(std::string typeName);

private:
    int mWidth;
    int mHeight;
    std::shared_ptr<CanvasRenderingContextBase> mContextSkia;
};