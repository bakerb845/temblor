#include <cstdio>
#include <cstdlib>
#include <string>
#include <array>
#include <cmath>
//#include <giomm/resource.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <epoxy/gl.h> //GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <GL/glew.h>
#if __has_include(<pstl/algorithm>)
#include <pstl/execution>
#include <pstl/algorithm>
#define USE_PSTL
#else
#include <algorithm>
#endif
#include "temblor/private/filesystem.hpp"
#include "glWiggle.hpp"
#include "glslShader.hpp"


namespace
{

void checkGlError(const char* op);
void checkGlError(const char* op)
{
    for (GLint error = glGetError(); error; error=glGetError())
    {
        fprintf(stderr, "After %s() glError (0x%d)%d\n",
                op, error, GL_INVALID_OPERATION);
    }
}

struct point
{
    GLfloat x = 0;
    GLfloat y = 0;
};

struct Character
{
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class FreeType
{
public:
    /// Constructor - loads the library
    FreeType()
    {
        if (FT_Init_FreeType(&mFreeType))
        {
            fprintf(stderr, "Could initialize FreeType library\n");
            return;
        }
        mLinit = true;
    } 
    /// Destructor releases the library
    ~FreeType()
    {
        if (mLinit)
        {
            FT_Done_FreeType(mFreeType);
            mLinit = false; 
        }
        mCharacters.clear();
        mFontsFile.clear();
    }
    /// Sets the pixel width and height
    /// If width is 0 then it will be automatically set from height
    void setPixelWidthAndHeight(const int width, const int height)
    {
        if (!mLinit)
        {
            fprintf(stderr, "FreeType not initialized\n");
            return;
        }
        if (!mHaveFace)
        {
            fprintf(stderr, "Facetype not yet set - call setFontFile");
            return;
        }
        FT_Set_Pixel_Sizes(mFace, width, height);
    } 
    /// Sets the font file
    void setFontFile(const std::string &fontsFile)
    {
        if (!mLinit)
        {
            fprintf(stderr, "FreeType not initialized\n");
            return;
        }
#ifdef TEMBLOR_USE_FILESYSTEM
        if (!fs::exists(fontsFile))
        {
            fprintf(stderr, "File %s does not exist\n", fontsFile.c_str());
        }
#endif 
        mHaveFace = false;
        mCharacters.clear();
        mFontsFile = fontsFile;
        if (FT_New_Face(mFreeType, mFontsFile.c_str(), 0, &mFace))
        {
            fprintf(stderr, "Failed to load font from %s\n", mFontsFile.c_str());
            return;
        }
        mHaveFace = true;
    }
    /// Load the 128 characters
    void loadCharacters()
    {
        if (!mLinit)
        {
            fprintf(stderr, "FreeType not initialized\n");
            return;
        }
        if (!mHaveFace)
        {
            if (FT_New_Face(mFreeType, mFontsFile.c_str(), 0, &mFace))
            {
                fprintf(stderr, "Failed to load font from %s\n", mFontsFile.c_str());
                return;
            }
            mHaveFace = true;
        }
        mCharacters.clear();
        // Disable the byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (GLubyte c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(mFace, c, FT_LOAD_RENDER))
            {
                fprintf(stderr, "Failed to load Glyph\n");
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RED,
                         mFace->glyph->bitmap.width,
                         mFace->glyph->bitmap.rows,
                         0,
                         GL_RED,
                         GL_UNSIGNED_BYTE,
                         mFace->glyph->bitmap.buffer);
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {texture,
                                   glm::ivec2(mFace->glyph->bitmap.width, mFace->glyph->bitmap.rows),
                                   glm::ivec2(mFace->glyph->bitmap_left, mFace->glyph->bitmap_top),
                                   static_cast<GLuint> (mFace->glyph->advance.x)
                                  };
            mCharacters.insert(std::pair<GLchar, Character>(c, character));
        }
    }
    std::map<GLchar, Character> getCharacters() const
    {
        if (mCharacters.size() == 0)
        {
            fprintf(stderr, "Characters not yet initialized\n");
        }
        return mCharacters;
    }

