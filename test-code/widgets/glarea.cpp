
#include "Inventor/Ww/widgets/SoWwGLArea.h"

extern bool ConvertWXAttrsWwGLFormat(const int *wxattrs,
                                     SoWwGLArea::GLFormat  &format);

int main() {

    {
        int gl_attr[] = {0};
        SoWwGLArea::GLFormat format;
        ConvertWXAttrsWwGLFormat(gl_attr, format);
    }
    {
        int gl_attr[] = {WX_GL_RGBA, 0};
        SoWwGLArea::GLFormat format;
        ConvertWXAttrsWwGLFormat(gl_attr, format);
    }

    {
        // set DEPTH BUFFER
        int gl_attr[] = {WX_GL_DEPTH_SIZE, 16, 0};
        SoWwGLArea::GLFormat format;
        ConvertWXAttrsWwGLFormat(gl_attr, format);
        (void)(format);
    }

}