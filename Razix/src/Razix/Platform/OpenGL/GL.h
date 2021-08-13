#pragma once

#ifdef Razix_PLATFORM_MOBILE
#ifdef Razix_PLATFORM_IOS
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif Razix_PLATFORM_ANDROID
#include <GLES3/gl3.h>
#endif

#else
#ifdef Razix_PLATFORM_WINDOWS
#include <glad/glad.h>
#elif Razix_PLATFORM_LINUX
#include <glad/glad.h>
#elif Razix_PLATFORM_MACOS
#include <glad/glad.h>
#include <OpenGL/gl.h>
#endif

#endif