    FT_Library mFreeType;
    FT_Face mFace;
    std::map<GLchar, Character> mCharacters;
    std::string mFontsFile = "fonts/arial.ttf";
    bool mLinit = false;
    bool mHaveFace = false;
};

void packCharacters(std::map<GLchar, Character> &characters,
                    const int pixelWidth = 0,
                    const int pixelHeight = 48,
                    const std::string &fontsFile = "fonts/arial.ttf")
{
    // Initialize FreeType and load the fonts
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        fprintf(stderr, "Could initialize FreeType library\n");
        return;
    }
    FT_Face face;
    if (FT_New_Face(ft, fontsFile.c_str(), 0, &face))
    {
        fprintf(stderr, "Failed to load font from %s\n", fontsFile.c_str());
        return;
    }
    // Set the pixel size based.  Note if the pixel width is 0 then it will be
    // dynamically set based on the pixel height.
    FT_Set_Pixel_Sizes(face, pixelWidth, pixelHeight); //0, 48);
    // Disable the byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Failed to load Glyph\n");
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {texture, 
                               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               static_cast<GLuint> (face->glyph->advance.x)
                              };
        characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

//std::map<GLchar, Character> Characters;

struct TimeSeries
{
    /// Destructor
    ~TimeSeries()
    {
        clear();
    }
    /// Clears the buffer
    void clear() noexcept
    {
        freeBuffers();
        mTimeSeries.clear();
    }
    /// Sets the seismogram
    void setSeismogram(const int npts, const double x[])
    {
        // Release the old OpenGL buffers
        clear();
        // Set space
        mTimeSeries.resize(npts);
        GLfloat *dataPtr = mTimeSeries.data();
        float maxAbs = 0;
        #pragma omp simd reduction(max:maxAbs)
        for (auto i=0; i<npts; ++i)
        {
            dataPtr[i] = static_cast<GLfloat> (x[i]);
            maxAbs = std::max(maxAbs, std::abs(dataPtr[i]));
        }
        // Normalize and flip
        if (maxAbs > 0)
        {
            #pragma omp simd
            for (auto i=0; i<npts; ++i)
            {
                dataPtr[i] =-dataPtr[i]/maxAbs;
            }
        }
    }
    /// Binds the seismogram to the OpenGL buffers
    void bindSeismogramToGLBuffers(const int waveformIndex,
                                   const int nWaveforms)
    {
        // Figure out the y shift.  There are going to be nWaveforms cells
        // so the waveformIndex'th waveform should be offset by dy + dy/2.
        auto dy = 2.0f/static_cast<float> (nWaveforms); 
        auto dy2 = dy/2.0f; // Only plot half
        auto y0 = -1.0f + static_cast<float> (waveformIndex)*dy + dy/2;
 
        freeBuffers();
        // Create and populate the buffer objects
        glGenBuffers(1, &mCoord2DVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mCoord2DVBO);
        // Copy time series to GPU memory
        int len = static_cast<int> (mTimeSeries.size());
        //std::vector<point> graph(len); //std::array<point, 2000> graph;
        //for (auto i=0; i<len; ++i)
        //{
        //    float x = 2.0f*static_cast<float> (i)/static_cast<float> (len-1) - 1.0f; //100.0;
        //    graph[i].x = x;
        //    graph[i].y = 0.95*mTimeSeries[i];
        //}
        //glBufferData(GL_ARRAY_BUFFER, len*sizeof(point), graph.data(), GL_STATIC_DRAW);
        //checkGlError("glBufferData");
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //checkGlError("glBufferClose")
        // Map to GPU memory
        glBufferData(GL_ARRAY_BUFFER, len*sizeof(point), NULL, GL_STATIC_DRAW);
        auto graphPtr = reinterpret_cast<point *> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        for (auto i=0; i<len; ++i)
        {
            float x = 2.0f*static_cast<float> (i)/static_cast<float> (len-1) - 1.0f;
            graphPtr[i].x = x;
            graphPtr[i].y = y0 + dy2*mTimeSeries[i];
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        checkGlError("glUnmapBuffer");
        // Create and setup the vertex array object
        glGenVertexArrays(1, &mVAOHandle);
        glBindVertexArray(mVAOHandle);
        mMadeBuffers = true;
    }
    /// Frees the OpenGL buffers
    void freeBuffers()
    {
        if (mMadeBuffers)
        {
            glDeleteBuffers(1, &mCoord2DVBO);
            checkGlError("Delete mCoord2dVBO");
            glDeleteBuffers(1, &mVAOHandle);
            checkGlError("Delete mVAOHandle");
        }
    }
    std::vector<GLfloat> mTimeSeries;
    GLuint mVAOHandle = 0;
    GLuint mCoord2DVBO = 0;
    bool mMadeBuffers = false;
};

}

class GLWiggle::GLWiggleImpl
{
public:
    class GLSLShader mShader;
    class GLSLShader mTextShader;
    std::map<GLchar, Character> mCharacters;
    float mScaleX = 1;
    float mShiftX = 0;
    float mCenterX = 0;
    std::pair<double, double> mGLScreenXLimits{-1.0, 1.0};
    std::pair<double, double> mGLScreenYLimits{-1.0, 1.0};
    //GLuint mVBO[2] = {0, 0};
    GLuint mVAOHandle = 0;
    GLuint mCoord2DVBO = 0;
    std::string mVertexShaderFile;
    std::string mFragmentShaderFile;
    std::string mTextVertexShaderFile;
    std::string mTextFragmentShaderFile;
    std::vector<GLfloat> mAxis;
    std::vector<GLfloat> mTimeSeries;
std::vector<TimeSeries> mTS;
    GLfloat mMaxAbs;
};

GLWiggle::GLWiggle() :
    pImpl(std::make_unique<GLWiggleImpl> ())
{
    // Load the shader programs
    setVertexShaderFileName("shaders/array.vs");
    setFragmentShaderFileName("shaders/array.fs");

    setTextShaderFileNames("shaders/text.vs",
                           "shaders/text.fs");

    // Connect signals
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::KEY_PRESS_MASK);
    add_events(Gdk::SCROLL_MASK);
    //signal_button_press_event().connect(
    //    sigc::mem_fun(*this, &GLWiggle::onButtonPress), false);
    //signal_button_release_event().connect(
    //    sigc::mem_fun(*this, &GLWiggle::onButtonRelease), false);
    //signal_key_press_event().connect(
    //    sigc::mem_fun(*this, &GLWiggle::onKeyPress), false);
    signal_scroll_event().connect(
        sigc::mem_fun(*this, &GLWiggle::onScrollEvent), false);
/*
FT_Library ft;
if (FT_Init_FreeType(&ft))
    printf("error\n"); //std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

FT_Face face;
if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    printf("error2\n"); //std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl; 
*/
    // Hook up the signals for GLArea
    signal_realize().connect(sigc::mem_fun(*this,
                             &GLWiggle::initializeRenderer));
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    signal_unrealize().connect(sigc::mem_fun(*this,
                               &GLWiggle::destroyRenderer), false);
    // This does the actual drawing
    signal_render().connect(sigc::mem_fun(*this,
                            &GLWiggle::render));
}

