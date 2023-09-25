#include <iostream>
#include "shader_program.h"
#include "gl_viewer.h"
#include "easycppogl_src/mesh.h"
#include "texturecubemap.h"
#include "gl_viewer.h"
#include "fbo.h"


#define macro_str(s) #s
#define macro_xstr(s) macro_str(s)
#define DATA_PATH std::string(macro_xstr(DEF_DATA_PATH))
#define SHADERS_PATH std::string(macro_xstr(DEF_SHADERS_PATH))
#define WIDTH 1920
#define HEIGHT 1080

using namespace EZCOGL;

// Color map for the terrain shader
GLVVec4 colorMap = { {0.0,        0.6372549,  0.88823529, 1.0},
                    {0.0,        0.64509804, 0.86470588, 1.0},
                    {0.0,        0.65294118, 0.84117647, 1.0},
                    {0.0,        0.66078431, 0.81764706, 1.0},
                    {0.0,        0.66078431, 0.81764706, 1.0},
                    {0.0,        0.66862745, 0.79411765, 1.0},
                    {0.0,        0.67647059, 0.77058824, 1.0},
                    {0.0,        0.68431373, 0.74705882, 1.0},
                    {0.0,        0.69215686, 0.72352941, 1.0},
                    {0.0,        0.7,        0.7,        1.0},
                    {0.0,        0.7,        0.7,        1.0},
                    {0.0,        0.70784314, 0.67647059, 1.0},
                    {0.0,        0.71568627, 0.65294118, 1.0},
                    {0.0,        0.72352941, 0.62941176, 1.0},
                    {0.0,        0.73137255, 0.60588235, 1.0},
                    {0.0,        0.73921569, 0.58235294, 1.0},
                    {0.0,        0.73921569, 0.58235294, 1.0},
                    {0.0,        0.74705882, 0.55882353, 1.0},
                    {0.0,        0.75490196, 0.53529412, 1.0},
                    {0.0,        0.7627451,  0.51176471, 1.0},
                    {0.0,        0.77058824, 0.48823529, 1.0},
                    {0.0,        0.77843137, 0.46470588, 1.0},
                    {0.0,        0.77843137, 0.46470588, 1.0},
                    {0.0,        0.78627451, 0.44117647, 1.0},
                    {0.0,        0.79411765, 0.41764706, 1.0},
                    {0.00392157, 0.80078431, 0.40078431, 1.0},
                    {0.01960784, 0.80392157, 0.40392157, 1.0},
                    {0.03529412, 0.80705882, 0.40705882, 1.0},
                    {0.03529412, 0.80705882, 0.40705882, 1.0},
                    {0.05098039, 0.81019608, 0.41019608, 1.0},
                    {0.06666667, 0.81333333, 0.41333333, 1.0},
                    {0.08235294, 0.81647059, 0.41647059, 1.0},
                    {0.09803922, 0.81960784, 0.41960784, 1.0},
                    {0.11372549, 0.8227451,  0.4227451,  1.0},
                    {0.11372549, 0.8227451,  0.4227451,  1.0},
                    {0.12941176, 0.82588235, 0.42588235, 1.0},
                    {0.14509804, 0.82901961, 0.42901961, 1.0},
                    {0.16078431, 0.83215686, 0.43215686, 1.0},
                    {0.17647059, 0.83529412, 0.43529412, 1.0},
                    {0.19215686, 0.83843137, 0.43843137, 1.0},
                    {0.19215686, 0.83843137, 0.43843137, 1.0},
                    {0.20784314, 0.84156863, 0.44156863, 1.0},
                    {0.22352941, 0.84470588, 0.44470588, 1.0},
                    {0.23921569, 0.84784314, 0.44784314, 1.0},
                    {0.25490196, 0.85098039, 0.45098039, 1.0},
                    {0.27058824, 0.85411765, 0.45411765, 1.0},
                    {0.28627451, 0.8572549,  0.4572549,  1.0},
                    {0.28627451, 0.8572549,  0.4572549,  1.0},
                    {0.30196078, 0.86039216, 0.46039216, 1.0},
                    {0.31764706, 0.86352941, 0.46352941, 1.0},
                    {0.33333333, 0.86666667, 0.46666667, 1.0},
                    {0.34901961, 0.86980392, 0.46980392, 1.0},
                    {0.36470588, 0.87294118, 0.47294118, 1.0},
                    {0.36470588, 0.87294118, 0.47294118, 1.0},
                    {0.38039216, 0.87607843, 0.47607843, 1.0},
                    {0.39607843, 0.87921569, 0.47921569, 1.0},
                    {0.41176471, 0.88235294, 0.48235294, 1.0},
                    {0.42745098, 0.8854902,  0.4854902,  1.0},
                    {0.44313725, 0.88862745, 0.48862745, 1.0},
                    {0.44313725, 0.88862745, 0.48862745, 1.0},
                    {0.45882353, 0.89176471, 0.49176471, 1.0},
                    {0.4745098,  0.89490196, 0.49490196, 1.0},
                    {0.49019608, 0.89803922, 0.49803922, 1.0},
                    {0.50588235, 0.90117647, 0.50117647, 1.0},
                    {0.52156863, 0.90431373, 0.50431373, 1.0},
                    {0.52156863, 0.90431373, 0.50431373, 1.0},
                    {0.5372549,  0.90745098, 0.50745098, 1.0},
                    {0.55294118, 0.91058824, 0.51058824, 1.0},
                    {0.56862745, 0.91372549, 0.51372549, 1.0},
                    {0.58431373, 0.91686275, 0.51686275, 1.0},
                    {0.6,        0.92,       0.52,       1.0},
                    {0.6,        0.92,       0.52,       1.0},
                    {0.61568627, 0.92313725, 0.52313725, 1.0},
                    {0.63137255, 0.92627451, 0.52627451, 1.0},
                    {0.64705882, 0.92941176, 0.52941176, 1.0},
                    {0.6627451,  0.93254902, 0.53254902, 1.0},
                    {0.67843137, 0.93568627, 0.53568627, 1.0},
                    {0.67843137, 0.93568627, 0.53568627, 1.0},
                    {0.69411765, 0.93882353, 0.53882353, 1.0},
                    {0.70980392, 0.94196078, 0.54196078, 1.0},
                    {0.7254902,  0.94509804, 0.54509804, 1.0},
                    {0.74117647, 0.94823529, 0.54823529, 1.0},
                    {0.75686275, 0.95137255, 0.55137255, 1.0},
                    {0.75686275, 0.95137255, 0.55137255, 1.0},
                    {0.77254902, 0.9545098,  0.5545098,  1.0},
                    {0.78823529, 0.95764706, 0.55764706, 1.0},
                    {0.80392157, 0.96078431, 0.56078431, 1.0},
                    {0.81960784, 0.96392157, 0.56392157, 1.0},
                    {0.83529412, 0.96705882, 0.56705882, 1.0},
                    {0.83529412, 0.96705882, 0.56705882, 1.0},
                    {0.85098039, 0.97019608, 0.57019608, 1.0},
                    {0.86666667, 0.97333333, 0.57333333, 1.0},
                    {0.88235294, 0.97647059, 0.57647059, 1.0},
                    {0.89803922, 0.97960784, 0.57960784, 1.0},
                    {0.91372549, 0.9827451,  0.5827451,  1.0},
                    {0.91372549, 0.9827451,  0.5827451,  1.0},
                    {0.92941176, 0.98588235, 0.58588235, 1.0},
                    {0.94509804, 0.98901961, 0.58901961, 1.0},
                    {0.96078431, 0.99215686, 0.59215686, 1.0},
                    {0.97647059, 0.99529412, 0.59529412, 1.0},
                    {0.99215686, 0.99843137, 0.59843137, 1.0},
                    {0.99607843, 0.99498039, 0.59788235, 1.0},
                    {0.99607843, 0.99498039, 0.59788235, 1.0},
                    {0.98823529, 0.98494118, 0.59364706, 1.0},
                    {0.98039216, 0.97490196, 0.58941176, 1.0},
                    {0.97254902, 0.96486275, 0.58517647, 1.0},
                    {0.96470588, 0.95482353, 0.58094118, 1.0},
                    {0.95686275, 0.94478431, 0.57670588, 1.0},
                    {0.95686275, 0.94478431, 0.57670588, 1.0},
                    {0.94901961, 0.9347451,  0.57247059, 1.0},
                    {0.94117647, 0.92470588, 0.56823529, 1.0},
                    {0.93333333, 0.91466667, 0.564,      1.0},
                    {0.9254902,  0.90462745, 0.55976471, 1.0},
                    {0.91764706, 0.89458824, 0.55552941, 1.0},
                    {0.91764706, 0.89458824, 0.55552941, 1.0},
                    {0.90980392, 0.88454902, 0.55129412, 1.0},
                    {0.90196078, 0.8745098,  0.54705882, 1.0},
                    {0.89411765, 0.86447059, 0.54282353, 1.0},
                    {0.88627451, 0.85443137, 0.53858824, 1.0},
                    {0.87843137, 0.84439216, 0.53435294, 1.0},
                    {0.87843137, 0.84439216, 0.53435294, 1.0},
                    {0.87058824, 0.83435294, 0.53011765, 1.0},
                    {0.8627451,  0.82431373, 0.52588235, 1.0},
                    {0.85490196, 0.81427451, 0.52164706, 1.0},
                    {0.84705882, 0.80423529, 0.51741176, 1.0},
                    {0.83921569, 0.79419608, 0.51317647, 1.0},
                    {0.83921569, 0.79419608, 0.51317647, 1.0},
                    {0.83137255, 0.78415686, 0.50894118, 1.0},
                    {0.82352941, 0.77411765, 0.50470588, 1.0},
                    {0.81568627, 0.76407843, 0.50047059, 1.0},
                    {0.80784314, 0.75403922, 0.49623529, 1.0},
                    {0.8,        0.744,      0.492,      1.0},
                    {0.8,        0.744,      0.492,      1.0},
                    {0.79215686, 0.73396078, 0.48776471, 1.0},
                    {0.78431373, 0.72392157, 0.48352941, 1.0},
                    {0.77647059, 0.71388235, 0.47929412, 1.0},
                    {0.76862745, 0.70384314, 0.47505882, 1.0},
                    {0.76078431, 0.69380392, 0.47082353, 1.0},
                    {0.76078431, 0.69380392, 0.47082353, 1.0},
                    {0.75294118, 0.68376471, 0.46658824, 1.0},
                    {0.74509804, 0.67372549, 0.46235294, 1.0},
                    {0.7372549,  0.66368627, 0.45811765, 1.0},
                    {0.72941176, 0.65364706, 0.45388235, 1.0},
                    {0.72156863, 0.64360784, 0.44964706, 1.0},
                    {0.72156863, 0.64360784, 0.44964706, 1.0},
                    {0.71372549, 0.63356863, 0.44541176, 1.0},
                    {0.70588235, 0.62352941, 0.44117647, 1.0},
                    {0.69803922, 0.6134902,  0.43694118, 1.0},
                    {0.69019608, 0.60345098, 0.43270588, 1.0},
                    {0.68235294, 0.59341176, 0.42847059, 1.0},
                    {0.6745098,  0.58337255, 0.42423529, 1.0},
                    {0.6745098,  0.58337255, 0.42423529, 1.0},
                    {0.66666667, 0.57333333, 0.42,       1.0},
                    {0.65882353, 0.56329412, 0.41576471, 1.0},
                    {0.65098039, 0.5532549,  0.41152941, 1.0},
                    {0.64313725, 0.54321569, 0.40729412, 1.0},
                    {0.63529412, 0.53317647, 0.40305882, 1.0},
                    {0.63529412, 0.53317647, 0.40305882, 1.0},
                    {0.62745098, 0.52313725, 0.39882353, 1.0},
                    {0.61960784, 0.51309804, 0.39458824, 1.0},
                    {0.61176471, 0.50305882, 0.39035294, 1.0},
                    {0.60392157, 0.49301961, 0.38611765, 1.0},
                    {0.59607843, 0.48298039, 0.38188235, 1.0},
                    {0.59607843, 0.48298039, 0.38188235, 1.0},
                    {0.58823529, 0.47294118, 0.37764706, 1.0},
                    {0.58039216, 0.46290196, 0.37341176, 1.0},
                    {0.57254902, 0.45286275, 0.36917647, 1.0},
                    {0.56470588, 0.44282353, 0.36494118, 1.0},
                    {0.55686275, 0.43278431, 0.36070588, 1.0},
                    {0.55686275, 0.43278431, 0.36070588, 1.0},
                    {0.54901961, 0.4227451,  0.35647059, 1.0},
                    {0.54117647, 0.41270588, 0.35223529, 1.0},
                    {0.53333333, 0.40266667, 0.348,      1.0},
                    {0.5254902,  0.39262745, 0.34376471, 1.0},
                    {0.51764706, 0.38258824, 0.33952941, 1.0},
                    {0.51764706, 0.38258824, 0.33952941, 1.0},
                    {0.50980392, 0.37254902, 0.33529412, 1.0},
                    {0.50196078, 0.3625098,  0.33105882, 1.0},
                    {0.50588235, 0.36752941, 0.33788235, 1.0},
                    {0.51372549, 0.37756863, 0.34839216, 1.0},
                    {0.52156863, 0.38760784, 0.35890196, 1.0},
                    {0.52156863, 0.38760784, 0.35890196, 1.0},
                    {0.52941176, 0.39764706, 0.36941176, 1.0},
                    {0.5372549,  0.40768627, 0.37992157, 1.0},
                    {0.54509804, 0.41772549, 0.39043137, 1.0},
                    {0.55294118, 0.42776471, 0.40094118, 1.0},
                    {0.56078431, 0.43780392, 0.41145098, 1.0},
                    {0.56078431, 0.43780392, 0.41145098, 1.0},
                    {0.56862745, 0.44784314, 0.42196078, 1.0},
                    {0.57647059, 0.45788235, 0.43247059, 1.0},
                    {0.58431373, 0.46792157, 0.44298039, 1.0},
                    {0.59215686, 0.47796078, 0.4534902,  1.0},
                    {0.6,        0.488,      0.464,      1.0},
                    {0.6,        0.488,      0.464,      1.0},
                    {0.60784314, 0.49803922, 0.4745098,  1.0},
                    {0.61568627, 0.50807843, 0.48501961, 1.0},
                    {0.62352941, 0.51811765, 0.49552941, 1.0},
                    {0.63137255, 0.52815686, 0.50603922, 1.0},
                    {0.63921569, 0.53819608, 0.51654902, 1.0},
                    {0.63921569, 0.53819608, 0.51654902, 1.0},
                    {0.64705882, 0.54823529, 0.52705882, 1.0},
                    {0.65490196, 0.55827451, 0.53756863, 1.0},
                    {0.6627451,  0.56831373, 0.54807843, 1.0},
                    {0.67058824, 0.57835294, 0.55858824, 1.0},
                    {0.67843137, 0.58839216, 0.56909804, 1.0},
                    {0.68627451, 0.59843137, 0.57960784, 1.0},
                    {0.68627451, 0.59843137, 0.57960784, 1.0},
                    {0.69411765, 0.60847059, 0.59011765, 1.0},
                    {0.70196078, 0.6185098,  0.60062745, 1.0},
                    {0.70980392, 0.62854902, 0.61113725, 1.0},
                    {0.71764706, 0.63858824, 0.62164706, 1.0},
                    {0.7254902,  0.64862745, 0.63215686, 1.0},
                    {0.7254902,  0.64862745, 0.63215686, 1.0},
                    {0.73333333, 0.65866667, 0.64266667, 1.0},
                    {0.74117647, 0.66870588, 0.65317647, 1.0},
                    {0.74901961, 0.6787451,  0.66368627, 1.0},
                    {0.75686275, 0.68878431, 0.67419608, 1.0},
                    {0.76470588, 0.69882353, 0.68470588, 1.0},
                    {0.76470588, 0.69882353, 0.68470588, 1.0},
                    {0.77254902, 0.70886275, 0.69521569, 1.0},
                    {0.78039216, 0.71890196, 0.70572549, 1.0},
                    {0.78823529, 0.72894118, 0.71623529, 1.0},
                    {0.79607843, 0.73898039, 0.7267451,  1.0},
                    {0.80392157, 0.74901961, 0.7372549,  1.0},
                    {0.80392157, 0.74901961, 0.7372549,  1.0},
                    {0.81176471, 0.75905882, 0.74776471, 1.0},
                    {0.81960784, 0.76909804, 0.75827451, 1.0},
                    {0.82745098, 0.77913725, 0.76878431, 1.0},
                    {0.83529412, 0.78917647, 0.77929412, 1.0},
                    {0.84313725, 0.79921569, 0.78980392, 1.0},
                    {0.84313725, 0.79921569, 0.78980392, 1.0},
                    {0.85098039, 0.8092549,  0.80031373, 1.0},
                    {0.85882353, 0.81929412, 0.81082353, 1.0},
                    {0.86666667, 0.82933333, 0.82133333, 1.0},
                    {0.8745098,  0.83937255, 0.83184314, 1.0},
                    {0.88235294, 0.84941176, 0.84235294, 1.0},
                    {0.88235294, 0.84941176, 0.84235294, 1.0},
                    {0.89019608, 0.85945098, 0.85286275, 1.0},
                    {0.89803922, 0.8694902,  0.86337255, 1.0},
                    {0.90588235, 0.87952941, 0.87388235, 1.0},
                    {0.91372549, 0.88956863, 0.88439216, 1.0},
                    {0.92156863, 0.89960784, 0.89490196, 1.0},
                    {0.92156863, 0.89960784, 0.89490196, 1.0},
                    {0.92941176, 0.90964706, 0.90541176, 1.0},
                    {0.9372549,  0.91968627, 0.91592157, 1.0},
                    {0.94509804, 0.92972549, 0.92643137, 1.0},
                    {0.95294118, 0.93976471, 0.93694118, 1.0},
                    {0.96078431, 0.94980392, 0.94745098, 1.0},
                    {0.96078431, 0.94980392, 0.94745098, 1.0},
                    {0.96862745, 0.95984314, 0.95796078, 1.0},
                    {0.97647059, 0.96988235, 0.96847059, 1.0},
                    {0.98431373, 0.97992157, 0.97898039, 1.0},
                    {0.99215686, 0.98996078, 0.9894902,  1.0},
                    {1.0,        1.0,        1.0,        1.0},
                    {1.0,        1.0,        1.0,        1.0}
};


