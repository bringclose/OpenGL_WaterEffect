// if you use STL, please include this header AFTER all other include
#pragma once

#include "esUtil.h"
#include "esUtil_win.h"
#include "Math.h"
#include "TGA.h"
#include "memDbg.h"

#define SAFE_DEL(p)   {if(p){ delete p; p=0; }}
#define SAFE_DEL_ARRAY(p) {if(p){ delete []p; p=0; }}
#define SAFE_DEL_BUFFER(x) {if(x){glDeleteBuffers(1, &x); x=0;}}
#define SAFE_DEL_TEXTURE(x) {if(x){glDeleteTextures(1, &x); x=0;}}
#define SAFE_DEL_PROGRAM(x) {if(x){glDeleteProgram(x); x=0;}}
#define SAFE_DEL_SHADER(x) {if(x){glDeleteShader(x); x=0;}}