GLWiggle::~GLWiggle() = default;

/// Signals
bool GLWiggle::onScrollEvent(GdkEventScroll *event)
{
    if (event->state == GDK_CONTROL_MASK)
    {
        if (event->direction == GDK_SCROLL_UP)
        {
            printf("Scroll left\n");
            panLeft();
            return true;
        }
        else if (event->direction == GDK_SCROLL_DOWN)
        {
            printf("Scroll right\n");
            panRight();
            return true;
        }
    }
    else if (event->state == GDK_SHIFT_MASK)
    {
        double xPosition = event->x;
        if (event->direction == GDK_SCROLL_LEFT)
        {
            printf("Zoom in on %lf %lf\n", event->x_root, event->y_root);
            zoom(xPosition);
            return true;
        }
        else if (event->direction == GDK_SCROLL_RIGHT)
        {
            printf("Zoom out on %lf %lf\n", event->x_root, event->y_root);
            unZoom(xPosition);
            return true;
        }
    }
    else if (event->state == GDK_MOD1_MASK)
    {
        if (event->direction == GDK_SCROLL_UP)
        {
            printf("subtract waveforms\n");
            return true;
        }
        else if (event->direction == GDK_SCROLL_DOWN)
        {
            printf("add waveforms\n");
            return true;
        }
    }
    else
    {
        if (event->direction == GDK_SCROLL_DOWN)
        {
            printf("Scroll down\n");
            return true;
        }
        else if (event->direction == GDK_SCROLL_UP)
        {
            printf("Scroll up\n");
            return true;
        }
    }
    return false;
}