class SceneManager
{
public:

    GLVec3 scale = GLVec3(1, 1, 1);
    GLVec3 sun_pos = GLVec3(100, 100, 200);
    
    float water_level = 0.2;
    float water_tiling = 5.0;
    float water_strength = 0.015;
    int clipping_plane_orientation; // if equal 1, clipping plane oriented towards sky. If equal 2, clipping plane oriented towards ground

    float rotationX = -90.f; // In this project, all objects in the scene are rotated by -90° so that when executing the program, the terrain is seen from the side (and not from above)
    float rotationY = 0.f;
    float rotationZ = 0.f;

    float ground_z_scale = 1.f;

    // 4 noise octave
    float noise_freq_1 = 1.f;
    float noise_freq_2 = 2.f;
    float noise_freq_3 = 4.f;
    float noise_freq_4 = 8.f;
    
    float noise_freq = 1.f; // One global noise frequency

    float exponent = 1.f; // Redistribution factor
    unsigned int linear_reshaping_mode = 0;

    float linear_reshaping_ratio = 0.5f; // Square bump linear reshaping ratio (allows to control if the terrain looks more like a single island or more like an archipelago)
};


class Viewer : public GLViewer
{
    SceneManager sceneManager; // Contains all the parameters of the GUI and other variable useful for the whole scene

