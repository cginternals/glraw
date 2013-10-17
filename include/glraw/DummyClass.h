
#include <glraw/glraw.h>

namespace glraw
{

class GLRAW_API DummyClass
{
    DummyClass();
    ~DummyClass();

    void foo() const;

private:
    bool m_bar;
};

} // namespace glraw