/*
bool GLWiggle::onKeyPress(GdkEventKey *event)
{
printf("key press\n");
    if (event->keyval == GDK_KEY_Escape)
    {
        printf("Cancelling event\n");
        return true;
    }
    else if (event->keyval == GDK_KEY_r)
    {
        printf("Reset to center\n");
        return true;
    }
    else if (event->keyval == GDK_KEY_m)
    {
        printf("Make manual pick\n");
        return true;
    }
    return false;
}
*/

bool GLWiggle::on_button_press_event(GdkEventButton *event)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        if (event->button == 1)
        {
            printf("left clicked %lf, %lf\n", event->x, event->y);
            return true;
        }
        else if (event->button == 2)
        {
            printf("zoom\n");
            return true;
        }
        else if (event->button == 3)
        {
            printf("right clicked\n");
            //mPopupMenu.popup(event->button, event->time);
            return true;
        }
    }
    return false;
}

/// Sets the seismogram for plotting
void GLWiggle::setSeismogram(const int npts, const double x[])
{
pImpl->mTS.resize(2);
pImpl->mTS[0].setSeismogram(npts, x);
pImpl->mTS[1].setSeismogram(npts, x);
    return;

    pImpl->mTimeSeries.resize(npts);
    pImpl->mAxis.resize(npts);
    GLfloat *dataPtr = pImpl->mTimeSeries.data();
    //#pragma omp simd
    pImpl->mMaxAbs = 0;
    for (auto i=0; i<npts; ++i)
    {
        dataPtr[i] = static_cast<GLfloat> (x[i]);
        pImpl->mMaxAbs = std::max(pImpl->mMaxAbs, std::abs(dataPtr[i]));
        pImpl->mAxis[i] = i/static_cast<float> (npts);
    }
    // Normalize and flip
    for (auto i=0; i<npts; ++i)
    {
        dataPtr[i] =-dataPtr[i]/pImpl->mMaxAbs;
    }
//#ifdef USE_PSTL
//     auto minMax = std::minmax(std::execution::unseq,
//                              pImpl->mTimeSeries.begin(),
//                              pImpl->mTimeSeries.end());
//#else
//    auto minMax = std::minmax(pImpl->mTimeSeries.begin(),
//                              pImpl->mTimeSeries.end());
//#endif
//    pImpl->mMaxAbs = std::max(std::abs(*minMax.first),
//                              std::abs(*minMax.second));
}

void GLWiggle::panLeft()
{
    double shiftX = pImpl->mShiftX + 2*0.05;
    if (std::abs(shiftX) >= 1.1){return;}
    pImpl->mShiftX = shiftX;
    queue_render();         
}

void GLWiggle::panRight()
{
    double shiftX = pImpl->mShiftX - 2*0.05;
    if (std::abs(shiftX) >= 1.1){return;}
    pImpl->mShiftX = shiftX;
    queue_render();
}
/// Zoom in
void GLWiggle::zoom(const double xPosition) //onKeyPress(GdkEventKey *event)
{
    if (xPosition >= 0)
    {
        auto allocation = get_allocation();
        auto width  = allocation.get_width();
        // Put the x position into relative coordinates
        double xCenter =-1.0 + 2.0*xPosition/static_cast<double> (width);
        printf("xCenter: %lf %d %lf\n", xPosition, width, xCenter);
    }
    pImpl->mScaleX = pImpl->mScaleX*1.5;
    queue_render();
}