    // 3 Shaders
    ShaderProgram::UP shaderTerrain;
    ShaderProgram::UP shaderWater;
    ShaderProgram::UP shaderEnvMap;

    // 3 Mesh
    MeshRenderer::UP terrain_rend;
    MeshRenderer::UP water_rend;
    MeshRenderer::UP sky_rend;

    FBO_Depth::SP fbo_reflect; // FBO for reflection
    FBO_Depth::SP fbo_refract; // FBO for refraction

    TextureCubeMap::SP tex_envMap; // Sky box texture
    Texture2D::SP tex_distortionMap; // Water distortion map
    Texture2D::SP tex_normalMap; // Water normal map
    Texture2D::SP texHeightmap;

    float fboTexReflectWidth = 1024; // Reflection texture width
    float fboTexReflectHeight = 1024; // Reflection texture height

    float fboTexRefractWidth = 1024; // Refraction texture width
    float fboTexRefractHeight = 1024; // Refraction texture height

    unsigned int terrainSize = 300; // Number of vertices of the ground plane (less than 300 shows poor lighting result if the low noise octaves are set to high values)

public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;
    void resize_ogl(int32_t w, int32_t h) override;

private :
    // One method for each mesh
    void draw_envmap(const GLMat4& proj, const GLMat4& view);
    void draw_terrain(const GLMat4& proj, const GLMat4& view, const GLMat4 trans);
    void draw_water(const GLMat4& proj, const GLMat4& view, const GLMat4 trans);
};

int main(int, char**)
{
    Viewer v;
    float window_ratio = 1.f;
    v.set_size(WIDTH * window_ratio, HEIGHT * window_ratio); // Set the size of the window
    return v.launch3d();
}

Viewer::Viewer() {}

void Viewer::resize_ogl(int32_t w, int32_t h)
{
    fbo_reflect->resize(w, h);
    fbo_refract->resize(w, h);
}

void Viewer::init_ogl()
{

    // FBO Reflection
    auto tex_reflection = Texture2D::create({ GL_NEAREST, GL_CLAMP_TO_EDGE });
    tex_reflection->init(GL_RGB8);
    fbo_reflect = FBO_Depth::create({ tex_reflection });

    // FBO Refraction
    auto tex_refraction = Texture2D::create({ GL_NEAREST, GL_CLAMP_TO_EDGE });
    tex_refraction->init(GL_RGB8);
    fbo_refract = FBO_Depth::create({ tex_refraction });

    // SHADERS
    shaderTerrain = ShaderProgram::create({ {GL_VERTEX_SHADER, load_src(SHADERS_PATH + "/terrain.vs")}, {GL_FRAGMENT_SHADER, load_src(SHADERS_PATH + "/terrain.fs")} }, "Terrain Shader");
    shaderWater = ShaderProgram::create({ {GL_VERTEX_SHADER, load_src(SHADERS_PATH + "/water.vs")}, {GL_FRAGMENT_SHADER, load_src(SHADERS_PATH + "/water.fs")} }, "Water Shader");
    shaderEnvMap = ShaderProgram::create({ {GL_VERTEX_SHADER, load_src(SHADERS_PATH + "/sky.vs")}, {GL_FRAGMENT_SHADER, load_src(SHADERS_PATH + "/sky.fs")} }, "Sky Shader");

    // TEXTURES
    texHeightmap = Texture2D::create({ GL_NEAREST, GL_CLAMP_TO_EDGE });
    texHeightmap->load({ DATA_PATH + "/skull.jfif"});

    // TERRAIN
    auto meshTerrain = Mesh::Grid(terrainSize, terrainSize);
    terrain_rend = meshTerrain->renderer(1, -1, 2, -1, -1);
    
    // WATER
    tex_distortionMap = Texture2D::create({ GL_NEAREST, GL_REPEAT }); // Distortion map
    tex_distortionMap->load({ DATA_PATH + "/distortion_map.png"});
    tex_normalMap = Texture2D::create({ GL_NEAREST, GL_REPEAT }); // Normal map
    tex_normalMap->load({ DATA_PATH + "/normal_map.png" });
    auto meshWater = Mesh::Grid();
    water_rend = meshWater->renderer(1, -1, 2, -1, -1);

    // ENV MAP
    auto meshSky = Mesh::CubePosOnly();
    sky_rend = meshSky->renderer(1, -1, -1, -1, -1);

    tex_envMap = TextureCubeMap::create();
    tex_envMap->load({DATA_PATH + "/skybox/skybox1/right.bmp", DATA_PATH + "/skybox/skybox1/left.bmp",
        DATA_PATH + "/skybox/skybox1/top.bmp", DATA_PATH + "/skybox/skybox1/bottom.bmp",
        DATA_PATH + "/skybox/skybox1/front.bmp", DATA_PATH + "/skybox/skybox1/back.bmp"
    });

    // CAMERA
    set_scene_center(meshTerrain->BB()->center());
    set_scene_radius(meshTerrain->BB()->radius() * 3);

    // Define the color to use when refreshing screen
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void Viewer::draw_envmap(const GLMat4& proj, const GLMat4& view)
{
    glDisable(GL_DEPTH_TEST); // No depth test for the env map

    // ENV MAP
    shaderEnvMap->bind();

    GLMat4 vi = view; // New view matrix especially for the env map (Translating or scaling the camera shouldn't translate or scale the env map)
    vi.block<3, 1>(0, 3).setZero(); // remove translation
    vi.block<3, 1>(0, 0).normalize();
    vi.block<3, 1>(0, 1).normalize(); // Remove scale
    vi.block<3, 1>(0, 2).normalize();

    set_uniform_value(1, proj);
    set_uniform_value(2, vi);
    set_uniform_value(11, tex_envMap->bind(0));
    sky_rend->draw(GL_TRIANGLES);
}

void Viewer::draw_terrain(const GLMat4& proj, const GLMat4& view, const GLMat4 trans)
{
    // TERRAIN
    glEnable(GL_DEPTH_TEST);
    shaderTerrain->bind();

    set_uniform_value(0, terrainSize);
    set_uniform_value(1, proj);
    set_uniform_value(2, view);
    set_uniform_value(3, trans);
    // Construct a MV matrix for normals
    const GLMat3& mvNormalMatrix = Transfo::inverse_transpose(view * trans);
    set_uniform_value(14, mvNormalMatrix);


    GLVec4 water_plane = GLVec4(0, sceneManager.clipping_plane_orientation, 0, -sceneManager.water_level * sceneManager.clipping_plane_orientation); // Define the water plane position and orientation for clipping (needed by Reflection and Refraction)
    set_uniform_value(4, water_plane);

    set_uniform_value(6, sceneManager.noise_freq_1);
    set_uniform_value(7, sceneManager.noise_freq_2);
    set_uniform_value(8, sceneManager.noise_freq_3);
    set_uniform_value(13, sceneManager.noise_freq_4);

    set_uniform_value(9, sceneManager.noise_freq);
    set_uniform_value(10, sceneManager.exponent);
    set_uniform_value(11, sceneManager.linear_reshaping_ratio);
    set_uniform_value(5, sceneManager.linear_reshaping_mode);
    set_uniform_value(15, texHeightmap->bind(0));

    set_uniform_value(12, Transfo::sub33(view) * Transfo::sub33(trans) * sceneManager.sun_pos); // Send light position in view space
    set_uniform_value(16, colorMap);

    terrain_rend->draw(GL_TRIANGLES);
}


void Viewer::draw_water(const GLMat4& proj, const GLMat4& view, const GLMat4 trans)
{
    shaderWater->bind();

    set_uniform_value(1, proj);
    set_uniform_value(2, view);
    set_uniform_value(3, trans);

    set_uniform_value(6, sceneManager.water_level);
    set_uniform_value(7, sceneManager.water_tiling);
    set_uniform_value(8, view.inverse() * GLVec4(0.f, 0.f, 0.f, 1.f)); // Send the position of the camera in world space
    set_uniform_value(9, Transfo::sub33(trans) * sceneManager.sun_pos); // Send the light position in world space

    // Send the FBO textures as uniform
    set_uniform_value(11, fbo_reflect->texture(0)->bind(0));
    set_uniform_value(12, fbo_refract->texture(0)->bind(1));

    set_uniform_value(13, tex_distortionMap->bind(2));
    set_uniform_value(14, sceneManager.water_strength);
    set_uniform_value(15, current_time());
    set_uniform_value(16, tex_normalMap->bind(3));

    // Construct a MV matrix for normals
    const GLMat3& mvNormalMatrix = Transfo::inverse_transpose(view * trans);
    set_uniform_value(17, mvNormalMatrix);

    water_rend->draw(GL_TRIANGLES);
}


void Viewer::draw_ogl()
{
    const GLMat4& proj = this->get_projection_matrix();
    const GLMat4& view = this->get_view_matrix();


    GLMat4 ms = Transfo::scale(sceneManager.scale);
    GLMat4 mb_x = Transfo::rotate(sceneManager.rotationX, GLVec3(1, 0, 0));
    GLMat4 mb_y = Transfo::rotate(sceneManager.rotationY, GLVec3(0, 1, 0));
    GLMat4 mb_z = Transfo::rotate(sceneManager.rotationZ, GLVec3(0, 0, 1));
    const GLMat4 trans = mb_z * mb_y * mb_x * ms; // "World" transformation matrix (applied to all mesh in the scene, except the sky box)

    GLMat4 ground_scale = Transfo::scale(GLVec3(1, 1, sceneManager.ground_z_scale)); // Scale matrix for the ground only
    
    // ***********************************
    // FBO (Pass 1 & 2)
    // ***********************************
    FBO::push();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0); // Enable cliping plane

    // ***********************************
    // 1st Pass : Reflection
    // ***********************************
    fbo_reflect->bind();
    // Clear the GL "color" and "depth" framebuffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // CAMERA MODIFICATIONS
    GLVec4 cameraPosW = view.inverse() * GLVec4(0.f, 0.f, 0.f, 1.f);
    float dir = (cameraPosW(1) - sceneManager.water_level) >= 0 ? -1 : 1; // Indicate if we are under or above water (allows reflection & refraction from underwater)
    GLMat4 reflection_translation{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, dir, 0.0f, 2.0f * sceneManager.water_level},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f }
    };
    GLMat4 reflection_cam = view * reflection_translation;

    // ENV MAP
    draw_envmap(proj, reflection_cam);

    // TERRAIN
    sceneManager.clipping_plane_orientation = -dir;
    draw_terrain(proj, reflection_cam, trans * ground_scale);

    // ***********************************
    // 2nd Pass : Refraction
    // ***********************************
    fbo_refract->bind();
    // Clear the GL "color" and "depth" framebuffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TERRAIN
    sceneManager.clipping_plane_orientation = dir;
    draw_terrain(proj, view, trans * ground_scale);

    glDisable(GL_CLIP_DISTANCE0); // Disable cliping plane
    FBO::pop();


    // ***********************************
    // 3rd Pass : Rendering
    // ***********************************
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, width(), height());

	// Clear the buffer before to draw the next frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // By default we enable the depth test

    // ***********************************
    // Rendering
    // ***********************************
    // ENV MAP
    draw_envmap(proj, view);

    // TERRAIN
    draw_terrain(proj, view, trans * ground_scale);

    // WATER
    draw_water(proj, view, trans);
}