/// Zoom out
void GLWiggle::unZoom(const double xPosition)
{
    // Can't zoom out anymore
    if (pImpl->mScaleX <= 1.0)
    {
        return;
    }
    auto allocation = get_allocation();
    auto height = allocation.get_height();
    auto width  = allocation.get_width();
    // Clip on max zoom
    pImpl->mScaleX = std::max(1.0, pImpl->mScaleX/1.5);
    queue_render();
}

//GLWiggle::setShaders(const std::string &

void GLWiggle::setTextShaderFileNames(const std::string &textVertexShaderFile,
                                      const std::string &textFragmentShaderFile)
{
#ifdef TEMBLOR_USE_FILESYSTEM
    if (!fs::exists(textVertexShaderFile))
    {
        throw std::invalid_argument("Vertex shader = " + textVertexShaderFile
                                  + " does not exist");
    }
    if (!fs::exists(textFragmentShaderFile))
    {
        throw std::invalid_argument("Vertex shader = " + textFragmentShaderFile
                                  + " does not exist");
    }
#endif
    pImpl->mTextVertexShaderFile = textVertexShaderFile;
    pImpl->mTextFragmentShaderFile = textFragmentShaderFile;
}

void GLWiggle::setVertexShaderFileName(const std::string &fileName)
{
    pImpl->mVertexShaderFile.clear();
#ifdef TEMBLOR_USE_FILESYSTEM
    if (!fs::exists(fileName))
    {
        throw std::invalid_argument("Vertex shader = " + fileName
                                  + " does not exist");
    }
#endif
    pImpl->mVertexShaderFile = fileName;
}

void GLWiggle::setFragmentShaderFileName(const std::string &fileName)
{
    pImpl->mFragmentShaderFile.clear();
#ifdef TEMBLOR_USE_FILESYSTEM
    if (!fs::exists(fileName))
    {
        throw std::invalid_argument("Fragment shader = " + fileName
                                  + " does not exist");
    }
#endif
    pImpl->mFragmentShaderFile = fileName;
}

void GLWiggle::initializeBuffers()
{
    // Create and populate teh buffer objects
    glGenBuffers(1, &pImpl->mCoord2DVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);

    int len = pImpl->mTimeSeries.size();
    std::vector<point> graph(len); //std::array<point, 2000> graph;
    for (auto i=0; i<len; ++i)
    {
        float x = 2.0f*static_cast<float> (i)/static_cast<float> (len-1) - 1.0f; //100.0;
        graph[i].x = x;
        graph[i].y = 0.95*pImpl->mTimeSeries[i];
    }
    
    //glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, len*sizeof(point), graph.data(), GL_STATIC_DRAW);
    checkGlError("glBufferData");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkGlError("glBufferClose");
    // Create and setup the vertex array object
    glGenVertexArrays(1, &pImpl->mVAOHandle);
    glBindVertexArray(pImpl->mVAOHandle);
    // Enable the vertex attribute arrays
    //glEnableVertexAttribArray(0);
    
}

/// signal_realize: nitialize the renderer
void GLWiggle::initializeRenderer()
{
    make_current();
    if (pImpl->mVertexShaderFile.length() == 0)
    {
        fprintf(stderr, "%s: Vertex shader must be set\n", __func__);
        return;
    }  
    if (pImpl->mFragmentShaderFile.length() == 0)
    {
        fprintf(stderr, "%s: Fragment shader must be set\n", __func__);
    }
    try 
    {
        throw_if_error();
        // Fire up the seismogram shaders
        pImpl->mShader.createVertexShaderFromFile(pImpl->mVertexShaderFile);
        pImpl->mShader.createFragmentShaderFromFile(pImpl->mFragmentShaderFile);
        pImpl->mShader.makeShaderProgram();
        // And the text shaders
        pImpl->mTextShader.createVertexShaderFromFile(pImpl->mTextVertexShaderFile);
        pImpl->mTextShader.createFragmentShaderFromFile(pImpl->mTextFragmentShaderFile); 
        pImpl->mTextShader.makeShaderProgram();
        //pImpl->mTextShader.createVertexShaderFromFile( );
        //initializeBuffers();
pImpl->mTS[0].bindSeismogramToGLBuffers(0, 2);
pImpl->mTS[1].bindSeismogramToGLBuffers(1, 2);
/*
        glGenBuffers(2, pImpl->mVBO); //mVBOHandles);
        glGenVertexArrays(1, &pImpl->mVAO);
        glBindVertexArray(pImpl->mVAO);

        glGenBuffers(1, &pImpl->mCoord2DVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
*/
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "%s: An error occured making the context\n", __func__);
        fprintf(stderr, "%s: %u-%d-%s\n", __func__, gle.domain(),
                gle.code(), gle.what().c_str());
        return;
    }
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to initialize renderer %s\n", __func__, e.what());
    }
    // Add the attributes and uniforms
    try
    {
        // Waveform vertex shader
        pImpl->mShader.useProgram();
            pImpl->mShader.addAttribute("coord2d");
            checkGlError("coord2d");
            pImpl->mShader.addUniform("offset_x");
            checkGlError("offset_x");
            pImpl->mShader.addUniform("scale_x");
            checkGlError("scale_x");
            pImpl->mShader.addUniform("color");
            checkGlError("color");
        pImpl->mShader.releaseProgram();
        // Text shader
        pImpl->mTextShader.useProgram();
            pImpl->mTextShader.addAttribute("vertex"); 
            checkGlError("vertex");
            pImpl->mTextShader.addAttribute("TexCoords");
            checkGlError("TexCoords");
            pImpl->mTextShader.addUniform("projection");
            checkGlError("projection");
            pImpl->mTextShader.addUniform("text");
            checkGlError("text");
            pImpl->mTextShader.addUniform("textColor");
            checkGlError("textColor");
        pImpl->mTextShader.releaseProgram(); 
    }
    catch (const std::exception &e) 
    {
        fprintf(stderr, "%s: Failed to add attributes\n", __func__);
        fprintf(stderr, "%s\n", e.what());
    }
}

void GLWiggle::drawLinePlot(const int waveform,
                            const float xOffset,
                            const float xScale,
                            const float color[4])
{
    // Use the program
    pImpl->mShader.useProgram();
    checkGlError("glUseProgram");

    auto offsetHandle = pImpl->mShader("offset_x");
    auto scaleXHandle = pImpl->mShader("scale_x");
    auto colorHandle = pImpl->mShader("color");

    // Bind the uniform parameters of the shader
    glUniform1f(offsetHandle, xOffset); //0.0f);
    checkGlError("offset_x");
    glUniform1f(scaleXHandle,  xScale); //1.0f);//1.0f);
    checkGlError("scale_x");
    glUniform4f(colorHandle, color[0], color[1], color[2], color[3]); //0.0f, 1.0f, 0.0f, 1.0f);
    checkGlError("color");
    // Bind the appropriate buffer object
    //glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pImpl->mTS[waveform].mCoord2DVBO);
    checkGlError("bindVBO");
    // Draw the coord2d data on this VBO
    glEnableVertexAttribArray(pImpl->mShader["coord2d"]);
    checkGlError("enableCoord2d");
    glVertexAttribPointer(pImpl->mShader["coord2d"], 2,
                          GL_FLOAT, GL_FALSE, 0, nullptr);
    checkGlError("attribPointer");
    // Do the drawing 
    //int len = pImpl->mTimeSeries.size();
    int len = pImpl->mTS[waveform].mTimeSeries.size();
    glDrawArrays(GL_LINE_STRIP, 0, len);
    checkGlError("drawArrays");

    glDisableVertexAttribArray(0);
    checkGlError("disable");
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    checkGlError("close array");

    // Unuse the program
    pImpl->mShader.releaseProgram();
    checkGlError("glUnuseProgram"); 
}

/*
void GLWiggle::on_resize(const int width, const int height)
{
    make_current();
    glViewport(0, 0, width, height);
    checkGlError("glViewportResize");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    checkGlError("glClearColor");
}
*/