void Viewer::interface_ogl()
{
    //// DEBUG REFLECTION
    //ImGui::Begin("Reflection", nullptr, ImGuiWindowFlags_NoSavedSettings);
    //ImGui::SetWindowSize({ 0,0 });
    //ImGui::Image((void*)(intptr_t) fbo_reflect->id(), ImVec2(fboTexReflectWidth/2, fboTexReflectHeight/2));
    //ImGui::End();

    //// DEBUG REFRACTION
    //ImGui::Begin("Refraction", nullptr, ImGuiWindowFlags_NoSavedSettings);
    //ImGui::SetWindowSize({ 0,0 });
    //ImGui::Image((void*)(intptr_t)fbo_refract->id(), ImVec2(fboTexRefractWidth / 2, fboTexRefractHeight / 2));
    //ImGui::End();

    // OTHER DEBUG
    //ImGui::Begin("Debug Window", nullptr, ImGuiWindowFlags_NoSavedSettings);
    //ImGui::SetWindowSize({ 0,0 });
    //ImGui::Image((void*)(intptr_t)tex_distortionMap->id(), ImVec2(tex_distortionMap->width(), tex_distortionMap->height()));
    //ImGui::End();

    ImGui::GetIO().FontGlobalScale = 2.0f;
    ImGui::Begin("TP3", nullptr, ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowSize({ 0,0 });
    ImGui::Text("FPS :(%2.2lf)", fps_);

    // TERRAIN
    ImGui::TextColored(ImVec4(0, 1, 0.3, 1), "Terrain Noise Parameters");
    ImGui::SliderFloat("Noise Frequency 1", &sceneManager.noise_freq_1, 0.f, 10.f);
    ImGui::SliderFloat("Noise Frequency 2", &sceneManager.noise_freq_2, 0.f, 5.f);
    ImGui::SliderFloat("Noise Frequency 3", &sceneManager.noise_freq_3, 0.f, 10.f);
    ImGui::SliderFloat("Noise Frequency 4", &sceneManager.noise_freq_4, 0.f, 20.f);
    ImGui::SliderFloat("Overall Noise Frequency", &sceneManager.noise_freq, 0.f, 5.f);

    ImGui::TextColored(ImVec4(0.4, 1, 0.5, 1), "Terrain Reshaping Parameters");
    ImGui::SliderFloat("Elevation exponent", &sceneManager.exponent, 0.f, 10.f);
    ImGui::SliderFloat("Linear reshaping ratio", &sceneManager.linear_reshaping_ratio, 0.f, 1.f);
    ImGui::SliderFloat("Ground Z Scale", &sceneManager.ground_z_scale, 0.f, 2.f);

    const char* items[] = { "Square Bump", "Euclidian Squared", "Star", "Diagonal", "Manhattan", "Hyperboloid", "Texture" };
    static const char* current_item = NULL;
    if (ImGui::BeginCombo("Linear Reshaping", current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(items[n], is_selected))
                sceneManager.linear_reshaping_mode = n;
                current_item = items[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    // SUN
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sun Position");
    ImGui::SliderFloat("Sun pos x", &sceneManager.sun_pos(0), -200.f, 200.f);
    ImGui::SliderFloat("Sun pos y", &sceneManager.sun_pos(1), -200.f, 200.f); 
    ImGui::SliderFloat("Sun pos z", &sceneManager.sun_pos(2), -200.f, 200.f); // The sun Z position shouldn't be modified

    // WATER
    ImGui::TextColored(ImVec4(0, 0.3, 1, 1), "Water Parameters");
    ImGui::SliderFloat("Water tiling", &sceneManager.water_tiling, 0.f, 10.f);
    ImGui::SliderFloat("Water level", &sceneManager.water_level, -0.5f, 1.f);
    ImGui::SliderFloat("Wave strength", &sceneManager.water_strength, 0.f, 0.05f);

    //ImGui::SliderFloat("Scale X", &sceneManager.scale[0], 0.f, 1.f);
    //ImGui::SliderFloat("Scale Y", &sceneManager.scale[1], 0.f, 1.f);
    //ImGui::SliderFloat("Scale Z", &sceneManager.scale[2], 0.f, 2.f);

    //ImGui::SliderFloat("Rotate X", &sceneManager.rotationX, -360.f, 360.f);
    //ImGui::SliderFloat("Rotate Y", &sceneManager.rotationY, -360.f, 360.f);
    //ImGui::SliderFloat("Rotate Z", &sceneManager.rotationZ, -360.f, 360.f);

    //const GLMat4& view = this->get_view_matrix();
    //ImGui::Text("Cam�ra x position : %f", view(0, 3));
    //ImGui::Text("Cam�ra y position : %f", view(1, 3));
    //ImGui::Text("Cam�ra z position : %f", view(2, 3));

    //ImGui::Text("Cam�ra (0, 0) : %f", view(0, 0));
    //ImGui::Text("Cam�ra (1, 0) : %f", view(1, 0));
    //ImGui::Text("Cam�ra (2, 0) : %f", view(2, 0));
    //ImGui::Text("Cam�ra (3, 0) : %f", view(3, 0));

    //ImGui::Text("Cam�ra (0, 1) : %f", view(0, 1));
    //ImGui::Text("Cam�ra (1, 1) : %f", view(1, 1));
    //ImGui::Text("Cam�ra (2, 1) : %f", view(2, 1));
    //ImGui::Text("Cam�ra (3, 1) : %f", view(3, 1));

    //ImGui::Text("Cam�ra (0, 2) : %f", view(0, 2));
    //ImGui::Text("Cam�ra (1, 2) : %f", view(1, 2));
    //ImGui::Text("Cam�ra (2, 2) : %f", view(2, 2));
    //ImGui::Text("Cam�ra (3, 2) : %f", view(3, 2));

    //ImGui::Text("Cam�ra (0, 3) : %f", view(0, 3));
    //ImGui::Text("Cam�ra (1, 3) : %f", view(1, 3));
    //ImGui::Text("Cam�ra (2, 3) : %f", view(2, 3));
    //ImGui::Text("Cam�ra (3, 3) : %f", view(3, 3));

    ImGui::End();
}