/// signal_render: Does the rendering
bool GLWiggle::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
    //printf("render\n");
    make_current();
    auto allocation = get_allocation();
    auto height = allocation.get_height();
    auto width  = allocation.get_width();
    float ratio = static_cast<float> (height)/static_cast<float> (width);
    ratio = static_cast<float> (width)/static_cast<float> (height);
    if (width > height)
    {
        ratio = static_cast<float> (height)/static_cast<float> (width);
    }
    //glViewport(0, 0, width, height);
    //checkGlError("glViewport");
    //glClear(GL_COLOR_BUFFER_BIT);
    //checkGlError("glClear");
    try
    {
        throw_if_error();
        glClearColor(0.98, 0.98, 0.98, 1.0);
        checkGlError("clear color");
        glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
        checkGlError("clear");

        // Prevent wrap-around
/*
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        checkGlError("Parameteri");

        float xScale = pImpl->mScaleX; //ratio;
        float xOffset = pImpl->mShiftX; //xOffset; //0;
        float color[4] = {0, 0, 0, 1}; //ratio, ratio, ratio, 1.0};
        drawLinePlot(0, xOffset, xScale, color);
        float red[4] = {1, 0, 0, 1};
        drawLinePlot(1, xOffset, xScale, red);//color);
/*
        // Bind the uniform parameters of the shader
        glUniform1f(pImpl->mShader("offset_x"), 0.0f);
        checkGlError("offset_x");
        glUniform1f(pImpl->mShader("scale_x"),  1.0f);//1.0f);
        checkGlError("scale_x");
        glUniform4f(pImpl->mShader("color"), 0.0f, 1.0f, 0.0f, 1.0f);
        checkGlError("color");
        // Bind the appropriate buffer object
        glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
        checkGlError("bindVBO");
        // Draw the coord2d data on this VBO
        glEnableVertexAttribArray(pImpl->mShader["coord2d"]);
        checkGlError("enableCoord2d");
        glVertexAttribPointer(pImpl->mShader["coord2d"], 2,
                              GL_FLOAT, GL_FALSE, 0, nullptr);
        checkGlError("attribPointer");
        // Do the drawing 
        int len = pImpl->mTimeSeries.size();
        glDrawArrays(GL_LINE_STRIP, 0, len);
        checkGlError("drawArrays");

        glDisableVertexAttribArray(0);
        checkGlError("disable");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        checkGlError("close array");
*/
/*
        glBindVertexArray(pImpl->mVAOHandle); 
        checkGlError("bindVAO");
        glBindBuffer(GL_ARRAY_BUFFER, pImpl->mCoord2DVBO);
        checkGlError("bindVBO");
*/
        
        //glDrawArrays(GL_POINTS, 0, 2000);
        
        //checkGlError("drawArrays");
        //glBindVertexArray(mVAOHandle);
        //glDrawArrays(GL_LINE_STRIP, 0, 3 );
//        pImpl->mShader.releaseProgram();
        //checkGlError("glUnuseProgram");

        glFlush();
     }
     catch(const Gdk::GLError& gle)
     {
        fprintf(stderr, "%s: An error occurred in the render callback\n",
                __func__);
        fprintf(stderr, "%s: %u-%d-%s\n",
                __func__, gle.domain(), gle.code(), gle.what().c_str());
        return false;
     }   
     return true;
}

void GLWiggle::freeBuffers()
{
    glDeleteBuffers(1, &pImpl->mCoord2DVBO);
    checkGlError("Delete mCoord2dVBO");
    glDeleteBuffers(1, &pImpl->mVAOHandle); 
    checkGlError("Delete mVAOHandle");
}
/// signal_unrealize: Deletes the renderer and frees resources
void GLWiggle::destroyRenderer()
{
    make_current();
    try
    {
        throw_if_error();
        pImpl->mShader.deleteShaderProgram();
        pImpl->mTextShader.deleteShaderProgram();
        freeBuffers();
    }
    catch (const Gdk::GLError &gle)
    {
        fprintf(stderr, "%s: An error occurred unrealizing context\n",
                __func__);
        fprintf(stderr, "%s: %u-%d-%s\n", __func__, gle.domain(),
                gle.code(), gle.what().c_str());
    } 
}
